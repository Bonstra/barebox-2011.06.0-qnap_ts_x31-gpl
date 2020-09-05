#include <common.h>
#include <asm/types.h>
#include <asm/io.h>
#include <mach/bits.h>
#include <mach/clkcore.h>
#include <mach/gpio.h>

#if defined(CONFIG_COMCERTO_PLL)

static inline void HAL_set_clock_divider(int clk_freq, int pll_src, u32 reg);

struct clock_cfg_settings *cfg_clk;

static u32 pll_cfg_freq_table[] = {
	[PLL_CFG_1800] = PLL_FREQ_1800,
	[PLL_CFG_1600] = PLL_FREQ_1600,
	[PLL_CFG_1500] = PLL_FREQ_1500,
	[PLL_CFG_1400] = PLL_FREQ_1400,
	[PLL_CFG_1300] = PLL_FREQ_1300,
	[PLL_CFG_1000] = PLL_FREQ_1000,
	[PLL_CFG_800] = PLL_FREQ_800,
	[PLL_CFG_750] = PLL_FREQ_750,
	[PLL_CFG_500] = PLL_FREQ_500,
	[PLL_CFG_2400] = PLL_FREQ_2400	
};

static u32 pll3_cfg_freq_table[] = {
	[PLL3_CFG_1066] = PLL_FREQ_1066,
	[PLL3_CFG_800] = PLL_FREQ_800
};

static struct pll_info pll_table[] = {
#if (CFG_REFCLKFREQ == CFG_REFCLKFREQ_24)
        [PLL_CFG_1800] = {450,  6,  0,	1},
        [PLL_CFG_1600] = {400,  6,  0,	1},
        [PLL_CFG_1500] = {375,  6,  0,	1},
        [PLL_CFG_1400] = {350,  6,  0,	0},
        [PLL_CFG_1300] = {325,  6,  0,	0},
        [PLL_CFG_1000] = {500,  6,  1,	1},
        [PLL_CFG_800] =  {400,  6,  1,	1},
        [PLL_CFG_750] =  {375,  6,  1,	1},
        [PLL_CFG_500] =  {500,  6,  2,	1},
		[PLL_CFG_2400] =  {600,	6,	0,	1}
#else
        [PLL_CFG_1800] = {450,  12,  0,  1},
        [PLL_CFG_1600] = {400,  12,  0,  1},
        [PLL_CFG_1500] = {375,  12,  0,  1},
        [PLL_CFG_1400] = {350,  12,  0,  0},
        [PLL_CFG_1300] = {325,  12,  0,  0},
        [PLL_CFG_1000] = {500,  12,  1,  1},
        [PLL_CFG_800] =  {400,  12,  1,  1},
        [PLL_CFG_750] =  {375,  12,  1,  1},
        [PLL_CFG_500] =  {500,  12,  2,  1},
		[PLL_CFG_2400] =  {600,	12,  0,  1}
#endif
};

static struct pll3_info pll3_table[] = {
#if (CFG_REFCLKFREQ == CFG_REFCLKFREQ_24)
	[PLL3_CFG_1066]	= {0x57, 2, 0, 0x755, 1},
#else
        [PLL3_CFG_1066]	= {0x57, 4, 0, 0x755, 1},
	[PLL3_CFG_800]	= {0x41, 4, 0, 0x6ab, 0}
#endif
};

struct pll_setting pll_freq_table[] = {
        {PLL0_CFG_1800, PLL1_CFG_1000, PLL2_CFG_1500, PLL3_CFG_1066},
	{PLL0_CFG_750, PLL1_CFG_1000, PLL2_CFG_1500, PLL3_CFG_800},
        {PLL0_CFG_1300, PLL1_CFG_800, PLL2_CFG_500, PLL3_CFG_800},
        {PLL0_CFG_1300, PLL1_CFG_800, PLL2_CFG_500, PLL3_CFG_1066},
        {PLL0_CFG_1800, PLL1_CFG_800, PLL2_CFG_500, PLL3_CFG_1066},
        {PLL0_CFG_2400, PLL1_CFG_1000, PLL2_CFG_1500, PLL3_CFG_1066},        
};

