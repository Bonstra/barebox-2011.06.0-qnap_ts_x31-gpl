#include "tests.h"
#include <diags.h>
#include <init.h>
#include "slic.h"
#include <asm/types.h>
#include <common.h>
#include <asm/io.h>
#include <asm/system.h>
#include <asm-generic/div64.h>

#include <mach/gpio.h>
#include <mach/comcerto-2000.h>
#include <mach/clkcore.h>

extern int legerity_init(void);

static inline void delay(volatile u32 delay_count)
{
	while(delay_count--);
}

#define TDMNTG_REG_RST          (1 << 0)
#define FSYNC_FALL_EDGE         (1 << 1)
#define NTG_DIV_RST_N           (1 << 5)
#define NTG_EN                  (1 << 0)

#define TDMNTG_ADDR_SPACE_BASEADDR	 (COMCERTO_APB_CLK_BASE + 0x280)
#define TDM_NTG_CLK_CTRL        (TDMNTG_ADDR_SPACE_BASEADDR + 0x00)
#define TDM_NTG_INCR            (TDMNTG_ADDR_SPACE_BASEADDR + 0x04)
#define TDM_FSYNC_GEN_CTRL      (TDMNTG_ADDR_SPACE_BASEADDR + 0x08)
#define TDM_FSYNC_LOW           (TDMNTG_ADDR_SPACE_BASEADDR + 0x0C)
#define TDM_FSYNC_HIGH          (TDMNTG_ADDR_SPACE_BASEADDR + 0x10)

#define TDMNTG_DEFAULT_REF_CLK 500000000
#define PLL_SOURCE_TDM_CLK      PLL2

/* TDM Divider value to 24 */
#define NTG_TDM_CLK_DIV 24

unsigned long tdmntg_ref_clk;

/* since 'ppm' means parts-per-million */
#define MILLION (1000000UL)

static unsigned long BaseClock = 0; /* frequency is set by clock_frequency_set() */

struct comcerto_tdm_data {
        u8 fsoutput; /* Generic Pad Control and Version ID Register[2] */
        u8 fspolarity; /*  28 FSYNC_FALL(RISE)_EDGE */
        u16 fshwidth; /* High_Phase_Width[10:0] */
        u16 fslwidth; /* Low_Phase_Width[26:16]] */
        u32 clockhz; /* INC_VALUE[29:0] According to the desired TDM clock output frequency, this field should be configured */
        u8 clockout; /* IO Control Register[21]  hardware or software control selection  IO Control Register[20] pads are input (output) */
        u8 tdmmux;
        u32 tdmck;
        u32 tdmfs;
        u32 tdmdx;
        u32 tdmdr;
};

//Legerity
static struct comcerto_tdm_data comcerto_tdm_pdata = {
        .fsoutput = 1, /* Generic Pad Control and Version ID Register[2] */
        .fspolarity = 0, /* 28 FSYNC_FALL(RISE)_EDGE */
        .fshwidth = 1, /* High_Phase_Width[10:0] */
        .fslwidth = 0xFF, /* Low_Phase_Width[10:0] */
        .clockhz = 2048000, /* INC_VALUE[29:0] According to the desired TDM clock output frequency, this field should be configured */
        .clockout = 1, /* 0 -> set bit 21, clear bit 20 in COMCERTO_GPIO_IOCTRL_REG
                          (software control, clock input)
                          1 -> set bit 21 and 20 in COMCERTO_GPIO_IOCTRL_REG
                          (software control, clock output)
                          2 -> clear bit 21 in COMCERTO_GPIO_IOCTRL_REG (hardware control) */
                /* TDM interface Muxing:0x0 - TDM block, 0x1 - ZDS block,
                0x2 - GPIO[63:60] signals and 0x3 - MSIF block is selected */
        .tdmmux = 0x1,
#if 0
        .tdmck = 0x11, //0x3F,
        .tdmfs = 0x11, //0x3F,
        .tdmdx = 0x11, //0x3F,
        .tdmdr = 0x11, //0x3F,
#endif
};


