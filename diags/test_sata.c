#include "tests.h"
#include <diags.h>
#include <common.h>
#include <asm/io.h>
#include <mach/comcerto-2000.h>
#include <mach/gpio.h>
#include <mach/clkcore.h>
#include "common_func.h"
#include "serdes_common.h"
#include "sata_common.h"

#define GPIO_BASE 0x90470000

int Comcerto_sata0_test (void)
{
	U32 rd_data, ret = -1, offset;
	int i;

 //bootstrap overwriting
   reg_wr(GPIO_BASE + 0x44,0x30000);

//PLL programming to get different clks
  reg_wr(0x904B0004,0x0);
  reg_wr(0x904B0008,0x0);
  reg_wr(0x904B0160,0x3);
  reg_wr(0x904B0164,0x3);
  reg_wr(0x904B0174,0x6);

 // SATA, Port#0 Initialization
 // sbphy_init_wait_lane_rdy(bool forPCIE, bool forSATA, bool forSGMII, int PCIE_num, int SATA_num, bool sata_p0_gen1, bool sata_p1_gen1, bool debug)
 sbphy_init_wait_lane_rdy(false, true, false, 0, 0, false, false, false);

 // sata_oob_detect(bool both_ports, int port_num, bool port0_gen1, bool port1_gen1, bool debug)
 //sata_oob_detect(false, 0, false, false, false);

// printf("\nStart SATA BIST Far End Retransmit Test...\n\n");
 // Set the Command-List Base Address in the Port#0 Command-List Base Address Register(P0CLB) for SATA Controller
 // printf("Set P0.CLB:Command-List Base Address on Port0\n");
 // reg_wr(P0CLB, P0_CMD_BASE);          //P0_CMD_BASE, 0x83010000

 // Set the FIS Base Address in the Port#0 FIS Base Address Register(P0FB) for SATA Controller
 // printf("Set P0.FIS:FIS Base Address in the Port0\n");
 // reg_wr(P0FIS, P0_FIS_BASE);          //P0_FIS_BASE, 0x8301A000

 // Clear all the bits for the detected Interface errors on Port0
 //printf("Clear P0.SERR\n");
 //offset = 0x130;
 //reg_wr((SATA_CFG_BASE + offset), 0xFFFFFFFF);
 reg_wr(P0SERR, 0xFFFFFFFF);

 // Configure the Port0 Command Register to Spin-Up Device, Enable FIS Receive & Start processing the command-list
 //printf("Set P0.CMD\n");
 //offset = 0x118;
 //reg_wr((SATA_CFG_BASE + offset), 0x00000013);
 reg_wr(P0CMD, 0x00000013);

 // Clear all the Interrupts in the Interrupt Status(IS) Register
 //printf("Clear IS :Clear all the Interrupts in the Interrupt Status(IS) Register\n");
 //offset = 0x008;
 //reg_wr((SATA_CFG_BASE + offset), 0xFFFFFFFF);
 reg_wr(IS, 0xFFFFFFFF);

 // Read the AHCI Enable bit set in the Global HBA Control Register(GHC)
 //offset = 0x004;
 //rd_data = reg_rd((SATA_CFG_BASE + offset));
 rd_data = reg_rd(GHC);
 //printf("GHC = 0x%x\n", rd_data);
 //printf("Set P0.GHC:Enable the IRQ in the Global HBA Control Register\n");
 //reg_wr((SATA_CFG_BASE + offset), 0x80000002);
 reg_wr(GHC, 0x80000002);

 // Read the Port0 Command Issue Register(P0CI)
 //offset = 0x138;
 //rd_data = reg_rd((SATA_CFG_BASE + offset));
 rd_data = reg_rd(P0CI);
// printf("P0.CI = 0x%x\n", rd_data);

//// SATA 0
//  Start bist loopback initiator
//  select the port1 for BIST operation TESTR.PSEL
 //offset = 0x0F4;
 //reg_wr((SATA_CFG_BASE + offset), 0x0);
 reg_wr(TESTR, 0x0);

 //  select the bist pattern from BISTCR[3:0], currently selecting Low frequency test pattern (LFTP)
 //offset = 0x0A4;
 //reg_wr((SATA_CFG_BASE + offset), 0x00000008);
 reg_wr(BISTCR, 0x00000007);

 // Builds a BIST FIS
 // Backdoor Load the Command-List for port1 into IRAM (to be replaced with DDR Address Backddor or backdoor in TB)
 // printf("Set up CMD for BIST\n");
 reg_wr((P0_CMD_BASE), 0x00000205);		//[31:16] PRDTL = 0x0001, [4:0] CFL = 5'b01001
 reg_wr((P0_CMD_BASE + 0x4), 0x00000000);	//PRD Byte Count
 reg_wr((P0_CMD_BASE + 0x8), P0_CMD_TABLE);	//Command table base
 reg_wr((P0_CMD_BASE + 0xC), 0x00000000);	//Command table base upper

 // Backdoor Load the Command Register FIS words for port1 to Device into IRAM (to be replaced with DDR Address Backddor or backdoor in TB)
 //printf("Set up CMD Table for BIST FIS\n");
 reg_wr((P0_CMD_TABLE), 0x00100058);		// BIST Activate FIS with Far-end retimed
 reg_wr((P0_CMD_TABLE + 0x4),  0x33221100);	//Dev/Head,CycHigh,CycLow,SectorNum
 reg_wr((P0_CMD_TABLE + 0x8),  0x77665544);	//Dev/Head,CycHigh,CycLow,SectorNum
 reg_wr((P0_CMD_TABLE + 0xC),  0x00000000);	//Control,REVD,SectorCnt,SectorCnt
 reg_wr((P0_CMD_TABLE + 0x10), 0x00000000);	//REVD
 reg_wr((P0_CMD_TABLE + 0x14), 0x00000000);	//REVD ??

 // Backdoor Load the PRDT/PRDT pointer at offset P0_CMD_TABLE + 0x80
 reg_wr((P0_CMD_TABLE + 0x80), P0_DAT_BUFF);	//Data base address for TX
 reg_wr((P0_CMD_TABLE + 0x84), 0x00000000);	//Data base address upper
 reg_wr((P0_CMD_TABLE + 0x88), 0x00000000);	//REVD
 reg_wr((P0_CMD_TABLE + 0x8C), 0x800001FF);	//ByteCnt

 //printf("BIST CMD Kickoff\n");
 //offset = 0x138;
 //reg_wr((SATA_CFG_BASE + offset), 0x00000001);
 reg_wr(P0CI, 0x00000001);

 // Waiting until PxSSTS.DET return 4h, when read.  it mean is in BIST mode
 //offset = 0x128;
 //rd_data = (SATA_CFG_BASE + offset);
 rd_data = reg_rd(P0SSTS);
// printf("P0.SSTS = 0x%x\n", rd_data);
 while((rd_data & 0x00000004) != 0x4)
 {
 	rd_data = reg_rd(P0SSTS);
 	//printf("P0.SSTS = 0x%x\n", rd_data);
 }

 // Read BIST FIS Count Register  (BISTFCTR)
 //offset = 0x0A8;
 //rd_data = (SATA_CFG_BASE + offset);
 rd_data = reg_rd(BISTFCTR);
 //printf("BISTFCTR = 0x%x\n", rd_data);

 // Read BIST Status Register (BISTSR)
 //offset = 0x0AC;
 //rd_data = (SATA_CFG_BASE + offset);
 rd_data = reg_rd(BISTSR);
// printf("BISTSR = 0x%x\n", rd_data);

 // Read BIST DWORD Error Count Register (BISTDECR)
 //offset = 0x0B0;
 //rd_data = (SATA_CFG_BASE + offset);
 rd_data = reg_rd(BISTDECR);
// printf("BISTDECR = 0x%x\n", rd_data);
 	if(rd_data == 0x00000000)
	{
		printf("SATA0 BIST Far End Retransmit Test Passed!\n");
		ret = 0;
	}
	else
		printf("SATA0 BIST Far End Retransmit Test Failed!\n");

 //pass_ctl();
 reg_wr(P0CMD, 0x00000012);
 rd_data = reg_rd(P0CI);
 printf("P0.CI = 0x%x\n", rd_data);

 // Apply COMRESET
 reg_wr(P0SCTL,0x1);
 printf("Port Reset P0.SCTL = 0x%x\n",0x1);

 rd_data = reg_rd(P0SSTS);
 printf("P0.SSTS = 0x%x\n", rd_data);

	return ret;
}


