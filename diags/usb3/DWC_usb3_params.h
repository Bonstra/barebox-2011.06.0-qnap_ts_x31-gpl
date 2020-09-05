//============================================================================================
//
//                  (C) COPYRIGHT 2008-2011 SYNOPSYS, INC.
//                            ALL RIGHTS RESERVED
//
//  This software and the associated documentation are confidential and
//  proprietary to Synopsys, Inc.  Your use or disclosure of this
//  software is subject to the terms and conditions of a written
//  license agreement between you, or your company, and Synopsys, Inc.
//
//  The entire notice above must be reproduced on all authorized copies.
//
//  Filename        : DWC_usb3_params.v
//  Author          : S.Arvind
//  Release version : 1.85a
//  Date            : $Date: 2014/03/12 12:17:40 $
//  File Version    : $Revision: 1.1.1.1 $
//  Description     : USB3 Core Parameters
//
// Modification History:
// Date           By               Version  Change Description
//=============================================================================
// 07/03/07       SA                1.0      Initial Release
//=============================================================================



/************* Basic Configuration Parameters  ************/


// Name:         DWC_USB3_MODE
// Default:      Device (NPI or Non-NPI)
// Values:       Device (NPI or Non-NPI) (0), Host (1), Device and Host (DRD) (2), 
//               Hub (3)
// 
// Selects controller mode for DWC_usb3. 
//  Choice decides the value of DWC_USB3_MODE in DWC_usb3_params.v file.
#define DWC_USB3_MODE 2


// Name:         DWC_USB3_EN_ISOC_SUPT
// Default:      Yes
// Values:       No (0), Yes (1)
// Enabled:      (DWC_USB3_MODE == 0 || DWC_USB3_MODE == 1 || DWC_USB3_MODE == 2 || 
//               DWC_USB3_MODE == 4)
// 
// Enables isochronous endpoint capability. 
//  Choice decides the value of DWC_USB3_EN_ISOC_SUPT in DWC_usb3_params.v file.
#define DWC_USB3_EN_ISOC_SUPT 1

// euse-pragma attr SymbolicNames {Non-OTG Device}

// Name:         DWC_USB3_EN_OTG
// Default:      No
// Values:       No (0), Yes (1)
// Enabled:      DWC_USB3_MODE !=3
// 
// Enables OTG capability in USB2.0 Mode. In Device only or Host only mode 
//  when OTG Capability is enabled, only SRP Capability is enabled. 
//  Choice decides the value of DWC_USB3_EN_OTG in DWC_usb3_params.v file.
#define DWC_USB3_EN_OTG 0


#define DWC_USB3_EN_OTG_SS 0


// Name:         DWC_USB3_MBUS_TYPE
// Default:      AHB
// Values:       AHB (0), AXI (1), Reserved2 (2), Native (3), Non Processor 
//               Interface (NPI) (4)
// Enabled:      DWC_USB3_MODE !=3
// 
// Selects the SoC Master Bus interface type. The Master bus is used for DMA. 
//  If your SoC is AHB-based, or if you have an AHB to your 
//  SoC bus bridge, select AHB. 
//  If your SoC is AXI-based, or if you have an AXI to your 
//  SoC bus bridge, select AXI. 
//  If your SoC is Non-Processor based, then select NPI. 
//  Otherwise, select the Native bus interface, and design a 
//  bridge from DWC_usb3's Native interface to your SoC bus. 
//  For more information, see the databook's "Hardware Integration" chapter. 
//  Choice decides the value of DWC_USB3_MBUS_TYPE in DWC_usb3_params.v file.
#define DWC_USB3_MBUS_TYPE 1


// Name:         DWC_USB3_SBUS_TYPE
// Default:      AHB
// Values:       AHB (0), AXI (1), Reserved2 (2), Native (3)
// Enabled:      DWC_USB3_MODE !=3 && DWC_USB3_MBUS_TYPE!=4
// 
// Selects the SoC Slave Bus interface type. The Slave bus is used for 
//     register programming.  
//  If your SoC is AHB-based, or if you have an AHB to your 
//  SoC bus bridge, select AHB. 
//  Otherwise, select the Native bus interface, and design a 
//  bridge from DWC_usb3's Native interface to your SoC bus. 
//  For more information, see the databook's "Hardware Integration" chapter. 
//  Choice decides the value of DWC_USB3_SBUS_TYPE in DWC_usb3_params.v file.
#define DWC_USB3_SBUS_TYPE 1


// Name:         DWC_USB3_MDWIDTH
// Default:      64 bits
// Values:       32 bits (32), 33 bits (33), 64 bits (64), 128 bits (128)
// Enabled:      DWC_USB3_MODE !=3 && DWC_USB3_MBUS_TYPE!=4
// 
// Selects data bus width of the Master Bus interface. 
//  For more information, see the databook's "Hardware Integration" 
//  chapter. 33-bit is used only for Hub configuration. 
//  Choice decides the value of DWC_USB3_MDWIDTH in DWC_usb3_params.v file.
#define DWC_USB3_MDWIDTH 64


#define DWC_USB3_MDWIDTH_BYTESHFT 3

#define DWC_USB3_MADDR_LO 3

#define DWC_USB3_MDWORDS 2

#define DWC_USB3_MBYTES 8


// Name:         DWC_USB3_SDWIDTH
// Default:      32 bits
// Values:       32 bits (32), 64 bits (64), 128 bits (128)
// Enabled:      DWC_USB3_MODE !=3 && DWC_USB3_MBUS_TYPE!=4
// 
// Selects the data bus width of the Slave Bus interface. 
//  Note: The slave data bus width must not be larger than the 
//  master data bus width. 
//  For more information, see the databook's "Hardware Integration" chapter. 
//  Choice decides the value of DWC_USB3_SDWIDTH in DWC_usb3_params.v file.
#define DWC_USB3_SDWIDTH 32


// Name:         DWC_USB3_AWIDTH
// Default:      32 bits
// Values:       32 bits (32), 64 bits (64)
// Enabled:      DWC_USB3_MODE !=3
// 
// Selects the address bus width of the master and slave interfaces. 
//  If your system address width is 32 bits, then select 32 bits; 
//  if your address bus width is 64 bits, then select 64 bits.  
//  The address width is common for both master and slave interface; 
//  and should be set to meet the DMA master interface requirement. 
//  The slave interface uses only address bits [19:0] others are don't care. 
//  For more information, see the databook's "Hardware Integration" chapter. 
//  Choice decides the value of DWC_USB3_AWIDTH in DWC_usb3_params.v file.
#define DWC_USB3_AWIDTH 32


// Name:         DWC_USB3_IDWIDTH
// Default:      4
// Values:       1 2 3 4 5 6 7 8
// Enabled:      (DWC_USB3_MBUS_TYPE!=0 && DWC_USB3_MODE !=3 && 
//               DWC_USB3_MBUS_TYPE!=4)
// 
// Selects the ID port width of the master bus interface. AXI gate count 
//  is exponentially proportional to the width of this ID. 
//  For more information, see the databook's "Hardware Integration" chapter. 
//  Choice decides the value of DWC_USB3_IDWIDTH in DWC_usb3_params.v file.
#define DWC_USB3_IDWIDTH 4


// Name:         DWC_USB3_SIDWIDTH
// Default:      4
// Values:       1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16
// Enabled:      (DWC_USB3_SBUS_TYPE!=0 && DWC_USB3_MODE !=3 && 
//               DWC_USB3_MBUS_TYPE!=4)
// 
// Selects the ID port width of the slave bus interface. 
//  For more information, see the databook's "Hardware Integration" chapter. 
//  Choice decides the value of DWC_USB3_SIDWIDTH in DWC_usb3_params.v file.
#define DWC_USB3_SIDWIDTH 14


// Name:         DWC_USB3_BURSTWIDTH
// Default:      8
// Values:       1 2 3 4 5 6 7 8
// Enabled:      ((DWC_USB3_MBUS_TYPE!=0 || DWC_USB3_SBUS_TYPE!=0) && DWC_USB3_MODE 
//               !=3 && DWC_USB3_MBUS_TYPE!=4)
// 
// Selects the Burst port width of the master and slave bus 
//  interfaces. 
//  For more information, see the databook's "Hardware Integration" chapter. 
//  Choice decides the value of DWC_USB3_BURSTWIDTH in DWC_usb3_params.v file.
#define DWC_USB3_BURSTWIDTH 8


// Name:         DWC_USB3_DATAINFOWIDTH
// Default:      4
// Values:       1 2 3 4 5 6
// Enabled:      ((DWC_USB3_MBUS_TYPE!=0 || DWC_USB3_SBUS_TYPE!=0) && DWC_USB3_MODE 
//               !=3 && DWC_USB3_MBUS_TYPE!=4)
// 
// Selects the Data Info port width of the master and slave bus 
//  interfaces. 
//  For more information, see the databook's "Hardware Integration" chapter. 
//  Choice decides the value of DWC_USB3_DATAINFOWIDTH in DWC_usb3_params.v file.
#define DWC_USB3_DATAINFOWIDTH 4


// Name:         DWC_USB3_REQINFOWIDTH
// Default:      4
// Values:       1 2 3 4 5 6
// Enabled:      ((DWC_USB3_MBUS_TYPE!=0 || DWC_USB3_SBUS_TYPE!=0) && DWC_USB3_MODE 
//               !=3 && DWC_USB3_MBUS_TYPE!=4)
// 
// Selects the Request/Response Info port width of the master 
//  and slave bus interfaces. 
//  For more information, see the databook's "Hardware Integration" chapter. 
//  Choice decides the value of DWC_USB3_REQINFOWIDTH in DWC_usb3_params.v file.
#define DWC_USB3_REQINFOWIDTH 4


// Name:         DWC_USB3_ASPACEWIDTH
// Default:      4
// Values:       1 2 3 4 5 6
// Enabled:      ((DWC_USB3_MBUS_TYPE!=0 || DWC_USB3_SBUS_TYPE!=0) && DWC_USB3_MODE 
//               !=3 && DWC_USB3_MBUS_TYPE!=4)
// 
// Selects the Address Space port width of the master and 
//  slave bus interfaces. 
//  For more information, see the databook's "Hardware Integration" chapter. 
//  Choice decides the value of DWC_USB3_ASPACEWIDTH in DWC_usb3_params.v file.
#define DWC_USB3_ASPACEWIDTH 4


#define DWC_USB3_ATOMICWIDTH 4

#define DWC_USB3_MISCWIDTH 4


// Name:         DWC_USB3_NUM_OUTSTANDING_TXDMA
// Default:      1
// Values:       1 2
// Enabled:      DWC_USB3_MODE !=3 && (DWC_USB3_MBUS_TYPE==1 || 
//               DWC_USB3_MBUS_TYPE==3)
// 
// Selects the maximum number of master DMA packet read requests (1KB) the core can issue 
//  on the native bus at a given time. In AHB it is fixed to 1 since AHB does not support multiple outstanding requests 
//  and the burst size are much smaller than 1KB.  
//  This provides performance when designing gaskets for higher latency and larger burst-size buses like PCIe. 
//  Choice decides the value of DWC_USB3_NUM_OUTSTANDING_TXDMA in DWC_usb3_params.v file.
#define DWC_USB3_NUM_OUTSTANDING_TXDMA 2


// Name:         DWC_USB3_NUM_RAMS
// Default:      3
// Values:       1 2 3
// Enabled:      DWC_USB3_MODE !=3  && DWC_USB3_MBUS_TYPE!=4
// 
// Selects the number of RAMs. 
//  Minimal Configuration Recommendations: 
//    SuperSpeed device and single port host require 2 or 3 RAMs (3 recommended): 
//    RAM0 for registers, descriptor cache, and Rx-buffering; and RAM1 for Tx prefetch. 
//  
//  SuperSpeed multi-port host requires 2 or 3 RAMs (3 recommended): 
//  RAM0 for registers, descriptor cache; RAM1 for Tx prefetch; and RAM2 for Rx-buffering. 
//  
//  High-Speed device and host requires 1 RAM (USB 2.0 needs less bandwidth): 
//  RAM0 for registers, descriptor cache, Rx buffering, and Tx prefetch. 
//  
//  Hub requires 2 dual-port RAMs. 
//  
//  For SuperSpeed device and host, three  2-Port rams provide the best compromise between 
//  performance, lower clock frequency, and area. 
//  For more information, see the databook's "Hardware Integration" chapter. 
//  Choice decides the value of DWC_USB3_NUM_RAMS in DWC_usb3_params.v file.
#define DWC_USB3_NUM_RAMS 2


// Name:         DWC_USB3_SPRAM_TYP
// Default:      2-Port RAM
// Values:       2-Port RAM (0), Single-Port RAM (SPRAM) (1)
// Enabled:      DWC_USB3_MODE !=3  && DWC_USB3_MBUS_TYPE!=4
// 
// Selects the FIFO Synchronous Static RAM type: 
//  2-Port (1 port read, 1 port write) or Single-Port (SPRAM). 
//  Select 2-port RAM for performance-sensitive applications 
//  or if your system bus frequency is low (e.g., below 66 MHz). 
//  Select single-port RAM for area-sensitive applications. 
//  Depending on your memory compiler and RAM size, the 
//  area of a true dual-port RAM is 1.2-1.8 times larger than 
//  a single-port RAM of the same RAM size. The 2-port RAMs are 
//  normally a little larger than SPRAM but much smaller than DPRAM. 
//  When SPRAM is selected, the same port is shared by both 
//  the MAC and AHB. The MAC has priority over the AHB, so 
//  busy cycles are inserted on the AHB Master interface 
//  during simultaneous MAC and AHB accesses to the RAM. 
//  For SuperSpeed device and host, three 2-Port RAMs provides the best compromise between 
//  performance, lower clock frequency, and area. 
//  For more information, see the databook's "Hardware Integration" chapter. 
//  Choice decides the value of DWC_USB3_SPRAM_TYP in DWC_usb3_params.v file.
#define DWC_USB3_SPRAM_TYP 0


// Name:         DWC_USB3_EN_PWROPT
// Default:      No Power Optimization
// Values:       No Power Optimization (0), Clock Gating Only (1), Reserved1 (2)
// Enabled:      DWC_USB3_MODE ==0 && DWC_USB3_MBUS_TYPE !=4
// 
// Specifies the power optimization mode. 
//  If Clock Gating Only is selected, RAM and PHY clocks are gated 
//  when the core is inactive during U1, U2, or U3 states. 
//  If Two Power Rails Support is selected, in addition to clock 
//  gating, most of the core can be be powered down to minimize 
//  power leakage during U2 or U3 states. Only a small portion of 
//  the core is active to check for the wakeup. Also, isolation 
//  power clamps are instantiated on the ports from the power-down 
//  module to always ON modules. During synthesis, you can map these 
//  modules to your library-specific cells. 
//  For more information, see the databook's "Hardware Integration" chapter. 
//  Choice decides the value of DWC_USB3_EN_PWROPT in DWC_usb3_params.v file.
#define DWC_USB3_EN_PWROPT 0


#define DWC_USB3_EN_PWROPT_N 1


// Name:         DWC_USB3_MAC_PHY_CLKS_SYNC
// Default:      No
// Values:       No (0), Yes (1)
// 
// Specifies whether the MAC clock and the PHY clock are 
//  synchronous to each other. If they are not synchronous, 
//  clock-domain-crossing synchronizers are instantiated 
//  between them and false paths are set between the two clocks 
//  are in synthesis. 
//  For more information, see the databook's "Hardware Integration" chapter. 
//  Choice decides the value of DWC_USB3_MAC_PHY_CLKS_SYNC in DWC_usb3_params.v file.
#define DWC_USB3_MAC_PHY_CLKS_SYNC 0


#define DWC_USB3_MAC3_BUS_CLKS_SYNC 0


// Name:         DWC_USB3_MAC_RAM_CLKS_SYNC
// Default:      No
// Values:       No (0), Yes (1)
// 
// Specifies whether the MAC clock and the RAM clock are 
//  synchronous to each other. If they are not synchronous, 
//  clock-domain-crossing synchronizers are instantiated 
//  between them and false paths are set between the two 
//  clocks in synthesis. Because the MAC clock is used for 
//  packet timing calculations, this clock must be fixed 
//  at a frequency equal to or greater than the PHY clock 
//  frequency, which is nominally 66 MHz. This solution 
//  is recommended when you do not want to connect the PHY clock 
//  to the MAC clock in order to avoid using a PLL to generate 
//  a 2x MAC clock. 
//  For more information, see the databook's "Hardware Integration" chapter. 
//  Choice decides the value of DWC_USB3_MAC_RAM_CLKS_SYNC in DWC_usb3_params.v file.
#define DWC_USB3_MAC_RAM_CLKS_SYNC 0


// Name:         DWC_USB3_RAM_BUS_CLKS_SYNC
// Default:      No
// Values:       No (0), Yes (1)
// 
// Specifies whether the RAM clock and the Bus clock are 
//  synchronous to each other. If they are not synchronous, 
//  clock-domain-crossing synchronizers are instantiated 
//  between them and false paths are set between the two 
//  clocks in synthesis. Because the MAC clock is used for 
//  packet timing calculations, this clock must be fixed at 
//  a frequency equal to or greater than the PHY clock frequency, 
//  which is nominally 66 MHz. This solution is recommended when 
//  when you do not want to connect the PHY clock to the MAC clock 
//  in order to avoid using a PLL to generate a 2x MAC clock. 
//  For more information, see the databook's "Hardware Integration" chapter. 
//  Choice decides the value of DWC_USB3_RAM_BUS_CLKS_SYNC in DWC_usb3_params.v file.
#define DWC_USB3_RAM_BUS_CLKS_SYNC 0


// Name:         DWC_USB3_SYNC_RST
// Default:      Asynchronous
// Values:       Asynchronous (0), Synchronous (1)
// 
// Specifies either asynchronous or synchronous reset coding 
//  style for all clocked always blocks in the RTL. 
//  For more information, see the databook's "Hardware Integration" chapter. 
//  Synchronous reset mode is not HW tested.  
//  Choice decides the value of DWC_USB3_SYNC_RST in DWC_usb3_params.v file.
#define DWC_USB3_SYNC_RST 0


// Name:         DWC_USB3_RM_OPT_FEATURES
// Default:      No
// Values:       No (0), Yes (1)
// 
// Specifies whether to remove optional features. When this 
//  parameter is enabled, the User ID register, General Purpose 
//  Input/Output ports, and SOF toggle and counter ports are 
//  removed to reduce area by 0.6K gates. The SOF toggle and 
//  counter ports are required if you plan to use the DWC_usb3 
//  in a Device Wire Adapter (DWA) design where the SOF count 
//  value is used to synchronize the USB 2.0 host controller 
//  SOF timer in the DWA. 
//  Choice decides the value of DWC_USB3_RM_OPT_FEATURES in DWC_usb3_params.v file.
#define DWC_USB3_RM_OPT_FEATURES 0


// Name:         DWC_USB3_USERID
// Default:      0x12345678
// Values:       0x0, ..., 0xffffffff
// Enabled:      DWC_USB3_MODE !=3 && DWC_USB3_MBUS_TYPE!=4
// 
// Specifies the Global User ID (GUID) register's power-on 
//  initialization value. After power-up, the software can change 
//  this value. This register can be used as either a scratch 
//  pad or identification register. 
//  For more information, see the databook's "Control and Status 
//  Registers" chapter. 
//  Choice decides the value of DWC_USB3_USERID in DWC_usb3_params.v file.
#define DWC_USB3_USERID 32'h12345678


// Name:         DWC_USB3_GUCTL
// Default:      0x8010
// Values:       0x0, ..., 0xffffffff
// Enabled:      DWC_USB3_MODE !=3 && DWC_USB3_MBUS_TYPE!=4
// 
// Specified the global User Specific Control Register's power-on 
//  initialization value. After power-up, the software can change 
//  this value.  
//  For more information, see the databook's "Control and Status 
//  Registers" chapter. 
//  Choice decides the value of DWC_USB3_USERID in DWC_usb3_params.v file.
#define DWC_USB3_GUCTL 32'h8010


// Name:         DWC_USB3_CSR_RAL_BASE_ADDR_VERIF_ONLY
// Default:      0x100000
// Values:       0x0, ..., 0xffffffff
// 
// change to allow CSR to be remapped
#define DWC_USB3_CSR_RAL_BASE_ADDR_VERIF_ONLY 64'h100000

/********** PHY Specific Parameters ***********/


// Name:         DWC_USB3_SSPHY_INTERFACE
// Default:      PIPE3
// Values:       None (0), PIPE3 (1)
// Enabled:      0
// 
// Specifies the SuperSpeed PHY interface. In USB 2.0 only 
//  mode, choose None. Otherwise, select the PIPE3 interface. 
//  Choice decides the value of DWC_USB3_SSPHY_INTERFACE in DWC_usb3_params.v file.
#define DWC_USB3_SSPHY_INTERFACE 1
// euse-pragma attr Label SuperSpeed PHY Interface?


