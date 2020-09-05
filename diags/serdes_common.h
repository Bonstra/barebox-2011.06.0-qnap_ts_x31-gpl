// Define the Base Addresses of the SB-PHY Configuration Space
// (from Memory Map)
//  for SD0 --> 0x9059_0000
//              0x9059_3FFF(0x2BFF)
//

#define PCIE_SATA_RST_CNTRL 0x904B0008

#define SD0_CFG_BASE  0x90590000
#define SD1_CFG_BASE  0x90594000
#define SD2_CFG_BASE  0x90598000
#define PHY_CFG_BASE  0x90410000
#define SD_COMMON_CMU     0x000
#define SD_LANE0          0x200
#define SD_LANE1          0x400
#define SD_LANE2          0x600
#define SD_LANE3          0x800
#define SD_COMMON_LANE    0xA00
#define GPIO_BASE 0x90470000

// worked TX_LEV 6, TLEV 7, others=0

#define TX_EQMODE0 0
#define TX_EQMODE12 0
#define TX_LEV       0x6
#define TX_DTLEV02 0
#define TX_TLEV02 0x7

#include "PCIeMacro.h"

#define	U32	u32
#define	U8	u8

U8 clock_48Mhz = 0;

// ###################################################################################
// ###################################################################################

//***************************************************************************************************************************
// Method to wait for the 'CMU OK' to be signaled by the Snowbush Serdes PHY
// Input Parameter: sbphy_num - Specifies the instance of the Serdes for which we are waiting to recieve the 'CMU OK' on (0 - SD1, 1 - SD1 & 2 -SD2)
//                  debug - Enables the printing of Debug messages
void wait_cmu_ok(int sbphy_num, bool debug)
{
 U32 rd_data = 0x00000000;
 U32 cmu_ok_dtctd_mask = 0x00004000;
 U32 masked_data;
 int CMU_Offset;
 bool entry = true;


 switch(sbphy_num) // Check the Serdes instance/port
 {
  case 0: CMU_Offset = 0x2C;
          break;
  case 1: CMU_Offset = 0x3C; // 10'h00F
          break;
  case 2: CMU_Offset = 0x4C;
          break;
  default: printf("Error: wait_cmu_ok - Invalid Snowbush PHY#%d specified!\n", sbphy_num);
           return;
 }

 do // Keep looping until you see the cmu_ok_o of Serdes
 { /*
  if(entry == false) // Check this is not the first-time in the loop
  {
   // Wait for some time before you perform the next read
   nop(1); // Have the ARM issue some NOPs
  } */
  rd_data = reg_rd(PHY_CFG_BASE + CMU_Offset);
  if(debug)
   printf("SD Ctrl Reg:0x%x\n", rd_data);
  entry = false; // The first entry into the loop has been completed
  masked_data = rd_data & cmu_ok_dtctd_mask; // Bit-wise AND the read-data with the Mask so that we are only left with the bit-position corresponding to cmu_ok Status Pending
  if(masked_data == cmu_ok_dtctd_mask)
  {
   switch(sbphy_num) // Check the appropriate Serdes instance/port
   {
    case 0: printf("CMU OK Detected on PCIE Serdes0!\n");
            break;
    case 1: printf("CMU OK Detected on Serdes1!\n");
            break;
    case 2: printf("CMU OK Detected on Serdes2!\n");
            break;
    default: printf("Error: wait_cmu_ok - Invalid Snowbush PHY#%d specified!\n", sbphy_num);
   }
  }
 }
 while(masked_data != cmu_ok_dtctd_mask);
 return;
}



//***************************************************************************************************************************
// Method to wait for the 'Lane OK' to be signaled by the Snowbush Serdes PHY
// Input Parameter: sbphy_num - Specifies the instance of the Serdes for which we are waiting to recieve the 'Lane OK' on (0 - SD1, 1 - SD1 & 2 -SD2)
//                  debug - Enables the printing of Debug messages
void wait_lane_ok(int sbphy_num, bool debug)
{
 U32 rd_data = 0x00000000;
 U32 lane_ok_dtctd_mask = 0x00001000;
 U32 masked_data;
 int Lane_Offset;
 bool entry = true;

 switch(sbphy_num) // Check the Serdes instance/port
 {
  case 0: Lane_Offset = 0x2C;
          break;
  case 1: Lane_Offset = 0x3C; // 10'h00F
          break;
  case 2: Lane_Offset = 0x4C;
          break;
  default: printf("Error: wait_lane_ok - Invalid Snowbush PHY #%d specified!\n", sbphy_num);
           return;
 }

 do // Keep looping until you see the lane_ok_o of Serdes
 { /*
  if(entry == false) // Check this is not the first-time in the loop
  {
   // Wait for some time before you perform the next read
   nop(1); // Have the ARM issue some NOPs
  } */
  rd_data = reg_rd(PHY_CFG_BASE + Lane_Offset);
  if(debug)
   printf("SD Ctrl Reg:0x%x\n", rd_data);
  entry = false; // The first entry into the loop has been completed
  masked_data = rd_data & lane_ok_dtctd_mask; // Bit-wise AND the read-data with the Mask so that we are only left with the bit-position corresponding to lane_ok Status Pending
  if(masked_data == lane_ok_dtctd_mask)
  {
   switch(sbphy_num) // Check the appropriate Serdes instance/port
   {
    case 0: printf("Lane OK Detected on PCIE Serdes0!\n");
            break;
    case 1: printf("Lane OK Detected on Serdes1!\n");
            break;
    case 2: printf("Lane OK Detected on Serdes2!\n");
            break;
    default: printf("Error: wait_lane_ok - Invalid Snowbush PHY #%d specified!\n", sbphy_num);
   }
  }
 }
 while(masked_data != lane_ok_dtctd_mask);
 return;
}



