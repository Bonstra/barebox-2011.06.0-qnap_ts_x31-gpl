#include <common.h>
#include <command.h>
#include <asm/io.h>
#include <mach/ddr.h>

static int do_socinfo(struct command *cmdtp, int argc, char *argv[])
{
        int clk;
	int c2k_rev;
#if defined(CONFIG_COMCERTO_DDR_TRAINING)
	unsigned char wdqs[4];
	unsigned char rdqs[4];
#endif

	printf ("-----------------------------\n");
	printf ("\n%s\n", version_string);

	printf ("PLL0: %0d, PLL1: %0d, PLL2: %0d, PLL3: %0d MHz\n",
		HAL_get_pll_freq(0), HAL_get_pll_freq(1), HAL_get_pll_freq(2), HAL_get_pll_freq(3));

	clk = HAL_get_ref_clk(); 
	printf ("Ref Clock: %0d MHz\n", clk);

	clk = HAL_get_arm_clk(); 
	printf ("ARM Clock: %0d MHz\n", clk);

	clk = HAL_get_axi_clk();
	printf ("AXI Clock: %d MHz\n", clk);

	clk = HAL_get_ddr_clk();
	printf ("DDR Clock: %d MHz\n", clk);

	clk = HAL_get_ipsec_clk();
	printf ("IPSEC Clock: %d MHz\n", clk);

	c2k_rev = HAL_get_c2k_revision();

	if (c2k_rev <= 1)
		printf ("C2K Revision: %s\n", (c2k_rev == 0) ? "A0":"A1");
	else
		printf ("C2K Revision: %04d\n", c2k_rev);

	printf("DDR Width : %s\n", (is_16bitDDR_config() ? "16":"32")); 

	printf ("-----------------------------\n");

	return 0;
}

BAREBOX_CMD_START(socinfo)
        .cmd            = do_socinfo,
        .usage          = "print SoC info",
BAREBOX_CMD_END

