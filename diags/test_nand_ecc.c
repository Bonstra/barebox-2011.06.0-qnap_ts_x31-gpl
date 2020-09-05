#include <common.h>
#include "tests.h"
#include <diags.h>
#include <fs.h>
#include <errno.h>
#include <asm-generic/sections.h>

uint32_t const TEST_PTRN_SIZE =	0xA00000; /* 10MB */
uint32_t const TEST_PTRN_SEED = 0x10;
uint32_t const DDR_PAD_SIZE = 0x10;
uint32_t const PTRN_START_DDR_ADDR = 0x1600000;
uint32_t const PTRN_START_NAND_OFFSET = 0x0;

extern int update_nand(ulong src, ulong offset, ulong count);
extern int read_nand(ulong src, ulong offset, ulong count);

void ddr_incr_pattern(uint32_t * test_address, unsigned long max_size)
{
        unsigned long mem_offset = 0;
        unsigned long nWords = max_size / sizeof(test_address);

       // Fill memory with incremental pattern.
        for (mem_offset = 0; mem_offset < nWords; mem_offset++)
        {
                *(test_address + mem_offset) = TEST_PTRN_SEED + mem_offset;
        }


}

int do_complete_nand_test(void)
{

	printf("NAND test Started\n");
	printf("Writing incremental test pattern to DDR location 0x%x \n",  PTRN_START_DDR_ADDR);
	ddr_incr_pattern((uint32_t *) PTRN_START_DDR_ADDR, TEST_PTRN_SIZE);

	if (update_nand(PTRN_START_DDR_ADDR, PTRN_START_NAND_OFFSET, TEST_PTRN_SIZE)) {
		printf("ERR: NAND Update FAIL !!\n");
		return -1;
	} else {
		printf("NAND Update DONE.\n");
	}

	printf("Reading the test pattern bytes from NAND to DDR location 0x%x \n", (PTRN_START_DDR_ADDR + TEST_PTRN_SIZE + DDR_PAD_SIZE));
	read_nand((PTRN_START_DDR_ADDR + TEST_PTRN_SIZE + DDR_PAD_SIZE), PTRN_START_NAND_OFFSET, TEST_PTRN_SIZE);

	printf("Comparing test pattern bytes...\n");
	if(!memcmp((const void *) PTRN_START_DDR_ADDR, (const void *) (PTRN_START_DDR_ADDR + TEST_PTRN_SIZE + DDR_PAD_SIZE), TEST_PTRN_SIZE)) {
		printf("NAND Test Passed.\n");
	} else {
		printf("NAND Test Failed.\n");
		return -1;
	}

	return 0;
}




int Comcerto_nand_ecc_test (struct diags_test_param *p)
{
	int ret_val = 0;

	if (! do_complete_nand_test())
		ret_val = 0;
	else
		ret_val = -1;

	return ret_val;
}