struct clock_cfg_settings clk_cfg_table[] =
{
        [CLK_CFG1] = { PLL_CFG_1800_1000_1500_1066,
                        ARM_CLK_900, PLL0,
                        AXI_CLK_250, PLL1,
                        DDR_CLK_533, PLL3,
                        IPSEC_CLK_300, PLL2,
                        SATA_OOB_CLK_125, PLL1,
                        SATA_PMU_CLK_30, PLL0,
                        DECT_CLK_250, PLL2,
                        L2CC_CLK_450, PLL0,
                        HFE_CLK_500, PLL1,
                        GEMTX_CLK_125, PLL2,
                        EXTPHY0_CLK_125, PLL2,
                        EXTPHY1_CLK_125, PLL2,
                        EXTPHY2_CLK_125, PLL2,
			TPI_CLK_250, PLL0,
			CSYS_CLK_166, PLL0},


	[CLK_CFG2] = { PLL_CFG_750_1000_1500_800,
			ARM_CLK_750, PLL2,
			AXI_CLK_250, PLL2,
			DDR_CLK_400, PLL3,
			IPSEC_CLK_300 , PLL0,
			SATA_OOB_CLK_125, PLL0,
			SATA_PMU_CLK_30, PLL0,
			DECT_CLK_250, PLL0,
			L2CC_CLK_375, PLL0,
			HFE_CLK_500, PLL2,
			GEMTX_CLK_125, PLL0,
			EXTPHY0_CLK_125, PLL0,
			EXTPHY1_CLK_125, PLL0,
			EXTPHY2_CLK_125, PLL0,
			TPI_CLK_250, PLL0,
			CSYS_CLK_166, PLL0},

        [CLK_CFG3] = {  PLL_CFG_1300_800_500_800,
                        ARM_CLK_650, PLL0,
                        AXI_CLK_200, PLL1,
                        DDR_CLK_400, PLL3,
                        IPSEC_CLK_250, PLL2,
                        SATA_OOB_CLK_125, PLL2,
                        SATA_PMU_CLK_25, PLL2,
                        DECT_CLK_250, PLL2,
                        L2CC_CLK_325, PLL0,
                        HFE_CLK_400, PLL1,
                        GEMTX_CLK_125, PLL2,
                        EXTPHY0_CLK_125, PLL2,
                        EXTPHY1_CLK_125, PLL2,
                        EXTPHY2_CLK_125, PLL2,
			TPI_CLK_250, PLL0,
			CSYS_CLK_166, PLL0},

        [CLK_CFG4] = {  PLL_CFG_1300_800_500_1066,
                        ARM_CLK_650, PLL0,
                        AXI_CLK_200, PLL1,
                        DDR_CLK_533, PLL3,
                        IPSEC_CLK_250, PLL2,
                        SATA_OOB_CLK_125, PLL2,
                        SATA_PMU_CLK_25, PLL2,
                        DECT_CLK_250, PLL2,
                        L2CC_CLK_325, PLL0,
                        HFE_CLK_400, PLL1,
                        GEMTX_CLK_125, PLL2,
                        EXTPHY0_CLK_125, PLL2,
                        EXTPHY1_CLK_125, PLL2,
                        EXTPHY2_CLK_125, PLL2,
			TPI_CLK_250, PLL0,
			CSYS_CLK_166, PLL0},

        [CLK_CFG5] = {  PLL_CFG_1800_800_500_1066,
                        ARM_CLK_900, PLL0,
                        AXI_CLK_200, PLL1,
                        DDR_CLK_533, PLL3,
                        IPSEC_CLK_250, PLL2,
                        SATA_OOB_CLK_125, PLL2,
                        SATA_PMU_CLK_25, PLL2,
                        DECT_CLK_250, PLL2,
                        L2CC_CLK_450, PLL0,
                        HFE_CLK_400, PLL1,
                        GEMTX_CLK_125, PLL2,
                        EXTPHY0_CLK_125, PLL2,
                        EXTPHY1_CLK_125, PLL2,
                        EXTPHY2_CLK_125, PLL2,
			TPI_CLK_250, PLL0,
			CSYS_CLK_166, PLL0},

