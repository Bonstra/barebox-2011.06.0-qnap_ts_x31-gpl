//*********************************************************************************//
//Testcase Name:usb3_as_ss_host_on_board1_and_usb3_as_flash_drive_device
//Author: shaikm
//Objective:
//The objective of this test case to configure the usb3 controller
//on board1 in SuperSpeed host mode and perform control transfer handshaking
//(Device Slot,Set Address,Configure Endpoint commands) with connected
//usb3 flash drive device.
//*********************************************************************************//
#include <init.h>
#include <common.h>
#include <asm/io.h>
#include "usb3/DWC_usb3_params.h"
#include "usb3/usb3subsys_registers.h"
#include "usb3/test_defines.h"
#include "usb3subsys_func.h"
#include "usb3_phy_ber_test_defines.h"
#include "common_func.h"
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$//
 //Global variables:
 //
  int  usb3_i=0;
  u32  usb3_total_num_of_bulkout_bytes=0;
  int  usb3_host_num_of_bulkout_trbs=0;
  u32  usb3_host_num_of_bulkout_bytes_per_trb=0;
  u32  usb3_total_num_of_bulkin_bytes=0;
  int  usb3_host_num_of_bulkin_trbs=0;
  u32  usb3_host_num_of_bulkin_bytes_per_trb=0;
  u32  usb3_step=0;
  u32  usb3_read_data=0;
  u32  usb3_read_data1=0;
  u32  usb3_read_data2=0;
  u32  usb3_read_data3=0;
  u32  usb3_start_data=0;
  u32  usb3_caplength;
  u32  usb3_fail=0;
  u32  usb3_first=0;
  u32  usb3_xHC_data_addr=0;
  u32  usb3_xHC_Transfer_TRB_addr=0;
  u32  usb3_xHC_Command_TRB_addr=0;
  u32  usb3_xHC_Event_TRB_addr=0;
  u32  usb3_xHC_register_base=0;
  u32  usb3_xHC_DCBAR_addr=0;
  u32  usb3_xHC_Device_Context_addr=0;
  u32  usb3_xHC_Input_Context_addr=0;
  u32  usb3_xHC_Scratch_Pad_Array_addr=0;
  u32  usb3_xHC_Scratch_Pad_buf0_addr=0;
  u32  usb3_xHC_EventRing_Segment_addr=0;
  u32  usb3_xHC_EventRing_Segment_table_addr=0;
  u32  usb3_xHC_CommandRing_Segment_addr=0;
  u32  usb3_xHC_TransferRing_Segment_addr=0;
  u32  usb3_xHC_Data_BulkOut_Buffer_addr=0;
  u32  usb3_xHC_Data_BulkIn_Buffer_addr=0;
  u32  usb3_Runtime_Base_addr=0;
  u32  usb3_Doorbell_Array_Base_addr;
  u32  usb3_ERDP=0;
  u32  usb3_PortID=0;
  u32  usb3_SlotID=0;
  u32  usb3_Event_TRBs=0;
  u32  usb3_PortSpeed=0;
  u32  usb3_CNR=0;
  u32  usb3_hst_event_count=0;
  u32  usb3_ss_port_number=0;
  u32  usb3_ss_port_addr=0;
  u32  usb3_hs_port_number=0;
  u32  usb3_hs_port_addr=0;
  u32  pmu_intr_status=0;
  u32  a9_irq_bus_31_0_bits_status=0;
  u32  a9_irq_bus_63_32_bits_status=0;

  int device_connected_on_ss_port=0;
  int device_slots_cmd_pass=0;
  int set_addr_cmd_pass=0;
  int config_end_point_cmd_pass=0;

//************************************************************************************************************
//This file contains the usb3 host mode functions
#include "usb3/usb3_test_func.h"
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&//

int Comcerto_usb3_test (struct diags_test_param *p)
{
  u32 debug=0;
  //[0.0]::APPLYING THE RESET TO USB3 UTMI,USB3 PHY,USB2 UTMI,USB2 PHY
  writel(0x33,0x904B000C);
  //[0.1]APLLYING RESET TO USB3 AXI RESET ,USB2 RESET
  //[4]Bit-When Set to 1,USB1(USB3) Controller's AXI clock domain is in Reset +
  //[3]Bit-When Set to 1, USB0(USB2) Controller's AXI clock domain is in Reset.
  writel(0x18,0x904B0058);
  //************ USB3 POR SEQUENCE ***********************//
  //[1] Initializing the USB3 phy registers
  //    while usb3 controller and phy are in reset
  usb3subsys_phy_init();
  //[2]::RELEASING POR TO USB3 PHY
  writel(0x23,0x904B000C);
  //writel(0x20,0x904B000C);
  nop(8000000);//Delay
  //[3]::RELEASING UTMI RST TO USB3 CONTROLLER
  writel(0x03,0x904B000C);
  //writel(0x00,0x904B000C);
  nop(1000000);//Delay
  //[4]::RELEASING THE USB3 CONTROLLER AXI CLOCK DOMAIN RESET
  //Bit[4]-USB3,Bit[3]-USB2
  writel(0x08,0x904B0058);
  //writel(0x00,0x904B0058);
  nop(1000000);//Delay
  //Initializing the xHC (USB3 controller & configuring as a Host
  //Initializing Global registers
  usb3_controller_global_regs_init();
  //USB3 constants initialization
  usb3_constants_init();
  //Wait for xHC controller ready before writing to its register space
  usb3_xHC_controller_init();
  //Configure xHc controller registers for Host programming.
  usb3_xHC_controller_regs_config();
  //Initialize xHC Device Context to '0'
  usb3_xHC_device_context_init();
  //Initialize the xHC Event Ring Segments to '0'
  usb3_xHC_event_TRBs_init();
  //Initialize CommandRing Segments to 0
  usb3_xHC_command_TRBs_init();
  //Initialize TransferRing Segments to 0
  usb3_xHC_transfer_TRBs_init();
  //Initializing the xHC device context data structure
  usb3_xHC_device_context_datastructure_init();
  //Declaring the xHC scratchpad buffer pointer.
  usb3_xHC_scratchpad_buffer_datastructure_init();
  //Declaring the xHC Event Ring Segement Table
  usb3_xHC_eventring_datastructure_init();
  //Initializing the xHC interrupter registers
  usb3_xHC_interrupter_regs_init();
  //Enabling the RUN bit on xHC-USBCMD register to excecute outstanding commands
  usb3_xHC_enable_run_mode();

  printf("Waiting for USB3 host controller to drive VBUS to high ");
  usb3_wait_untill_phy_vbus_is_high();
  printf("...done\n");
  printf("USB3 Controller initialization done\n");
  nop(1000000);//Delay

  printf("Polling for 'Device detection' interrupt from USB3 host controller\n");
#if 0
  usb3_read_data=reg_rd((USB3p0_PHY_CFG_BASE_ADDR+0x30));
  printf("USB3PHY STATUS REG @ A:%x,RdD:%x\n",(USB3p0_PHY_CFG_BASE_ADDR+0x30),usb3_read_data);
  usb3_read_data=reg_rd((USB3p0_CFG_BASE_ADDR+usb3_caplength+0x400));
  printf("USB3 HS PORTSC  REG @ A:%x,RdD:%x\n",(USB3p0_CFG_BASE_ADDR+usb3_caplength+0x400),usb3_read_data);
  usb3_read_data=reg_rd((USB3p0_CFG_BASE_ADDR+usb3_caplength+0x410));
  printf("USB3 SS PORTSC  REG @ A:%x,RdD:%x\n",(USB3p0_CFG_BASE_ADDR+usb3_caplength+0x410),usb3_read_data);
  printf("USB3:WAITING FOR CONNECTION DONE INTERRUPT\n");
#endif
  printf("-----------------------------------------------\n");
  printf("Connect the USB3 device or Press Ctrl-c to exit\n");
  printf("-----------------------------------------------\n");
  if(usb3_wait_for_interrupt())
	return -1;
#if 0
  usb3_read_data=reg_rd((USB3p0_CFG_BASE_ADDR+usb3_caplength+0x400));
  printf("USB3 HS PORTSC  REG @ A:%x,RdD:%x\n",(USB3p0_CFG_BASE_ADDR+usb3_caplength+0x400),usb3_read_data);
  usb3_read_data=reg_rd((USB3p0_CFG_BASE_ADDR+usb3_caplength+0x410));
  printf("USB3 SS PORTSC  REG @ A:%x,RdD:%x\n",(USB3p0_CFG_BASE_ADDR+usb3_caplength+0x410),usb3_read_data);
#endif
  printf("\n\n DEVICE CONNECTION DONE INTERRUPT DETECTED ON USB3 CONTROLLER \n\n");
  printf("Waiting for handshaking to complete with the USB3 device...\n\n");
//  printf("Check the port status change event(completion code,cycle bit,TRB type) generated from USB3 host controller\n");
  //Acknowledge the port status change interrupt;Read Event interrupt type.
  if(usb3_xHC_ack_port_status_change_interrupt())
	return -1;

  //Clear the xHC event interrupts
  usb3_xHC_clear_event_interupt();

  nop(1000000);//Delay
  //Clearing the PMU status register interrupts
  gpio_interrupt_status_register_clear();
  nop(1000000);
  //Check whether xHC is in Enable or Disable state.
  usb3_xHC_state_check();

  nop(1000000);//Delay
  //Enable xHC Device slot
  usb3_xHC_enable_device_slot();

  nop(1000000);//Delay
  //Ring xHC Doorbell with "Host Controller Command"
  usb3_xHC_ring_doorbell_with_host_control_cmd();

  nop(1000000);//Delay
  //Wait for interrupt -for device slot availability
  usb3_wait_for_interrupt();

  nop(1000000);//Delay
  //Check the status of cyclic bit,TRB type,slotID after 'Device enable slot' command completion
  usb3_xHC_check_for_device_slot();

  nop(1000000);//Delay
  //Clear the xHC event interrupts
  usb3_xHC_clear_event_interupt();

  nop(1000000);//Delay
  //Clearing the PMU status register interrupts
  gpio_interrupt_status_register_clear();
  nop(1000000);//Delay
  usb3_host_get_ss_port_status();
  nop(1000000);//Delay
  //Issue xHC set address device  command
  usb3_xHC_issue_addr_device_cmd();

  nop(1000000);//Delay
  //Ring xHC Doorbell with "Host Controller Command"
  usb3_xHC_ring_doorbell_with_host_control_cmd();

  nop(1000000);//Delay
  usb3_host_get_ss_port_status();
  //Wait for interrupt-on command Completion Event TRB
  usb3_wait_for_interrupt();

  nop(1000000);//Delay
  //Check the status of cyclic bit,TRB type,slotID after 'set address' command completion
  usb3_xHC_check_cmd_completion_status();

  nop(1000000);//Delay
  //Clear the xHC event interrupts
  usb3_xHC_clear_event_interupt();

  nop(1000000);//Delay
  //Clearing the PMU status register interrupts
  gpio_interrupt_status_register_clear();
  nop(1000000);//Delay
  //Issue xHC configure endpoint command
  usb3_xHC_issue_configure_endpoint_cmd();

  nop(1000000);//Delay
  //Ring xHC Doorbell with "Host Controller Command"
  usb3_xHC_ring_doorbell_with_host_control_cmd();

  nop(1000000);//Delay
  //Wait for event interrupt -on configure end point command completion
  usb3_wait_for_interrupt();

  nop(1000000);//Delay
  //Check the status of cyclic bit,TRB type,slotID after 'configure endpoint' command completion
  usb3_xHC_check_cmd_completion_status();

  nop(1000000);//Delay
  //Clear the xHC event interrupts
  usb3_xHC_clear_event_interupt();

  nop(1000000);//Delay
  //Clearing the PMU status register interrupts
  gpio_interrupt_status_register_clear();
  nop(100000);
  if(check_host_to_device_handshaking_result())
	return -1;

  return 0;
} //main





