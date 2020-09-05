#ifndef _DECT_TEST_H
#define _DECT_TEST_H

#include "tests.h"

#define MAX_DTCM_DATA 8

struct dect_test {
        unsigned int id;
        char *name;
	char *goal;
	unsigned int flag;
        int (*host_fnc) (void);
	unsigned char num_dtcm_data;
	unsigned int dtcm_data[MAX_DTCM_DATA];
	unsigned int *itcm_code;
};

#define SCU_ARM_DTCM_WITH_CORTEX	0
#define SCU_ARM_ITCM_WITH_CORTEX	1
#define SCU_ARM_DTCM_BOUNDARIES		2
#define SCU_ARM_ITCM_JUMP		3

#define DECT_CA9_RES_ADDR		0x401ffffc
#define DECT_ARM926_RES_ADDR		0xc01ffff8

#define DECT_PASS_U32			0x900dd009
#define DECT_PASS_U16			0x900d
#define DECT_FAIL_U32			0xbaadbaad
#define DECT_FAIL_U16			0xbaad

//Flags definitions, specifying wether itcm or dtcm loading is required
#define NEED_DTCM			0x1
#define NEED_ITCM			0x2

//CSS DTCM memory address
#define SCU_CSS_DTCM		0x00100000 // as seen from inside css
#define MCU_CSS_DTCM		0x9e100000 // as seen from outside css

//CSS ITCM memory address 
#define SCU_CSS_ITCM		0x00300000 // as seen from inside css 
#define MCU_CSS_ITCM		0x9e000000 // as seen from outside css
#define SCU_CSS_ITCM_EXT	0x9e000000 // as seen from outside css

void dect_delay(void);

#endif
