// Define the Base Address of the SATA Controller's Configuration Space (from Memory Map)

typedef unsigned int U32;

#define SATA_CFG_BASE	COMCERTO_AXI_SATA_CFG_BASE

#define CAP		0x9D000000
#define GHC		0x9D000004
#define IS		0x9D000008
#define PI		0x9D00000C
#define VS		0x9D000010
#define CCC_CTL		0x9D000014
#define CCC_PORT	0x9D000018
#define CAP2		0x9D000024
#define BISTAFR		0x9D0000A0
#define BISTCR		0x9D0000A4
#define BISTFCTR	0x9D0000A8
#define BISTSR		0x9D0000AC
#define BISTDECR	0x9D0000B0
#define OOBR		0x9D0000BC
#define GPCR		0x9D0000D0
#define GPSR		0x9D0000D4
#define TIMER1MS	0x9D0000E0
#define GPARAM1R	0x9D0000E8
#define GPARAM2R	0x9D0000EC
#define PPARAMR		0x9D0000F0
#define TESTR		0x9D0000F4
#define VERSIONR	0x9D0000F8
#define IDR		0x9D0000FC

#define P0CLB		0x9D000100
#define P0CLBU		0x9D000104
#define P0FB		0x9D000108
#define P0FBU		0x9D00010C
#define P0IS		0x9D000110
#define P0IE		0x9D000114
#define P0CMD		0x9D000118
#define P0TFD		0x9D000120
#define P0SIG		0x9D000124
#define P0SSTS		0x9D000128
#define P0SCTL		0x9D00012C
#define P0SERR		0x9D000130
#define P0SACT		0x9D000134
#define P0CI		0x9D000138
#define P0SNTF		0x9D00013C
#define P0DMACR		0x9D000170
#define P0PHYCR		0x9D000178
#define P0PHYSR		0x9D00017C

#define P1CLB		0x9D000180
#define P1CLBU		0x9D000184
#define P1FB		0x9D000188
#define P1FBU		0x9D00018C
#define P1IS		0x9D000190
#define P1IE		0x9D000194
#define P1CMD		0x9D000198
#define P1TFD		0x9D0001A0
#define P1SIG		0x9D0001A4
#define P1SSTS		0x9D0001A8
#define P1SCTL		0x9D0001AC
#define P1SERR		0x9D0001B0
#define P1SACT		0x9D0001B4
#define P1CI		0x9D0001B8
#define P1SNTF		0x9D0001BC
#define P1DMACR		0x9D0001F0
#define P1PHYCR		0x9D0001F8
#define P1PHYSR		0x9D0001FC

#define P0_CMD_BASE	0x83010000
#define P0_CMD_TABLE	0x83011000
#define P0_CMD_TABLE1	0x83011100
#define P0_CMD_TABLE2	0x83011200
#define P0_CMD_TABLE3	0x83011300
#define P0_PRDT		0x83012000
#define P0_DAT_BUFF	0x83015000
#define P0_DAT_BUFF1	0x83016000
#define P0_DAT_BUFF2	0x83017000
#define P0_DAT_BUFF3	0x83018000
#define P0_FIS_BASE	0x8301A000
#define P0_DAT_BUFF4	0x8301C000
#define P0_DAT_BUFF5	0x8301D000
#define P0_DAT_BUFF6	0x8301E000
#define P0_DAT_BUFF7	0x8301F000

#define P1_CMD_BASE	0x83030000
#define P1_CMD_TABLE	0x83031000
#define P1_CMD_TABLE1	0x83031100
#define P1_CMD_TABLE2	0x83031200
#define P1_CMD_TABLE3	0x83031300
#define P1_PRDT		0x83032000
#define P1_DAT_BUFF	0x83035000
#define P1_DAT_BUFF1	0x83036000
#define P1_DAT_BUFF2	0x83037000
#define P1_DAT_BUFF3	0x83038000
#define P1_FIS_BASE	0x8303A000
#define P1_DAT_BUFF4	0x8303C000
#define P1_DAT_BUFF5	0x8303D000
#define P1_DAT_BUFF6	0x8303E000
#define P1_DAT_BUFF7	0x8303F000