        [CLK_CFG6] = { PLL_CFG_2400_1000_1500_1066,
                        ARM_CLK_1200, PLL0,
                        AXI_CLK_250, PLL1,
                        DDR_CLK_533, PLL3,
                        IPSEC_CLK_300, PLL2,
                        SATA_OOB_CLK_125, PLL1,
                        SATA_PMU_CLK_30, PLL0,
                        DECT_CLK_250, PLL2,
                        L2CC_CLK_600, PLL0,
                        HFE_CLK_500, PLL1,
                        GEMTX_CLK_125, PLL2,
                        EXTPHY0_CLK_125, PLL2,
                        EXTPHY1_CLK_125, PLL2,
                        EXTPHY2_CLK_125, PLL2,
						TPI_CLK_250, PLL0,
						CSYS_CLK_166, PLL0},
};

static inline void HAL_set_pll(u32 pll_no, u32 pll_freq_idx)
{
	u32 m = 0;
	u32 p = 0;
	u32 s = 0;
	u32 k;
	u32 vsel = 0;
	u32 val;

	if (pll_no < PLL3)
	{
		m = pll_table[pll_freq_idx].m;
		p = pll_table[pll_freq_idx].p;
		s = pll_table[pll_freq_idx].s;
		vsel = pll_table[pll_freq_idx].vsel;

		switch (pll_no)
		{
		case PLL0:
			writel((m & 0xFF),   PLL0_M_LSB);
			writel((m >> 8) & 0x3,   PLL0_M_MSB);
			writel(p,  PLL0_P);
			writel(s,  PLL0_S);
			val = readl(PLL0_CNTRL);
			val &= ~(1 << 6); //clear the vsel bit
			val |= PLL_LOCK_EN | PLL_RESET | (vsel << 6); //configure for vsel bit
			writel(val, PLL0_CNTRL);
			break;

		case PLL1:
			writel(m & 0xFF,   PLL1_M_LSB);
			writel((m >> 8) & 0x3,   PLL1_M_MSB);
			writel(p,  PLL1_P);
			writel(s,  PLL1_S);
			val = readl(PLL1_CNTRL);
			val &= ~(1 << 6);
			val |= PLL_LOCK_EN | PLL_RESET | (vsel << 6);
			writel(val, PLL1_CNTRL);
			break;

		case PLL2:
			writel(m & 0xFF,   PLL2_M_LSB);
			writel((m >> 8) & 0x3,   PLL2_M_MSB);
			writel(p,  PLL2_P);
			writel(s,  PLL2_S);
			val = readl(PLL2_CNTRL);
			val &= ~(1 << 6);
			val |= PLL_LOCK_EN | PLL_RESET | (vsel << 6);
			writel(val, PLL2_CNTRL);
			break;

		default:
			break;
		}
	}
	else if (pll_no == PLL3)
	{
		m = pll3_table[pll_freq_idx].m;
		p = pll3_table[pll_freq_idx].p;
		s = pll3_table[pll_freq_idx].s;
		k = pll3_table[pll_freq_idx].k;
		vsel = pll3_table[pll_freq_idx].vsel;

		writel(m & 0xFF,   PLL3_M_LSB);
		writel((m >> 0x8) & 0x1,   PLL3_M_MSB);
		writel(p,  PLL3_P);
		writel(s,  PLL3_S);
		writel(0x0, PLL3_DITHER_CNTRL);
		writel(k & 0xff, PLL3_K_LSB);
		writel((k >> 8) & 0xf, PLL3_K_MSB);
		writel(0x1, PLL3_MFR);
		writel(0x7, PLL3_MRR);
		val = readl(PLL3_CNTRL);
		val &= ~(1 << 6);
		val |= PLL_LOCK_EN | PLL_RESET | (vsel << 6);
		writel(val, PLL3_CNTRL);
	}
}

