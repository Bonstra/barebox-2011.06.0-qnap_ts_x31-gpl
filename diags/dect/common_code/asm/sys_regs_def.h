/*
* this file contain defines used in the arm tests code
*/

/*--------------------------------------------
* CSS DTCM memory address
*/
#define ScuCssDtcm						 0x00100000  /*as seen from inside css*/
#define McuCssDtcm						 0x9e100000  /*as seen from outside css*/

/*--------------------------------------------
* CSS ITCM memory address 
*/
#define ScuCssItcm						 0x00300000  /*as seen from inside css */
#define McuCssItcm						 0x9e000000  /*as seen from outside css*/
#define ScuCssItcmExt						 0x9e000000  /*as seen from outside css*/



/*--------------------------------------------
* external memory address - DDR 
*/
#define ExternalMemBase 				 0x00000000
#define ExternalMemBase_4 				 0x40000000
#define ExternalMemBase_1MB				 0x00100000


/*--------------------------------------------
* external memory address - Shared Memory
*/
#define SharedMemoryBase 	       		 0x83000000



/*--------------------------------------------
* Icu - Interrupt Control Unit Constants 
*/
#define CssIcuBase 				 0x9ea00000
#define CssIcuBaseFromScu 				 0x08a00000

#define IcuChClrInt1Offset                       0x0000
#define IcuChClrInt2Offset                       0x0004   
#define IcuChEdgeLevel1Offset                    0x0008
#define IcuChEdgeLevel2Offset                    0x000c
#define IcuChSwTrigSet1Offset                    0x0010
#define IcuChSwTrigSet2Offset                    0x0014
#define IcuChReqOffset                           0x0018
#define IcuStatus1Offset                         0x001c 
#define IcuStatus2Offset                         0x0020
#define IcuPendingReq1Offset                     0x0024
#define IcuPendingReq2Offset                     0x0028
#define IcuCause1Offset                          0x002c
#define IcuCause2Offset                          0x0030
#define IcuChMask1Offset                         0x0034
#define IcuChMask2Offset                         0x0038
#define IcuSetChMask1Offset                      0x003c 
#define IcuSetChMask2Offset                      0x0040
#define IcuClrChMask1Offset                      0x0044
#define IcuClrChMask2Offset                      0x0048
#define IcuChPriority1Offset                     0x004c
#define IcuChPriority2Offset                     0x0050
#define IcuChPriority3Offset                     0x0054
#define IcuChPriority4Offset                     0x0058
#define IcuChPriority5Offset                     0x005c
#define IcuChPriority6Offset                     0x0060
#define IcuChPriority7Offset                     0x0064
#define IcuChPriority8Offset                     0x0068
#define IcuFiqSel1Offset                         0x006c
#define IcuFiqSel2Offset                         0x0070
#define IcuFiqPendingReq1Offset                  0x0074
#define IcuFiqPendingReq2Offset                  0x0078
#define IcuFiqCasue1Offset                       0x007c
#define IcuFiqCasue2Offset                       0x0080
#define IcuPriMaskOffset                         0x0084
#define IcuPriMaskFiqOffset                      0x0088

#define ConstantCssTmr1int           0x00000200
#define ConstantCssTmr2int           0x00000008 
#define ConstantCssGdmacint          0x00000008  /*IcuCause1Offset*/
#define ConstantCssGdmacErrint       0x00000004  /*IcuCause1Offset*/

#define ConstantSccSecint  	         0x00000002 


/*-----------------------------------------------
* GDMAC - Generic DMA registers 
*/

#define CssGdmacBase                             0x08800000
#define CssGdmacBaseFromCortex                   0x9e800000

