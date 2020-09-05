// Define the Base Addresses of the SB-PHY Configuration Space
// (from Memory Map)
//  for SD0 --> 0x9059_0000
//              0x9059_3FFF(0x2BFF)
//

#define SD0_CFG_BASE  0x90590000
#define SD1_CFG_BASE  0x90594000
#define SD2_CFG_BASE  0x90598000
#define PHY_CFG_BASE  0x90410000
#define SD0_COMMON_CMU     0x000
#define SD0_LANE0          0x200
#define SD0_LANE1          0x400
#define SD0_LANE2          0x600
#define SD0_LANE3          0x800
#define SD0_COMMON_LANE    0xA00
#define SD1_COMMON_CMU     0x000
#define SD1_LANE0          0x200
#define SD1_LANE1          0x400
#define SD1_LANE2          0x600
#define SD1_LANE3          0x800
#define SD1_COMMON_LANE    0xA00
#define SD2_COMMON_CMU     0x000
#define SD2_LANE0          0x200
#define SD2_LANE1          0x400
#define SD2_LANE2          0x600
#define SD2_LANE3          0x800
#define SD2_COMMON_LANE    0xA00

// Method to wait for the 'CMU OK' to be signaled by the Snowbush Serdes PHY
// Input Parameter: debug - Enables the printing of Debug messages
void wait_sd0_cmu_ok(bool debug)
{
 U32 rd_data = 0x00000000;
 U32 cmu_ok_dtctd_mask = 0x00004000;
 U32 masked_data;
 int CMU_Offset = 0x2C;  // 10'h00F
 bool entry = true;

 do // Keep looping until you see the cmu_ok_o of Serdes
 { /*
  if(entry == false) // Check this is not the first-time in the loop
  {
   // Wait for some time before you perform the next read
   nop(1); // Have the ARM issue some NOPs
  } */
  rd_data = reg_rd(PHY_CFG_BASE + CMU_Offset);
  if(debug) 
   printf("SD0 Ctrl Reg:0x%x\n", rd_data);
  entry = false; // The first entry into the loop has been completed
  masked_data = rd_data & cmu_ok_dtctd_mask; // Bit-wise AND the read-data with the Mask so that we are only left with the bit-position corresponding to cmu_ok Status Pending
  if((masked_data == cmu_ok_dtctd_mask) && debug)
   printf("SD0 CMU OK Dtctd\n");
 }
 while(masked_data != cmu_ok_dtctd_mask);
 return;
}

void wait_sd1_cmu_ok(bool debug)
{
 U32 rd_data = 0x00000000;
 U32 cmu_ok_dtctd_mask = 0x00004000;
 U32 masked_data;
 int CMU_Offset = 0x3C;  // 10'h00F
 bool entry = true;

 do // Keep looping until you see the cmu_ok_o of Serdes
 { /*
  if(entry == false) // Check this is not the first-time in the loop
  {
   // Wait for some time before you perform the next read
   nop(1); // Have the ARM issue some NOPs
  } */
  rd_data = reg_rd(PHY_CFG_BASE + CMU_Offset);
  if(debug) 
   printf("SD1 Ctrl Reg:0x%x\n", rd_data);
  entry = false; // The first entry into the loop has been completed
  masked_data = rd_data & cmu_ok_dtctd_mask; // Bit-wise AND the read-data with the Mask so that we are only left with the bit-position corresponding to cmu_ok Status Pending
  if((masked_data == cmu_ok_dtctd_mask) && debug)
   printf("SD1 CMU OK Dtctd\n");
 }
 while(masked_data != cmu_ok_dtctd_mask);
 return;
}
