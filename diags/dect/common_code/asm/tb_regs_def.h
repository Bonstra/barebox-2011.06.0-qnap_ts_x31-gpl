;------------------
; Top_tb_regs constants
;------------------

;the following constants are used in scu_test_end.s file found in tests/arm_code/asm

;the first 2 costants are used to generate an address in ext memory that is used for ending the test as mentioned in the readme file
; you can change the address to whatever fits you

;; Defining the base address for all the Test-Bench registers 
TopTbRegsBase         EQU 0x830ff000 ;; Upper 4KB
TopTbRegsBase_Exp         EQU 0xc01ff000 ;; Upper 4KB

; Defining the offset from tb base                                                     
CssTubeOffset                           EQU 0x00000FF8                


; Defining the word to be written to the above address in case of a successfull run                                                      
TestEndConstant                         EQU 0x900DD009

; Defining the word to be written to the above address in case of an error                                                      
TestFailConstant                        EQU 0xBAADBAAD

DbgAckAnalyzerCtlOffset  EQU 0x0e0

McuCnctAnCtrlOffset  EQU  0xA80
McuCnctAnStat01Offset EQU 0xA84
McuCnctAnStat02Offset EQU 0xA88
McuCnctAnStat03Offset EQU 0xA8c
McuCnctAnStat04Offset EQU 0xA90

	END

