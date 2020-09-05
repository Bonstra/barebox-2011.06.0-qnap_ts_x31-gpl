#include <common.h>
#include <init.h>
#include <asm/io.h>
#include <asm/types.h>
#include <mach/clkcore.h>
#include <mach/ddr.h>


struct ddr_reg_val evm_ddrc_cfg[] = {DDRC_CFG};
struct ddr_reg_val evm_ddr_phy_cfg[] = {DDR_PHY_CFG};

void SoC_DDR_init(void)
{
	int i;
	int reg_count;
	int reg_phy_count;
        volatile u32 delay_count;

	writel(0x1, DDR_RESET); /* DDR controller out of reset and PHY is put into reset */

	reg_phy_count = sizeof(evm_ddr_phy_cfg)/sizeof(struct ddr_reg_val);

	for(i = 0 ; i < reg_phy_count; i++)
	{
		writel(evm_ddr_phy_cfg[i].val, evm_ddr_phy_cfg[i].reg);

		//wait for few us. VLSI has 5 NOPs here.
		delay_count = 0x1;
		while (delay_count--);
	}

	writel(0, DDR_RESET); /* DDR PHY out of reset */

	reg_count = sizeof(evm_ddrc_cfg)/sizeof(struct ddr_reg_val);

	for(i = 0 ; i < reg_count; i++)
	{
		writel(evm_ddrc_cfg[i].val, evm_ddrc_cfg[i].reg);
	}

	/* start the DDR Memory Controller */
	writel((MC_START | readl(DDRC_CTL_00_REG)), DDRC_CTL_00_REG);

	/* Wait for the Memory Controller to complete initialization */
	while (!(readl(DDRC_CTL_51_REG) & MC_INIT_STAT_MASK))
		;
}