//***************************************************************************************************************************
// Method to Initialize the Snowbush PHY (Serdes) for operation with the one (or all) of the PCIE,SATA and/or SGMII IP blocks
// Input Parameters: forPCIE - Specifies if the Serdes port(s)/lanes corresponding to the PCIE IP are to be initialized
//                   forSATA - Specifies if the Serdes port(s)/lanes corresponding to the SATA IP are to be initialized
//                   forSGMII - Specifies if the Serdes port(s)/lanes corresponding to the SGMII are to be initialized
//                   PCIE_num - Specifies the specific PCIE IP instance(s), for which the port/lanes are to be initialized (0: PCIE0, 1:PCIE1, & 2 or 3:both PCIE0 & PCIE1)
//                   SATA_num - Specifies the specific SATA IP Port(s), for which the port/lanes are to be initialized (0:SATA0, 1:SATA1, & 2 or 3:both SATA0 & SATA1)
//                   sata_p0_gen1 - Specifies that the Serdes Instance-1 for SATA Port0 is to be configured for Gen-I (1.5Gbps) operation [Only applicable when forSATA:true & SATA_num: 0,2 or 3]
//                   sata_p1_gen1 - Specifies that the Serdes Instance-2 for SATA Port1 is to be configured for Gen-I (1.5Gbps) operation [Only applicable when forSATA:true & SATA_num: 1,2 or 3]
//                   debug - Enables the printing of Debug messages
// Note: To initialize all 3 simultaneously - forPCIE=true, forSATA=true, forSGMII = true, PCIE_num = 2 & SATA_num = 2
void init_sbphy(bool forPCIE, bool forSATA, bool forSGMII, int PCIE_num, int SATA_num, bool sata_p0_gen1, bool sata_p1_gen1, bool debug)
{
 // Invalid Parameter Check
 if((forPCIE) && (PCIE_num > 3))
 {
  printf("Error: init_sbphy - Invalid PCIE (option or) Instance#%d was specified!\n", PCIE_num);
  return;
 }
 if((forSATA) && (SATA_num > 3))
 {
  printf("Error: init_sbphy - Invalid SATA (option or) Port#%d was specified!\n", SATA_num);
  return;
 }

 //printf("SATA_En=%d, SATA Serdes Num:%d & PCIE Serdes:%d\n", forSATA, SATA_num, PCIE_num);

 // Serdes-0(SD0) Initialization
 if((forPCIE) && ((PCIE_num == 0) || (PCIE_num == 2) || (PCIE_num == 3)))
 {
  if(debug)
   printf("Performing the Snowbush PHY (SD0) Initialization for PCIE0...\n");
  if(debug)
   printf("SD0 Init - Common CMU Block\n");
  reg_wr((SD0_CFG_BASE+ (0x000 << 2)), 0x06);
  reg_wr((SD0_CFG_BASE+ (0x001 << 2)), 0x00);
  //reg_wr((SD0_CFG_BASE+ (0x002 << 2)), 0x81);

  // enable output clock
  reg_wr((SD0_CFG_BASE+ (0x002 << 2)), 0x01);

  reg_wr((SD0_CFG_BASE+ (0x003 << 2)), 0x00);



  // for 48 Mhz
  if (clock_48Mhz){
  reg_wr((SD0_CFG_BASE+ (0x004 << 2)), 0x60);
  reg_wr((SD0_CFG_BASE+ (0x005 << 2)), 0x09);
  reg_wr((SD0_CFG_BASE+ (0x006 << 2)), 0x0e);
  }
  else{
  reg_wr((SD0_CFG_BASE+ (0x004 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x005 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x006 << 2)), 0x00);
  }


  reg_wr((SD0_CFG_BASE+ (0x007 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x008 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x009 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x00A << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x00B << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x00C << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x00D << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x00E << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x00F << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x010 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x011 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x012 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x013 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x014 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x015 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x016 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x017 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x018 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x019 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x01A << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x01B << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x01C << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x01D << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x01E << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x01F << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x020 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x021 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x022 << 2)), 0xa0);


  // for 48 MHz
  if (clock_48Mhz)
    reg_wr((SD0_CFG_BASE+ (0x023 << 2)), 0x6C);
  else
    reg_wr((SD0_CFG_BASE+ (0x023 << 2)), 0x64);

  reg_wr((SD0_CFG_BASE+ (0x024 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x025 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x026 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x027 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x028 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x029 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x02A << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x02B << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x02C << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x02D << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x02E << 2)), 0x04);
  reg_wr((SD0_CFG_BASE+ (0x02F << 2)), 0x50);
  reg_wr((SD0_CFG_BASE+ (0x030 << 2)), 0x70);
  reg_wr((SD0_CFG_BASE+ (0x031 << 2)), 0x02);
  reg_wr((SD0_CFG_BASE+ (0x032 << 2)), 0x25);
  reg_wr((SD0_CFG_BASE+ (0x033 << 2)), 0x40);
  reg_wr((SD0_CFG_BASE+ (0x034 << 2)), 0x01);
  reg_wr((SD0_CFG_BASE+ (0x035 << 2)), 0x40);
  reg_wr((SD0_CFG_BASE+ (0x036 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x037 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x038 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x039 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x03A << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x03B << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x03C << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x03D << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x03E << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x03F << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x040 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x041 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x042 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x043 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x044 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x045 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x046 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x047 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x048 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x049 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x04A << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x04B << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x04C << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x04D << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x04E << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x04F << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x050 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x051 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x052 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x053 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x054 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x055 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x056 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x057 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x058 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x059 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x05A << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x05B << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x05C << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x05D << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x05E << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x05F << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x060 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x061 << 2)), 0x2e);



  // for 48Mhz
  if (clock_48Mhz)
    reg_wr((SD0_CFG_BASE+ (0x062 << 2)), 0x08);
  else
    reg_wr((SD0_CFG_BASE+ (0x062 << 2)), 0x00);

  reg_wr((SD0_CFG_BASE+ (0x063 << 2)), 0x5e);
  reg_wr((SD0_CFG_BASE+ (0x064 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x065 << 2)), 0x42);
  reg_wr((SD0_CFG_BASE+ (0x066 << 2)), 0xd1);
  //reg_wr((SD0_CFG_BASE+ (0x067 << 2)), 0x90);///changed from 80

  reg_wr((SD0_CFG_BASE+ (0x067 << 2)), 0x90);///changed from 80

  reg_wr((SD0_CFG_BASE+ (0x068 << 2)), 0x08);



  // for 48 Mhz
  if (clock_48Mhz){
  reg_wr((SD0_CFG_BASE+ (0x069 << 2)), 0x90);
  reg_wr((SD0_CFG_BASE+ (0x06A << 2)), 0x2C);
  reg_wr((SD0_CFG_BASE+ (0x06B << 2)), 0x32);
  reg_wr((SD0_CFG_BASE+ (0x06C << 2)), 0x59);
  reg_wr((SD0_CFG_BASE+ (0x06D << 2)), 0x03);
  }else
  {
  reg_wr((SD0_CFG_BASE+ (0x069 << 2)), 0x50);
  reg_wr((SD0_CFG_BASE+ (0x06A << 2)), 0x44);
  reg_wr((SD0_CFG_BASE+ (0x06B << 2)), 0xce);
  reg_wr((SD0_CFG_BASE+ (0x06C << 2)), 0x0b);
  reg_wr((SD0_CFG_BASE+ (0x06D << 2)), 0x00);
  }

  reg_wr((SD0_CFG_BASE+ (0x06E << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x06F << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x070 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x071 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x072 << 2)), 0x00);

  if(debug)
   printf("SD0 Init - Lane0 Block\n");

  // don't revesrse polarity on TX
  reg_wr((SD0_CFG_BASE+ (0x200 << 2)), 0x00);
  // reverse polarity TX
  //reg_wr((SD0_CFG_BASE+ (0x200 << 2)), 0x08);

  // loopback on PIPE interface
  reg_wr((SD0_CFG_BASE+ (0x200 << 2)), 0x04);

  reg_wr((SD0_CFG_BASE+ (0x201 << 2)), 0x00);




  // don't reverse polarity on RX
  reg_wr((SD0_CFG_BASE+ (0x202 << 2)), 0x00);
  // reverse polarity RX
  //reg_wr((SD0_CFG_BASE+ (0x202 << 2)), 0x02);

  reg_wr((SD0_CFG_BASE+ (0x203 << 2)), 0x00);


  reg_wr((SD0_CFG_BASE+ (0x204 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x205 << 2)), 0x10);
// ORIGINAL_SETUP
  //reg_wr((SD0_CFG_BASE+ (0x206 << 2)), 0x84);
  //reg_wr((SD0_CFG_BASE+ (0x207 << 2)), 0x04);

  // bhimsen settings for Gen1
  //reg_wr((SD0_CFG_BASE+ (0x206 << 2)), 0x44);
  //reg_wr((SD0_CFG_BASE+ (0x207 << 2)), 0x34);

  // bhimsen settings for Gen2
  //reg_wr((SD0_CFG_BASE+ (0x206 << 2)), 0xC4);
  //reg_wr((SD0_CFG_BASE+ (0x207 << 2)), 0x34);

  // our settings
  //reg_wr((SD0_CFG_BASE+ (0x206 << 2)), (0x04 | TX_EQMODE12 << 6));
  //reg_wr((SD0_CFG_BASE+ (0x207 << 2)), (TX_LEV << 2) | TX_EQMODE0);

  // final settings
  reg_wr((SD0_CFG_BASE+ (0x206 << 2)), 0x04);
  reg_wr((SD0_CFG_BASE+ (0x207 << 2)), 0x18);

  reg_wr((SD0_CFG_BASE+ (0x208 << 2)), 0xe0);
  reg_wr((SD0_CFG_BASE+ (0x210 << 2)), 0x23);
  reg_wr((SD0_CFG_BASE+ (0x211 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x212 << 2)), 0x00);



  // for 48 Mhz
  if (clock_48Mhz){
  reg_wr((SD0_CFG_BASE+ (0x213 << 2)), 0x04);
  reg_wr((SD0_CFG_BASE+ (0x214 << 2)), 0xC0);
  reg_wr((SD0_CFG_BASE+ (0x215 << 2)), 0x18);
  }
  else{
  reg_wr((SD0_CFG_BASE+ (0x213 << 2)), 0x03);
  reg_wr((SD0_CFG_BASE+ (0x214 << 2)), 0x3c);
  reg_wr((SD0_CFG_BASE+ (0x215 << 2)), 0x04);

  }
  reg_wr((SD0_CFG_BASE+ (0x216 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x217 << 2)), 0x68);
  reg_wr((SD0_CFG_BASE+ (0x218 << 2)), 0xa2);
  reg_wr((SD0_CFG_BASE+ (0x219 << 2)), 0x1e);
  reg_wr((SD0_CFG_BASE+ (0x21A << 2)), 0x18);
  reg_wr((SD0_CFG_BASE+ (0x21B << 2)), 0x0d);
  reg_wr((SD0_CFG_BASE+ (0x21C << 2)), 0x0d);
  reg_wr((SD0_CFG_BASE+ (0x21D << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x21E << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x21F << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x220 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x221 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x222 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x223 << 2)), 0x00);
  // adjust RX/TX impedance using registers 0x24 and 0x25
  // original settings
  reg_wr((SD0_CFG_BASE+ (0x224 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x225 << 2)), 0x00);
  // manual RX impedance settings
  //reg_wr((SD0_CFG_BASE+ (0x224 << 2)), 0x10);
  //reg_wr((SD0_CFG_BASE+ (0x225 << 2)), 0x01);

  reg_wr((SD0_CFG_BASE+ (0x226 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x227 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x228 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x229 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x22A << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x22B << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x22C << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x22D << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x22E << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x22F << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x230 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x231 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x232 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x233 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x234 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x235 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x236 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x237 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x238 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x239 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x23A << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x23B << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x23C << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x23D << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x23E << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x23F << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x240 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x241 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x242 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x243 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x244 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x245 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x246 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x247 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x248 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x249 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x24A << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x24B << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x24C << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x24D << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x24E << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0x24F << 2)), 0x00);
  // original
  //reg_wr((SD0_CFG_BASE+ (0x250 << 2)), 0x60);
  //reg_wr((SD0_CFG_BASE+ (0x251 << 2)), 0x0f);
    // bhimsen settings  for Gen1
  //reg_wr((SD0_CFG_BASE+ (0x250 << 2)), 0xB6);
  //reg_wr((SD0_CFG_BASE+ (0x251 << 2)), 0x17);

   // bhimsen settings  for Gen2
  //reg_wr((SD0_CFG_BASE+ (0x250 << 2)), 0xF6);
  //reg_wr((SD0_CFG_BASE+ (0x251 << 2)), 0x07);

  // our settings
  //reg_wr((SD0_CFG_BASE+ (0x250 << 2)), 0x16| TX_TLEV02 << 5);
  //reg_wr((SD0_CFG_BASE+ (0x251 << 2)), 0x3);
  //printf("x250 0x%X\n",0x16| TX_TLEV02 << 5 );

  // final settings
  reg_wr((SD0_CFG_BASE+ (0x250 << 2)), 0xF6);
  reg_wr((SD0_CFG_BASE+ (0x251 << 2)), 0x3);

  if(debug)
   printf("SD0 Init - Common Lane Block\n");
  reg_wr((SD0_CFG_BASE+ (0xA00 << 2)), 0xc0);
  reg_wr((SD0_CFG_BASE+ (0xA01 << 2)), 0x90);
  reg_wr((SD0_CFG_BASE+ (0xA02 << 2)), 0x02);
  reg_wr((SD0_CFG_BASE+ (0xA03 << 2)), 0x40);
  reg_wr((SD0_CFG_BASE+ (0xA04 << 2)), 0x3c);
  reg_wr((SD0_CFG_BASE+ (0xA05 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0xA06 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0xA07 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0xA08 << 2)), 0x00);



  // for 48Mhz
  if (clock_48Mhz){
    reg_wr((SD0_CFG_BASE+ (0xA09 << 2)), 0xC3);
    reg_wr((SD0_CFG_BASE+ (0xA0A << 2)), 0xCA);
  }
  else{
    reg_wr((SD0_CFG_BASE+ (0xA09 << 2)), 0x63);
    reg_wr((SD0_CFG_BASE+ (0xA0A << 2)), 0x49);
  }
  reg_wr((SD0_CFG_BASE+ (0xA0B << 2)), 0xc6);
  reg_wr((SD0_CFG_BASE+ (0xA0C << 2)), 0x01);
  reg_wr((SD0_CFG_BASE+ (0xA0D << 2)), 0x03);
  reg_wr((SD0_CFG_BASE+ (0xA0E << 2)), 0x28);
  reg_wr((SD0_CFG_BASE+ (0xA0F << 2)), 0x98);
  reg_wr((SD0_CFG_BASE+ (0xA10 << 2)), 0x19);
  reg_wr((SD0_CFG_BASE+ (0xA11 << 2)), 0x28);
  reg_wr((SD0_CFG_BASE+ (0xA12 << 2)), 0x78);
  reg_wr((SD0_CFG_BASE+ (0xA13 << 2)), 0xe1);
  reg_wr((SD0_CFG_BASE+ (0xA14 << 2)), 0xf0);
  reg_wr((SD0_CFG_BASE+ (0xA15 << 2)), 0x10);
  reg_wr((SD0_CFG_BASE+ (0xA16 << 2)), 0xf4);
  reg_wr((SD0_CFG_BASE+ (0xA17 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0xA30 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0xA31 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0xA32 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0xA33 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0xA34 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0xA35 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0xA36 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0xA37 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0xA38 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0xA39 << 2)), 0xa0);
  reg_wr((SD0_CFG_BASE+ (0xA3A << 2)), 0xa0);
  reg_wr((SD0_CFG_BASE+ (0xA3B << 2)), 0xa0);
  reg_wr((SD0_CFG_BASE+ (0xA3C << 2)), 0xa0);
  reg_wr((SD0_CFG_BASE+ (0xA3D << 2)), 0xa0);
  reg_wr((SD0_CFG_BASE+ (0xA3E << 2)), 0xa0);
  reg_wr((SD0_CFG_BASE+ (0xA3F << 2)), 0xa0);



  // for 48 Mhz
  if (clock_48Mhz)
    reg_wr((SD0_CFG_BASE+ (0xA40 << 2)), 0x6C);
  else
    reg_wr((SD0_CFG_BASE+ (0xA40 << 2)), 0x64);

  reg_wr((SD0_CFG_BASE+ (0xA41 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0xA42 << 2)), 0xc0);
  reg_wr((SD0_CFG_BASE+ (0xA43 << 2)), 0x9f);
  reg_wr((SD0_CFG_BASE+ (0xA44 << 2)), 0x01);
  reg_wr((SD0_CFG_BASE+ (0xA45 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0xA46 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0xA47 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0xA48 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0xA49 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0xA4A << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0xA4B << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0xA4C << 2)), 0x30);
  reg_wr((SD0_CFG_BASE+ (0xA4D << 2)), 0x41);
  reg_wr((SD0_CFG_BASE+ (0xA4E << 2)), 0x7e);
  reg_wr((SD0_CFG_BASE+ (0xA4F << 2)), 0xd0);
  reg_wr((SD0_CFG_BASE+ (0xA50 << 2)), 0xcc);
  reg_wr((SD0_CFG_BASE+ (0xA51 << 2)), 0x85);
  reg_wr((SD0_CFG_BASE+ (0xA52 << 2)), 0x52);
  reg_wr((SD0_CFG_BASE+ (0xA53 << 2)), 0x93);
  reg_wr((SD0_CFG_BASE+ (0xA54 << 2)), 0xe0);
  reg_wr((SD0_CFG_BASE+ (0xA55 << 2)), 0x49);
  reg_wr((SD0_CFG_BASE+ (0xA56 << 2)), 0xdd);
  reg_wr((SD0_CFG_BASE+ (0xA57 << 2)), 0xb0);
  reg_wr((SD0_CFG_BASE+ (0xA58 << 2)), 0x0b);
  reg_wr((SD0_CFG_BASE+ (0xA59 << 2)), 0x02);
  reg_wr((SD0_CFG_BASE+ (0xA5A << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0xA5B << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0xA5C << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0xA5D << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0xA5E << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0xA5F << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0xA60 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0xA61 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0xA62 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0xA63 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0xA64 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0xA65 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0xA66 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0xA67 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0xA68 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0xA69 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0xA6A << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0xA6B << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0xA6C << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0xA6D << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0xA6E << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0xA6F << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0xA70 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0xA71 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0xA72 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0xA73 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0xA74 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0xA75 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0xA76 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0xA77 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0xA78 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0xA79 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0xA7A << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0xA7B << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0xA7C << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0xA7D << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0xA7E << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0xA7F << 2)), 0xd8);
  reg_wr((SD0_CFG_BASE+ (0xA80 << 2)), 0x1a);
  reg_wr((SD0_CFG_BASE+ (0xA81 << 2)), 0xff);
  reg_wr((SD0_CFG_BASE+ (0xA82 << 2)), 0x01);
  reg_wr((SD0_CFG_BASE+ (0xA83 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0xA84 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0xA85 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0xA86 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0xA87 << 2)), 0xf0);
  reg_wr((SD0_CFG_BASE+ (0xA88 << 2)), 0xff);
  reg_wr((SD0_CFG_BASE+ (0xA89 << 2)), 0xff);
  reg_wr((SD0_CFG_BASE+ (0xA8A << 2)), 0xff);
  reg_wr((SD0_CFG_BASE+ (0xA8B << 2)), 0xff);
  reg_wr((SD0_CFG_BASE+ (0xA8C << 2)), 0x1c);
  reg_wr((SD0_CFG_BASE+ (0xA8D << 2)), 0xc2);
  reg_wr((SD0_CFG_BASE+ (0xA8E << 2)), 0xc3);
  reg_wr((SD0_CFG_BASE+ (0xA8F << 2)), 0x3f);
  reg_wr((SD0_CFG_BASE+ (0xA90 << 2)), 0x0a);
  reg_wr((SD0_CFG_BASE+ (0xA91 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0xA92 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0xA93 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0xA94 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0xA95 << 2)), 0x00);
  reg_wr((SD0_CFG_BASE+ (0xA96 << 2)), 0xf8);

  reg_wr((SD0_CFG_BASE+ (0x000 << 2) ), 0x07);



  if(debug)
   printf("Completed PCIE0 Serdes Initialization!\n");
 }

 // Serdes-1(SD1) Initialization
 if(((forPCIE) && ((PCIE_num == 1) || (PCIE_num == 2) || (PCIE_num == 3))) || ((forSATA) && ((SATA_num == 0) || (SATA_num == 2) || (SATA_num == 3))))
   printf("ERROR - trying to configure Serdes #1!!!!\n");

 // Serdes-2(SD2) Initialization
 if(((forSATA) && ((SATA_num == 1) || (SATA_num == 2) || (SATA_num == 3))) || (forSGMII))
   printf("ERROR - trying to configure Serdes #2!!!!\n");

// reset pcie



 return;
}