#define GdmacCemsrOffset                         0x0000
#define GdmacCedrOffset                          0x0004
#define GdmacCdrOffset                           0x0008
#define GdmacCtorOffset                          0x000C
#define GdmacCcierOffset                         0x0010
#define GdmacCdierOffset                         0x0014
#define GdmacCcicrOffset                         0x0018
#define GdmacCdicrOffset                         0x001C
#define GdmacCciclrOffset                        0x0020
#define GdmacCdiclrOffset                        0x0024
#define GdmacCrdrOffset                          0x0028
#define GdmacCtoenrOffset                        0x002C
#define GdmacCterenrOffset                       0x0030
#define GdmacCtocrOffset                         0x0034
#define GdmacCtecrOffset                         0x0038
#define GdmacCtoclrOffset                        0x003C
#define GdmacCteclrOffset                        0x0040
#define GdmacCrconf1Offset                       0x0044
#define GdmacCrconf2Offset                       0x0048
#define GdmacCrconf3Offset                       0x004C
#define GdmacCrconf4Offset                       0x0050
#define GdmacCrconf5Offset                       0x0054
#define GdmacCrconf6Offset                       0x0058
#define GdmacCdprOffset                          0x005C
#define GdmacC0descadrOffset                     0x0060
#define GdmacC1descadrOffset                     0x0064
#define GdmacC2descadrOffset                     0x0068
#define GdmacC3descadrOffset                     0x006C
#define GdmacC4descadrOffset                     0x0070
#define GdmacC5descadrOffset                     0x0074
#define GdmacC6descadrOffset                     0x0078
#define GdmacC7descadrOffset                     0x007C
#define GdmacC8descadrOffset                     0x0080
#define GdmacC9descadrOffset                     0x0084
#define GdmacC10descadrOffset                    0x0088
#define GdmacC11descadrOffset                    0x008C
#define GdmacC12descadrOffset                    0x0090
#define GdmacC13descadrOffset                    0x0094
#define GdmacC14descadrOffset                    0x0098
#define GdmacC15descadrOffset                    0x009C
#define GdmacC16descadrOffset                    0x00A0
#define GdmacC17descadrOffset                    0x00A4
#define GdmacC18descadrOffset                    0x00A8
#define GdmacC19descadrOffset                    0x00AC
#define GdmacC20descadrOffset                    0x00B0
#define GdmacC21descadrOffset                    0x00B4
#define GdmacC22descadrOffset                    0x00B8
#define GdmacC23descadrOffset                    0x00BC
#define GdmacC24descadrOffset                    0x00C0
#define GdmacC25descadrOffset                    0x00C4
#define GdmacC26descadrOffset                    0x00C8
#define GdmacC27descadrOffset                    0x00CC
#define GdmacC28descadrOffset                    0x00D0
#define GdmacC29descadrOffset                    0x00D4
#define GdmacC30descadrOffset                    0x00D8
#define GdmacC31descadrOffset                    0x00DC
#define GdmacC0rtcOffset                         0x00E0
#define GdmacC1rtcOffset                         0x00E4
#define GdmacC2rtcOffset                         0x00E8
#define GdmacC3rtcOffset                         0x00EC
#define GdmacC4rtcOffset                         0x00F0
#define GdmacC5rtcOffset                         0x00F4
#define GdmacC6rtcOffset                         0x00F8
#define GdmacC7rtcOffset                         0x00FC
#define GdmacC8rtcOffset                         0x0100
#define GdmacC9rtcOffset                         0x0104
#define GdmacC10rtcOffset                        0x0108
#define GdmacC11rtcOffset                        0x010C
#define GdmacC12rtcOffset                        0x0110
#define GdmacC13rtcOffset                        0x0114
#define GdmacC14rtcOffset                        0x0118
#define GdmacC15rtcOffset                        0x011C
#define GdmacC16rtcOffset                        0x0120
#define GdmacC17rtcOffset                        0x0124
#define GdmacC18rtcOffset                        0x0128
#define GdmacC19rtcOffset                        0x012C
#define GdmacC20rtcOffset                        0x0130
#define GdmacC21rtcOffset                        0x0134
#define GdmacC22rtcOffset                        0x0138
#define GdmacC23rtcOffset                        0x013C
#define GdmacC24rtcOffset                        0x0140
#define GdmacC25rtcOffset                        0x0144
#define GdmacC26rtcOffset                        0x0148
#define GdmacC27rtcOffset                        0x014C
#define GdmacC28rtcOffset                        0x0150
#define GdmacC29rtcOffset                        0x0154
#define GdmacC30rtcOffset                        0x0158
#define GdmacC31rtcOffset                        0x015C
#define GdmacTefldrOffset                        0x0160
#define GdmacTepAdrOffset                        0x0164
#define GdmacTepBdrOffset                        0x0168
#define GdmacMfcsdrOffset                        0x016C
#define GdmacDsrOffset				 0x0170


/*-------------------------------------------
* Timers Constants
*/

#define CssTimers12Base				 0x08900000   
#define CssTimers12BaseFromMcu			 0x9e900000   

#define Tmr1loadOffset        			 0x0000  
#define Tmr1valueOffset        			 0x0004    
#define Tmr1controlOffset     			 0x0008   
#define Tmr1intclrOffset       			 0x000c  
#define Tmr1risOffset         			 0x0010
#define Tmr1misOffset         			 0x0014
#define Tmr1bgloadOffset      			 0x0018

#define Tmr2loadOffset        			 0x0020  
#define Tmr2valueOffset        			 0x0024    
#define Tmr2controlOffset     			 0x0028   
#define Tmr2intclrOffset       			 0x002c  
#define Tmr2risOffset         			 0x0030
#define Tmr2misOffset         			 0x0034
#define Tmr2bgloadOffset      			 0x0038

#define Timers34Base				 0x05900000   

#define Tmr3loadOffset        			 0x0000  
#define Tmr3valueOffset        			 0x0004    
#define Tmr3controlOffset     			 0x0008   
#define Tmr3intclrOffset       			 0x000c  
#define Tmr3risOffset         			 0x0010
#define Tmr3misOffset         			 0x0014
#define Tmr3bgloadOffset      			 0x0018