static inline void HAL_set_clock_pll_source(void)
{
	u32 val;

	val = readl(A9DP_CLK_CNTRL);
	val &=~(CLK_PLL_SRC_MASK << CLK_PLL_SRC_SHIFT);
	val |= (cfg_clk->arm_clk_src << 1);
	writel(val , A9DP_CLK_CNTRL);

	val = readl(AXI_CLK_CNTRL_0);
	val &=~(CLK_PLL_SRC_MASK << CLK_PLL_SRC_SHIFT);
	val |= (cfg_clk->axi_clk_src << 1);
	writel(val, AXI_CLK_CNTRL_0);

	val = readl(DDR_CLK_CNTRL);
	val &=~(CLK_PLL_SRC_MASK << CLK_PLL_SRC_SHIFT);
	val |= (cfg_clk->ddr_clk_src << 1);
	writel(val, DDR_CLK_CNTRL);

	val = readl(IPSEC_CLK_CNTRL);
	val &=~(CLK_PLL_SRC_MASK << CLK_PLL_SRC_SHIFT);
	val |= (cfg_clk->ipsec_clk_src << 1);
	writel(val, IPSEC_CLK_CNTRL);

	val = readl(SATA_OOB_CLK_CNTRL);
	val &=~(CLK_PLL_SRC_MASK << CLK_PLL_SRC_SHIFT);
	val |= (cfg_clk->sata_oob_clk_src << 1);
	writel(val, SATA_OOB_CLK_CNTRL);

	val = readl(SATA_PMU_CLK_CNTRL);
	val &=~(CLK_PLL_SRC_MASK << CLK_PLL_SRC_SHIFT);
	val |=  (cfg_clk->sata_pmu_clk_src << 1);
	writel(val, SATA_PMU_CLK_CNTRL);

	val = readl(DECT_CLK_CNTRL);
	val &=~(CLK_PLL_SRC_MASK << CLK_PLL_SRC_SHIFT);
	val |= (cfg_clk->dect_clk_src << 1);
	writel(val, DECT_CLK_CNTRL);

	val = readl(L2CC_CLK_CNTRL);
	val &=~(CLK_PLL_SRC_MASK << CLK_PLL_SRC_SHIFT);
	val |= (cfg_clk->l2cc_clk_src << 1);
	writel(val, L2CC_CLK_CNTRL);

	val = readl(HFE_CLK_CNTRL);
	val &=~(CLK_PLL_SRC_MASK << CLK_PLL_SRC_SHIFT);
	val |= (cfg_clk->hfe_clk_src << 1);
	writel(val, HFE_CLK_CNTRL);

	val = readl(GEMTX_CLK_CNTRL);
	val &=~(CLK_PLL_SRC_MASK << CLK_PLL_SRC_SHIFT);
	val |= (cfg_clk->gemtx_clk_src << 1);
	writel(val, GEMTX_CLK_CNTRL);

	val = readl(EXTPHY0_CLK_CNTRL);
	val &=~(CLK_PLL_SRC_MASK << CLK_PLL_SRC_SHIFT);
	val |= (cfg_clk->extphy0_clk_src << 1);
	writel(val, EXTPHY0_CLK_CNTRL);

	val = readl(EXTPHY1_CLK_CNTRL);
	val &=~(CLK_PLL_SRC_MASK << CLK_PLL_SRC_SHIFT);
	val |= (cfg_clk->extphy1_clk_src << 1);
	writel(val, EXTPHY1_CLK_CNTRL);

	val = readl(EXTPHY2_CLK_CNTRL);
	val &=~(CLK_PLL_SRC_MASK << CLK_PLL_SRC_SHIFT);
	val |= (cfg_clk->extphy2_clk_src << 1);
	writel(val, EXTPHY2_CLK_CNTRL);

	val = readl(TPI_CLK_CNTRL);
	val &=~(CLK_PLL_SRC_MASK << CLK_PLL_SRC_SHIFT);
	val |= (cfg_clk->tpi_clk_src << 1);
	writel(val, TPI_CLK_CNTRL);

	val = readl(CSYS_CLK_CNTRL);
	val &=~(CLK_PLL_SRC_MASK << CLK_PLL_SRC_SHIFT);
	val |= (cfg_clk->csys_clk_src << 1);
	writel(val, CSYS_CLK_CNTRL);
}

