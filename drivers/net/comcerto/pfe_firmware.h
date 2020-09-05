

/** @file
 *  Contains all the defines to handle parsing and loading of PE firmware files.
 */

#ifndef __PFE_FIRMWARE_H__
#define __PFE_FIRMWARE_H__


#define CLASS_FIRMWARE_FILENAME		"class_sbl_c2000_elf.fw"
#define TMU_FIRMWARE_FILENAME		   "tmu_sbl_c2000_elf.fw"
#define UTIL_FIRMWARE_FILENAME		"util_c2000_elf.fw" 


int pfe_firmware_init(void);
void pfe_firmware_exit(void);


#endif