// Name:         DWC_USB3_SSPHY_INTERFACE_NUM_PIPE
// Default:      No Additional Pipeline
// Values:       No Additional Pipeline (0), 1 stage of Additional Pipeline (1), 2 
//               stages of Additional Pipeline (2)
// Enabled:      DWC_USB3_SSPHY_INTERFACE ==1
// 
// Insert additional pipelines in the Rx and Tx of the PIPE interface. 
//  This may be required in the Off chip PHY interface to meet Timing on the pipe 
//  interface. In the src/pwrm/DWC_usb3_pwrm_u3piu module, the following additional pipeline register control the pipelining:  
//  phy_pipe3_rx_stage1, phy_pipe3_rx_stage2, phy_pipe3_tx_stage1, and phy_pipe3_tx_stage2. 
//  Choice decides the number of pipelines to be added.
#define DWC_USB3_SSPHY_INTERFACE_NUM_PIPE 0

// euse-pragma attr CheckExpr[@DWC_USB3_HSPHY_INTERFACE!=0] {[check_license -available DWC-USB30-SRC] || [check_license -available DWC-USB30-DEV-SRC] || [check_license -available DWC-USB30-HOST-SRC] || [check_license -available DWC-USB30] || [check_license -available DWC-USB30-DEV] || [check_license -available DWC-USB30-HOST] || [check_license -available DWC-USB30-SE-SRC] || [check_license -available DWC-USB30-SE]}
// euse-pragma attr CheckExprMessage[@DWC_USB3_MODE<=2] "Site does not have DWC-USB30-SRC or DWC-USB30-DEV-SRC or DWC-USB30-HOST-SRC or DWC-USB30 or DWC-USB30-DEV or DWC-USB30-HOST key"
//// reuse-pragma attr CheckExpr[@DWC_USB3_HSPHY_INTERFACE == 0 && (@DWC_USB3_MODE !=3 && @DWC_USB3_MODE !=4)] @DWC_USB3_SINGLE_POINT==1
//// euse-pragma attr CheckExprMessage[@DWC_USB3_HSPHY_INTERFACE == 0] "Splits cannot be enabled in FS Only Mode"

// Name:         DWC_USB3_HSPHY_INTERFACE
// Default:      UTMI+
// Values:       None (0), UTMI+ (1), ULPI (2), UTMI+ and ULPI (3)
// Enabled:      DWC_USB3_MODE !=3
// 
// Specifies the High-Speed PHY interface(s). Choose both 
//  UTMI+ and ULPI if you are not sure whether a UTMI+ or 
//  ULPI off-chip PHY will be used in the product, or if you 
//  have an on-chip UTMI+ PHY and want to bring out the ULPI 
//  interface as a backup in case the on-chip PHY fails. Because 
//  the MAC connects to the UTMI+ interface, choosing  an ULPI 
//  interface adds about 1.5K gates for the ULPI-to-UTMI+ 
//  conversion logic. If the UTMI+ and ULPI option is chosen, 
//  the software can select either interface. 
//  Choice decides the value of DWC_USB3_HSPHY_INTERFACE in DWC_usb3_params.v file.
#define DWC_USB3_HSPHY_INTERFACE 1


#define DWC_USB3_ULPI_DDR 0

// euse-pragma attr CheckExpr[@DWC_USB3_HSPHY_INTERFACE != 1 && @DWC_USB3_HSPHY_INTERFACE != 3] @DWC_USB3_FSPHY_INTERFACE!=2
// euse-pragma attr CheckExpr[@DWC_USB3_HSPHY_INTERFACE != 2 && @DWC_USB3_HSPHY_INTERFACE != 3] @DWC_USB3_FSPHY_INTERFACE != 3
// euse-pragma attr CheckExprMessage[@DWC_USB3_HSPHY_INTERFACE!=2 && @DWC_USB3_HSPHY_INTERFACE!=3] "Since there is no ULPI Interface FS Pin sharing with ULPI is not possible"
// euse-pragma attr Enabled {@DWC_USB3_MODE !=3}

// Name:         DWC_USB3_FSPHY_INTERFACE
// Default:      None
// Values:       None (0), Dedicated FS (1), FS pins shared with UTMI+ (2), FS pins 
//               shared with ULPI (3)
// Enabled:      0
// 
// Specifies the USB 1.1 Full-Speed Serial Transceiver 
//  interface. You can choose dedicated USB 1.1 FS pins, 
//  or the pins can be shared with a UTMI+ or ULPI interface. 
//  Implement both an FS transceiver interface and an HS PHY 
//  interface if you  want to use the same chip in FS and HS 
//  products. You can also have the FS interface as a backup in case 
//  the on-chip high-speed PHY fails. Selecting the Dedicated 
//  FS interface adds about 3.5K gates. 
//  Choice decides the value of DWC_USB3_FSPHY_INTERFACE in DWC_usb3_params.v file.
#define DWC_USB3_FSPHY_INTERFACE 0

// euse-pragma attr CheckExpr[@DWC_USB3_HSPHY_INTERFACE == 2 || @DWC_USB3_HSPHY_INTERFACE == 3] @DWC_USB3_HSPHY_DWIDTH!=1
// euse-pragma attr CheckExprMessage[@DWC_USB3_HSPHY_INTERFACE == 2 || @DWC_USB3_HSPHY_INTERFACE == 3] "When Using ULPI PHY, you must choose 8-bit or 8/16-bit data width in order to meet USB turnaround time"

// Name:         DWC_USB3_HSPHY_DWIDTH
// Default:      8/16 bits
// Values:       8 bits (0), 16 bits (1), 8/16 bits (2)
// Enabled:      DWC_USB3_MODE !=3
// 
// Specifies the data width of the UTMI+ PHY interface. 
//  Choose either 8 bits or 16 bits if your UTMI+ PHY only 
//  supports one of these. If you are not sure which PHY 
//  you will be using in your product, choose 8/16 bits 
//  (software selectable). Choosing 8 bits or 16 bits only 
//  reduces area by 0.5K gates. Running the MAC UTMI+ 
//  interface at 16 bits will consume less power because 
//  the MAC runs at 30 MHz instead of 60 MHz. When a ULPI PHY 
//  is used, an internal wrapper converts ULPI to UTMI+. Since 
//  the ULPI-to-UTMI+  wrapper adds additional delay when 
//  the ULPI PHY is selected, only 8-bit mode should be used. 
//  (To meet USB turnaround time, select either a hardware-fixed 
//  8-bit mode using this parameter, or 8-bit mode using software 
//  when 8/16 bit mode is selected for hardware.) 
//  Choice decides the value of DWC_USB3_HSPHY_DWIDTH in DWC_usb3_params.v file.
#define DWC_USB3_HSPHY_DWIDTH 2

// use-pragma attr CheckExpr[@DWC_USB3_ENABLE_LPM!=0] {[check_license -available DWC-HSOTG-LPM] }
// use-pragma attr CheckExprMessage[@DWC_USB3_ENABLE_LPM!=0] "Site does not have DWC-HSOTG-LPM key"

// Name:         DWC_USB3_ENABLE_LPM
// Default:      LPM capable Core
// Values:       Non-LPM Capable Core (0), LPM capable Core (1)
// Enabled:      DWC_USB3_MODE !=3
// 
// Selects the Link Power Management (LPM) mode of operation for DWC_otg. 
//  LPM Mode in device modes increases area by X-Y gates 
//  LPM Mode in host modes increases area by A-B gates 
//  Note that special license DWC-HSOTG-LPM is required to enable this feature. 
//  Choice decides the value of DWC_USB3_ENABLE_LPM in DWC_usb3_params.v file.
#define DWC_USB3_ENABLE_LPM 1


// Name:         DWC_USB3_ENABLE_HSIC
// Default:      Non-HSIC Capable Core
// Values:       Non-HSIC Capable Core (0), HSIC capable Core (1)
// Enabled:      DWC_USB3_HSPHY_INTERFACE == 1 || DWC_USB3_HSPHY_INTERFACE == 3
// 
// Selects the High Speed Interchip (HSIC) mode of operation for DWC_usb3. 
//  HSIC Mode in device modes increases area by P-Q gates 
//  HSIC Mode in host modes increases area by C-D gates 
//  Note that special license DWC-USB30-HSIC is required to enable this feature. 
//  Choice decides the value of DWC_USB3_ENABLE_HSIC in DWC_usb3_params.v file.
#define DWC_USB3_ENABLE_HSIC 0

// euse-pragma attr Enabled @DWC_USB3_FSPHY_INTERFACE!=0
// use-pragma attr CheckExpr[@DWC_USB3_ENABLE_IC_USB!=0] {[check_license -available DWC-HSOTG-FS-ICUSB] }
// use-pragma attr CheckExprMessage[@DWC_USB3_ENABLE_IC_USB!=0] "Site does not have DWC-HSOTG-FS-ICUSB key"
// euse-pragma attr Enabled {@DWC_USB3_MODE !=3}

// Name:         DWC_USB3_ENABLE_IC_USB
// Default:      None
// Values:       None (0), IC_USB interface (1)
// Enabled:      0
// 
// Specifies the IC_USB Full-Speed Serial Transceiver interface. You can 
//  choose dedicated USB 1.1 FS pins, or the IC_USB additional interface. 
//  Implement both an FS transceiver interface and an IC_USB transceiver 
//  interface if you  want to use the same chip in FS and IC_USB products. 
//  Selecting the IC_USB interface adds about 400 gates. 
//  Note that special license DWC-HSOTG-FS-ICUSB is required to enable this feature. 
//  Choice decides the value of DWC_USB3_ENABLE_IC_USB in DWC_usb3_params.v file.
#define DWC_USB3_ENABLE_IC_USB 0

// euse-pragma attr Enabled {@DWC_USB3_MODE !=3}

// Name:         DWC_USB3_SELECT_IC_USB
// Default:      FS_USB interface
// Values:       FS_USB interface (0), IC_USB interface (1)
// Enabled:      0
// 
// Specifies the initial mode of the controller when IC_USB is enabled. 
//  Select IC_USB interface if controller needs to come up with IC_USB mode as default 
//  Select FS_USB interface if controller needs to come up with FS_USB mode as default 
//  Choice decides the value of DWC_USB3_SELECT_IC_USB in DWC_usb3_params.v file.
#define DWC_USB3_SELECT_IC_USB 0


// Name:         DWC_USB3_I2C_INTERFACE
// Default:      No
// Values:       No (0), I2C Only (1), I2C and UTMI+ OTG (2)
// Enabled:      DWC_USB3_MODE !=3
// 
// Specifies whether the I2C interface is enabled. On the 
//  USB 1.1 Full-Speed Transceiver interface, I2C is required 
//  for OTG functionality and this parameter also controls 
//  which signals will be used for OTG. If enabled, I2C signals 
//  can be used for OTG control instead of, or in addition to, 
//  the UTMI+ OTG signals (software selectable). Using the I2C 
//  interface reduces the OTG pin count from 11 to 3. 
//  The I2C interface is also used for CarKit applications in OTG 
//  and non-OTG configurations. If CarKit mode is used with the FS 
//  Transceiver interface, the I2C interface must be enabled to 
//  access CarKit registers. 
//  This feature adds about 1.5K gates. 
//  Choice decides the value of DWC_USB3_I2C_INTERFACE in DWC_usb3_params.v file.
#define DWC_USB3_I2C_INTERFACE 0


// Name:         DWC_USB3_FAST_TAT_EN
// Default:      UTMI Fast_TAT disabled
// Values:       UTMI Fast_TAT disabled (0), UTMI Fast_TAT enabled (1)
// Enabled:      (DWC_USB3_HSPHY_INTERFACE == 1 || DWC_USB3_HSPHY_INTERFACE == 3)
// 
// Enable this parameter only if you require 16-bit UTMI support 
//  In multi-port mode, this parameter can be enabled only if all your USB2.0 port clocks are synchronous to each other. 
//  If your USB2.0 port clocks are asynchronous to each other, then enabling this would cause functional failures. 
//  
//  Selects the USB2.0 Device FAST Turnaround logic for UTMI 16-bit mode. 
//  The USB specification defines the High-Speed turn around delay to  
//  be 192 bit times, which translates into 12 PHY clocks at 30 MHz. 
//  Typically, the PHY will take 7 PHY clocks, leaving 5 PHY clocks 
//  to the controller. Without this FAST_TAT feature enabled,  
//  for IN transactions, when operating at a RAM clock of 60 MHz 
//  the USB3 device core can take up to 7 clocks for its response. 
//  In this case, the UTMI PHY must consume no more than 5 clocks for 
//  the Receive End Delay and Transmit Start Delay, combined. 
//  If the UTMI PHY cannot guarantee that the total time consumed for 
//  the Receive End Delay and Transmit Start Delay is 5 PHY clock 
//  cycles or less, then the turn-around delay may be higher than 192 
//  HS bit times. As a result, the USB host may timeout (if the 
//  device is connected to the 5th hub in a 5-tier hub topology) for 
//  a data packet returned by the USB3 device core for an IN token. 
//  Enabling this option optimizes/eliminates some registers in the 
//  device mac/ptl path and host mode clock-crossing FIFO, thereby achieving faster turn-around.  
//  
//  NOTE: If you plan to use ULPI mode only or UTMI 8-bit mode only, 
//  then do not enable this option.  
//  NOTE: Selecting this option and running the core at UTMI 8-bit mode 
//  (i.e., 60MHz) could pose synthesis/timing challenges. 
//  Depending on your UTMI PHY time consumption and your ram clock 
//  frequency select this option. 
//  Choice decides the value of DWC_USB3_FAST_TAT_EN in DWC_usb3_params.v file.
#define DWC_USB3_FAST_TAT_EN 0


// Name:         DWC_USB3_VENDOR_CTL_INTERFACE
// Default:      No
// Values:       No (0), Yes (1)
// Enabled:      (DWC_USB3_HSPHY_INTERFACE==1 || DWC_USB3_HSPHY_INTERFACE == 3) && 
//               DWC_USB3_MODE !=3
// 
// Specifies whether the UTMI+ PHY Vendor Control interface is 
//  enabled. For ULPI PHYs, since there are no additional ports 
//  Vendor Control is always enabled. 
//  Choice decides the value of DWC_USB3_VENDOR_CTL_INTERFACE in DWC_usb3_params.v file.
#define DWC_USB3_VENDOR_CTL_INTERFACE 0

// euse-pragma attr Enabled {@DWC_USB3_MODE !=3}

// Name:         DWC_USB3_ULPI_CARKIT
// Default:      No
// Values:       No (0), Yes (1)
// Enabled:      0
// 
// Specifies whether the CarKit is enabled for interface. 
//  If enabled, the software uses the ULPI interface for 
//  ULPI PHY internal register access. Software can access ULPI 
//  PHY internal registers using register read/writes to DWC_usb3. 
//  Choice decides the value of DWC_USB3_ULPI_CARKIT in DWC_usb3_params.v file.
#define DWC_USB3_ULPI_CARKIT 0


// Name:         DWC_USB3_GUSB2PHYCFG_INIT
// Default:      0x2400
// Values:       0x0, ..., 0xffffffff
// Enabled:      (DWC_USB3_HSPHY_INTERFACE!=0 || DWC_USB3_FSPHY_INTERFACE!=0) && 
//               DWC_USB3_MBUS_TYPE!=4
// 
// Specifies the Global USB2 PHY Configuration Register's (GUSB2PHYCFG) 
//  power-on initialization value. The GUSB2PHYCFG register defines USB2 PHY configuration. 
//  The bits other than defined here must be 0. The different fields are: 
//   bit[13:10] -  USB 2.0 Turnaround Time. 4'h5 - 16-bit UTMI+, 4'h9 - 8-bit UTMI+/ULPI. 
//   bit[9]   - Reserved 
//   bit[8]   - The application uses this bit to control utmi_sleep_n assertion to the PHY in the L1 state. 1'b0 - Disable, 1'b1 - Enable.  
//   bit[7]   - 1'b0 USB2.0 High Speed (UTMI/ULPI), 1'b1 - USB1.1 Full Speed  
//   bit[6]   - When set, USB2.0 PHY enters Suspend mode if Suspend conditions are valid. 1'b0 - Disable, 1'b1 - Enable. 
//   bit[5]   - 1'b0 - 6-Pin  Unidirectional FS, 1'b1 - 3-Pin Bidirectional FS 
//   bit[4]   - 1'b0 - 0 - UTMI, 1'b1 - ULPI 
//   bit[3]   - UTMI Mode: 1'b0 8-bit UTMI, 1'b1 - 16-bit UTMI; ULPI Mode - Should be 1'b0 
//   bit[2:0] - HS/FS Timeout Calibration 
//  If you use a standard driver, such as the xHCI driver from Microsoft, you must initialize 
//  this register to meet your system requirements before synthesizing the core. If you are 
//  developing your own xHCI host driver, then this register can be configured by your driver. 
//  For more information, see the databook's "Control and Status Registers" chapter. 
//  Choice decides the value of DWC_USB3_GUSB2PHYCFG_INIT in DWC_usb3_params.v file.
#define DWC_USB3_GUSB2PHYCFG_INIT 32'h2400


// Name:         DWC_USB3_GUSB3PIPECTL_INIT
// Default:      0x248002
// Values:       0x0, ..., 0xffffffff
// Enabled:      (DWC_USB3_SSPHY_INTERFACE!=0) && DWC_USB3_MBUS_TYPE!=4
// 
// Specifies the Global USB3 PIPE Control Register's (GUSB3PIPECTL) 
//  power-on initialization value. The GUSB3PIPECTL register defines USB3 PIPE Control. 
//  Refer to PIPE3 Specification for more details. The bits other than defined here must be 0. The different fields are: 
//   bit[24]    - When set, the core will always request PHY power change from P0 to P1/P2/P3 during U0 to U1/U2/U3 
//                transition. If this bit is 0 and immediate Ux exit (remote initiated or Locally initiated) happens,  
//                Core may not request P1/P2/P3 power state change. For Synopsys PHY this should be enabled. For non-Synopsys PHY, 
//                check with your PHY vendor.          
//   bit[23]    - If DWC_USB3_GUSB3PIPECTL_INIT[22] is set and link is in U3 or Rx.Detect state, core will start  
//                receiver detection on rising edge of this bit. For Downstream port only. This feature should not be used 
//                during normal operation. 
//   bit[22]    - When set, the core will not do receiver detection in U3/Rx.Detect. DWC_USB3_GUSB3PIPECTL_INIT[23]  
//                should be used to start receiver detection manually. For Downstream port only. This feature should not be 
//                enabled for normal operation. Contact Synopsys if have to use this feature.  
//   bit[21:19] - Delay P0 to P1/P2/P3 request when entering U1/U2/U3 until (DWC_USB3_GUSB3PIPECTL_INIT[21:19]*8) 8B10B  
//                error happens or Pipe3_RxValid drops. DWC_USB3_GUSB3PIPECTL_INIT[18] must be "1" to enable  
//                this functionality. This should be 3'h100 for Synopsys PHY.  
//   bit[18]    - Delay P1/P2/P3 transition when entering U1/U2/U3 until rxelecIdle is 1 and Rxvalid is 0. 
//                1 - Delay P1/P2/P3 transition. 
//                0 - Do not delay P1/P2/P3 based on rxelecIdle and Rxvalid. 
//                This should be '1' for Synopsys PHY. 
//   bit[17]    - Enable USB3.0 SS PHY Suspend.  
//   bit[16:15] - PIPE Data Width. The clock after reset, these bits will reflect the values of the "pipe3_DataBusWidth" port. 
//                2'b00 - 32-bit, 2'b01 - 16-bit, 2'b10 - 8-bit  
//   bit[14]    - In link state U2, abort receiver detection if remote partner starts U2 exit 
//   bit[13]    - No Rx Detect If RxElecIdle Low, when set the core will skip Rx Detection if pipe3_RxElecIdle is low.  
//                Skipping means waiting for the appropriate timeout and then repeating the operation. 
//   bit[12]    - LFPS P0 Align, when set the core will deassert LFPS transmission on the same clock edge that it requests  
//                Phy power state 0 when exiting U1, U2, or U3 low power states. Otherwise, LFPS transmission will be asserted  
//                one clock earlier. In addition, the core will request symbol transmission two pipe3_rx_pclks after 
//                the PHY asserts PhyStatus when the PHY has switched from the P1 or P2 to P0 state. 
//   bit[11]    - P3 P2 Transitions OK, when set the core will transition directly from Phy power states P2 to P3, and from  
//                P3 to P2. Otherwise, P0 will always be entered between P2 and P3 as defined in the PIPE3 specification. 
//                For Synopsys PHY this should be "0". 
//   bit[10]    - P3 Exit Signal in P2, when set the core will always change the Phy power state to P2, before attempting  
//                a U3 exit handshake. Otherwise, the Phy power state will not be changed during U3 exit signaling.  
//   bit[9]     - LFPS Filter Value, when set filter LFPS reception with pipe3_RxVaid in PHY power state P0, i.e ignore LFPS  
//                reception from the PHY unless both pipe3_Rxelecidle and pipe3_RxValid are deasserted.  
//   bit[6]     - Tx Swing, 1'b0 - Full Swing, 1'b1 - Low Swing 
//   bit[5:3]   - Tx Margin, 3'b000 - Normal, 3'b001 - 800-1200mV Full Swing/400-700mV Half Swing, 3'b010, 3'b011 - Vendor Defined, 
//                           3'b101 - 3'b111  - 200-400mV Full Swing/100-200mV Half Swing        
//   bit[2:1]   - Tx Deemphasis, 2'b00 - -6DB, 2'b01 - -3.5DB, 2'b10 - No Deemphesis, 2'b11 - Reserved 
//   bit[0]     - PHY Elasticity Mode, 1'b0 - Nominal Half Full Buffer, 1'b1 - Nominal Empty Buffer 
//  If you use a standard driver, such as the xHCI driver from Microsoft, you must initialize 
//  this register to meet your system requirements before synthesizing the core. If you are 
//  developing your own xHCI host driver, then this register can be configured by your driver. 
//  For more information, see the databook's "Control and Status Registers" chapter. 
//  Choice decides the value of DWC_USB3_GUSB3PIPECTL_INIT in DWC_usb3_params.v file.
#define DWC_USB3_GUSB3PIPECTL_INIT 32'h1240002