static inline void HAL_set_clock(void)
{
	HAL_set_clock_divider(cfg_clk->axi_clk, cfg_clk->axi_clk_src, AXI_CLK_DIV_CNTRL);
	HAL_set_clock_divider(cfg_clk->ddr_clk, cfg_clk->ddr_clk_src, DDR_CLK_DIV_CNTRL);
	HAL_set_clock_divider(cfg_clk->hfe_clk, cfg_clk->hfe_clk_src, HFE_CLK_DIV_CNTRL);
	HAL_set_clock_divider(cfg_clk->gemtx_clk, cfg_clk->gemtx_clk_src, GEMTX_CLK_DIV_CNTRL);
	HAL_set_clock_divider(cfg_clk->ipsec_clk, cfg_clk->ipsec_clk_src, IPSEC_CLK_DIV_CNTRL);
	HAL_set_clock_divider(cfg_clk->extphy0_clk, cfg_clk->extphy0_clk_src, EXTPHY0_CLK_DIV_CNTRL);
	HAL_set_clock_divider(cfg_clk->extphy1_clk, cfg_clk->extphy1_clk_src, EXTPHY1_CLK_DIV_CNTRL);
	HAL_set_clock_divider(cfg_clk->extphy2_clk, cfg_clk->extphy2_clk_src, EXTPHY2_CLK_DIV_CNTRL);
	HAL_set_clock_divider(cfg_clk->sata_oob_clk, cfg_clk->sata_oob_clk_src, SATA_OOB_CLK_DIV_CNTRL);
	HAL_set_clock_divider(cfg_clk->sata_pmu_clk, cfg_clk->sata_pmu_clk_src, SATA_PMU_CLK_DIV_CNTRL);
	HAL_set_clock_divider(cfg_clk->dect_clk, cfg_clk->dect_clk_src, DECT_CLK_DIV_CNTRL);
	HAL_set_clock_divider(cfg_clk->l2cc_clk, cfg_clk->l2cc_clk_src, L2CC_CLK_DIV_CNTRL);
	HAL_set_clock_divider(cfg_clk->arm_clk, cfg_clk->arm_clk_src, A9DP_CLK_DIV_CNTRL);
	HAL_set_clock_divider(cfg_clk->tpi_clk, cfg_clk->tpi_clk_src, TPI_CLK_DIV_CNTRL);
	HAL_set_clock_divider(cfg_clk->csys_clk, cfg_clk->csys_clk_src, CSYS_CLK_DIV_CNTRL);
}



static inline void HAL_set_clock_divider(int clk_freq, int pll_src, u32 reg)
{
	u32 pll_freq;
	int pll_freq_idx;
	int divider;
	u32 val;

	if (pll_src < PLL3)
	{
		switch(pll_src)
		{
		case PLL0:
			pll_freq_idx = pll_freq_table[cfg_clk->pll_cfg_idx].pll0_freq_idx;
			break;

		case PLL1:
			pll_freq_idx = pll_freq_table[cfg_clk->pll_cfg_idx].pll1_freq_idx;
			break;

		case PLL2:
			pll_freq_idx = pll_freq_table[cfg_clk->pll_cfg_idx].pll2_freq_idx;
			break;

		default:
			pll_freq_idx = pll_freq_table[cfg_clk->pll_cfg_idx].pll0_freq_idx;
			break;
		}

		pll_freq = pll_cfg_freq_table[pll_freq_idx];
	}
	else
	{
		pll_freq_idx = pll_freq_table[cfg_clk->pll_cfg_idx].pll3_freq_idx;
		pll_freq = pll3_cfg_freq_table[pll_freq_idx];
	}

	divider = pll_freq / clk_freq;

	/* Write the clock divider value and backup the divider bypass value in IRAM */
	/* This is to workaround an hardware bug where the divider bypass value read is not working */
	/* The global bypass is still overriding the individual clock bypasses */
	if (divider == 1)
	{
		write_clk_div_bypass_backup(CLK_DIV_BYPASS, reg);
	}
	else
	{
		write_clk_div_bypass_backup(0, reg);

		val = readl(reg);
		val &= ~0x1f;
		val |= divider;
		writel(val, reg);
	}
}

static inline void HAL_clock_remove_reg_bypass(u32 reg)
{
	u32 val;

	/* Remove individual clock bypass, based on bypass value backed up in IRAM */
	/* The global bypass is still overriding the individual clock bypasses */
	if (!(read_clk_div_bypass_backup(reg) & CLK_DIV_BYPASS))
	{
		val = readl(reg);
		val &= ~(1 << 7);
		writel(val, reg);
	}
}

