#include "tests.h"
#include "dump.h"
#include <diags.h>
#include <common.h>
#include <init.h>
#include <asm/io.h>
#include <asm/mmu.h>

#define SCU_BASE_ADDR	0xFFF00000

#define TTB_BASE	0x4000

#define PMD_SEC_TEX2	 (1 << 14)
#define COHERENT_SECTION	(PMD_SECT_AP_WRITE | PMD_SECT_XN | PMD_SECT_S | PMD_SECT_BUFFERABLE | PMD_SEC_TEX2 | PMD_TYPE_SECT)

#define COHE_TEST_AREA	0x00600000
#define COHE_TEST_LOC	0x00600010	

#define A9DP_CPU_RESET  0x904b0078
#define NEON1_RESET 0x8 /* When Set to 1, A9DP NEON1 is in Reset. */
#define CPU1_RESET 0x4 /* When Set to 1, A9DP CPU1 is in Reset.  */

#define CPU_SYNC_LOC	0x8300ff04
#define CPU0_SYNC_VAL0	0xdeaddead
#define CPU0_SYNC_VAL1	0xdeadbeef
#define CPU1_SYNC_VAL0	0xbeefbeef
#define CPU0_SYNC_VAL_COHERENT	0xbeefdead
#define CPU0_SYNC_VAL_COHERENT2	0xbeefdddd

#define COHE_PATTERN0	0xa1a1b1b1
#define COHE_PATTERN1	0xaaaabbbb
#define COHE_PATTERN2	0xbbbbcccc
#define COHE_PATTERN3	0xccccdddd

#define DEBUG_LOC	0x8300ff0c
#define PATTERN_TEMP1	0xdead5555
#define PATTERN_TEMP2	0xdead6666
#define PATTERN_PASS	0xdeadaaaa
#define PATTERN_FAIL	0xdeadffff

static unsigned long *ttb;

static inline void set_marker(int mark)
{
	writel(mark, 0x8300ff0c);
}

void cpu_reset_1()
{
        unsigned int val;

	val = readl(A9DP_CPU_RESET);
	val &= ~(NEON1_RESET | CPU1_RESET);
	writel(val, A9DP_CPU_RESET);
	
}

void smp_mode_enable(void) {

        unsigned long val;

	//ENable the coherency bit
	asm("mrc p15, 0, %0, c1, c0, 0  @ get CR" : "=r" (val) : : "cc");
	val |= (1 << 6);
	asm volatile("mcr p15, 0, %0, c1, c0, 0 @ set CR" : : "r" (val) : "cc");

}

void enable_SCU(void)
{
        unsigned long val;

	//Invalidate SCU Tag
	val = 0;
	writel(val, SCU_BASE_ADDR + 0xC);
	
	//Enable SCU
	val = readl(SCU_BASE_ADDR);
	val |= 0x1;
	writel(val, SCU_BASE_ADDR);
}


static void create_section_entry(unsigned long virt, unsigned long phys, int size_m,
                unsigned int flags)
{
        int i;

        phys >>= 20;
        virt >>= 20;

        for (i = size_m; i > 0; i--, virt++, phys++)
                ttb[virt] = (phys << 20) | flags;

}

static void setup_pagetable(void)
{
	ttb = (void *)TTB_BASE;
	
	create_section_entry(0x0, 0x0, 2, PMD_SECT_DEF_UNCACHED);
        create_section_entry(COHE_TEST_AREA, COHE_TEST_AREA, 1, COHERENT_SECTION);
        //IRAM section
        create_section_entry(0x83000000, 0x83000000, 1, PMD_SECT_DEF_UNCACHED);
        //SCU register section
        create_section_entry(SCU_BASE_ADDR, SCU_BASE_ADDR, 1, PMD_SECT_DEF_UNCACHED);
}

static void mmu_init_minimal(void)
{
        int i;

        ttb = (void *)TTB_BASE;

        /* Set the Domain Access Control Register */
        i = 0x3;
        asm volatile ("mcr  p15,0,%0,c3,c0,0" : : "r"(i) /*:*/);

        /* Set the ttb register */
        asm volatile ("mcr  p15,0,%0,c2,c0,0" : : "r"(ttb) /*:*/);

        //MMU Enable
	mmu_enable();
}

int Comcerto_dualcore_test (struct diags_test_param *p)
{
        unsigned int r = 0;
        volatile int *ptr = COHE_TEST_LOC;
        *ptr = COHE_PATTERN0;

	writel(CPU0_SYNC_VAL0, CPU_SYNC_LOC);

        cpu_reset_1();

        while(r != CPU1_SYNC_VAL0)
        {
		r = readl(CPU_SYNC_LOC);
        }


	writel(CPU0_SYNC_VAL1, CPU_SYNC_LOC);

	*(volatile int *)0x90470034 = 0x10;

	setup_pagetable();
	mmu_init_minimal();
        smp_mode_enable();
	enable_SCU();
	cpu0_coherency_test();

}

void cpu1_test(void)
{
        unsigned int r  = 0;

	writel(CPU1_SYNC_VAL0, CPU_SYNC_LOC);

        while(r != CPU0_SYNC_VAL1)
        {
		r = readl(CPU_SYNC_LOC);
        }


#define STACK_BASE_1 0x83005000
#define STACK_SIZE_1 0x1000
	/* Setup the stack */
	r = STACK_BASE_1 + STACK_SIZE_1 - 16;
	__asm__ __volatile__("mov sp, %0" : : "r"(r));


	setup_pagetable();
	mmu_init_minimal();
        smp_mode_enable();
	cpu1_coherency_test();
}

void cpu0_coherency_test()
{
	int timeout = 0;
	volatile int r = 0;

	writel(COHE_PATTERN1, COHE_TEST_LOC);

	writel(CPU0_SYNC_VAL_COHERENT, CPU_SYNC_LOC);

        while(r != CPU0_SYNC_VAL_COHERENT2)
        {
                r = readl(CPU_SYNC_LOC);
        }

	r = readl(COHE_TEST_LOC);

        if(r == COHE_PATTERN2)
        {
                set_marker(PATTERN_PASS);
        }
        else
                set_marker(PATTERN_FAIL);

}

void cpu1_coherency_test()
{
        volatile  int r;
	int timeout = 0;

        while(r != CPU0_SYNC_VAL_COHERENT)
        {
                r = readl(CPU_SYNC_LOC);
        }

	r = readl(COHE_TEST_LOC);

	if(r == COHE_PATTERN1)
	{
		set_marker(PATTERN_PASS);
	}
	else
		set_marker(PATTERN_FAIL);

	writel(COHE_PATTERN2, COHE_TEST_LOC);
	writel(CPU0_SYNC_VAL_COHERENT2, CPU_SYNC_LOC);

	while(1);
}