// Name:         DWC_USB3_PIPE_RXTERM_RESET_VAL
// Default:      0
// Values:       0 1
// Enabled:      DWC_USB3_SSPHY_INTERFACE!=0
// 
// Determines the value of the pipe3_RxTermination signal while 
// hardware reset is asserted.
#define DWC_USB3_PIPE_RXTERM_RESET_VAL 0


// Name:         DWC_USB3_SUSPEND_ON_DISCONNECT_EN
// Default:      0
// Values:       0 1
// Enabled:      (DWC_USB3_HSPHY_INTERFACE!=0 || DWC_USB3_FSPHY_INTERFACE!=0) && 
//               (DWC_USB3_MBUS_TYPE!=4)  && (DWC_USB3_MODE==1 || 
//               DWC_USB3_MODE==2)
// 
// For USB2.0 ports enable suspend during disconnect. This will save 
// power when no devices are connected to USB2.0 port.
#define DWC_USB3_SUSPEND_ON_DISCONNECT_EN 0


// Name:         DWC_USB3_RXDET_IN_P3_DS
// Default:      0
// Values:       0 1
// Enabled:      (DWC_USB3_SSPHY_INTERFACE!=0) && (DWC_USB3_MODE!=0)
// 
// For downstream port, Allow Receiver detection in PHY power state P3. 
//  Please check whether your PHY supports this feature. Even though this feature is supported in Synopsys PHY, some of the exiting Synopsys PHY test chips 
//  do not support this. Check with Synopsys whether a particular test chip supports this feature before enabling this feature for FPGA hardware validation.
#define DWC_USB3_RXDET_IN_P3_DS 0

/********** Device Specific Parameters ***********/


// Name:         DWC_USB3_DEVICE_NUM_INT
// Default:      1
// Values:       1 2 4 8 16 32
// Enabled:      DWC_USB3_MODE !=1 && DWC_USB3_MODE !=3 && DWC_USB3_MBUS_TYPE!=4
// 
// Selects the number of Event Buffers in device mode. Each Event Buffer will also have a separate interrupt. 
//  For more information, see the databook's "Hardware Integration" chapter. 
//  Choice decides the value of DWC_USB3_DEVICE_NUM_INT in DWC_usb3_params.v file.
#define DWC_USB3_DEVICE_NUM_INT 1


// Name:         DWC_USB3_NUM_EPS
// Default:      8
// Values:       4, ..., 32
// Enabled:      DWC_USB3_MODE !=1 && DWC_USB3_MODE !=3
// 
// Specifies the number of Device mode single directional endpoints, including 
//  OUT and IN control endpoint 0 which is always present. The DWC_usb3 core 
//  provides flexible endpoint configuration, where an endpoint can be mapped to  
//  any USB IN or OUT endpoint. For example if you need 3 bi-directional endpoints 
//  then you need to choose 6 single directional endpoint configuration. 
//  Choose the maximum possible number of endpoints that must be supported for all 
//  configurations and alternate settings. Specifying additional 
//  endpoints is not useful because the core will not use the 
//  excess registers, resulting in increased gate count but no 
//  gain in performance. If area is not a concern, and you want 
//  a flexible design that can be used in multiple applications, 
//  instantiate all 16 IN and 16 OUT endpoints. The default value of 8 endpoints reflects 
//  the UASP mass-storage class plus an ISOC application requirement: 
//  Control-OUT, Control-IN, Bulk-Data-OUT, Bulk-Data-IN, Bulk-Command-OUT, Bulk-Status-IN, 
//  ISOC-OUT, and ISOC-IN. The cost of an OUT/IN endpoint is 2.5/3.5Kgates plus transfer resource cache. 
//  Choice decides the value of DWC_USB3_NUM_EPS in DWC_usb3_params.v file.
#define DWC_USB3_NUM_EPS 8


// Name:         DWC_USB3_NUM_IN_EPS
// Default:      4
// Values:       2, ..., 16
// Enabled:      DWC_USB3_MODE != 1 && DWC_USB3_MODE !=3
// 
// Specifies the maximum number of Device mode IN endpoints 
//  active at any time, including control endpoint 0, which is always 
//  present. This parameter determines the number of Device mode 
//  TxFIFOs to be instantiated and Tx RAM allocated. The default value of 4 IN 
//  endpoints reflects the UASP mass-storage class plus an ISOC application 
//  requirement: 
//  Control-IN, Bulk-Data-IN, Bulk-Status-IN, and ISOC-IN. 
//  Choice decides the value of DWC_USB3_NUM_IN_EPS in DWC_usb3_params.v file.
#define DWC_USB3_NUM_IN_EPS 4


#define DWC_USB3_NUM_OUT_EPS 4


// Name:         DWC_USB3_CACHE_TOTAL_XFER_RESOURCES
// Default:      8
// Values:       4, ..., 127
// Enabled:      DWC_USB3_MODE != 1 && DWC_USB3_MODE !=3 && DWC_USB3_MBUS_TYPE!=4
// 
// Selects the maximum number of transfer resources in the core (must be at 
//  least equal to the number of unidirectional endpoints configured). 
//  It is recommended that 10 resources be allocated for each Stream endpoint 
//  and one for non Stream endpoints. 
//  For more information, see the databook's "Hardware Integration" 
//  chapter. The default value of 8 endpoints reflects 
//  the UASP mass-storage class plus an ISOC application requirement: 
//  1 transfer resource for each of the Control-OUT, Control-IN, Bulk-Command-OUT, ISOC-OUT, ISOC-IN endpoints and 
//  10 transfer resource for each of the Bulk-Data-OUT, Bulk-Data-IN, Bulk-Status-IN endpoints. 
//  The cost of a transfer resource is (Number of cached TRBs * 16 + 32) bytes of cache memory. 
//  Choice decides the value of DWC_USB3_CACHE_TOTAL_XFER_RESOURCES in DWC_usb3_params.v file.
#define DWC_USB3_CACHE_TOTAL_XFER_RESOURCES 8


// Name:         DWC_USB3_CACHE_TRBS_PER_TRANSFER
// Default:      4
// Values:       2, ..., 32
// Enabled:      DWC_USB3_MODE != 1 && DWC_USB3_MODE !=3 && DWC_USB3_MBUS_TYPE!=4
// 
// Selects the number of TRBs per transfer that can be cached within the 
//  core. It is recommended that the endpoint cache be able to hold one 
//  burst amount of TRBs. If your application buffer size is less than the 
//  packet size, then the TRB cache must also have at least one packet 
//  amount of TRBs for correct operation. 
//  The cost of TRB cache, 16 * Number of cached TRBs bytes. 
//  For more information, see the databook's "Hardware Integration" chapter. 
//  Choice decides the value of DWC_USB3_CACHE_TRBS_PER_TRANSFER in DWC_usb3_params.v file.
#define DWC_USB3_CACHE_TRBS_PER_TRANSFER 8


// Name:         DWC_USB3_PERIODIC_PARAMS
// Default:      0x0
// Values:       0x0, ..., 0x3ff
// Enabled:      DWC_USB3_MODE == 0 || DWC_USB3_MODE == 2
// 
// Specifies the Periodic Parameter register's power-on initialization 
//  value, which contains the System Exit Latency (SEL). After 
//  power-up, the software can change this value via the "Set Periodic 
//  Parameters" device generic command. 
//  For more information, see the databook's "Control and Status 
//  Registers" chapter. 
//  Choice decides the value of DWC_USB3_PERIODIC_PARAMS in DWC_usb3_params.v file.
#define DWC_USB3_PERIODIC_PARAMS 32'h0


// Name:         DWC_USB3_EN_COMPANION_USB2_CTRL
// Default:      No
// Values:       No (0), Yes (1)
// Enabled:      (DWC_USB3_MODE == 0 && DWC_USB3_MBUS_TYPE!=4)
// 
// Enables companion USB2.0 controller support. This feature is supported only in Non-NPI Device mode. 
//  For USB2.0 functions, an external USB2.0 controller can provide the support instead the DWC_usb3 controller. 
//  When this mode is selected, you are responsible to validate the DWC_usb3 core with your USB2.0 controller and  
//  check all the SS, HS, FS fallback modes are working. 
//  For more information, see the databook's "Hardware Integration" chapter. 
//  Choice decides the value of DWC_USB3_EN_ISOC_SUPT in DWC_usb3_params.v file.
#define DWC_USB3_EN_COMPANION_USB2_CTRL 0


// Name:         DWC_USB3_EXT_BUFF_CONTROL
// Default:      No
// Values:       No (0), Yes (1)
// Enabled:      ((DWC_USB3_MODE == 0) || (DWC_USB3_MODE == 2))
// 
// Enables Device External Buffer Control Sideband Controls
#define DWC_USB3_EXT_BUFF_CONTROL 0

/********** Device NPI Specific Parameters ***********/


// Name:         DWC_USB3_NPI_RX_STATUS_DEPTH
// Default:      16
// Values:       8 16 32
// Enabled:      DWC_USB3_MODE ==0 && DWC_USB3_MBUS_TYPE ==4
// 
// Selects OUT Endpoints Common Rx Status FIFO Depth
#define DWC_USB3_NPI_RX_STATUS_DEPTH 16


// Name:         DWC_USB3_NPI_TX_STATUS_DEPTH
// Default:      6
// Values:       4 5 6 7 8 16
// Enabled:      DWC_USB3_MODE ==0 && DWC_USB3_MBUS_TYPE ==4
// 
// Selects IN Endpoints Tx Status FIFO Depth (one per IN EP)
#define DWC_USB3_NPI_TX_STATUS_DEPTH 6


// Name:         DWC_USB3_ULPI_VENDOR_CTL_INTERFACE
// Default:      No
// Values:       No (0), Yes (1)
// Enabled:      (DWC_USB3_HSPHY_INTERFACE==2 || DWC_USB3_HSPHY_INTERFACE == 3) && 
//               DWC_USB3_MBUS_TYPE==4
// 
// Specifies whether the NPI Mode ULPI PHY Vendor Control interface is 
//  enabled. 
//  Choice decides the value of DWC_USB3_ULPI_VENDOR_CTL_INTERFACE in DWC_usb3_params.v file.
#define DWC_USB3_ULPI_VENDOR_CTL_INTERFACE 0


// Name:         DWC_USB3_NPI_USE_BMU_SIM
// Default:      Yes
// Values:       No (0), Yes (1)
// Enabled:      DWC_USB3_MODE ==0 && DWC_USB3_MBUS_TYPE==4
// 
// Specifies whether to use the NPI Buffer Management (DWC_usb3_npi_bufm.v) module in the simulation. When enabled, 
//  the BMU module will also be used in the simulation and the NPI BFMs will drive the BMU interface. When not enabled, 
//  the NPI interface will be exercised. 
//  directly. This affects only the simulation.
#define DWC_USB3_NPI_USE_BMU_SIM 1


// Name:         DWC_USB3_NPI_LARGEST_IN_EP_FIFO_DEPTH
// Default:      1024
// Values:       0, ..., 8192
// Enabled:      DWC_USB3_MODE ==0 && DWC_USB3_MBUS_TYPE ==4 && 
//               DWC_USB3_NPI_USE_BMU_SIM
// 
// The largest IN Endpoint FIFO Depth
#define DWC_USB3_NPI_LARGEST_IN_EP_FIFO_DEPTH 1024


// Name:         DWC_USB3_NPI_DYNAMIC_FIFO_DEPTH_EN
// Default:      No
// Values:       No (0), Yes (1)
// Enabled:      DWC_USB3_MODE ==0 && DWC_USB3_MBUS_TYPE ==4 && 
//               DWC_USB3_NPI_USE_BMU_SIM
// 
// Enables Dynamic FIFO depth. The BMU FIFO depth can be static or dynamic. 
//  If Dynamic FIFO depth is chosen, then the npi_tx_fifo_depth and npi_tx_fifo_depth_reconfig signals 
//  configure the depth. The clock after reset the npi_tx_fifo_depth will be used to configure the depth of each FIFO. 
//  After reset, the application can use npi_tx_fifo_depth_reconfig (one clock pulse) to reconfigure the FIFO.  
//  Tx FIFO-0 depth should not be changed. When npi_tx_fifo_depth_reconfig is asserted for one clock, it will take  
//  "DWC_USB3_NUM_IN_EPS+1" clocks to configure the FIFOs.
#define DWC_USB3_NPI_DYNAMIC_FIFO_DEPTH_EN 0


// Name:         DWC_USB3_NPI_IN_EP0_FIFO_DEPTH
// Default:      128
// Values:       32, ..., 8192
// Enabled:      DWC_USB3_NPI_DYNAMIC_FIFO_DEPTH_EN ==0 && DWC_USB3_MODE ==0 && 
//               DWC_USB3_MBUS_TYPE ==4 && DWC_USB3_NUM_IN_EPS > 0 && 
//               DWC_USB3_NPI_USE_BMU_SIM
// 
// Selects IN Endpoint 0 FIFO Depth
#define DWC_USB3_NPI_IN_EP0_FIFO_DEPTH 128


// Name:         DWC_USB3_NPI_IN_EP1_FIFO_DEPTH
// Default:      1024
// Values:       32, ..., 8192
// Enabled:      DWC_USB3_NPI_DYNAMIC_FIFO_DEPTH_EN ==0 && DWC_USB3_MODE ==0 && 
//               DWC_USB3_MBUS_TYPE ==4 && DWC_USB3_NUM_IN_EPS > 1 && 
//               DWC_USB3_NPI_USE_BMU_SIM
// 
// Selects IN Endpoint 1 FIFO Depth 
//  For UASP Data Endpoint which supports burst, 4 packets are recommended.
#define DWC_USB3_NPI_IN_EP1_FIFO_DEPTH 1024


// Name:         DWC_USB3_NPI_IN_EP2_FIFO_DEPTH
// Default:      768
// Values:       0, ..., 8192
// Enabled:      DWC_USB3_NPI_DYNAMIC_FIFO_DEPTH_EN ==0 && DWC_USB3_MODE ==0 && 
//               DWC_USB3_MBUS_TYPE ==4 && DWC_USB3_NUM_IN_EPS > 2 && 
//               DWC_USB3_NPI_USE_BMU_SIM
// 
// Selects IN Endpoint 2 FIFO Depth 
//  For UASP status Endpoint 256 byte is sufficient.
#define DWC_USB3_NPI_IN_EP2_FIFO_DEPTH 768


// Name:         DWC_USB3_NPI_IN_EP3_FIFO_DEPTH
// Default:      256
// Values:       0, ..., 8192
// Enabled:      DWC_USB3_NPI_DYNAMIC_FIFO_DEPTH_EN ==0 && DWC_USB3_MODE ==0 && 
//               DWC_USB3_MBUS_TYPE ==4 && DWC_USB3_NUM_IN_EPS > 3 && 
//               DWC_USB3_NPI_USE_BMU_SIM
// 
// Selects IN Endpoint 3 FIFO Depth
#define DWC_USB3_NPI_IN_EP3_FIFO_DEPTH 256


// Name:         DWC_USB3_NPI_IN_EP4_FIFO_DEPTH
// Default:      0
// Values:       0, ..., 8192
// Enabled:      DWC_USB3_NPI_DYNAMIC_FIFO_DEPTH_EN ==0 && DWC_USB3_MODE ==0 && 
//               DWC_USB3_MBUS_TYPE ==4 && DWC_USB3_NUM_IN_EPS > 4 && 
//               DWC_USB3_NPI_USE_BMU_SIM
// 
// Selects IN Endpoint 4 FIFO Depth
#define DWC_USB3_NPI_IN_EP4_FIFO_DEPTH 0


// Name:         DWC_USB3_NPI_IN_EP5_FIFO_DEPTH
// Default:      0
// Values:       0, ..., 8192
// Enabled:      DWC_USB3_NPI_DYNAMIC_FIFO_DEPTH_EN ==0 && DWC_USB3_MODE ==0 && 
//               DWC_USB3_MBUS_TYPE ==4 && DWC_USB3_NUM_IN_EPS > 5 && 
//               DWC_USB3_NPI_USE_BMU_SIM
// 
// Selects IN Endpoint 5 FIFO Depth
#define DWC_USB3_NPI_IN_EP5_FIFO_DEPTH 0


// Name:         DWC_USB3_NPI_IN_EP6_FIFO_DEPTH
// Default:      0
// Values:       0, ..., 8192
// Enabled:      DWC_USB3_NPI_DYNAMIC_FIFO_DEPTH_EN ==0 && DWC_USB3_MODE ==0 && 
//               DWC_USB3_MBUS_TYPE ==4 && DWC_USB3_NUM_IN_EPS > 6 && 
//               DWC_USB3_NPI_USE_BMU_SIM
// 
// Selects IN Endpoint 6 FIFO Depth
#define DWC_USB3_NPI_IN_EP6_FIFO_DEPTH 0


// Name:         DWC_USB3_NPI_IN_EP7_FIFO_DEPTH
// Default:      0
// Values:       0, ..., 8192
// Enabled:      DWC_USB3_NPI_DYNAMIC_FIFO_DEPTH_EN ==0 && DWC_USB3_MODE ==0 && 
//               DWC_USB3_MBUS_TYPE ==4 && DWC_USB3_NUM_IN_EPS > 7 && 
//               DWC_USB3_NPI_USE_BMU_SIM
// 
// Selects IN Endpoint 7 FIFO Depth
#define DWC_USB3_NPI_IN_EP7_FIFO_DEPTH 0
 

// Name:         DWC_USB3_NPI_IN_EP8_FIFO_DEPTH
// Default:      0
// Values:       0, ..., 8192
// Enabled:      DWC_USB3_NPI_DYNAMIC_FIFO_DEPTH_EN ==0 && DWC_USB3_MODE ==0 && 
//               DWC_USB3_MBUS_TYPE ==4 && DWC_USB3_NUM_IN_EPS > 8 && 
//               DWC_USB3_NPI_USE_BMU_SIM
// 
// Selects IN Endpoint 8 FIFO Depth
#define DWC_USB3_NPI_IN_EP8_FIFO_DEPTH 0


// Name:         DWC_USB3_NPI_IN_EP9_FIFO_DEPTH
// Default:      0
// Values:       0, ..., 8192
// Enabled:      DWC_USB3_NPI_DYNAMIC_FIFO_DEPTH_EN ==0 && DWC_USB3_MODE ==0 && 
//               DWC_USB3_MBUS_TYPE ==4 && DWC_USB3_NUM_IN_EPS > 9 && 
//               DWC_USB3_NPI_USE_BMU_SIM
// 
// Selects IN Endpoint 9 FIFO Depth
#define DWC_USB3_NPI_IN_EP9_FIFO_DEPTH 0


// Name:         DWC_USB3_NPI_IN_EP10_FIFO_DEPTH
// Default:      0
// Values:       0, ..., 8192
// Enabled:      DWC_USB3_NPI_DYNAMIC_FIFO_DEPTH_EN ==0 && DWC_USB3_MODE ==0 && 
//               DWC_USB3_MBUS_TYPE ==4 && DWC_USB3_NUM_IN_EPS > 10 && 
//               DWC_USB3_NPI_USE_BMU_SIM
// 
// Selects IN Endpoint 10 FIFO Depth
#define DWC_USB3_NPI_IN_EP10_FIFO_DEPTH 0


// Name:         DWC_USB3_NPI_IN_EP11_FIFO_DEPTH
// Default:      0
// Values:       0, ..., 8192
// Enabled:      DWC_USB3_NPI_DYNAMIC_FIFO_DEPTH_EN ==0 && DWC_USB3_MODE ==0 && 
//               DWC_USB3_MBUS_TYPE ==4 && DWC_USB3_NUM_IN_EPS > 11 && 
//               DWC_USB3_NPI_USE_BMU_SIM
// 
// Selects IN Endpoint 11 FIFO Depth
#define DWC_USB3_NPI_IN_EP11_FIFO_DEPTH 0