static inline void HAL_clock_remove_bypass(void)
{
	HAL_clock_remove_reg_bypass(AXI_CLK_DIV_CNTRL);
	HAL_clock_remove_reg_bypass(HFE_CLK_DIV_CNTRL);
	HAL_clock_remove_reg_bypass(DDR_CLK_DIV_CNTRL);
	HAL_clock_remove_reg_bypass(GEMTX_CLK_DIV_CNTRL);
	HAL_clock_remove_reg_bypass(EXTPHY0_CLK_DIV_CNTRL);
	HAL_clock_remove_reg_bypass(EXTPHY1_CLK_DIV_CNTRL);
	HAL_clock_remove_reg_bypass(EXTPHY2_CLK_DIV_CNTRL);
	HAL_clock_remove_reg_bypass(DECT_CLK_DIV_CNTRL);
	HAL_clock_remove_reg_bypass(IPSEC_CLK_DIV_CNTRL);
	HAL_clock_remove_reg_bypass(SATA_OOB_CLK_DIV_CNTRL);
	HAL_clock_remove_reg_bypass(SATA_PMU_CLK_DIV_CNTRL);
	HAL_clock_remove_reg_bypass(L2CC_CLK_DIV_CNTRL);
	HAL_clock_remove_reg_bypass(A9DP_CLK_DIV_CNTRL);

	//TBD for TPI, NTG
}

static inline void HAL_bypass_all(void)
{
	//PLL Output Clock is bypassed using Reference Clock
	writel(0xf, PLLS_GLOBAL_CNTRL);

	writel(CLK_DIV_BYPASS|CLK_DIV_VAL_DEFAULT, A9DP_CLK_DIV_CNTRL);
	writel(CLK_DIV_BYPASS|CLK_DIV_VAL_DEFAULT, L2CC_CLK_DIV_CNTRL);
	writel(CLK_DIV_BYPASS|CLK_DIV_VAL_DEFAULT, AXI_CLK_DIV_CNTRL);
	writel(CLK_DIV_BYPASS|CLK_DIV_VAL_DEFAULT, DDR_CLK_DIV_CNTRL);
	writel(CLK_DIV_BYPASS|CLK_DIV_VAL_DEFAULT, HFE_CLK_DIV_CNTRL);
	writel(CLK_DIV_BYPASS|CLK_DIV_VAL_DEFAULT, GEMTX_CLK_DIV_CNTRL);
	writel(CLK_DIV_BYPASS|CLK_DIV_VAL_DEFAULT, SATA_PMU_CLK_DIV_CNTRL);
	writel(CLK_DIV_BYPASS|CLK_DIV_VAL_DEFAULT, SATA_OOB_CLK_DIV_CNTRL);

	writel(0x210000, USB0_PHY_CTRL_REG0);
	writel(0x210000, USB1_PHY_CTRL_REG0);
	writel(0x22, USB_RST_CNTRL);

	//PLL is put in Reset
	writel(PLL_RESET|PLL_LOCK_EN, PLL0_CNTRL);
	writel(PLL_RESET|PLL_LOCK_EN, PLL1_CNTRL);
	writel(PLL_RESET|PLL_LOCK_EN, PLL2_CNTRL);
	writel(PLL_RESET|PLL_LOCK_EN, PLL3_CNTRL);
}

static inline void HAL_unreset_pll(void)
{
	int val;

        //PLL taken out of Reset
	val = readl(PLL0_CNTRL);
	val &= ~PLL_RESET;
        writel(val, PLL0_CNTRL);

	val = readl(PLL1_CNTRL);
	val &= ~PLL_RESET;
        writel(val, PLL1_CNTRL);

	val = readl(PLL2_CNTRL);
	val &= ~PLL_RESET;
        writel(val, PLL2_CNTRL);

	val = readl(PLL3_CNTRL);
	val &= ~PLL_RESET;
        writel(val, PLL3_CNTRL);
}

