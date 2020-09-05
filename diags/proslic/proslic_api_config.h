/*
** Copyright © 2007 by Silicon Laboratories
**
** $Id: proslic_api_config.h,v 1.1.1.1 2014/03/12 12:17:40 wokes Exp $
**
**
** Author(s): 
** laj
**
** Distributed by: 
** Silicon Laboratories, Inc
**
** This file contains proprietary information.	 
** No dissemination allowed without prior written permission from
** Silicon Laboratories, Inc.
**
**
*/

//#define DISABLE_DTMF_SETUP 
//#define DISABLE_FSK_SETUP 
//#define DISABLE_TONE_SETUP 
//#define DISABLE_RING_SETUP 
//#define DISABLE_AUDIOGAIN_SETUP 
//#define DISABLE_DCFEED_SETUP 
#define DISABLE_GPIO_SETUP 
#define DISABLE_PULSE_SETUP
//#define DISABLE_PCM_SETUP 
#define ENABLE_DEBUG 
//#define DISABLE_CI_SETUP 
//#define DISABLE_ZSYNTH_SETUP 
//#define DISABLE_MALLOC
//#define GCI_MODE  

#if 0 //Rajiv
#include <linux/kernel.h>
#endif
#define LOGPRINT(str, args...) printf(str, ## args)

/*si321x hardware specific settings
* If these are not defined the default
* values are loaded */
#define	INIT_PWR_ALARM_Q1Q2		0x7c0  		
#define	INIT_PWR_ALARM_Q3Q4		0x2600	
#define	INIT_PWR_ALARM_Q5Q6		0x1B80		
#define	INIT_THERM_LP_POLE_Q1Q2		0x08c	
#define	INIT_THERM_LP_POLE_Q3Q4		0x0100		
#define	INIT_THERM_LP_POLE_Q5Q6		0x010
#define SI3215_TONE /*we want to support Si3215/16 tones*/
#define SI3210_TONE /*we want to support Si3210 tones*/