#define Tmr4loadOffset        			 0x0020  
#define Tmr4valueOffset        			 0x0024    
#define Tmr4controlOffset     			 0x0028   
#define Tmr4intclrOffset       			 0x002c  
#define Tmr4risOffset         			 0x0030
#define Tmr4misOffset         			 0x0034
#define Tmr4bgloadOffset      			 0x0038

/*
* general timers constants
*/
#define ConstantTimerEnable                      0x0080
#define ConstantTimerMode                        0x0040
#define ConstantTimerIntEnable                   0x0020
#define ConstantTimerPrescale                    0x000c
#define ConstantTimerPrescaleDiv16               0x0004
#define ConstantTimerPrescaleDiv256              0x0008
#define ConstantTimerSize                        0x0002
#define ConstantTimerOneShot                     0x0001

#define ConstantTimerRIS                         0x0001
#define ConstantTimerMIS                         0x0001
#define ConstantFreeRunningCounterOneClockTimout  1024
#define ConstantTimerFreeRunning                 0x0000
#define ConstantLSBbit                           0x01




/*--------------------------------------------
* CSS sys cfg regs
*/

#define CssSysCfgBase							 0x9eb00000 
#define CssSysCfgBaseFromScu					 0x8b00000 
	
#define CssSysCfgOutCntrlRegOffset				 0x0000 
#define CssSysCfgScPadRegOffset					 0x0004 
#define CssSysCfgTestModeRegOffset				 0x0008 
/*--------------------------------------------
* C2K SEMAPHORE REGISTER SPACE
*/
#define SemaphoreBaseAddr	 0x91000000 
#define Semaphore1Offset         0x0000
#define Semaphore2Offset         0x0004
#define Semaphore3Offset         0x0008
#define Semaphore4Offset         0x000C
#define Semaphore5Offset         0x0010
#define Semaphore6Offset         0x0014
#define Semaphore7Offset         0x0018
#define Semaphore8Offset         0x001C
#define Semaphore9Offset         0x0020
#define Semaphore10Offset        0x0024
#define Semaphore11Offset        0x0028
#define Semaphore12Offset        0x002C
#define Semaphore13Offset        0x0030
#define Semaphore14Offset        0x0034
#define Semaphore15Offset        0x0038
#define Semaphore16Offset        0x003C
#define Semaphore17Offset        0x0040
#define Semaphore18Offset        0x0044
#define Semaphore19Offset        0x0048
#define Semaphore20Offset        0x004C
#define Semaphore21Offset        0x0050
#define Semaphore22Offset        0x0054
#define Semaphore23Offset        0x0058
#define Semaphore24Offset        0x005C
#define Semaphore25Offset        0x0060
#define Semaphore26Offset        0x0064
#define Semaphore27Offset        0x0068
#define Semaphore28Offset        0x006C
#define Semaphore29Offset        0x0070
#define Semaphore30Offset        0x0074
#define Semaphore31Offset        0x0078
#define Semaphore32Offset        0x007C
#define Semaphore33Offset        0x0080
#define Semaphore34Offset        0x0084
#define Semaphore35Offset        0x0088
#define Semaphore36Offset        0x008C
#define Semaphore37Offset        0x0090
#define Semaphore38Offset        0x0094
#define Semaphore39Offset        0x0098
#define Semaphore40Offset        0x009C
#define Semaphore41Offset        0x00A0
#define Semaphore42Offset        0x00A4
#define Semaphore43Offset        0x00A8
#define Semaphore44Offset        0x00AC
#define Semaphore45Offset        0x00B0
#define Semaphore46Offset        0x00B4
#define Semaphore47Offset        0x00B8
#define Semaphore48Offset        0x00BC
#define Semaphore49Offset        0x00C0
#define Semaphore50Offset        0x00C4
#define Semaphore51Offset        0x00C8
#define Semaphore52Offset        0x00CC
#define Semaphore53Offset        0x00D0
#define Semaphore54Offset        0x00D4
#define Semaphore55Offset        0x00D8
#define Semaphore56Offset        0x00DC
#define Semaphore57Offset        0x00E0
#define Semaphore58Offset        0x00E4
#define Semaphore59Offset        0x00E8
#define Semaphore60Offset        0x00EC
#define Semaphore61Offset        0x00F0
#define Semaphore62Offset        0x00F4
#define Semaphore63Offset        0x00F8
#define Semaphore64Offset        0x00FC
/*--------------------------------------------
*--------------------------------------------
* A9-GICU CONSTATNTS
*--------------------------------------------
* PERIPH_BASE 	 0xFFF00000
* IC_DIST_BASE	 0xFFF01000
* IC_INT_BASE	 0xFFF00100
*/

/*	END */