static inline void HAL_wait_pll_lock(void)
{
	int p0, p1, p2;
        volatile u32 delay_count;

        //wait more than 750 uS for PLL3 to get locked
        delay_count = 0x9000;
        while (delay_count--);

         //clear status bit
        do {
                p0 = readl(PLL0_STATUS);

                p1 = readl(PLL1_STATUS);

                p2 = readl(PLL2_STATUS);

        }
        while(!p0 || !p1 || !p2);
}

#define	GLB_CLK_ENABLE	1
#define	GLB_CLK_SHIFT	0

#define PFE_SYS_AXI_RESET_BIT           (1<<3)

static inline void HAL_unreset_blocks(void)
{
	unsigned int val = 0;

	val = readl(AXI_RESET_1);
	val &= PFE_SYS_AXI_RESET_BIT; //skip HFE

	writel(0, AXI_RESET_0);
	writel(val, AXI_RESET_1);
	writel(0, AXI_RESET_2);
}

static inline HAL_hfe_unreset(void)
{
	unsigned int val = 0;
        volatile u32 delay_count;

	//set bit[0] in register 0x904B0100 to '0'
	val = readl(HFE_CLK_CNTRL);
	val &=((~GLB_CLK_ENABLE) << GLB_CLK_SHIFT);
	writel(val, HFE_CLK_CNTRL);

	//count 50 uS
        delay_count = 0x40 * 50;
        while (delay_count--);

	//Take HFE out of reset
	writel(readl(AXI_RESET_1) & ~PFE_SYS_AXI_RESET_BIT, AXI_RESET_1);

	//writing '1' to bit [0] of register 0x904B0100 to Power up HFE clock
	val = readl(HFE_CLK_CNTRL);
	val |=(GLB_CLK_ENABLE << GLB_CLK_SHIFT);
	writel(val, HFE_CLK_CNTRL);
}

void SoC_PLL_init(void)
{
        volatile u32 delay_count;

	cfg_clk = &clk_cfg_table[CLK_CFG];

	HAL_bypass_all();

	//configure PLL0, PLL1, PLL2 and PLL3
	HAL_set_pll(PLL0, pll_freq_table[cfg_clk->pll_cfg_idx].pll0_freq_idx);
	HAL_set_pll(PLL1, pll_freq_table[cfg_clk->pll_cfg_idx].pll1_freq_idx);
	HAL_set_pll(PLL2, pll_freq_table[cfg_clk->pll_cfg_idx].pll2_freq_idx);
	HAL_set_pll(PLL3, pll_freq_table[cfg_clk->pll_cfg_idx].pll3_freq_idx);

	//count 1 uS
        delay_count = 0x40;
        while (delay_count--);

	//bring PLL out of reset
	HAL_unreset_pll();

	//set the Global Bypass bit 
	writel(0xd1, GNRL_CLK_CNTRL_1);

	//configure the PLL source for all the clocks
	HAL_set_clock_pll_source();

	//configure all the clocks
	HAL_set_clock();

	//clock divider resync
	writel(0x80, DEVICE_RST_CNTRL);

	HAL_wait_pll_lock();

	HAL_clock_remove_bypass();

	//Global Bypass is off
	writel(0xd0, GNRL_CLK_CNTRL_1);

	//switch to PLL Clock
	writel(0, PLLS_GLOBAL_CNTRL);

	//bring HFE out of reset
	HAL_hfe_unreset();

	//bring all the block out of reset
	HAL_unreset_blocks();
}

#endif