// Name:         DWC_USB3_NPI_IN_EP12_FIFO_DEPTH
// Default:      0
// Values:       0, ..., 8192
// Enabled:      DWC_USB3_NPI_DYNAMIC_FIFO_DEPTH_EN ==0 && DWC_USB3_MODE ==0 && 
//               DWC_USB3_MBUS_TYPE ==4 && DWC_USB3_NUM_IN_EPS > 12 && 
//               DWC_USB3_NPI_USE_BMU_SIM
// 
// Selects IN Endpoint 12 FIFO Depth
#define DWC_USB3_NPI_IN_EP12_FIFO_DEPTH 0


// Name:         DWC_USB3_NPI_IN_EP13_FIFO_DEPTH
// Default:      0
// Values:       0, ..., 8192
// Enabled:      DWC_USB3_NPI_DYNAMIC_FIFO_DEPTH_EN ==0 && DWC_USB3_MODE ==0 && 
//               DWC_USB3_MBUS_TYPE ==4 && DWC_USB3_NUM_IN_EPS > 13 && 
//               DWC_USB3_NPI_USE_BMU_SIM
// 
// Selects IN Endpoint 13 FIFO Depth
#define DWC_USB3_NPI_IN_EP13_FIFO_DEPTH 0


// Name:         DWC_USB3_NPI_IN_EP14_FIFO_DEPTH
// Default:      0
// Values:       0, ..., 8192
// Enabled:      DWC_USB3_NPI_DYNAMIC_FIFO_DEPTH_EN ==0 && DWC_USB3_MODE ==0 && 
//               DWC_USB3_MBUS_TYPE ==4 && DWC_USB3_NUM_IN_EPS > 14 && 
//               DWC_USB3_NPI_USE_BMU_SIM
// 
// Selects IN Endpoint 14 FIFO Depth
#define DWC_USB3_NPI_IN_EP14_FIFO_DEPTH 0


// Name:         DWC_USB3_NPI_IN_EP15_FIFO_DEPTH
// Default:      0
// Values:       0, ..., 8192
// Enabled:      DWC_USB3_NPI_DYNAMIC_FIFO_DEPTH_EN ==0 && DWC_USB3_MODE ==0 && 
//               DWC_USB3_MBUS_TYPE ==4 && DWC_USB3_NUM_IN_EPS > 15 && 
//               DWC_USB3_NPI_USE_BMU_SIM
// 
// Selects IN Endpoint 15 FIFO Depth
#define DWC_USB3_NPI_IN_EP15_FIFO_DEPTH 0



/********** Host Specific Parameters ***********/


// Name:         DWC_USB3_NUM_DEVICE_SUPT
// Default:      64
// Values:       1, ..., 127
// Enabled:      (DWC_USB3_MODE == 1 || DWC_USB3_MODE == 2)
// 
// Specifies the number of devices supported. The value you 
//  choose does not increase the DWC_usb3's gate count, but the 
//  memory requirements increase to store context information. 
//  32 bytes of internal RAM is required for each device supported. 
//  For more information, see the databook's "Area, Speed, and 
//  Power for DWC_usb3 Configurations" appendix. 
//  Choice decides the value of DWC_USB3_NUM_DEVICE_SUPT in DWC_usb3_params.v file.
#define DWC_USB3_NUM_DEVICE_SUPT 16


// Name:         DWC_USB3_HOST_NUM_INTERRUPTER_SUPT
// Default:      1
// Values:       1, ..., 1024
// Enabled:      (DWC_USB3_MODE == 1 || DWC_USB3_MODE == 2)
// 
// Specifies the number of interrupters supported. The value you 
//  choose affects the gate count and memory requirements of DWC_usb3. 
//  100 bytes of internal RAM is required for each interrupter.  
//  For more information, see the "Area, Speed, and 
//  Power for DWC_usb3 Configurations" appendix in the Databook. 
//  Choice decides the value of DWC_USB3_HOST_NUM_INTERRUPTER_SUPT in DWC_usb3_params.v file.
#define DWC_USB3_HOST_NUM_INTERRUPTER_SUPT 1


// Name:         DWC_USB3_HOST_NUM_U2_ROOT_PORTS
// Default:      1
// Values:       1 2 3 4 5 6 7 8 9 10 11 12 13 14 15
// Enabled:      (DWC_USB3_MODE == 1 || DWC_USB3_MODE == 2)
// 
// Specifies the number of USB2 Root Hub ports.  
//  The area of a USB 2.0 port is about 9K gates. 
//  Choice decides the value of DWC_USB3_HOST_NUM_U2_ROOT_PORTS in DWC_usb3_params.v file.
#define DWC_USB3_HOST_NUM_U2_ROOT_PORTS 1


// Name:         DWC_USB3_HOST_NUM_U3_ROOT_PORTS
// Default:      1
// Values:       1 2 3 4 5 6 7 8 9 10 11 12 13 14 15
// Enabled:      (DWC_USB3_MODE == 1 || DWC_USB3_MODE == 2)
// 
// Specifies the number of USB 3.0 Root Hub ports. 
//  The area of a USB3 port is about 52K gates. 
//  Choice decides the value of DWC_USB3_HOST_NUM_U3_ROOT_PORTS in DWC_usb3_params.v file.
#define DWC_USB3_HOST_NUM_U3_ROOT_PORTS 1

//  euse-pragma attr DefaultValue = (((@DWC_USB3_MODE==1) | (@DWC_USB3_MODE==2)) && (@DWC_USB3_HOST_NUM_U3_ROOT_PORTS != 1))? 1 : 0

// Name:         DWC_USB3_COMMON_USB3_PCLK_EXIST
// Default:      No
// Values:       No (0), Yes (1)
// Enabled:      (DWC_USB3_MODE == 1 || DWC_USB3_MODE == 2) && 
//               DWC_USB3_HOST_NUM_U3_ROOT_PORTS != 1
// 
// Specifies whether your USB 3.0 Multi-Port PHY provides a common PHY clock for multiport support 
//  which is active when any port is not in low-power state. If your USB 3.0 Multi-Port PHY provides 
//  a common pipe clock, it should be connected to "pipe3_common_rx_pclk" and "pipe3_common_tx_pclk" ports.  
//  If your USB 3.0 Multi-Port PHY does not provide a common PHY clock, then Port-0 clock will be used by the core 
//  for generating internal mac clock. 
//  Choice decides the value of DWC_USB3_COMMON_USB3_PCLK_EXIST in DWC_usb3_params.v file.
#define DWC_USB3_COMMON_USB3_PCLK_EXIST 0

//  euse-pragma attr DefaultValue = (((@DWC_USB3_MODE==1) | (@DWC_USB3_MODE==2)) && (@DWC_USB3_HOST_NUM_U2_ROOT_PORTS != 1))? 1 : 0

// Name:         DWC_USB3_COMMON_USB2_PCLK_EXIST
// Default:      No
// Values:       No (0), Yes (1)
// Enabled:      (DWC_USB3_MODE == 1 || DWC_USB3_MODE == 2) && 
//               DWC_USB3_HOST_NUM_U2_ROOT_PORTS != 1
// 
// Specifies whether your Multi-Port USB 2.0 PHY provides a common PHY clock for multiport support 
//  which is active when any port is not in low-power state. If your USB 2.0 Multi-Port PHY provides 
//  a common PHY clock, it should be connected to "usb2_phy_clk". 
//  If your USB 2.0 Multi-Port PHY does not provide a common PHY clock, then Port-0 clock will be used by the core 
//  for generating internal mac2 clock. 
//  Choice decides the value of DWC_USB3_COMMON_USB2_PCLK_EXIST in DWC_usb3_params.v file.
#define DWC_USB3_COMMON_USB2_PCLK_EXIST 0

//  euse-pragma attr DefaultValue = 1

// Name:         DWC_USB3_NUM_SS_USB_INSTANCES
// Default:      1
// Values:       1 2 3 4 5 6 7 8 9 10 11 12 13 14 15
// Enabled:      (DWC_USB3_MODE == 1 || DWC_USB3_MODE == 2) && 
//               DWC_USB3_HOST_NUM_U3_ROOT_PORTS != 1
// 
// Specifies the number of SuperSpeed USB Bus-Instances. When the number of 
//  SuperSpeed USB Bus-Instances is equal to the number of USB 3.0 ports, you 
//  can do concurrent USB 3.0 transfers on each port. For example, if you 
//  have 4 ports and 4 SuperSpeed USB Bus-Instances, then in concurrent mode your 
//  effective data rate is 32 Gbps (8 Gbps * 4). The area overhead of a 
//  SuperSpeed USB Bus-Instance is about 80K gates. When compared to the area of 
//  the PHY, which is about 400K gates per port, this is negligible, but the 
//  performance increase is multifold. For example, if you have 4 ports, 1 port 
//  can be connected to a USB 3.0 drive, another port can be connected to a 
//  display, and 2 more more ports can be used as general purpose I/O interface. 
//  Here, each port has a guaranteed throughput of 8 Gbps (or 4 Gbps simplex). 
//  Choice decides the value of DWC_USB3_NUM_SS_USB_INSTANCES in DWC_usb3_params.v file.
#define DWC_USB3_NUM_SS_USB_INSTANCES 1

//  euse-pragma attr DefaultValue = 1

// Name:         DWC_USB3_NUM_HS_USB_INSTANCES
// Default:      1
// Values:       1 2 3 4 5 6 7 8 9 10 11 12 13 14 15
// Enabled:      (DWC_USB3_MODE == 1 || DWC_USB3_MODE == 2) && 
//               DWC_USB3_HOST_NUM_U2_ROOT_PORTS != 1
// 
// Specifies the number of High-Speed USB Bus-Instances. When the number of 
//  High-Speed USB Bus-Instances is equal to the number of root ports, you can do 
//  concurrent High-Speed transfers on each port. For example, if you have 4 
//  ports and 4 High-Speed USB Bus-Instances, then in concurrent mode your 
//  effective data  rate is 1.92 Gbps (480 Mbps * 4). The area overhead of a 
//  HighSpeed USB Bus-Instance is about 60K gates. 
//  Choice decides the value of DWC_USB3_NUM_HS_USB_INSTANCES in DWC_usb3_params.v file.
#define DWC_USB3_NUM_HS_USB_INSTANCES 1

//  euse-pragma attr DefaultValue = 1

// Name:         DWC_USB3_NUM_FSLS_USB_INSTANCES
// Default:      1
// Values:       1 2 3 4 5 6 7 8 9 10 11 12 13 14 15
// Enabled:      0
// 
// Specifies the number of Full/Low-Speed USB Bus-Instances. When the number of 
//  Full/Low-Speed USB Bus-Instances is equal to the number of root ports, you can do  
//  concurrent FS transfers on each port. For example, if you have 4 ports and 
//  4 Full/Low-Speed USB Bus-Instances, then in concurrent mode your effective 
//  data rate is 48 Mbps for Full-Speed or 6 Mbps for Low-Speed. The area overhead of a 
//  Full/Low Speed USB Bus-Instance is about 50K gates. 
//  Choice decides the value of DWC_USB3_NUM_FSLS_USB_INSTANCES in DWC_usb3_params.v file.
#define DWC_USB3_NUM_FSLS_USB_INSTANCES 1
// euse-pragma attr Enabled {(@DWC_USB3_MODE == 1 || @DWC_USB3_MODE == 2) && @DWC_USB3_HOST_NUM_U2_ROOT_PORTS != 1}


// Name:         DWC_USB3_HOST_NUM_CACHE_EP_PER_SS_INSTANCE
// Default:      8
// Values:       1, ..., 32
// Enabled:      (DWC_USB3_MODE == 1 || DWC_USB3_MODE == 2)
// 
// Specifies the number of SuperSpeed endpoints that can be cached for each 
//  SuperSpeed USB Bus-Instance. (136 + DWC_USB3_HOST_NUM_CACHE_TRB_PER_SS_EP * 16) bytes of internal RAM  
//  are allocated for each super speed endpoint cache. 
//  Choice decides the value of DWC_USB3_HOST_NUM_CACHE_EP_PER_SS_INSTANCE in DWC_usb3_params.v file.
#define DWC_USB3_HOST_NUM_CACHE_EP_PER_SS_INSTANCE 8


// Name:         DWC_USB3_HOST_NUM_CACHE_TRB_PER_SS_EP
// Default:      16
// Values:       2, ..., 126
// Enabled:      (DWC_USB3_MODE == 1 || DWC_USB3_MODE == 2)
// 
// Specifies the number of TRBs that can be cached in the core for each cached 
//  SuperSpeed endpoint. 16 bytes of internal RAM are allocated for each cached 
//  TRB. The TRB cache must be large enough to hold TRBs (including Link TRBs) 
//  for at least one maximum-packet-size packet. For better performance or when system latency  
//  is large, TRB cache for maximum-burst-size of packets is recommended.  
//  Choice decides the value of DWC_USB3_HOST_NUM_CACHE_TRB_PER_SS_EP in DWC_usb3_params.v file.
#define DWC_USB3_HOST_NUM_CACHE_TRB_PER_SS_EP 16


// Name:         DWC_USB3_HOST_NUM_CACHE_EP_PER_HS_INSTANCE
// Default:      4
// Values:       1, ..., 32
// Enabled:      (DWC_USB3_MODE == 1 || DWC_USB3_MODE == 2)
// 
// Specifies the number of High-Speed endpoints that can be cached for each 
//  High-Speed USB Bus-Instance. (120 + DWC_USB3_HOST_NUM_CACHE_TRB_PER_HS_EP * 16) bytes of internal RAM  
//  are allocated for each high speed endpoint cache. 
//  If the system bus latency is more than 2.1uS, then at least 20 endpoint cache and the following  
//  " Number of Cached TRBs for Each Cached High-Speed Endpoint" parameter size can be reduced 
//  to 10 from 16 for optimized ram size. 
//  Choice decides the value of DWC_USB3_HOST_NUM_CACHE_EP_PER_HS_INSTANCE in DWC_usb3_params.v file.
#define DWC_USB3_HOST_NUM_CACHE_EP_PER_HS_INSTANCE 2


// Name:         DWC_USB3_HOST_NUM_CACHE_TRB_PER_HS_EP
// Default:      16
// Values:       2, ..., 126
// Enabled:      (DWC_USB3_MODE == 1 || DWC_USB3_MODE == 2)
// 
// Specifies the number of TRBs that can be cached in the core for each cached 
//  High-Speed endpoint. 16 bytes of internal RAM are allocated for each cached 
//  TRB. The TRB cache must be large enough to hold TRBs (including Link TRBs) 
//  for at least one maximum-packet-size packet. For better performance, or when system latency  
//  is large, TRB cache for maximum-burst-size of packets is recommended. 
//  Choice decides the value of DWC_USB3_HOST_NUM_CACHE_TRB_PER_HS_EP in DWC_usb3_params.v file.
#define DWC_USB3_HOST_NUM_CACHE_TRB_PER_HS_EP 8


// Name:         DWC_USB3_HOST_NUM_CACHE_EP_PER_FSLS_INSTANCE
// Default:      2
// Values:       1, ..., 32
// Enabled:      (DWC_USB3_MODE == 1 || DWC_USB3_MODE == 2)
// 
// Specifies the number of Full/Low-Speed endpoints that can be cached for 
//  each Full/Low-Speed USB Bus-Instance. (120 + DWC_USB3_HOST_NUM_CACHE_TRB_PER_FSLS_EP * 16) bytes of  
//  internal RAM are allocated for each full/low speed endpoint cache. 
//  Choice decides the value of DWC_USB3_HOST_NUM_CACHE_EP_PER_FSLS_INSTANCE in DWC_usb3_params.v file.
#define DWC_USB3_HOST_NUM_CACHE_EP_PER_FSLS_INSTANCE 2


// Name:         DWC_USB3_HOST_NUM_CACHE_TRB_PER_FSLS_EP
// Default:      16
// Values:       2, ..., 126
// Enabled:      (DWC_USB3_MODE == 1 || DWC_USB3_MODE == 2)
// 
// Specifies the number of TRBs that can be cached in the core for each 
//  cached Full/Low-Speed endpoint. 16 bytes of internal RAM are allocated for 
//  each cached TRB. The TRB cache must be large enough to hold TRBs (including Link TRBs) 
//  for at least one maximum-packet-size packet. For better performance, or when system latency 
//  is large, TRB cache for more than one packets is recommended.   
//  Choice decides the value of DWC_USB3_HOST_NUM_CACHE_TRB_PER_FSLS_EP in DWC_usb3_params.v file.
#define DWC_USB3_HOST_NUM_CACHE_TRB_PER_FSLS_EP 8


// Name:         DWC_USB3_HOST_NUM_PERIODIC_EP
// Default:      32
// Values:       0, ..., 510
// Enabled:      (DWC_USB3_MODE == 1 || DWC_USB3_MODE == 2)
// 
// Specifies the number of periodic endpoints for all USB Bus-Instance. 20 
//  bytes of internal RAM are required for each periodic endpoint. 
//  Choice decides the value of DWC_USB3_HOST_NUM_PERIODIC_EP_PER_USB_INSTANCE in DWC_usb3_params.v file.
#define DWC_USB3_HOST_NUM_PERIODIC_EP 32


#define DWC_USB3_MPS 1024


// Name:         DWC_USB3_NUM_RXF_SS_PKTS
// Default:      3
// Values:       1, ..., 32
// Enabled:      (DWC_USB3_MODE == 1 || DWC_USB3_MODE == 2) && DWC_USB3_MBUS_TYPE!=4
// 
// Specifies the size of the SuperSpeed RxFIFO in number of 1024-byte packets.  
//  Each SuperSpeed USB Bus-Instance would require one RxFIFO. For example, if you have 2 SS Bus-Instances and chose  
//  3 for this parameter, then 6144 bytes of RAM is allocated for SS RxFIFO. 
//  Each 1024 bytes provides a 2.1us latency tolerance. 
//  If the system bus latency is more than 2.1uS, then at least a 5 to 6 packet buffer is recommended. 
//  For more information, see the databook's "Hardware Integration" chapter. 
//  Choice decides the value of DWC_USB3_NUM_RXF_SS_PKTS in DWC_usb3_params.v file.
#define DWC_USB3_NUM_RXF_SS_PKTS 6


#define DWC_USB3_HOST_RXFIFO_DEPTH_PER_SS_INSTANCE 768

// euse-pragma attr DefaultValue = ((@DWC_USB3_MDWIDTH >= 64) ? 1 : 0)

// Name:         DWC_USB3_BMU_SRCBUF_TYPE
// Default:      0
// Values:       0 1
// Enabled:      DWC_USB3_MODE !=3 && DWC_USB3_MBUS_TYPE!=4 && DWC_USB3_MDWIDTH >= 
//               64
// 
// Specifies alternate implementation of BMU-PTL Source Buffers. 
//  0: The FIFOs are 32 bits wide and synthesized into flops. 
//  1: The FIFO's are RAM data width; this allows a slower RAM clk for FPGA. 
//  This applies only when DWC_USB3_MDWIDTH is 64 or 128. The DWC_USB3_BMU_SRCBUF_TYPE and 
//  the DWC_USB3_BMU_SNKBUF_TYPE be enable or disabled at the same time. 
//  It is recommended to enable this for fpga testing when DWC_USB3_MDWIDTH is 64 or 128 bits. This allows you 
//  to run ram_clk at lower than 125MHz for FPGA validation.  
//  For more information, see the databook's Section B.12. 
//  Choice decides the value of DWC_USB3_BMU_PTL_DEPTH in DWC_usb3_params.v file.
#define DWC_USB3_BMU_SRCBUF_TYPE 0

// euse-pragma attr DefaultValue = ((@DWC_USB3_MDWIDTH >= 64) ? 1 : 0)

// Name:         DWC_USB3_BMU_SNKBUF_TYPE
// Default:      0
// Values:       0 1
// Enabled:      DWC_USB3_MODE !=3 && DWC_USB3_MBUS_TYPE!=4 && DWC_USB3_MDWIDTH >= 
//               64
// 
// Specifies alternate implementation of BMU-PTL Sink Buffers. 
//  0: The FIFOs are 32 bits wide and synthesized into flops. 
//  1: The FIFO's are RAM data width; this allows a slower RAM clk for FPGA 
//  This applies only when DWC_USB3_MDWIDTH is 64 or 128. The DWC_USB3_BMU_SRCBUF_TYPE and 
//  the DWC_USB3_BMU_SNKBUF_TYPE be enable or disabled at the same time. 
//  It is recommended to enable this for fpga testing when DWC_USB3_MDWIDTH is 64 or 128 bits. This allows you 
//  to run ram_clk at lower than 125MHz for FPGA validation.  
//  For more information, see the databook's section B.12. 
//  Choice decides the value of DWC_USB3_BMU_PTL_DEPTH in DWC_usb3_params.v file.
#define DWC_USB3_BMU_SNKBUF_TYPE 0