// Method to wait for the detection of the COMINIT from the connected device for the specified SATA Port (detected by the setting of bit-26[DIAG_X] in the Port Error Register)
// Input Parameters: port_num - Port Number of the SATA Controller on which we want to wait for the COMINIT
//                   clr_intrpt - Specifies to clear the COMINIT detected/DIAG_X bit in P0SERR
//                   debug - Enable Debug Messaging
void wait_dev_cominit_dtctd(int port_num, bool clr_intrpt, bool debug)
{
 U32 rd_data;
 U32 devcominit_dtctd_mask = 0x04000000;
 U32 masked_data;
 int P0SERR_Offset = 0x130;
 int P1SERR_Offset = 0x1B0;
 bool entry = true;

 if((port_num > 1) || (port_num < 0)) // Invalid parameter check
 {
  printf("Error: wait_dev_cominit_dtctd - Invalid SATA Port number:%d was specified!\n", port_num);
  return;
 }

 do // Keep looping until you see the SATA Controller detect a COMINIT from Device
 {
  if(port_num == 0)
  {
   rd_data = readl(SATA_CFG_BASE + P0SERR_Offset);
   if(debug)
    printf("Read data 0x%x from P0.SERR for SATA Controller.\n", rd_data);
  }
  else
  {
   rd_data = readl(SATA_CFG_BASE + P1SERR_Offset);
   if(debug)
    printf("Read data 0x%x from P1.SERR for SATA Controller.\n", rd_data);
  }
  entry = false; // The first entry into the loop has been completed
  masked_data = rd_data & devcominit_dtctd_mask; // Bit-wise AND the read-data with the Mask so that we are only left with the bit-position corresponding to Device COMINIT Detect
  if(port_num == 0)
  {
   if((masked_data == devcominit_dtctd_mask) && debug)
    printf("Detected a COMINIT from the connected Device on Port#0!\n");
  }
  else
  {
   if((masked_data == devcominit_dtctd_mask) && debug)
    printf("Detected a COMINIT from the connected Device on Port#1!\n");
  }
 }
 while(masked_data != devcominit_dtctd_mask);
 // Now that the interrupt has been seen (bit has been set), check if we need to clear the detection bit/interrupt
 if(clr_intrpt)
 {
  if(port_num == 0)
  {
   writel(devcominit_dtctd_mask, (SATA_CFG_BASE + P0SERR_Offset));
   if(debug)
    printf("The Device COMINIT detection bit on Port#0 is Cleared!\n");
  }
  else
  {
   writel(devcominit_dtctd_mask, (SATA_CFG_BASE + P1SERR_Offset));
   if(debug)
    printf("The Device COMINIT detection bit on Port#1 is Cleared!\n");
  }
 }
 return;
}

