#include <common.h>
#include <init.h>
#include <clock.h>
#include <asm/types.h>
#include <asm/io.h>
#include <mach/bits.h>
#include <mach/comcerto-2000.h>
#include <mach/a9_mpu.h>

#define MAX_TIMER_COUNT	0xffffffff
#define MHz	1024*1024

#define PERI_CLK (162* 1024 * 1024)

static uint64_t comcerto_clocksource_read (void)
{
        return (MAX_TIMER_COUNT - readl(COMCERTO_A9_TIMER_BASE + A9_TIMER_COUNTER));
}

static struct clocksource cs = {
        .read   = comcerto_clocksource_read,
        .mask   = CLOCKSOURCE_MASK(32),
        .shift  = 10,
};

static int clocksource_init(void)
{
	writel(MAX_TIMER_COUNT, COMCERTO_A9_TIMER_BASE + A9_TIMER_LOAD);
	writel(MAX_TIMER_COUNT, COMCERTO_A9_TIMER_BASE + A9_TIMER_COUNTER);
	writel(A9_TIMER_ENABLE | A9_TIMER_RELOAD, COMCERTO_A9_TIMER_BASE + A9_TIMER_CNTRL);
	cs.mult = clocksource_hz2mult(HAL_get_arm_peri_clk() * (1000 * 1000), cs.shift);
	init_clock(&cs);
	return 0;
}

core_initcall(clocksource_init);

