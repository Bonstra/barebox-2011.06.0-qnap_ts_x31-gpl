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

# -----------------------------------------------------------------------------
# Monitor SMCHandler
# Description
#      Main SMC handler. Called from Monitor Vector base + SMC vector
# -----------------------------------------------------------------------------
.global tzmonitor_smchandler
tzmonitor_smchandler:

	SRSDB sp!, #Mode_MON

	# Save some workable registers
	STMDB   sp!, {r0-r3}

	#--------------------------------------------------------------
	# Depending on from which world the SMC is called from the code
	# switches to appropriate world by properly setting the SCR
	# register.
	#--------------------------------------------------------------
	# read SCR
	MRC     p15, 0, r3, c1, c1, 0
	# Extract current SCR->NS bit
	ANDS    r1, r3, #1
	# If NS bit is set then it is a call from NWd. Otherwise it is a return
	# from SWd.
	BEQ    SWdCall

NWdCall:
	# Here register r3 contains NS bit information i.e. NS=1 (NWd)
	BIC     r3, r3, #1

	# Clear NS bit in SCR register before switching to SWd.
	MCR     p15, 0, r3, c1, c1, 0

	# Restore corrupted registers
	LDMIA   sp!, {r0-r3}

	# Return back to the caller
	RFEIA	sp!

SWdCall:
	# read SCR into register r0
	MRC     p15, 0, r0, c1, c1, 0
	ORR     r0, r0, #1

	# Set NS bit before switching to NWd
	MCR     p15, 0, r0, c1, c1, 0

	# Restore corrupted registers
	LDMIA     sp!, {r0-r3}

	# Return back to the caller
	RFEIA	sp!

