#include <common.h>
#include <mach/clkcore.h>
#include <mach/ddr.h>
#include <mach/gpio.h>
#include <asm/io.h>
#include <mach/gpio.h>

#define CONFIG_GEM_RGMII_2_5V
//#define CONFIG_GEM_RGMII_3_3V
#define CONFIG_HFE_OVERDRIVE

/* Initialize pad config according to C2K_ChipDef_spec_010.doc */
void comcerto_pad_config()
{
	u32 val;

	/* HFE configurations */

	/* Memories EMA Config 1 */
	val = readl(COMCERTO_GPIO_MEM_EMA_CONFIG1);
	val &= ~((0x3 << 6)|(0x3 << 17));
	writel(val, COMCERTO_GPIO_MEM_EMA_CONFIG1);

#if defined(CONFIG_GEM_RGMII_3_3V)
	/*  Driving Strength of IO Pads */
	/*In case GEMX operates in RMII or RGMII @3.3V mode then need to adjust
	 *the driving strength of its IO pad (default value is for 2.5V) */
	val = readl(COMCERTO_GPIO_PAD_CONFIG3);
	val &= ~((0x3 << 6) | (0x3 << 12));
	val |= ((0x2 << 6) | (0x2 << 12));
	writel(val, COMCERTO_GPIO_PAD_CONFIG3);

	val = readl(COMCERTO_GPIO_PAD_CONFIG4);
	val &= ~((0x3 << 6)| (0x3 << 12));
	val |= ((0x2 << 6) | (0x2 << 12));
	writel(val, COMCERTO_GPIO_PAD_CONFIG4);

	val = readl(COMCERTO_GPIO_PAD_CONFIG5);
	val &= ~((0x3 << 6) | (0x3 << 12));
	val |= ((0x2 << 6) | (0x2 << 12));
	writel(val, COMCERTO_GPIO_PAD_CONFIG5);

#elif defined(CONFIG_GEM_RGMII_2_5V)
	/* RGMII Pad Compensation Logic (Tx side only)*/
	val = readl(COMCERTO_GPIO_PAD_CTRL);
	val &= ~((0x3 << 22) | (0x3 << 26) | (0x3 << 30));
	writel(val, COMCERTO_GPIO_PAD_CTRL);
#else
#error "Either CONFIG_GEM_RGMII_3_3V or CONFIG_GEM_RGMII_2_5V should be selected"
#endif
	/* Slew Rate Control of IO Pads */
	val = readl(COMCERTO_GPIO_PAD_CONFIG3);
	val |= ((0x1 << 11) | (0x1 << 17));
	writel(val, COMCERTO_GPIO_PAD_CONFIG3);

	val = readl(COMCERTO_GPIO_PAD_CONFIG4);
	val |= ((0x1 << 11) | (0x1 << 17));
	writel(val, COMCERTO_GPIO_PAD_CONFIG4);

	val = readl(COMCERTO_GPIO_PAD_CONFIG5);
	val |= ((0x1 << 11) | (0x1 << 17));
	writel(val, COMCERTO_GPIO_PAD_CONFIG5);

#if defined (CONFIG_HFE_OVERDRIVE)
	/* Memory Margin Bit setting */
	/* In case HFE block operates in overdrive (1.2V) mode then need to
	* modify few EMA fields related to HFE memories. The configuration
	* registers are in GPIO block */
	val = readl(COMCERTO_GPIO_MEM_EMA_CONFIG0);
	val &= ~(0xFFF << 18);
	val |= (0x38E << 18);
	writel(val, COMCERTO_GPIO_MEM_EMA_CONFIG0);

	val = readl(COMCERTO_GPIO_MEM_EMA_CONFIG1);
	val &= ~(0x3F << 19);
	val |= (0x8 << 18);
	writel(val, COMCERTO_GPIO_MEM_EMA_CONFIG1);
#endif

	/*TODO Add rest of the configurations here*/

}