// Method to wait for the detection of the completion of the Speed negotiation with the PHY, and for the PHY interface to be clear and readyCOMWAKE from the connected device (detected by the setting of bit-18[DIAG_W] in the Port#0 SATA Error Register)
// Input Parameters: port_num - Port Number of the SATA Controller on which we want to wait for the COMWAKE
//                   clr_intrpt - Specifies to clear the COMWAKE detected/DIAG_W bit in P0SERR
//                   debug - Enable Debug Messaging
void wait_dev_comwake_dtctd(int port_num, bool clr_intrpt, bool debug)
{
 U32 rd_data;
 U32 devcomwake_dtctd_mask = 0x00040000;
 U32 masked_data;
 int P0SERR_Offset = 0x130;
 int P1SERR_Offset = 0x1B0;
 bool entry = true;

 if((port_num > 1) || (port_num < 0)) // Invalid parameter check
 {
  printf("Error: wait_dev_comwake_dtctd - Invalid SATA Port number:%d was specified!\n", port_num);
  return;
 }

 do // Keep looping until you see the SATA Controller detect a COMWAKE from Device
 {
  if(port_num == 0)
  {
   rd_data = readl(SATA_CFG_BASE + P0SERR_Offset);
   if(debug)
    printf("Read data 0x%x from P0.SERR for SATA Controller.\n", rd_data);
  }
  else
  {
   rd_data = readl(SATA_CFG_BASE + P1SERR_Offset);
   if(debug)
    printf("Read data 0x%x from P1.SERR for SATA Controller.\n", rd_data);
  }
  entry = false; // The first entry into the loop has been completed
  masked_data = rd_data & devcomwake_dtctd_mask; // Bit-wise AND the read-data with the Mask so that we are only left with the bit-position corresponding to Device COMWAKE Detect
  if(port_num == 0)
  {
   if((masked_data == devcomwake_dtctd_mask) && debug)
    printf("Detected a COMWAKE from the connected Device on Port#0!\n");
  }
  else
  {
   if((masked_data == devcomwake_dtctd_mask) && debug)
    printf("Detected a COMWAKE from the connected Device on Port#1!\n");
  }
 }
 while(masked_data != devcomwake_dtctd_mask);
 // Now that the interrupt has been seen (bit has been set), check if we need to clear the detection bit/interrupt
 if(clr_intrpt)
 {
  if(port_num == 0)
  {
   writel(devcomwake_dtctd_mask, (SATA_CFG_BASE + P0SERR_Offset));
   if(debug)
    printf("The Device COMWAKE detection bit on Port#0 is Cleared!\n");
  }
  else
  {
   writel(devcomwake_dtctd_mask, (SATA_CFG_BASE + P1SERR_Offset));
   if(debug)
    printf("The Device COMWAKE detection bit on Port#1 is Cleared!\n");
  }
 }
 return;
}

