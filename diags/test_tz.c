/*
*=======================================================================
*   Copyright (C) 2011 Mindspeed Technologies, Inc.
*
*        Author   : Calvin Johnson
*        Filename : test_tz.c
*        Purpose  : Validate TrustZone block for c2k devices.
*        Reference: Bug#62347. NFD describes tests.
*======================================================================
*/

#include <types.h>
#include <common.h>
#include <asm/io.h>
#include "tests.h"
#include "dump.h"
#include "diags.h"
#include <mach/clkcore.h>
#include "tz/tz.h"

static const struct slave ft_slv[MAX_TZ_FT_SLAVES] =
{
	{0, 0x94000000}, {1, 0x95000000},   /* DPI0 cfg, DPI1 cfg          */
	{2, 0x9E000000}, {3, 0x97000000},   /* DECT cfg, DDR cfg           */
	{4, 0x93000000}, {5, 0x96000000},   /* TrustZone cfg, UART_SPI cfg */
	{6, 0x9C200000}, {7, 0x9B000000},   /* HFE cfg, SPAcc cfg          */
	{8, 0x9A000000}, {9, 0x98000000},   /* EspAh cfg, PCIe0 cfg        */
	{10, 0x99000000}, {11, 0x9D000000}, /* PCIe1 cfg, SATA cfg         */
	{12, 0x9F000000}, /* {13, 0x80000000}, *//* USB3 cfg, ACP          */
/*	{17, 0xA0000000}, {18, 0xB0000000}*/  /* PCIe0, PCIe1              */
/* Note:- ACP slave will not be tested by A9 as it cannot be accessed. This will be
 * tested by other masters later. */
};

static const struct pt_slave pt_slv[MAX_TZ_PT_SLAVES] =
{
	{14, 0x00000000, 0x2, 0x1},	/* DDR  */
	{15, 0xC0000000, 0x2, 0x1},	/* EXP  */
	{16, 0x83000000, 0x4, 0x3}	/* IRAM */
};

static const struct slave apb_slv[MAX_TZ_APB_SLAVES] =
{
	{0, 0x90420000}, {1, 0x90410000},   /* TDMA, USB PHY    */
	{2, 0x90400000}, {5, 0x90450000},   /* TDM, TIMER       */
	{6, 0x90460000}, {7, 0x90470028},   /* PSU, GPIO        */
	{9, 0x90490000}, {10, 0x904A0000},  /* UART, USB3       */
	{11, 0x904B0120}, {14, 0x904E0000}, /* CLK, RTC         */
	{15, 0x904F0000}, {16, 0x90500000}, /* OTP, HFE Wrapper */
	{25, 0x90590000}, {26, 0x905A0000}, /* SERDES, EXP      */
	{27, 0x905B0000}, {29, 0x905D0000}, /* DDR PHY, TDMA2   */
	{30, 0x905E0000}, {0, 0x90600000}   /* DMAC, A9 Debug   */
};

/* Global variable to identify return from abort */
u8 return_from_abt = 0;

static u8 a9_master_tz_state = tz_state_t;	/* A9 AXI master will be trusted initially */
static u32 tz_marker = 0x77777777;
static u32 hw_blks_reset_state[12];

/*
*/
static void unreset_hw_blocks(void)
{
		hw_blks_reset_state[0] = readl(SERDES_RST_CNTRL);
		writel(0x0, SERDES_RST_CNTRL);

		hw_blks_reset_state[1] = readl(PCIe_SATA_RST_CNTRL);
		writel(0x0, PCIe_SATA_RST_CNTRL);

		hw_blks_reset_state[2] = readl(USB_RST_CNTRL);
		writel(0x0, USB_RST_CNTRL);

		hw_blks_reset_state[3] = readl(DDR_RESET);
		writel(0x0, DDR_RESET);

		hw_blks_reset_state[4] = readl(HFE_RESET);
		writel(0x0, HFE_RESET);

		hw_blks_reset_state[5] = readl(IPSEC_RESET);
		writel(0x0, IPSEC_RESET);

		hw_blks_reset_state[6] = readl(DECT_RESET);
		writel(0x0, DECT_RESET);

		hw_blks_reset_state[7] = readl(GEMTX_RESET);
		writel(0x0, GEMTX_RESET);

		hw_blks_reset_state[8] = readl(TDMNTG_RESET);
		writel(0x0, TDMNTG_RESET);

		hw_blks_reset_state[9] = readl(TSUNTG_RESET);
		writel(0x0, TSUNTG_RESET);

		hw_blks_reset_state[10] = readl(SATA_PMU_RESET);
		writel(0x0, SATA_PMU_RESET);

		hw_blks_reset_state[11] = readl(SATA_OOB_RESET);
		writel(0x0, SATA_OOB_RESET);
}

