#include "comcerto-2000.h"

#define USB0_PHY_CTRL_REG0           0x90410000
#define USB1_PHY_CTRL_REG0           0x90410010
#define USB_PHY_SCALEDOWN_ADDR       0x9046003C

#define DEVICE_RST_CNTRL	     (COMCERTO_APB_CLK_BASE + 0x00)
#define SERDES_RST_CNTRL             (COMCERTO_APB_CLK_BASE + 0x04)
#define PCIe_SATA_RST_CNTRL          (COMCERTO_APB_CLK_BASE + 0x08)
#define USB_RST_CNTRL                (COMCERTO_APB_CLK_BASE + 0x0C)
#define A9DP_PWR_STAT                (COMCERTO_APB_CLK_BASE + 0x28)
#define A9DP_PWR_CNTRL               (COMCERTO_APB_CLK_BASE + 0x2C)
#define GNRL_CLK_CNTRL_0	     (COMCERTO_APB_CLK_BASE + 0x30)
#define GNRL_CLK_CNTRL_1	     (COMCERTO_APB_CLK_BASE + 0x34)
#define PLLS_GLOBAL_CNTRL            (COMCERTO_APB_CLK_BASE + 0x38)
#define AXI_CLK_CNTRL_0              (COMCERTO_APB_CLK_BASE + 0x40)
#define AXI_CLK_CNTRL_1              (COMCERTO_APB_CLK_BASE + 0x44)
#define AXI_CLK_CNTRL_2              (COMCERTO_APB_CLK_BASE + 0x48)
#define AXI_CLK_DIV_CNTRL            (COMCERTO_APB_CLK_BASE + 0x4C)
#define AXI_RESET_0                  (COMCERTO_APB_CLK_BASE + 0x50)
#define AXI_RESET_1                  (COMCERTO_APB_CLK_BASE + 0x54)
#define AXI_RESET_2                  (COMCERTO_APB_CLK_BASE + 0x58)
#define A9DP_MPU_CLK_CNTRL           (COMCERTO_APB_CLK_BASE + 0x68)
#define A9DP_MPU_CLK_DIV_CNTRL       (COMCERTO_APB_CLK_BASE + 0x6C)
#define A9DP_MPU_RESET               (COMCERTO_APB_CLK_BASE + 0x70)
#define A9DP_CPU_CLK_CNTRL           (COMCERTO_APB_CLK_BASE + 0x74)
#define A9DP_CPU_RESET               (COMCERTO_APB_CLK_BASE + 0x78)
#define A9DP_CLK_CNTRL               (COMCERTO_APB_CLK_BASE + 0x80)
#define A9DP_CLK_DIV_CNTRL           (COMCERTO_APB_CLK_BASE + 0x84)
#define A9DP_RESET                   (COMCERTO_APB_CLK_BASE + 0x88)
#define L2CC_CLK_CNTRL               (COMCERTO_APB_CLK_BASE + 0x90)
#define L2CC_CLK_DIV_CNTRL           (COMCERTO_APB_CLK_BASE + 0x94)
#define L2CC_RESET                   (COMCERTO_APB_CLK_BASE + 0x98)
#define TPI_CLK_CNTRL                (COMCERTO_APB_CLK_BASE + 0xA0)
#define TPI_CLK_DIV_CNTRL            (COMCERTO_APB_CLK_BASE + 0xA4)
#define TPI_RESET                    (COMCERTO_APB_CLK_BASE + 0xA8)
#define CSYS_CLK_CNTRL               (COMCERTO_APB_CLK_BASE + 0xB0)
#define CSYS_CLK_DIV_CNTRL           (COMCERTO_APB_CLK_BASE + 0xB4)
#define CSYS_RESET                   (COMCERTO_APB_CLK_BASE + 0xB8)
#define EXTPHY0_CLK_CNTRL            (COMCERTO_APB_CLK_BASE + 0xC0)
#define EXTPHY0_CLK_DIV_CNTRL        (COMCERTO_APB_CLK_BASE + 0xC4)
#define EXTPHY0_RESET                (COMCERTO_APB_CLK_BASE + 0xC8)
#define EXTPHY1_CLK_CNTRL            (COMCERTO_APB_CLK_BASE + 0xD0)
#define EXTPHY1_CLK_DIV_CNTRL        (COMCERTO_APB_CLK_BASE + 0xD4)
#define EXTPHY1_RESET                (COMCERTO_APB_CLK_BASE + 0xD8)
#define EXTPHY2_CLK_CNTRL            (COMCERTO_APB_CLK_BASE + 0xE0)
#define EXTPHY2_CLK_DIV_CNTRL        (COMCERTO_APB_CLK_BASE + 0xE4)
#define EXTPHY2_RESET                (COMCERTO_APB_CLK_BASE + 0xE8)
#define DDR_CLK_CNTRL                (COMCERTO_APB_CLK_BASE + 0xF0)
#define DDR_CLK_DIV_CNTRL            (COMCERTO_APB_CLK_BASE + 0xF4)
#define DDR_RESET                    (COMCERTO_APB_CLK_BASE + 0xF8)
#define HFE_CLK_CNTRL                (COMCERTO_APB_CLK_BASE + 0x100)
#define HFE_CLK_DIV_CNTRL            (COMCERTO_APB_CLK_BASE + 0x104)
#define HFE_RESET                    (COMCERTO_APB_CLK_BASE + 0x108)
#define IPSEC_CLK_CNTRL              (COMCERTO_APB_CLK_BASE + 0x110)
#define IPSEC_CLK_DIV_CNTRL          (COMCERTO_APB_CLK_BASE + 0x114)
#define IPSEC_RESET                  (COMCERTO_APB_CLK_BASE + 0x118)
#define DECT_CLK_CNTRL               (COMCERTO_APB_CLK_BASE + 0x120)
#define DECT_CLK_DIV_CNTRL           (COMCERTO_APB_CLK_BASE + 0x124)
#define DECT_RESET                   (COMCERTO_APB_CLK_BASE + 0x128)
#define GEMTX_CLK_CNTRL              (COMCERTO_APB_CLK_BASE + 0x130)
#define GEMTX_CLK_DIV_CNTRL          (COMCERTO_APB_CLK_BASE + 0x134)
#define GEMTX_RESET                  (COMCERTO_APB_CLK_BASE + 0x138)
#define TDMNTG_REF_CLK_CNTRL         (COMCERTO_APB_CLK_BASE + 0x140)
#define TDMNTG_REF_CLK_DIV_CNTRL     (COMCERTO_APB_CLK_BASE + 0x144)
#define TDMNTG_RESET                 (COMCERTO_APB_CLK_BASE + 0x148)
#define TDM_CLK_CNTRL                (COMCERTO_APB_CLK_BASE + 0x14C)
#define TSUNTG_REF_CLK_CNTRL         (COMCERTO_APB_CLK_BASE + 0x150)
#define TSUNTG_REF_CLK_DIV_CNTRL     (COMCERTO_APB_CLK_BASE + 0x154)
#define TSUNTG_RESET                 (COMCERTO_APB_CLK_BASE + 0x158)
#define SATA_PMU_CLK_CNTRL           (COMCERTO_APB_CLK_BASE + 0x160)
#define SATA_PMU_CLK_DIV_CNTRL       (COMCERTO_APB_CLK_BASE + 0x164)
#define SATA_PMU_RESET               (COMCERTO_APB_CLK_BASE + 0x168)
#define SATA_OOB_CLK_CNTRL           (COMCERTO_APB_CLK_BASE + 0x170)
#define SATA_OOB_CLK_DIV_CNTRL       (COMCERTO_APB_CLK_BASE + 0x174)
#define SATA_OOB_RESET               (COMCERTO_APB_CLK_BASE + 0x178)
#define PLL0_M_LSB                   (COMCERTO_APB_CLK_BASE + 0x1C0)
#define PLL0_M_MSB                   (COMCERTO_APB_CLK_BASE + 0x1C4)
#define PLL0_P                       (COMCERTO_APB_CLK_BASE + 0x1C8)
#define PLL0_S                       (COMCERTO_APB_CLK_BASE + 0x1CC)
#define PLL0_CNTRL                   (COMCERTO_APB_CLK_BASE + 0x1D0)
#define PLL0_TEST                    (COMCERTO_APB_CLK_BASE + 0x1D4)
#define PLL0_STATUS                  (COMCERTO_APB_CLK_BASE + 0x1D8)
#define PLL1_M_LSB                   (COMCERTO_APB_CLK_BASE + 0x1E0)
#define PLL1_M_MSB                   (COMCERTO_APB_CLK_BASE + 0x1E4)
#define PLL1_P                       (COMCERTO_APB_CLK_BASE + 0x1E8)
#define PLL1_S                       (COMCERTO_APB_CLK_BASE + 0x1EC)
#define PLL1_CNTRL                   (COMCERTO_APB_CLK_BASE + 0x1F0)
#define PLL1_TEST                    (COMCERTO_APB_CLK_BASE + 0x1F4)
#define PLL1_STATUS                  (COMCERTO_APB_CLK_BASE + 0x1F8)
#define PLL2_M_LSB                   (COMCERTO_APB_CLK_BASE + 0x200)
#define PLL2_M_MSB                   (COMCERTO_APB_CLK_BASE + 0x204)
#define PLL2_P                       (COMCERTO_APB_CLK_BASE + 0x208)
#define PLL2_S                       (COMCERTO_APB_CLK_BASE + 0x20C)
#define PLL2_CNTRL                   (COMCERTO_APB_CLK_BASE + 0x210)
#define PLL2_TEST                    (COMCERTO_APB_CLK_BASE + 0x214)
#define PLL2_STATUS                  (COMCERTO_APB_CLK_BASE + 0x218)
#define PLL3_M_LSB                   (COMCERTO_APB_CLK_BASE + 0x220)
#define PLL3_M_MSB                   (COMCERTO_APB_CLK_BASE + 0x224)
#define PLL3_P                       (COMCERTO_APB_CLK_BASE + 0x228)
#define PLL3_S                       (COMCERTO_APB_CLK_BASE + 0x22C)
#define PLL3_CNTRL                   (COMCERTO_APB_CLK_BASE + 0x230)
#define PLL3_TEST                    (COMCERTO_APB_CLK_BASE + 0x234)
#define PLL3_STATUS                  (COMCERTO_APB_CLK_BASE + 0x238)
#define PLL3_DITHER_CNTRL 	     (COMCERTO_APB_CLK_BASE + 0x23C) 
#define PLL3_K_LSB 		     (COMCERTO_APB_CLK_BASE + 0x240) 
#define PLL3_K_MSB 		     (COMCERTO_APB_CLK_BASE + 0x244) 
#define PLL3_MFR 		     (COMCERTO_APB_CLK_BASE + 0x248) 
#define PLL3_MRR 		     (COMCERTO_APB_CLK_BASE + 0x24C)