// Name:         DWC_USB3_BMU_PTL_DEPTH
// Default:      8
// Values:       2 4 8 16
// Enabled:      DWC_USB3_MODE !=3 && DWC_USB3_MBUS_TYPE!=4
// 
// Specifies the depth of the BMU-PTL Source/Sink Buffers. 
//  The FIFOs are 32 bits wide and synthesized into flops. 
//  For more information, see the databook's "Hardware Integration" chapter. 
//  Choice decides the value of DWC_USB3_BMU_PTL_DEPTH in DWC_usb3_params.v file.
#define DWC_USB3_BMU_PTL_DEPTH 8


// Name:         DWC_USB3_NUM_RXF_HS_PKTS
// Default:      2
// Values:       1, ..., 16
// Enabled:      (DWC_USB3_MODE == 1 || DWC_USB3_MODE == 2) && DWC_USB3_MBUS_TYPE!=4
// 
// Specifies the size of the High-Speed RxFIFO in number of 1024-byte packets.  
//  Each High-Speed USB Bus-Instance would require one RxFIFO. For example, if you have 2 HS Bus-Instances and chose  
//  2 for this parameter, then 4096 bytes of RAM is allocated for HS RxFIFO. 
//  Since HS ISOC max packet size is 1024, at least 2048 bytes should be allocated for HS RxFIFO. 
//  Each 1024 bytes provides 20us latency tolerance. 
//  For more information, see the databook's "Hardware Integration" chapter. 
//  Choice decides the value of DWC_USB3_NUM_RXF_HS_PKTS in DWC_usb3_params.v file.
#define DWC_USB3_NUM_RXF_HS_PKTS 2


#define DWC_USB3_HOST_RXFIFO_DEPTH_PER_HS_INSTANCE 260


// Name:         DWC_USB3_NUM_RXF_FSLS_PKTS
// Default:      1
// Values:       1, ..., 4
// Enabled:      (DWC_USB3_MODE == 1 || DWC_USB3_MODE == 2) && DWC_USB3_MBUS_TYPE!=4
// 
// Specifies the size of the Full-Speed/Low-Speed RxFIFO in number of 1024-byte packets.  
//  Each Full-Speed/Low-Speed USB Bus-Instance would require one RxFIFO. For example, if you have 1 HS Bus-Instances and chose  
//  1 for this parameter, then 1024 bytes of RAM is allocated for FS/LS RxFIFO.  
//  Since FS ISOC max payload size is 1023, at least 1024 bytes should be allocated for FS/LS RxFIFO. 
//  For more information, see the databook's "Hardware Integration" chapter. 
//  Choice decides the value of DWC_USB3_NUM_RXF_FSLS_PKTS in DWC_usb3_params.v file.
#define DWC_USB3_NUM_RXF_FSLS_PKTS 1


#define DWC_USB3_HOST_RXFIFO_DEPTH_PER_FSLS_INSTANCE 132


// Name:         DWC_USB3_NUM_TXF_SS_PKTS
// Default:      4
// Values:       1, ..., 32
// Enabled:      (DWC_USB3_MODE == 1 || DWC_USB3_MODE == 2) && DWC_USB3_MBUS_TYPE!=4
// 
// Specifies the size of the SuperSpeed TxFIFO in number of 1024-byte packets.  
//  Each SuperSpeed USB Bus-Instance would require one TxFIFO. For example, if you have 2 SS Bus-Instances and chose  
//  4 for this parameter, then 8182 bytes of RAM is allocated for SS TxFIFO. 
//  In PCIe application, the read access has more latency than write. It is recommended to allocate more TxFIFO than RxFIFO  
//  which gives better area and performance trade off. 
//  Each 1024byte provides a 2.1us latency tolerance. 
//  If the system bus latency is more than 2.1uS, then at least a 5 to 6 packet buffer is recommended. 
//  For more information, see the databook's "Hardware Integration" chapter. 
//  Choice decides the value of DWC_USB3_NUM_TXF_SS_PKTS in DWC_usb3_params.v file.  
//    Specifies the size of the TXFIFO for each host SS USB Bus-Instance. 
//  For more information, see the databook's "Hardware Integration" 
//  chapter. 
//  Choice decides the value of DWC_USB3_HOST_TXFIFO_DEPTH_PER_SS_INSTANCE in DWC_usb3_params.v file.
#define DWC_USB3_NUM_TXF_SS_PKTS 7


#define DWC_USB3_HOST_TXFIFO_DEPTH_PER_SS_INSTANCE 904


// Name:         DWC_USB3_NUM_TXF_HS_PKTS
// Default:      2
// Values:       1, ..., 16
// Enabled:      (DWC_USB3_MODE == 1 || DWC_USB3_MODE == 2) && DWC_USB3_MBUS_TYPE!=4
// 
// Specifies the size of the High-Speed TxFIFO in number of 1024-byte packets.  
//  Each High-Speed USB Bus-Instance would require one TxFIFO. For example, if you have 2 HS Bus-Instances and chose  
//  2 for this parameter, then 4096 bytes of RAM is allocated for HS TxFIFO. 
//  Since HS ISOC max packet size is 1024, at least 2048 bytes should be allocated for HS TxFIFO. 
//  Each 1024 bytes provides 20us latency tolerance. 
//  For more information, see the databook's "Hardware Integration" chapter. 
//  Choice decides the value of DWC_USB3_NUM_TXF_HS_PKTS in DWC_usb3_params.v file.
#define DWC_USB3_NUM_TXF_HS_PKTS 2


#define DWC_USB3_HOST_TXFIFO_DEPTH_PER_HS_INSTANCE 259


// Name:         DWC_USB3_NUM_TXF_FSLS_PKTS
// Default:      1
// Values:       1, ..., 4
// Enabled:      (DWC_USB3_MODE == 1 || DWC_USB3_MODE == 2) && DWC_USB3_MBUS_TYPE!=4
// 
// Specifies the size of the Full-Speed/Low-Speed TxFIFO in number of 1024-byte packets.  
//  Each Full-Speed/Low-Speed USB Bus-Instance would require one TxFIFO. For example, if you have 1 HS Bus-Instances and chose  
//  1 for this parameter, then 1024 bytes of RAM is allocated for FS/LS TxFIFO.  
//  Since FS ISOC max payload size is 1023, at least 1024 bytes should be allocated for FS/LS TxFIFO. 
//  For more information, see the databook's "Hardware Integration" chapter. 
//  Choice decides the value of DWC_USB3_NUM_TXF_FSLS_PKTS in DWC_usb3_params.v file.
#define DWC_USB3_NUM_TXF_FSLS_PKTS 1


#define DWC_USB3_HOST_TXFIFO_DEPTH_PER_FSLS_INSTANCE 130


// Name:         DWC_USB3_HOST_RXMSG_DEPTH_PER_SS_INSTANCE
// Default:      16
// Values:       16 32
// Enabled:      (DWC_USB3_MODE == 1 || DWC_USB3_MODE == 2) && DWC_USB3_MBUS_TYPE!=4
// 
// Specifies the size of the BMU Protocol Status Queue for each host SS USB Bus-Instance. 
//  They are actually the PTL Message Sink Buffers. 
//  The FIFOs are 64 bits wide and synthesized into flops. 
//  For more information, see the databook's "Hardware Integration" 
//  chapter. 
//  Choice decides the value of DWC_USB3_HOST_RXMSG_DEPTH_PER_SS_INSTANCE in DWC_usb3_params.v file.
#define DWC_USB3_HOST_RXMSG_DEPTH_PER_SS_INSTANCE 16


// Name:         DWC_USB3_LEGACY_SUPT
// Default:      No
// Values:       No (0), Yes (1)
// Enabled:      (DWC_USB3_MODE == 1 || DWC_USB3_MODE == 2)
// 
// Specifies whether to enable Legacy Keyboard/Mice BIOS support (IRQ and Address 60/64 I/O access). 
//  This is different from the xHCI's SMI Legacy feature specified in the section 7.1 of xHCI specification. 
//  The xHCI's SMI Legacy feature is always enabled.  
//  Likely just xHCI's SMI Legacy feature is enough for most applications and this Legacy Keyboard/Mice BIOS support 
//  is normally not needed. This implements the features required in: 
//  "Universal Serial Bus PC Legacy Compatibility Specification 0.9 Draft Revision May 30, 1996".
#define DWC_USB3_LEGACY_SUPT 0


// Name:         DWC_USB3_GSBUSCFG0_INIT
// Default:      0x1
// Values:       0x0, ..., 0xffffffff
// Enabled:      (DWC_USB3_MODE == 1 || DWC_USB3_MODE == 2) && DWC_USB3_MBUS_TYPE!=4
// 
// Specifies the Global System Bus Configuration Register-0's (GSBUSCFG0) 
//  power-on initialization value. The GSBUSCFG0 register defines the system bus configuration. 
//  The bits other than defined here must be 0. The different fields are: 
//   bit[31:28] AHB-prot/AXI-cache/OCP-ReqInfo for Data Read 
//   bit[27:24] AHB-prot/AXI-cache/OCP-ReqInfo for Descriptor Read 
//   bit[23:20] AHB-prot/AXI-cache/OCP-ReqInfo for Data Write 
//   bit[19:16] AHB-prot/AXI-cache/OCP-ReqInfo for Descriptor Write 
//   bit[15]    1'b1 - Read back the Last word of the descriptor write before setting the 
//                     XferComplete interrupt 
//   bit[14]    1'b1 - For AXI/OCP interface, force only one request to be sent on the bus at a time 
//   bit[12]    1'b1 - Enable BUS-GM Store-and-Forward mode  
//   bit[11]    1'b1 - Data Access is Big Endian 
//   bit[10]    1'b1 - Descriptor Access is Big Endian 
//   bit[9]     1'b1 - Data Write is Posted 
//   bit[8]     1'b1 - Descriptor Write is Posted 
//   bit[7]     1'b1 - Enable INCR256 Burst Type 
//   bit[6]     1'b1 - Enable INCR128 Burst Type 
//   bit[5]     1'b1 - Enable INCR64 Burst Type 
//   bit[4]     1'b1 - Enable INCR32 Burst Type 
//   bit[3]     1'b1 - Enable INCR16 Burst Type 
//   bit[2]     1'b1 - Enable INCR8 Burst Type 
//   bit[1]     1'b1 - Enable INCR4 Burst Type 
//   bit[0]     1'b1 - Enable Undefined Length INCR Burst Type 
//  If you use a standard driver, such as the xHCI driver from Microsoft, you must initialize 
//  this register to meet your system requirements before synthesizing the core. If you are  
//  developing your own xHCI host driver, then this register must be configured by your driver. 
//  For more information, see the "Control and Status Registers" chapter in the Databook. 
//  Choice decides the value of DWC_USB3_GSBUSCFG0_INIT in DWC_usb3_params.v file.
#define DWC_USB3_GSBUSCFG0_INIT 32'h1


// Name:         DWC_USB3_GSBUSCFG1_INIT
// Default:      0x300
// Values:       0x0, ..., 0xffffffff
// Enabled:      (DWC_USB3_MODE == 1 || DWC_USB3_MODE == 2) && DWC_USB3_MBUS_TYPE!=4
// 
// Specifies the Global System Bus Configuration Register-1's (GSBUSCFG1) 
//  power-on initialization value. The GSBUSCFG1 register defines the system bus configuration. 
//  The bits other than defined here must be 0. The different fields are: 
//  bit [7:4] OCP Address Space for Data 
//  bit [3:0] OCP Address Space for Descriptor 
//  If you use a standard driver, such as the xHCI driver from Microsoft, you must initialize 
//  this register to meet your system requirements before synthesizing the core. If you are 
//  developing your own xHCI host driver, then this register must be configured by your driver. 
//  For more information, see the "Control and Status Registers" chapter in the Databook. 
//  Choice decides the value of DWC_USB3_GSBUSCFG1_INIT in DWC_usb3_params.v file.
#define DWC_USB3_GSBUSCFG1_INIT 32'h300


// Name:         DWC_USB3_GTXTHRCFG_INIT
// Default:      0x0
// Values:       0x0, ..., 0xffffffff
// Enabled:      (DWC_USB3_MODE == 1 || DWC_USB3_MODE == 2) && DWC_USB3_MBUS_TYPE!=4
// 
// Specifies the Global Transmit Threshold Configuration Register (GTXTHRCFG) 
//  power-on initialization value. This register contains threshold enable bits 
//  and threshold count fields. To enable USB or DMA byte or packet thresholding 
//  upon power-on, then you need to update this value. 
//  For more information, see the "Control and Status Registers" chapter in the Databook. 
//  Choice decides the value of DWC_USB3_GTXTHRCFG_INIT in DWC_usb3_params.v file.
#define DWC_USB3_GTXTHRCFG_INIT 32'h0
//`efine DWC_USB3_GTXTHRCFG_INIT ((2 << 24) + ((`DWC_USB3_MPS >> (`DWC_USB3_MADDR_LO+1)) << 16)) + (8 << 0)


// Name:         DWC_USB3_GRXTHRCFG_INIT
// Default:      0x0
// Values:       0x0, ..., 0xffffffff
// Enabled:      (DWC_USB3_MODE == 1 || DWC_USB3_MODE == 2) && DWC_USB3_MBUS_TYPE!=4
// 
// Specifies the Global Receive Threshold Configuration Register (GRXTHRCFG) 
//  power-on initialization value. This register contains threshold enable bits 
//  and threshold count fields. To enable USB or DMA byte or packet thresholding 
//  upon power-on, then you need to update this value. 
//  For more information, see the "Control and Status Registers" chapter in the Databook. 
//  Choice decides the value of DWC_USB3_GRXTHRCFG_INIT in DWC_usb3_params.v file.
#define DWC_USB3_GRXTHRCFG_INIT 32'h0
//`efine DWC_USB3_GRXTHRCFG_INIT (2 << 24) + (8 << 0)


// Name:         DWC_USB3_GCTL_INIT
// Default:      0x30c02000
// Values:       0x0, ..., 0xffffffff
// Enabled:      (DWC_USB3_MODE == 1 || DWC_USB3_MODE == 2) && DWC_USB3_MBUS_TYPE!=4
// 
// Specifies the Global Control Register-1's (GCTL) power-on initialization value. 
//  bit [31:19] - Suspend Clock Scaledown value (suspend_clk_period/mac3_clk_period) 
//  bit [18:17] - Reserved 
//  bit [16]    -  Attempt 3 SS connection 
//  bit [15:14] - SOF Scaledown Value 
//  bit [13:12] Port Capability Direction 
//                2'b01 - for Host configurations 
//                2'b10 - for Device configurations 
//                2'b11 - for OTG configurations 
//  bit [7:6]   RAM Clock Select  
//                2'b00: bus clock 
//                2'b01: pipe clock 
//                2'b10: pipe/2 clock 
//                2'b11: mac2 clock 
//  bit [5:4]   Scaledown Mode  
//  bit [3]     1'b0 - Enable Scrambling 
//              1'b1 - Disable Scrambling 
//  bit [2]     1'b0 - Clamp the interface signals of SS modules.  
//              1'b1 - No clamp the interface signals of SS modules.  
//  bit [1]     1'b0 - Clamp the interface signals of the HS/FS/LS modules  
//              1'b1 - No clamp the interface signals of the HS/FS/LS modules  
//  bit [0]     1'b0 - Enable internal clock gating. 
//              1'b1 - Disable internal clock gating. 
//  If you use a standard driver, such as the xHCI driver from Microsoft, you must initialize 
//  this register to meet your system requirements before synthesizing the core. If you are 
//  developing your own xHCI host driver, then this register must be configured by your driver. 
//  For more information, see the "Control and Status Registers" chapter in the Databook. 
//  Choice decides the value of DWC_USB3_GCTL_INIT in DWC_usb3_params.v file.
#define DWC_USB3_GCTL_INIT 32'h30c02000


// Name:         DWC_USB3_HOST_EN_EXTERNAL_CAP
// Default:      No
// Values:       No (0), Yes (1)
// Enabled:      (DWC_USB3_MODE == 1 || DWC_USB3_MODE == 2) && DWC_USB3_MBUS_TYPE!=4
// 
// Enables extended capabilities to be implemented outside the core. 
//  If enabled, a read to the first DWORD of the last internal extended capability (currently the "xHCI 
//  Supported Protocol Capability" for USB 3.0) will return a value of 4 in the "Next Capability Pointer" field.  This 
//  will indicate to software that there is another capability 4 DWORDs after this capability (for example, at address 
//  N+16 where N is the address of this DWORD). 
//  If disabled, a read to the same DWORD will return 0 in the "Next Capability Pointer" field.  This indicates there are 
//  no more capabilities. 
//  If enabled, an external address decoder that snoops the xHC slave interface will need to be implemented.  If it sees 
//  an access to N+16 or greater, the slave access is re-routed to a piece of hardware which returns the external 
//  capability pointer register of the new capability and also handles reads/writes to this new capability and the 
//  side-effects.
#define DWC_USB3_HOST_EN_EXTERNAL_CAP 0

/********** Hub Specific Parameters ***********/


// Name:         DWC_USB3_HUB_NUM_PORTS
// Default:      1
// Values:       1 2 3 4 5 6 7 8 9 10 11 12 13 14 15
// Enabled:      DWC_USB3_MODE == 3
// 
// Specifies the number of down-stream Hub ports. This includes both 
//  USB 3.0 and USB 2.0 ports. The area of a USB 3.0 port is about 
//  xKGates, and the area of an USB 2.0 port is yKGates. 
//  Choice decides the value of DWC_USB3_HUB_NUM_PORTS in DWC_usb3_params.v file.
#define DWC_USB3_HUB_NUM_PORTS 1


// Name:         DWC_USB3_HUB_NUM_U3_PORTS
// Default:      4
// Values:       1 2 3 4 5 6 7 8 9 10 11 12 13 14 15
// Enabled:      DWC_USB3_MODE == 3
// 
// Specifies the number of USB 3.0 capable down-stream Hub ports. 
//  Choice decides the value of DWC_USB3_HUB_NUM_U3_PORTS in DWC_usb3_params.v file.
#define DWC_USB3_HUB_NUM_U3_PORTS 4


// Name:         DWC_USB3_EN_HUB_SFLASH_SUPT
// Default:      No
// Values:       No (0), Yes (1)
// Enabled:      DWC_USB3_MODE == 3
// 
// Specifies whether Xilinx Serial Flash-RAM/PROM support is required to 
//  load Hub Descriptors. 
//  The memory requirements for the Hub descriptor is 256bytes (2048bits). 
//  Refer to Xilinx website for a Serial Flash/PROM that meets your 
//  requirements. Most Xilinx Serial Flash/PROM are much 
//  larger in size than the required 2048bits. 
//  Choice decides the value of DWC_USB3_EN_HUB_SFLASH_SUPT in DWC_usb3_params.v file.
#define DWC_USB3_EN_HUB_SFLASH_SUPT 0


// Name:         DWC_USB3_HUB_SFLASH_DIVFACTOR
// Default:      16
// Values:       4, ..., 128
// Enabled:      DWC_USB3_MODE == 3
// 
// Specifies the MAC clock(125MHz) to Serial Flash/PROM clock divide factor. 
//  The typical Xilinx Serial Flash/PROM memory operating frequency range 
//  is 2.5 to 20 MHz. Refer to Xilinx website for a Serial Flash/PROM that 
//  meets your requirements. 
//  Choice decides the value of DWC_USB3_HUB_SFLASH_DIVFACTOR in DWC_usb3_params.v file.
#define DWC_USB3_HUB_SFLASH_DIVFACTOR 16


// Name:         DWC_USB3_JTAG_INTERFACE
// Default:      No
// Values:       No (0), Yes (1)
// Enabled:      DWC_USB3_MODE ==3
// 
// Specifies whether JTAG debug interface is enabled. 
//  Choice decides the value of DWC_USB3_JTAG_INTERFACE in DWC_usb3_params.v file.
#define DWC_USB3_JTAG_INTERFACE 0


// Name:         DWC_USB3_HUB_NUM_RXHEADERS
// Default:      4
// Values:       4 8
// Enabled:      DWC_USB3_MODE ==3
// 
// Specifies the number of Headers in each Rx Header Buffers 
//  Choice decides the value of DWC_USB3_HUB_NUM_RXHEADERS in DWC_usb3_params.v file.
#define DWC_USB3_HUB_NUM_RXHEADERS 4


// Name:         DWC_USB3_HUB_NUM_TXHEADERS
// Default:      4
// Values:       4 8
// Enabled:      DWC_USB3_MODE ==3
// 
// Specifies the number of Headers in each Tx Header Buffers 
//  Choice decides the value of DWC_USB3_HUB_NUM_TXHEADERS in DWC_usb3_params.v file.
#define DWC_USB3_HUB_NUM_TXHEADERS 4


// Name:         DWC_USB3_HUB_NUM_UPSTRMRX_DATAPKTS
// Default:      1
// Values:       1 2 3 4 5 6 7 8
// Enabled:      DWC_USB3_MODE ==3
// 
// Specifies the number of packets in Upstream Rx Data Packet Buffer.  Each packet 
//  requires 1024 bytes of space. 
//  Choice decides the value of DWC_USB3_HUB_NUM_UPSTRMRX_DATAPKTS in DWC_usb3_params.v file.
#define DWC_USB3_HUB_NUM_UPSTRMRX_DATAPKTS 1


