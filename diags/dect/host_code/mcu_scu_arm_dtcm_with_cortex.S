/*
*
*     Project: DMW96
*
*     Module Name: 
*
*     $Source: /home/cvsroot/NasX86/Others/bootldr/barebox-2011.06.0/diags/dect/host_code/mcu_scu_arm_dtcm_with_cortex.S,v $
*
*     $ReVision: $
*
*     $Date: 2014/03/12 12:17:40 $
*
*     $Author: wokes $
*
*     Copyright (C) 2010 DSPG Ltd. All Rights Reserved
*
*     Description: in this test the cortex performs transactions 
*	  to the DTCM while the arm petches instructions simultanously
*             
*/

	/*#include "../common_code/asm/macros.h"*/
	#include "../common_code/asm/sys_regs_def.h"
	#include "../arm926_boot/mcu_test_end.s"

	/* EXPORT  mainc_mcu_scu_arm_dtcm_with_cortex */
/*
*	EXPORT  undefined_Handler
*	EXPORT  swi_Handler
*	EXPORT  prefetchAbort_Handler
*	EXPORT  dataAbort_Handler
*	EXPORT  irq_Handler
*	EXPORT  fiq_Handler
*/


/* 	AREA    bank0, CODE, READONLY */

.global mainc_mcu_scu_arm_dtcm_with_cortex

mainc_mcu_scu_arm_dtcm_with_cortex:

/******************************************
* TEST
*****************************************/


/*
* read-compare from DTCM physical boundaries
* the data was inserted ahead by readmemh
*/
   LDR r12, =0x0001

   
      LDR      r0, =McuCssDtcm
      

/* read from the start of physical 32KRam */
      LDR      r2, =0x01111011 
	  ldr	   r6,[r0]
      CMP      r6,r2
      BNE      ErrorTest_host

/* read from the end of physical 32KRam */
      LDR      r7, =0x7FFC
	  ADD	   r1,r0,r7   
      LDR      r2, =0x02222022 
	  ldr	   r6,[r1]
      CMP      r6,r2
      BNE      ErrorTest_host



/* read from the start of physical 32KRam with wraped memory address
*      LDR      r7, =0x10000 ;wraped memory address to adress 0x0000000
*	  ADD	   r1,r0,r7  
*      LDR      r2, =0x01111011 
*	  ldr	   r6,[r1]
*      CMP      r6,r2
*      BNE      ErrorTest_host
*/

/*
* write to all DTCM physical boundaries
*/
   LDR r12, =0x0002

   
      
/* write to the start of physical 32KRam */
      LDR      r2, =0x12212211 
	  str	   r2,[r0]

/* write to the end of physical 32KRam */
      LDR      r7, =0x7FFC
	  ADD	   r1,r0,r7   
      LDR      r2, =0x23323322 
	  str	   r2,[r1]


/*
* read-compare from all DTCM physical boundaries
*/
   LDR r12, =0x0003

   

/* read-compare from the start of physical 32KRam0 */
      LDR      r2, =0x12212211 
      ldr	   r6,[r0]
      CMP      r6,r2
      BNE      ErrorTest_host

/* read-compare from the end of physical 32KRam0 */
      LDR      r7, =0x7FFC
	  ADD	   r1,r0,r7   
      LDR      r2, =0x23323322 
	  ldr	   r6,[r1]
      CMP      r6,r2
      BNE      ErrorTest_host


 


/*
* ordering the test env to load the scu out files 
*/

/*   ; ldr   r0, =0x8000 */
/*   ; ldr   r5, =TopTbRegsBase + TbEnvConfigOffset */
/*   ; strh  r0, [r5] */



/*
* enabling the arm clock  & removing out of reset
*/
   LDR r12, =0x0006

   /* Write the GPIO registers to bring ARM926 out fo reset */
      LDR      r11, =0xf3 
      LDR      r10, =0x904700b0 
	 str	   r11,[r10]

      LDR      r11, =0x0 
      LDR      r10, =0x904700b4 
	 str	   r11,[r10]


/* enabeling the write- protected  clk_en register for writing		
*	WriteWord CmuBase, CmuWrProtOffset, 0x00000090
*
* enable clock for arm (bit[1])
* other fields are set to their default values
* WriteWord CmuBase, CmuSwCtrlClkEn1Offset, 0x00804012
*/

