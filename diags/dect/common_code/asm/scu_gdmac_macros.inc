;;--------------------------------------
;; Transfer Mode register
;;--------------------------------------
;; 31               direction 				
A2B_direction		EQU	0x00000000
B2A_direction		EQU	0x80000000

;; 30               owner					
SW_owner		EQU	0x00000000
GDMAC_owner		EQU	0x40000000

;; 29               interrupt				
disable_interrupt		EQU	0x00000000
enable_interrupt		EQU	0x20000000

;; 28               TimeOut int				
disable_T_Out_int		EQU	0x00000000
enable_T_Out_int	EQU	0x10000000

;; 27-25			 Transfer Mode 			
mode_SW_Block_at_once					EQU	0x00000000
mode_Chained_Block_At_Once				EQU	0x02000000
mode_HW_Burst_request_single			EQU	0x04000000
mode_HW_Burst_request_consecutiv_modee	EQU	0x06000000
mode_2D_Image_Swap						EQU	0x08000000
mode_2D_Image_Rotate					EQU	0x0A000000
mode_Reserved110						EQU	0x0C000000
mode_Reserved111						EQU	0x0E000000

;; 24				X
;; 23-22	TmpBuf size
TmpBufSize_256					EQU	0x00000000
TmpBufSize_512					EQU	0x00400000
TmpBufSize_1024					EQU	0x00800000
TmpBufSize_2049					EQU	0x00C00000

;; 21		TmpBuf Port
TmpBufPort_A					EQU	0x00000000
TmpBufPort_B					EQU	0x00200000

;; 20		"HW FIFO PORT" 
WhFifoPort_A					EQU	0x00000000
WhFifoPort_B					EQU	0x00100000

;; 19-18	"HW Data Size/ 2D bpp" 
HwDataSize_2Dbpp_Byte					EQU	0x00000000
HwDataSize_2Dbpp_16bit					EQU	0x00040000
HwDataSize_2Dbpp_32bit					EQU	0x00080000
HwDataSize_2Dbpp_reserved32				EQU	0x000C0000

;; 17-13	Transfer length
TransLen_1	EQU	0x00000000
TransLen_2	EQU	0x00002000
TransLen_3	EQU	0x00004000
TransLen_4	EQU	0x00006000
TransLen_5	EQU	0x00008000
TransLen_6	EQU	0x0000A000
TransLen_7	EQU	0x0000C000
TransLen_8	EQU	0x0000E000
TransLen_9	EQU	0x00010000
TransLen_10	EQU	0x00012000
TransLen_11	EQU	0x00014000
TransLen_12	EQU	0x00016000
TransLen_13	EQU	0x00018000
TransLen_14	EQU	0x0001A000
TransLen_15	EQU	0x0001C000
TransLen_16	EQU	0x0001E000
TransLen_17	EQU	0x00020000
TransLen_18	EQU	0x00022000
TransLen_19	EQU	0x00024000
TransLen_20	EQU	0x00026000
TransLen_21	EQU	0x00028000
TransLen_22	EQU	0x0002A000
TransLen_23	EQU	0x0002C000
TransLen_24	EQU	0x0002E000
TransLen_25	EQU	0x00030000
TransLen_26	EQU	0x00032000
TransLen_27	EQU	0x00034000
TransLen_28	EQU	0x00036000
TransLen_29	EQU	0x00038000
TransLen_30	EQU	0x0003A000
TransLen_31	EQU	0x0003C000
TransLen_32	EQU	0x0003E000

;; 12		"Chained channel trigger"
trigger_nminus	EQU	0x00000000
trigger_nplus	EQU	0x00001000

;; 11-10	2D Rotate mode
Rotate_mode2D_90ccw		EQU	0x00000000
Rotate_mode2D_90cw		EQU	0x00000400
Rotate_mode2D_180		EQU	0x00000800
Rotate_mode2D_reserved	EQU	0x00000C00

;; 9		"2D Swap mode"
Swap_mode2D_Hor		EQU	0x00000000
Swap_mode2D_Ver		EQU	0x00000200