static void fsync_output_set(unsigned int fsoutput)
{
        if (fsoutput)
        {
                writel(readl(COMCERTO_GPIO_TDM_MUX) | (1 << 0), COMCERTO_GPIO_TDM_MUX);
                writel(readl(TDM_FSYNC_GEN_CTRL) | (1 << 0), TDM_FSYNC_GEN_CTRL);
        }
        else
        {
                writel(readl(COMCERTO_GPIO_TDM_MUX) & ~(1 << 0), COMCERTO_GPIO_TDM_MUX);
                writel(readl(TDM_FSYNC_GEN_CTRL) & ~(1 << 0), TDM_FSYNC_GEN_CTRL);
        }
}

static void fsync_polarity_set(unsigned int fspolarity)
{
        /* 28 FSYNC_FALL(RISE)_EDGE */
        if (fspolarity)
                writel(readl(TDM_FSYNC_GEN_CTRL) | FSYNC_FALL_EDGE, TDM_FSYNC_GEN_CTRL);
        else
                writel(readl(TDM_FSYNC_GEN_CTRL) & ~FSYNC_FALL_EDGE, TDM_FSYNC_GEN_CTRL);
}

static void fsync_lphase_set(u32 fslwidth)
{
        /* Low_Phase_Width 7ff- maximum */
        if (fslwidth > 0x7FF) {
                printf("%s: Low Phase width value is out of range %#x > 0x7FF", __func__, fslwidth);
                return;
        }

        writel(fslwidth, TDM_FSYNC_LOW);
}

static void fsync_hphase_set(u32 fshwidth)
{
        /* High_Phase_Width 7ff- maximum */
        if (fshwidth > 0x7FF) {
                printf("%s: High Phase width value is out of range %#x > 0x7FF", __func__, fshwidth);
                return;
        }

        writel(fshwidth, TDM_FSYNC_HIGH);
}

static void clock_frequency_set(unsigned long clockhz)
{
        unsigned long long ntg_incr; // = clockhz;

        /* Multiply  with  NTG_TDM_CLK_DIV(24) */
        ntg_incr = NTG_TDM_CLK_DIV * clockhz;

        /* get frequency resolution on an 32-bit accumulator */
        ntg_incr = ntg_incr * (1ULL << 32) + tdmntg_ref_clk / 2;
        do_div(ntg_incr, tdmntg_ref_clk);
        printf("%s: NTG INCR value is 0x%x\n", __func__, ntg_incr);

        /* ntg_incr = 0x192A7371 for 49.152 MHz */
        writel(ntg_incr, TDM_NTG_INCR);
}

static unsigned long clock_frequency_get(void)
{
        unsigned long long clc_data;

        /* According to the desired TDM clock output frequency, this field should be configured */
        clc_data = (readl(TDM_NTG_INCR) & 0x3FFFFFFF);/* get frequency from resolution on an 32-bit accumulator */
        clc_data = (clc_data * tdmntg_ref_clk + (1ULL << 31)) >> 32;
        return (unsigned long)clc_data;
}


static void clock_output_set(unsigned long clockout)
{
        switch (clockout) {
        case 0:
                writel((0x2 << 12) | (readl(COMCERTO_GPIO_BOOTSTRAP_OVERRIDE) & ~(0x3 << 12)), COMCERTO_GPIO_BOOTSTRAP_OVERRIDE);
                break;
        case 1:
                writel((0x3 << 12) | (readl(COMCERTO_GPIO_BOOTSTRAP_OVERRIDE) & ~(0x3 << 12)), COMCERTO_GPIO_BOOTSTRAP_OVERRIDE);
                break;
        case 2:
                writel((0x0 << 12) | (readl(COMCERTO_GPIO_BOOTSTRAP_OVERRIDE) & ~(0x3 << 12)), COMCERTO_GPIO_BOOTSTRAP_OVERRIDE);
                break;
        default:
                printf("%s: Unknown clock output value\n", __func__);
        }
}

