
#define 	USB3p0_CFG_BASE_ADDR 	0x9F000000
//oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo//
// USB30 CONTROLLER REGISTERS ADDRESS MAP
//oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo//
/*
Address 	Index 		Register Type
0x0_0000 to 0x0_7FFF xHCI Registers (Xxxxx)
   -- 0 to CAPLENGTH-1 		 -- eXtensible Host Controller Capability Registers
   -- CAPLENGTH to RTSOFF - 1	 -- Host Controller Operational Registers
 -- RTSOFF to DBOFF - 1 		 -- Controller Runtime Registers
 -- DBOFF to (xECP*4-1) 		 -- Doorbell Register
 -- xECP*4 to 0x0_7FFF 		 -- xHCI Extended Capabilities
0x0_C100 to 0x0_C6FF Global Registers (Gxxxx)
0x0_C700 to 0x0_CBFF Device Registers (Dxxxx)
0x0_CC00 to 0x0_CCFF USB 2.0 OTG and Battery Charger Registers (Oxxxx/Axxxx/Bxxxx)
0x0_CD00 to 0x0_CFFF Unused/Aliased
0x4_0000 to 0x7_FFFF Internal RAM 0 - Debug Access (256KB)
0x8_0000 to 0xB_FFFF Internal RAM 1 - Debug Access (256KB)
0xC_0000 to 0xF_FFFF Internal RAM 2 - Debug Access (256KB) 
*/
//oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo//
//The following defines are taken from the DWC_usb3_params.v file.
#define 	USB3p0_HC_CAPLENGTH  	0x20   
#define 	USB3p0_HC_RTSOFF 	0x440 //(= decimal 1088)
#define 	USB3p0_HC_DBOFF 	0x480 //(= decimal 1152)
#define 	USB3p0_HC_XECP 	0x220 //(= decimal 544)
//oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo//

//888888888888888888888888888888888888888888888888888888888//
//**************Host Registers Map************************//
//888888888888888888888888888888888888888888888888888888888//
//-------------------------------------------------//
// eXtensible Host Controller Capability Registers
// Base = 0x0000
//-------------------------------------------------//
#define 	USB3p0_xHC_CAP_BASE_ADDR  	(USB3p0_CFG_BASE_ADDR + 0x0000) 
#define 	USB3p0_xHC_CAPLENGTH_REG_ADDR	(USB3p0_xHC_CAP_BASE_ADDR + 0x0000) 
#define 	USB3p0_xHC_HCSPARAMS1_REG_ADDR  (USB3p0_xHC_CAP_BASE_ADDR + 0x0004)  
#define 	USB3p0_xHC_HCSPARAMS2_REG_ADDR  (USB3p0_xHC_CAP_BASE_ADDR + 0x0008) 
#define 	USB3p0_xHC_HCSPARAMS3_REG_ADDR  (USB3p0_xHC_CAP_BASE_ADDR + 0x000C)  
#define		USB3p0_xHC_HCCPARAMS_REG_ADDR  	(USB3p0_xHC_CAP_BASE_ADDR + 0x0010)  
#define 	USB3p0_xHC_DBOFF_REG_ADDR  	(USB3p0_xHC_CAP_BASE_ADDR + 0x0014)  
#define		USB3p0_xHC_RTSOFF_REG_ADDR  	(USB3p0_xHC_CAP_BASE_ADDR + 0x0018) 
//0x001C Rsvd
//-------------------------------------------------//
// Host Controller Operational Registers
// Operatiobal Base = Base+CAPLENTH = 0x0020
//-------------------------------------------------//
#define 	USB3p0_xHC_OP_BASE_ADDR 	(USB3p0_xHC_CAP_BASE_ADDR + USB3p0_HC_CAPLENGTH)
#define 	USB3p0_xHC_USBCMD_REG_ADDR 	(USB3p0_xHC_OP_BASE_ADDR + 0x0000)  
#define 	USB3p0_xHC_USBSTS_REG_ADDR	(USB3p0_xHC_OP_BASE_ADDR + 0x0004) 
#define 	USB3p0_xHC_PAGESIZE_REG_ADDR	(USB3p0_xHC_OP_BASE_ADDR + 0x0008) 
//0x000C-0x0013 Rsvd Rsvd:32'h0
#define 	USB3p0_xHC_DNCTRL_REG_ADDR 	(USB3p0_xHC_OP_BASE_ADDR + 0x0014)  
#define 	USB3p0_xHC_CRCR_REG_ADDR 	(USB3p0_xHC_OP_BASE_ADDR + 0x0018)  
#define 	USB3p0_xHC_CRCR1_REG_ADDR 	(USB3p0_xHC_OP_BASE_ADDR + 0x001C)  
//0x0020-0x002F Rsvd Rsvd:32'h0
#define 	USB3p0_xHC_DCBAAP_REG_ADDR 	(USB3p0_xHC_OP_BASE_ADDR + 0x0030)  
	                                                                            