/*
* waiting for arm to start fetching 
*
* poling for "c4c4" at css sys cfg scratch pad reg  
* this is written by the arm to indicate to the cortex that the arm started his tasks
*/
loop1:
      LDR r11, = CssSysCfgBase + CssSysCfgScPadRegOffset
	  ldr r10, [r11]
      LDR r7, =0xc4c4
  	  cmp r10,r7
	  BEQ loop1end
	  b loop1
loop1end:


/*
* write-read-compare to DTCM in intervals
*/
   LDR r12, =0x0007

   
      LDR      r0, =McuCssDtcm
   
      ADD 	   r0, r0, #0x6000 @ points 0x6000 bytes into DTCM memory
      ADD 	   r1, r0, #0x0040 
      
      LDR      r2, =0x38576328 
      LDR      r3, =0x38473694       
      LDR      r4, =0x97473864
      LDR      r5, =0x50374673

loop2:
      STM      r0,{r2-r5}
      LDMIA    r0!,{r6-r9}

/*
*	  str	   r2,[r0]
*	  nop
*	  ldr	   r6,[r0]
*	  add	   r0,r0,#4
*/      
	  CMP      r6,r2
      BNE      ErrorTest_host
      CMP      r7,r3
      BNE      ErrorTest_host
      CMP      r8,r4
      BNE      ErrorTest_host
      CMP      r9,r5
      BNE      ErrorTest_host
      

      CMP r0, r1
      BNE loop2


/*
* write-read-compare to DTCM in intervals 16bit
*/
   LDR r12, =0x0008

   
      LDR      r0, =McuCssDtcm
   
      ADD 	   r0, r0, #0x6000 @ points 0x6000 bytes into ITCM memory
      ADD 	   r0, r0, #0x0040 
      ADD 	   r1, r0, #0x0010 
      
      LDR      r2, =0x3328 
      LDR      r3, =0x3694       
      LDR      r4, =0x9744
      LDR      r5, =0x3746

loop4:
      strh     r2,[r0]
      strh     r3,[r0,#2]
      strh     r4,[r0,#4]
      strh     r5,[r0,#6]

      ldrh     r6,[r0]
      ldrh     r7,[r0,#2]
      ldrh     r8,[r0,#4]
      ldrh     r9,[r0,#6]

      ADD 	   r0, r0, #8 
      
	  CMP      r6,r2
      BNE      ErrorTest_host
      CMP      r7,r3
      BNE      ErrorTest_host
      CMP      r8,r4
      BNE      ErrorTest_host
      CMP      r9,r5
      BNE      ErrorTest_host
      

      CMP r0, r1
      BLT loop4


/*
* write-read-compare to DTCM in intervals 8bit
*/
   LDR r12, =0x0009

   
      LDR      r0, =McuCssDtcm
   
      ADD 	   r0, r0, #0x6000 @ points 0x6000 bytes into ITCM memory
      ADD 	   r0, r0, #0x0060 
      ADD 	   r1, r0, #0x0010 
      
      LDR      r2, =0x6a 
      LDR      r3, =0x23       
      LDR      r4, =0x8f
      LDR      r5, =0x2d

loop5:
      strb     r2,[r0]
      strb     r3,[r0,#1]
      strb     r4,[r0,#2]
      strb     r5,[r0,#3]

      ldrb     r6,[r0]
      ldrb     r7,[r0,#1]
      ldrb     r8,[r0,#2]
      ldrb     r9,[r0,#3]

      ADD 	   r0, r0, #4 
      
	  CMP      r6,r2
      BNE      ErrorTest_host
      CMP      r7,r3
      BNE      ErrorTest_host
      CMP      r8,r4
      BNE      ErrorTest_host
      CMP      r9,r5
      BNE      ErrorTest_host
      

      CMP r0, r1
      BLT loop5

/*
* checking that the arm finished  fetching 
*/
   LDR r12, =0x00010
/* poling for "a2a2" at css sys cfg scratch pad reg
* this is written by the arm to indicate to the cortex that the arm finished his tasks
*/
loop3:
      LDR      r11, =CssSysCfgBase + CssSysCfgScPadRegOffset
	  ldr r10, [r11]
      LDR r7, =0xa2a2
  	  cmp r10,r7
	  beq loop3end
	  b loop3
loop3end:


	b StopTest_host
	  nop
	  nop
	  nop
	  nop


/*****************************************
* Interrupt HANDLERS
*****************************************/
/*
* undefined_Handler
* swi_Handler
* prefetchAbort_Handler
* dataAbort_Handler
* irq_Handler
* fiq_Handler
*/


/*	END */


