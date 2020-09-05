#include <common.h>
#include "tests.h"
#include <diags.h>
#include <fs.h>
#include <errno.h>
#include <asm-generic/sections.h>

#define PHYS_FLASH1                   0xC0000000      /* Flash Bank #1 */
#define PHYS_FLASH1_SECT_SIZE         0x00480000

// Pattern buffer size.
#define DIAGS_NOR_PTRN_BUF_SIZE		(PHYS_FLASH1_SECT_SIZE)

// NOR Usage Info.
#define NOR_SIZE	(((4 * 1024) + 512) * 1024)

// SDRAM location, to where the NOR instruction bytes to be copied from NOR.
#define DIAGS_NOR_INST_COPY_LOC         __bss_stop

// Test patterns.
#define TEST_PTRN_AA			0xAA
#define TEST_PTRN_55			0x55

// Buffer to hold the pattern test bytes.
unsigned char nor_tst_ptrn[DIAGS_NOR_PTRN_BUF_SIZE];

extern int update_nor(ulong src, ulong offset, ulong count);

int do_complete_nor_test(unsigned char ptrn)
{
	unsigned long from_loc = ( (unsigned long) PHYS_FLASH1);
	unsigned long to_loc = ( (unsigned long) (PHYS_FLASH1 + PHYS_FLASH1_SECT_SIZE - 1) );
	int ret_val = 0, nor_byte = 0, count;
	unsigned char *nor_ptr = ( (unsigned char *) from_loc );
	unsigned char *sdr_ptr = ( (unsigned char *) DIAGS_NOR_INST_COPY_LOC );

        for (count = 0; count < NOR_SIZE;  count++)
        {
                *(sdr_ptr + count) = *(nor_ptr + count);
        }

	printf("Backup of NOR content (env and barebox/diags partition) DONE.\n");

	// Initialize the test string with the test pattern bytes.
	memset(nor_tst_ptrn, ptrn, DIAGS_NOR_PTRN_BUF_SIZE);

	printf("NOR test has started with test pattern 0x%02X \n", nor_tst_ptrn[0]);

	if(update_nor(nor_tst_ptrn, 0, (to_loc - from_loc + 1)) < 0)
		printf("ERR: NOR Update FAIL !!\n");
	else
		printf("NOR Update DONE.\n");

	printf("Reading and Comparing the test pattern bytes...\n");

	// Read and Compare test pattern bytes.
	for (nor_byte = 0; nor_byte < DIAGS_NOR_PTRN_BUF_SIZE; nor_byte++)
	{
		if ( nor_ptr[nor_byte] != nor_tst_ptrn[0])
		{
			ret_val = -1;
			printf("\nError at Address: 0x%08X, Written 0x%02X and Read 0x%02X !!\n", (nor_ptr + nor_byte), nor_tst_ptrn[0], nor_ptr[nor_byte]);
		}
	}

        if(update_nor((char *)DIAGS_NOR_INST_COPY_LOC, 0, NOR_SIZE) < 0)
		printf("ERR: NOR Restore FAIL !!\n");
	else
		printf("NOR Restore DONE.\n");

	printf("NOR Test Completed.\n");

	return ret_val;
}

int Comcerto_nor_test (struct diags_test_param *p)
{
	do_complete_nor_test(TEST_PTRN_AA);
	do_complete_nor_test(TEST_PTRN_55);

	return 0;
}