static void restore_hw_blks_rst_state(void)
{
		writel(hw_blks_reset_state[0], SERDES_RST_CNTRL);

		writel(hw_blks_reset_state[1], PCIe_SATA_RST_CNTRL);

		writel(hw_blks_reset_state[2], USB_RST_CNTRL);

		writel(hw_blks_reset_state[3], DDR_RESET);

		writel(hw_blks_reset_state[4], HFE_RESET);

		writel(hw_blks_reset_state[5], IPSEC_RESET);

		writel(hw_blks_reset_state[6], DECT_RESET);

		writel(hw_blks_reset_state[7], GEMTX_RESET);

		writel(hw_blks_reset_state[8], TDMNTG_RESET);

		writel(hw_blks_reset_state[9], TSUNTG_RESET);

		writel(hw_blks_reset_state[10], SATA_PMU_RESET);

		writel(hw_blks_reset_state[11], SATA_OOB_RESET);
}

/*
 * A9 AXI master's Trust state can be set to trusted or non-trusted.
 */
static void set_a9axi_master_tz_state(u8 tz_state)
{
	if(tz_state == a9_master_tz_state) {
		return;			/* Master already in the requested trust state */
	} else {
		tz_switch_trust_state(); /* Switch master's trust state */
		a9_master_tz_state = tz_state;
	}
	return;
}

/*
 * Set the non-secure state vector base address
 */
static void set_non_secure_state_vbr(void)
{
	uint32_t s;

	set_a9axi_master_tz_state(tz_state_nt);

	/* Set the exception base address for non-secure state */
#if !defined (COMCERTO_VERIFICATION_TZ)
	s = 0x00000000;
#else
	s = 0x83000000;
#endif
	__asm__ __volatile__("mcr p15, 0, %0, c12, c0, 0" : : "r"(s));

	/* Set up stack for abort operations */
	__asm__ __volatile__("mrs	r0, cpsr\n"
			     "bic	r1, r0, #0x1f\n"
			     "orr	r1, r1, #0x17\n"
			     "msr	CPSR_c, r1\n"
#if !defined (COMCERTO_VERIFICATION_TZ)
			     "ldr	r13, =0x10010500\n"
#else
			     "ldr	r13, =0x8300d500\n"
#endif
			     "msr	cpsr_c, r0");

	set_a9axi_master_tz_state(tz_state_t);
}

/*
 * Set the secure state vector base address
 */
static void set_secure_state_vbr(void)
{
	uint32_t s;

	set_a9axi_master_tz_state(tz_state_t);

	/* Set the exception base address for non-secure state */
#if !defined (COMCERTO_VERIFICATION_TZ)
	s = 0x00000000;
#else
	s = 0x83000000;
#endif
	__asm__ __volatile__("mcr p15, 0, %0, c12, c0, 0" : : "r"(s));

	/* Set up stack for abort operations */
	__asm__ __volatile__("mrs	r0, cpsr\n"
			     "bic	r1, r0, #0x1f\n"
			     "orr	r1, r1, #0x17\n"
			     "msr	CPSR_c, r1\n"
#if !defined (COMCERTO_VERIFICATION_TZ)
			     "ldr	r13, =0x10010000\n"
#else
			     "ldr	r13, =0x8300d100\n"
#endif
			     "msr	cpsr_c, r0");

	set_a9axi_master_tz_state(tz_state_t);
}

/*
 * Fully Trusted Slave's Trust state can be set to trusted or non-trusted.
 */
static void set_ft_slv_tz_state(u8 slave_id, u8 tz_state)
{
	u32 tmp;

	/* A9 AXI master should be trusted state to change slave's trust state */
	if(a9_master_tz_state != tz_state_t)
		return;

	if(tz_state) {
		tmp = readl(TZ_SLV_CFG);
		tmp |= 1 << ft_slv[slave_id].tz_cfg_bit_shift;
		writel(tmp , TZ_SLV_CFG);
		tmp = readl(TZ_SLV_CFG);	/* For Debug */
	} else {
		tmp = readl(TZ_SLV_CFG);
		tmp &= ~(1 << ft_slv[slave_id].tz_cfg_bit_shift);
		writel(tmp , TZ_SLV_CFG);
		tmp = readl(TZ_SLV_CFG);	/* For Debug */
	}
}

