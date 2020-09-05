#include <common.h>
#include <asm/io.h>
#include <mach/clkcore.h>

/*
 *  * Reset the cpu through the reset controller
 *   */
void __noreturn reset_cpu (unsigned long addr)
{
	 /* Soft reset */
	writel(0x1, DEVICE_RST_CNTRL);

        while (1);
}
EXPORT_SYMBOL(reset_cpu);