;; 8		"2D Image port"
Image_Port2D_A		EQU	0x00000000
Image_Port2D_B		EQU	0x00000100

;; 7		X
;; 6-2		"# of consecutive HW addresses"
Num_of_HwAddress_1	EQU	0x00000000
Num_of_HwAddress_2	EQU	0x00000004
Num_of_HwAddress_3	EQU	0x00000008
Num_of_HwAddress_4	EQU	0x0000000C
Num_of_HwAddress_5	EQU	0x00000010
Num_of_HwAddress_6	EQU	0x00000014
Num_of_HwAddress_7	EQU	0x00000018
Num_of_HwAddress_8	EQU	0x0000001C
Num_of_HwAddress_9	EQU	0x00000020
Num_of_HwAddress_10	EQU	0x00000024
Num_of_HwAddress_11	EQU	0x00000028
Num_of_HwAddress_12	EQU	0x0000002C
Num_of_HwAddress_13	EQU	0x00000030
Num_of_HwAddress_14	EQU	0x00000034
Num_of_HwAddress_15	EQU	0x00000038
Num_of_HwAddress_16	EQU	0x0000003C
Num_of_HwAddress_17	EQU	0x00000040
Num_of_HwAddress_18	EQU	0x00000044
Num_of_HwAddress_19	EQU	0x00000048
Num_of_HwAddress_20	EQU	0x0000004C
Num_of_HwAddress_21	EQU	0x00000050
Num_of_HwAddress_22	EQU	0x00000054
Num_of_HwAddress_23	EQU	0x00000058
Num_of_HwAddress_24	EQU	0x0000005C
Num_of_HwAddress_25	EQU	0x00000060
Num_of_HwAddress_26	EQU	0x00000064
Num_of_HwAddress_27	EQU	0x00000068
Num_of_HwAddress_28	EQU	0x0000006C
Num_of_HwAddress_29	EQU	0x00000070
Num_of_HwAddress_30	EQU	0x00000074
Num_of_HwAddress_31	EQU	0x00000078
Num_of_HwAddress_32	EQU	0x0000007C


;; 1		Done
done		EQU	0x00000000
not_done	EQU	0x00000002

;; 0		X


;;--------------------------------------
;; Channels decoding
;;--------------------------------------
ConstantGdmacChan0				EQU	0x00000001
ConstantGdmacChan1				EQU	0x00000002
ConstantGdmacChan2				EQU	0x00000004
ConstantGdmacChan3				EQU	0x00000008
ConstantGdmacChan4				EQU	0x00000010
ConstantGdmacChan5				EQU	0x00000020
ConstantGdmacChan6				EQU	0x00000040
ConstantGdmacChan7				EQU	0x00000080
ConstantGdmacChan8				EQU	0x00000100
ConstantGdmacChan9				EQU	0x00000200
ConstantGdmacChan10				EQU	0x00000400
ConstantGdmacChan11				EQU	0x00000800
ConstantGdmacChan12				EQU	0x00001000
ConstantGdmacChan13				EQU	0x00002000
ConstantGdmacChan14				EQU	0x00004000
ConstantGdmacChan15				EQU	0x00008000
ConstantGdmacChan16				EQU	0x00010000
ConstantGdmacChan17				EQU	0x00020000
ConstantGdmacChan18				EQU	0x00040000
ConstantGdmacChan19				EQU	0x00080000
ConstantGdmacChan20				EQU	0x00100000
ConstantGdmacChan21				EQU	0x00200000
ConstantGdmacChan22				EQU	0x00400000
ConstantGdmacChan23				EQU	0x00800000
ConstantGdmacChan24				EQU	0x01000000
ConstantGdmacChan25				EQU	0x02000000
ConstantGdmacChan26				EQU	0x04000000
ConstantGdmacChan27				EQU	0x08000000
ConstantGdmacChan28				EQU	0x10000000
ConstantGdmacChan29				EQU	0x20000000
ConstantGdmacChan30				EQU	0x40000000
ConstantGdmacChan31				EQU	0x80000000



             END

