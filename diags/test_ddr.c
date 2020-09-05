#include "tests.h"
#include "dump.h"
#include <diags.h>

#define DDR_ADDR_TEST_DESC	"DDR Addr Bus test"
#define DDR_DATA_TEST_DESC	"DDR Data Bus test"
#define DDR_PATTERN_AA_DESC	"DDR Pattern 0xAA test"
#define DDR_PATTERN_55_DESC	"DDR Pattern 0x55 test"

static void output_result(char *out_str, int res);
static int ddr_addr_bus(unsigned long baseaddr, unsigned long size);
static int ddr_data_bus(unsigned long baseaddr, unsigned long size);
static int ddr_pattern_aa(unsigned long baseaddr, unsigned long size);
static int ddr_pattern_55(unsigned long baseaddr, unsigned long size);


int Comcerto_ddr_test (struct diags_test_param *p)
{
	int res = 0;
	int no_of_tests = 0;
	unsigned int baseaddr = p->params[0];
	unsigned int size = p->params[1];
	unsigned int flags = p->params[2];

#ifdef CONFIG_COMCERTO_VERIFICATION
	int *curr = get_current_loc(); //Saving the current location to set number of tests done
#endif

	if(flags & MEMTEST_ADDRBUS)
	{
		res = ddr_addr_bus(baseaddr, size);
		output_result(DDR_ADDR_TEST_DESC, res);
		no_of_tests++;
	}

        if(flags & MEMTEST_DATABUS)
        {
                res = ddr_data_bus(baseaddr, size);
		output_result(DDR_DATA_TEST_DESC, res);
		no_of_tests++;
        }	

        if(flags & MEMTEST_PATTERN_AA)
        {
                res = ddr_pattern_aa(baseaddr, size);
                output_result(DDR_PATTERN_AA_DESC, res);
		no_of_tests++;
        }	

        if(flags & MEMTEST_PATTERN_55)
        {
                res = ddr_pattern_55(baseaddr, size);
                output_result(DDR_PATTERN_55_DESC, res);
                no_of_tests++;
        }

#ifdef CONFIG_COMCERTO_VERIFICATION
	set_addr_val(curr, no_of_tests);
#endif
}

static void output_result(char *out_str, int res)
{
                if(res)
                {
                        printf("%s: FAIL\n", out_str);
#ifdef CONFIG_COMCERTO_VERIFICATION
                        set_pass();
#endif
                }
                else
                {
                        printf("%s: PASS\n", out_str);
#ifdef CONFIG_COMCERTO_VERIFICATION
                        set_fail();
#endif
                }
	
}

/********************************************************************************
 *
 * Function Name        : ddr_data_bus 
 *
 * Description          : Test the data bus wiring in an address space or memory region by performing
 *                                a walking 1's test at a fixed address within that space or region.
 *
 * Return Value : Zero - if the test succeeds.
 *                              : NonZero - the first pattern that failed.
 *
 * Notes                        : None.
 *
 ********************************************************************************/
static int ddr_data_bus(unsigned long baseaddr, unsigned long size)
{
        volatile unsigned long *test_address = 0;
        unsigned long test_pattern = 0;

        // Get the base address of the memory. Also take this as a memory location for testing.
        test_address = (unsigned long *) baseaddr;

        // Perform a walking 1's test at the selected address.
        for (test_pattern = 1; test_pattern !=0; test_pattern <<= 1)
        {
                // Write the test pattern at the given address.
                (*test_address) = test_pattern;

                if ((*test_address) != test_pattern)
                {
                        printf("\nFound test pattern error and the test pattern is 0x%08X. Returning back from line %03d of %s\n", test_pattern, __LINE__, __FUNCTION__);
                        return -1;
                } 
        }

	return 0;
}

