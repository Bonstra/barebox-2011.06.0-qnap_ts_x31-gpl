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

.globl __smc_entry
__smc_entry:
		ldr	pc, =tzmonitor_smchandler
		ldr	pc, =tzmonitor_smchandler
		ldr	pc, =tzmonitor_smchandler
		ldr	pc, =tzmonitor_smchandler
		ldr	pc, =tzmonitor_smchandler
		ldr	pc, =tzmonitor_smchandler
		ldr	pc, =tzmonitor_smchandler
		ldr	pc, =tzmonitor_smchandler

.globl _smc_start
_smc_start:
	.word	__smc_entry
