//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^//
//THIS FILE CONTAINS THE C-TEST DEFINES USED IN MULTIPLE FUNCTIONS
//================================================================//
//The following defines represents different segmented regions of 
//DDR for dumping pkts payload from different IP's.
//Currently only 256MB DDR2 memory implemented in TB.
#define  DDR2_MEMOY_START_ADDR         0x00000000
#define  DDR2_MEMORY_END_ADDR          0x0FFFFFFF
//Currently only 512MB DDR3 memory implemented in TB.
#define  DDR3_MEMOY_START_ADDR         0x00000000
#define  DDR3_MEMORY_END_ADDR          0x1FFFFFFF
//The C2K supported IRAM size is 64KB
#define  IRAM_MEMORY_START_ADDR        0x83000000
#define  IRAM_MEMORY_END_ADDR          0x8300FFFF
//================================================================//
//The C2K SOC usb3 controller register config base address.
#define USB3_CONTROLLER_REG_CFG_BASE_ADDRESS 	((u32)(0x9F000000))
//=============================================================================================//
// The following defines your system memory starting address allocated for DWC_usb3 DMA purpose.
// The TRB, Data-Buffers, and Event-Queue will be build here. The data buffers are placed here first.
// The "number_of_bytes" variable in the tests and the TRB and EventQ size should not exceed 
// the allocated memory in this region.
//#define SYS_MEMORY_BASE_ADDRESS 					((u32)(0x83000000))   //IRAM space
#define SYS_MEMORY_BASE_ADDRESS 					((u32)(0x10000000))   //DDR Space
// The following defines represent the segemented regions of shared memory used for 
// different command executions for DWC_usb3 as a xHC 
#define xHC_SYS_MEMORY_Transfer_TRB_OFFSET_ADDRESS                      ((u32)(0x00001000))
#define xHC_SYS_MEMORY_Command_TRB_OFFSET_ADDRESS                       ((u32)(0x00002000))
#define xHC_SYS_MEMORY_Event_TRB_OFFSET_ADDRESS                         ((u32)(0x00003000))
#define xHC_SYS_MEMORY_Device_Context_Base_Address_Array_OFFSET_ADDRESS ((u32)(0x00004000))
#define xHC_SYS_MEMORY_Device_Context_OFFSET_ADDRESS                    ((u32)(0x00004800))
//#define xHC_SYS_MEMORY_Stream_Context_OFFSET_ADDRESS                    ((u32)(0x00006100))
//#define xHC_SYS_MEMORY_Stream_Array_OFFSET_ADDRESS                      ((u32)(0x00006200))
#define xHC_SYS_MEMORY_Transfer_Ring_Segment_OFFSET_ADDRESS             ((u32)(0x00005000))
#define xHC_SYS_MEMORY_Event_Ring_Segment_OFFSET_ADDRESS                ((u32)(0x00006000))
#define xHC_SYS_MEMORY_Command_Ring_Segment_OFFSET_ADDRESS              ((u32)(0x00007000))
#define xHC_SYS_MEMORY_Event_Ring_Segment_Table_OFFSET_ADDRESS          ((u32)(0x00008000))
#define xHC_SYS_MEMORY_Input_Context_OFFSET_ADDRESS                     ((u32)(0x00009000))
#define xHC_SYS_MEMORY_Scratch_Pad_Array_ADDRESS                        ((u32)(0x00009800))
#define xHC_SYS_MEMORY_Scratch_Pad_BUF0_ADDRESS                         ((u32)(0x00009C00))
#define xHC_SYS_MEMORY_Data_BULKOUT_BUF_ADDRESS                         ((u32)(0x0000A000))
#define xHC_SYS_MEMORY_Data_BULKIN_BUF_ADDRESS                          ((u32)(0x0000B000))
//=============================================================================================//
//----------------------------------------------------------------------//
//The value shoud be multiple of 4 bytes.
#define     	USB3_NUM_OF_BULKOUT_BYTES_TO_BE_XFERED        ((u32)(0x400)) //1KB 
#define 	USB3_NUM_OF_BULKOUT_TRBS			1
#define     	USB3_NUM_OF_BULKIN_BYTES_TO_BE_XFERED         ((u32)(0x400)) //1KB
#define 	USB3_NUM_OF_BULKIN_TRBS				1
//----------------------------------------------------------------------//