u32 HAL_get_pll_freq(int pll_no)
{
	u32 p;
	u32 od;
	u32 m;
	u32 k;
	u32 s;
	u32 pll_clk = 0;

	if (pll_no < PLL3)
	{
		//get NF, NR and OD values
		switch (pll_no)
		{
		case PLL0:
			m = readl(PLL0_M_LSB) & 0xff;
			m |= (readl(PLL0_M_MSB) & 0x3) << 8;
			p = readl(PLL0_P) & 0x3f;
			s = readl(PLL0_S) & 0x7;
			od = (1 << s); // 2^s;
			break;

		case PLL1:
			m = readl(PLL1_M_LSB) & 0xff;
			m |= (readl(PLL1_M_MSB) & 0x3) << 8;
			p = readl(PLL1_P) & 0x3f;
			s = readl(PLL1_S) & 0x7;
			od = (1 << s);
			break;

		case PLL2:
			m = readl(PLL2_M_LSB) & 0xff;
			m |= (readl(PLL2_M_MSB) & 0x3) << 8;
			p = readl(PLL2_P) & 0x3f;
			s = readl(PLL2_S) & 0x7;
			od = (1 << s);
			break;

		default:
			return 0;
			break;
		}

		/* Ref Clock divided by 1000000. It should be displayed in MHz. */
		pll_clk = ((CFG_REFCLKFREQ / 1000000) * m) / p / od ;
	}
	else if (pll_no == PLL3)
	{
		m = readl(PLL3_M_LSB) & 0xff;
		m |= (readl(PLL3_M_MSB) & 0x3) << 8;
		p = readl(PLL3_P) & 0x3f;
		s = readl(PLL3_S) & 0x7;
		k = readl(PLL3_K_LSB) & 0xff;
		k |= (readl(PLL3_K_MSB) & 0xf) << 8;
		od = (1 << s);

		pll_clk = (((CFG_REFCLKFREQ / 1000000) * (m * 1024 + k)) / p / od + 1023) / 1024;
	}

	return pll_clk;
}

static inline u32 HAL_get_clk_freq(u32 ctrl_reg, u32 div_reg)
{
	u32 pll_src;
	u32 pll_clk;
	u32 clk_div;
	u32 clk_out;
	int bypass = 0;

	//get PLL source
	pll_src = readl(ctrl_reg);
	pll_src = (pll_src >> CLK_PLL_SRC_SHIFT) & CLK_PLL_SRC_MASK;

	//get clock divider bypass value from IRAM Clock Divider registers mirror location
	clk_div = read_clk_div_bypass_backup(div_reg);

	if (clk_div & CLK_DIV_BYPASS)
		bypass = 1;
	else
	{
		clk_div = readl(div_reg);
		clk_div &= 0x1f;
	}

	pll_clk = HAL_get_pll_freq(pll_src);

	if (bypass)
		clk_out = pll_clk;
	else
		clk_out = pll_clk / clk_div;

	return clk_out;
}

u32 HAL_get_arm_clk(void)
{
	return HAL_get_clk_freq(A9DP_CLK_CNTRL, A9DP_CLK_DIV_CNTRL);
}

u32 HAL_get_axi_clk(void)
{
	return HAL_get_clk_freq(AXI_CLK_CNTRL_0, AXI_CLK_DIV_CNTRL);
}

u32 HAL_get_ddr_clk(void)
{
	return HAL_get_clk_freq(DDR_CLK_CNTRL, DDR_CLK_DIV_CNTRL);
}

u32 HAL_get_ipsec_clk(void)
{
	return HAL_get_clk_freq(IPSEC_CLK_CNTRL, IPSEC_CLK_DIV_CNTRL);
}


u32 HAL_get_arm_peri_clk(void)
{
        u32 clk = HAL_get_arm_clk() / 4 ;

        return clk;
}

/*
 * Get the reference clock after reading bootstrap
 */
u32 HAL_get_ref_clk (void)
{
        unsigned long clock_freq = 0;
        unsigned int boot_strap, tmp;

        boot_strap = readl(COMCERTO_GPIO_SYSTEM_CONFIG);
        tmp = (boot_strap & GPIO_SYS_PLL_REF_CLK_MASK) >> GPIO_SYS_PLL_REF_CLK_SHIFT;

        if ( USB_XTAL_REF_CLK == tmp )
        {
                        if ( boot_strap & GPIO_USB_OSC_PAD_MASK )
                                clock_freq = CFG_REFCLKFREQ_24;
                        else
                                clock_freq = CFG_REFCLKFREQ_48;
        }
        else if ( SERDES_XTAL_REF_CLK == tmp )
        {
                if ( boot_strap & GPIO_SERDES_OSC_PAD_MASK )
                        clock_freq = CFG_REFCLKFREQ_24;
                else
                        clock_freq = CFG_REFCLKFREQ_48;
        }

        return (clock_freq/1000000);
}

