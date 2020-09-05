#include "tests.h"
#include "dump.h"
#include <diags.h>
#include <common.h>
#include <init.h>
#include <asm/io.h>
#include <asm/mmu.h>
#error
/*
 * Hardware page table definitions.
 *
 * + Level 1 descriptor (PMD)
 *   - common
 */
#define PMD_TYPE_MASK           (3 << 0)
#define PMD_TYPE_FAULT          (0 << 0)
#define PMD_TYPE_TABLE          (1 << 0)
#define PMD_TYPE_SECT           (2 << 0)
#define PMD_BIT4                (1 << 4)
#define PMD_DOMAIN(x)           ((x) << 5)
#define PMD_PROTECTION          (1 << 9)        /* v5 */
/*
 *   - section
 */
#define PMD_SECT_BUFFERABLE     (1 << 2)
#define PMD_SECT_CACHEABLE      (1 << 3)
#define PMD_SECT_XN             (1 << 4)        /* v6 */
#define PMD_SECT_AP_WRITE       (1 << 10)
#define PMD_SECT_AP_READ        (1 << 11)
#define PMD_SECT_TEX(x)         ((x) << 12)     /* v5 */
#define PMD_SECT_APX            (1 << 15)       /* v6 */
#define PMD_SECT_S              (1 << 16)       /* v6 */
#define PMD_SECT_nG             (1 << 17)       /* v6 */
#define PMD_SECT_SUPER          (1 << 18)       /* v6 */


#define PMD_SECT_DEF_UNCACHED (PMD_SECT_AP_WRITE | PMD_SECT_AP_READ | PMD_TYPE_SECT)
#define PMD_SECT_DEF_CACHED (PMD_SECT_WB | PMD_SECT_DEF_UNCACHED)

#define TEST_AREA	0x20000000

#define L2CC_BASE               0xFFF10000

#define L2_SIZE		256*1024
#define L2_WAY_SIZE	32768 //32*1024
#define CACHE_LINE_SIZE         32

#define L2CC_TEST_DESC_PATTERN_FILL	"L2CC Test to verify the cache fill"
#define L2CC_TEST_DESC_LOCKDOWN_WAY	"L2CC Test to verify lockdown by way"

extern void l2x0_enable(void);
extern void l2x0_latency( __u32 tag_lat, __u32 data_lat);
extern void l2x0_unlock_dc_all_way(void);
extern void l2x0_inv_all(void);
extern void l2x0_clean_all(void);
extern void l2x0_lockdown_dc_all_way(void);
extern void l2x0_lockdown_dc_lines(void);
extern void l2x0_unlock_dc_lines(void);
extern void l2x0_lockdown_dc_all_way_except_one(int way);
extern void l2x0_lock_lines_enable(void);
extern void l2x0_lock_lines_disable(void);
extern void l2x0_unlock_all_lines(void);


unsigned long *ttb;
static void mmu_init_minimal(void);
static void create_section_entry(unsigned long virt, unsigned long phys, int size_m,
                unsigned int flags);

static void fill_pattern(volatile unsigned int* ptr, int size, unsigned int pattern);
static void read_pattern(volatile unsigned int* ptr, int size);
static void read_pattern_cacheline(volatile unsigned int* ptr, int size);
static int verify_pattern(volatile unsigned int* ptr, int size, unsigned int pattern);

int l2cc_test_invalidate_by_way(unsigned int startaddr, int size);
int l2cc_test_invalidate_by_line(unsigned int startaddr, int size);
int l2cc_test_clean_by_way(unsigned int startaddr, int size);
int l2cc_test_clean_by_line(unsigned int startaddr, int size);
int l2cc_test_lockdown_way(unsigned int startaddr, unsigned int size);
int l2cc_test_lockdown_line(unsigned int startaddr, unsigned int size);

int Comcerto_l2cc_init(void)
{
	mmu_init_minimal();
	set_marker(0xdead4444);

	//L2 8-way associativity with 32KB way size
	l2x0_init((void *)L2CC_BASE, 0x00040000, 0x000F0000);
	set_marker(0xdead5555);

//	l2x0_latency(0x00000200, 0x00000200);
}

