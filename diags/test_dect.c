/*************************************************************************************************
* Validation code for DECT block (CSS)
*
* All tests cases come from DSPG / MSPD VLSI Team, and adapted for barebox validation framework
*
**************************************************************************************************/

#include "tests.h"
#include "dump.h"
#include "test_dect.h"
#include "stdio.h"
#include <string.h>
#include <diags.h>

#ifndef caseretstr
#define caseretstr(x) case x: return #x
#endif

extern int host_scu_arm_dtcm_with_cortex(void);

//make sure following define reflects exactly the number of tests cases in dect_test_list
#define DECT_NUM_TESTS 1 

static unsigned int armcode_scu_arm_dtcm_with_cortex[] = {
#include "dect/arm926_code/scu_arm_dtcm_with_cortex.h"
};
//unsigned char armcode_scu_arm_itcm_with_cortex[] = {
//#include "dect/arm926_code/scu_arm_itcm_with_cortex.h"
//};
//unsigned char armcode_scu_arm_dtcm_boundaries[] = {
//#include "dect/arm926_code/scu_arm_dtcm_boundaries.h"
//};
//unsigned char armcode_scu_arm_itcm_jump[] = {
//#include "dect/arm926_code/scu_arm_itcm_jump.h"
//};


// test id, test name, test description, flag (itcm,dtcm), host (Cortex A9) test function, num CSS dtcm data, CSS dtcm data(addr,data), CSS itcm code 
static struct dect_test dect_test_list[] = {
  {SCU_ARM_DTCM_WITH_CORTEX, "scu_arm_dtcm_with_cortex", "Read Write from to DTCM by both cortex A9 and ARM926", NEED_DTCM | NEED_ITCM, &host_scu_arm_dtcm_with_cortex, 4, {0x00100000,0x01111011,0x00101FFF,0x02222022}, armcode_scu_arm_dtcm_with_cortex},
  //{SCU_ARM_ITCM_WITH_CORTEX, "scu_arm_itcm_with_cortex", "Read Write from to ITCM by both cortex A9 and ARM926", NEED_DTCM | NEED_ITCM, &host_scu_arm_itcm_with_cortex, 0, {0,0,0,0}, armcode_scu_arm_itcm_with_cortex},
  //{SCU_ARM_DTCM_BOUNDARIES, "scu_arm_dtcm_boundaries", "ARM926 reads writes to physical boundaries of DTCM", NEED_DTCM | NEED_ITCM, &host_scu_arm_dtcm_boundaries, 0, {0,0,0,0}, armcode_scu_arm_dtcm_boundaries},
  //{SCU_ARM_ITCM_JUMP, "scu_arm_itcm_jump", "ARM926 jumps between all physical rams of the ITCM", NEED_DTCM | NEED_ITCM, &host_scu_arm_itcm_jump, 0, {0,0,0,0}, armcode_scu_arm_itcm_jump},  
};

char * dect_id2string(unsigned short id)
{
	switch (id)
	{
		caseretstr(SCU_ARM_DTCM_WITH_CORTEX);
		caseretstr(SCU_ARM_ITCM_WITH_CORTEX);
		caseretstr(SCU_ARM_DTCM_BOUNDARIES);
		caseretstr(SCU_ARM_ITCM_JUMP);
		default:
			return (char *) "Unknown DECT test case";
	}
}


void dect_delay(void)
{
	int i;
	for(i=0;i<30;i++);
}

static void dect_load_arm_data(unsigned int *base, struct dect_test *test)
{
	int i;
	unsigned int offset, val;
#ifndef CONFIG_COMCERTO_VERIFICATION	
	printf("dect_css_load: num_dtcm_data %d\n", test->num_dtcm_data);
#endif	
	for(i = 0; i < test->num_dtcm_data; i+=2) {
		offset = test->dtcm_data[i]; val = test->dtcm_data[i+1];
#ifndef CONFIG_COMCERTO_VERIFICATION		
		printf("0x%08x: %08x\n", offset, val);
#endif		
		memcpy(base + offset, val, sizeof(unsigned int));	
	}
	printf("\n");
}