#define 	USB3p0_xHC_DCBAAP1_REG_ADDR 	(USB3p0_xHC_OP_BASE_ADDR + 0x0034)  
#define 	USB3p0_xHC_CONFIG_REG_ADDR	(USB3p0_xHC_OP_BASE_ADDR + 0x0038)  
//0x003C-0x03FF Rsvd Rsvd:32'h0
//-------------------------------------------------//
// Host Controller Port Register Set
//Port_base =Base + CAPLENGTH + 400h = 0x0420
//-------------------------------------------------//
#define 	USB3p0_xHC_PORT_BASE_ADDR      	 (USB3p0_xHC_CAP_BASE_ADDR + USB3p0_HC_CAPLENGTH + 0x400)
#define 	USB3p0_xHC_PORTSC_REG_ADDR 	 (USB3p0_xHC_PORT_BASE_ADDR + 0x0000) 
#define 	USB3p0_xHC_PORTPMSC_SS1_REG_ADDR (USB3p0_xHC_PORT_BASE_ADDR + 0x0004)
#define 	USB3p0_xHC_PORTPMSC_U21_REG_ADDR (USB3p0_xHC_PORT_BASE_ADDR + 0x0004)
#define 	USB3p0_xHC_PORTLI_REG_ADDR 	 (USB3p0_xHC_PORT_BASE_ADDR + 0x0008)
//0x000C Rsvd Rsvd:32'h0
//-------------------------------------------------//
//Host Controller Runtime Registers
// Run time base = Base + RTSOFF  = 0x0000+0x440= 0x440
//-------------------------------------------------//
#define 	USB3p0_xHC_RUN_BASE_ADDR	(USB3p0_xHC_CAP_BASE_ADDR + USB3p0_HC_RTSOFF)
#define 	USB3p0_xHC_MFINDEX_REG_ADDR	(USB3p0_xHC_RUN_BASE_ADDR + 0x0000) 
//0x0004 Rsvd
//-------------------------------------------------//
//Interrupter Registers
// Interrupt_base = Base + RTSOFF + 20h = 0x0000+0x440+0x20 = 0x460
//-------------------------------------------------//
#define 	USB3p0_xHC_INTR_BASE_ADDR	(USB3p0_xHC_CAP_BASE_ADDR + USB3p0_HC_RTSOFF + 0x20)
#define		USB3p0_xHC_IMAN_REG_ADDR	(USB3p0_xHC_INTR_BASE_ADDR + 0x0000)  
#define		USB3p0_xHC_IMOD_REG_ADDR	(USB3p0_xHC_INTR_BASE_ADDR + 0x0004)  
#define		USB3p0_xHC_ERSTSZ_REG_ADDR	(USB3p0_xHC_INTR_BASE_ADDR + 0x0008)  
//0x000C Rsvd 
#define		USB3p0_xHC_ERSTBA_REG_ADDR	(USB3p0_xHC_INTR_BASE_ADDR + 0x0010)  
#define		USB3p0_xHC_ERSTBA1_REG_ADDR	(USB3p0_xHC_INTR_BASE_ADDR + 0x0014)  
#define		USB3p0_xHC_ERDP_REG_ADDR	(USB3p0_xHC_INTR_BASE_ADDR + 0x0018)  
#define		USB3p0_xHC_ERDP1_REG_ADDR	(USB3p0_xHC_INTR_BASE_ADDR + 0x001C)  
//-------------------------------------------------//
//Doorbell Register
//Door bell base = Base + DBOFF = 0x0000+0x480 = 0x480
//-------------------------------------------------//
#define 	USB3p0_xHC_DB_BASE_ADDR		(USB3p0_xHC_CAP_BASE_ADDR + USB3p0_HC_DBOFF) 
#define		USB3p0_xHC_DB_REG_ADDR	        (USB3p0_xHC_DB_BASE_ADDR + 0x0000)
//-------------------------------------------------//
//xHCI Extended Capabilities
//xECP =544
//xECP *4 = 544*2= 2176
//Extended capability base = Base + xECP * 4 USB Legacy Support Capability = 0x0000+ 0x220*4=0x880
//-------------------------------------------------//
#define 	USB3p0_xHC_EXTND_CAP_BASE_ADDR	(USB3p0_xHC_CAP_BASE_ADDR + (USB3p0_HC_XECP * 4))
#define 	USB3p0_xHC_USBLEGSUP_REG_ADDR   (USB3p0_xHC_EXTND_CAP_BASE_ADDR + 0x0000) 
#define 	USB3p0_xHC_USBLEGCTLSTS_REG_ADDR (USB3p0_xHC_EXTND_CAP_BASE_ADDR + 0x0004)
//-------------------------------------------------//
//Addr1 xHCI Supported Protocol Capability (USB 2.0)
//-------------------------------------------------//
/*0x0000 SUPTPRT2_DW0 Major Revision:8’h2 Minor Revision:8’h0 Next Capability Pointer Capability ID:8’h2 FF RAM
0x0004 SUPTPRT2_DW1 Name String:“USB “ FF RAM
0x0008 SUPTPRT2_DW2 Rsvd:15'h0 n0 Compatible Port Count Compatible Port Offset FF RAM*/
//-------------------------------------------------//
//Addr2 xHCI Supported Protocol Capability (USB 3.0)
//-------------------------------------------------//
/*0x0000 SUPTPRT3_DW0 Major Revision:8’h3 Minor Revision:8’h0 Next Capability Pointer Capability ID:8’h2 FF RAM
0x0004 SUPTPRT3_DW1 Name String:“USB “ FF RAM
0x0008 SUPTPRT3_DW2 Rsvd:15'h0 n0 Compatible Port Count Compatible Port Offset FF RAM*/