static void mmu_init_minimal(void)
{
	int i;

	ttb = (void *)0x100000;

        /* Set the ttb register */
        asm volatile ("mcr  p15,0,%0,c2,c0,0" : : "r"(ttb) /*:*/);

        /* Set the Domain Access Control Register */
        i = 0x3;
        asm volatile ("mcr  p15,0,%0,c3,c0,0" : : "r"(i) /*:*/);

	//First 2 MB of DDR
	create_section_entry(0x0, 0x0, 2, PMD_SECT_DEF_UNCACHED);

	//1MB of DDR Test Area for L2 at 0x20000000, only L2 cacheable
        create_section_entry(TEST_AREA, TEST_AREA, 1, PMD_SECT_DEF_UNCACHED | PMD_SECT_TEX(5));

	//IRAM section
	create_section_entry(0x83000000, 0x83000000, 1, PMD_SECT_DEF_UNCACHED);

	//L2 register section
	create_section_entry(0xFFF00000, 0xFFF00000, 1, PMD_SECT_DEF_UNCACHED);


	//MMU Enable
        asm volatile (
                "bl __mmu_cache_on;"
                :
                :
                : "r0", "r1", "r2", "r3", "r6", "r10", "r12", "cc", "memory"
        );

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

void l2_disable(void)
{
        writel(0, L2CC_BASE + 0x100);	
}


int Comcerto_l2cc_test (struct diags_test_param *p)
{
        int res = 0;
        unsigned int startaddr = (int)TEST_AREA; //p->params[0];
        unsigned int size = 0x40; //p->params[1];
        unsigned int flags = p->params[2];

	set_marker(0xdead6666);

        if(flags & L2CCTEST_INVALIDATE_BY_WAY)
        {
		res = l2cc_test_invalidate_by_way(startaddr, size);
	}
	set_marker(0xdead7777);

        if(flags & L2CCTEST_INVALIDATE_BY_LINE)
        {
		res = l2cc_test_invalidate_by_line(startaddr, size);
	}
	set_marker(0xdead8888);

        if(flags & L2CCTEST_CLEAN_BY_WAY)
        {
		res = l2cc_test_clean_by_way(startaddr, size);
	}
	set_marker(0xdead9999);

        if(flags & L2CCTEST_CLEAN_BY_LINE)
        {
		res = l2cc_test_clean_by_line(startaddr, size);
	}
	set_marker(0xdeadaaaa);

        if(flags & L2CCTEST_LOCK_BY_WAY)
        {
		res = l2cc_test_lockdown_way(startaddr, size);
	}
	set_marker(0xdeadbbbb);

        if(flags & L2CCTEST_LOCK_BY_LINE)
        {
		res = l2cc_test_lockdown_line(startaddr, size);
	}
	set_marker(0xdeadcccc);

	//MMU Disable
        asm volatile (
                "bl __mmu_cache_off;"
                :
                :
                : "r0", "r1", "r2", "r3", "r6", "r10", "r12", "cc", "memory"
        );


	//End of Test
	*(volatile int *)0x85ff0000 = 0x4;

	return res;
}


int l2cc_test_invalidate_by_way(unsigned int startaddr, int size)
{
        unsigned int* ptr ;
        unsigned int pattern1, pattern2;
	int res;

	l2x0_enable();
	l2x0_unlock_dc_all_way();
        l2x0_inv_all();
	l2_disable();

        pattern1 = 0xaaaaaaaa;
        ptr = (int *)startaddr ;
	fill_pattern (ptr, size, pattern1);

	l2x0_enable();
        ptr = (int *)startaddr ;
        read_pattern(ptr, size);


        l2_disable();
        pattern2 = 0xbbbbbbbb;
        ptr = (int *)startaddr ;
        fill_pattern (ptr, size, pattern2);

        l2x0_enable();
        ptr = (int *)startaddr ;
	res = verify_pattern(ptr, size, pattern1);
	if(res)
		set_result_marker(0xffff1111);

	l2x0_inv_all();

        ptr = (int *)startaddr ;
	res = verify_pattern(ptr, size, pattern2);
	if(res)
		set_result_marker(0xffff1111);
	else
		set_result_marker(0xaaaa1111);

	return res;
}


int l2cc_test_invalidate_by_line(unsigned int startaddr, int size)
{
        unsigned int* ptr ;
        unsigned int pattern1, pattern2;
        int res;

        l2x0_enable();
        l2x0_unlock_dc_all_way();
        l2x0_inv_all();
        l2_disable();

        pattern1 = 0xaaaaaaaa;
        ptr = (int *)startaddr ;
        fill_pattern (ptr, size, pattern1);

        l2x0_enable();
        ptr = (int *)startaddr ;
        read_pattern(ptr, size);


        l2_disable();
        pattern2 = 0xbbbbbbbb;
        ptr = (int *)startaddr ;
        fill_pattern (ptr, size, pattern2);

        l2x0_enable();
        ptr = (int *)startaddr ;
        res = verify_pattern(ptr, size, pattern1);
        if(res)
		set_result_marker(0xffff2222);

        outer_cache.inv_range(startaddr, startaddr + size);

        ptr = (int *)startaddr ;
        res = verify_pattern(ptr, size, pattern2);
        if(res)
		set_result_marker(0xffff2222);
        else
		set_result_marker(0xaaaa2222);

        return res;
}

int l2cc_test_clean_by_way(unsigned int startaddr, int size)
{
        unsigned int* ptr ;
        unsigned int pattern1, pattern2, pattern3;
        int res;

        l2x0_enable();
        l2x0_unlock_dc_all_way();
        l2x0_inv_all();
        l2_disable();

        pattern1 = 0xaaaaaaaa;
        ptr = (int *)startaddr ;
        fill_pattern (ptr, size, pattern1);

        l2x0_enable();
        ptr = (int *)startaddr ;
        read_pattern(ptr, size);

        l2_disable();
        pattern2 = 0xbbbbbbbb;
        ptr = (int *)startaddr ;
        fill_pattern (ptr, size, pattern2);

        l2x0_enable();
        ptr = (int *)startaddr ;
        res = verify_pattern(ptr, size, pattern1);
        if(res)
		set_result_marker(0xffff3333);

        //Make the cache lines dirty
        ptr = (int *)startaddr ;
        pattern3 = 0xaaaacccc;
        fill_pattern (ptr, size, pattern3);

        l2_disable();
        ptr = (int *)startaddr ;
        res = verify_pattern(ptr, size, pattern2);
        if(res)
		set_result_marker(0xffff3333);

        l2x0_enable();
	l2x0_clean_all();

        l2_disable();
        ptr = (int *)startaddr ;
        res = verify_pattern(ptr, size, pattern3);
        if(res)
		set_result_marker(0xffff3333);
        else
		set_result_marker(0xaaaa3333);

        return res;
}


int l2cc_test_clean_by_line(unsigned int startaddr, int size)
{
        unsigned int* ptr ;
        unsigned int pattern1, pattern2, pattern3;
	int res;

        l2x0_enable();
        l2x0_unlock_dc_all_way();
        l2x0_inv_all();
        l2_disable();

        pattern1 = 0xaaaaaaaa;
        ptr = (int *)startaddr ;
        fill_pattern (ptr, size, pattern1);

        l2x0_enable();
        ptr = (int *)startaddr ;
        read_pattern(ptr, size);

        l2_disable();
        pattern2 = 0xbbbbbbbb;
        ptr = (int *)startaddr ;
        fill_pattern (ptr, size, pattern2);

        l2x0_enable();
        ptr = (int *)startaddr ;
        res = verify_pattern(ptr, size, pattern1);
        if(res)
		set_result_marker(0xffff4444);

	//Make the cache lines dirty
        ptr = (int *)startaddr ;
	pattern3 = 0xaaaacccc;
        fill_pattern (ptr, size, pattern3);

        l2_disable();
        ptr = (int *)startaddr ;
        res = verify_pattern(ptr, size, pattern2);
        if(res)
		set_result_marker(0xffff4444);

        l2x0_enable();
	outer_cache.clean_range(startaddr, startaddr + size);

        l2_disable();
        ptr = (int *)startaddr ;
        res = verify_pattern(ptr, size, pattern3);
        if(res)
		set_result_marker(0xffff4444);
	else
		set_result_marker(0xaaaa4444);

        return res;
}


int l2cc_test_lockdown_way(unsigned int startaddr, unsigned int size)
{
        int res;
	unsigned int* ptr ;
        unsigned int pattern1, pattern2;

	l2x0_enable();
	l2x0_inv_all();
	l2_disable();


	//fill pattern1 in L3 mem 
	pattern1 = 0xaaaaaaaa;
	ptr = (int *)startaddr;
	fill_pattern (ptr, size, pattern1);

	//Lock all way except way 0
	l2x0_lockdown_dc_all_way_except_one(0);

	//Read the pattern into L2. This will be read into way 0
	l2x0_enable();	
	ptr = (int *)startaddr;		
	read_pattern(ptr, size);

	//Lock all the ways
	l2x0_lockdown_dc_all_way();

	//fill another pattern in L3 mem 
	l2_disable();
	pattern2 = 0xbbbbbbbb;
	ptr = (int *)startaddr ;
	fill_pattern (ptr, size, pattern2);

	//Read the region
	l2x0_enable();	
	ptr = (int *)startaddr ;
	read_pattern_cacheline(ptr, 2*L2_WAY_SIZE);

	//check that old pattern(pattern1) is locked in L2
	ptr = (int *)startaddr ;
	res = verify_pattern(ptr, size, pattern1);
	if(res == -1)
		set_result_marker(0xffff5555);

	//also verify that the locked line was never evicted
        l2_disable();
        ptr = (int *)startaddr ;
        res = verify_pattern(ptr, size, pattern2);

	l2x0_unlock_dc_all_way();

	if(res == -1)
		set_result_marker(0xffff5555);
	else
		set_result_marker(0xaaaa5555);
	

	return res;
}


int l2cc_test_lockdown_line(unsigned int startaddr, unsigned int size)
{
        int res;
        unsigned int* ptr ;
        unsigned int pattern1, pattern2;

        l2x0_enable();
        l2x0_inv_all();
        l2_disable();


        //fill pattern1 in L3 mem
        pattern1 = 0xaaaaaaaa;
        ptr = (int *)startaddr;
        fill_pattern (ptr, size, pattern1);

        //Lock all way except way 0
        l2x0_lockdown_dc_all_way_except_one(0);

        //Read the pattern into L2 with line lock enabled
        l2x0_enable();
        //Enable the lock of the lines for linefills
	l2x0_lock_lines_enable();
        ptr = (int *)startaddr;
        read_pattern(ptr, size);
	//Disable the lock of the lines for linefills
	l2x0_lock_lines_disable();

        //fill pattern2 in L3 mem
        l2_disable();
        pattern2 = 0xbbbbbbbb;
        ptr = (int *)startaddr ;
        fill_pattern (ptr, size, pattern2);

        //Read the region more than the way size
        l2x0_enable();
        ptr = (int *)startaddr ;
        read_pattern_cacheline(ptr, 2*L2_WAY_SIZE);

        //check that old pattern is locked in L2
        ptr = (int *)startaddr ;
        res = verify_pattern(ptr, size, pattern1);
        if(res == -1)
		set_result_marker(0xffff6666);

        //also verify that the locked line was never evicted
        l2_disable();
        ptr = (int *)startaddr ;
        res = verify_pattern(ptr, size, pattern2);

	l2x0_unlock_all_lines();

        if(res == -1)
		set_result_marker(0xffff6666);
        else
		set_result_marker(0xaaaa6666);


        return res;

}

static void fill_pattern(volatile unsigned int* ptr, int size, unsigned int pattern)
{
	int i;
	for(i=0;i <size ; i+=4) 
	{
		*ptr++ = pattern;	
	}
} 

static void read_pattern(volatile unsigned int* ptr, int size)
{
        int i;
        int val;
        for(i=0; i <size ; i+=4)
        {
                val =  *ptr;
		ptr++;
	}
}

static void read_pattern_cacheline(volatile unsigned int* ptr, int size)
{
        int i;
        int val;
        int ptr_skip = CACHE_LINE_SIZE/sizeof(int);
        for(i=0; i < size ; i+=CACHE_LINE_SIZE)
        {
                val =  *ptr;
                ptr += ptr_skip;
        }
}

static int verify_pattern(volatile unsigned int* ptr, int size, unsigned int pattern)
{
	int i;
	int val;

	for(i=0; i <size ; i+=4)
	{
		val =  *ptr;
		if (val != pattern) 
		{
#ifndef CONFIG_COMCERTO_VERIFICATION
			printf("L2<->DDR test: DATA RAM read fail at PA 0x%x\n",(unsigned int)ptr);
			printf("Expected Pattern: 0x%x\n", pattern);
			printf("Actual Pattern: 0x%x\n", val);
#endif
			return -1;
		}	
		ptr++;	
	}	
	return 0;
}

