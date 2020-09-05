/*
** $Id: proslic_timer_intf.c,v 1.1.1.1 2014/03/12 12:17:40 wokes Exp $
**
** system.c
** System specific functions implementation file
**
** Author(s): 
** laj
**
** Distributed by: 
** Silicon Laboratories, Inc
**
** File Description:
** This is the implementation file for the system specific functions like timer functions.
**
**
*/
#include "proslic_mspd.h"
//#include "proslic_api/proslic_datatypes.h"
//#include "proslic_api/proslic_timer_intf.h"

#include "proslic_api/si_voice_datatypes.h"
#include "proslic_api/si_voice_timer_intf.h"
#if 0 //Rajiv
#include <linux/delay.h>
#endif



/*
** Function: SYSTEM_Delay
*/
#define HZ 100
int time_DelayWrapper (void *hTimer, int timeInMs){
	signed long delay = timeInMs * HZ / 1000;


	udelay(delay*1000);

	return 0;
}




/*
** $Log: proslic_timer_intf.c,v $
** Revision 1.1.1.1  2014/03/12 12:17:40  wokes
** bootloader for MindSpeed
**
** Revision 1.1  2008/11/27 07:29:38  rajivkumar.vishwakarma
** LOG: ProSlic driver integrated to DiagTool for POTS testing
**
** Revision 1.1.2.1  2008/11/19 12:10:42  mouli.burla
** LOG: 42063
** Proslic driver with Si3215 & Si3227 source integrated.
** Code verified for Si3215
**
** Revision 1.1  2008/04/28 13:46:35  appalaraju.chapala
** proslic initial checkin from silabs ver5.00.1
**
*/