/* PCIe, SATA,  and SERDES Reset bits*/ 
#define PCIE0_PWR_RST           (1 << 0)
#define PCIE0_REG_RST  	        (1 << 1)
#define PCIE1_PWR_RST           (1 << 2)
#define PCIE1_REG_RST  	        (1 << 3)

#define SATA0_RX_RST           	(1 << 4)
#define SATA0_TX_RST 	       	(1 << 5)
#define SATA1_RX_RST           	(1 << 6)
#define SATA1_TX_RST  	        (1 << 7)

#define PCIE0_AXI_RST  	        (1 << 0)
#define PCIE1_AXI_RST  	        (1 << 1)
#define SATA_AXI_RST	        (1 << 2)
#define AXI_PCIE0_CLK_EN	(1 << 0)
#define AXI_PCIE1_CLK_EN	(1 << 1)
#define AXI_SATA_CLK_EN		(1 << 2)

#define SERDES0_RST           	(1 << 0)
#define SERDES1_RST           	(1 << 1)
#define SERDES2_RST           	(1 << 2)

/* AXI_RESET_1 bits */
#define HFE_AXI_RESET		(1 << 3)

//Clock Divider mirror mechanism in IRAM. It is going to take more than 256 bytes of IRAM
#define IRAM_CLK_REG_MIRROR	0x8300FC00 //maybe to need to think of the another fixed location
#define CLK_REG_DIV_BUG_BASE	AXI_CLK_DIV_CNTRL	