// Method to wait for the completion of Speed negotiation with the Device & detection of the PHY Interface being ready/active & clear for transactions
// Input Parameters: port_num - Port Number of the SATA Controller on which we want to wait for the Device speed negotiation completion & PHY interface Ready/Clear
//                   gen1_det - Specifies that the Speed Negotiation detection for the SATA Controller is for a Gen-1 Link
//                   debug - Enable Debug Messaging
//                   clr_intrpts - Specifies to clear the associated bits corresponding to Device speed negotation completion & PHY Interface ready & clear detection {Removed}
void wait_phyif_rdyclr_dtctd(int port_num, bool gen1_det, bool debug)
{
 U32 rd_data1, rd_data2;
 U32 speedneg_ifactive_dtctd_mask;
 U32 phyif_clear_dtctd_mask = 0x00000100;
 U32 masked_data1, masked_data2;
 int P0STS_Offset = 0x128;
 int P1STS_Offset = 0x1A8;
 int P0TFD_Offset = 0x120;
 int P1TFD_Offset = 0x1A0;
 bool entry = true;

 if((port_num > 1) || (port_num < 0)) // Invalid parameter check
 {
  printf("Error: wait_phyif_rdyclr_dtctd - Invalid SATA Port number:%d was specified!\n", port_num);
  return;
 }

 if(gen1_det)
  speedneg_ifactive_dtctd_mask = 0x00000113;
 else
  speedneg_ifactive_dtctd_mask = 0x00000123;

 do // Keep looping until you see the SATA Controller complete the speed negotation with the Device, and get the PHY Interface ready/active & clear for operation
 {
  if(port_num == 0)
  {
   rd_data1 = readl(SATA_CFG_BASE + P0STS_Offset);
   if(debug)
    printf("Read data 0x%x from P0.STS for SATA Controller.\n", rd_data1);
   rd_data2 = readl(SATA_CFG_BASE + P0TFD_Offset);
   if(debug)
    printf("Read data 0x%x from P0.TFD for SATA Controller.\n", rd_data2);
  }
  else
  {
   rd_data1 = readl(SATA_CFG_BASE + P1STS_Offset);
   if(debug)
    printf("Read data 0x%x from P1.STS for SATA Controller.\n", rd_data1);
   rd_data2 = readl(SATA_CFG_BASE + P1TFD_Offset);
   if(debug)
    printf("Read data 0x%x from P1.TFD for SATA Controller.\n", rd_data2);
  }
  entry = false; // The first entry into the loop has been completed
  masked_data1 = rd_data1 & speedneg_ifactive_dtctd_mask; // Bit-wise AND the read-data with the Mask so that we are only left with the bit-positions corresponding to 'PHY Ready' detection, Speed-Negotiation completion & Interface active detection
  masked_data2 = rd_data2 & phyif_clear_dtctd_mask; // Bit-wise AND the read-data with the Mask so that we are only left with the bit-positions corresponding to Task-File Error register LSB & Interface Clear (no longer busy)
  if(port_num == 0)
  {
   if((masked_data1 == speedneg_ifactive_dtctd_mask) && (masked_data2 == phyif_clear_dtctd_mask) && debug)
   {
    if(gen1_det)
     printf("Detected Gen-I Speed Negotiation completion, & PHY Interface on Port#0 is Ready & Clear!\n");
    else
     printf("Detected Gen-II Speed Negotiation completion, & PHY Interface on Port#0 is Ready & Clear!\n");
   }
  }
  else
  {
   if((masked_data1 == speedneg_ifactive_dtctd_mask) && (masked_data2 == phyif_clear_dtctd_mask) && debug)
   {
    if(gen1_det)
     printf("Detected Gen-I Speed Negotiation completion, & PHY Interface on Port#1 is Ready & Clear!\n");
    else
     printf("Detected Gen-II Speed Negotiation completion, & PHY Interface on Port#1 is Ready & Clear!\n");
   }
  }
 }
 while((masked_data1 != speedneg_ifactive_dtctd_mask) || (masked_data2 != phyif_clear_dtctd_mask));
 return;
}