// Name:         DWC_USB3_HUB_NUM_UPSTRMTX_DATAPKTS
// Default:      1
// Values:       1 2 3 4 5 6 7 8
// Enabled:      DWC_USB3_MODE ==3
// 
// Specifies the number Packets in Upstream Tx Data Packet Buffer.  Each packet needs 1024 bytes of space. 
//  Choice decides the value of DWC_USB3_HUB_NUM_UPSTRMTX_DATAPKTS in DWC_usb3_params.v file.
#define DWC_USB3_HUB_NUM_UPSTRMTX_DATAPKTS 1


// Name:         DWC_USB3_HUB_DESC_DEPTH
// Default:      64
// Values:       64 128 256
// Enabled:      DWC_USB3_MODE ==3
// 
// Specifies the depth of the Descriptor ROM or Serial-Flash in 32-bit Dwords.  
//  For example a value of 64 represents a 256byte (64 * 4) ROM. 
//  Choice decides the value of DWC_USB3_HUB_DESC_DEPTH in DWC_usb3_params.v file.
#define DWC_USB3_HUB_DESC_DEPTH 64


// Name:         DWC_USB3_HUB_DESC_SS_START_DEPTH
// Default:      0
// Values:       0 64 128
// Enabled:      DWC_USB3_MODE ==3
// 
// Specifies the starting depth of the Descriptor ROM or Serial-Flash in 32-bit Dwords used for DWC_usb3 core. 
//  If you share the same ROM or Serial-Flash between USB2.0 hub and USB3.0 SS Hub, the lower portion 
//  of the ROM or Serial-Flash will have data for the USB2.0 Hub and the upper part will have data for the SS Hub. 
//  The DWC_usb3 controller will read the entire RAM or Serial-Flash but will use only the data from this depth. 
//  The DWC_usb3 controller provides the USB2.0 descriptors to the USB2.0 controller through the "hub_usb2_desc" interface.
#define DWC_USB3_HUB_DESC_SS_START_DEPTH 0


#define DWC_USB3_NUM_ROOT_PORTS 1


#define DWC_USB3_NUM_U2_ROOT_PORTS 1


#define DWC_USB3_NUM_U3_ROOT_PORTS 1


#define DWC_USB3_EN_HUB_SFLASH_SUPT_SIM 0


// Name:         DWC_USB3_HUB_ENABLE_SCRAMBLING
// Default:      Disable
// Values:       Disable (0), Enable (1)
// Enabled:      DWC_USB3_MODE ==3
// 
// This option only affects the Simulation. 
//  This controls if scrambling is enabled/disabled when running hub tests in coreConsultant. 
//  This directly controls the hub_enable_scrambling input pin of the Hub. 
//  0 - A value of 0 will be assigned to "hub_enable_scrambling" input to disable scrambling on all ports. 
//  1 - A value of 1 will be assigned to "hub_enable_scrambling" input to enable scrambling on all ports.
#define DWC_USB3_HUB_ENABLE_SCRAMBLING 0



/********** Advanced Parameters ***********/


// Name:         DWC_USB3_BMU_LSP_DEPTH
// Default:      4
// Values:       1 4 8
// Enabled:      DWC_USB3_MODE !=3 && DWC_USB3_MBUS_TYPE!=4
// 
// Specifies the depth of the BMU-LSP Status Buffer. 
//  The FIFO is 26 bits wide and synthesized into flops. 
//  For more information, see the databook's "Hardware Integration" chapter. 
//  Choice decides the value of DWC_USB3_BMU_LSP_DEPTH in DWC_usb3_params.v file.
#define DWC_USB3_BMU_LSP_DEPTH 4


// Name:         DWC_USB3_WBUFF
// Default:      4
// Values:       3 4 5 6 7 8
// Enabled:      DWC_USB3_MODE !=3 && DWC_USB3_MBUS_TYPE!=4
// 
// Specifies the depth of the AHB-GM Buffers.   
//  The FIFOs are 32/64/128 bits wide, matching the bus master 
//  data width, and synthesized into flops. 
//  For more information, see the databook's "Hardware Integration" chapter in the Databook. 
//  Choice decides the value of DWC_USB3_WBUFF in DWC_usb3_params.v file.
#define DWC_USB3_WBUFF 4


#define DWC_USB3_RBUFF 2


#define DWC_USB3_COMBO 0


#define DWC_USB3_MAX_RREQ 16

#define DWC_USB3_MAX_WREQ 16

#define DWC_USB3_WRESP_BUFF 4

#define DWC_USB3_WREQ_BUFF 4

#define DWC_USB3_RREQ_BUFF 4


#define DWC_USB3_MPS_TXF0 512

#define DWC_USB3_NUM_RXF_PKTS 3

#define DWC_USB3_NUM_TXF_PKTS 3

#define DWC_USB3_TXDMA_CMD_FIFO_DEPTH 2

#define DWC_USB3_TXDMA_CMD_MSEG_FIFO_DEPTH 1

#define DWC_USB3_RXDMA_CMD_FIFO_DEPTH 1

#define DWC_USB3_RXDMA_CMD_MSEG_FIFO_DEPTH 1

#define DWC_USB3_DFDMA_CMD_FIFO_DEPTH 2

#define DWC_USB3_DWDMA_CMD_FIFO_DEPTH 2


// Name:         DWC_USB3_BMU_BUSGM_DEPTH
// Default:      8
// Values:       4 8
// Enabled:      DWC_USB3_MODE !=3 && DWC_USB3_MBUS_TYPE!=4
// 
// Specifies the depth of the BMU-BUSGM Source/Sink Buffers. 
//  The FIFOs are 32/64/128 bits wide, matching the bus master 
//  data width, and synthesized into flops. The "AHB Master Source/Sink Buffer" and  
//  "BMU-BUSGM Source/Sink Buffers" combined together should be large enough to  
//  to meet your bus burst without waitstates. This provides rate-matching between  
//  bus clock and ram clock domains. The area of 16, 32-bit register is around 5KGates.  
//  For more information, see the databook's "Hardware Integration" chapter. 
//  Choice decides the value of DWC_USB3_BMU_BUSGM_DEPTH in DWC_usb3_params.v file.
#define DWC_USB3_BMU_BUSGM_DEPTH 8


// Name:         DWC_USB3_RXQ_FIFO_DEPTH
// Default:      16
// Values:       16 32 64
// Enabled:      DWC_USB3_MODE ==0 && DWC_USB3_MBUS_TYPE!=4
// 
// Specifies the size of the BMU Rx Request Queue. 
//  The specified depth is allocated in the Data FIFO RAM 
//  32/64/128 bits wide) and defines the number of Rx commands 
//  the Scheduler can queue to the BMU. A typical Rx command 
//  comprises 2 to 4 32-bit DWORDs. 
//  After power-up, software can dynamically change the memory 
//  allocation among the Rx Data FIFO, Tx Data FIFO, and 
//  Rx Request Queue. 
//  For more information, see the databook's "Hardware Integration" chapter. 
//  Choice decides the value of DWC_USB3_RXQ_FIFO_DEPTH in DWC_usb3_params.v file.
#define DWC_USB3_RXQ_FIFO_DEPTH 16
//`efine DWC_USB3_RXQ_FIFO_DEPTH ((`DWC_USB3_NUM_RXF_PKTS+2) * `DWC_USB3_RXDMA_CMD_FIFO_DEPTH)


// Name:         DWC_USB3_HOST_RXQ_DEPTH_PER_SS_INSTANCE
// Default:      62
// Values:       2, ..., 1024
// Enabled:      DWC_USB3_MODE ==1
// 
// Specifies the size of the BMU Rx Request Queue for each host SS USB Bus-Instance. 
//  The specified depth is allocated in the Data FIFO RAM 
//  32/64/128 bits wide) and defines the number of Rx commands 
//  the Scheduler can queue to the BMU. A typical Rx command 
//  comprises 2 or 4 32-bit DWORDs. 
//  After power-up, software can dynamically change the memory 
//  allocation among the Rx Data FIFO, Tx Data FIFO, and 
//  Rx Request Queue. 
//  For more information, see the databook's "Hardware Integration" chapter. 
//  Choice decides the value of DWC_USB3_HOST_RXQ_DEPTH_PER_SS_INSTANCE in DWC_usb3_params.v file.
#define DWC_USB3_HOST_RXQ_DEPTH_PER_SS_INSTANCE 62


// Name:         DWC_USB3_HOST_RXQ_DEPTH_PER_HS_INSTANCE
// Default:      20
// Values:       2, ..., 1024
// Enabled:      DWC_USB3_MODE ==1
// 
// Specifies the size of the BMU Rx Request Queue for each host HS USB Bus-Instance. 
//  The specified depth is allocated in the Data FIFO RAM 
//  32/64/128 bits wide) and defines the number of Rx commands 
//  the Scheduler can queue to the BMU. A typical Rx command 
//  comprises 2 or 4 32-bit DWORDs. 
//  After power-up, software can dynamically change the memory 
//  allocation among the Rx Data FIFO, Tx Data FIFO, and 
//  Rx Request Queue. 
//  For more information, see the databook's "Hardware Integration" chapter. 
//  Choice decides the value of DWC_USB3_HOST_RXQ_DEPTH_PER_HS_INSTANCE in DWC_usb3_params.v file.
#define DWC_USB3_HOST_RXQ_DEPTH_PER_HS_INSTANCE 20


// Name:         DWC_USB3_HOST_RXQ_DEPTH_PER_FSLS_INSTANCE
// Default:      16
// Values:       2, ..., 1024
// Enabled:      DWC_USB3_MODE ==1
// 
// Specifies the size of the BMU Rx Request Queue for each host FS/LS USB Bus-Instance. 
//  The specified depth is allocated in the Data FIFO RAM 
//  32/64/128 bits wide) and defines the number of Rx commands 
//  the Scheduler can queue to the BMU. A typical Rx command 
//  comprises 2 or 4 32-bit DWORDs. 
//  After power-up, software can dynamically change the memory 
//  allocation among the Rx Data FIFO, Tx Data FIFO, and 
//  Rx Request Queue. 
//  For more information, see the databook's "Hardware Integration" chapter. 
//  Choice decides the value of DWC_USB3_HOST_RXQ_DEPTH_PER_FSLS_INSTANCE in DWC_usb3_params.v file.
#define DWC_USB3_HOST_RXQ_DEPTH_PER_FSLS_INSTANCE 16


// Name:         DWC_USB3_TXQ_FIFO_DEPTH
// Default:      16
// Values:       16 32
// Enabled:      DWC_USB3_MODE ==0 && DWC_USB3_MBUS_TYPE!=4
// 
// Specifies the size of the BMU Tx Request Queue. 
//  The specified depth is allocated in the Data FIFO RAM 
//  (32/64/128 bits wide) and defines the number of Tx commands 
//  the Scheduler can queue to the BMU. A typical Tx command 
//  comprises 3 or 5 32-bit DWORDs. 
//  For more information, see the databook's "Hardware Integration" chapter. 
//  Choice decides the value of DWC_USB3_TXQ_FIFO_DEPTH in DWC_usb3_params.v file.
#define DWC_USB3_TXQ_FIFO_DEPTH 16
//#define DWC_USB3_TXQ_FIFO_DEPTH ((`DWC_USB3_NUM_TXF_PKTS+2) * `DWC_USB3_TXDMA_CMD_FIFO_DEPTH)


// Name:         DWC_USB3_HOST_TXQ_DEPTH_PER_SS_INSTANCE
// Default:      141
// Values:       2, ..., 1024
// Enabled:      DWC_USB3_MODE ==1
// 
// Specifies the size of the BMU Tx Request Queue for each host SS USB Bus-Instance. 
//  The specified depth is allocated in the Data FIFO RAM 
//  (32/64/128 bits wide) and defines the number of Tx commands 
//  the Scheduler can queue to the BMU. A typical Tx command 
//  comprises 3 or 5 32-bit DWORDs. 
//  For more information, see the databook's "Hardware Integration" chapter. 
//  Choice decides the value of DWC_USB3_HOST_TXQ_DEPTH_PER_SS_INSTANCE in DWC_usb3_params.v file.
#define DWC_USB3_HOST_TXQ_DEPTH_PER_SS_INSTANCE 141


// Name:         DWC_USB3_HOST_TXQ_DEPTH_PER_HS_INSTANCE
// Default:      26
// Values:       2, ..., 1024
// Enabled:      DWC_USB3_MODE ==1
// 
// Specifies the size of the BMU Tx Request Queue for each host HS USB Bus-Instance. 
//  The specified depth is allocated in the Data FIFO RAM 
//  (32/64/128 bits wide) and defines the number of Tx commands 
//  the Scheduler can queue to the BMU. A typical Tx command 
//  comprises 3 or 5 32-bit DWORDs. 
//  For more information, see the databook's "Hardware Integration" chapter. 
//  Choice decides the value of DWC_USB3_HOST_TXQ_DEPTH_PER_HS_INSTANCE in DWC_usb3_params.v file.
#define DWC_USB3_HOST_TXQ_DEPTH_PER_HS_INSTANCE 26


// Name:         DWC_USB3_HOST_TXQ_DEPTH_PER_FSLS_INSTANCE
// Default:      18
// Values:       2, ..., 1024
// Enabled:      DWC_USB3_MODE ==1
// 
// Specifies the size of the BMU Tx Request Queue for each host FS/LS USB Bus-Instance. 
//  The specified depth is allocated in the Data FIFO RAM 
//  (32/64/128 bits wide) and defines the number of Tx commands 
//  the Scheduler can queue to the BMU. A typical Tx command 
//  comprises 3 or 5 32-bit DWORDs. 
//  For more information, see the databook's "Hardware Integration" chapter. 
//  Choice decides the value of DWC_USB3_HOST_TXQ_DEPTH_PER_SS_INSTANCE in DWC_usb3_params.v file.
#define DWC_USB3_HOST_TXQ_DEPTH_PER_FSLS_INSTANCE 18


// Name:         DWC_USB3_TXPFL_FIFO_DEPTH
// Default:      1
// Values:       1
// Enabled:      DWC_USB3_MODE ==1
// 
// Specifies the size of the BMU Tx Packet Flush Queue. 
//  The specified depth is allocated in flop-based FIFO 
//  (1 bit wide) and defines the number of Tx Packet Flush commands 
//  the Scheduler can queue to the BMU. 
//  For more information, see the databook's "Hardware Integration" chapter. 
//  Choice decides the value of DWC_USB3_TXPFL_FIFO_DEPTH in DWC_usb3_params.v file.
#define DWC_USB3_TXPFL_FIFO_DEPTH 1


// Name:         DWC_USB3_DWQ_FIFO_DEPTH
// Default:      32
// Values:       4, ..., 1024
// Enabled:      DWC_USB3_MODE !=3 && DWC_USB3_MBUS_TYPE!=4
// 
// Specifies the size of the BMU Descriptor Write Queue. 
//  The specified depth is allocated in the Data FIFO RAM 
//  (32/64/128 bits wide) and defines the number of descriptor write 
//  commands the Scheduler can queue to the BMU. A typical descriptor 
//  write command comprises five 32-bit DWORDs. 
//  For more information, see the databook's "Hardware Integration" chapter. 
//  Choice decides the value of DWC_USB3_DWQ_FIFO_DEPTH in DWC_usb3_params.v file.
#define DWC_USB3_DWQ_FIFO_DEPTH 32
//#define DWC_USB3_DWQ_FIFO_DEPTH (`DWC_USB3_DWDMA_CMD_FIFO_DEPTH * (2 + `DWC_USB3_NUM_TXFIFO + (`DWC_USB3_NUM_RXF_PKTS * `DWC_USB3_NUM_RXFIFO)))


// Name:         DWC_USB3_DFQ_FIFO_DEPTH
// Default:      16
// Values:       4, ..., 1024
// Enabled:      DWC_USB3_MODE !=3 && DWC_USB3_MBUS_TYPE!=4
// 
// Specifies the size of the BMU Descriptor Fetch Request Queue. 
//  The specified depth is allocated in the Data FIFO RAM 
//  (32/64/128 bits wide) and defines the number of descriptor fetch 
//  commands the Scheduler can queue to the BMU. A typical descriptor 
//  fetch command comprises three 32-bit DWORDs. 
//  For more information, see the databook's "Hardware Integration" chapter. 
//  Choice decides the value of DWC_USB3_DFQ_FIFO_DEPTH in DWC_usb3_params.v file.
#define DWC_USB3_DFQ_FIFO_DEPTH 16
//#define DWC_USB3_DFQ_FIFO_DEPTH (`DWC_USB3_DFDMA_CMD_FIFO_DEPTH * (`DWC_USB3_NUM_TXFIFO + (`DWC_USB3_NUM_RXF_PKTS * `DWC_USB3_NUM_RXFIFO)))


// Name:         DWC_USB3_PSQ_FIFO_DEPTH
// Default:      32
// Values:       16 32
// Enabled:      DWC_USB3_MODE !=3 && DWC_USB3_MBUS_TYPE!=4
// 
// Specifies the size of the BMU Protocol Status Queue. 
//  The specified depth is allocated in the Data FIFO RAM 
//  (32/64/128 bits wide) and defines the number of header and status 
//  dwords the PTL can queue to the LSP. For each RX packet, the PTL 
//  queues a total of two entries: one header and one status. For each 
//  TX packet, the PTL queues one entry for the status. 
//  For more information, see the databook's "Hardware Integration" chapter. 
//  Choice decides the value of DWC_USB3_PSQ_FIFO_DEPTH in DWC_usb3_params.v file.
#define DWC_USB3_PSQ_FIFO_DEPTH 32


// Name:         DWC_USB3_DEV_RXMSG_FIFO_DEPTH
// Default:      4
// Values:       4
// Enabled:      DWC_USB3_MODE !=3 && DWC_USB3_MBUS_TYPE!=4
// 
// Specifies the size of the Device-mode BMU RX Message Buffer. 
//  It is actually the PTL Message Sink Buffers. 
//  The FIFO is 32 bits wide and synthesized into flops. 
//  Choice decides the value of DWC_USB3_DEV_RXMSG_FIFO_DEPTH in DWC_usb3_params.v file.
#define DWC_USB3_DEV_RXMSG_FIFO_DEPTH 4

// MAC requires RXFIFO space of 1KB packet + three 8-byte setup packets + 16 byte synchronization delay

#define DWC_USB3_DEV_RXFIFO_OK_SPACE_MARGIN 10

#define DWC_USB3_DEV_RXFIFO_OK_SPACE 266

#define DWC_USB3_HOST_RXFIFO_OK_SPACE 256



// Name:         DWC_USB3_HOST_RXMSG_DEPTH_PER_HS_INSTANCE
// Default:      8
// Values:       8
// Enabled:      (DWC_USB3_MODE == 1 || DWC_USB3_MODE == 2) && DWC_USB3_MBUS_TYPE!=4
// 
// Specifies the size of the BMU Protocol Status Queue for each host HS USB Bus-Instance. 
//  They are actually the PTL Message Sink Buffers. 
//  The FIFOs are 64 bits wide and synthesized into flops. 
//  For more information, see the databook's "Hardware Integration" 
//  chapter. 
//  Choice decides the value of DWC_USB3_HOST_RXMSG_DEPTH_PER_HS_INSTANCE in DWC_usb3_params.v file.
#define DWC_USB3_HOST_RXMSG_DEPTH_PER_HS_INSTANCE 8


// Name:         DWC_USB3_HOST_RXMSG_DEPTH_PER_FSLS_INSTANCE
// Default:      2
// Values:       2
// Enabled:      (DWC_USB3_MODE == 1 || DWC_USB3_MODE == 2) && DWC_USB3_MBUS_TYPE!=4
// 
// Specifies the size of the BMU Protocol Status Queue for each host FS/LS USB Bus-Instance. 
//  They are actually the PTL Message Sink Buffers. 
//  The FIFOs are 64 bits wide and synthesized into flops. 
//  For more information, see the databook's "Hardware Integration" 
//  chapter. 
//  Choice decides the value of DWC_USB3_HOST_RXMSG_DEPTH_PER_FSLS_INSTANCE in DWC_usb3_params.v file.
#define DWC_USB3_HOST_RXMSG_DEPTH_PER_FSLS_INSTANCE 2


// Name:         DWC_USB3_PIU_RING_BUFFER_DEPTH_P2
// Default:      4
// Values:       3 4 5
// Enabled:      DWC_USB3_NUM_U3_ROOT_PORTS!=1
// 
// Specifies the depth of the PIPE3 PIU Tx and Rx ring buffers. 
//  The ring buffers are 32 bits wide and synthesized into flops. 
//  For more information, see the databook's "Hardware Integration" chapter. 
//  Choice decides the value of DWC_USB3_PIU_RING_DEPTH_P2 in DWC_usb3_params.v file.  
//  Ring buffer depth will be 2**value of the parameter
#define DWC_USB3_PIU_RING_BUFFER_DEPTH_P2 4



//------------------------------------------------------------------------------------------
// Macros
//------------------------------------------------------------------------------------------
// Extra parentheses ensure correct order of operations for complex expressions