/********************************************************************************
 *
 * Function Name        : ddr_addr_bus
 *
 * Description          : Test the address bus wiring in an address space or memory region by performing
 *                                a walking 1's test on the relevant bits of the address and checking for aliasing.
 *                                This test will find single-bit address failures such as stuck-high, stuck-low, and
 *                                shorted pins.  The base address and size of the region are selected by the caller.
 *
 * Return Value : Zero - if the test succeeds.
 *                              : NonZero - the first address at which an aliasing problem was uncovered.
 *                                By examining the contents of memory, it may be possible to gather
 *                                additional information about the problem.
 *
 * Notes                        : For best results, the selected base address should have enough LSB 0's to
 *                                guarantee single address bit changes.  For example, to test a 64-Kbyte region,
 *                                select a base address on a 64-Kbyte boundary.  Also, select the region size as
 *                                a power-of-two--if at all possible.
 *
 ********************************************************************************/
static int ddr_addr_bus(unsigned long baseaddr, unsigned long size)
{
        volatile unsigned long *test_address = 0;

        unsigned long test_pattern_A = 0xAAAAAAAA;
        unsigned long test_pattern_5 = 0x55555555;

        unsigned long address_mask = ((size/(sizeof(baseaddr))) - 1);

        unsigned long mem_offset = 0;
        unsigned long test_mem_offset = 0;

        // Get the base address of the memory.
        test_address = (unsigned long *) baseaddr;

        // Write the default pattern at each of the power-of-two offsets.
        for (mem_offset = 1; (mem_offset & address_mask) != 0; mem_offset <<= 1)
        {
		*(test_address + mem_offset) = test_pattern_A;
		printf("\nWrite: At address 0x%08X the value is 0x%08X\n", (test_address + mem_offset), test_pattern_A);
        } 

        // Check for address bits stuck high.
        test_mem_offset = 0;
        *(test_address + test_mem_offset) = test_pattern_5;
        printf("\nWrite: At address 0x%08X the value is 0x%08X\n", (test_address + test_mem_offset), test_pattern_5);

        for (mem_offset = 1; (mem_offset & address_mask) != 0; mem_offset <<= 1)
        {
		if (*(test_address + mem_offset) != test_pattern_A)
		{
			printf("\nFound test pattern error and the test pattern is 0x%08X. Returning back from line %03d of %s\n", ((unsigned long *)(test_address + mem_offset)), __LINE__, __FUNCTION__);

			return -1;
		} 
        } 

        // Check for address bits stuck low or shorted.
        test_mem_offset = 0;
        *(test_address + test_mem_offset) = test_pattern_A;
        printf("\nWrite: At address 0x%08X the value is 0x%08X\n", (test_address + test_mem_offset), test_pattern_A);

        for (test_mem_offset = 1; (test_mem_offset & address_mask) != 0; test_mem_offset <<= 1)
        {
                *(test_address + test_mem_offset) = test_pattern_5;

                if (*(test_address + 0) != test_pattern_A)
                {
                        printf("\nFound test pattern error and the test pattern is 0x%08X. Returning back from line %03d of %s\n", ((unsigned long *)(test_address + test_mem_offset)), __LINE__, __FUNCTION__);

			return -1;
                } 

                for (mem_offset = 1; (mem_offset & address_mask) != 0; mem_offset <<= 1)
                {
                        if ( (*(test_address + mem_offset) != test_pattern_A) && (mem_offset != test_mem_offset) )
                        {
                               printf("\nFound test pattern error and the test pattern is 0x%08X. Returning back from line %03d of %s\n", ((unsigned long *)(test_address + test_mem_offset)), __LINE__, __FUNCTION__);

				return -1;
                        } 
                } 

                *(test_address + test_mem_offset) = test_pattern_A;
                printf("\nWrite: At address 0x%08X the value is 0x%08X\n", (test_address + test_mem_offset), test_pattern_A);
        }

	return 0;
}

/********************************************************************************
 *
 * Function Name        : ddr_pattern_aa
 *
 * Description          : Test the integrity of a physical memory device by performing an
 *                                increment/decrement test over the entire region.  In the process every storage
 *                                bit in the device is tested as a zero and a one.  The base address and the size
 *                                of the region are selected by the caller.
 *
 * Return Value : NULL - if the test succeeds.
 *                              : NonZero - the first address at which an incorrect value was read back.
 *
 * Notes                        : None.
 *
 ********************************************************************************/