//***************************************************************************************************************************
// Method to wait for the specified configured Snowbush PHY (Serdes) to issue it's CMU-OK, and it's Lane to become Ready after releasing the CMU & Lane Resets
// Input Parameters: sbphy_num - Specifies the instance(s) of the Serdes for which we are waiting the lane(s) to get ready (0 - SD1, 1 - SD1, 2 -SD2, 3- SD0 & SD1, 4 - SD1 & SD2)
//                   forPCIE - Specifies if the wait is for Serdes lane corresponding to the PCIE IP
//                   forSATA - Specifies if the wait is for Serdes lane corresponding to the SATA IP
//                   debug - Enables the printing of debug messages
void wait_sb_cmu_lane_rdy(int sbphy_num, bool forPCIE, bool forSATA, bool debug)
{
 U32 SD_Ctrl_Reg_offset;
 U32 rd_data;
 U32 CMU_Reset_mask = 0x00010000;
 U32 Lane_Reset_mask = 0x00000040;
int reset_counter = 0;

 // Input Parameter Check
 if(sbphy_num > 4)
 {
  printf("Error: wait_sb_cmu_lane_rdy - Invalid Snowbush PHY instance(s) option SD#%d specified!\n", sbphy_num);
  return;
 }

 switch(sbphy_num) // Check the Serdes instance/port
 {
  case 0: SD_Ctrl_Reg_offset = 0x34;
          break;
  case 1: SD_Ctrl_Reg_offset = 0x44;
	  break;
  case 2: SD_Ctrl_Reg_offset = 0x54;
	  break;
  case 3: SD_Ctrl_Reg_offset = 0x34;
          break;
  case 4: SD_Ctrl_Reg_offset = 0x44;
          break;
  default: printf("Error: wait_sb_cmu_lane_rdy - Invalid Snowbush PHY option:#%d specified!\n", sbphy_num);
	   return;
 }

 if(debug)
  printf("Releasing the CMU Reset...\n");
 reg_rmw((PHY_CFG_BASE + SD_Ctrl_Reg_offset), CMU_Reset_mask, CMU_Reset_mask);
 if(sbphy_num == 3) // Check if we are also trying to check SD1 for PCIE simultaneously
 {
  SD_Ctrl_Reg_offset = 0x44; // Do the CMU Reset for SD1 also
  reg_rmw((PHY_CFG_BASE + SD_Ctrl_Reg_offset), CMU_Reset_mask, CMU_Reset_mask);
  SD_Ctrl_Reg_offset = 0x34; // Restore back to previous value
 }
 else // Otherwise, check if we are also trying to check SD2 for SATA simultaneously
 {
  if(sbphy_num == 4)
  {
   SD_Ctrl_Reg_offset = 0x54; // Do the CMU Reset for SD2 also
   reg_rmw((PHY_CFG_BASE + SD_Ctrl_Reg_offset), CMU_Reset_mask, CMU_Reset_mask);
   SD_Ctrl_Reg_offset = 0x44; // Restore back to previous value
  }
 }

 // Wait for CMU OK
 if(sbphy_num > 2)
 {
  if(sbphy_num == 3)
  {
   //if(debug)
    printf("Waiting for SD0 & SD1 CMU OK...\n");
   wait_cmu_ok(0, false);
   wait_cmu_ok(1, false);
  }
  else
  {
   //if(debug)
    printf("Waiting for SD1 & SD2 CMU OK...\n");
   wait_cmu_ok(1, false);
   wait_cmu_ok(2, false);
  }
 }
 else
 {
  //if(debug)
   printf("Waiting for CMU OK...\n");
  wait_cmu_ok(sbphy_num, false);
 }


 switch(sbphy_num)
 {
  case 0: if(debug)
           printf("SD0:Configuring the common lane register...\n");
          reg_wr((SD0_CFG_BASE + (SD_COMMON_LANE << 2)), 0xC3);
          break;
  case 1: if(forPCIE)
          {
           if(debug)
            printf("SD1:Configuring the common lane register for PCIE...\n");
           reg_wr((SD1_CFG_BASE + (SD_COMMON_LANE << 2)), 0xC3);
          }
          else
          {
           if(debug)
            printf("SD1:Configuring the common lane register for SATA...\n");
           reg_wr((SD1_CFG_BASE + (SD_COMMON_LANE << 2)), 0x03);
          }
          break;
  case 2: if(forSATA)
          {
           if(debug)
            printf("SD2:Configuring the common lane register for SATA...\n");
          }
          else
          {
           if(debug)
            printf("SD2:Configuring the common lane register for SGMII...\n");
          }
          reg_wr((SD2_CFG_BASE + (SD_COMMON_LANE << 2)), 0x03);
          break;
  case 3: if(debug)
            //changed  by mahendra
           printf("Configuring the common lane registers for SD0 & SD1 for PCIE...\n");
       //   reg_wr((SD0_CFG_BASE + (SD_COMMON_LANE << 2)), 0x0);
       //   reg_wr((SD1_CFG_BASE + (SD_COMMON_LANE << 2)), 0x0);
          break;
  case 4: if(debug)
           printf("Configuring the common lane registers for SD1 & SD2 for SATA...\n");
          reg_wr((SD1_CFG_BASE + (SD_COMMON_LANE << 2)), 0x03);
          reg_wr((SD2_CFG_BASE + (SD_COMMON_LANE << 2)), 0x03);
          break;
  default: printf("Error: wait_sb_cmu_lane_rdy - Invalid Snowbush PHY intsance SD#%d specified!\n", sbphy_num);
          return;
 }




  printf("reset mpcie 1\n");
  reg_rmw((GPIO_BASE+ 0x0),0x08000000, 0x08000000);


//if(debug)
//   printf("Getting read to Releasing the Lane Reset...Addr = %x, data=%x \n", (PHY_CFG_BASE+SD_Ctrl_Reg_offset), Lane_Reset_mask);
 //nop(400000000);
 if(debug)
   printf("Releasing the Lane Reset...Addr = %x, data=%x \n", (PHY_CFG_BASE+SD_Ctrl_Reg_offset), Lane_Reset_mask);
 reg_rmw((PHY_CFG_BASE + SD_Ctrl_Reg_offset), Lane_Reset_mask, Lane_Reset_mask);


//nop(4000000);


 // ##################### BEDROS #####################
 //nop(400000000);
  /*for (reset_counter=0;reset_counter < 5;reset_counter++)
 {
    printf("reset mpcie 1\n");
    reg_rmw((GPIO_BASE+ 0x0),0x08000000, 0x08000000);
    //nop(40000000);

    rd_data = reg_rd (DWC_STS_REG0);
    if ((rd_data & 0x10000) == 0x10000) {
      reg_rmw(DWC_CFG_REG5, 0x00000200, 0x0);
      printf("PCIe0 - link is UP!! STS_REG0=0x%x\n",rd_data);
    }

    nop(400000000);
    printf("reset mpcie 0\n");
    reg_rmw((GPIO_BASE+ 0x0),0x08000000, 0x0);
    nop(400000000);

 }
  */
/*
  for (reset_counter=0;reset_counter < 5;reset_counter++)
 {

    printf("reset Link CTRL bit 5 = 1\n");
    reg_rmw( PCIE0_LCNT_REG , 0x00000020 , 0x00000020 );

    rd_data = reg_rd (DWC_STS_REG0);
    if ((rd_data & 0x10000) == 0x10000) {
      reg_rmw(DWC_CFG_REG5, 0x00000200, 0x0);
      printf("PCIe0 - link is UP!! STS_REG0=0x%x\n",rd_data);
    }

    nop(400000000);
    printf("reset Link CTRL bit 5 = 0\n");
    reg_rmw( PCIE0_LCNT_REG , 0x00000020 , 0x00000000 );
    nop(400000000);

 }
*/


// ############################# added to force G1 #################
 //Ack Frequency and L0-L1 ASPM Control Register
 rd_data = reg_rd(PCIE0_AFL0L1_REG);
 printf("A=0x%x d=0x%x \n", PCIE0_AFL0L1_REG, rd_data);

 reg_rmw( PCIE0_AFL0L1_REG , 0x00FFFF00 , 0x00F1F100 );


 //Gen2 Control Control Register
 rd_data = reg_rd(PCIE0_G2CTRL_REG);
 printf("A=0x%x d=0x%x \n", PCIE0_G2CTRL_REG, rd_data);

 reg_rmw( PCIE0_G2CTRL_REG , 0x000000FF , 0x000000F1 );

 //Symbol Number Register
 rd_data = reg_rd(PCIE0_SYMNUM_REG);
 printf("Addr = 0x%x rd=0x%x \n", PCIE0_SYMNUM_REG, rd_data);


// ####################################################################
 // moved to after lane reset
 printf("reset mpcie 1\n");
 reg_rmw((GPIO_BASE+ 0x0),0x08000000, 0x08000000);

//nop(400000000);
nop(400000);
 if(sbphy_num == 3) // Check if we are also trying to check SD1 for PCIE simultaneously
 {
  SD_Ctrl_Reg_offset = 0x44; // Do the Lane Reset for SD1 also
 // reg_rmw((PHY_CFG_BASE + SD_Ctrl_Reg_offset), Lane_Reset_mask, Lane_Reset_mask); mahendraa
 }
 else // Otherwise, check if we are also trying to check SD2 for SATA simultaneously
 {
  if(sbphy_num == 4)
  {
   SD_Ctrl_Reg_offset = 0x54; // Do the Lane Reset for SD2 also
   reg_rmw((PHY_CFG_BASE + SD_Ctrl_Reg_offset), Lane_Reset_mask, Lane_Reset_mask);
  }
 }

 // Wait for the Lane Ready
 if(sbphy_num == 4)
 {
  //if(debug)
   printf("Waiting for SD1 & SD2 Lane Ready...\n");
  wait_lane_ok(1, false);
  wait_lane_ok(2, false);
 }
 else
 {
   if(!forPCIE){
    //if(debug)
    printf("Waiting for the Lane Ready...\n");
    wait_lane_ok(sbphy_num, false);
   }
 }

 switch(sbphy_num)
 {
  case 0: //if(debug)
    rd_data = reg_rd (DWC_STS_REG0);
    if ((rd_data & 0x10000) == 0x10000) {
      reg_rmw(DWC_CFG_REG5, 0x00000200, 0x0);
      printf("PCIe0 - link is UP!! STS_REG0=0x%x\n",rd_data);
    } else {
      printf("PCIe0 - link is DOWN!! STS_REG0=0x%x\n",rd_data);
    }
    //printf("Serdes-0 PHY is Ready!\n");
    break;
  case 1: //if(debug)
           printf("Serdes-1 PHY is Ready!\n");
          break;
  case 2: //if(debug)
           printf("Serdes-2 PHY is Ready!\n");
          break;
  case 3: //if(debug)
           printf("Serdes-0 & 1 PHY are Ready!\n");
          break;
  case 4: //if(debug)
           printf("Serdes-1 & 2 PHY are Ready!\n");
          break;
  default: printf("Error: wait_sb_cmu_lane_rdy - Invalid Snowbush PHY instance option SD#%d specified!\n", sbphy_num);
           return;
 }
 return;
}