#define PLL_RESET (1 << 0)
#define PLL_BYPASS (1 << 4)
#define PLL_LOCK_EN (1 << 5)
#define PLL_VSEL (1 << 6)

#define CLK_DIV_BYPASS	(1 << 7)
#define CLK_A9DP_PERI_DIV_BYPASS	(1 << 3)
#define CLK_A9DP_ACP_DIV_BYPASS	(1 << 7)

#define CLK_PLL_SRC_MASK	0x7
#define CLK_PLL_SRC_SHIFT	1

#define CLK_DIV_VAL_DEFAULT		0x2
#define A9DP_ACP_CLK_DIV_VAL_DEFAULT	0x2
#define A9DP_PERI_CLK_DIV_VAL_DEFAULT	0x2

struct pll_info {
	u32 m;
	u32 p;
	u32 s;
	u32 vsel;
};

struct pll3_info {
	u32 m;
	u32 p;
	u32 s;
	u32 k;
	u32 vsel;
};

struct pll_setting {
        u32 pll0_freq_idx;
        u32 pll1_freq_idx;
        u32 pll2_freq_idx;
        u32 pll3_freq_idx;
};

struct clock_cfg_settings 
{
	u32 pll_cfg_idx;

	u32 arm_clk;
	u32 arm_clk_src;

	u32 axi_clk;
	u32 axi_clk_src;

	u32 ddr_clk;
	u32 ddr_clk_src;

	u32 ipsec_clk;
	u32 ipsec_clk_src;

	u32 sata_oob_clk;
	u32 sata_oob_clk_src;

	u32 sata_pmu_clk;
	u32 sata_pmu_clk_src;

	u32 dect_clk;
	u32 dect_clk_src;

	u32 l2cc_clk;
	u32 l2cc_clk_src;

	u32 hfe_clk;
	u32 hfe_clk_src;
	
	u32 gemtx_clk;
	u32 gemtx_clk_src;

