; *******************************************************
; Initialize stack pointer registers (in different modes)
; *******************************************************

; standard definitions of mode bits and interrupts in PSR
;--------------------------------------------------------
mode_usr		EQU	0x10
mode_fiq		EQU	0x11
mode_irq		EQU	0x12
mode_svc		EQU	0x13
mode_abt		EQU	0x17
mode_und		EQU	0x1b
mode_sys  		EQU	0x1f
i_bit			EQU	0x80	; when I bit is set, IRQ is disabled
f_bit			EQU	0x40	; when F bit is set, IRQ is disabled

; initialize all stack pointer modes locations & sizes
;-----------------------------------------------------
;top_of_stacks		EQU	0x307ffffc
;;yb top_of_stacks		EQU	0x3ffc
;;top_of_stacks		EQU	0x07fc
top_of_stacks		EQU	0x07ffc

len_fiq_stack		EQU	0x20
len_irq_stack		EQU	0x20
len_abt_stack		EQU	0x20
len_und_stack		EQU	0x20
len_svc_stack		EQU	0x20
len_sys_stack		EQU	0x20
len_usr_stack		EQU	0x20

offset_fiq_stack	EQU	0
offset_irq_stack	EQU	offset_fiq_stack + len_irq_stack
offset_abt_stack	EQU	offset_irq_stack + len_abt_stack
offset_und_stack	EQU	offset_abt_stack + len_und_stack
offset_svc_stack	EQU	offset_und_stack + len_svc_stack
offset_sys_stack	EQU	offset_svc_stack + len_sys_stack
offset_usr_stack	EQU	offset_sys_stack + len_usr_stack

hsel_high               EQU     0x04
main_osc_sel            EQU     0x01

RfExtP1Bit              EQU     0x04
RfExtP2Bit              EQU     0x08
	END