static void clock_ppm_adjust(long ppm)
{
        unsigned long long clc_data;
        unsigned long freq_set = BaseClock;
        int nsign = 0;

        if (!freq_set) {
                printf("(%s): Could not adjust frequency: you should set it before\n", __func__);
                return;
        }

        if (ppm < 0) {
                nsign = 1;
                ppm = -ppm;
        }

        if (nsign && (ppm >= MILLION)) {
                /* overflow dangerous */
                printf("(%s): This is too much ppm: -%lu\n", __func__, (unsigned long)ppm);
                return;
        }

        clc_data = ppm * (1ULL << 32);

        do_div(clc_data, MILLION);

        if (nsign) {
                clc_data = (1ULL << 32) - clc_data;
        } else {
                clc_data = (1ULL << 32) + clc_data;
        }

        clc_data = clc_data * freq_set + tdmntg_ref_clk / 2; /* with rounding to nearest integer */

        if (clc_data & ~0x3FFFFFFF) {
                /* unaccounted bits dangerous */
                printf("(%s): This is too much ppm: %lu\n", __func__, (unsigned long)ppm);
                return;
        }

        writel((clc_data & 0x3FFFFFFF) | (readl(TDM_NTG_INCR) & ~0x3FFFFFFF), TDM_NTG_INCR);
}

static long clock_ppm_get(void)
{
        unsigned long freq_set = BaseClock;
        unsigned long freq = clock_frequency_get();
        unsigned long long ppm;

        if (freq > freq_set) {
                ppm = (freq - freq_set) * MILLION + (freq_set >> 1);
                do_div(ppm, freq_set);
                return (long)ppm;
        } else {
                ppm = (freq_set - freq) * MILLION + (freq_set >> 1);
                do_div(ppm, freq_set);
                return (-1 * (long)ppm);
        }
}

static void tdm_mux_set(u32 tdmmux)
{
        switch (tdmmux){
        case 0:
                // TDM block selected
                writel((0x0 << 4) |(readl(COMCERTO_GPIO_MISC_PIN_SELECT_REG) & ~(0x3 << 4)), COMCERTO_GPIO_MISC_PIN_SELECT_REG);
                break;

        case 1:
                // ZDS (Zarlink) block selected
                writel((0x1 << 4) |(readl(COMCERTO_GPIO_MISC_PIN_SELECT_REG) & ~(0x3 << 4)), COMCERTO_GPIO_MISC_PIN_SELECT_REG);
                break;

        case 2:
                // GPIO[63:60] signals selected
                writel((0x2 << 4) |(readl(COMCERTO_GPIO_MISC_PIN_SELECT_REG) & ~(0x3 << 4)), COMCERTO_GPIO_MISC_PIN_SELECT_REG);
                break;

        case 3:
                // MSIF (SiLabs) selected
                writel((0x3 << 4) |(readl(COMCERTO_GPIO_MISC_PIN_SELECT_REG) & ~(0x3 << 4)), COMCERTO_GPIO_MISC_PIN_SELECT_REG);
                break;

        default:
                printf("%s: Unknown TDM MUX value\n", __func__);
        }
}

static void tdm_dr_set(u32 tdmdr)
{
        if(tdmdr > 0x3F)
        {
                printf("%s: TDM_DR value is out of range %#x > 0x3F",  __func__, tdmdr);
                return;
        }

        writel((tdmdr << 24) |(readl(COMCERTO_GPIO_PAD_CONFIG0) & ~(0x3F << 24)), COMCERTO_GPIO_PAD_CONFIG0);
}

static void tdm_dx_set(u32 tdmdx)
{
        if(tdmdx > 0x3F)
        {
                printf("%s: TDM_DX value is out of range %#x > 0x3F", __func__, tdmdx);
                return;
        }

        writel((tdmdx << 18) |(readl(COMCERTO_GPIO_PAD_CONFIG0) & ~(0x3F << 18)), COMCERTO_GPIO_PAD_CONFIG0);
}

