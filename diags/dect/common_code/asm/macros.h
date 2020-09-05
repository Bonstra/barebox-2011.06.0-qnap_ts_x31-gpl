;--------------------------------------------------------------------------------------
;Write to 32-bit register (optimized - i.e destination base address is in a register (rN_Base)) 
;--------------------------------------------------------------------------------------

	MACRO
$label	WriteWordOpt $rN_Base, $RegOffsetAddr, $WrData

$label	ldr  r0, =$WrData
	str  r0, [$rN_Base, #$RegOffsetAddr]
      	MEND
;--------------------------------------------------------------------------
;Write to 32-bit register.
;--------------------------------------------------------------------------

	MACRO
$label	WriteWord $UnitBaseAddr, $RegOffsetAddr, $WrData

$label	ldr  r0, =$WrData
       	ldr  r1, =$UnitBaseAddr + $RegOffsetAddr
         str  r0, [r1]
      	MEND
;--------------------------------------------------------------------------
;Write to 32-bit register. with register r7 (reserved for connectivity tests)
;--------------------------------------------------------------------------

	MACRO
$label	WriteWordSpecial $UnitBaseAddr, $RegOffsetAddr

$label	ldr  r1, =$UnitBaseAddr + $RegOffsetAddr
        str  r7, [r1]
      	MEND
         
;--------------------------------------------------------------------------
;Write to 16-bit register (optimized - i.e destination base address is in a register (rN_Base))
;--------------------------------------------------------------------------

	MACRO
$label	WriteHWordOpt $rN_Base, $RegOffsetAddr, $WrData

$label	ldr   r0, =$WrData
	strh  r0, [$rN_Base, #$RegOffsetAddr]
      	MEND
         
;--------------------------------------------------------------------------
;Write to 16-bit register.
;--------------------------------------------------------------------------

	MACRO
$label	WriteHWord $UnitBaseAddr, $RegOffsetAddr, $WrData

$label	ldr   r0, =$WrData
       	ldr   r1, =$UnitBaseAddr + $RegOffsetAddr
         strh  r0, [r1]
      	MEND
         
;--------------------------------------------------------------------------
;Write to 8-bit register.
;--------------------------------------------------------------------------

	MACRO
$label	WriteByte $UnitBaseAddr, $RegOffsetAddr, $WrData

$label	ldr   r0, =$WrData
       	ldr   r1, =$UnitBaseAddr + $RegOffsetAddr
         strb  r0, [r1]
      	MEND

;--------------------------------------------------------------------------
;Write to 32-bit register.
; This Macro gets the $WrData field as a register (r1, r2 etc.)
;--------------------------------------------------------------------------

	MACRO
$label	WriteWord_r_param $UnitBaseAddr, $RegOffsetAddr, $WrData

$label	ldr  r0, =$UnitBaseAddr + $RegOffsetAddr
        str  $WrData, [r0]
      	MEND
         
;--------------------------------------------------------------------------
;Write to 16-bit register.
; This Macro gets the $WrData field as a register (r1, r2 etc.)
;--------------------------------------------------------------------------

	MACRO
$label	WriteHWord_r_param $UnitBaseAddr, $RegOffsetAddr, $WrData

$label	ldr   r0, =$UnitBaseAddr + $RegOffsetAddr
        strh  $WrData, [r0]
      	MEND
         
;--------------------------------------------------------------------------
;Write to 8-bit register.
; This Macro gets the $WrData field as a register (r1, r2  etc.)
;--------------------------------------------------------------------------

	MACRO
$label	WriteByte_r_param $UnitBaseAddr, $RegOffsetAddr, $WrData

$label	ldr   r0, =$UnitBaseAddr + $RegOffsetAddr
        strb  $WrData, [r0]
      	MEND

;--------------------------------------------------------------------------
;Read from 32-bit register (optimized - i.e destination base address is in a register (rN_Base)) 
;--------------------------------------------------------------------------

	MACRO 
$label	ReadWordOpt $rN_Base, $RegOffsetAddr
	ldr  r0, [$rN_Base, #$RegOffsetAddr]
      	MEND

;--------------------------------------------------------------------------
;Read from 32-bit register.
;--------------------------------------------------------------------------

	MACRO 
$label	ReadWord $UnitBaseAddr, $RegOffsetAddr
       	ldr  r1, =$UnitBaseAddr + $RegOffsetAddr
	      ldr  r0, [r1]
      	MEND


;--------------------------------------------------------------------------
;Read from 16-bit register (optimized - i.e destination base address is in a register (rN_Base)) 
;--------------------------------------------------------------------------

	MACRO 
$label	ReadHWordOpt $rN_Base, $RegOffsetAddr
	ldrh  r0, [$rN_Base, #$RegOffsetAddr]
      	MEND

;--------------------------------------------------------------------------
;Read from 16-bit register.
;--------------------------------------------------------------------------

	MACRO 
$label	ReadHWord $UnitBaseAddr, $RegOffsetAddr
       	ldr  r1, =$UnitBaseAddr + $RegOffsetAddr
	      ldrh r0, [r1]
      	MEND

;--------------------------------------------------------------------------
;Read from two consecutive 16-bit registers into Rx (Note - 1) Rx must not be r5, 2) As it inteend to support TB registers each register space is of 4 Byte-address)
;--------------------------------------------------------------------------

	MACRO 
$label	Read2HWord $UnitBaseAddr, $RegOffsetAddr, $rN_Base
       	ldr  r1, =$UnitBaseAddr + $RegOffsetAddr
	ldrh $rN_Base, [r1]
        add r1, #4
	ldrh r5, [r1]
        lsl r5,#16
        orr $rN_Base, r5
      	MEND

;--------------------------------------------------------------------------
;Read from 8-bit register.
;--------------------------------------------------------------------------

	MACRO 
$label	ReadByte $UnitBaseAddr, $RegOffsetAddr
       	ldr  r1, =$UnitBaseAddr + $RegOffsetAddr
	      ldrb r0, [r1]
      	MEND

;--------------------------------------------------------------------------
;Compare to 32-bit register value (optimized - i.e use pre-set registers rN_Base)
;--------------------------------------------------------------------------
	MACRO 
$label	CompareNeWordMaskImmOpt $rN_Base, $RegOffsetAddr, $ExpectedValue, $MaskValue       
        ldr  r1, =$MaskValue
        ldr  r2, =$ExpectedValue
        and  r2, r1
        mov  r0, #0xaec3
        ldr  r0, [$rN_Base, #$RegOffsetAddr]
        and  r0, r1
        cmp  r0, r2
        bne  ErrorTest 
      	MEND

;--------------------------------------------------------------------------
;Compare to 32-bit register value (optimized - i.e use pre-set registers rN_Base,rN_ExpectedValue,rN_MaskValue)
;--------------------------------------------------------------------------
	MACRO 
$label	CompareNeWordMaskOpt $rN_Base, $RegOffsetAddr, $rN_ExpectedValue, $rN_MaskValue       
        mov  r0, #0xaec3
        ldr  r0, [$rN_Base, #$RegOffsetAddr]
        and  r0, $rN_MaskValue
        and  $rN_ExpectedValue,$rN_MaskValue 
        cmp  r0, $rN_ExpectedValue
        bne  ErrorTest 
      	MEND

;--------------------------------------------------------------------------
;Compare to 32-bit register value ARM9 (optimized - i.e use pre-set registers rN_Base,rN_ExpectedValue,rN_MaskValue)
;--------------------------------------------------------------------------
	MACRO 
$label	CompareNeWordMaskOptA9 $rN_Base, $RegOffsetAddr, $rN_ExpectedValue, $rN_MaskValue       
        ldr  r0, =0xaec3
        ldr  r0, [$rN_Base, #$RegOffsetAddr]
        and  r0, $rN_MaskValue
        and  $rN_ExpectedValue,$rN_MaskValue 
        cmp  r0, $rN_ExpectedValue
        bne  ErrorTest 
      	MEND

;--------------------------------------------------------------------------
;Compare to 32-bit register value.
;--------------------------------------------------------------------------
	MACRO 
$label	CompareWord $UnitBaseAddr, $RegOffsetAddr, $ExpectedValue
	      ldr  r3, =0xadd1
       	ldr  r1, =$UnitBaseAddr + $RegOffsetAddr
	      ldr  r3, [r1]
      	ldr  r2, =$ExpectedValue
       	cmp  r2, r3
        BX	pc
	LTORG
         bne  ErrorTest 
      	MEND
;;NOTE(M): The BX pc and LTORG added to avoid 
;;Error: A1284E: Literal pool too distant, use LTORG to assemble it within 4KB

;--------------------------------------------------------------------------
;Compare to 16-bit register value.
;--------------------------------------------------------------------------
	MACRO 
$label	CompareHWord $UnitBaseAddr, $RegOffsetAddr, $ExpectedValue
	      ldr  r3, =0x4d1f
       	ldr  r1, =$UnitBaseAddr + $RegOffsetAddr
	      ldrh r3, [r1]
      	ldr  r2, =$ExpectedValue
       	cmp  r2, r3
         bne  ErrorTest 
      	MEND

;--------------------------------------------------------------------------
;Compare to 16-bit register value.
;--------------------------------------------------------------------------
	MACRO 
$label	CompareHWord_Host $UnitBaseAddr, $RegOffsetAddr, $ExpectedValue
	      ldr  r3, =0x4d1f
       	ldr  r1, =$UnitBaseAddr + $RegOffsetAddr
	      ldrh r3, [r1]
      	ldr  r2, =$ExpectedValue
       	cmp  r2, r3
         bne  ErrorTest_host 
      	MEND

;--------------------------------------------------------------------------
;Compare to 8-bit register value.
;--------------------------------------------------------------------------
	MACRO 
$label	CompareByte $UnitBaseAddr, $RegOffsetAddr, $ExpectedValue
	      ldr  r3, =0xadd1
       	ldr  r1, =$UnitBaseAddr + $RegOffsetAddr
	      ldrb r3, [r1]
      	ldr  r2, =$ExpectedValue
       	cmp  r2, r3
         bne  ErrorTest 
      	MEND

;--------------------------------------------------------------------------
; Comparing  16-bit register. With Mask
; $ExpectedValue Should be inserted as Const Value
;--------------------------------------------------------------------------

	MACRO ; 
$label	CompareNeHWordMask $UnitBaseAddr, $RegOffsetAddr, $ExpectedValue, $MaskValue
	ldr  r3, =0xCAFE
       	ldr  r1, =$UnitBaseAddr + $RegOffsetAddr
	ldrh r3, [r1]
      	ldr  r2, =$ExpectedValue
        ldr  r1, =$MaskValue
	and  r3, r3, r1
       	cmp  r2, r3
        bne  ErrorTest
      	MEND


;--------------------------------------------------------------------------
; Comparing  32-bit register. With Mask
; $ExpectedValue Should be inserted as Const Value
;--------------------------------------------------------------------------

	MACRO ; 
$label	CompareNeWordMask $UnitBaseAddr, $RegOffsetAddr, $ExpectedValue, $MaskValue
	ldr  r3, =0xCAFECAFE
       	ldr  r1, =$UnitBaseAddr + $RegOffsetAddr
	ldr r3, [r1]
      	ldr  r2, =$ExpectedValue
        ldr  r1, =$MaskValue
	and  r3, r3, r1
       	cmp  r2, r3
        bne  ErrorTest
      	MEND

;--------------------------------------------------------------------------
; Comparing  32-bit register. With Mask
; $ExpectedValue Should be inserted as Const Value
; mask value is r7
;--------------------------------------------------------------------------

	MACRO ; 
$label	CompareNeWordMaskSpecial $UnitBaseAddr, $RegOffsetAddr, $ExpectedValue, $MaskValue
	ldr  r3, =0xCAFECAFE
       	ldr  r1, =$UnitBaseAddr + $RegOffsetAddr
	ldr r3, [r1]
      	ldr  r2, =$ExpectedValue
	and  r3, r3, r7
       	cmp  r2, r3
        bne  ErrorTest
      	MEND

;--------------------------------------------------------------------------
;Compare to 32-bit register value.
; This Macro gets the $ExpectedValue field as a register (r1, r2 etc.)
;--------------------------------------------------------------------------
	MACRO 
$label	CompareWord_r_param $UnitBaseAddr, $RegOffsetAddr, $ExpectedValue
	      ldr  r3, =0x0000
       	ldr  r1, =$UnitBaseAddr + $RegOffsetAddr
	      ldr  r3, [r1]
       	cmp  $ExpectedValue, r3
         bne  ErrorTest 
      	MEND

;--------------------------------------------------------------------------
;Compare to 16-bit register value.
; This Macro gets the $ExpectedValue field as a register (r1, r2 etc.)
;--------------------------------------------------------------------------
	MACRO 
$label	CompareHWord_r_param $UnitBaseAddr, $RegOffsetAddr, $ExpectedValue
	      ldr  r3, =0x0000
       	ldr  r1, =$UnitBaseAddr + $RegOffsetAddr
	      ldrh r3, [r1]
       	cmp  $ExpectedValue, r3
         bne  ErrorTest 
      	MEND

;--------------------------------------------------------------------------
;Compare to 8-bit register value.
; This Macro gets the $ExpectedValue field as a register (r1, r2 etc.)
;--------------------------------------------------------------------------
	MACRO 
$label	CompareByte_r_param $UnitBaseAddr, $RegOffsetAddr, $ExpectedValue
	      ldr  r3, =0x0000
       	ldr  r1, =$UnitBaseAddr + $RegOffsetAddr
	      ldrb r3, [r1]
       	cmp  $ExpectedValue, r3
         bne  ErrorTest 
      	MEND

;--------------------------------------------------------------------------
;Polling register value - For/till ISR (interrupt service routine) changing the value
;--------------------------------------------------------------------------
	MACRO
$label	PollReg $Rexpected, $in
        ldr  $Rexpected, =0x0
PolR$in	cmp  $Rexpected, 0x0
        beq  PolR$in 
      	MEND

;--------------------------------------------------------------------------
;Polling 32-bit register.
;--------------------------------------------------------------------------
	MACRO
$label	PollWord $UnitBaseAddr, $RegOffsetAddr, $ExpectedValue, $in
PolW$in	ldr  r3, =0x0000
       	ldr  r1, =$UnitBaseAddr + $RegOffsetAddr
	      ldr  r3, [r1]
      	ldr  r2, =$ExpectedValue
       	cmp  r2, r3
         bne  PolW$in 
      	MEND

;--------------------------------------------------------------------------
;Polling 32-bit register with Mask 
;--------------------------------------------------------------------------
	MACRO
$label	PollMaskWord_Fix $UnitBaseAddr, $RegOffsetAddr, $ExpectedValue, $MaskValue, $in
        ldr  r3, =0x0000
       	ldr  r1, =$UnitBaseAddr + $RegOffsetAddr
      	ldr  r2, =$ExpectedValue
      	ldr  r4, =$MaskValue
PolW$in	ldr  r3, [r1]
	and  r3, r3, r4 
       	cmp  r2, r3
        bne  PolW$in 
      	MEND

;--------------------------------------------------------------------------
;Polling 32-bit register for set bits according to the ExpectedValue
;--------------------------------------------------------------------------
	MACRO
$label	PollMaskWord $UnitBaseAddr, $RegOffsetAddr, $ExpectedValue, $in
PolW$in	ldr  r3, =0x0000
       	ldr  r1, =$UnitBaseAddr + $RegOffsetAddr
        ldr  r3, [r1]
      	ldr  r2, =$ExpectedValue
	and  r3, r3, r2 
       	cmp  r2, r3
        bne  PolW$in 
      	MEND

;--------------------------------------------------------------------------
;Polling 16-bit register.
;--------------------------------------------------------------------------
	MACRO
$label	PollHWord $UnitBaseAddr, $RegOffsetAddr, $ExpectedValue, $in
PolHW$in	ldr  r3, =0x0000
       	ldr  r1, =$UnitBaseAddr + $RegOffsetAddr
	ldrh r3, [r1]
      	ldr  r2, =$ExpectedValue
       	cmp  r2, r3
         bne  PolHW$in 
      	MEND

;--------------------------------------------------------------------------
;Polling 16-bit register optimized - i.e the poll address is in a register
;--------------------------------------------------------------------------
	MACRO
$label	PollHWordOpt $rN_Base, $ExpectedValue, $label
PolHW$label	ldr  r3, =0x0000
	ldrh r3, [$rN_Base]
      	ldr  r2, =$ExpectedValue
       	cmp  r2, r3
         bne  PolHW$label 
      	MEND

;--------------------------------------------------------------------------
;Polling 16-bit register with mask
;--------------------------------------------------------------------------
	MACRO
$label	PollMaskHWord_Fix $UnitBaseAddr, $RegOffsetAddr, $ExpectedValue, $MaskValue, $in
        ldr  r3, =0x0000
       	ldr  r1, =$UnitBaseAddr + $RegOffsetAddr
      	ldr  r2, =$ExpectedValue
      	ldr  r4, =$MaskValue
PolHW$in ldrh r3, [r1]
	and  r3, r3, r4 
       	cmp  r2, r3
        bne  PolHW$in 
      	MEND

;--------------------------------------------------------------------------
;Polling 16-bit register for set bits according to the ExpectedValue
;--------------------------------------------------------------------------
	MACRO
$label	PollMaskHWord $UnitBaseAddr, $RegOffsetAddr, $ExpectedValue, $in
PolHW$in ldr  r3, =0x0000
       	ldr  r1, =$UnitBaseAddr + $RegOffsetAddr
	ldrh r3, [r1]
      	ldr  r2, =$ExpectedValue
	and  r3, r3, r2 
       	cmp  r2, r3
        bne  PolHW$in 
      	MEND

;--------------------------------------------------------------------------
;Polling 8-bit register.
;--------------------------------------------------------------------------
	MACRO
$label	PollByte $UnitBaseAddr, $RegOffsetAddr, $ExpectedValue, $in
PolB$in	ldr  r3, =0x0000
       	ldr  r1, =$UnitBaseAddr + $RegOffsetAddr
	ldrb r3, [r1]
      	ldr  r2, =$ExpectedValue
       	cmp  r2, r3
         bne  PolB$in 
      	MEND

;--------------------------------------------------------------------------
;Polling 16-bit register. With Mask
;This Macro gets the $ExpectedValue field as a register (r1, r2 etc.)
;--------------------------------------------------------------------------

	MACRO ;  Pool register value
$label	PollNeHRegMask $UnitBaseAddr, $RegOffsetAddr, $ExpectedValue, $MaskValue, $in
loop$in	ldr  r3, =0xCAFE
       	ldr  r1, =$UnitBaseAddr + $RegOffsetAddr
	ldrh r3, [r1]
      	mov  r2, $ExpectedValue
        mov  r1, $MaskValue
	and  r3, r3, r1
       	cmp  r2, r3
        bne  loop$in
      	MEND

;--------------------------------------------------------------------------
;Polling 32-bit register. Without Mask
;This Macro gets the $ExpectedValue field as a register (r1, r2 etc.)
;--------------------------------------------------------------------------

	MACRO ;  Pool register value
$label	PollNeReg $UnitBaseAddr, $RegOffsetAddr, $ExpectedValue, $in
loop$in	ldr  r3, =0xCAFE
       	ldr  r1, =$UnitBaseAddr + $RegOffsetAddr
	ldr r3, [r1]
      	mov  r2, $ExpectedValue
       	cmp  r2, r3
        bne  loop$in
      	MEND
;--------------------------------------------------------------------------
;Polling 8-bit register with mask bits.
;--------------------------------------------------------------------------
	MACRO
$label	PollMaskByte_Fix $UnitBaseAddr, $RegOffsetAddr, $ExpectedValue, $MaskValue, $in
	ldr  r3, =0x0000
       	ldr  r1, =$UnitBaseAddr + $RegOffsetAddr
      	ldr  r2, =$ExpectedValue
      	ldr  r4, =$MaskValue
PolB$in	ldrb r3, [r1]
	and  r3, r3, r4
       	cmp  r2, r3
        bne  PolB$in 
      	MEND

;--------------------------------------------------------------------------
;Polling 8-bit register for set bits according to the ExpectedValue
;--------------------------------------------------------------------------
	MACRO
$label	PollMaskByte $UnitBaseAddr, $RegOffsetAddr, $ExpectedValue, $in
PolB$in	ldr  r3, =0x0000
       	ldr  r1, =$UnitBaseAddr + $RegOffsetAddr
	ldrb r3, [r1]
      	ldr  r2, =$ExpectedValue
	and  r3, r3, r2
       	cmp  r2, r3
        bne  PolB$in 
      	MEND

;--------------------------------------------------------------------------
; Read 32-bit data from two locations and compare it.
; "NumBytes" - number of bytes to compare.
;--------------------------------------------------------------------------
	      MACRO
$label	MultipleCompare $Addr0Offset, $Addr1Offset, $NumBytes, $in
         ldr  r0, =$Addr0Offset
         ldr  r1, =$Addr1Offset
         ldr  r2, =$NumBytes
WaitEnd$in
         ldmia r0!,{r3}
         ldmia r1!,{r7}
         cmp r3, r7
         bne ErrorTest
         sub r2, r2, #0x4      
         cmp r2, #0x0      
         bne  WaitEnd$in 
 	      MEND

;--------------------------------------------------------------------------
;Set 32-bit register. (Note - here it assumes the U only want to set to 1 not to set to a value!!)
;--------------------------------------------------------------------------
	      MACRO
$label	SetFieldWord $BaseAddress, $OffsetAddress, $Data

         ldr  r0, =$BaseAddress+$OffsetAddress
         ldr  r1, [r0]
         ldr  r2, =$Data
         orr  r1, r1, r2
         str  r1, [r0]
	      MEND

;--------------------------------------------------------------------------
;Set 32-bit register. (Note - here it place the required value in the register according to a mask)
;--------------------------------------------------------------------------
	      MACRO
$label SetFieldWordMask $BaseAddress, $OffsetAddress, $Mask, $Data

         ldr r0, =$BaseAddress+$OffsetAddress
         ldr r1, [r0]
         ldr r3, =$Mask
         bic r1, r1, r3
         ldr r2, =$Data
         orr r1, r1, r2
         str r1, [r0]
	      MEND

;--------------------------------------------------------------------------
;Set protected 32-bit register. (Note - here it place the required value in the register according to a mask)
;--------------------------------------------------------------------------
	      MACRO
$label SetProtectFieldWordMask $BaseAddress, $OffsetAddress, $Mask, $Data, $ProtectOffset, $ProtectCode

         ldr r0, =$BaseAddress
         ldr r1, [r0,#$OffsetAddress]
         ldr r3, =$Mask
         bic r1, r1, r3
         ldr r2, =$Data
         orr r1, r1, r2
         ldr r2, =$ProtectCode
         str r2,[r0,#$ProtectOffset] ; Remove protection before write
         str r1, [r0,#$OffsetAddress]
	      MEND

;--------------------------------------------------------------------------
;Set 16-bit register.
;--------------------------------------------------------------------------
	      MACRO
$label	SetFieldHWord $BaseAddress, $OffsetAddress, $Data

         ldr   r0, =$BaseAddress+$OffsetAddress
         ldrh  r1, [r0]
         ldr   r2, =$Data
         orr   r1, r1, r2
         strh  r1, [r0]
	      MEND

;--------------------------------------------------------------------------
;Set 8-bit register.
;--------------------------------------------------------------------------
	      MACRO
$label	SetFieldByte $BaseAddress, $OffsetAddress, $Data

         ldr   r0, =$BaseAddress+$OffsetAddress
         ldrb  r1, [r0]
         ldr   r2, =$Data
         orr   r1, r1, r2
         strb  r1, [r0]
	      MEND


;--------------------------------------------------------------------------
;Clear 32-bit register.
;--------------------------------------------------------------------------
	      MACRO
$label	ClearFieldWord $BaseAddress, $OffsetAddress, $Data

         ldr  r0, =$BaseAddress+$OffsetAddress
         ldr  r1, [r0]
         ldr  r2, =$Data
         bic  r1, r1, r2
         str  r1, [r0]
	      MEND

;--------------------------------------------------------------------------
;Clear 32-bit register. r5 is inserted as data register
;--------------------------------------------------------------------------
	      MACRO
$label	ClearFieldWordSpecial $BaseAddress, $OffsetAddress

         ldr  r0, =$BaseAddress+$OffsetAddress
         ldr  r1, [r0]
         bic  r1, r1, r5
         str  r1, [r0]
	      MEND
;--------------------------------------------------------------------------
;Clear 16-bit register. 
;--------------------------------------------------------------------------
	      MACRO
$label	ClearFieldHWord $BaseAddress, $OffsetAddress, $Data

         ldr   r0, =$BaseAddress+$OffsetAddress
         ldrh  r1, [r0]
         ldr   r2, =$Data
         bic   r1, r1, r2
         strh  r1, [r0]
	      MEND
;--------------------------------------------------------------------------
;Clear 16-bit register. r5 is inserted as data register
;--------------------------------------------------------------------------
	      MACRO
$label	ClearFieldHWordSpecial $BaseAddress, $OffsetAddress

         ldr   r0, =$BaseAddress+$OffsetAddress
         ldrh  r1, [r0]
         bic   r1, r1, r5
         strh  r1, [r0]
	      MEND
;--------------------------------------------------------------------------
;Clear 8-bit register.
;--------------------------------------------------------------------------
	      MACRO
$label	ClearFieldByte $BaseAddress, $OffsetAddress, $Data

         ldr  r0, =$BaseAddress+$OffsetAddress
         ldrb r1, [r0]
         ldr  r2, =$Data
         bic  r1, r1, r2
         strb r1, [r0]
	      MEND

;--------------------------------------------------------------------------
;Check bits 32-bit register.
;--------------------------------------------------------------------------
	      MACRO
$label	CheckBitsWord $BaseAddress, $OffsetAddress, $Mask, $ExpectedValue

         ldr  r0, =$BaseAddress+$OffsetAddress
         ldr  r1, [r0]
         ldr  r0, =$Mask
         and  r1, r1, r0
         ldr  r2, =$ExpectedValue
       	 cmp  r1, r2
         bne  ErrorTest 
	      MEND

;--------------------------------------------------------------------------
;Check bits 32-bit register, expected value is inserted in r7
;--------------------------------------------------------------------------
	      MACRO
$label	CheckBitsWordSpecial $BaseAddress, $OffsetAddress, $Mask

         ldr  r0, =$BaseAddress+$OffsetAddress
         ldr  r1, [r0]
         ldr  r0, =$Mask
         and  r1, r1, r0
       	 cmp  r1, r7
         bne  ErrorTest 
	      MEND

;--------------------------------------------------------------------------
;Check bits 32-bit register with arm_reg (rX) register.
;--------------------------------------------------------------------------
	      MACRO
$label	CheckBitsWordReg $BaseAddress, $OffsetAddress, $Mask, $ExpectedValue

         ldr  r0, =$BaseAddress+$OffsetAddress
         ldr  r1, [r0]
         ldr  r0, =$Mask
         and  r1, r1, r0
         ldr  r2, $ExpectedValue
       	 cmp  r1, r2
         bne  ErrorTest 
	      MEND

;--------------------------------------------------------------------------
;Check bits 16-bit register.
;--------------------------------------------------------------------------
	      MACRO
$label	CheckBitsHWord $BaseAddress, $OffsetAddress, $Mask, $ExpectedValue

         ldr  r0, =$BaseAddress+$OffsetAddress
         ldrh r1, [r0]
         ldr  r0, =$Mask
         and  r1, r1, r0
         ldr  r2, =$ExpectedValue
       	 cmp  r1, r2
         bne  ErrorTest 
	      MEND

;--------------------------------------------------------------------------
;Check bits 8-bit register.
;--------------------------------------------------------------------------
	      MACRO
$label	CheckBitsByte $BaseAddress, $OffsetAddress, $Mask, $ExpectedValue

         ldr  r0, =$BaseAddress+$OffsetAddress
         ldrb r1, [r0]
         ldr  r0, =$Mask
         and  r1, r1, r0
         ldr  r2, =$ExpectedValue
       	 cmp  r1, r2
         bne  ErrorTest 
	      MEND
         
;;---------------------------------------------------------------*/
;;------   MACRO USED by ICU tests        -----------------------*/	
;;---------------------------------------------------------------*/
	MACRO
$label	GENERATE_INT_AND_THEN_CHECK_HANDLING $in
	;; increment the test number
	ADD r7, r7, #1	
	;; generate interrupt:
	ADR  r9, return_a_$in
	B    GENERATE_INTERRUPT
return_a_$in
	;; check that interrupt was serviced:
	ADR  r9, return_b_$in
	B    CHECK_INTERRUPT_WAS_RECEIVED
return_b_$in
      	MEND	
;;---------------------------------------------------------------*/

;--------------------------------------------------------------------------
;Clear 32-bit protected register.
;--------------------------------------------------------------------------
; $BaseAddress is assumed to be the same for both $OffsetAddress & $ProtectOffset

	MACRO
$label ClearProtectFieldWord $BaseAddress, $OffsetAddress, $Data, $ProtectOffset, $ProtectCode
        ldr r0, =$BaseAddress
        ldr r1, [r0,#$OffsetAddress]
        ldr r2, =$Data
        bic r1, r1, r2
        ldr r2, =$ProtectCode
        str r2,[r0,#$ProtectOffset] ; Remove protection before write
        str r1, [r0,#$OffsetAddress]
        MEND

;--------------------------------------------------------------------------
;Set 32-bit protected register.
;--------------------------------------------------------------------------
; $BaseAddress is assumed to be the same for both $OffsetAddress & $ProtectOffset
        MACRO
$label SetProtectFieldWord $BaseAddress, $OffsetAddress, $Data, $ProtectOffset, $ProtectCode
        ldr r0, =$BaseAddress
        ldr r1, [r0,#$OffsetAddress]
        ldr r2, =$Data
        orr r1, r1, r2
        ldr r2, =$ProtectCode
        str r2,[r0,#$ProtectOffset] ; Remove protection before write
        str r1, [r0,#$OffsetAddress]
        MEND

;--------------------------------------------------------------------------

;--------------------------------------------------------------------------
; Configure and Lock for PLL - according to the TB Pll control register (it is up to YOU to switch the Clock-SW to the right value), This MACRO is good for PLL2-4. Use EnPll1 for PLL1 switching
; Note - uses registers r6-r8
;--------------------------------------------------------------------------

	MACRO
$label	EnPll $TbPllXCntrlOffset, $CmuPllXCtrlOffset

     mov r9, #1
     ldr r10, =TopTbRegsBase ;; 
     ldr r11, =CmuBase ;;       

	; Reading the TB dedicated registers 
        mov r6,#0 ; re-writting the upper part default values + clearing the BP bit
        ldrh r6,[r10, #$TbPllXCntrlOffset]

	;; Store updated value into the PLL control register
	str r6, [r11, #$CmuPllXCtrlOffset]

	;; Clear the PLL PD bit - required to be done after the PLL configuration (the above store operation)
	  bic r6, r9, lsl #(ConstantCmuPllPdbit)
	  str r6, [r11, #$CmuPllXCtrlOffset]

	;; PLL2 Poll for lock
	;;====================
	;; Generating the expected value (setting the lock bit) 
	  ;	mov r6, r9,lsl #(ConstantCmuPllLockbit)


Pollpll_lock_$CmuPllXCtrlOffset	ldr r4, [r11, #$CmuPllXCtrlOffset]
	and r4,r9,lsl #(ConstantCmuPllLockbit) 
	cmp r4,#0 
	beq Pollpll_lock_$CmuPllXCtrlOffset
	     		    
      	MEND
          
;--------------------------------------------------------------------------
; Set DRAM Clock-SW to PLL3 
;--------------------------------------------------------------------------

	MACRO
$label	SW_Pll3 
        
	;; Switching the SYS Clock-SW to the PLL3 output
	;;===============================================
	;; enable DRAM clock
	SetProtectFieldWord   CmuBase,CmuSwCtrlClkEn2Offset, ConstantDramClkEn, CmuWrProtOffset, ConstantCmuClkEnWrpr 
        SetFieldWord CmuBase,CmuClkSwCtrlOffset , 0x00000001
      	MEND

;--------------------------------------------------------------------------
; Configure, Lock and SW for PLL3 
; Note - uses registers r6-r8
;--------------------------------------------------------------------------

	MACRO
$label	ENandSW_Pll3 
        
        ;; Configure and Lock for PLL - according to the TB Pll control register(it is up to YOU to switch the Clock-SW to the right value)
	EnPll TbPll3CntrlOffset, CmuPll3CtrlOffset

	;; Switching the SYS Clock-SW to the PLL3 output
	SW_Pll3

      	MEND


;--------------------------------------------------------------------------
; Configure and Lock for PLL - according to the TB Pll control register (it is up to YOU to switch the Clock-SW to the right value), This MACRO is good for PLL1 only!!!!
; Note - uses registers r6-r8
; Note - SYSSRC_NOR bits should be configured before entering this Macro
;--------------------------------------------------------------------------

	MACRO
$label	EnPll1BP $TbPllXCntrlOffset, $CmuPllXCtrlOffset

     mov r9, #1
     ldr r10, =TopTbRegsBase ;; 
     ldr r11, =CmuBase ;;       

	; Reading the TB dedicated registers 
        mov r6,#0 ; re-writting the upper part default values + clearing the BP bit
        ldrh r6,[r10, #$TbPllXCntrlOffset]

	;; Store updated value into the PLL control register
	str r6, [r11, #$CmuPllXCtrlOffset]

	;; Clear the PLL PD bit - required to be done after the PLL configuration (the above store operation)
	ClearFieldWord CmuBase,CmuCpuClkCtrlOffset,ConstantCmuPll1Pdbit_nor

	;; PLL1 Poll for lock
	;;====================


Pollpll_lock_$CmuPllXCtrlOffset	ldr r4, [r11, #$CmuPllXCtrlOffset]
	and r4,r9,lsl #(ConstantCmuPllLockbit) 
	cmp r4,#0 
	beq Pollpll_lock_$CmuPllXCtrlOffset
	     		    
      	MEND
          
;;----------------------------------------------------------------*/
;;------   MACRO USED by LCDC tests        -----------------------*/	
;;----------------------------------------------------------------*/

;--------------------------------------------------------------------------
;Polling 16-bit register with mask bits on the 8 msb for done .
; and check 8 lsb bits for 8 errors;
;--------------------------------------------------------------------------
        MACRO
$label  PollDoneErr $UnitBaseAddr, $RegOffsetAddr, $ExpectedValue, $in
PolW$in ldr  r3, =0x0000
        ldr  r1, =$UnitBaseAddr + $RegOffsetAddr
        ldrh  r3, [r1]
        ldr  r2, =$ExpectedValue
        and  r3, r3, r2
        cmp  r2, r3
        bne  PolW$in ; wait for done

        ;; check for errors after done by clean high bits and check error on 8 lsb bits
        ldr  r3, =0x0000
        ldrh  r3, [r1]
        and  r3, r3, #0x00ff
        cmp  r3, #0x0000 ; if no errors
        bne     ErrorTest
        MEND


;--------------------------------------------------------------------------
; read mem data with add on 16 msb and data on 16 lsb
; write data into the address location with base offset;
; This new Macro handles transactions of data within the test
; using DCD.
;--------------------------------------------------------------------------
 
	MACRO
$label  ConfigDatMem $StartLabel, $RegOffsetAddr
        ldr  r1, =$StartLabel
        ldr  r3, =0x0000
        ldr  r4, =0x0000
        ldr  r5, =0x0000
        ldr  r6, =0x0000
loop_wrr ldr  r6, [r1]     ; Address
        add r1, r1, #0x4
        mov r4,r6, lsr #16 ; check if we at end
        ldr  r5, =0x0000ffff
        and  r5, r5, r6  
        ldr  r6, =0xffff
        cmp  r4, r6 ; if no ffff
        beq  wrr_done
	nop
	nop
	nop
	ldr  r3, [r1]      ; Data
        add  r1, r1, #0x4
	and  r3, r3, r6 
        nop

        ldr  r2, =$RegOffsetAddr 
        add r2, r2, r5 
        strh  r3, [r2]
        b  loop_wrr
        nop

wrr_done nop
        nop
	MEND
      


;--------------------------------------------------------------------------
; read mem data with add on 16 msb and data on 16 lsb
; write data into the address location with base offset;
;--------------------------------------------------------------------------
        MACRO
$label  ConfigRegMem $UnitBaseAddr, $MemOffsetAddr ,$RegOffsetAddr
        ldr  r1, =$UnitBaseAddr + $MemOffsetAddr
        ldr  r4, =0x0000
        ldr  r5, =0x0000
        ldr  r6, =0x0000
loop_wr ldr  r6, [r1]
        add r1, r1, #0x4
        mov r4,r6, lsr #16 ; save add into r4
        ldr  r5, =0x0000ffff
        and  r5, r5, r6  ; save data into r5
        ldr  r6, =0xffff
        cmp  r4, r6 ; if no ffff
        beq  wr_done
        nop

        ldr  r2, =$RegOffsetAddr 
        add r2, r2, r4 
        strh  r5, [r2]
        b  loop_wr
        nop

wr_done nop
        nop

        MEND



;--------------------------------------------------------------------------
; read mem data with add on 16 msb and data on 16 lsb
; wait for ack before  next write
; write data into the address location with base offset;
;--------------------------------------------------------------------------
        MACRO
$label  ConfigCpuMem $UnitBaseAddr, $MemOffsetAddr ,$RegOffsetAddr
        ldr  r1, =$UnitBaseAddr + $MemOffsetAddr
        ldr  r4, =0x0000
        ldr  r5, =0x0000
        ldr  r6, =0x0000
loop_wr1 ldr  r6, [r1]
        add r1, r1, #0x4
        mov r4,r6, lsr #16 ; save add into r4
        ldr  r5, =0x0000ffff
        and  r5, r5, r6  ; save data into r5
        ldr  r6, =0xffff
        cmp  r4, r6 ; if no ffff
        beq  wr_done1
        nop

        ldr  r2, =$RegOffsetAddr
        add r2, r2, r4 ; real add to write

       ; r2 is the address and r5 is the data

        ldr  r6, [r1]
        add r1, r1, #0x4
        mov r4,r6, lsr #16 ; save add into r4
        ldr  r7, =0x0000ffff
        and  r7, r7, r6  ; save data into r7
        ldr  r6, =0xffff
        cmp  r4, r6 ; if no ffff
        beq  wr_done1
        nop

       ; r7 has the data 


waitfull ldr  r3, =$RegOffsetAddr + LcdcIstar1Regoffset ; cpucmd status reg address  
        ldr  r6, [r3]
        ldr  r8, =0x00000002
        and  r6, r6, r8  ; mask bits  
        ldr  r4, =0x0002
        cmp  r4, r6 ; check bit 1 TXCMDF for not full , eq 0
        beq  waitfull
        nop

        strh  r5, [r2]
        add r2, r2, #0x4
        strh  r7, [r2]
        b  loop_wr1
        nop

wr_done1 nop
        nop
waitempty
	ldr  r3, =$RegOffsetAddr + LcdcIstar1Regoffset ; cpucmd status reg address  
        ldr  r4, =0x00000001
        ldr  r6, [r3]
        and  r6, r6, r4 
        cmp  r4, r6 ; check bit 0 TXCMDF for empty , bit eq 1
        bne  waitempty
        nop
        nop

        MEND

; -------------------------------------------------------------
; Read pairs of (destination_address,destination_32bit_word) from a given address
; and write destination_address <- destination_data
; stop condition is when address equals a given value
; -------------------------------------------------------------
	MACRO
$label	MemToMem $Pairs_start_addr,  $Destination_Base,  $Stop_value_addr, $inst

$label  stmfd  sp!, {r6-r8}    ; save regs
	mov r6, $Pairs_start_addr
	mov r7, $Destination_Base
	mov r8, $Stop_value_addr
mem_to_mem_loop_$inst
	ldr r0, [r6]
	cmp r0, r8
	beq end_of_mem_to_mem_loop_$inst
	mov r1, r0
	add r6, r6, #0x4
	ldr r0, [r6]
	add r6, r6, #0x4
	str r0, [r7,r1]
	bl mem_to_mem_loop_$inst
end_of_mem_to_mem_loop_$inst
	add r6, r6, #0x8	; skip the (0xFFFFFFFF,0xFFFFFFFF) pair
	mov r0, r6
	LDMFD  sp!, {r6-r8}
	MEND



; -------------------------------------------------------------
; This macro checks the memories.
; each bit in the HWORD $mask is for different element
; $unique_label is any non-spaced string.
; bit 13 ($mask == 0x2000) is for DDR0 checks
; -------------------------------------------------------------
	MACRO
$label External_mem_compare   $mask,  $unique_label
        WriteHWord TopTbRegsBase, TbCheckDataGoOffset,     $mask		;; start compare DDR0
        PollHWord  TopTbRegsBase, TbCheckDataFinishOffset, $mask, $unique_label ;; wait for compare task to finish
	CompareNeHWordMask TopTbRegsBase, TbCheckDataRsltOffset, 0x0, $mask     ;; check the status of the comparison
        WriteHWord TopTbRegsBase, TbCheckDataFinishOffset, 0x0                  ;; clear TB finish register
	MEND


      	END

 