int Comcerto_usb3_phy_ber_test (struct diags_test_param *p)
{
	int ret = -1;
  U32 err_cnt=0;
  U32 marker_code=0x900d0000;
  U32 read_data=0;
  U32 exp_read_data=0;

printf("\nusb3p0_phy_ber_test....\n");

  printf("ST:\n");
  //------------------------------------------------------------//
  //printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
  //printf("[1]: $$ Continuing resets on usb3,usb2 controller+phy blocks $$\n");
  //printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
  //[0.0]::APPLYING THE RESET TO USB3 UTMI,USB3 PHY,USB2 UTMI,USB2 PHY
  reg_wr(0x904B000C, 0x33);
  printf("USB_RST_CNTRL @ A:%x,WrD:%x\n",0x904B000C,0x33);
  printf("USB3_UTMI_RST:ON,USB3_PHY_RST:ON,USB2_UTMI_RST:ON,USB2_PHY_RST:ON\n");
  //[0.1]APLLYING RESET TO USB3 AXI RESET ,USB2 RESET
  //[4]Bit-When Set to 1,USB1(USB3) Controller's AXI clock domain is in Reset +
  //[3]Bit-When Set to 1, USB0(USB2) Controller's AXI clock domain is in Reset.
  reg_wr(0x904B0058, 0x18);
  printf("AXI_RESET_2 @ A:%x,WrD:%x\n",0x904B0058,0x18);
  printf("USB3_AXI_RST:ON,USB2_HRESET:ON\n");
  //------------------------------------------------------------//
  //printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
  //printf("[2.1]: $$ Initializing usb3 phy & controller top level registers while phy &
  //                  controller are in reset $$ \n");
  //printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
  //[1] Initializing the USB3 phy registers
  //    while usb3 controller and phy are in reset
  //----------------------------------------------------------------
  //Configuring the PHY Control Register#0 w.r.t dump.
  //(M) reg_wr(USB3p0_PHY_CNTRL_REG0_ADDR, 0xCAD159AA); //For external 24MHz clock
  reg_wr(USB3p0_PHY_CNTRL_REG0_ADDR, 0xCAD159A9); //For internal 48MHz clock
  printf("A:%x,WrD:%x\n",USB3p0_PHY_CNTRL_REG0_ADDR, 0xCAD159A9); //For internal 48MHz clock
  //Configuring the PHY Control Register#1 w.r.t dump.
  reg_wr(USB3p0_PHY_CNTRL_REG1_ADDR, 0x95CFBC38);
  printf("A:%x,WrD:%x\n",USB3p0_PHY_CNTRL_REG1_ADDR, 0x95CFBC38);
  //Configuring the PHY Control Register#2 w.r.t dump.
  reg_wr(USB3p0_PHY_CNTRL_REG2_ADDR, 0xA9A15555);
  printf("A:%x,WrD:%x\n",USB3p0_PHY_CNTRL_REG2_ADDR, 0xA9A15555);
  //Configuring the PHY Control Register#3 w.r.t dump.
  reg_wr(USB3p0_PHY_CNTRL_REG3_ADDR, 0x0000A001);
  printf("A:%x,WrD:%x\n",USB3p0_PHY_CNTRL_REG3_ADDR, 0x0000A001);
  //----------------------------------------------------------------
  nop(100000);//Delay
  //----------------------------------------------------------------
  //printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
  //printf("[3]: $$ Releasing the POR (Power on Reset) to usb3 phy $$ \n");
  //printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
  //[2]::RELEASING POR TO USB3 PHY
  reg_wr(0x904B000C, 0x23);
  nop(100000);//Delay
  //reg_wr(0x904B000C, 0x20);
  printf("USB_RST_CNTRL @ A:%x,WrD:%x\n",0x904B000C,0x23);
  printf("USB3_UTMI_RST:ON,USB3_PHY_RST:OFF,USB2_UTMI_RST:ON,USB2_PHY_RST:ON\n");
  nop(8000000);//Delay
  //printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
  //printf("[4]: $$ Releasing the UTMI reset to usb3 controller $$ \n");
  //printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
  //[3]::RELEASING UTMI RST TO USB3 CONTROLLER
  reg_wr(0x904B000C, 0x03);
  nop(100000);//Delay
  //reg_wr(0x904B000C, 0x00);
  printf("USB_RST_CNTRL @ A:%x,WrD:%x\n",0x904B000C,0x03);
  printf("USB3_UTMI_RST:OFF,USB3_PHY_RST:OFF,USB2_UTMI_RST:ON,USB2_PHY_RST:ON\n");
  nop(1000000);//Delay
  //printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
  //printf("[5]: $$ Releasing the AXI reset to usb3 controller $$ \n");
  //printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
  //[4]::RELEASING THE USB3 CONTROLLER AXI CLOCK DOMAIN RESET
  //Bit[4]-USB3,Bit[3]-USB2
  reg_wr(0x904B0058, 0x08);
  printf("AXI_RESET_2 @ A:%x,WrD:%x\n",0x904B0058,0x08);
  printf("USB3_AXI_RST:OFF,USB2_HRESET:ON \n");
  //nop(1000000);//Delay
  nop(100000);//Delay
  //------------------------------------------------------------//
  set_marker(0x900d0001);
  //----------------------------------------------------------------
  // Read default values from the PHY CR registers
  //----------------------------------------------------------------
  // Checking the default of PHY CR IDCODE_LO register
  err_cnt = err_cnt + phy_cr_reg_def_val_check(0x0000,0x64CD);
  // Checking the default of PHY CR IDCODE_HI register
  //(old) err_cnt = err_cnt + phy_cr_reg_def_val_check(0x0001,0x8218);
  err_cnt = err_cnt + phy_cr_reg_def_val_check(0x0001,0x9218);
  if (err_cnt == 0)
    printf("IDCODE CHECK TEST PASS\n");
  else
    printf("IDCODE CHECK TEST FAIL\n");
  //----------------------------------------------------------------
  nop(3000);//Delay
  //----------------------------------------------------------------
  set_marker(0x900d0002);
  //$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$//
  //+INITIALIZE
  //+*************************************************************************
  //+This routine does general initialization of the ssp. It is intended that
  //+this script be run first before any tests. It will set the clock frequency,
  //+turn on the pattern matchers/generators, enable rx, enable tx and invert
  //+the channel data where needed.
  //+At the end of this script the device will be powered up and sending and
  //+receiving lfsr7
  //+**************************************************************************
  //*************************************************************************
  // lbert_mode    = mode of the lbert (tx and rx, integer between 0 and 7)
  //                 0 - disabled
  //                 1 - lfsr31. X^31 + X^28 + 1
  //                 2 - lfsr23. X^23 + X^18 + 1
  //                 3 - lfsr15.  X^15 + X^14 + 1
  //                 4 - lfsr7.  X^7 + X^6 + 1
  //                 5 - fixed word (PAT0)
  //                 6 - DC balanced word (PAT0, ~PAT0)
  //                 7 - Fixed pattern: (000, PAT0, 3ff, ~PAT0)
  // lbert_pattern = pattern for lbert (optional) (integer representing
  //                 binary pattern of 10 bits, 0 to 1023) used for
  //                 lbert_mode 5, 6, and 7
  // tx_lvl        = transmit level (integer value between 0 and 127)
  //                 output amplitude is set by vptx*(1+tx_lvl)/128
  // tx_boost      = transmit boost level (integer value between 0 and 127)
  //                 boost (or EQ or preemphasis) is set by
  //                 tx_eq_ratio = (128 - 2*tx_boost)/128
  //                 tx_eq_dB = -20*log10(tx_eq_ratio)
  //                 for tx_boost = 21, tx_eq_dB is 3.45dB
  // rx_eq         = eq value for rx (integer value between 0 and 7)
  //                 0 is lest EQ and 7 is most EQ.
  // ssc_ppm       = ssc ppm, if 0 ssc is not enabled.
  //*************************************************************************
  // This will reset the device and power the ssp and hsp as specifiecd in the
  // gui. It will override in the digital. It will set the baud rate as what
  // is specified in the part_params
  //**************************************************************************

  //verify the reading writting of registers
  //Verify can write register in IP write to 101010101
  // [11:3]=341
  SSP_CR_LANE0_DIG_RX_SCOPE_CTL_val=((SSP_CR_LANE0_DIG_RX_SCOPE_CTL_val & 0xFFFFF007)|(341<<3));
  phy_cr_write(SSP_CR_LANE0_DIG_RX_SCOPE_CTL,SSP_CR_LANE0_DIG_RX_SCOPE_CTL_val);
  read_data=phy_cr_read(SSP_CR_LANE0_DIG_RX_SCOPE_CTL);
  exp_read_data=0x0aa8;
  if(read_data==exp_read_data)
  {
    printf("Write[1] is Pass\n");
  }
  else
  {
    printf("Write[1] is Fail\n");
    err_cnt++;
    printf("Error:%d\n",err_cnt);
  }

  //Verify can write register in IP write to 010101010
  // [11:3]=170
  SSP_CR_LANE0_DIG_RX_SCOPE_CTL_val=((SSP_CR_LANE0_DIG_RX_SCOPE_CTL_val & 0xFFFFF007)|(170<<3));
  phy_cr_write(SSP_CR_LANE0_DIG_RX_SCOPE_CTL,SSP_CR_LANE0_DIG_RX_SCOPE_CTL_val);
  set_marker(0x900d0003);
  read_data=phy_cr_read(SSP_CR_LANE0_DIG_RX_SCOPE_CTL);
  exp_read_data=0x0550;
  if(read_data==exp_read_data)
  {
    printf("Write[2] is Pass\n");
  }
  else
  {
    printf("Write[2] is Fail\n");
    err_cnt++;
    printf("Error:%d\n",err_cnt);
  }
  //-Set ref_clock divider value to 0 (SUP_DIG_ATEOVRD.ref_clkdiv2)
  // [0:0]
  SSP_CR_SUP_DIG_ATEOVRD_val= ((SSP_CR_SUP_DIG_ATEOVRD_val & 0xFFFFFFFE) | (0x0));
  phy_cr_write(SSP_CR_SUP_DIG_ATEOVRD,SSP_CR_SUP_DIG_ATEOVRD_val);

  //enable usb ref clock from ssp
  //-Enable ref clock to usb(SUP_DIG_ATEOVRD.ref_usb2_en
  //[1:1]
  SSP_CR_SUP_DIG_ATEOVRD_val = ((SSP_CR_SUP_DIG_ATEOVRD_val & 0xFFFFFFFD) | (0x1<<1));
  phy_cr_write(SSP_CR_SUP_DIG_ATEOVRD,SSP_CR_SUP_DIG_ATEOVRD_val);
  //-Enable ATEOVRD (SUP_DIG_ATEOVRD.ateovrd_en)
  //[2:2]
  SSP_CR_SUP_DIG_ATEOVRD_val= ((SSP_CR_SUP_DIG_ATEOVRD_val & 0xFFFFFFFB) | (0x1<<2));
  phy_cr_write(SSP_CR_SUP_DIG_ATEOVRD,SSP_CR_SUP_DIG_ATEOVRD_val);

  //- Set tx_vboost_lvl
  //[15:13]
  SSP_CR_SUP_DIG_MPLL_OVRD_IN_HI_val=((SSP_CR_SUP_DIG_MPLL_OVRD_IN_HI_val & 0xFFFFEFFF)|(0x4<<13));
  phy_cr_write(SSP_CR_SUP_DIG_MPLL_OVRD_IN_HI,SSP_CR_SUP_DIG_MPLL_OVRD_IN_HI_val);

  //- Set los_bias
  //[15:13]
  SSP_CR_SUP_DIG_LEVEL_OVRD_IN_val=((SSP_CR_SUP_DIG_LEVEL_OVRD_IN_val & 0xFFFFEFFF)|(0x5<<13));
  phy_cr_write(SSP_CR_SUP_DIG_LEVEL_OVRD_IN,SSP_CR_SUP_DIG_LEVEL_OVRD_IN_val);

  //- Set los_level
  //[4:0]
  SSP_CR_SUP_DIG_LEVEL_OVRD_IN_val=((SSP_CR_SUP_DIG_LEVEL_OVRD_IN_val & 0xFFFFFFE0)|(0x9));
  phy_cr_write(SSP_CR_SUP_DIG_LEVEL_OVRD_IN,SSP_CR_SUP_DIG_LEVEL_OVRD_IN_val);
  set_marker(0x900d0004);

  //- Set acjt_level
  // [9:5]
  SSP_CR_SUP_DIG_LEVEL_OVRD_IN_val=((SSP_CR_SUP_DIG_LEVEL_OVRD_IN_val & 0xFFFFFC1F)|(13<<5));
  phy_cr_write(SSP_CR_SUP_DIG_LEVEL_OVRD_IN,SSP_CR_SUP_DIG_LEVEL_OVRD_IN_val);

  //- Set enable for tx_vboost_lvl/los_bias/los_level/acjt_level
  //[10:10]
  SSP_CR_SUP_DIG_LEVEL_OVRD_IN_val=((SSP_CR_SUP_DIG_LEVEL_OVRD_IN_val & 0xFFFFFBFF)|(0x1<<10));
  phy_cr_write(SSP_CR_SUP_DIG_LEVEL_OVRD_IN,SSP_CR_SUP_DIG_LEVEL_OVRD_IN_val);

  //- Set tx_vboost_en
  //[8:8]
  SSP_CR_LANEX_DIG_TX_OVRD_IN_HI_val=((SSP_CR_LANEX_DIG_TX_OVRD_IN_HI_val & 0xFFFFFEFF)|(0x1<<8));
  phy_cr_write(SSP_CR_LANEX_DIG_TX_OVRD_IN_HI,SSP_CR_LANEX_DIG_TX_OVRD_IN_HI_val);
  //[9:9]
  SSP_CR_LANEX_DIG_TX_OVRD_IN_HI_val=((SSP_CR_LANEX_DIG_TX_OVRD_IN_HI_val & 0xFFFFFDFF)|(0x1<<9));
  phy_cr_write(SSP_CR_LANEX_DIG_TX_OVRD_IN_HI,SSP_CR_LANEX_DIG_TX_OVRD_IN_HI_val);

  //-Set Tx Reset Low(LANEX_DIG_TX_OVRD_IN_HI.TX_RESET)
  //[6:6]
  SSP_CR_LANEX_DIG_TX_OVRD_IN_HI_val=((SSP_CR_LANEX_DIG_TX_OVRD_IN_HI_val & 0xFFFFFFDF)|(0x1<<6));
  phy_cr_write(SSP_CR_LANEX_DIG_TX_OVRD_IN_HI,SSP_CR_LANEX_DIG_TX_OVRD_IN_HI_val);

  //-Enable override (LANEX_DIG_TX_OVRD_IN_HI.TX_RESET_OVRD)
  //[7:7]
  SSP_CR_LANEX_DIG_TX_OVRD_IN_HI_val=((SSP_CR_LANEX_DIG_TX_OVRD_IN_HI_val & 0xFFFFFF7F)|(0x1<<7));
  phy_cr_write(SSP_CR_LANEX_DIG_TX_OVRD_IN_HI,SSP_CR_LANEX_DIG_TX_OVRD_IN_HI_val);
  set_marker(0x900d0005);

  //-Set Rx reset low(LANEX_DIG_RX_OVRD_IN_HI.RX_RESET)
  //[12:12]
  SSP_CR_LANEX_DIG_RX_OVRD_IN_HI_val=((SSP_CR_LANEX_DIG_RX_OVRD_IN_HI_val & 0xFFFFEFFF)|(0x1<<12));
  phy_cr_write(SSP_CR_LANEX_DIG_RX_OVRD_IN_HI,SSP_CR_LANEX_DIG_RX_OVRD_IN_HI_val);

  //-Enable override (LANEX_DIG_RX_OVRD_IN_HI.RX_RESET_OVRD)
  //[13:13]
  SSP_CR_LANEX_DIG_RX_OVRD_IN_HI_val=((SSP_CR_LANEX_DIG_RX_OVRD_IN_HI_val & 0xFFFFDFFF)|(0x1<<13));
  phy_cr_write(SSP_CR_LANEX_DIG_RX_OVRD_IN_HI,SSP_CR_LANEX_DIG_RX_OVRD_IN_HI_val);

  //turn off the mpll while setting it up
  //-Disable mpll(SUP_DIG_MPLL_OVRD_IN_LO.MPLL_EN)
  //[0:0]
  SSP_CR_SUP_DIG_MPLL_OVRD_IN_LO_val=((SSP_CR_SUP_DIG_MPLL_OVRD_IN_LO_val & 0xFFFFFFFE)|(0x0));
  phy_cr_write(SSP_CR_SUP_DIG_MPLL_OVRD_IN_LO,SSP_CR_SUP_DIG_MPLL_OVRD_IN_LO_val);

  //-Enable override (SUP_DIG_MPLL_OVRD_IN_LO.MPLL_EN_OVRD)
  //[1:1]
  SSP_CR_SUP_DIG_MPLL_OVRD_IN_LO_val=((SSP_CR_SUP_DIG_MPLL_OVRD_IN_LO_val & 0xFFFFFFFD)|(0x1<<1));
  phy_cr_write(SSP_CR_SUP_DIG_MPLL_OVRD_IN_LO,SSP_CR_SUP_DIG_MPLL_OVRD_IN_LO_val);

  //select to remove the fsel from pins
  //-Do not use FSEL override pins for multiplier for ssp (SUP.DIG.MPLL_OVRD_IN_HI.FSEL)
  //[8:6]
  SSP_CR_SUP_DIG_MPLL_OVRD_IN_HI_val=((SSP_CR_SUP_DIG_MPLL_OVRD_IN_HI_val & 0xFFFFFE3F)|(0x0));
  phy_cr_write(SSP_CR_SUP_DIG_MPLL_OVRD_IN_HI,SSP_CR_SUP_DIG_MPLL_OVRD_IN_HI_val);

  //-Enable FSEL override (SUP.DIG.MPLL_OVRD_IN_HI.FSEL_OVRD)
  //[9:9]
  SSP_CR_SUP_DIG_MPLL_OVRD_IN_HI_val=((SSP_CR_SUP_DIG_MPLL_OVRD_IN_HI_val & 0xFFFFFDFF)|(0x1<<9));
  phy_cr_write(SSP_CR_SUP_DIG_MPLL_OVRD_IN_HI,SSP_CR_SUP_DIG_MPLL_OVRD_IN_HI_val);

  //set divide and enable ssp refclk
  //-Set multiplier value to %x (SUP_DIG_MPLL_OVRD_IN_LO.MPLL_MULTIPLIER)", div_val);
  //[8:2]
  SSP_CR_SUP_DIG_MPLL_OVRD_IN_LO_val=((SSP_CR_SUP_DIG_MPLL_OVRD_IN_LO_val & 0xFFFFFE03)|(0x68<<2));
  phy_cr_write(SSP_CR_SUP_DIG_MPLL_OVRD_IN_LO,SSP_CR_SUP_DIG_MPLL_OVRD_IN_LO_val);
  set_marker(0x900d0006);

  //-Enable override (SUP_DIG_MPLL_OVRD_IN_LO.MPLL_MULTIPLIER_OVRD)
  //[9:9]
  SSP_CR_SUP_DIG_MPLL_OVRD_IN_LO_val=((SSP_CR_SUP_DIG_MPLL_OVRD_IN_LO_val &  0xFFFFFDFF)|(0x1<<9));
  phy_cr_write(SSP_CR_SUP_DIG_MPLL_OVRD_IN_LO,SSP_CR_SUP_DIG_MPLL_OVRD_IN_LO_val);

  //-Write Fraction multipler to: 0x%x (SUP_DIG_SSC_OVRD_IN.SSC_REF_CLK_SEL)", ssc_ref_clk_sel);
  //[8:0]
  SSP_CR_SUP_DIG_SSC_OVRD_IN_val=((SSP_CR_SUP_DIG_SSC_OVRD_IN_val & 0xFFFFFE00)|(132));
  phy_cr_write(SSP_CR_SUP_DIG_SSC_OVRD_IN,SSP_CR_SUP_DIG_SSC_OVRD_IN_val);

  //turn on the mpll
  //-Enable mpll(SUP_DIG_MPLL_OVRD_IN_LO.MPLL_EN)
  //[0:0]
  SSP_CR_SUP_DIG_MPLL_OVRD_IN_LO_val=((SSP_CR_SUP_DIG_MPLL_OVRD_IN_LO_val & 0xFFFFFFFE)|(0x1));
  phy_cr_write(SSP_CR_SUP_DIG_MPLL_OVRD_IN_LO,SSP_CR_SUP_DIG_MPLL_OVRD_IN_LO_val);

  //reset the mpll now that it is up and running
  //-Reset MPll. Set bit to 0 (SUP_DIG_MPLL_OVRD_IN_HI.MPLL_RST)
  //[10:10]
  SSP_CR_SUP_DIG_MPLL_OVRD_IN_HI_val=((SSP_CR_SUP_DIG_MPLL_OVRD_IN_HI_val & 0xFFFFFBFF)|(0x0));
  phy_cr_write(SSP_CR_SUP_DIG_MPLL_OVRD_IN_HI,SSP_CR_SUP_DIG_MPLL_OVRD_IN_HI_val);

  //-Reset MPll. Set bit to 1 (SUP_DIG_MPLL_OVRD_IN_HI.MPLL_RST)
  //[10:10]
  SSP_CR_SUP_DIG_MPLL_OVRD_IN_HI_val=((SSP_CR_SUP_DIG_MPLL_OVRD_IN_HI_val & 0xFFFFFBFF)|(0x1<<10));
  phy_cr_write(SSP_CR_SUP_DIG_MPLL_OVRD_IN_HI,SSP_CR_SUP_DIG_MPLL_OVRD_IN_HI_val);
  set_marker(0x900d0007);

  //-Reset MPll. Set bit to 0 (SUP_DIG_MPLL_OVRD_IN_HI.MPLL_RST)
  //[10:10]
  SSP_CR_SUP_DIG_MPLL_OVRD_IN_HI_val=((SSP_CR_SUP_DIG_MPLL_OVRD_IN_HI_val & 0xFFFFFBFF)|(0x0));
  phy_cr_write(SSP_CR_SUP_DIG_MPLL_OVRD_IN_HI,SSP_CR_SUP_DIG_MPLL_OVRD_IN_HI_val);

  //-Disable ssc override (SUP_DIG_SSC_OVRD_IN.SSC_EN)
  //[12:12]
  SSP_CR_SUP_DIG_SSC_OVRD_IN_val=((SSP_CR_SUP_DIG_SSC_OVRD_IN_val & 0xFFFFEFFF)|(0x0));
  phy_cr_write(SSP_CR_SUP_DIG_SSC_OVRD_IN,SSP_CR_SUP_DIG_SSC_OVRD_IN_val);

  //-Enable override (SUP_DIG_SSC_OVRD_IN.SSC_OVRD_IN_EN)
  //[13:13]
  SSP_CR_SUP_DIG_SSC_OVRD_IN_val=((SSP_CR_SUP_DIG_SSC_OVRD_IN_val & 0xFFFFDFFF)|(0x1<<13));
  phy_cr_write(SSP_CR_SUP_DIG_SSC_OVRD_IN,SSP_CR_SUP_DIG_SSC_OVRD_IN_val);

  //turn on the tx
  //-Turn off digital loopback (LANEX.DIG.TX.OVRD_IN_LO.LOOPBK_EN)
  //[0:0]
  SSP_CR_LANEX_DIG_TX_OVRD_IN_LO_val=((SSP_CR_LANEX_DIG_TX_OVRD_IN_LO_val & 0xFFFFFFFE)|(0x0));
  phy_cr_write(SSP_CR_LANEX_DIG_TX_OVRD_IN_LO,SSP_CR_LANEX_DIG_TX_OVRD_IN_LO_val);

  //-Enable override (LANEX.DIG.TX.OVRD_IN_LO.TX_LOOPBK_EN_OVRD)
  //[1:1]
  SSP_CR_LANEX_DIG_TX_OVRD_IN_LO_val=((SSP_CR_LANEX_DIG_TX_OVRD_IN_LO_val & 0xFFFFFFFD)|(0x1<<1));
  phy_cr_write(SSP_CR_LANEX_DIG_TX_OVRD_IN_LO,SSP_CR_LANEX_DIG_TX_OVRD_IN_LO_val);

  //-Override tx_term_offset to 0 (LANE0_DIG_TX_OVRD_DRV_HI.TERM_OFFSET)
  //[4:0]
  SSP_CR_LANEX_DIG_TX_OVRD_DRV_HI_val=((SSP_CR_LANEX_DIG_TX_OVRD_DRV_HI_val & 0xFFFFFFE0)|(0x0));
  phy_cr_write(SSP_CR_LANEX_DIG_TX_OVRD_DRV_HI,SSP_CR_LANEX_DIG_TX_OVRD_DRV_HI_val);
  //-Enable override (LANE0_DIG_TX_OVRD_DRV_HI.EN)
  //[5:5]
  SSP_CR_LANEX_DIG_TX_OVRD_DRV_HI_val=((SSP_CR_LANEX_DIG_TX_OVRD_DRV_HI_val & 0xFFFFFFDF)|(0x1<<5));
  phy_cr_write(SSP_CR_LANEX_DIG_TX_OVRD_DRV_HI,SSP_CR_LANEX_DIG_TX_OVRD_DRV_HI_val);
  set_marker(0x900d0008);

  //-Turn off Beacon Enable (LANEX.DIG.TX.OVRD_IN_LO.TX_BEACON_EN)
  //[10:10]
  SSP_CR_LANEX_DIG_TX_OVRD_IN_LO_val=((SSP_CR_LANEX_DIG_TX_OVRD_IN_LO_val & 0xFFFFFBFF)|(0x0));
  phy_cr_write(SSP_CR_LANEX_DIG_TX_OVRD_IN_LO,SSP_CR_LANEX_DIG_TX_OVRD_IN_LO_val);
  //-Enable override (LANEX.DIG.TX.OVRD_IN_LO.TX_BEACON_EN_OVRD)
  //[11:11]
  SSP_CR_LANEX_DIG_TX_OVRD_IN_LO_val=((SSP_CR_LANEX_DIG_TX_OVRD_IN_LO_val & 0xFFFFF7FF)|(0x1<<11));
  phy_cr_write(SSP_CR_LANEX_DIG_TX_OVRD_IN_LO,SSP_CR_LANEX_DIG_TX_OVRD_IN_LO_val);

  //-Set TX CM Enable (LANEX.DIG.TX.OVRD_IN_LO.TX_CM_EN)
  //[8:8]
  SSP_CR_LANEX_DIG_TX_OVRD_IN_LO_val=((SSP_CR_LANEX_DIG_TX_OVRD_IN_LO_val & 0xFFFFFEFF)|(0x1<<8));
  phy_cr_write(SSP_CR_LANEX_DIG_TX_OVRD_IN_LO,SSP_CR_LANEX_DIG_TX_OVRD_IN_LO_val);
  //-Enable override (LANEX.DIG.TX.OVRD_IN_LO.TX_CM_EN_OVRD)
  //[9:9]
  SSP_CR_LANEX_DIG_TX_OVRD_IN_LO_val=((SSP_CR_LANEX_DIG_TX_OVRD_IN_LO_val & 0xFFFFFDFF)|(0x1<<9));
  phy_cr_write(SSP_CR_LANEX_DIG_TX_OVRD_IN_LO,SSP_CR_LANEX_DIG_TX_OVRD_IN_LO_val);

  //-Turn off tx_rx detect (LANEX.DIG.TX.OVRD_IN_LO.TX_DETECT_RX_REQ)
  //[12:12]
  SSP_CR_LANEX_DIG_TX_OVRD_IN_LO_val=((SSP_CR_LANEX_DIG_TX_OVRD_IN_LO_val & 0xFFFFEFFF)|(0x0<<12));
  phy_cr_write(SSP_CR_LANEX_DIG_TX_OVRD_IN_LO,SSP_CR_LANEX_DIG_TX_OVRD_IN_LO_val);

  //-Enable override (LANEX.DIG.TX.OVRD_IN_LO.TX_DETECT_RX_REQ_OVRD)
  //[13:13]
  SSP_CR_LANEX_DIG_TX_OVRD_IN_LO_val=((SSP_CR_LANEX_DIG_TX_OVRD_IN_LO_val & 0xFFFFDFFF)|(0x1<<13));
  phy_cr_write(SSP_CR_LANEX_DIG_TX_OVRD_IN_LO,SSP_CR_LANEX_DIG_TX_OVRD_IN_LO_val);

  //-Turn off tx invert(LANEX.DIG.TX.OVRD_IN_LO.TX_INVERT)
  //[2:2]
  SSP_CR_LANEX_DIG_TX_OVRD_IN_LO_val=((SSP_CR_LANEX_DIG_TX_OVRD_IN_LO_val & 0xFFFFFFFB)|(0x0<<2));
  phy_cr_write(SSP_CR_LANEX_DIG_TX_OVRD_IN_LO,SSP_CR_LANEX_DIG_TX_OVRD_IN_LO_val);

  //-Enable override (LANEX.DIG.TX.OVRD_IN_LO.TX_INVERT_OVRD)
  //[3:3]
  SSP_CR_LANEX_DIG_TX_OVRD_IN_LO_val=((SSP_CR_LANEX_DIG_TX_OVRD_IN_LO_val & 0xFFFFFFF7)|(0x1<<3));
  phy_cr_write(SSP_CR_LANEX_DIG_TX_OVRD_IN_LO,SSP_CR_LANEX_DIG_TX_OVRD_IN_LO_val);

  //-Set the TX amplitude(LANEX_DIG_TX_OVRD_DRV_LO.AMPLITUDE)
  //[6:0]
  SSP_CR_LANEX_DIG_TX_OVRD_DRV_LO_val=((SSP_CR_LANEX_DIG_TX_OVRD_DRV_LO_val & 0xFFFFFF80)|(0x7F));
  phy_cr_write(SSP_CR_LANEX_DIG_TX_OVRD_DRV_LO,SSP_CR_LANEX_DIG_TX_OVRD_DRV_LO_val);
  set_marker(0x900d0009);

  //-Set the TX boost(LANEX_DIG_TX_OVRD_DRV_LO.PREEMPH)
  //[13:7]
  SSP_CR_LANEX_DIG_TX_OVRD_DRV_LO_val=((SSP_CR_LANEX_DIG_TX_OVRD_DRV_LO_val & 0xFFFFC07F)|(0x0<<7));
  phy_cr_write(SSP_CR_LANEX_DIG_TX_OVRD_DRV_LO,SSP_CR_LANEX_DIG_TX_OVRD_DRV_LO_val);

  //-Enable override (LANEX_DIG_TX_OVRD_DRV_LO.EN)
  //[14:14]
  SSP_CR_LANEX_DIG_TX_OVRD_DRV_LO_val=((SSP_CR_LANEX_DIG_TX_OVRD_DRV_LO_val & 0xFFFFBFFF)|(0x1<<14));
  phy_cr_write(SSP_CR_LANEX_DIG_TX_OVRD_DRV_LO,SSP_CR_LANEX_DIG_TX_OVRD_DRV_LO_val);

  //-Set Data rate(LANEX_DIG_TX_OVRD_IN_HI.TX_RATE)
  //[1:0]
  SSP_CR_LANEX_DIG_TX_OVRD_IN_HI_val=((SSP_CR_LANEX_DIG_TX_OVRD_IN_HI_val & 0xFFFFFFFC)|(0x0));
  phy_cr_write(SSP_CR_LANEX_DIG_TX_OVRD_IN_HI,SSP_CR_LANEX_DIG_TX_OVRD_IN_HI_val);

  //-Enable override (LANEX_DIG_TX_OVRD_IN_HI.TX_RATE_OVRD)
  //[2:2]
  SSP_CR_LANEX_DIG_TX_OVRD_IN_HI_val=((SSP_CR_LANEX_DIG_TX_OVRD_IN_HI_val & 0xFFFFFFFB)|(0x1<<2));
  phy_cr_write(SSP_CR_LANEX_DIG_TX_OVRD_IN_HI,SSP_CR_LANEX_DIG_TX_OVRD_IN_HI_val);
  set_marker(0x900d000A);

  //have the lberts send out data before turning on
  //*************************************************
  //-Set pattern generator to send commas not commas(LANEX_DIG_TX_LBERT_CTL.PAT0)
  //[14:5]
  SSP_CR_LANEX_DIG_TX_LBERT_CTL_val=((SSP_CR_LANEX_DIG_TX_LBERT_CTL_val & 0xFFFF801F)|(31<<5));
  phy_cr_write(SSP_CR_LANEX_DIG_TX_LBERT_CTL,SSP_CR_LANEX_DIG_TX_LBERT_CTL_val);

  //-Set pattern generator to correct mode(LANEX_DIG_TX_LBERT_CTL.MODE)
  //[3:0]
  SSP_CR_LANEX_DIG_TX_LBERT_CTL_val=((SSP_CR_LANEX_DIG_TX_LBERT_CTL_val & 0xFFFFFFF0)|(0x6));
  phy_cr_write(SSP_CR_LANEX_DIG_TX_LBERT_CTL,SSP_CR_LANEX_DIG_TX_LBERT_CTL_val);
  //**************************************************

  //-Turn on TX Enable(LANEX.DIG.TX.OVRD_IN_LO.TX_EN)
  //[6:6]
  SSP_CR_LANEX_DIG_TX_OVRD_IN_LO_val=((SSP_CR_LANEX_DIG_TX_OVRD_IN_LO_val & 0xFFFFFFBF)|(0x0));
  phy_cr_write(SSP_CR_LANEX_DIG_TX_OVRD_IN_LO,SSP_CR_LANEX_DIG_TX_OVRD_IN_LO_val);

  //-Enable override (LANEX.DIG.TX.OVRD_IN_LO.TX_EN_OVRD)
  //[7:7]
  SSP_CR_LANEX_DIG_TX_OVRD_IN_LO_val=((SSP_CR_LANEX_DIG_TX_OVRD_IN_LO_val & 0xFFFFFF7F)|(0x1<<7));
  phy_cr_write(SSP_CR_LANEX_DIG_TX_OVRD_IN_LO,SSP_CR_LANEX_DIG_TX_OVRD_IN_LO_val);

  //do tune to get tx amp and boost to take
  //-Set Rtune to TX(SUP.DIG.RTUNE_DEBUG.TYPE)
  //[4:3]
  SSP_CR_SUP_DIG_RTUNE_DEBUG_val=((SSP_CR_SUP_DIG_RTUNE_DEBUG_val & 0xFFFFFFE7)|(0x2<<3));
  phy_cr_write(SSP_CR_SUP_DIG_RTUNE_DEBUG,SSP_CR_SUP_DIG_RTUNE_DEBUG_val);
  set_marker(0x900d000B);

  //-Force Rtune High(SUP.DIG.RTUNE_DEBUG.MAN_TUNE)
  //[1:1]
  SSP_CR_SUP_DIG_RTUNE_DEBUG_val=((SSP_CR_SUP_DIG_RTUNE_DEBUG_val & 0xFFFFFFFD)|(0x1<<1));
  phy_cr_write(SSP_CR_SUP_DIG_RTUNE_DEBUG,SSP_CR_SUP_DIG_RTUNE_DEBUG_val);

  //-Force Rtune Low(SUP.DIG.RTUNE_DEBUG.MAN_TUNE)
  //[1:1]
  SSP_CR_SUP_DIG_RTUNE_DEBUG_val=((SSP_CR_SUP_DIG_RTUNE_DEBUG_val & 0xFFFFFFFD)|(0x0<<1));
  phy_cr_write(SSP_CR_SUP_DIG_RTUNE_DEBUG,SSP_CR_SUP_DIG_RTUNE_DEBUG_val);

  //take tx out of reset
  //-Set Tx Reset Low(LANEX_DIG_TX_OVRD_IN_HI.TX_RESET)
  //[6:6]
  SSP_CR_LANEX_DIG_TX_OVRD_IN_HI_val=((SSP_CR_LANEX_DIG_TX_OVRD_IN_HI_val & 0xFFFFFFBF)|(0x0<<6));
  phy_cr_write(SSP_CR_LANEX_DIG_TX_OVRD_IN_HI,SSP_CR_LANEX_DIG_TX_OVRD_IN_HI_val);

  //-Set Tx Data Enable Low(LANEX_DIG_TX_OVRD_IN_LO.TX_DATA_EN)
  //[4:4]
  SSP_CR_LANEX_DIG_TX_OVRD_IN_LO_val=((SSP_CR_LANEX_DIG_TX_OVRD_IN_LO_val & 0xFFFFFFEF)|(0x0<<4));
  phy_cr_write(SSP_CR_LANEX_DIG_TX_OVRD_IN_LO,SSP_CR_LANEX_DIG_TX_OVRD_IN_LO_val);

  //-Enable override (LANEX_DIG_TX_OVRD_IN_LO.TX_DATA_EN_OVRD)
  //[5:5]
  SSP_CR_LANEX_DIG_TX_OVRD_IN_LO_val=((SSP_CR_LANEX_DIG_TX_OVRD_IN_LO_val & 0xFFFFFFDF)|(0x1<<5));
  phy_cr_write(SSP_CR_LANEX_DIG_TX_OVRD_IN_LO,SSP_CR_LANEX_DIG_TX_OVRD_IN_LO_val);

  //-Set Tx Data Enable High(LANEX_DIG_TX_OVRD_IN_LO.TX_DATA_EN)
  //[4:4]
  SSP_CR_LANEX_DIG_TX_OVRD_IN_LO_val=((SSP_CR_LANEX_DIG_TX_OVRD_IN_LO_val & 0xFFFFFFEF)|(0x1<<4));
  phy_cr_write(SSP_CR_LANEX_DIG_TX_OVRD_IN_LO,SSP_CR_LANEX_DIG_TX_OVRD_IN_LO_val);
  set_marker(0x900d000C);

  //start of receiver testing
  //-Set Tx Eq to use register value(LANEX.DIG.RX.OVRD_IN_HI.RX_EQ_EN)
  //[6:6]
  SSP_CR_LANEX_DIG_RX_OVRD_IN_HI_val=((SSP_CR_LANEX_DIG_RX_OVRD_IN_HI_val & 0xFFFFFFBF));
  phy_cr_write(SSP_CR_LANEX_DIG_RX_OVRD_IN_HI,SSP_CR_LANEX_DIG_RX_OVRD_IN_HI_val);

  //-Enable override (LANEX.DIG.RX.OVRD_IN_HI.RX_EQ_EN_OVRD)
  //[7:7]
  SSP_CR_LANEX_DIG_RX_OVRD_IN_HI_val=((SSP_CR_LANEX_DIG_RX_OVRD_IN_HI_val & 0xFFFFFF7F)|(0x1<<7));
  phy_cr_write(SSP_CR_LANEX_DIG_RX_OVRD_IN_HI,SSP_CR_LANEX_DIG_RX_OVRD_IN_HI_val);

  //-Set Tx Eq register value(LANEX_DIG_RX_OVRD_IN_HI.RX_EQ)
  //[10:8]
  SSP_CR_LANEX_DIG_RX_OVRD_IN_HI_val=((SSP_CR_LANEX_DIG_RX_OVRD_IN_HI_val & 0xFFFFF8FF)|(0x0<<8));
  phy_cr_write(SSP_CR_LANEX_DIG_RX_OVRD_IN_HI,SSP_CR_LANEX_DIG_RX_OVRD_IN_HI_val);

  //-Enable override (LANEX_DIG_RX_OVRD_IN_HI.RX_EQ_OVRD)
  //[11:11]
  SSP_CR_LANEX_DIG_RX_OVRD_IN_HI_val=((SSP_CR_LANEX_DIG_RX_OVRD_IN_HI_val & 0xFFFFF7FF)|(0x1<<11));
  phy_cr_write(SSP_CR_LANEX_DIG_RX_OVRD_IN_HI,SSP_CR_LANEX_DIG_RX_OVRD_IN_HI_val);

  //force eq value
  //-Set Rx Los filer to 0(LANEX.DIG.RX.OVRD_IN_HI.RX_LOS_FILTER)
  //[4:3]
  SSP_CR_LANEX_DIG_RX_OVRD_IN_HI_val=((SSP_CR_LANEX_DIG_RX_OVRD_IN_HI_val & 0xFFFFFFE7)|(0x0));
  phy_cr_write(SSP_CR_LANEX_DIG_RX_OVRD_IN_HI,SSP_CR_LANEX_DIG_RX_OVRD_IN_HI_val);
  set_marker(0x900d000D);

  //-Enable override (LANEX.DIG.RX.OVRD_IN_HI.RX_LOS_FILTER_OVRD)
  //[5:5]
  SSP_CR_LANEX_DIG_RX_OVRD_IN_HI_val=((SSP_CR_LANEX_DIG_RX_OVRD_IN_HI_val & 0xFFFFFFDF)|(0x1<<5));
  phy_cr_write(SSP_CR_LANEX_DIG_RX_OVRD_IN_HI,SSP_CR_LANEX_DIG_RX_OVRD_IN_HI_val);

  //-Set Rx Bit shift to 0(LANEX.DIG.RX.OVRD_IN_LO.RX_BIT_SHIFT)
  //[8:8]
  SSP_CR_LANEX_DIG_RX_OVRD_IN_LO_val=((SSP_CR_LANEX_DIG_RX_OVRD_IN_LO_val & 0xFFFFFEFF)|(0x0<<8));
  phy_cr_write(SSP_CR_LANEX_DIG_RX_OVRD_IN_LO,SSP_CR_LANEX_DIG_RX_OVRD_IN_LO_val);

  //-Enable override (LANEX.DIG.RX.OVRD_IN_LO.RX_BIT_SHIFT_OVRD)
  //[9:9]
  SSP_CR_LANEX_DIG_RX_OVRD_IN_LO_val=((SSP_CR_LANEX_DIG_RX_OVRD_IN_LO_val & 0xFFFFFDFF)|(0x1<<9));
  phy_cr_write(SSP_CR_LANEX_DIG_RX_OVRD_IN_LO,SSP_CR_LANEX_DIG_RX_OVRD_IN_LO_val);

  //-Set Rx invert bit to: %x (LANEX.DIG.RX.OVRD_IN_LO.RX_INVERT)", `FLIP_ONE);
  //[0:0]
  SSP_CR_LANEX_DIG_RX_OVRD_IN_LO_val=((SSP_CR_LANEX_DIG_RX_OVRD_IN_LO_val & 0xFFFFFFFE)|(0x0));
  phy_cr_write(SSP_CR_LANEX_DIG_RX_OVRD_IN_LO,SSP_CR_LANEX_DIG_RX_OVRD_IN_LO_val);

  //-Enable override (LANEX.DIG.RX.OVRD_IN_LO.RX_INVERT_OVRD)
  //[1:1]
  SSP_CR_LANEX_DIG_RX_OVRD_IN_LO_val=((SSP_CR_LANEX_DIG_RX_OVRD_IN_LO_val & 0xFFFFFFFD)|(0x1<<1));
  phy_cr_write(SSP_CR_LANEX_DIG_RX_OVRD_IN_LO,SSP_CR_LANEX_DIG_RX_OVRD_IN_LO_val);

  //-Enable rx los(LANEX.DIG.RX.OVRD_IN_LO.RX_LOS_EN)
  //[12:12]
  SSP_CR_LANEX_DIG_RX_OVRD_IN_LO_val=((SSP_CR_LANEX_DIG_RX_OVRD_IN_LO_val & 0xFFFFEFFF)|(0x1<<12));
  phy_cr_write(SSP_CR_LANEX_DIG_RX_OVRD_IN_LO,SSP_CR_LANEX_DIG_RX_OVRD_IN_LO_val);

  //-Enable override (LANEX.DIG.RX.OVRD_IN_LO.RX_LOS_EN_OVRD)
  //[13:13]
  SSP_CR_LANEX_DIG_RX_OVRD_IN_LO_val=((SSP_CR_LANEX_DIG_RX_OVRD_IN_LO_val & 0xFFFFDFFF)|(0x1<<13));
  phy_cr_write(SSP_CR_LANEX_DIG_RX_OVRD_IN_LO,SSP_CR_LANEX_DIG_RX_OVRD_IN_LO_val);
  set_marker(0x900d000E);

  //-Set rx datarate to full(LANEX_DIG_RX_OVRD_IN_HI.RX_RATE)
  //[1:0]
  SSP_CR_LANEX_DIG_RX_OVRD_IN_HI_val=((SSP_CR_LANEX_DIG_RX_OVRD_IN_HI_val & 0xFFFFFFFC)|(0x0<<0));
  phy_cr_write(SSP_CR_LANEX_DIG_RX_OVRD_IN_HI,SSP_CR_LANEX_DIG_RX_OVRD_IN_HI_val);

  //-Enable override (LANEX_DIG_RX_OVRD_IN_HI.RX_RATE_OVRD)
  //[2:2]
  SSP_CR_LANEX_DIG_RX_OVRD_IN_HI_val=((SSP_CR_LANEX_DIG_RX_OVRD_IN_HI_val & 0xFFFFFFFB)|(0x1<<2));
  phy_cr_write(SSP_CR_LANEX_DIG_RX_OVRD_IN_HI,SSP_CR_LANEX_DIG_RX_OVRD_IN_HI_val);

  //-Set Rx Align(LANEX_DIG_RX_OVRD_IN_LO.RX_ALIGN_EN)
  //[6:6]
  SSP_CR_LANEX_DIG_RX_OVRD_IN_LO_val=((SSP_CR_LANEX_DIG_RX_OVRD_IN_LO_val & 0xFFFFFFBF)|(0x1<<6));
  phy_cr_write(SSP_CR_LANEX_DIG_RX_OVRD_IN_LO,SSP_CR_LANEX_DIG_RX_OVRD_IN_LO_val);

  //-Enable override (LANEX_DIG_RX_OVRD_IN_LO.RX_ALIGN_EN_OVRD)
  //[7:7]
  SSP_CR_LANEX_DIG_RX_OVRD_IN_LO_val=((SSP_CR_LANEX_DIG_RX_OVRD_IN_LO_val & 0xFFFFFF7F)|(0x1<<7));
  phy_cr_write(SSP_CR_LANEX_DIG_RX_OVRD_IN_LO,SSP_CR_LANEX_DIG_RX_OVRD_IN_LO_val);

  //-Set Term enable(LANEX_DIG_RX_OVRD_IN_LO.RX_TERM_EN)
  //[10:10]
  SSP_CR_LANEX_DIG_RX_OVRD_IN_LO_val=((SSP_CR_LANEX_DIG_RX_OVRD_IN_LO_val & 0xFFFFFBFF)|(0x1<<10));
  phy_cr_write(SSP_CR_LANEX_DIG_RX_OVRD_IN_LO,SSP_CR_LANEX_DIG_RX_OVRD_IN_LO_val);

  //-Enable override (LANEX_DIG_RX_OVRD_IN_LO.RX_TERM_EN_OVRD)
  //[11:11]
  SSP_CR_LANEX_DIG_RX_OVRD_IN_LO_val=((SSP_CR_LANEX_DIG_RX_OVRD_IN_LO_val & 0xFFFFF7FF)|(0x1<<11));
  phy_cr_write(SSP_CR_LANEX_DIG_RX_OVRD_IN_LO,SSP_CR_LANEX_DIG_RX_OVRD_IN_LO_val);
  set_marker(0x900d000F);

  //-Enable Rx PLL(LANEX_DIG_RX_OVRD_IN_LO.RX_PLL_EN)
  //[2:2]
  SSP_CR_LANEX_DIG_RX_OVRD_IN_LO_val=((SSP_CR_LANEX_DIG_RX_OVRD_IN_LO_val & 0xFFFFFFFB)|(0x1<<2));
  phy_cr_write(SSP_CR_LANEX_DIG_RX_OVRD_IN_LO,SSP_CR_LANEX_DIG_RX_OVRD_IN_LO_val);

  //-Enable override (LANEX_DIG_RX_OVRD_IN_LO.RX_PLL_EN_OVRD)
  //[3:3]
  SSP_CR_LANEX_DIG_RX_OVRD_IN_LO_val=((SSP_CR_LANEX_DIG_RX_OVRD_IN_LO_val & 0xFFFFFFF7)|(0x1<<3));
  phy_cr_write(SSP_CR_LANEX_DIG_RX_OVRD_IN_LO,SSP_CR_LANEX_DIG_RX_OVRD_IN_LO_val);

  //-Enable Rx(LANEX_DIG_RX_OVRD_IN_LO.RX_DATA_EN)
  //[4:4]
  SSP_CR_LANEX_DIG_RX_OVRD_IN_LO_val=((SSP_CR_LANEX_DIG_RX_OVRD_IN_LO_val & 0xFFFFFFEF)|(0x1<<4));
  phy_cr_write(SSP_CR_LANEX_DIG_RX_OVRD_IN_LO,SSP_CR_LANEX_DIG_RX_OVRD_IN_LO_val);

  //-Enable override (LANEX_DIG_RX_OVRD_IN_LO.RX_DATA_EN_OVRD)
  //[5:5]
  SSP_CR_LANEX_DIG_RX_OVRD_IN_LO_val=((SSP_CR_LANEX_DIG_RX_OVRD_IN_LO_val & 0xFFFFFFDF)|(0x1<<5));
  phy_cr_write(SSP_CR_LANEX_DIG_RX_OVRD_IN_LO,SSP_CR_LANEX_DIG_RX_OVRD_IN_LO_val);

  //take the Rx out of reset
  //-Set Rx reset low(LANEX_DIG_RX_OVRD_IN_HI.RX_RESET)
  //[12:12]
  SSP_CR_LANEX_DIG_RX_OVRD_IN_HI_val=((SSP_CR_LANEX_DIG_RX_OVRD_IN_HI_val & 0xFFFFEFFF)|(0x0<<12));
  phy_cr_write(SSP_CR_LANEX_DIG_RX_OVRD_IN_HI,SSP_CR_LANEX_DIG_RX_OVRD_IN_HI_val);
  set_marker(0x900d0010);

  //-Pause 1mS
  nop(100000);//Delay

  set_marker(0x900d0011);
  printf("Waiting for  RX CDR is lock on LANE0...\n");
  //-Check to see if RX CDR is locked on LANE0
  //Verify CDR has locked
  read_data=phy_cr_read(SSP_CR_LANE0_DIG_RX_CDR_CDR_FSM_DEBUG);
  exp_read_data = (read_data | 0x0008); // Checking to see if LANE0_DIG_RX_CDR_CDR_FSM_DEBUG.ALIGNED is 1
  while(read_data != exp_read_data)
  {
    read_data=phy_cr_read(SSP_CR_LANE0_DIG_RX_CDR_CDR_FSM_DEBUG);
    exp_read_data = (read_data | 0x0008); // Checking to see if LANE0_DIG_RX_CDR_CDR_FSM_DEBUG.ALIGNED is 1
  }
  //if(read_data == exp_read_data)
  //{
    printf("RX CDR is locked on LANE0\n");
  //}
  //else
  //{
  //  printf("RX CDR is not-locked on LANE0\n");
  //  err_cnt++;
  //  printf("Error:%d\n",err_cnt);
  //}
  //set bert back to normal mode
  //-Turn off data alignment(LANEX_DIG_RX_OVRD_IN_LO.RX_ALIGN_EN)");
  //[6:6]
  SSP_CR_LANEX_DIG_RX_OVRD_IN_LO_val=((SSP_CR_LANEX_DIG_RX_OVRD_IN_LO_val & 0xFFFFFFBF)|(0x0<<6));
  phy_cr_write(SSP_CR_LANEX_DIG_RX_OVRD_IN_LO,SSP_CR_LANEX_DIG_RX_OVRD_IN_LO_val);

  //-Set patten generator to selected pattern(LANEX_DIG_TX_LBERT_CTL.PAT0)
  //[14:5]
  SSP_CR_LANEX_DIG_TX_LBERT_CTL_val=((SSP_CR_LANEX_DIG_TX_LBERT_CTL_val & 0xFFFF801F)|(302<<5));
  phy_cr_write(SSP_CR_LANEX_DIG_TX_LBERT_CTL,SSP_CR_LANEX_DIG_TX_LBERT_CTL_val);

  //-Set patten generator to selected mode(LANEX_DIG_TX_LBERT_CTL.MODE)
  //[3:0]
  SSP_CR_LANEX_DIG_TX_LBERT_CTL_val=((SSP_CR_LANEX_DIG_TX_LBERT_CTL_val & 0xFFFFFFF0)|(0x4));
  phy_cr_write(SSP_CR_LANEX_DIG_TX_LBERT_CTL,SSP_CR_LANEX_DIG_TX_LBERT_CTL_val);
  set_marker(0x900d0012);

  //-Set patten matcher to selected mode(LANEX_DIG_RX_LBERT_CTL.MODE)
  //[3:0]
  SSP_CR_LANEX_DIG_RX_LBERT_CTL_val=((SSP_CR_LANEX_DIG_RX_LBERT_CTL_val & 0xFFFFFFF0)|(0x4));
  phy_cr_write(SSP_CR_LANEX_DIG_RX_LBERT_CTL,SSP_CR_LANEX_DIG_RX_LBERT_CTL_val);

  //-Sync pattern matcher low(LANEX_DIG_RX_LBERT_CTL.SYNC)
  //[4:4]
  SSP_CR_LANEX_DIG_RX_LBERT_CTL_val=((SSP_CR_LANEX_DIG_RX_LBERT_CTL_val & 0xFFFFFFEF)|(0x0<<4));
  phy_cr_write(SSP_CR_LANEX_DIG_RX_LBERT_CTL,SSP_CR_LANEX_DIG_RX_LBERT_CTL_val);

  //-Sync pattern matcher high(LANEX_DIG_RX_LBERT_CTL.SYNC)
  //[4:4]
  SSP_CR_LANEX_DIG_RX_LBERT_CTL_val=((SSP_CR_LANEX_DIG_RX_LBERT_CTL_val & 0xFFFFFFEF)|(0x1<<4));
  phy_cr_write(SSP_CR_LANEX_DIG_RX_LBERT_CTL,SSP_CR_LANEX_DIG_RX_LBERT_CTL_val);

  //-Sync pattern matcher low(LANEX_DIG_RX_LBERT_CTL.SYNC)
  //[4:4]
  SSP_CR_LANEX_DIG_RX_LBERT_CTL_val=((SSP_CR_LANEX_DIG_RX_LBERT_CTL_val & 0xFFFFFFEF)|(0x0<<4));
  phy_cr_write(SSP_CR_LANEX_DIG_RX_LBERT_CTL,SSP_CR_LANEX_DIG_RX_LBERT_CTL_val);
  set_marker(0x900d0013);
  //$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$//
 //while(1)
 //{
  //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
  // BERT_TEST
  //*************************************************************************
  // Functional test to verify that the device is operational and
  // the BERTS are functioning. The device is set-up in external loopback.
  // The berts are tested that they can match lfsr7 and that they can count
  // errors
  //
  // DETAILED PROGRAM STEPS
  // Assume device is in set-up mode
  // Sync pattern matchers
  // Wait 5ms
  // Check for errors
  // Generate 1 error in pattern generator
  // Wait 5mS
  // Check for 1 error at patten matchers
  //*************************************************************************
  //sync pattern matchers
  //-Sync the patten matchers high (LANEX.DIG.RX.LBERT_CTL.SYNC)
  // set sync bit
  //[4:4]
  SSP_CR_LANEX_DIG_RX_LBERT_CTL_val=((SSP_CR_LANEX_DIG_RX_LBERT_CTL_val & 0xFFFFFFEF)|(0x1<<4));
  phy_cr_write(SSP_CR_LANEX_DIG_RX_LBERT_CTL,SSP_CR_LANEX_DIG_RX_LBERT_CTL_val);

  //-Sync the pattern matchers low (LANEX.DIG.RX.LBERT_CTL.SYNC)
  // clear sync bit
  //[4:4]
  SSP_CR_LANEX_DIG_RX_LBERT_CTL_val=(( SSP_CR_LANEX_DIG_RX_LBERT_CTL_val & 0xFFFFFFEF)|(0x0<<4));
  phy_cr_write(SSP_CR_LANEX_DIG_RX_LBERT_CTL,SSP_CR_LANEX_DIG_RX_LBERT_CTL_val);
  set_marker(0x900d0014);

  //wait 5ms to check if errors are generated
  //-Pause 5mS
  nop(50000);//Delay

  set_marker(0x900d0015);
  //verify no errors on pattern matcher
  //-Check for 0 errors on channel0 .DIG.RX.LBERT_ERR.COUNT)
  //Bert errors on channel0 expect 0 errors
  read_data=phy_cr_read(SSP_CR_LANE0_DIG_RX_LBERT_ERR);
  exp_read_data=0x0000;//(read_data & 0x8000);
  if(read_data == exp_read_data)
  {
    printf("NO BIST ERRORS\n");
  }
  else
  {
    printf("%d BIST ERRORS\n",read_data);
    err_cnt++;
    printf("Error:%d\n",err_cnt);
  }

  //insert 1 error on all channels
  //-Insert 1 error on the pattern generator high (LANEX.DIG.TX.LBERT_CTL.TRIGGER_ERR)
  //[4:4]
  SSP_CR_LANEX_DIG_TX_LBERT_CTL_val=((SSP_CR_LANEX_DIG_TX_LBERT_CTL_val & 0xFFFFFFEF)|(0x1<<4));
  phy_cr_write(SSP_CR_LANEX_DIG_TX_LBERT_CTL,SSP_CR_LANEX_DIG_TX_LBERT_CTL_val);

  //-Insert 1 error on the pattern generator low (LANEX.DIG.TX.LBERT_CTL.TRIGGER_ERR)
  //[4:4]
  SSP_CR_LANEX_DIG_TX_LBERT_CTL_val=((SSP_CR_LANEX_DIG_TX_LBERT_CTL_val & 0xFFFFFFEF)|(0x0<<4));
  phy_cr_write(SSP_CR_LANEX_DIG_TX_LBERT_CTL,SSP_CR_LANEX_DIG_TX_LBERT_CTL_val);
  set_marker(0x900d0016);

  //wait 5ms to check if errors are generated
  //-Wait 5mS
  nop(50000);//Delay

  set_marker(0x900d0017);
  //verify 1 error on pattern matcher
  //-Check for 1 error on channel0 .DIG.RX.LBERT_ERR.COUNT
  //Bert errors on channel0 expect 1 error
  read_data = phy_cr_read(SSP_CR_LANE0_DIG_RX_LBERT_ERR);
  exp_read_data = 0x0001;//(read_data & 0x8001);
  if(read_data == exp_read_data)
  {
    printf("BIST ERRORS MATCHED\n");
  }
  else
  {
    printf("%d BIST ERRORS\n",read_data);
    err_cnt++;
    printf("Error:%d\n",err_cnt);
  }
  //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
  if(err_cnt==0)
  {
    printf(">>BER TEST PASS<<\n");
    set_marker(0x900D900D);
    ret = 0;
  }
  else
  {
    printf(">>BER TEST FAIL<<\n");
    set_marker(0xBAADBAAD);
  }
  nop(50);

	return ret;
}