/*
 * Partially Trusted Slave's Trust state can be set to trusted or non-trusted.
 */
static void set_pt_slv_tz_state(u8 slave_id, u8 tz_state)
{
	u32 tmp;

	/* A9 AXI master should be trusted state to change slave's trust state */
	if(a9_master_tz_state != tz_state_t)
		return;

	if(tz_state){

		tmp = readl(TZ_SLV_CFG);
		tmp |= (1 << pt_slv[slave_id].tz_cfg_bit_shift);
		writel(tmp , TZ_SLV_CFG);

		switch (slave_id) {

		case ddr_axi_slave:

			tmp = readl(TZ_DDR_THRESH_CFG);
			tmp &= ~(TZ_CFG_DDR_HIGH_THR_MASK);
			tmp |= (pt_slv[slave_id].upper_thr << TZ_CFG_DDR_HIGH_THR_SHIFT);
			writel(tmp , TZ_DDR_THRESH_CFG);

			tmp = readl(TZ_DDR_THRESH_CFG);
			tmp &= ~(TZ_CFG_DDR_LOW_THR_MASK);
			tmp |= pt_slv[slave_id].lower_thr;
			writel(tmp , TZ_DDR_THRESH_CFG);
			tmp = readl(TZ_DDR_THRESH_CFG);	/* For Debug */
			break;

		case exp_ahb_slave:

			tmp = readl(TZ_EXP_THRESH_CFG);
			tmp &= ~(TZ_CFG_EXP_HIGH_THR_MASK);
			tmp |= (pt_slv[slave_id].upper_thr << TZ_CFG_EXP_HIGH_THR_SHIFT);
			writel(tmp , TZ_EXP_THRESH_CFG);

			tmp = readl(TZ_EXP_THRESH_CFG);
			tmp &= ~(TZ_CFG_EXP_LOW_THR_MASK);
			tmp |= pt_slv[slave_id].lower_thr;
			writel(tmp , TZ_EXP_THRESH_CFG);
			tmp = readl(TZ_EXP_THRESH_CFG);	/* For Debug */
			break;

		case iram_axi_slave:

			tmp = readl(TZ_IRAM_THRESH_CFG);
			tmp &= ~(TZ_CFG_IRAM_HIGH_THR_MASK);
			tmp |= (pt_slv[slave_id].upper_thr << TZ_CFG_IRAM_HIGH_THR_SHIFT);
			writel(tmp , TZ_IRAM_THRESH_CFG);

			tmp = readl(TZ_IRAM_THRESH_CFG);
			tmp &= ~(TZ_CFG_IRAM_LOW_THR_MASK);
			tmp |= pt_slv[slave_id].lower_thr;
			writel(tmp , TZ_IRAM_THRESH_CFG);
			tmp = readl(TZ_IRAM_THRESH_CFG); /* For Debug */
			break;

		default:
			break;
		}
	} else {
		tmp = readl(TZ_SLV_CFG);
		tmp &= ~(1 << pt_slv[slave_id].tz_cfg_bit_shift);
		writel(tmp , TZ_SLV_CFG);
		tmp = readl(TZ_SLV_CFG);	/* For Debug */
	}
}

/*
 * APB Slave's Trust state can be set to trusted to non-trusted.
 */
static void set_apb_slv_tz_state(u8 slave_id, u8 tz_state)
{
	u32 tmp;

	/* A9 AXI master should be trusted state to change slave's trust state */
	if(a9_master_tz_state != tz_state_t)
		return;

	if(tz_state) {
		if(slave_id != a9_dbg_slave) {
			tmp = readl(TZ_AABL_CFG);
			tmp |= (1 << apb_slv[slave_id].tz_cfg_bit_shift);
			writel(tmp , TZ_AABL_CFG);
			tmp = readl(TZ_AABL_CFG);	/* For Debug */
		} else {
			tmp = readl(TZ_AABL_CFG);
			writel(tmp , TZ_AABH_CFG);	/* Write to AABH will set the 33rd bit */
			tmp = readl(TZ_AABH_CFG);	/* For Debug */
			tmp = readl(TZ_AABL_CFG);	/* For Debug */
		}
	} else {
		if(slave_id != a9_dbg_slave) {
			tmp = readl(TZ_AABL_CFG);
			tmp &= ~(1 << apb_slv[slave_id].tz_cfg_bit_shift);
			writel(tmp , TZ_AABL_CFG);
			tmp = readl(TZ_AABL_CFG);	/* For Debug */
		} else {
			tmp = readl(TZ_AABL_CFG);
			writel(tmp , TZ_AABL_CFG);	/* Write to AABL will reset the 33rd bit */
			tmp = readl(TZ_AABH_CFG);	/* For Debug */
			tmp = readl(TZ_AABL_CFG);	/* For Debug */
		}
	}
}