static void dect_load_arm_code(unsigned int *base, struct dect_test *test)
{
	memcpy(base, test->itcm_code, sizeof(test->itcm_code)*sizeof(unsigned int));
}

static void dect_css_load (struct dect_test *test)
{
	// load data in CSS dtcm (@MCU_CSS_DTCM)
	if(test->flag & NEED_DTCM)
		dect_load_arm_data((unsigned int *)MCU_CSS_DTCM, test);
	
	// load arm926 code (axf) in CSS itcm (@MCU_CSS_ITCM)
	if(test->flag & NEED_ITCM)
		dect_load_arm_code((unsigned int *)MCU_CSS_ITCM, test);
}

static int dect_run (struct dect_test *test)
{
	unsigned int read_data;
	unsigned int arm_926_pass, arm_a9_pass;
  
	// load CSS itcm and dtcm 		
	dect_css_load(test);
	
	// CA9 entry point
	test->host_fnc();
	
	// polls for CSS ARM926 result
	read_data = 0;	
	while((read_data != DECT_PASS_U16) && (read_data != DECT_FAIL_U16))
	{
		read_data = readw(DECT_ARM926_RES_ADDR);
	}

	if(read_data == DECT_PASS_U16) {
		arm_926_pass = 1;
#ifndef CONFIG_COMCERTO_VERIFICATION		
		printf("arm926 PASS\n");
#endif
	}	
	else {
		arm_926_pass = 0; 
#ifndef CONFIG_COMCERTO_VERIFICATION	
		printf("arm926 FAIL\n");
#endif
	}	

	// polls for Host Cortex A9 result
	read_data = readl(DECT_CA9_RES_ADDR);
	while((read_data != DECT_PASS_U32) && (read_data != DECT_FAIL_U32))
	{
		read_data = readl(DECT_CA9_RES_ADDR);
	}
	
	// final test pass/fail checking
	if(read_data == DECT_PASS_U32) {
		arm_a9_pass = 1; 
#ifndef CONFIG_COMCERTO_VERIFICATION	
		printf("a9 PASS\n");
#endif		
	}	
	else {
		arm_a9_pass = 0; 
#ifndef CONFIG_COMCERTO_VERIFICATION	
		printf("a9 FAIL\n");
#endif
	}	

	return (arm_a9_pass & arm_926_pass);
}


static void dect_output_result(struct dect_test *test, int res)
{
	if(res)
	{
#ifdef CONFIG_COMCERTO_VERIFICATION
		  set_pass();
#else
		  printf("%s: PASS\n", dect_id2string(test->id));
#endif
	}
	else
	{

#ifdef CONFIG_COMCERTO_VERIFICATION
		set_fail();
#else
		printf("%s: FAIL\n", dect_id2string(test->id));
#endif
	}
}

/* 
* DECT validation
*
*
*/
int Comcerto_dect_test (struct diags_test_param *p)
{
	int i;
	int res = 0;
	int no_of_tests = 0;
	struct dect_test *test;

#ifdef CONFIG_COMCERTO_VERIFICATION
	int *curr = get_current_loc(); //Saving the current location to set number of tests done
#endif
    
	for(i = 0; i < DECT_NUM_TESTS; i++)
	{
		test = &dect_test_list[i];
#ifndef CONFIG_COMCERTO_VERIFICATION
		printf("Comcerto_dect_test:\nID %d\n %s\n%s\n", test->id, test->name, test->goal);
#endif
		res = dect_run(test);
		
		dect_output_result(test, res);	
		
		no_of_tests++;
	}

#ifdef CONFIG_COMCERTO_VERIFICATION
	set_addr_val(curr, no_of_tests);
#endif
      
	return 0;
}