	u32 extphy0_clk;
	u32 extphy0_clk_src;

	u32 extphy1_clk;
	u32 extphy1_clk_src;

	u32 extphy2_clk;
	u32 extphy2_clk_src;

	u32 tpi_clk;
	u32 tpi_clk_src;

	u32 csys_clk;
	u32 csys_clk_src;
};

enum {
	PLL0 = 0,
	PLL1,
	PLL2,
	PLL3
};

#define CFG_REFCLKFREQ_24          24000000        /* 24 MHz */
#define CFG_REFCLKFREQ_48          48000000        /* 48 MHz */
#define CFG_REFCLKFREQ          CFG_REFCLKFREQ_48

#define PLL_FREQ_2400	2400
#define PLL_FREQ_1800	1800
#define PLL_FREQ_1600	1600
#define PLL_FREQ_1500	1500
#define PLL_FREQ_1400	1400
#define PLL_FREQ_1300	1300
#define PLL_FREQ_1066	1066
#define PLL_FREQ_1000	1000
#define PLL_FREQ_800	800
#define PLL_FREQ_750	750
#define PLL_FREQ_500	500

enum {
	PLL_CFG_1800 = 0,
	PLL_CFG_1600,
	PLL_CFG_1500,
	PLL_CFG_1400,
	PLL_CFG_1300,
	PLL_CFG_1000,
	PLL_CFG_800,
	PLL_CFG_750,
	PLL_CFG_500,
	PLL_CFG_2400	
};

enum {
	PLL3_CFG_1066 = 0,
	PLL3_CFG_800
};

#define PLL0_CFG_2400   PLL_CFG_2400
#define PLL0_CFG_1300   PLL_CFG_1300
#define PLL0_CFG_1800   PLL_CFG_1800
#define PLL0_CFG_750    PLL_CFG_750

#define PLL1_CFG_1000   PLL_CFG_1000
#define PLL1_CFG_800    PLL_CFG_800

#define PLL2_CFG_1500   PLL_CFG_1500
#define PLL2_CFG_500    PLL_CFG_500


enum {
	CLK_CFG1 = 0,
	CLK_CFG2,
	CLK_CFG3,
	CLK_CFG4,
	CLK_CFG5,
	CLK_CFG6	
};

enum {
	PLL_CFG_1800_1000_1500_1066 = 0,
	PLL_CFG_750_1000_1500_800,
	PLL_CFG_1300_800_500_800,
	PLL_CFG_1300_800_500_1066,
	PLL_CFG_1800_800_500_1066,
	PLL_CFG_2400_1000_1500_1066	
};

#if     defined(CONFIG_M86201) || defined(CONFIG_M86202) || defined(CONFIG_M86203) || defined(CONFIG_M86204) || defined(CONFIG_M86206) || defined(CONFIG_M86207) || defined(CONFIG_M86208)
#define CLK_CFG		CLK_CFG6
#elif 	defined(CONFIG_M86260) || defined(CONFIG_M86261) || defined(CONFIG_M86262)
#define CLK_CFG		CLK_CFG3
#elif 	defined(CONFIG_M86261_NAS) 
#define CLK_CFG		CLK_CFG4
#elif   defined (CONFIG_M86291) || defined (CONFIG_M86292) || defined (CONFIG_M86293) || defined (CONFIG_M86294) || defined (CONFIG_M86295) || defined (CONFIG_M86296) || defined (CONFIG_M86297) || defined (CONFIG_M86298)
#define CLK_CFG		CLK_CFG1
#endif

#define ARM_CLK_1200	1200
#define ARM_CLK_900	900
#define ARM_CLK_750	750
#define ARM_CLK_650	650
#define AXI_CLK_250	250
#define AXI_CLK_200	200
#define DDR_CLK_533	533
#define DDR_CLK_400	400
#define SATA_OOB_CLK_125	125
#define SATA_PMU_CLK_30		30
#define SATA_PMU_CLK_25		25
#define	IPSEC_CLK_300	300
#define	IPSEC_CLK_250	250
#define DECT_CLK_250	250
#define L2CC_CLK_600	600
#define L2CC_CLK_450	450
#define L2CC_CLK_375	375
#define L2CC_CLK_325	325
#define HFE_CLK_500	500
#define HFE_CLK_400	400
#define GEMTX_CLK_125	125
#define EXTPHY0_CLK_125	125
#define EXTPHY1_CLK_125	125
#define EXTPHY2_CLK_125	125
#define TPI_CLK_250	250
#define CSYS_CLK_166	166


#define read_clk_div_bypass_backup(reg) readl(reg - CLK_REG_DIV_BUG_BASE + IRAM_CLK_REG_MIRROR)
#define write_clk_div_bypass_backup(val, reg) writel(val, reg - CLK_REG_DIV_BUG_BASE + IRAM_CLK_REG_MIRROR)