/*
 * Verify address' read access by trying to read the address. In case of
 * failure, data abort will occur. There is also a mechanism added to this
 * function to continue with further testing in case of a data abort.
 */
static void verify_rd_access(u32 address)
{
	u32 value_read = 0;

	value_read = readl(address);

	if(return_from_abt) {
		return_from_abt = 0;
		set_value(0x7777ffff);
		return;
	}

	set_value(value_read);
	set_value(0x7777aaaa);
	return;
}

/*
 * Partially trusted slaves read access is verified.
 */
static void pt_verify_rd_access(u8 slave_id, u8 bit_shift)
{
	u32 test_addr = 0x0;

	test_addr = pt_slv[slave_id].address + 0x10;
	verify_rd_access(test_addr);
	test_addr = pt_slv[slave_id].address + (pt_slv[slave_id].lower_thr << bit_shift) - 0x10;
	verify_rd_access(test_addr);
	test_addr = pt_slv[slave_id].address + (pt_slv[slave_id].lower_thr << bit_shift) + 0x10;
	verify_rd_access(test_addr);
	test_addr = pt_slv[slave_id].address + (pt_slv[slave_id].upper_thr << bit_shift) - 0x10;
	verify_rd_access(test_addr);
	test_addr = pt_slv[slave_id].address + (pt_slv[slave_id].upper_thr << bit_shift) + 0x10;
	verify_rd_access(test_addr);

}

/*
 * APB slaves are configured to be trusted
 */
static void apb_slave_ft(void)
{
	 u8 slave_id;

	 tz_marker = 0x7777a100;
	 for(slave_id = 0; slave_id < MAX_TZ_APB_SLAVES;  slave_id++) {
		set_value(tz_marker + slave_id);
		set_apb_slv_tz_state(slave_id, tz_state_t);

	 }
}

/*
 * APB slaves are configured to be non-trusted
 */
static void apb_slave_fnt(void)
{
	u8 slave_id;

	tz_marker = 0x7777a200;
	for(slave_id = 0; slave_id < MAX_TZ_APB_SLAVES;  slave_id++) {
		set_value(tz_marker + slave_id);
		set_apb_slv_tz_state(slave_id, tz_state_nt);
	}
}

/*
 * AXI slaves are configured to be trusted
 */
static void axi_slave_ft(void)
{
	u8 slave_id;

	tz_marker = 0x7777a300;

	for(slave_id = 0; slave_id < MAX_TZ_FT_SLAVES;  slave_id++) {
		set_value(tz_marker + slave_id);
		set_ft_slv_tz_state(slave_id, tz_state_t);
	}
}

/*
 * AXI slaves are configured to be non-trusted
 */
static void axi_slave_fnt(void)
{
	u8 slave_id;

	tz_marker = 0x7777a400;

	for(slave_id = 0; slave_id < MAX_TZ_FT_SLAVES;  slave_id++) {
		set_value(tz_marker + slave_id);
		set_ft_slv_tz_state(slave_id, tz_state_nt);
	}
}

/*
 * The partially trusted slaves are configured with trusted and non-trusted
 * regions.
 */
static void axi_slave_pt(void)
{
	u8 slave_id;

	tz_marker = 0x7777a500;

	for(slave_id = 0; slave_id < MAX_TZ_PT_SLAVES; slave_id++) {
		set_value(tz_marker + slave_id);
		set_pt_slv_tz_state(slave_id, tz_state_t);
	}
}

/*
 * The partially trusted slaves are configured as fully non-trusted.
 */
static void axi_slave_pnt(void)
{
	u8 slave_id;

	tz_marker = 0x7777a600;

	for(slave_id = 0; slave_id < MAX_TZ_PT_SLAVES; slave_id++) {
		set_value(tz_marker + slave_id);
		set_pt_slv_tz_state(slave_id, tz_state_nt);
	}
}