// Returns minimum/maximum value
#define DWC_USB3_MIN_CB(a, b) (((a) < (b)) ? (a): (b))
#define DWC_USB3_MAX_CB(a, b) (((a) > (b)) ? (a): (b))

#define DWC_USB3_LOG2(a) (((a)<=2) ? 1: ((a)<=4) ? 2: ((a)<=8) ? 3: ((a)<=16) ? 4: ((a)<=32) ? 5: ((a)<=64) ? 6: ((a)<=128) ? 7: ((a)<=256) ? 8: ((a)<=512) ? 9: ((a)<=1024) ? 10: ((a)<=2048) ? 11: ((a)<=4096) ? 12: ((a)<=8192) ? 13: ((a)<=16384) ? 14: ((a)<=32768) ? 15: ((a)<=65536) ? 16: ((a)<=131072) ? 17: ((a)<=262144 ? 18: ((a)<=524288) ? 19: 20))
#define DWC_USB3_LOG2P1(a) `DWC_USB3_LOG2(a+1)

#define DWC_USB3_ALIGN_MDW(a) ((((a) + 15) >> 4) << 4)
//#define DWC_USB3_ALIGN_MDW(a) ((((a) + (`DWC_USB3_MDWIDTH-8)/8) >> `DWC_USB3_MDWIDTH_BYTESHFT) << `DWC_USB3_MDWIDTH_BYTESHFT)

//------------------------------------------------------------------------------------------
// Derived Params
//------------------------------------------------------------------------------------------

#define DWC_USB3_RIQ_FIFO_DEPTH 34

#define DWC_USB3_HOST_RIQ_DEPTH_PER_SS_INSTANCE 64

#define DWC_USB3_HOST_RIQ_DEPTH_PER_HS_INSTANCE 22

#define DWC_USB3_HOST_RIQ_DEPTH_PER_FSLS_INSTANCE 34

#define DWC_USB3_DEV_RXMSG_FIFO_DEPTH_LOG2P1 3

#define DWC_USB3_HOST_RXMSG_DEPTH_PER_SS_INSTANCE_LOG2P1 5

#define DWC_USB3_HOST_RXMSG_DEPTH_PER_HS_INSTANCE_LOG2P1 4

#define DWC_USB3_HOST_RXMSG_DEPTH_PER_FSLS_INSTANCE_LOG2P1 2


#define DWC_USB3_HOST_RXMSG_FIFO_DEPTH 16


#define DWC_USB3_MAX_RXMSG_FIFO_DEPTH 16

#define DWC_USB3_MAX_RXMSG_FIFO_LOG2P1 6


#define DWC_USB3_MAX_PSQ_FIFO_DEPTH 32

#define DWC_USB3_PSQ_FIFO_LOG2 5

#define DWC_USB3_PSQ_FIFO_LOG2P1 6


//------------------------------------------------------------------------------------------
// RAM Allocation
//------------------------------------------------------------------------------------------

#define DWC_USB3_NUM_TXFIFO_HOST 3

#define DWC_USB3_NUM_TXFIFO_SDRD 4

#define DWC_USB3_NUM_TXFIFO 4

#define DWC_USB3_NUM_RXFIFO 3


#define DWC_USB3_NUM_TXFIFO_LOG2 2

#define DWC_USB3_NUM_RXFIFO_LOG2 2

#define DWC_USB3_NUM_TXFIFO_LOG2P1 3

#define DWC_USB3_NUM_RXFIFO_LOG2P1 2

#define DWC_USB3_RIQ_FIFO_LOG2 7

#define DWC_USB3_RIQ_FIFO_LOG2P1 8


#define DWC_USB3_TXQ_FIFO_DEVICE_DEPTH 64

#define DWC_USB3_TXQ_FIFO_HOST_DEPTH 185

#define DWC_USB3_TXQ_FIFO_DRD_DEPTH 201

#define DWC_USB3_TXQ_FIFO_ALL_DEPTH 201

#define DWC_USB3_RXQ_FIFO_ALL_DEPTH 98

#define DWC_USB3_RIQ_FIFO_ALL_DEPTH 120


#define DWC_USB3_TXQ_MAX_SEGMENTS 8


#define DWC_USB3_TXQ_FIFO_LOG2 9

#define DWC_USB3_TXQ_FIFO_LOG2P1 9

#define DWC_USB3_TXPFL_FIFO_LOG2 1

#define DWC_USB3_TXPFL_FIFO_LOG2P1 1

#define DWC_USB3_RXQ_FIFO_LOG2 7

#define DWC_USB3_RXQ_FIFO_LOG2P1 7


#define DWC_USB3_DFQ_FIFO_LOG2 4

#define DWC_USB3_DFQ_FIFO_LOG2P1 5

#define DWC_USB3_DWQ_FIFO_LOG2 5

#define DWC_USB3_DWQ_FIFO_LOG2P1 6


#define DWC_USB3_NPI_RX_STATUS_DEPTH_LOG2 4

#define DWC_USB3_NPI_RX_STATUS_DEPTH_LOG2P1 5

#define DWC_USB3_NPI_TX_STATUS_DEPTH_LOG2 3

#define DWC_USB3_NPI_TX_STATUS_DEPTH_LOG2P1 3


#define DWC_USB3_NPI_IN_EP0_FIFO_LOG2 7

#define DWC_USB3_NPI_IN_EP0_FIFO_LOG2P1 8

#define DWC_USB3_NPI_IN_EP1_FIFO_LOG2 10

#define DWC_USB3_NPI_IN_EP1_FIFO_LOG2P1 11

#define DWC_USB3_NPI_IN_EP2_FIFO_LOG2 10

#define DWC_USB3_NPI_IN_EP2_FIFO_LOG2P1 10

#define DWC_USB3_NPI_IN_EP3_FIFO_LOG2 8

#define DWC_USB3_NPI_IN_EP3_FIFO_LOG2P1 9

#define DWC_USB3_NPI_IN_EP4_FIFO_LOG2 1

#define DWC_USB3_NPI_IN_EP4_FIFO_LOG2P1 1

#define DWC_USB3_NPI_IN_EP5_FIFO_LOG2 1

#define DWC_USB3_NPI_IN_EP5_FIFO_LOG2P1 1

#define DWC_USB3_NPI_IN_EP6_FIFO_LOG2 1

#define DWC_USB3_NPI_IN_EP6_FIFO_LOG2P1 1

#define DWC_USB3_NPI_IN_EP7_FIFO_LOG2 1

#define DWC_USB3_NPI_IN_EP7_FIFO_LOG2P1 1


#define DWC_USB3_NPI_IN_EP8_FIFO_LOG2 1

#define DWC_USB3_NPI_IN_EP8_FIFO_LOG2P1 1

#define DWC_USB3_NPI_IN_EP9_FIFO_LOG2 1

#define DWC_USB3_NPI_IN_EP9_FIFO_LOG2P1 1

#define DWC_USB3_NPI_IN_EP10_FIFO_LOG2 1

#define DWC_USB3_NPI_IN_EP10_FIFO_LOG2P1 1

#define DWC_USB3_NPI_IN_EP11_FIFO_LOG2 1

#define DWC_USB3_NPI_IN_EP11_FIFO_LOG2P1 1

#define DWC_USB3_NPI_IN_EP12_FIFO_LOG2 1

#define DWC_USB3_NPI_IN_EP12_FIFO_LOG2P1 1

#define DWC_USB3_NPI_IN_EP13_FIFO_LOG2 1

#define DWC_USB3_NPI_IN_EP13_FIFO_LOG2P1 1

#define DWC_USB3_NPI_IN_EP14_FIFO_LOG2 1

#define DWC_USB3_NPI_IN_EP14_FIFO_LOG2P1 1

#define DWC_USB3_NPI_IN_EP15_FIFO_LOG2 1

#define DWC_USB3_NPI_IN_EP15_FIFO_LOG2P1 1

#define DWC_USB3_NPI_LARGEST_IN_EP_FIFO_LOG2 10

#define DWC_USB3_NPI_LARGEST_IN_EP_FIFO_LOG2P1 11

// MBYTES Depth of Queues defined in cC
// MBYTES Address (fixed) of Queues

#define DWC_USB3_ADDR_TXQ 0

#define DWC_USB3_ADDR_RXQ 201

#define DWC_USB3_ADDR_RIQ 299

#define DWC_USB3_ADDR_DFQ 419

#define DWC_USB3_ADDR_DWQ 435

#define DWC_USB3_ADDR_PSQ 467


#define DWC_USB3_CACHE_BCU_ENTRY_SIZE 0

// Byte Depth of CSR and Cache RAM Regions

#define DWC_USB3_CACHE_BCU_DEPTH 0

#define DWC_USB3_RCSR_GLOBAL_DEPTH 16

#define DWC_USB3_RCSR_DEVICE_DEPTH 128

#define DWC_USB3_RCSR_HOST_DEPTH 144

#define DWC_USB3_CACHE_EP_DEPTH 128

#define DWC_USB3_CACHE_DEVICE_DEPTH 16

#define DWC_USB3_CACHE_STREAMS_DEPTH 1280


#define DWC_USB3_HCACHE_SCRATCH_PAD_SIZE 64

#define DWC_USB3_HCACHE_EPINFO_BITMAP_SIZE_PER_SLOT 16

#define DWC_USB3_HCACHE_EPTYPE_BITMAP_SIZE_PER_SLOT 8

#define DWC_USB3_HCACHE_SS_EPHEADER_SIZE 96

#define DWC_USB3_HCACHE_HS_EPHEADER_SIZE 80

#define DWC_USB3_HCACHE_FSLS_EPHEADER_SIZE 80

#define DWC_USB3_HCACHE_TR_SIZE 8

#define DWC_USB3_HCACHE_PERIODIC_EPINFO_SIZE 20

#define DWC_USB3_HCACHE_PERIODIC_SLOTINFO_SIZE 4

#define DWC_USB3_HOST_NUM_CACHE_TRB_PER_SS_EP_INT 18

#define DWC_USB3_HOST_NUM_CACHE_TRB_PER_HS_EP_INT 10

#define DWC_USB3_HOST_NUM_CACHE_TRB_PER_FSLS_EP_INT 10

#define DWC_USB3_HCACHE_EPINFO_BITMAP_SIZE 272

#define DWC_USB3_HCACHE_EPTYPE_BITMAP_SIZE 136

#define DWC_USB3_HCACHE_PERIODIC_LIST_SIZE 640

#define DWC_USB3_HCACHE_SLOT_LIST_SIZE 68

#define DWC_USB3_HCACHE_FSLS_EP_SIZE 480

#define DWC_USB3_HCACHE_FSLS_TRQ_SIZE 16

#define DWC_USB3_HCACHE_FSLS_TMPTRB_SIZE 16

#define DWC_USB3_HCACHE_HS_EP_SIZE 480

#define DWC_USB3_HCACHE_HS_TRQ_SIZE 16

#define DWC_USB3_HCACHE_HS_TMPTRB_SIZE 16

#define DWC_USB3_HCACHE_SS_EP_SIZE 3072

#define DWC_USB3_HCACHE_SS_TRQ_SIZE 64

#define DWC_USB3_HCACHE_SS_TMPTRB_SIZE 16

//---------------------------
//Command Handler Cache sizes
//---------------------------

#define DWC_USB3_HCACHE_CMD_RING_POINTER_SIZE 8

#define DWC_USB3_HCACHE_CMD_STORE_SIZE 16

#define DWC_USB3_HCACHE_SLOT_TABLE_SIZE 4

#define DWC_USB3_HCACHE_DEVICE_CONTEXT_ADDR_SIZE 8

#define DWC_USB3_HCACHE_DEVICE_SLOT_CONTEXT_SIZE 32

#define DWC_USB3_HCACHE_DEVICE_EP_CONTEXT_SIZE 36

#define DWC_USB3_HCACHE_INPUT_CONTROL_CONTEXT_SIZE 8

#define DWC_USB3_HCACHE_INPUT_SLOT_CONTEXT_SIZE 20

#define DWC_USB3_HCACHE_INPUT_EP_CONTEXT_SIZE 20

#define DWC_USB3_HCACHE_DEVICE_ADDRESS_TABLE_SIZE 16

#define DWC_USB3_HCACHE_SLOT_ID_SIZE 128

#define DWC_USB3_HCACHE_SCRATCH_PAD_ADDR_SIZE 16

#define DWC_USB3_HCACHE_HUB_CONTEXT_ADDR_SIZE 8

#define DWC_USB3_HCACHE_HUB_SLOT_CONTEXT_SIZE 16

#define DWC_USB3_HCACHE_DEVICE_BASE_ADDR_SIZE 136

#define DWC_USB3_HCACHE_PRIMARY_STREAM_CONTEXT_ADDR_SIZE 16

#define DWC_USB3_HCACHE_SECONDARY_STREAM_CONTEXT_ADDR_SIZE 16

#define DWC_USB3_HCACHE_PORT_BANDWIDTH_CONTEXT_SIZE 4
//#define DWC_USB3_HCACHE_PORT_BANDWIDTH_CONTEXT_SIZE         16*3 //16bytes for each one of the speeds
//#define DWC_USB3_HCACHE_BI_BANDWIDTH_SIZE (16)*4  //4bytes for each one of the instances

#define DWC_USB3_HCACHE_BI_BANDWIDTH_SIZE 12

#define DWC_USB3_HCACHE_HCMD_TRB_SIZE 16
//End of Command Handler Cache Size definitions



#define DWC_USB3_HCACHE_HERH_DATA_SIZE 68

#define DWC_USB3_HCACHE_HALM_DATA_SIZE 320

// Byte Address (fixed) of CSRs and Cache

#define DWC_USB3_ADDR_CACHE_BCU 3992

#define DWC_USB3_ADDR_RCSR_GLOBAL 4000


#define DWC_USB3_ADDR_RCSR_DEVICE 4016

#define DWC_USB3_ADDR_CACHE_EP 4144

#define DWC_USB3_ADDR_CACHE_DEVICE 4272

#define DWC_USB3_ADDR_CACHE_STREAMS 4288

#define DWC_USB3_ADDR_CACHE_HOST 5568


#define DWC_USB3_ADDR_RCSR_HOST 4016

#define DWC_USB3_HCACHE_SCRATCH_PAD 4160

#define DWC_USB3_HCACHE_EPINFO_BITMAP 4224

#define DWC_USB3_HCACHE_EPTYPE_BITMAP 4496

#define DWC_USB3_HCACHE_PERIODIC_LIST 4632

#define DWC_USB3_HCACHE_SLOT_LIST 5272

#define DWC_USB3_HCACHE_FSLS_EP 5340

#define DWC_USB3_HCACHE_FSLS_TRQ 5820

#define DWC_USB3_HCACHE_FSLS_TMPTRB 5836

#define DWC_USB3_HCACHE_HS_EP 5852

#define DWC_USB3_HCACHE_HS_TRQ 6332

#define DWC_USB3_HCACHE_HS_TMPTRB 6348

#define DWC_USB3_HCACHE_SS_EP 6364

#define DWC_USB3_HCACHE_SS_TRQ 9436

#define DWC_USB3_HCACHE_SS_TMPTRB 9500

///////////////////////////////////////////////////////////////////////////////////
//                      Command Handler & HDNH Cache Pointers                    //
///////////////////////////////////////////////////////////////////////////////////

#define DWC_USB3_HCACHE_CMD_HANDLER 9520

#define DWC_USB3_HCACHE_CMD_RING_POINTER 9536

#define DWC_USB3_HCACHE_SLOT_TABLE 9552

#define DWC_USB3_HCACHE_DEVICE_CONTEXT_ADDR 9568

#define DWC_USB3_HCACHE_DEVICE_SLOT_CONTEXT 9584

#define DWC_USB3_HCACHE_DEVICE_EP_CONTEXT 9616

#define DWC_USB3_HCACHE_INPUT_SLOT_CONTEXT 9664

#define DWC_USB3_HCACHE_INPUT_EP_CONTEXT 9696

#define DWC_USB3_HCACHE_INPUT_CONTROL_CONTEXT 9728

#define DWC_USB3_HCACHE_DEVICE_ADDRESS_TABLE 9744

#define DWC_USB3_HCACHE_SCRATCH_PAD_ADDR 9760

#define DWC_USB3_HCACHE_HUB_CONTEXT_ADDR 9776

#define DWC_USB3_HCACHE_PRIMARY_STREAM_CONTEXT_ADDR 9792

#define DWC_USB3_HCACHE_SECONDARY_STREAM_CONTEXT_ADDR 9808

#define DWC_USB3_HCACHE_PORT_BANDWIDTH_CONTEXT 9824

#define DWC_USB3_HCACHE_BI_BANDWIDTH 9840

#define DWC_USB3_HCACHE_HCMD_TRB 9856

#define DWC_USB3_HCACHE_HUB_SLOT_CONTEXT 9872

#define DWC_USB3_HCACHE_DEVICE_BASE_ADDR 9888

#define DWC_USB3_HCACHE_SLOT_ID 10032


#define DWC_USB3_HCACHE_CMD_HANDLER_SIZE 640


#define DWC_USB3_HCACHE_HUB_DEVICE_ADDR 9776

/////////////////////// End of Command Handler Address Map /////////////////////////////////////////////////////////



#define DWC_USB3_HCACHE_HERH_DATA 10172
//#define DWC_USB3_HCACHE_HERH_DATA  `DWC_USB3_HCACHE_DEVICE_CONTEXT_ADDR


#define DWC_USB3_HCACHE_HALM_DATA 10240


#define DWC_USB3_HOST_CACHEEND_ADDR 10560


#define DWC_USB3_DCACHE_DEPTH_DEV 696

#define DWC_USB3_DCACHE_DEPTH_HST 1320

#define DWC_USB3_DCACHE_DEPTH_DRD 1320


#define DWC_USB3_DCACHE_DEPTH 1320


// Name:         DWC_USB3_DCACHE_DEPTH_INFO
// Default:      1320
// Values:       32, ..., 32768
// Enabled:      0
// 
// This informational (read-only) value defines the minimum RAM0 requirement 
//  for Cache, Register, and internal queues mapped into RAM0.  
//  When RAM0 is shared with the TXFIFOs and/or RXFIFOs, take the sum of this 
//  base value and the required TXFIFO and/or RXFIFO depths to find the total RAM0 depth. 
//  Choice decides the value of DWC_USB3_DCACHE_DEPTH_INFO in DWC_usb3_params.v file.
#define DWC_USB3_DCACHE_DEPTH_INFO 1320


#define DWC_USB3_DEV_RXFIFO_DEPTH_DEFAULT 389

#define DWC_USB3_DEV_TXFIFO0_DEPTH 66

#define DWC_USB3_DEV_TXFIFOX_DEPTH_DEFAULT 388


#define DWC_USB3_HOST_RXF_ALL_DEPTH 1160

#define DWC_USB3_RXF_ALL_DEPTH_DEFAULT 1160


#define DWC_USB3_DEV_TXF_ALL_DEPTH_DEFAULT 1230

#define DWC_USB3_HOST_TXF_ALL_DEPTH 1293

#define DWC_USB3_TXF_ALL_DEPTH_DEFAULT 1293


#define DWC_USB3_RAM0_DEPTH_DEFAULT 2480


#define DWC_USB3_NPI_TX_FIFO_DEPTH 2176
    

#define DWC_USB3_RAM1_DEPTH_DEFAULT 1293

#define DWC_USB3_RAM2_DEPTH_DEFAULT 1160


// Name:         DWC_USB3_RAM0_DEPTH
// Default:      2480
// Values:       32, ..., 32768
// 
// Specifies the depth of RAM0. In Device, Host, and DRD configuration RAM1  
//    contains: 
//  3 RAM Config: Contains Descriptor Cache. 
//  2 RAM Config: Contains Descriptor Cache and RxFIFOs. 
//  1 RAM Config: Contains Descriptor Cache, RxFIFOs, and TxFIFOs. 
//  In Hub configuration, RAM0 contains Hub descriptors and Upstream Tx Data  
//  Packets. 
//  For Device, Host, or DRD configuration, this should have minimum space for Cache  
//  (previous parameter default value) plus additional RxFIFO ram space in 2 RAM configuration  
//  plus TxFIFO ram space in 1-RAM configuration. At least three max packet size RxFIFO is recommended. 
//  In two RAM configuration, the RAM0 depth should be: 
//  Device: (DWC_USB3_DCACHE_DEPTH_INFO + 3 * 1024 + 40 to 64)/MDWidth-Bytes. 
//  The additional 40-64-bytes are used in Device configuration for holding up to 3 back to back SETUP packets and provide clock-crossing  
//  delay margin. When Master Bus width is 128-bits, 64 bytes is needed else 40 bytes is needed. 
//  If the system latency is more than 2.1uS, then a 5 to 6 packet FIFO is recommended instead 3 for a burstable SuperSpeed IN endpoint. 
//  
//  Host: (DWC_USB3_DCACHE_DEPTH_INFO + 3* 1024 (SS RxFIFO) + 2*1024 (HS RxFIFO) + 1024 (FS/LS RxFIFO))/MDWIDTH-Byte 
//  
//  For more information, see the databook's "Hardware Integration" chapter in the Databook. 
//  Choice decides the value of DWC_USB3_RAM0_DEPTH in DWC_usb3_params.v file.
#define DWC_USB3_RAM0_DEPTH 2608


