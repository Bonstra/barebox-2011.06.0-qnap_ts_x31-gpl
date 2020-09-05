/*
;
;  Copyright:
;  ----------------------------------------------------------------------------
;   This confidential and proprietary software may be used only as authorised
;   by a licensing agreement from ARM Limited.
;           (C) COPYRIGHT 2008-2009 ARM Limited , ALL RIGHTS RESERVED
;   The entire notice above must be reproduced on all authorised copies and
;   copies may only be made to the extent permitted by a licensing agreement
;   from ARM Limited.
;  ----------------------------------------------------------------------------
;  $LastChangedRevision: 2887 $
;  ----------------------------------------------------------------------------
*/
/*
------------------------------------------------------------------------------
 Standard definitions of ARM processor mode bits
------------------------------------------------------------------------------
*/
#define Mode_MON         0x16
#define Mode_FIQ         0x11
#define Mode_IRQ         0x12
#define Mode_SVC         0x13
#define Mode_ABT         0x17
#define Mode_UND         0x1B
#define Mode_SYS         0x1F
/*
;------------------------------------------------------------------------------
; Standard definitions of interrupt (I & F) flags in PSRs
;------------------------------------------------------------------------------
*/
#define CPSR_I_BIT       0x80   /*when I bit is set, IRQ is disabled*/
#define CPSR_F_BIT       0x40   /*when F bit is set, FIQ is disabled*/
/*
;------------------------------------------------------------------------------
; Standard definitions of SCR IRQ and FIQ bits(Secure Configuration Register)
;------------------------------------------------------------------------------
*/
#define SCR_FIQ_BIT      0x04   /*when SCR[1](FIQ) bit is set, branch to
                                   Monitor mode on an FIQ exception*/
#define SCR_IRQ_BIT      0x02   /*when SCR[2](IRQ) bit is set, branch to
                                   Monitor mode on an FIQ exception*/
/*
; -----------------------------------------------------------------------------
; Standard definitions of Non-Secure Access Control register configuration
; -----------------------------------------------------------------------------
*/
#define NSACR_BIT_CP     0x03FFF  /*Bit 0-13	*/
#define NSACR_BIT_CL     0x10000  /*Bit 16	*/
#define NSACR_BIT_TL     0x20000  /*Bit 17	*/
#define NSACR_BIT_DMA    0x40000  /*Bit 18	*/
/*
;------------------------------------------------------------------------------
; Standard definitions Secure Debug Enable Register value
;------------------------------------------------------------------------------
*/
#define DEBUG_ENABLE_BIT    0x03   /*Enable non-invasive and invasive debug in
                                   secure user mode*/

#if !defined (COMCERTO_VERIFICATION_TZ)
	#define MON_STACK_BASE		0x10008000
#else
	#define MON_STACK_BASE		0x83004000
#endif

#define MON_STACK_LEN		0X400 /* 1 KB */