/*
 * Access test for each APB slave
 */
static void access_test_apb(void)
{
	u8 slave_id;

	tz_marker = 0x7777a700;
	 for(slave_id = 0; slave_id < MAX_TZ_APB_SLAVES;  slave_id++) {
		set_value(tz_marker + slave_id);
		verify_rd_access(apb_slv[slave_id].address);
	 }
}

/*
 * Access test for each AXI slave
 */
static void access_test_axi_ft(void)
{
	u8 slave_id;

	tz_marker = 0x7777a800;
	 for(slave_id = 0; slave_id < MAX_TZ_FT_SLAVES;  slave_id++) {
		set_value(tz_marker + slave_id);
		verify_rd_access(ft_slv[slave_id].address);
	 }
}

/*
 * Access test for each partially trusted slave
 */
static void access_test_axi_pt(void)
{
	u8 slave_id;

	for(slave_id = 0; slave_id < MAX_TZ_PT_SLAVES;  slave_id++) {
		switch (slave_id) {

		case ddr_axi_slave:
			set_value(0x777700ab);
			pt_verify_rd_access(slave_id, 20);
			break;

		case exp_ahb_slave:
			set_value(0x777700bb);
			pt_verify_rd_access(slave_id, 20);
			break;

		case iram_axi_slave:
			set_value(0x777700cb);
			pt_verify_rd_access(slave_id, 13);
			break;

		default:
			break;
		}

	}
}

/*
 * APB slaves are tested.
 */
static void test_apb_slave(int if_nsecure)
{

	//Switch to Secure Mode to change the trust state
	if(if_nsecure)
		set_a9axi_master_tz_state(tz_state_t);

	set_value(0x77770003);
#if !defined (COMCERTO_VERIFICATION_TZ)
	printf("Configure APB slaves as fully non-trusted\n");
#endif
	apb_slave_fnt();


	set_value(0x77770004);
#if !defined (COMCERTO_VERIFICATION_TZ)
	printf("Test access to non-trusted APB slaves\n");
#endif
	//Switch to Normal Mode to perform the access test
        if(if_nsecure)
		set_a9axi_master_tz_state(tz_state_nt);

	access_test_apb();

	//Switch to Secure Mode to change the trust state
	if(if_nsecure)
		set_a9axi_master_tz_state(tz_state_t);

	set_value(0x77770005);
#if !defined (COMCERTO_VERIFICATION_TZ)
	printf("Configure APB slaves as fully trusted\n");
#endif
	apb_slave_ft();

	set_value(0x77770006);
#if !defined (COMCERTO_VERIFICATION_TZ)
	printf("Test access to trusted APB slaves\n");
#endif
	//Switch to Normal Mode to perform the access test
        if(if_nsecure)
		set_a9axi_master_tz_state(tz_state_nt);

	access_test_apb();

	//Switch to Secure Mode to change the trust state
	if(if_nsecure)
		set_a9axi_master_tz_state(tz_state_t);

}

/*
 * AXI slaves are tested
 */
static void test_axi_slave(int if_nsecure)
{
        if(if_nsecure)
		set_a9axi_master_tz_state(tz_state_t);

	set_value(0x77770008);
#if !defined (COMCERTO_VERIFICATION_TZ)
	printf("Configure AXI slaves as fully non-trusted\n");
#endif
	axi_slave_fnt();	/* Fully Non-Trusted AXI slaves */

        if(if_nsecure)
		set_a9axi_master_tz_state(tz_state_nt);

	set_value(0x77770009);
#if !defined (COMCERTO_VERIFICATION_TZ)
	printf("Test access to Fully trusted AXI slaves\n");
#endif
	access_test_axi_ft();

        if(if_nsecure)
		set_a9axi_master_tz_state(tz_state_t);

	set_value(0x7777000a);
#if !defined (COMCERTO_VERIFICATION_TZ)
	printf("Configure AXI slaves as fully trusted\n");
#endif
	axi_slave_ft();


	set_value(0x7777000b);
#if !defined (COMCERTO_VERIFICATION_TZ)
	printf("Test access to Fully non-trusted AXI slaves\n");
#endif
        if(if_nsecure)
		set_a9axi_master_tz_state(tz_state_nt);

	access_test_axi_ft();

        if(if_nsecure)
		set_a9axi_master_tz_state(tz_state_t);
}