//###############################################//
//	USB3p0 CONTROLLER Global Registers Map	//
//###############################################//
//GLOBAL REGISTERS
#define		USB3p0_GSBUSCFG0_REG_ADDR	(USB3p0_CFG_BASE_ADDR + 0xC100)  
#define		USB3p0_GSBUSCFG1_REG_ADDR	(USB3p0_CFG_BASE_ADDR + 0xC104)  
#define		USB3p0_GTXTHRCFG_REG_ADDR	(USB3p0_CFG_BASE_ADDR + 0xC108)  
#define		USB3p0_GRXTHRCFG_REG_ADDR	(USB3p0_CFG_BASE_ADDR + 0xC10C)  
#define		USB3p0_GCTL_REG_ADDR		(USB3p0_CFG_BASE_ADDR + 0xC110)  
#define		USB3p0_GEVTEN_REG_ADDR		(USB3p0_CFG_BASE_ADDR + 0xC114)  
#define		USB3p0_GSTS_REG_ADDR		(USB3p0_CFG_BASE_ADDR + 0xC118)  
//0xC11C <Reserved> Rsvd:32'h0
#define		USB3p0_GSNPSID_REG_ADDR		(USB3p0_CFG_BASE_ADDR + 0xC120)  //SynopsysID:32'h5533185A HW BUS
#define		USB3p0_GGPIO_REG_ADDR		(USB3p0_CFG_BASE_ADDR + 0xC124)  //GPO:16'h0 GPI:16'h0 FF BUS
#define		USB3p0_GUID_REG_ADDR		(USB3p0_CFG_BASE_ADDR + 0xC128)  //UserID FF BUS
#define		USB3p0_GUCTL_REG_ADDR		(USB3p0_CFG_BASE_ADDR + 0xC12C)  
#define		USB3p0_GBUSERRADDR_31_0_REG_ADDR (USB3p0_CFG_BASE_ADDR + 0xC130) //31:0] BusAddrLo[31:0]:’h0 FF RAM
#define		USB3p0_GBUSERRADDR_63_32_REG_ADDR (USB3p0_CFG_BASE_ADDR + 0xC134) //63:32] BusAddrHi[63:32]:’h0 FF RAM
#define		USB3p0_GPRTBIMAP_31_0_REG_ADDR	(USB3p0_CFG_BASE_ADDR + 0xC138) //31:0] Port-BIMapping[31:0] FF RAM
#define		USB3p0_GPRTBIMAP_63_32_REG_ADDR	(USB3p0_CFG_BASE_ADDR + 0xC13C) //63:32] Port-BIMapping[63:32] FF RAM
#define		USB3p0_GHWPARAMS0_REG_ADDR	(USB3p0_CFG_BASE_ADDR + 0xC140)  //coreConsultant parameters HW RAM
#define		USB3p0_GHWPARAMS1_REG_ADDR	(USB3p0_CFG_BASE_ADDR + 0xC144)  //coreConsultant parameters HW RAM
#define		USB3p0_GHWPARAMS2_REG_ADDR	(USB3p0_CFG_BASE_ADDR + 0xC148)  //coreConsultant parameters HW RAM
#define		USB3p0_GHWPARAMS3_REG_ADDR	(USB3p0_CFG_BASE_ADDR + 0xC14C)  //coreConsultant parameters HW RAM
#define		USB3p0_GHWPARAMS4_REG_ADDR	(USB3p0_CFG_BASE_ADDR + 0xC150)  //coreConsultant parameters HW RAM
#define		USB3p0_GHWPARAMS5_REG_ADDR	(USB3p0_CFG_BASE_ADDR + 0xC154)  //coreConsultant parameters HW RAM
#define		USB3p0_GHWPARAMS6_REG_ADDR	(USB3p0_CFG_BASE_ADDR + 0xC158)  //coreConsultant parameters HW RAM
#define		USB3p0_GHWPARAMS7_REG_ADDR	(USB3p0_CFG_BASE_ADDR + 0xC15C)  //coreConsultant parameters HW RAM
#define		USB3p0_GDBGFIFOSPACE_REG_ADDR	(USB3p0_CFG_BASE_ADDR + 0xC160)  //SpaceAvailable: Tx/Rx FIFO Depth Rsvd:8'h0
       										 //FIFO/QueueSelect or Port-Select:8'h0 HW RAM