//// SATA 1
int Comcerto_sata1_test (void)
{
	U32 rd_data, ret = -1, offset;
	int i;

 //bootstrap overwriting
   reg_wr(GPIO_BASE + 0x44,0x30000);

//PLL programming to get different clks
  reg_wr(0x904B0004,0x0);
  reg_wr(0x904B0008,0x0);
  reg_wr(0x904B0160,0x3);
  reg_wr(0x904B0164,0x3);
  reg_wr(0x904B0174,0x6);

 // SATA, Port#1 Initialization
 // sbphy_init_wait_lane_rdy(bool forPCIE, bool forSATA, bool forSGMII, int PCIE_num, int SATA_num, bool sata_p0_gen1, bool sata_p1_gen1, bool debug)
 sbphy_init_wait_lane_rdy(false, true, false, 0, 1, false, false, false);

 // sata_oob_detect(bool both_ports, int port_num, bool port0_gen1, bool port1_gen1, bool debug)
 //sata_oob_detect(false, 1, false, false, false);

 // Clear all the bits for the detected Interface errors on Port1
 //printf("Clear P1.SERR\n");
 reg_wr(P1SERR, 0xFFFFFFFF);

 // Configure the Port1 Command Register to Spin-Up Device, Enable FIS Receive & Start processing the command-list
 //printf("Set P1.CMD\n");
 reg_wr(P1CMD, 0x00000013);

 // Clear all the Interrupts in the Interrupt Status(IS) Register
 //printf("Clear IS :Clear all the Interrupts in the Interrupt Status(IS) Register\n");
 reg_wr(IS, 0xFFFFFFFF);

 // Read the AHCI Enable bit set in the Global HBA Control Register(GHC)
 rd_data = reg_rd(GHC);
 //printf("GHC = 0x%x\n", rd_data);
 // Enable the Interrupts in the Global HBA Control Register
// printf("Set GHC\n");
 reg_wr(GHC, 0x80000002);

 // Read the Port1 Command Issue Register(P1CI)
 rd_data = reg_rd(P1CI);

//  Start bist loopback initiator
// printf("Select the port1 for BIST operation TESTR.PSEL\n");
 //offset = 0x0F4;
 //reg_wr((SATA_CFG_BASE + offset), 0x0);
 reg_wr(TESTR, 0x10000);

 //  select the bist pattern from BISTCR[3:0], currently selecting Low frequency test pattern (LFTP)
 //offset = 0x0A4;
 //reg_wr((SATA_CFG_BASE + offset), 0x00000008);
 reg_wr(BISTCR, 0x00000007);

 // Builds a BIST FIS
 // Backdoor Load the Command-List for port1 into IRAM (to be replaced with DDR Address Backddor or backdoor in TB)
 // printf("Set up CMD for BIST\n");
 reg_wr((P1_CMD_BASE), 0x00000205);		//[31:16] PRDTL = 0x0001, [4:0] CFL = 5'b01001
 reg_wr((P1_CMD_BASE + 0x4), 0x00000000);	//PRD Byte Count
 reg_wr((P1_CMD_BASE + 0x8), P1_CMD_TABLE);	//Command table base
 reg_wr((P1_CMD_BASE + 0xC), 0x00000000);	//Command table base upper

 // Backdoor Load the Command Register FIS words for port1 to Device into IRAM (to be replaced with DDR Address Backddor or backdoor in TB)
 //printf("Set up CMD Table for BIST FIS\n");
 reg_wr((P1_CMD_TABLE), 0x00100058);		// BIST Activate FIS with Far-end retimed
 reg_wr((P1_CMD_TABLE + 0x4),  0x33221100);	//Dev/Head,CycHigh,CycLow,SectorNum
 reg_wr((P1_CMD_TABLE + 0x8),  0x77665544);	//Dev/Head,CycHigh,CycLow,SectorNum
 reg_wr((P1_CMD_TABLE + 0xC),  0x00000000);	//Control,REVD,SectorCnt,SectorCnt
 reg_wr((P1_CMD_TABLE + 0x10), 0x00000000);	//REVD
 reg_wr((P1_CMD_TABLE + 0x14), 0x00000000);	//REVD ??

 // Backdoor Load the PRDT/PRDT pointer at offset P1_CMD_TABLE + 0x80
 reg_wr((P1_CMD_TABLE + 0x80), P1_DAT_BUFF);	//Data base address for TX
 reg_wr((P1_CMD_TABLE + 0x84), 0x00000000);	//Data base address upper
 reg_wr((P1_CMD_TABLE + 0x88), 0x00000000);	//REVD
 reg_wr((P1_CMD_TABLE + 0x8C), 0x800001FF);	//ByteCnt

 //printf("BIST CMD Kickoff\n");
 //offset = 0x138;
 //reg_wr((SATA_CFG_BASE + offset), 0x00000001);
 reg_wr(P1CI, 0x00000001);

 // Waiting until PxSSTS.DET return 4h, when read.  it mean is in BIST mode
 //offset = 0x128;
 //rd_data = (SATA_CFG_BASE + offset);
 rd_data = reg_rd(P1SSTS);
// printf("P1.SSTS = 0x%x\n", rd_data);
 while((rd_data & 0x00000004) != 0x4)
 {
 	rd_data = reg_rd(P1SSTS);
 	//printf("P1.SSTS = 0x%x\n", rd_data);
 }

 // Read BIST FIS Count Register  (BISTFCTR)
 //offset = 0x0A8;
 //rd_data = (SATA_CFG_BASE + offset);
 rd_data = reg_rd(BISTFCTR);
 //printf("BISTFCTR = 0x%x\n", rd_data);

 // Read BIST Status Register (BISTSR)
 //offset = 0x0AC;
 //rd_data = (SATA_CFG_BASE + offset);
 rd_data = reg_rd(BISTSR);
// printf("BISTSR = 0x%x\n", rd_data);

 // Read BIST DWORD Error Count Register (BISTDECR)
 //offset = 0x0B0;
 //rd_data = (SATA_CFG_BASE + offset);
 rd_data = reg_rd(BISTDECR);
// printf("BISTDECR = 0x%x\n", rd_data);
 	if(rd_data == 0x00000000)
	{
		printf("SATA1 BIST Far End Retransmit Test Passed!\n");
		ret = 0;
	}
	else
		printf("SATA1 BIST Far End Retransmit Test Failed!\n");

 //pass_ctl();
 reg_wr(P1CMD, 0x00000012);
 rd_data = reg_rd(P1CI);
 printf("P1.CI = 0x%x\n", rd_data);

 // Apply COMRESET
 reg_wr(P1SCTL,0x1);
 printf("Port Reset P1.SCTL = 0x%x\n",0x1);

 rd_data = reg_rd(P1SSTS);
 printf("P1.SSTS = 0x%x\n", rd_data);

	return ret;
}

int Comcerto_sata_test (struct diags_test_param *p)
{
	int ret = 0;

	ret = Comcerto_sata0_test ();
	ret = Comcerto_sata1_test ();

	return ret;
}