static void tdm_fs_set(u32 tdmfs)
{
        if(tdmfs > 0x3F)
        {
                printf("%s: TDM_FS value is out of range %#x > 0x3F", __func__, tdmfs);
                return;
        }

        writel((tdmfs << 12) |(readl(COMCERTO_GPIO_PAD_CONFIG0) & ~(0x3F << 12)), COMCERTO_GPIO_PAD_CONFIG0);
}

static void tdm_ck_set(u32 tdmck)
{
        if(tdmck > 0x3F)
        {
                printf("%s: TDM_CK value is out of range %#x > 0x3F", __func__, tdmck);
                return;
        }

        writel((tdmck << 6) |(readl(COMCERTO_GPIO_PAD_CONFIG0) & ~(0x3F << 6)), COMCERTO_GPIO_PAD_CONFIG0);
}

void c2k_zds_init(void)
{
	tdmntg_ref_clk = TDMNTG_DEFAULT_REF_CLK;

	printf("Initializing ZDS/NTG..\n");

        // Take TDM NTG out of reset
        writel(readl(TDMNTG_RESET) & ~TDMNTG_REG_RST, TDMNTG_RESET);

        switch(PLL_SOURCE_TDM_CLK)
        {
        case PLL0:
                writel(readl(TDMNTG_REF_CLK_CNTRL) | ((1 << 0) | (1 << 0)), TDMNTG_REF_CLK_CNTRL);
                break;

        case PLL1:
                writel(readl(TDMNTG_REF_CLK_CNTRL) | ((1 << 1) | (1 << 0)), TDMNTG_REF_CLK_CNTRL);
                break;

        case PLL3:
                writel(readl(TDMNTG_REF_CLK_CNTRL) | ((1 << 3) | (1 << 0)), TDMNTG_REF_CLK_CNTRL);
                break;

        case PLL2:
        default:
                writel(readl(TDMNTG_REF_CLK_CNTRL) | ((1 << 2) | (1 << 0)), TDMNTG_REF_CLK_CNTRL);
                break;
        }

        // NTG REF CLK is derived from PLL2
        writel(0x3, TDMNTG_REF_CLK_DIV_CNTRL);
        writel((NTG_DIV_RST_N | NTG_EN), TDM_NTG_CLK_CTRL);
        writel(0x80, TDM_CLK_CNTRL); /* NTG = 24 x TDM */

        /* Inital configuration of tdm bus */
        fsync_polarity_set(comcerto_tdm_pdata.fspolarity);
        fsync_lphase_set(comcerto_tdm_pdata.fslwidth);
        fsync_hphase_set(comcerto_tdm_pdata.fshwidth);
        clock_frequency_set(comcerto_tdm_pdata.clockhz);
        clock_output_set(comcerto_tdm_pdata.clockout);
        fsync_output_set(comcerto_tdm_pdata.fsoutput);
        tdm_mux_set(comcerto_tdm_pdata.tdmmux);
#if 0
   // The default paramters are good
        tdm_dr_set(comcerto_tdm_pdata.tdmdr);
        tdm_dx_set(comcerto_tdm_pdata.tdmdx);
        tdm_fs_set(comcerto_tdm_pdata.tdmfs);
        tdm_ck_set(comcerto_tdm_pdata.tdmck);
#endif
        writel(0x18, TDM_CLK_CNTRL); //remove out of reset

#define COMCERTO_GPIO1_OUTPUT_REG                       ((COMCERTO_APB_GPIO_BASE + 0xd0))
        writel(0x1 << 30, COMCERTO_GPIO1_OUTPUT_REG);

}

int Comcerto_zds_slic_test (struct diags_test_param *p)
{
	int ret = 0;

	c2k_zds_init();
	ret = legerity_init();
	if(ret)
		return -1;

	return 0;
}