//***************************************************************************************************************************
// Method Initialize the Snowbush PHY (Serdes) for operation with the one of the PCIE,SATA or SGMII IP blocks, and then waiting until it issue it's CMU-OK, and it's Lane to become Ready after releasing the CMU & Lane Resets
// Input Parameters: forPCIE - Specifies if the Serdes port/lanes corresponding to the PCIE IP are to be initialized
//                   forSATA - Specifies if the Serdes port/lanes corresponding to the SATA IP are to be initialized
//                   forSGMII - Specifies if the Serdes port/lanes corresponding to the SGMII are to be initialized
//                   PCIE_num - Specifies the specific PCIE IP instance, for which the port/lanes are to be initialized (0: PCIE0, 1:PCIE1, 2 or 3: Both PCIE0 & PCIE1)
//                   SATA_num - Specifies the specific SATA IP Port, for which the port/lanes are to be initialized (0:SATA0, 1:SATA1, 2 or 3: Both SATA0 & SATA1)
//                   sata_p0_gen1 - Specifies that the Serdes Instance-1 for SATA Port0 is to be configured for Gen-I (1.5Gbps) operation [Only applicable when forSATA:true & SATA_num: 0,2 or 3]
//                   sata_p1_gen1 - Specifies that the Serdes Instance-2 for SATA Port1 is to be configured for Gen-I (1.5Gbps) operation [Only applicable when forSATA:true & SATA_num: 1,2 or 3]
//                   debug - Enables the printing of Debug messages
void sbphy_init_wait_lane_rdy(bool forPCIE, bool forSATA, bool forSGMII, int PCIE_num, int SATA_num, bool sata_p0_gen1, bool sata_p1_gen1, bool debug)
{
 int sbphy_num;

 // Parameter Checks
 if((forPCIE) && (PCIE_num > 3))
 {
  printf("Error: sbphy_init_wait_lane_rdy - Invalid PCIE Instance#%d was specified!\n", PCIE_num);
  return;
 }
 if((forSATA) && (SATA_num > 3))
 {
  printf("Error: sbphy_init_wait_lane_rdy - Invalid SATA Port#%d was specified!\n", SATA_num);
  return;
 }
 if(((forSATA) && (forPCIE) && ((PCIE_num == 0) || (SATA_num == 1))) || ((forSATA) && (forSGMII) && (SATA_num == 0)) || ((forPCIE) && (forSGMII))) // Check that we are only trying to initialize the Serdes for "one" of the ports/lanes, when we specify the lane for more than one Controller
 {
  printf("Error: sbphy_init_wait_lane_rdy - Method only permits the initialization of 'one' of the Serdes instances!\n");
  return;
 }

 // Perform the Initialization of the specified Snowbush PHY (Serdes) Port/Lanes
 init_sbphy(forPCIE, forSATA, forSGMII, PCIE_num, SATA_num, sata_p0_gen1, sata_p1_gen1, debug);

 // Calculation of the Serdes Instance
 if((forPCIE) && (PCIE_num == 0))
 {
  sbphy_num = 0; // Serdes-0
  printf("Serdes-0 PHY has been configured!\n");
 }
 else
 {
  if(((forPCIE) && (PCIE_num == 1)) || ((forSATA) && (SATA_num == 0)))
  {
   sbphy_num = 1; // Serdes-1
   printf("Serdes-1 PHY has been configured!\n");
  }
  else
  {
   if(((forSATA) && (SATA_num == 1)) || (forSGMII))
   {
    sbphy_num = 2; // Serdes-2
    printf("Serdes-2 PHY has been configured!\n");
   }
   else // Special cases where we are trying to initialize both SATA or both PCIE Serdes lanes simultaneously
   {
    if(((forSATA) && ((SATA_num == 2) || (SATA_num == 3))) || ((forPCIE) && ((PCIE_num == 2) || (PCIE_num == 3))))
    {
     if(((forSATA) && ((SATA_num == 2) || (SATA_num == 3))) && ((forPCIE) && ((PCIE_num == 2) || (PCIE_num == 3))))
     {
      printf("Error: sbphy_init_wait_lane_rdy - Method only permits the initialization of both PCIE OR both SATA Serdes lanes!\n");
      return;
     }
     else
     {
      if((forPCIE) && ((PCIE_num == 2) || (PCIE_num == 3)))
      {
       sbphy_num = 3; // Both PCIE Instances
       printf("Serdes 0 & 1 PHY have been configured!\n");
      }
      else
      {
       sbphy_num = 4; // Both SATA Instances
       printf("Serdes 1 & 2 PHY have been configured!\n");
      }
     }
    }
   }
  }
 }

 // Wait for the corresponding initialized Serdes Port/Lane to become Ready
 // ################################## BEDROS ##########################

 // To take the external card out of reset configure GPIO27 to be an output (to drive 0)
 //Configure  GPIO  output Enable
printf("reset mpcie 0\n");
reg_rmw((GPIO_BASE+ 0x0),0x08000000, 0x0);
// 0x4 is for output enable
reg_rmw((GPIO_BASE+ 0x4),0x08000000, 0x08000000);
 // Configure  GPIO Output register---GPIO_OUT  to be 1
 // Making   WifiCard reset is Active High to Active WiFiCard
//printf("reset mpcie 1");
//reg_rmw((GPIO_BASE+ 0x0),0x08000000, 0x08000000); //dror

 //####################################################################
 wait_sb_cmu_lane_rdy(sbphy_num, forPCIE, forSATA, true);
 return;
}