// Method to wait for the SATA Controller to detect Out Of Band(OOB) handshake signaling detection from the connected Device on the specified SATA Port
// Input Parameter: both_ports - Specifies that both of the SATA Ports are to be checked for Device OOB Detection
//                  port_num - Port Number of the SATA Controller on which we want to wait for the Device OOB Signaling (only when both_ports = false)
//                  port0_gen1 - Specifies that we want to perform the SATA Port#0 Speed Negotiation for Gen-1 operation
//                  port1_gen1 - Specifies that we want to perform the SATA Port#1 Speed Negotiation for Gen-1 operation
//                  debug - Enables the printing of debug messages
void sata_oob_detect(bool both_ports, int port_num, bool port0_gen1, bool port1_gen1, bool debug)
{
 U32 offset = 0x04; // Offset of Global HBA Register from Configuration Base-Address
 U32 rd_data;

 if(both_ports)
  port_num = 0; // We need to do the COMINIT, etc. checks for Port#0 first

 if((port_num > 1) || (port_num < 0)) // Invalid parameter check
 {
  printf("Error: sata_oob_detect - Invalid SATA Port number:%d was specified!\n", port_num);
  return;
 }

 // Performing HBA Reset using the Global HBA Control Register for SATA Controller
 rd_data = readl(0x9D000004);
 if(debug)
  printf("GHC = 0x%x\n", rd_data);

 if(port_num == 0) // For SATA Port#0
 {
  // Reading the Port#0 Command Register(P0CMD): (Reset Value)
  rd_data = readl(0x9D000118);
  if(debug)
   printf("P0.CMD = 0x%x\n", rd_data);

  // Reading the Port#0 Task File Data Register(P0TFD):
  rd_data = readl(0x9D000120);
  if(debug)
   printf("P0.TFD = 0x%x\n", rd_data);

  // Reading the Port#0 SATA Status Register(P0SSTS):
  rd_data = readl(0x9D000128);
  if(debug)
   printf("P0.SSTS = 0x%x\n", rd_data);
 }
 else // For SATA Port#1
 {
  // Reading the Port#1 Command Register(P1CMD): (Reset Value)
  rd_data = readl(0x9D000198);
  if(debug)
   printf("P1.CMD = 0x%x\n", rd_data);

  // Reading the Port#1 Task File Data Register(P1TFD):
  rd_data = readl(0x9D0001A0);
  if(debug)
   printf("P1.TFD = 0x%x\n", rd_data);

  // Reading the Port#1 SATA Status Register(P1SSTS):
  rd_data = readl(0x9D0001A8);
  if(debug)
   printf("P1.SSTS = 0x%x\n", rd_data);
 }

 // Offset Variable
 // Performing HBA Reset using the Global HBA Control Register for SATA Controller
   printf("Resetting HBA\n");
 writel(0x00000001, 0x9D000004);

 // Reading the HBA Capabilities Register(CAP):
 offset = 0x0;
 rd_data = readl((SATA_CFG_BASE + offset));
 if(debug)
  printf("CAP = 0x%x\n", rd_data);

 // Clearing the (Read-Only) HBA Capabilities Register for SATA Controller
 writel(0x00000000, (SATA_CFG_BASE + offset));

 // Reading the HBA Capabilities Register(CAP):
 rd_data = readl((SATA_CFG_BASE + offset));
 if(debug)
  printf("CAP = 0x%x\n", rd_data);

 // Set the (Read-Only) Ports Implemented Register for SATA Controller to enable Ports 0 & 1
 offset = 0xC;
 writel(0x00000003, (SATA_CFG_BASE + offset));
 rd_data = readl((SATA_CFG_BASE + offset));
 if(debug)
  printf("CAP = 0x%x\n", rd_data);

 // Reading the BIST Control Register(BISTCR):
 offset = 0xA4;
 rd_data = readl((SATA_CFG_BASE + offset));
 if(debug)
  printf("BISTCR = 0x%x\n", rd_data);

 if(port_num == 0) // For SATA Port#0
 {
  // Reading the Port#0 SATA Error Register(P0SERR):
  offset = 0x130;
  rd_data = readl((SATA_CFG_BASE + offset));
  if(debug)
   printf("P0.SERR = 0x%x\n", rd_data);

  // Reading the Port#0 Command Register(P0CMD):
  offset = 0x118;
  rd_data = readl((SATA_CFG_BASE + offset));
  if(debug)
   printf("P0.CMD = 0x%x\n", rd_data);

  // Set the Command-List Base Address in the Port#0 Command-List Base Address Register(P0CLB) for SATA Controller
  offset = 0x100;
  writel(P0_CMD_BASE, (SATA_CFG_BASE + offset));

  // Set the FIS Base Address in the Port#0 FIS Base Address Register(P0FB) for SATA Controller
  offset = 0x108;
  writel(P0_FIS_BASE, (SATA_CFG_BASE + offset));
 }
 if((port_num == 1) || (both_ports)) // For SATA Port#1 or in case both Ports are being checked
 {
  // Reading the Port#1 SATA Error Register(P1SERR):
  offset = 0x1B0;
  rd_data = readl((SATA_CFG_BASE + offset));
  if(debug)
   printf("P1.SERR = 0x%x\n", rd_data);

  // Reading the Port#1 Command Register(P1CMD):
  offset = 0x198;
  rd_data = readl((SATA_CFG_BASE + offset));
  if(debug)
   printf("P1.CMD = 0x%x\n", rd_data);

  // Set the Command-List Base Address in the Port#1 Command-List Base Address Register(P1CLB) for SATA Controller
  offset = 0x180;
  writel(P1_CMD_BASE, (SATA_CFG_BASE + offset));

  // Set the FIS Base Address in the Port#1 FIS Base Address Register(P1FB) for SATA Controller
  offset = 0x188;
  writel(P1_FIS_BASE, (SATA_CFG_BASE + offset));
 }


 if(port_num == 0) // For SATA Port#0
 {
	 printf("Configure the Port#0 to Spin-Up Device & Enable FIS Receive\n");
  // Configure the Port#0 to Spin-Up Device & Enable FIS Receive, using the Port#0 Command Register(P0CMD) for SATA Controller
  offset = 0x118;
  writel(0x00000012, (SATA_CFG_BASE + offset));
 }
 if((port_num == 1) || (both_ports)) // For SATA Port#1 or in case both Ports are being checked
 {
  // Configure the Port#1 to Spin-Up Device & Enable FIS Receive, using the Port#1 Command Register(P1CMD) for SATA Controller
  offset = 0x198;
  writel(0x00000012, (SATA_CFG_BASE + offset));
 }

	printf("Waiting until the DIAG_X (Bit 26) is set\n");

 // Waiting until the DIAG_X (Bit 26) is set in the P#SERR indicating the COMINIT detection on the specified port
 wait_dev_cominit_dtctd(port_num, true, false); // Clear the Device COMINIT detected bit after it is detected by the SATA Controller

	printf("Waiting until the DIAG_W (Bit 18) is set\n");

 // Waiting until the DIAG_W (Bit 18) is set in the P#SERR indicating the COMWAKE detection on specified port
 wait_dev_comwake_dtctd(port_num, true, true); // Clear the Device COMWAKE detected bit after it is detected by the SATA Controller

 if(both_ports) // If both Ports are to be checked for OOB, check for COMINIT & COMWAKE for Port#1 now
 {
  // Waiting until the DIAG_X (Bit 26) is set in the P1SERR indicating the COMINIT detection on Port#1
  wait_dev_cominit_dtctd(1, true, false); // Clear the Device COMINIT detected bit after it is detected by the SATA Controller

  // Waiting until the DIAG_W (Bit 18) is set in the P1SERR indicating the COMWAKE detection on Port#1
  wait_dev_comwake_dtctd(1, true, false); // Clear the Device COMWAKE detected bit after it is detected by the SATA Controller
 }

 if(port_num == 0) // For SATA Port#0
 {
	 printf("Enable the PHY Ready Change Enable\n");
  // Enable the PHY Ready Change Enable, Descriptor Processed & FIS Interrupts in the Port#0 Interrupt Enable Register(P0IE)
  offset = 0x114;
  writel(0x0040003F, (SATA_CFG_BASE + offset));

	printf("Clear all the Interrupts\n");
  // Clear all the Interrupts in the Port#0 Interrupt Status Register(P0IS)
  offset = 0x110;
  writel(0xFFFFFFFF, (SATA_CFG_BASE + offset));
 }
 if((port_num == 1) || (both_ports)) // For SATA Port#1 or in case both Ports are being checked
 {
  // Enable the PHY Ready Change Enable, Descriptor Processed & FIS Interrupts in the Port#1 Interrupt Enable Register(P1IE)
  offset = 0x194;
  writel(0x0040003F, (SATA_CFG_BASE + offset));

  // Clear all the Interrupts in the Port#0 Interrupt Status Register(P1IS)
  offset = 0x190;
  writel(0xFFFFFFFF, (SATA_CFG_BASE + offset));
 }

 // Waiting until the Speed-negotiation with the Device is completed and the PHY Interface is clear & ready for transactions for the specified Port
 if(debug)
  printf("WAIT FOR DEVICE READY...\n");
 if(port_num == 0)
  wait_phyif_rdyclr_dtctd(port_num, port0_gen1, false);
 else
  wait_phyif_rdyclr_dtctd(port_num, port1_gen1, false);
 if(both_ports) // If both Ports are to be checked for OOB, check for Port#1 Speed Negotiation & Lane Ready and Clear now
 {
  wait_phyif_rdyclr_dtctd(1, port1_gen1, false);
 }

 if(port_num == 0) // For SATA Port#0
 {
  // Clear all the bits for the detected Interface errors on Port#0

  // Configure the Port#0 Command Register(P0CMD) to Spin-Up Device, Enable FIS Receive & Start processing the command-list
  if(!(both_ports))
   printf("SATA Controller's OOB Detection for Port#0 has completed!\n");
 }
 if((port_num == 1) || (both_ports)) // For SATA Port#1 or in case both Ports are being checked
 {
  // Clear all the bits for the detected Interface errors on Port#1

  // Configure the Port#1 Command Register(P1CMD) to Spin-Up Device, Enable FIS Receive & Start processing the command-list
  if(!(both_ports))
   printf("SATA Controller's OOB Detection for Port#1 has completed!\n");
 }
 if(both_ports)
  printf("SATA Controller's OOB Detection for both Port#0 & Port#1 has completed!\n");

 return;
}