#define		USB3p0_GDBGLTSSM_REG_ADDR	(USB3p0_CFG_BASE_ADDR + 0xC164)  
#define		USB3p0_GDBGLNMCC_REG_ADDR	(USB3p0_CFG_BASE_ADDR + 0xC168)  //Rsvd LNMCC_BERC HW RAM
#define		USB3p0_GDBGBMU_REG_ADDR		(USB3p0_CFG_BASE_ADDR + 0xC16C)  //BMU_BCU Debug information j1 j0 HW RAM
#define		USB3p0_GDBGLSPMUX_REG_ADDR	(USB3p0_CFG_BASE_ADDR + 0xC170)  
#define		USB3p0_GDBGLSP_REG_ADDR		(USB3p0_CFG_BASE_ADDR + 0xC174)  //LSP Debug Information HW RAM
#define		USB3p0_GDBGEPINFO0_REG_ADDR	(USB3p0_CFG_BASE_ADDR + 0xC178)  //Endpoint Debug Information[31:0] HW RAM
#define		USB3p0_GDBGEPINFO1_REG_ADDR	(USB3p0_CFG_BASE_ADDR + 0xC17C)  //Endpoint Debug Information[63:32] HW RAM
#define		USB3p0_GPRTBIMAP_HS_31_0_REG_ADDR (USB3p0_CFG_BASE_ADDR + 0xC180) //31:0] Port-BIMapping_HS[31:0] FF RAM
#define		USB3p0_GPRTBIMAP_HS_63_32_REG_ADDR (USB3p0_CFG_BASE_ADDR + 0xC184) //63:32] Port-BIMapping_HS[63:32] FF RAM