// Name:         DWC_USB3_RAM1_DEPTH
// Default:      1293
// Values:       32, ..., 32768
// Enabled:      (DWC_USB3_NUM_RAMS > 1)
// 
// Specifies the depth of RAM1. In Device, Host, and DRD configuration RAM1  
//    contains: 
//  3 RAM Config: Contains TxFIFOs. 
//  2 RAM Config: Contains TxFIFOs. 
//  In Hub configuration RAM1 contains the Upstream Rx Data Packets. 
//  Device: 
//  In Device configuration 512 bytes for Control IN endpoint and 3 packet each of the other burstable IN endpoints is recommended. 
//  For example if you have 2 Bulk in endpoint in addition to 
//  Control endpoint, the RAM1 Depth requirement is: ((512+2*MDWidth-Bytes) + (2 * (3 * (1024+MDWidth-Bytes)) + MDWidth-Bytes))/MDWidth-Bytes + Tx-EP * MDWidth-Bytes.  
//  If the system latency is more than 2.1uS, then a 5 to 6 packet FIFO is recommended instead 3 for a burstable SuperSpeed IN endpoint. 
//  
//  Host: ((3 * (1024 + MDWIDTH-Bytes) + MDWIDTH-Bytes) (SS Tx FIFOs) + (2 * (1024 + MDWIDTH-Bytes) + MDWIDTH-Bytes) (HS Tx FIFOs) + (1 * (1024 + MDWIDTH-Bytes) + MDWIDTH-Bytes) (FS/LS Tx FIFOs))/MDWIDTH-Bytes 
//  
//  In addition to data, each Tx-FIFO also stores additional information and hence MDWidth-Bytes needed for each packet plus MDWidth-Bytes for each FIFO. 
//  For more information, see the databook's "Hardware Integration" chapter in the Databook. 
//  Choice decides the value of DWC_USB3_RAM1_DEPTH in DWC_usb3_params.v file.
#define DWC_USB3_RAM1_DEPTH 1293



// Name:         DWC_USB3_RAM2_DEPTH
// Default:      1160
// Values:       32, ..., 32768
// Enabled:      (DWC_USB3_NUM_RAMS > 2 && DWC_USB3_MODE !=3)
// 
// Specifies the depth of RAM2. 
//  3 RAM config: Contains RxFIFOs. 
//  The RAM2 depth should be: 
//  Device: (DWC_USB3_DCACHE_DEPTH_INFO + 3 * 1024 + 40 to 64)/MDWidth-Bytes. 
//  The additional 40-64-bytes are used in Device configuration for holding up to 3 back to back SETUP packets and provide clock-crossing  
//  delay margin. When Master Bus width is 128-bits, 64 bytes is needed else 40 bytes is needed. 
//  If the system latency is more thn 2.1uS, then a 5 to 6 packet FIFO is recommended instead 3. 
//  
//  Host: (DWC_USB3_DCACHE_DEPTH_INFO + 3* 1024 (SS RxFIFO) + 2*1024 (HS RxFIFO) + 1024 (FS/LS RxFIFO))/MDWIDTH-Byte 
//  For more information, see the databook's "Hardware Integration" chapter in the 
//  Databook. 
//  Choice decides the value of DWC_USB3_RAM2_DEPTH in DWC_usb3_params.v file.
#define DWC_USB3_RAM2_DEPTH 1160



#define DWC_USB3_ADDR_RXF 1320


#define DWC_USB3_DEV_RXFIFO_DEPTH 1288


#define DWC_USB3_ADDR_TXF 0

#define DWC_USB3_ADDR_HOST_TXF 0


#define DWC_USB3_DEV_TXFIFOX_DEPTH 409

#define DWC_USB3_DEV_TXF_ALL_DEPTH 1293


#define DWC_USB3_RXF_ALL_DEPTH 1288

#define DWC_USB3_TXF_ALL_DEPTH 1293

/*  function [31:0] log2(input [31:0] val);
    begin
      for (log2=0; val>0; log2=log2+1)
        val = val >> 1;
    end
  endfunction
*/


#define DWC_USB3_DCARAM_DEPTH 2608

#define DWC_USB3_RXFRAM_DEPTH 2608

#define DWC_USB3_TXFRAM_DEPTH 1293

#define DWC_USB3_DCARAM_A 12

#define DWC_USB3_RXFRAM_A 12

#define DWC_USB3_TXFRAM_A 11

#define DWC_USB3_DCARAM_A_LOG2P1 12

#define DWC_USB3_RXFRAM_A_LOG2P1 12

#define DWC_USB3_TXFRAM_A_LOG2P1 11


#define DWC_USB3_RAM0_A 12

#define DWC_USB3_RAM0_A_LOG2P1 12

#define DWC_USB3_RAM1_A 11

#define DWC_USB3_RAM1_A_LOG2P1 11

#define DWC_USB3_RAM2_A 11

#define DWC_USB3_RAM2_A_LOG2P1 11


#define DWC_USB3_ROM_A 6

#define DWC_UPTXDATA_FIFO_SIZE 2544

#define DWC_USB3_UPTXDATA_FIFO_A 12

#define DWC_USB3_UPTXDATA_FIFO_A_LOG2P1 12

#define DWC_USB3_HUB_NUM_TXHEADERS_LOG2P1 3

#define DWC_USB3_HUB_NUM_RXHEADERS_A 2

#define DWC_USB3_HUB_NUM_RXHEADERS_LOG2P1 3

#define DWC_USB3_NUM_U3_ROOT_PORTS_A 1


// Name:         DWC_USB3_EN_DBG_PORTS
// Default:      No
// Values:       No (0), Yes (1)
// Enabled:      DWC_USB3_MODE !=3
// 
// This parameter is used at Synopsys for FPGA hardware 
//  validation of the core. You can use these ports during 
//  driver development with an FPGA platform. 
//  When this parameter is 1, internal signals mictor1[31:0], 
//  mictor2[31:0], mictor3[31:0], and mictor_sel[3:0] are 
//  brought out of the core. Three 32-bit mictor connectors 
//  on the FPGA board can be hooked to logic analyzer PODs. 
//  The FPGA board also has a DIP switch, which is used to 
//  dynamically multiplex out different internal debug signals. 
//  Choice decides the value of DWC_USB3_EN_DBG_PORTS in DWC_usb3_params.v file.
#define DWC_USB3_EN_DBG_PORTS 0


// Name:         DWC_USB3_EN_FPGA
// Default:      No
// Values:       No (0), Yes (1)
// Enabled:      DWC_USB3_MODE !=3
// 
// This parameter is used at Synopsys for FPGA Device and Host hardware 
//  validation of the core. You can use this parameter during 
//  driver development with an FPGA platform. 
//  Set this parameter to 1 to improve FPGA timing by 
//    - adding a pipeline on RX data path in the PTL 
//    - adding a pipeline on cache/DMA requst in LSP BARB 
//    - adding a pipeline on address generation in LSP HCMD    
//    - adding a pipeline in the MAC 
//  Choice decides the value of DWC_USB3_EN_FPGA in DWC_usb3_params.v file.
#define DWC_USB3_EN_FPGA 0


// Name:         DWC_USB3_EN_LOG_PHYS_EP_SUPT
// Default:      Yes
// Values:       No (0), Yes (1)
// Enabled:      DWC_USB3_MODE != 1 && DWC_USB3_MODE !=3 && DWC_USB3_MBUS_TYPE!=4
// 
// This parameter is used at Synopsys for FPGA Device hardware 
//  validation of the core. You can use this parameter during 
//  driver development with an FPGA platform. 
//  Set this parameter to 0 to improve FPGA timing. 
//  Selects whether the device core supports a flexible or fixed logical->physical endpoint mapping 
//  A fixed mapping improves timing and may reduce gate count (5K gate reduction) 
//  Choice decides the value of DWC_USB3_EN_LOG_PHYS_EP_SUPT in DWC_usb3_params.v file.
#define DWC_USB3_EN_LOG_PHYS_EP_SUPT 1


// Name:         DWC_USB3_ATSPEED_DFT
// Default:      No
// Values:       No (0), Yes (1)
// Enabled:      DWC_USB3_MODE !=3 && DWC_USB3_MBUS_TYPE!=4
// 
// This parameter enables additional at speed DFT Control signals. This creates additional dft_* ports to control clocks. 
//  By default, even when this parameter is not enabled, at speed DFT is enabled and "scan_mode" port controls scan by-pass.  
//  This parameter controls only whether additional inputs are used to control clock muxes, clock-gate cell, and reset bypass. 
//  Choice decides the value of DWC_USB3_ATSPEED_DFT in DWC_usb3_params.v file.
#define DWC_USB3_ATSPEED_DFT 0


// Name:         DWC_USB3_PIPE_32BIT_ONLY
// Default:      No
// Values:       No (0), Yes (1)
// Enabled:      DWC_USB3_MBUS_TYPE!=4
// 
// If your PIPE interface is always 32-bits @125MHz, then mac3_clk will be set to pipe3_rx_pclk.  
//  This avoids the clock dividers and muliplexers on the mac3_clk and simplifies the DFT and clock insertion. 
//  If you enable this, please make sure the DWC_USB3_GUSB3PIPECTL_INIT parameter bit[16:15] are 2'b00 to match this. 
//  Choice decides the value of DWC_USB3_PIPE_32BIT_ONLY in DWC_usb3_params.v file.
#define DWC_USB3_PIPE_32BIT_ONLY 0


// Name:         DWC_USB3_RAM_CLK_TO_BUS_CLK
// Default:      No
// Values:       No (0), Yes (1)
// Enabled:      DWC_USB3_MODE !=3 && DWC_USB3_MBUS_TYPE!=4
// 
// This parameter enables if you plan to always connect bus_clk_early to ram_clk. 
//  The bus_clk_early SHOULD BE GREATER THAN OR EQUAL to 125MHz during SuperSpeed operation and 
//  greater than or equal to 60MHz during USB2.0 operation. If the above conditions are not met then 
//  it would cause functional FAILURE. This parameter avoids the clock dividers and muliplexers on the ram_clk  
//  and simplifies the DFT and clock insertion. 
//  Choice decides the value of DWC_USB3_RAM_CLK_TO_BUS_CLK in DWC_usb3_params.v file.
#define DWC_USB3_RAM_CLK_TO_BUS_CLK 0

//------------------------------------------------------------
// Gate count reduction parameters  
//------------------------------------------------------------


// Name:         DWC_USB3_EN_STREAM_SUPT
// Default:      1
// Values:       -2147483648, ..., 2147483647
// 
// Selects whether the device core supports stream-capable bulk endpoints (12K gate reduction when coupled with 
//       disabling isoc support)
#define DWC_USB3_EN_STREAM_SUPT 1


// Name:         DWC_USB3_EN_INTERRUPT_SUPT
// Default:      1
// Values:       -2147483648, ..., 2147483647
// 
// Selects whether the device core supports interrupt endpoints (7K gate reduction when coupled with disabling isoc 
//       support)
#define DWC_USB3_EN_INTERRUPT_SUPT 1


// Name:         DWC_USB3_EN_LSP_DEBUG
// Default:      1
// Values:       -2147483648, ..., 2147483647
// 
// Selects whether debug registers for the List Processor (LSP) are 
// enabled (1K gate reduction)
#define DWC_USB3_EN_LSP_DEBUG 1


// Name:         DWC_USB3_OUTEP_BYTECNT_W
// Default:      29
// Values:       -2147483648, ..., 2147483647
// 
// The number of bits used to store the TRB bytecount for OUT endpoints.  Default is 24 (width of BUFSIZ field in a 
//       TRB) + 5 (32 is the maximum number of TRBs per transfer resource) = 29 bits (1K gate reduction when reduced to 17)
#define DWC_USB3_OUTEP_BYTECNT_W 29

/************* Internal Parameters  ************/
// DWC_USB3_SYNC_VERIF_EN=1 enable delay randomization
// DWC_USB3_SYNC_FIFO_DSYNC_EDGE/DWC_USB3_SYNC_DSYNC_EDGE=3 - all posedge, 4 - 1st stage negedge  (Dsync Flops Only)
// DWC_USB3_SYNC_TOGGLE_EDGE=1  - all posedge, 2 - 1st stage negedge  (Toggle Synchronizer Only)
// DWC_USB3_SYNC_RAND_TYPE=1 - 0 to 1.9 clk delay randomization; 1 - 0 - 0.9 clk delay randomization
// Synchronizer Randomization

#define DWC_USB3_SYNC_VERIF_EN 0

#define DWC_USB3_SYNC_FIFO_DSYNC_EDGE 4

#define DWC_USB3_SYNC_DSYNC_EDGE 4

#define DWC_USB3_SYNC_DSYNC_PEDGE 3

#define DWC_USB3_SYNC_TOGGLE_EDGE 2

#define DWC_USB3_SYNC_RAND_TYPE 1


#define DWC_USB3_BYPASS_SF_INIT 0


#define DWC_USB3_VERSION 50

// BMU Clock Frequency in MHz; Not used in RTL - Only For Simulation

#define DWC_USB3_BUS_CLK_FREQ 250

#define DWC_USB3_BUS_CLK_PERIOD 4000


#define DWC_USB3_MAC_CLK_FREQ 125

#define DWC_USB3_MAC_CLK_PERIOD 8000


#define DWC_USB3_MAC3_CLK_FREQ 125

#define DWC_USB3_MAC3_CLK_PERIOD 8000


#define DWC_USB3_MAC2_CLK_FREQ 60

#define DWC_USB3_MAC2_CLK_PERIOD 16666.6
#define DWC_USB3_MAC2_CLK_PERIOD_INT 16666   // 1000 * TIMESCALE /DWC_USB3_BUS_MAC2_FREQ


#define DWC_USB3_BUS_CLK_FREQ_HOST_DUPLEX_MIN_REAL 86


#define DWC_USB3_BUS_CLK_FREQ_DEV_DUPLEX_MIN_REAL 78


#define DWC_USB3_BUS_CLK_FREQ_DUPLEX_MIN_REAL 86


#define DWC_USB3_BUS_CLK_FREQ_DUPLEX_MIN 86
 // ((`DWC_USB3_BUS_CLK_FREQ_DUPLEX_MIN_REAL + 0.99) - ((`DWC_USB3_BUS_CLK_FREQ_DUPLEX_MIN_REAL + 0.99) % 1))


#define DWC_USB3_BUS_CLK_FREQ_MIN 86


#define DWC_USB3_RAM_CLK_FREQ_MIN 125


#define DWC_USB3_RAM_CLK_FREQ 250

#define DWC_USB3_RAM_CLK_PERIOD 4000


#define DWC_USB3_HOST_CLK_FREQ 125


#define DWC_USB3_PIPE_CLK_PERIOD 2000


#define DWC_USB3_ADP_CLK_FREQ 4.125

#define DWC_USB3_ADP_CLK_PERIOD 66


#define DWC_USB3_GTECH_default_delay 0

#define GTECH_default_delay 0


#define DWC_USB3_BUS_MDWIDTH 64

#define DWC_USB3_BUS_SDWIDTH 32


#define DWC_USB3_EN_DESC_DMA 0

// euse-pragma attr Visible false
//#define DWC_USB3_DESC_DMA_EN 0


#define DWC_USB3_INEPS 16


#define DWC_USB3_OUTEPS 16


#define DWC_USB3_H 1'h1


#define DWC_USB3_L 1'h0

#define DWC_USB3_HOST_CAP_REG_LEN 'h20



#define DWC_USB3_HC_RUNTIME_REGISTER_SPACE_OFFSET 32'h440

#define DWC_USB3_HC_DOORBELL_ARRAY_OFFSET 1152


#define DWC_USB3_HC_EXTCAP_USBLEGSUP_OFFSET 2176

#define DWC_USB3_HC_EXTCAP_SUPT_PRTL_20_OFFSET 2192

#define DWC_USB3_HC_EXTCAP_SUPT_PRTL_30_OFFSET 2208

#define DWC_USB3_HC_EXTCAP_DBC_OFFSET 2224


#define DWC_USB3_HC_CAPLENGTH 32'h20

#define DWC_USB3_HC_HCIVERSION 32'h96

#define DWC_USB3_HC_MAXSLOTS 16

#define DWC_USB3_HC_CC_MAXSLOTS 16


#define DWC_USB3_HC_MAXINTRS 1

#define DWC_USB3_HC_CC_MAXINTRS 1


#define DWC_USB3_HOST_NUM_ROOT_PORTS 2


#define DWC_USB3_HOST_NUM_U2_ROOT_PORTS_PIN 1

#define DWC_USB3_HOST_NUM_U3_ROOT_PORTS_PIN 1

#define DWC_USB3_HOST_NUM_ROOT_PORTS_PIN 2


#define DWC_USB3_HC_MAXPORTS 2


//#define DWC_USB3_HC_CC_MAXPORTS `DWC_USB3_HC_MAXPORTS

#define DWC_USB3_HC_CC_MAXPORTS 2


#define DWC_USB3_HC_IST 32'h1

#define DWC_USB3_HC_ERSTMAX 15

#define DWC_USB3_HC_IOCINTERVAL 0

#define DWC_USB3_HC_SPR 1

#define DWC_USB3_HC_MAXSCRATCHPADBUFS 1

#define DWC_USB3_HC_U1_DEVICE_EXIT_LATENCY 32'ha

#define DWC_USB3_HC_U2_DEVICE_EXIT_LATENCY 32'h7ff

#define DWC_USB3_HC_AC64 0

#define DWC_USB3_HC_BNC 0

#define DWC_USB3_HC_CSZ 1

#define DWC_USB3_HC_PPC 1

#define DWC_USB3_HC_PIND 0

#define DWC_USB3_HC_LHRC 0

#define DWC_USB3_HC_LTC 1

#define DWC_USB3_HC_NSS 0

#define DWC_USB3_HC_FSE 0

#define DWC_USB3_HC_SBD 0

#define DWC_USB3_HC_MAXPSASIZE 15

#define DWC_USB3_HC_XECP 544

#define DWC_USB3_HC_PAGE_SIZE 1

#define DWC_USB3_HC_DBOFF 1152

#define DWC_USB3_HC_RTSOFF 1088


#define DWC_USB3_NO_P3_INIT 1


#define DWC_USB3_NO_P3 1







#define DWC_USB3_CC_HS_PHYSEL 0


#define DWC_USB3_CSR_ACCESS_TIMEOUT 17'h1ffff


// Name:         DWC_USB3_EN_ADP
// Default:      External ADP
// Values:       External ADP (0), Internal ADP (1)
// Enabled:      DWC_USB3_EN_OTG ==1
// 
// Enables Internal ADP capability of the USB3 core.  
//  Choice decides the value of DWC_USB3_EN_ADP in DWC_usb3_params.v file.
#define DWC_USB3_EN_ADP 0


// Name:         DWC_USB3_EN_BC
// Default:      No
// Values:       No (0), Yes (1)
// Enabled:      DWC_USB3_MODE !=3 && DWC_USB3_MBUS_TYPE != 4
// 
// Enables Battery Charging (ACA) capability.  
//  Choice decides the value of DWC_USB3_EN_BC in DWC_usb3_params.v file.
#define DWC_USB3_EN_BC 0


// Name:         DWC_USB3_EN_BUS_FILTERS
// Default:      No
// Values:       No (0), Yes (1)
// Enabled:      DWC_USB3_MODE !=3
// 
// Enables Bus Filters for on the following OTG control inputs from the PHY 
//  1. utmiotg_iddig 
//  2. utmiotg_avalid 
//  3. utmiotg_vbusvalid 
//  4. utmisrp_bvalid 
//  5. utmisrp_sessend 
//  Choice decides the value of DWC_USB3_EN_BUS_FILTERS in DWC_usb3_params.v file.
#define DWC_USB3_EN_BUS_FILTERS 1

//-----------------------------------------------------------------------------------------------
// Mappings for the top-level "debug" port
//-----------------------------------------------------------------------------------------------

#define DWC_USB3_DEBUGIO_LINK_SUB_STATE 0

#define DWC_USB3_DEBUGIO_LINK_SUB_STATE_W 4


#define DWC_USB3_DEBUGIO_LINK_STATE 4

#define DWC_USB3_DEBUGIO_LINK_STATE_W 4


#define DWC_USB3_DEBUGIO_SOF_NUM 8

#define DWC_USB3_DEBUGIO_SOF_NUM_W 19


#define DWC_USB3_DEBUGIO_DEV_BUS_ERR 27

#define DWC_USB3_DEBUGIO_DEV_BUS_ERR_W 1

// 8+ 6*U2 ports

#define DWC_USB3_DEBUGIO_U2_STATES_W 14


#define DWC_USB3_NUM_U2TIS 2


#define DWC_USB3_DEBUG_WIDTH 55