static int ddr_pattern_aa(unsigned long baseaddr, unsigned long size)
{
        volatile unsigned long *test_address = 0;

        unsigned long test_pattern = 0;
        unsigned long anti_pattern = 0;

        unsigned long nWords = size / sizeof(baseaddr);
        unsigned long mem_offset = 0;

        // Get the base address of the memory.
        test_address = (unsigned long *) baseaddr;

        // Get the pattern.
        test_pattern = 0xAAAAAAAA;
        anti_pattern = ~test_pattern;

        // Fill memory with a known pattern.
        for (mem_offset = 0; mem_offset < nWords; mem_offset++)
        {
                *(test_address + mem_offset) = test_pattern;
        } 

        // Check each location and invert it for the second pass.
        for (mem_offset = 0; mem_offset < nWords; mem_offset++)
        {
                if (*(test_address + mem_offset) != test_pattern)
                {
                        printf("\nFound test pattern error at address 0x%08X", (test_address + mem_offset));
                        printf("\nWritten value is: 0x%08X,  Read value is: 0x%08X\n", test_pattern, *(test_address + mem_offset));
			return -1;
                } 

                *(test_address + mem_offset) = anti_pattern;
        }

        // Check each location for the inverted pattern and zero it.
        for (mem_offset = 0; mem_offset < nWords; mem_offset++)
        {
                if (*(test_address + mem_offset) != anti_pattern)
                {
                        printf("\nFound test pattern error at address 0x%08X", (test_address + mem_offset));
                        printf("\nWritten value is: 0x%08X,  Read value is: 0x%08X\n", anti_pattern, *(test_address + mem_offset));
			return -1;
                } 
        } 

        return 0;

}

/********************************************************************************
 *
 * Function Name        : ddr_pattern_55 
 *
 * Description          : Test the integrity of a physical memory device by performing an
 *                                increment/decrement test over the entire region.  In the process every storage
 *                                bit in the device is tested as a zero and a one.  The base address and the size
 *                                of the region are selected by the caller.
 *
 * Return Value : NULL - if the test succeeds.
 *                              : NonZero - the first address at which an incorrect value was read back.
 *
 * Notes                        : None.
 *
 ********************************************************************************/
static int ddr_pattern_55(unsigned long baseaddr, unsigned long size)
{
        volatile unsigned long *test_address = 0;

        unsigned long test_pattern = 0;
        unsigned long anti_pattern = 0;

        unsigned long nWords = size / sizeof(baseaddr);
        unsigned long mem_offset = 0;

        // Get the base address of the memory.
        test_address = (unsigned long *) baseaddr;

        // Get the pattern.
        test_pattern = 0x55555555;
        anti_pattern = ~test_pattern;

        // Fill memory with a known pattern.
        for (mem_offset = 0; mem_offset < nWords; mem_offset++)
        {
                *(test_address + mem_offset) = test_pattern;
        } 

        // Check each location and invert it for the second pass.
        for (mem_offset = 0; mem_offset < nWords; mem_offset++)
        {
                if (*(test_address + mem_offset) != test_pattern)
                {
                        printf("\nFound test pattern error at address 0x%08X", (test_address + mem_offset));
                        printf("\nWritten value is: 0x%08X,  Read value is: 0x%08X\n", test_pattern, *(test_address + mem_offset));
			return -1;
                } 

                *(test_address + mem_offset) = anti_pattern;
        } 

        // Check each location for the inverted pattern and zero it.
        for (mem_offset = 0; mem_offset < nWords; mem_offset++)
        {
                if (*(test_address + mem_offset) != anti_pattern)
                {
                        printf("\nFound test pattern error at address 0x%08X", (test_address + mem_offset));
                        printf("\nWritten value is: 0x%08X,  Read value is: 0x%08X\n", anti_pattern, *(test_address + mem_offset));
			return -1;
                } 
        } 

        return 0;
}