/*
 * AXI partially trusted slaves are tested. This includes DDR, IRAM and EXP
 */
static void test_axi_pt_slave(int if_nsecure)
{
	if(if_nsecure)
		set_a9axi_master_tz_state(tz_state_t);

	set_value(0x7777000d);
#if !defined (COMCERTO_VERIFICATION_TZ)
	printf("Configure AXI slaves as partially non-trusted\n");
#endif
	axi_slave_pnt();	/* Partially Non-Trusted AXI slaves */


	set_value(0x7777000e);
#if !defined (COMCERTO_VERIFICATION_TZ)
	printf("Test access to partially trusted AXI slaves\n");
#endif
        if(if_nsecure)
		set_a9axi_master_tz_state(tz_state_nt);

	access_test_axi_pt();

        if(if_nsecure)
		set_a9axi_master_tz_state(tz_state_t);

	set_value(0x7777000f);
#if !defined (COMCERTO_VERIFICATION_TZ)
	printf("Configure AXI slaves as partially trusted\n");
#endif
	axi_slave_pt();


	set_value(0x77770010);
#if !defined (COMCERTO_VERIFICATION_TZ)
	printf("Test access to partially trusted AXI slaves\n");
#endif
        if(if_nsecure)
		set_a9axi_master_tz_state(tz_state_nt);

	access_test_axi_pt();

        if(if_nsecure)
		set_a9axi_master_tz_state(tz_state_t);

	axi_slave_pnt();	/* Partially Non-Trusted AXI slaves */
}

/*
 * A9 master will be in secure state and perform access tests on all
 * slaves.
 */
static void a9_secure_world(void)
{
	int if_nsecure = 0;

	set_a9axi_master_tz_state(tz_state_t);

	set_value(0x77770002);
#if !defined (COMCERTO_VERIFICATION_TZ)
	printf("Test APB slave access\n");
#endif
	test_apb_slave(if_nsecure);

	set_value(0x77770007);
#if !defined (COMCERTO_VERIFICATION_TZ)
	printf("Test AXI slave access\n");
#endif
	test_axi_slave(if_nsecure);

	set_value(0x7777000c);
#if !defined (COMCERTO_VERIFICATION_TZ)
	printf("Test AXI PT slave access\n");
#endif
	test_axi_pt_slave(if_nsecure);

}

/*
 * A9 master will be in non-secure state and perform access tests on all
 * slaves.
 */
static void a9_non_secure_world(void)
{
        int if_nsecure = 1;

      set_value(0x77770011);
#if !defined (COMCERTO_VERIFICATION_TZ)
	printf("Test APB slave access\n");
#endif
      test_apb_slave(if_nsecure);

      set_value(0x77770012);
#if !defined (COMCERTO_VERIFICATION_TZ)
      printf("Test AXI slave access\n");
#endif
	test_axi_slave(if_nsecure);

      set_value(0x77770013);
#if !defined (COMCERTO_VERIFICATION_TZ)
	printf("Test AXI PT slave access\n");
#endif
	test_axi_pt_slave(if_nsecure);

}

/*
 * Main test function which will initiate each tests sequentially
 */
int Comcerto_tz_test(struct diags_test_param *p)
{
	return_from_abt = 0;

	unreset_hw_blocks();

	set_start_mark();
#if !defined (COMCERTO_VERIFICATION_TZ)
	printf("##############################\n");
	printf("Initializing monitor.\n");
	printf("Setting secure/non-secure vbrs\n");
#endif
	tz_mon_init();
	set_secure_state_vbr();
	set_non_secure_state_vbr();

	set_value(0x77770001);
#if !defined (COMCERTO_VERIFICATION_TZ)
	printf("##############################\n");
	printf("Starting secure mode tests\n");
#endif
	a9_secure_world();

	set_value(0x77770101);
#if !defined (COMCERTO_VERIFICATION_TZ)
	printf("##############################\n");
	printf("Starting non-secure mode tests\n");
#endif
	a9_non_secure_world();

	set_end_mark();
#if !defined (COMCERTO_VERIFICATION_TZ)
	printf("##############################\n");
	printf("Completed TrustZone tests.\n");
#endif

#if defined (COMCERTO_VERIFICATION_TZ)
	restore_hw_blks_rst_state();
#endif

	return 0;
}

