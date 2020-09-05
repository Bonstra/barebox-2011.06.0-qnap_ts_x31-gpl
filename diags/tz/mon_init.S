#
#  Copyright:
#  ----------------------------------------------------------------------------
#   This confidential and proprietary software may be used only as authorised
#   by a licensing agreement from ARM Limited.
#           (C) COPYRIGHT 2008-2009 ARM Limited , ALL RIGHTS RESERVED
#   The entire notice above must be reproduced on all authorised copies and
#   copies may only be made to the extent permitted by a licensing agreement
#   from ARM Limited.
#  ----------------------------------------------------------------------------
#  $LastChangedRevision: 2887 $
#  ----------------------------------------------------------------------------

#include "mon_macro.h"

.global tz_mon_init
tz_mon_init:
        #----------------------------------------------------------------------
	#         # Configure Non-Secure Access control register
	#
	#----------------------------------------------------------------------
	MRC      p15, 0, r0, c1, c1, 2
	# Allow NS access DMA and Co-processor registers 0-13
	LDR      r1, =(NSACR_BIT_CL | NSACR_BIT_CP | NSACR_BIT_DMA)
	ORR      r0, r0, r1
	MCR      p15, 0, r0, c1, c1, 2

	# Switch to MON mode
	MRS      r0, CPSR
	# Clear Mode bits first before setting it to monitor mode
	BIC      r0, r0, #0x1f
	ORR      r0, r0, #Mode_MON
	MSR      CPSR_c, r0

	# set the monitor vector base
        LDR      r2, =_smc_start
        MCR      p15, 0, r2, c12, c0, 1

        # set the monitor stack
        LDR      sp, =MON_STACK_BASE

        # Switch back to SVC mode
        MRS      r0, CPSR
        # Clear Mode bits first before setting it to SVC mode
        BIC      r0, r0, #0x1f
        ORR      r0, r0, #Mode_SVC
        MSR      CPSR_c, r0

        # Return to SWd calling function
        BX      lr


