
#include <common.h>
#include <asm/io.h>
#include <diags.h>
#include <pfe/pfe.h>

#define DIAG_DDR_IPSEC_POLLRESULT_ADDR 0x3fa2000
#define DIAG_DMEM_IPSEC_TESTCASE_TYPE_ADDR 0x1a00

#define UTIL_IPSEC_OUT_POLL	(1 << 0)
#define UTIL_IPSEC_IN_POLL	(1 << 1)
#define UTIL_IPSEC_OUT_INTR	(1 << 2)
#define UTIL_IPSEC_IN_INTR	(1 << 3)

#define UPE_TEST_SUCCESS 0x0a0a
#define UPE_TEST_FAILURE 0x0505

#define TEST_NOT_STARTED 	0x1
#define TEST_NOT_FINISHED 	0x2

#define MAX_ITERATIONS	1000000
#define MAX_WAIT_TIME	20000

#define DIAG_UTIL_FIRMWARE_FILENAME "utilpe/util_c2000_diag_elf.fw"
/* UTIL-PE ELF file content */
unsigned char util_diag_fw[] __attribute__((aligned(sizeof(int)))) = {
#include DIAG_UTIL_FIRMWARE_FILENAME
};

struct firmware {
        const u8 *data;
};

//#include "utilpe/upe_inst.h"
//#include "utilpe/upe_testvect.h"
//#include "utilpe/utilpe.fw"

//u32 util_csr_base = UTIL_CSR_BASE_ADDR; 
/* The following functions are available in "drivers/net/comcerto/pfe.c" */
extern u32 pe_dmem_read(int id, u32 addr, u8 size);
extern void pe_dmem_write(int id, u32 val, u32 addr, u8 size);
extern int pfe_load_elf(int pe_mask, struct firmware *fw);

#if 0
extern u32 util_mem_access(int type, unsigned long addr, u32 val, u8 size);
extern int util_download_fw(u32 pe_fw[], int fw_size);
#endif
extern void util_reset(void);
extern void util_enable(void);
extern void util_disable(void);


/* The following test checks the command from the user
and runs the corresponding test in utilpe 
- It reads the utilpe's dmem location to check if previous test is complete.
- If the value in the location is 0 then, it writes the command read from the user to the same dmem location.
- Wait for some time to complete the test.
- Poll the result location in the DDR to check if the test is complete.
- Once test completes print the result.
*/


int utilpe_ipsec_start_test (unsigned int test_command)
{

	unsigned int  test_type, i;
	unsigned int count = 0;
	unsigned int test_result;

	//test_type = util_mem_access(PE_MEM_ACCESS_READ, DIAG_DMEM_IPSEC_TESTCASE_TYPE_ADDR, 0, 4);
	//pe_dmem_write(UTIL_ID,0,DIAG_DMEM_IPSEC_TESTCASE_TYPE_ADDR,4);
	do {
		test_type = pe_dmem_read(UTIL_ID, DIAG_DMEM_IPSEC_TESTCASE_TYPE_ADDR, 4);
	}while(test_type != 0);

	if (test_type == 0)
	{
		/* set the test result to 0 */
		writel(0,DIAG_DDR_IPSEC_POLLRESULT_ADDR);
		/* write the value to utilpe dmem start the test  */		
		//util_mem_access(PE_MEM_ACCESS_WRITE, DIAG_DMEM_IPSEC_TESTCASE_TYPE_ADDR, test_command, 4);
		pe_dmem_write(UTIL_ID,test_command,DIAG_DMEM_IPSEC_TESTCASE_TYPE_ADDR,4);
	}
  	else
		return TEST_NOT_STARTED;
	
	for (i = 0 ; i < MAX_WAIT_TIME; i++)
		;	
	/* wait for some time to finish the test */
	while (readl(DIAG_DDR_IPSEC_POLLRESULT_ADDR) == 0)
	{
		for (i = 0 ; i < MAX_WAIT_TIME; i++)
			;	
		count++;

	}

	test_result = readl(DIAG_DDR_IPSEC_POLLRESULT_ADDR);
	return test_result;
}

/* The following function reads the command from user and
   initiates the utilpe ipsec test */
int Comcerto_util_ipsec_test (struct diags_test_param *p)
{
 	struct firmware util_fw;
	unsigned int flags = p->params[0];
	int i, result,rc;
	unsigned int temp;

	util_fw.data = util_diag_fw;
	/* Load the utilepe firmware */
	/* TODO This function needs to be modified with modified pfe.c */
	//flags = 0xf;
	printf("flags : %x\n", flags);
	
	util_init(NULL);
	util_reset();

	rc = pfe_load_elf(UTIL_MASK, (struct firmware *)&util_fw);
	if (rc < 0) {
		printf("ERR: %s: util firmware load failed\n", __func__);
		return -1 ;
	}
	printk("util_firmware loaded\n");


	util_enable();
	

	for (i = 0 ; i < 4; i++)
	{
		if (flags & (1<<i))
		{
			result = utilpe_ipsec_start_test(1<<i);
			switch (result)
			{
				case TEST_NOT_STARTED:
					printf("Test : %d - Not started\n ", (1<<i));
					rc = -1;
					break;
				case TEST_NOT_FINISHED:
					printf("Test : %d - Not finished by utilpe\n ", (1<<i));
					rc = -1;
					break;
				case UPE_TEST_SUCCESS:
					printf("Test : %d - passed  \n", (1<<i));
					rc = 0;
					break;
				case UPE_TEST_FAILURE:
					printf("Test : %d - failed by utilpe\n ", (1<<i));
					rc = -1;
					break;
				default:
					printf("Default: %d \n", result);
					rc = -1;
			}
		}
	}
	
	return rc;
}