#define		USB3p0_GPRTBIMAP_FS_31_0_REG_ADDR  (USB3p0_CFG_BASE_ADDR + 0xC188) //31:0] Port-BIMapping_FS[31:0] FF RAM
#define		USB3p0_GPRTBIMAP_FS_63_32_REG_ADDR (USB3p0_CFG_BASE_ADDR + 0xC18C) //63:32] Port-BIMapping_FS[63:32] FF RAM
#define		USB3p0_GUSB2PHYCFG_0_REG_ADDR	(USB3p0_CFG_BASE_ADDR + 0xC200) 
#define		USB3p0_GUSB2PHYCFG_15_REG_ADDR	(USB3p0_CFG_BASE_ADDR + 0xC23C) 
#define		USB3p0_GUSB2I2CCTL_0_REG_ADDR	(USB3p0_CFG_BASE_ADDR + 0xC240) 
#define		USB3p0_GUSB2I2CCTL_15_REG_ADDR	(USB3p0_CFG_BASE_ADDR + 0xC27C) 
#define		USB3p0_GUSB2PHYACC_0_REG_ADDR	(USB3p0_CFG_BASE_ADDR + 0xC280) 
#define		USB3p0_GUSB2PHYACC_15_REG_ADDR	(USB3p0_CFG_BASE_ADDR + 0xC2BC) 
#define		USB3p0_GUSB3PIPECTL_0_REG_ADDR	(USB3p0_CFG_BASE_ADDR + 0xC2C0) 
#define		USB3p0_GUSB3PIPECTL_15_REG_ADDR	(USB3p0_CFG_BASE_ADDR + 0xC2FC) 
#define		USB3p0_GTXFIFOSIZ_0_REG_ADDR	(USB3p0_CFG_BASE_ADDR + 0xC300) //0) TxFStAddr_n: Derived from coreConsultant parameters
										//TxFDep_n: Derived from coreConsultant parameters FF RAM
#define		USB3p0_GTXFIFOSIZ_31_REG_ADDR	(USB3p0_CFG_BASE_ADDR + 0xC37C) //31) TxFStAddr_n: Derived from coreConsultant parameters
										//TxFDep_n: Derived from coreConsultant parameters FF RAM
#define		USB3p0_GRXFIFOSIZ_0_REG_ADDR	(USB3p0_CFG_BASE_ADDR + 0xC380) //0) RxFStAddr_n: Derived from coreConsultant parameters 
										//RxFDep_n: Derived from coreConsultant parameters FF RAM
#define		USB3p0_GRXFIFOSIZ_REG_ADDR	(USB3p0_CFG_BASE_ADDR + 0xC3FC) //31) RxFStAddr_n: Derived from coreConsultant parameters
										//RxFDep_n: Derived from coreConsultant parameters FF RAM