// Method to wait for the detection of an interrupt on the specified port in the general Interrupt Status(IS) Register
// Input Parameters: port_num - Specifies the SATA Port on which we are waiting to detect the Interrupt
//                   clr_intrpt - Specifies to clear the Interrupt pending bit of the specified port in the Interrupt Status Register
//                   debug - Enable Debug Messaging
void wait_port_intrpt_dtctd(int port_num, bool clr_intrpt, bool debug)
{
 U32 rd_data;
 U32 port0_intrpt_dtctd_mask = 0x00000001;
 U32 port1_intrpt_dtctd_mask = 0x00000002;
 U32 masked_data;
 int IS_Offset = 0x08;
 bool entry = true;

 if((port_num > 1) || (port_num < 0)) // Input Parameter Check (SATA Controller has only 2 Ports - 0 & 1)
 {
  printf("Invalid SATA Port#%d was specified!\n", port_num);
  return;
 }

 do // Keep looping until you see the SATA Controller detect an Interrupt on the specified port
 {
  rd_data = readl(SATA_CFG_BASE + IS_Offset);
  if(debug)
     printf("IS Read data:0x%x\n", rd_data);
  entry = false; // The first entry into the loop has been completed
  if(port_num == 0)
  {
   masked_data = rd_data & port0_intrpt_dtctd_mask; // Bit-wise AND the read-data with the Mask so that we are only left with the bit-position corresponding to Port#0 Interrupt Pending
   if((masked_data == port0_intrpt_dtctd_mask) && debug)
    printf("Detected an Interrupt on Port#0!\n");
  }
  else
  {
   masked_data = rd_data & port1_intrpt_dtctd_mask; // Bit-wise AND the read-data with the Mask so that we are only left with the bit-position corresponding to Port#1 Interrupt Pending
   if((masked_data == port1_intrpt_dtctd_mask) && debug)
    printf("Detected an Interrupt on Port#1!\n");
  }
 }
 while(((masked_data != port0_intrpt_dtctd_mask) && (port_num == 0)) || ((masked_data != port1_intrpt_dtctd_mask) && (port_num == 1)));
 // Now that the interrupt has been seen (bit has been set), check if we need to clear the detection bit/interrupt
 if(clr_intrpt)
 {
  if(port_num == 0)
  {
   writel(port0_intrpt_dtctd_mask, (SATA_CFG_BASE + IS_Offset));
   if(debug)
    printf("The Port#0 Interrupt-pending bit is Cleared!\n");
  }
  else
  {
   writel(port1_intrpt_dtctd_mask, (SATA_CFG_BASE + IS_Offset));
   if(debug)
    printf("The Port#1 Interrupt-pending bit is Cleared!\n");
  }
 }
 return;
}