#define		USB3p0_GEVNTADR_31_0_0_REG_ADDR	(USB3p0_CFG_BASE_ADDR + 0xC400) 
#define		USB3p0_GEVNTADR_63_32_0_REG_ADDR (USB3p0_CFG_BASE_ADDR + 0xC404) 
#define		USB3p0_GEVNTSIZ_0_REG_ADDR	(USB3p0_CFG_BASE_ADDR + 0xC408) 
#define		USB3p0_GEVNTCOUNT_0_REG_ADDR	(USB3p0_CFG_BASE_ADDR + 0xC40C) 
#define		USB3p0_GEVNTADR_31_0_x_REG_ADDR	(USB3p0_CFG_BASE_ADDR + 0xC5F0) 
#define		USB3p0_GEVNTADR_63_32_x_REG_ADDR (USB3p0_CFG_BASE_ADDR + 0xC5F4) 
#define		USB3p0_GEVNTSIZ_x_REG_ADDR	(USB3p0_CFG_BASE_ADDR + 0xC5F8) 
#define		USB3p0_GEVNTCOUNT_x_REG_ADDR	(USB3p0_CFG_BASE_ADDR + 0xC5FC) 
#define		USB3p0_GHWPARAMS8_REG_ADDR	(USB3p0_CFG_BASE_ADDR + 0xC600)  //coreConsultant parameters HW RAM
//###############################################//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
//DEVICE REGISTER MAP
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
//DEVICE REGISTERS
#define		USB3p0_DCFG_REG_ADDR	(USB3p0_CFG_BASE_ADDR + 0xC700)  //Rsvd:8’h0 a3 a2 NumP:5'h4 IntrNum:5'h0 a1 DevAddr:7'h0 a0 FF BUS
#define		USB3p0_DCTL_REG_ADDR	(USB3p0_CFG_BASE_ADDR + 0xC704)  //b9 b8 b7 HIRD_Thres:5'h0 b6 Rs TrgtULSt:4'h0 Rsvd:4’h0 b5 b4 b3 b2 b1 b0 Rs FF BUS
#define		USB3p0_DEVTEN_REG_ADDR	(USB3p0_CFG_BASE_ADDR + 0xC708)  //Rsvd:19'h0 c10 c9 c8 c7 Rs c6 c5 Rs c4 c3 c2 c1 c0 FF BUS
#define		USB3p0_DSTS_REG_ADDR	(USB3p0_CFG_BASE_ADDR + 0xC70C)  //Rsvd:7’h0 d5 d4 d3 d2 d1 SOFFN:14'h0 d0 FF BUS
#define		USB3p0_DGCMDPAR_REG_ADDR	(USB3p0_CFG_BASE_ADDR + 0xC710)  //Parameter:32’h0 FF RAM
#define		USB3p0_DGCMD_REG_ADDR	(USB3p0_CFG_BASE_ADDR + 0xC714)  //Rsvd:16'h0 e2 Rsvd:4’h0 e1 Rs e0 CmdTyp:8'h0 FF RAM
//0xC718 <Reserved> Rsvd:32'h0
//0xC71C <Reserved> Rsvd:32'h0
#define		USB3p0_DALEPENA_REG_ADDR	(USB3p0_CFG_BASE_ADDR + 0xC720)  //USBActEP[31-0]:32'h0 FF RAM
//0xC724-0xC7FC <Reserved> Rsvd
#define		USB3p0_DEPCMDPAR2_0_REG_ADDR	(USB3p0_CFG_BASE_ADDR + 0xC800) //0) Parameter 2 RAM RAM
#define		USB3p0_DEPCMDPAR1_0_REG_ADDR	(USB3p0_CFG_BASE_ADDR + 0xC804) //0) Parameter 1 RAM RAM
#define		USB3p0_DEPCMDPAR0_0_REG_ADDR	(USB3p0_CFG_BASE_ADDR + 0xC808) //0) Parameter 0 RAM RAM
#define		USB3p0_DEPCMD_0_REG_ADDR	(USB3p0_CFG_BASE_ADDR + 0xC80C) //0)
#define		USB3p0_DEPCMDPAR2_31_REG_ADDR	(USB3p0_CFG_BASE_ADDR + 0xC9F0) //31) Parameter 2 RAM RAM
#define		USB3p0_DEPCMDPAR1_31_REG_ADDR	(USB3p0_CFG_BASE_ADDR + 0xC9F4) //31) Parameter 1 RAM RAM
#define		USB3p0_DEPCMDPAR0_31_REG_ADDR	(USB3p0_CFG_BASE_ADDR + 0xC9F8) //31) Parameter 0 RAM RAM
#define		USB3p0_DEPCMD_31_REG_ADDR	(USB3p0_CFG_BASE_ADDR + 0xC9FC) //31)
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//USB 2.0 OTG and Battery Charger Registers Map
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//USB 2.0 OTG AND BATTERY CHARGER REGISTERS
#define		USB3p0_OCFG_REG_ADDR	(USB3p0_CFG_BASE_ADDR + 0xCC00)  
#define		USB3p0_OCTL_REG_ADDR	(USB3p0_CFG_BASE_ADDR + 0xCC04)  
#define		USB3p0_OEVT_REG_ADDR	(USB3p0_CFG_BASE_ADDR + 0xCC08)  
#define		USB3p0_OEVTEN_REG_ADDR	(USB3p0_CFG_BASE_ADDR + 0xCC0C)  
#define		USB3p0_OSTS_REG_ADDR	(USB3p0_CFG_BASE_ADDR + 0xCC10)  
#define		USB3p0_ADPCFG_REG_ADDR	(USB3p0_CFG_BASE_ADDR + 0xCC20)  
#define		USB3p0_ADPCTL_REG_ADDR	(USB3p0_CFG_BASE_ADDR + 0xCC24)  
#define		USB3p0_ADPEVT_REG_ADDR	(USB3p0_CFG_BASE_ADDR + 0xCC28)  
#define		USB3p0_ADPEVTEN_REG_ADDR (USB3p0_CFG_BASE_ADDR + 0xCC2C)  
#define		USB3p0_BCFG_REG_ADDR	(USB3p0_CFG_BASE_ADDR + 0xCC30)  
#define		USB3p0_BCEVT_REG_ADDR	(USB3p0_CFG_BASE_ADDR + 0xCC38)  
#define		USB3p0_BCEVTEN_REG_ADDR	(USB3p0_CFG_BASE_ADDR + 0xCC3C) 
//000000000000000000000000000000000000000000000000000000000000000000//
//**********USB3 SUBSYSTEM PHY CONFIGURATION REGISTERS**************//
//000000000000000000000000000000000000000000000000000000000000000000//
#define	USB3p0_PHY_CFG_BASE_ADDR 		0x904A0000
// USB3p0 PHY CR ACCESS CONTROLL REGISTER
#define USB3p0_PHY_CR_ACC_CNTRL_REG_ADDR	( USB3p0_PHY_CFG_BASE_ADDR + 0x0)
// USB3p0 PHY CR ACCESS STATUS REGISTER
#define USB3p0_PHY_CR_ACC_STS_REG_ADDR		( USB3p0_PHY_CFG_BASE_ADDR + 0x4)
// USB3p0 CONTROLLER CONTROL REGISTER #0
#define USB3p0_CONTROLLER_CNTRL_REG0_ADDR	( USB3p0_PHY_CFG_BASE_ADDR + 0x10)
// USB3p0 CONTROLLER CONTROL REGISTER #1
#define USB3p0_CONTROLLER_CNTRL_REG1_ADDR	( USB3p0_PHY_CFG_BASE_ADDR + 0x14)
// USB3p0 CONTROLLER CONTROL REGISTER #2
#define USB3p0_CONTROLLER_CNTRL_REG2_ADDR	( USB3p0_PHY_CFG_BASE_ADDR + 0x18)
// USB3p0 CONTROLLER STATUS REGISTER
#define USB3p0_CONTROLLER_STS_REG_ADDR		( USB3p0_PHY_CFG_BASE_ADDR + 0x1C)
// USB3p0 PHY CONTROL REGISTER #0
#define USB3p0_PHY_CNTRL_REG0_ADDR	        ( USB3p0_PHY_CFG_BASE_ADDR + 0x20)
// USB3p0 PHY CONTROL REGISTER #1
#define USB3p0_PHY_CNTRL_REG1_ADDR	        ( USB3p0_PHY_CFG_BASE_ADDR + 0x24)
// USB3p0 PHY CONTROL REGISTER #2
#define USB3p0_PHY_CNTRL_REG2_ADDR	        ( USB3p0_PHY_CFG_BASE_ADDR + 0x28)
// USB3p0 PHY CONTROL REGISTER #3
#define USB3p0_PHY_CNTRL_REG3_ADDR	        ( USB3p0_PHY_CFG_BASE_ADDR + 0x2C)
// USB3p0 PHY STATUS REGISTER 
#define USB3p0_PHY_STS_REG_ADDR	  	        ( USB3p0_PHY_CFG_BASE_ADDR + 0x30)
//000000000000000000000000000000000000000000000000000000000000000000//

/////$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ END OF DEFINES $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$//

