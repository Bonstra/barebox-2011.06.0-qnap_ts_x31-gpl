#ifdef USB_DEBUG
#define pr_debug(fmt, arg...)   printf(fmt, ##arg)
#else
#define pr_debug(fmt, arg...)   do {} while(0)
#endif

#define dbg_usb(fmt, arg...)      pr_debug(fmt, ##arg)

#if 0
void nop (volatile int no_of_nop){
  int temp, i;
  temp = 0;
  for(i = 0; i < no_of_nop; i++){
    temp = i;
  }
  return;
}
#endif

//****************************************************************************************//
void usb3subsys_phy_init(void)
{
	dbg_usb("*** Programming the usb3 controller & usb3 phy control top level registers,while usb3 controller+phy are in reset ****\n");
	//----------------------------------------------------------------
	// PROGRAMMING THE CONTROLLER + PHY CONTROLL REGISTERS
	// Note: Core+PHY need to be in reset to prior to programming these 
	//       registers.
	//----------------------------------------------------------------   
	//{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{[
	// Programming the controller control register #0 (0x904a_0010)
	// Note: register should be configured while Controller and PHY are in reset
	//31:24  	RSVD				=0000_0000
	//23          xhc_bme				=1 -bus mastering capability enabled;0-bus mastering capability disabled.			
	//22		xhsi_rev			=1 -this xHCI is compliant to xHCI standard revision1.0.
	//21:16	fladj_30mhz_reg[5:0]		=10_0000 (decimal:32)
	//15:12	bus_filter_bypass[3:0]		=0000 -bus filters enabled;1111-bus filters bypassed.
	//11:10	Reserved			=00
	//9		pme_en				=0 -disables the pme-generation;1-enables the pme generation.
	//8		host_port_power_control_present	=0- port does not have power switches;1- port has power switches
	//7		vbus_ctrl_en			=1- enable controller to drive PHY's DRVVBUS(host);0-for device
	//6		host_u3_port_disable		=0- USB3.0 port enabled;1-USB3.0 port disabled.
	//5		host_u2_port_disable		=0- USB2.0 port enabled;1-USB2.0 port disabled.
	//4		host_msi_enable			=0 -enables level type interrupt from controller;1-enables pulse type interrupt from controller.
	//3:2		hub_port_perm_attach[1:0]	=00-Device is not permanently attached;11-device is permanently attached.
	//1:0		hub_port_overcurrent[1:0]	=00-No over current ; 11- Over current

	writel(0x00E000A0,(USB3p0_PHY_CFG_BASE_ADDR+0x10)); //With USB2.0 port disabled 
	dbg_usb("USB3 CONTROLLER CONTROL REG#0 @ A:%x,WrD:%x\n",(USB3p0_PHY_CFG_BASE_ADDR+0x10),0x00E000A0);


	//{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{[
	// Programming Controller controll register #1 (0x904a_0014)
	//31:9	RSVD	
	//8		host_legacy_smi_bar_wr
	//7:5		RSVD
	//4		host_legacy_smi_pci_cmd_writel I
	//3:2		RSVD
	//1:0		pm_power_state_request[1:0]

	//{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{[
	//[0x18 RWSC] Controller Control Register #2
	//(Default: 0x00000001)
	//Bit	Name	Description	
	//31:1	RSVD (tied to ‘0’)	
	//0		light_reset_n	
	//Active low reset. It is similar to the xHCI "Light Reset" 
	//which does not reset any sticky bits. 
	//When operating as a device this bit should be kept high. 
	//Default=0b1	

	//{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{[
	// Programming the PHY controll register #0((0x904a_0020)
	// Note: register should be configured while Controller and PHY are in reset
	//31:23	ssc_ref_clk_sel[8:0] 	=0100_0010_0 //Spread Spectrum Reference Clock Shifting 
	//22:20	ssc_range[2:0] 		=000         //Spread Spectrum Clock Range
	//19		ssc_en			=1           //Spread Spectrum Enable
	//18:12	mpll_multiplier[6:0]	=001_1001 (100MHz) //MPLL Frequency Multiplier Control ,but only relevant if refclksel=11
	//11		commononn		=0	     //Common Block Power-Down Control
	//10		ref_clkdiv2		=0           //Input Reference Clock Divider Control
	//9:4		fsel[5:0]		=10_0111 (100MHz) //Frequency Select
	//3:2		refclksel		=10          //Reference Clock Select for HS PLL Block
	//1		ref_use_pad		=1           //Select Reference Clock
	//0		phy_sel_div2_clk	=0

	writel(0x4209927A,(USB3p0_PHY_CFG_BASE_ADDR+0x20)); 
	dbg_usb("USB3PHY CONTROL REG#0 @ A:%x,WrD:%x\n",(USB3p0_PHY_CFG_BASE_ADDR+0x20),0x4209927A);

	//{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{[
	//Programming the PHY controll register #1 (0x904a_0024)
	//Note: register should be configured while Controller and PHY are in reset
	//31:29	sqrxtune0[2:0]		=011
	//28:24	los_level[4:0]		=0_1001
	//23		ref_ssp_en		=1
	//22:20	otgtune0[2:0]		=100
	//19:16	txfslstune0[3:0]	=0011
	//15		txpreemppulserune0	=0
	//14:12	compdistune0[2:0]	=100
	//11:10	txpreempamptune0[1:0]	=11 
	//9:8		txhsxvtune0[1:0]	=11
	//7:6		txrestune0[1:0]		=01
	//5:4		txrisetune0[1:0]	=01
	//3:0	 	txvreftune0[3:0]	=0011

	writel(0x69C34F53,(USB3p0_PHY_CFG_BASE_ADDR+0x24)); 
	dbg_usb("USB3PHY CONTROL REG#1 @ A:%x,WrD:%x\n",(USB3p0_PHY_CFG_BASE_ADDR+0x24),0x69C34F53); 


	//{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{[
	//Programming the PHY controll register #2 (0x904a_0028)
	//Note: register should be configured while Controller and PHY are in reset
	//31:27	lane0_tx_term_offset[4:0]	=0000_0
	//26    	Reserved			=0
	//25:23 	tx_vboost_lvl[2:0]		=00_0
	//22:20	los_bias[2:0]			=000_
	//19		vbusvldextsel0			=0       
	//18:12	pcs_tx_swing_full[6:0]		=101_1101_
	//11:6	pcs_tx_deemph_6db[5:0]		=1000_00
	//5:0		pcs_tx_deemph_3p5db[5:0]	=01_0101

	writel(0x0005D815,(USB3p0_PHY_CFG_BASE_ADDR+0x28)); 
	dbg_usb("USB3PHY CONTROL REG#2 @ A:%x,WrD:%x\n",(USB3p0_PHY_CFG_BASE_ADDR+0x28),0x0005D815); 


	//{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{[
	//[0x2C RW] PHY Control Register #3
	//31:17	RSVD			=0000_0000_0000_000	
	//16		loopbackenb0		=0
	//15		adpprbenb0		=0
	//14		adpdischrg0		=0
	//13		adpchrg0		=0
	//12		idpullup0		=0 (Host)
	//11		drvvbus0		=1 (M)
	//10		vbusvldext0		=0
	//9:8		vatestenb[1:0]		=00
	//7		otgdisable0		=0 (M)
	//6		test_powerdown_ssp	=0
	//5		test_powerdown_hsp	=0
	//4		lane0_tx2rx_loopbk	=0
	//3		lane0_ext_pclk_req	=0
	//2:1		Reserved		=00
	//0		rerenablen		=1

	writel(0x00000801,(USB3p0_PHY_CFG_BASE_ADDR+0x2C)); 
	dbg_usb("USB3PHY CONTROL REG#3 @ A:%x,WrD:%x\n",(USB3p0_PHY_CFG_BASE_ADDR+0x2C),0x00000801); 


	dbg_usb("*** usb3:phy & controller top level registers conifguration done ****\n");
} //usb3subsys_phy_init


//****************************************************************************************//
void usb3_controller_global_regs_init(void)
{
	dbg_usb("*** Programming the usb3 controller global registers ****\n");
	//----------------------------------------------------------------
	// PROGRAMMING GLOBAL REGISTERS
	//----------------------------------------------------------------
	// GUSB2PHYCFG - De-assert usb2 PHY reset if default its set
	for(usb3_i=0; usb3_i<=DWC_USB3_HOST_NUM_U2_ROOT_PORTS-1; usb3_i=usb3_i+1)
	{
		usb3_read_data = readl((USB3p0_CFG_BASE_ADDR+0xc000 + (usb3_i * 4) + 0x200));   // GUSB2PHYCFG
		dbg_usb("USB3CONTROLER GUSB2PHYCFG REG @ A:%x,RdD:%x\n",(USB3p0_CFG_BASE_ADDR+0xc000 + (usb3_i * 4) + 0x200),usb3_read_data);
		usb3_read_data = (usb3_read_data & 0x7FFFFFFF); //make [31] = 1'b0
		writel( usb3_read_data,(USB3p0_CFG_BASE_ADDR+0xc000 + (usb3_i * 4) + 0x200));  // GUSB2PHYCFG
		dbg_usb("USB3CONTROLLER GUSB2PHYCFG REG @ A:%x,WrD:%x\n",(USB3p0_CFG_BASE_ADDR+0xc000 + (usb3_i * 4) + 0x200),usb3_read_data);
	}
	// GUSB3PIECL - De-assert usb3 PHY reset if default its set
	for(usb3_i=0; usb3_i<=DWC_USB3_HOST_NUM_U3_ROOT_PORTS-1; usb3_i=usb3_i+1)
	{
		usb3_read_data = readl((USB3p0_CFG_BASE_ADDR+0xc000 + (usb3_i * 4) + 0x2c0));    // GUSB3PIECL
		dbg_usb("USB3CONTROLLER GUSB3PIECL REG @ A:%x,RdD:%x\n",(USB3p0_CFG_BASE_ADDR+0xc000 + (usb3_i * 4) + 0x2c0),usb3_read_data);
		usb3_read_data = (usb3_read_data & 0x7FFFFFFF); //make [31] = 1'b0
		writel( usb3_read_data,(USB3p0_CFG_BASE_ADDR+0xc000 + (usb3_i * 4) + 0x2c0));    // GUSB3PIECL
		dbg_usb("USB3CONTROLLER GUSB3PIECL REG @ A:%x,WrD:%x\n",(USB3p0_CFG_BASE_ADDR+0xc000 + (usb3_i * 4) + 0x2c0), usb3_read_data);
	}
	nop(4);
	//-------------------------------------------------------------------------------
	// Bypass Scrambling and Set Shorter Training sequence for simulation
	//-------------------------------------------------------------------------------
	/* GCTL[31:19] is decided by susp}_clk = 15mhz, scale = susp}_clk/16khz, 15000/16 = 938(dec) = 3A9(Hex)
	   when left shifted, it becomes 1d48 assuming [18:16]==0. */
if((DWC_USB3_BUS_CLK_FREQ < 125) && (DWC_USB3_SSPHY_INTERFACE==1))
{
	// writel( 0x0000_1078,USB3p0_CFG_BASE_ADDR+0xC110);  // GCTL    - ram_clk_sel=pipe3_clk, scramble-off, scaledown
	writel( 0x1d481078,(USB3p0_CFG_BASE_ADDR+0xC110));  // GCTL    - ram_clk_sel=pipe3_clk, scramble-off, scaledown
	dbg_usb("USB3CONTROLLER  GCTL REG @ A:%x,WrD:%x\n",(USB3p0_CFG_BASE_ADDR+0xC110), 0x1d481038);
}
else
{
	//[31:19] PwrDwnScale - when susp_clk=48MHz => 48M/16K=3000=0xBB8 -> shift left by 8 -> 0x5DC0 
	//[18] Master Filter Bypass -> 0
	//[17] Reserved -> 0
	//[16] u2rstecn -> 0
	//[15:14] FRMSCLDWN = 125us -> 00
	//[13:12] PrtCapDir = Host -> 01
	//[11] Core Soft Reset = No -> 0
	//[10:9] Reserved -> 00
	//[8] Debug Attach -> 0
	//[7:6] RAM Clock = Bus Clock (if >125MHz) -> 2'b00
	//[5:4] Scale Down Mode = Disabled -> 00
	//[3] Disable Scrambling -> 1 (TEMP!!!!!)
	//[2:1] Reserved -> 00
	//[0] Disable Clock Gaing (at early validation stage we do not set it to 1) -> 0
	dbg_usb("*** Programming the usb3 controller in scaledown disable mode ****\n"); 
	writel( 0x5Dc11000,(USB3p0_CFG_BASE_ADDR+0xC110)); //With u2rstecn-1'b1,With enabled scrambling
	dbg_usb("USB3CONTROLLER  GCTL REG @ A:%x,WrD:%x\n",(USB3p0_CFG_BASE_ADDR+0xC110), 0x5DC11000);
}
} //usb3_controller_global_regs_init
//****************************************************************************************//
void usb3_constants_init(void)
{
	dbg_usb("*** USB3:Initializing variables with system memory mapped address ranges ****\n");
	//----------------------------------------------------------------
	// Defining SOC depedent data structures memory range.
	//----------------------------------------------------------------
	usb3_xHC_register_base                = ((u32)(USB3_CONTROLLER_REG_CFG_BASE_ADDRESS));  
	usb3_xHC_data_addr                    = ((u32)(SYS_MEMORY_BASE_ADDRESS));
	usb3_xHC_Transfer_TRB_addr            = ((u32)(usb3_xHC_data_addr + xHC_SYS_MEMORY_Transfer_TRB_OFFSET_ADDRESS)); 
	usb3_xHC_Command_TRB_addr             = ((u32)(usb3_xHC_data_addr + xHC_SYS_MEMORY_Command_TRB_OFFSET_ADDRESS)); 
	usb3_xHC_Event_TRB_addr               = ((u32)(usb3_xHC_data_addr + xHC_SYS_MEMORY_Event_TRB_OFFSET_ADDRESS));      
	usb3_xHC_DCBAR_addr                   = ((u32)(usb3_xHC_data_addr + xHC_SYS_MEMORY_Device_Context_Base_Address_Array_OFFSET_ADDRESS));     
	usb3_xHC_Device_Context_addr          = ((u32)(usb3_xHC_data_addr + xHC_SYS_MEMORY_Device_Context_OFFSET_ADDRESS));      
	usb3_xHC_Input_Context_addr           = ((u32)(usb3_xHC_data_addr + xHC_SYS_MEMORY_Input_Context_OFFSET_ADDRESS));     
	usb3_xHC_Scratch_Pad_Array_addr       = ((u32)(usb3_xHC_data_addr + xHC_SYS_MEMORY_Scratch_Pad_Array_ADDRESS));
	usb3_xHC_Scratch_Pad_buf0_addr        = ((u32)(usb3_xHC_data_addr + xHC_SYS_MEMORY_Scratch_Pad_BUF0_ADDRESS));
	usb3_xHC_EventRing_Segment_addr       = ((u32)(usb3_xHC_data_addr + xHC_SYS_MEMORY_Event_Ring_Segment_OFFSET_ADDRESS));     
	usb3_xHC_EventRing_Segment_table_addr = ((u32)(usb3_xHC_data_addr + xHC_SYS_MEMORY_Event_Ring_Segment_Table_OFFSET_ADDRESS));   
	usb3_xHC_CommandRing_Segment_addr     = ((u32)(usb3_xHC_data_addr + xHC_SYS_MEMORY_Command_Ring_Segment_OFFSET_ADDRESS));    
	usb3_xHC_TransferRing_Segment_addr    = ((u32)(usb3_xHC_data_addr + xHC_SYS_MEMORY_Transfer_Ring_Segment_OFFSET_ADDRESS));   
	usb3_xHC_Data_BulkOut_Buffer_addr     = ((u32)(usb3_xHC_data_addr + xHC_SYS_MEMORY_Data_BULKOUT_BUF_ADDRESS));
	usb3_xHC_Data_BulkIn_Buffer_addr      = ((u32)(usb3_xHC_data_addr + xHC_SYS_MEMORY_Data_BULKIN_BUF_ADDRESS));

	dbg_usb ("usb3_xHC_register_base                = %x\n",usb3_xHC_register_base );
	dbg_usb ("usb3_xHC_data_addr                    = %x\n",usb3_xHC_data_addr );
	dbg_usb ("usb3_xHC_Transfer_TRB_addr            = %x\n",usb3_xHC_Transfer_TRB_addr );
	dbg_usb ("usb3_xHC_Command_TRB_addr             = %x\n",usb3_xHC_Command_TRB_addr );
	dbg_usb ("usb3_xHC_Event_TRB_addr               = %x\n",usb3_xHC_Event_TRB_addr );
	dbg_usb ("usb3_xHC_DCBAR_addr                   = %x\n",usb3_xHC_DCBAR_addr );
	dbg_usb ("usb3_xHC_Device_Context_addr          = %x\n",usb3_xHC_Device_Context_addr );
	dbg_usb ("usb3_xHC_Input_Context_addr           = %x\n",usb3_xHC_Input_Context_addr );
	dbg_usb ("usb3_xHC_Scratch_Pad_Array_addr       = %x\n",usb3_xHC_Scratch_Pad_Array_addr );
	dbg_usb ("usb3_xHC_Scratch_Pad_buf0_addr        = %x\n",usb3_xHC_Scratch_Pad_buf0_addr );
	dbg_usb ("usb3_xHC_EventRing_Segment_addr       = %x\n",usb3_xHC_EventRing_Segment_addr );
	dbg_usb ("usb3_xHC_EventRing_Segment_table_addr = %x\n",usb3_xHC_EventRing_Segment_table_addr );
	dbg_usb ("usb3_xHC_CommandRing_Segment_addr     = %x\n",usb3_xHC_CommandRing_Segment_addr );
	dbg_usb ("usb3_xHC_TransferRing_Segment_addr    = %x\n",usb3_xHC_TransferRing_Segment_addr );
	dbg_usb ("usb3_xHC_Data_BulkIn_Buffer_addr      = %x\n",usb3_xHC_Data_BulkIn_Buffer_addr );
	dbg_usb ("usb3_xHC_Data_BulkOut_Buffer_addr     = %x\n",usb3_xHC_Data_BulkOut_Buffer_addr );
	//-----------------------------------------------------------
	usb3_ss_port_number = DWC_USB3_HOST_NUM_U2_ROOT_PORTS +1;
	dbg_usb("USB3:SS 1st port number is %d\n", usb3_ss_port_number);
	usb3_hs_port_number = 1; //HS/FS/LS port number is always 1
	dbg_usb("USB3:HS 1st port number is %d\n", usb3_hs_port_number);
	//-----------------------------------------------------------
	//Transfer Parameters  - 
	usb3_total_num_of_bulkout_bytes         =  USB3_NUM_OF_BULKOUT_BYTES_TO_BE_XFERED;  // Should be multiple of 4
	usb3_host_num_of_bulkout_trbs           =  USB3_NUM_OF_BULKOUT_TRBS;
	usb3_host_num_of_bulkout_bytes_per_trb  =  usb3_total_num_of_bulkout_bytes/usb3_host_num_of_bulkout_trbs; 
	usb3_total_num_of_bulkin_bytes          =  USB3_NUM_OF_BULKIN_BYTES_TO_BE_XFERED;  // Should be multiple of 4
	usb3_host_num_of_bulkin_trbs            =  USB3_NUM_OF_BULKIN_TRBS;
	usb3_host_num_of_bulkin_bytes_per_trb   =  usb3_total_num_of_bulkin_bytes/usb3_host_num_of_bulkin_trbs;

	//-----------------------------------------------------------
	//nop(50);
} //usb3_constants_init
//****************************************************************************************//
//xHC controller initialization
void usb3_xHC_controller_init(void)
{
	dbg_usb("*** Initializing the USB3 xHC Controller ****\n");
	//Read the value of CAPLENGTH out of Capability Register
	usb3_read_data = readl((USB3p0_CFG_BASE_ADDR+0x00));
	dbg_usb("USB3CONTROLLER CAPABILITY REG @ A:%x,RdD:%x\n",(USB3p0_CFG_BASE_ADDR+0x00),usb3_read_data);
	usb3_caplength = (usb3_read_data & 0x00FF);
	dbg_usb("USB3:Caplength = %x\n", usb3_caplength);
	// Poll the usb3_CNR bit11 of USBSTS Register after a Chip Hardware Reset
	usb3_read_data = readl((USB3p0_CFG_BASE_ADDR+usb3_caplength+0x04));
	dbg_usb("USB3CONTROLLER USBSTS REG @  A:%x,RdD:%x\n",(USB3p0_CFG_BASE_ADDR+usb3_caplength+0x04),usb3_read_data);
	usb3_CNR = (usb3_read_data & 0x00000800);
	//wait for xHC ready to begin accepting register writes
	dbg_usb("USB3: Waiting for controller ready to begin accepting register writes\n");
	while (usb3_CNR != 0x0)
	{
		usb3_read_data = readl((USB3p0_CFG_BASE_ADDR+usb3_caplength+0x04));
		dbg_usb("USB3CONTROLLER USBSTS REG @ A:%x,RdD:%x\n",(USB3p0_CFG_BASE_ADDR+usb3_caplength+0x04),usb3_read_data);
		usb3_CNR = (usb3_read_data & 0x00000800);
		dbg_usb("usb3_CNR = %x\n", usb3_CNR);
	}  
	dbg_usb("**** USB3:xHC is now ready to begin accepting register writes ****\n"); 
} //usb3_xHC_controller_init  
//****************************************************************************************//
// Configuring xHC controller registers for Host porgramming
void usb3_xHC_controller_regs_config(void)
{
	dbg_usb("*** Configuring the USB3 xHC Controller host programming registers ****\n");
	//Program the MaxSlotEn of Configure Register(CONFIG) 
	//usb3_read_data[31:8] = 0;        //Reserved
	//usb3_read_data[7:0]  = 16;       //Max device slot enabled
	usb3_read_data = 0x10;
	writel( usb3_read_data,(USB3p0_CFG_BASE_ADDR+usb3_caplength+0x38));  
	dbg_usb("USB3CONTROLLER CONFIGURATION REG @ A:%x,WrD:%x\n",(USB3p0_CFG_BASE_ADDR+usb3_caplength+0x38), usb3_read_data);  

	//Program the 64-bit address pointer of Device Context Base Address Array Pointer Register(DCBAAP)
	//usb3_read_data[63:6] = {32'h0, usb3_xHC_DCBAR_addr[31:6]}; 
	//usb3_read_data[5:0]  = 6'h0;        //Reserved
	usb3_read_data = ((usb3_xHC_DCBAR_addr & 0xFFFFFFC0) | 0x0);
	writel( usb3_read_data,(USB3p0_CFG_BASE_ADDR+usb3_caplength+0x30));
	dbg_usb("USB3CONTROLLER DCBAAP REG[31:0] @ A:%x,WrD:%x\n",(USB3p0_CFG_BASE_ADDR+usb3_caplength+0x30), usb3_read_data);
	usb3_read_data = 0x0;
	writel( usb3_read_data,(USB3p0_CFG_BASE_ADDR+usb3_caplength+0x34));
	dbg_usb("USB3CONTROLLER DCBAAP REG[63:32] @ A:%x,WrD:%x\n",(USB3p0_CFG_BASE_ADDR+usb3_caplength+0x34), usb3_read_data);

	//Program the Command Ring Dequeue Pointer of Command Ring Control Register(CRCR)
	////usb3_read_data[63:32] = 0;                             //Command Ring pointer Hi
	//dbg_usb("usb3_xHC_CommandRing_Segment_addr= %x\n", usb3_xHC_CommandRing_Segment_addr);
	//usb3_read_data[63:6]  = {32'h0, usb3_xHC_CommandRing_Segment_addr[31:6]};  //Command Ring pointer Lo
	//usb3_read_data[5:4]   = 2'b00;                             //Reserved
	//usb3_read_data[3]     = 1'b0;                             //Command Ring Running
	//usb3_read_data[2]     = 1'b0;                             //Command Abort
	//usb3_read_data[1]     = 1'b0;                             //Command Stop
	//usb3_read_data[0]     = 1'b1;                             //Ring Cycle State
	usb3_read_data = ((usb3_xHC_CommandRing_Segment_addr & 0xFFFFFFC0) | 0x1);
	dbg_usb("usb3_read_data crcr = %x\n", usb3_read_data);
	writel(usb3_read_data, (USB3p0_CFG_BASE_ADDR+usb3_caplength+0x18)); //[31:0],(USB3p0_CFG_BASE_ADDR+usb3_caplength+0x18));
	dbg_usb("USB3CONTROLLER COMMAND RING CONTROL REG[31:0] @ A:%x,WrD:%x\n",(USB3p0_CFG_BASE_ADDR+usb3_caplength+0x18), usb3_read_data); //[31:0]);
	usb3_read_data = 0x0000;
	writel(usb3_read_data, (USB3p0_CFG_BASE_ADDR+usb3_caplength+0x1c)); //[63:32],(USB3p0_CFG_BASE_ADDR+usb3_caplength+0x1c));
	dbg_usb("USB3CONTROLLER COMMAND RING CONTROL REG[61:32] @ A:%x,WrD:%x\n",(USB3p0_CFG_BASE_ADDR+usb3_caplength+0x1c), usb3_read_data); //[63:32]);

	//Read the Runtime Base address of Runtime Register Space Offset(RTSOFF)
	usb3_read_data = readl((USB3p0_CFG_BASE_ADDR +0x18));
	dbg_usb("USB3CONTROLLER RTSOFF REG @ A:%x,RdD:%x\n",(USB3p0_CFG_BASE_ADDR +0x18),usb3_read_data);
	//usb3_Runtime_Base_addr = usb3_read_data[31:5];
	usb3_Runtime_Base_addr = (usb3_read_data & 0xFFFFFFE0);
	dbg_usb("usb3_Runtime_Base_addr = %x\n", usb3_Runtime_Base_addr);
} //usb3_xHC_controller_regs_config
//****************************************************************************************//
//Initialize Device Context 
void usb3_xHC_device_context_init(void)
{
	dbg_usb("*** Initializing the USB3 xHC Controller host device context space at shared memory ****\n");
	// Initialize the Device Context
	for(usb3_i=0; usb3_i < 64; usb3_i=usb3_i+1) 
	{
		writel( 0,(usb3_xHC_Device_Context_addr + usb3_i*0x4)); // Bus access to AHB Slave memory
		//dbg_usb("A:%x,WrD:%x\n",(usb3_xHC_Device_Context_addr + usb3_i*4), 0); // Bus access to AHB Slave memory
	}
}//usb3_xHC_device_context_init
//****************************************************************************************//
// Initialize the Event Ring Segments to '0'
void usb3_xHC_event_TRBs_init(void)
{
	usb3_Event_TRBs = 64; 
	dbg_usb("*** Initializing the USB3 xHC Controller host event TRB's space at shared memory ****\n");
	for(usb3_i=0; usb3_i < (usb3_Event_TRBs*4); usb3_i=usb3_i+1) 
	{
		writel( 0,(usb3_xHC_EventRing_Segment_addr + (usb3_i*0x4))); // Bus access to AHB Slave memory
		//dbg_usb("A:%x,WrD:%x\n",(usb3_xHC_EventRing_Segment_addr + usb3_i*0x4), 0); // Bus access to AHB Slave memory
	}
} //usb3_xHC_event_TRBs_init
//****************************************************************************************//
//Initialize CommandRing Segments to 0
void usb3_xHC_command_TRBs_init(void)
{
	dbg_usb("*** Initializing the USB3 xHC Controller host command ring space at shared memory ****\n");
	for(usb3_i=0; usb3_i < 100; usb3_i=usb3_i+1)
	{
		writel( 0x0,(usb3_xHC_CommandRing_Segment_addr + (usb3_i*0x4)));
		//dbg_usb("A:%x,WrD:%x\n",(usb3_xHC_CommandRing_Segment_addr + usb3_i*0x4), 0x0);
	}
}//usb3_xHC_command_TRBs_init
//****************************************************************************************//
//Initialize TransferRing Segments to 0
void usb3_xHC_transfer_TRBs_init(void)
{
	dbg_usb("*** Initializing the USB3 xHC Controller host transfer ring space at shared memory ****\n");
	for(usb3_i=0; usb3_i < 100; usb3_i=usb3_i+1) 
	{
		writel( 0x0,(usb3_xHC_TransferRing_Segment_addr+ (usb3_i*0x4)));
		//dbg_usb("A:%x,WrD:%x\n",(usb3_xHC_TransferRing_Segment_addr+ usb3_i*0x4), 0x0);
	}
} //usb3_xHC_transfer_TRBs_init
//****************************************************************************************//
//Declaring xHC device context data structure
void usb3_xHC_device_context_datastructure_init(void)
{
	dbg_usb("*** Initializing the USB3 xHC Controller host device context data structure at  shared memory ****\n");
	// Initialize Device Context Base Address Array
	writel( usb3_xHC_Scratch_Pad_Array_addr,(usb3_xHC_DCBAR_addr+0x0));
	dbg_usb("A:%x,WrD:%x\n",usb3_xHC_DCBAR_addr, usb3_xHC_Scratch_Pad_Array_addr);
	writel( 0,(usb3_xHC_DCBAR_addr+0x4));
	dbg_usb("A:%x,WrD:%x\n",(usb3_xHC_DCBAR_addr+0x4), 0);
	writel( usb3_xHC_Device_Context_addr,(usb3_xHC_DCBAR_addr+0x8));
	dbg_usb("A:%x,WrD:%x\n",(usb3_xHC_DCBAR_addr+0x8), usb3_xHC_Device_Context_addr);
	writel( 0,(usb3_xHC_DCBAR_addr+0xc));
	dbg_usb("A:%x,WrD:%x\n",(usb3_xHC_DCBAR_addr+0xc), 0);
} //usb3_xHC_device_context_datastructure_init
//****************************************************************************************//
//Declaring the xHC scratchpad buffer pointer.
void usb3_xHC_scratchpad_buffer_datastructure_init(void)
{
	dbg_usb("*** Initializing the USB3 xHC Controller host scrtach pad array at  shared memory ****\n");
	writel( usb3_xHC_Scratch_Pad_buf0_addr,(usb3_xHC_Scratch_Pad_Array_addr+0x0));
	dbg_usb("A:%x,WrD:%x\n",usb3_xHC_Scratch_Pad_Array_addr, usb3_xHC_Scratch_Pad_buf0_addr);
	writel( 0,(usb3_xHC_Scratch_Pad_Array_addr+0x4));
	dbg_usb("A:%x,WrD:%x\n",(usb3_xHC_Scratch_Pad_Array_addr+0x4), 0);
} //usb3_xHC_scratchpad_buffer_datastructure_init
//****************************************************************************************//
//Declaring the xHC Event Ring Segement Table
void usb3_xHC_eventring_datastructure_init(void)
{
	dbg_usb("*** Initializing the USB3 xHC Controller host Event ring segment table at  shared memory ****\n");
	//Program Event Ring Segment Table(ERST)
	//tmp_data[63:32] = 0;                           // Hi address
	//tmp_data[31:0]  = usb3_xHC_EventRing_Segment_addr;  // Lo address
	usb3_read_data  = usb3_xHC_EventRing_Segment_addr; 
	writel( usb3_read_data,(usb3_xHC_EventRing_Segment_table_addr+0x0));
	dbg_usb("A:%x,WrD:%x\n",usb3_xHC_EventRing_Segment_table_addr, usb3_read_data);
	usb3_read_data = 0;
	writel( usb3_read_data,(usb3_xHC_EventRing_Segment_table_addr+0x4)); //Hi address
	dbg_usb("A:%x,WrD:%x\n",(usb3_xHC_EventRing_Segment_table_addr+0x4), usb3_read_data); //Hi address
	//usb3_read_data[31:16] = 0;                           // Reserved 
	//usb3_read_data[15:0]  = 64;                          // Ring Segment Size
	usb3_read_data = 0x0040;      
	writel( usb3_read_data,(usb3_xHC_EventRing_Segment_table_addr+0x8));
	dbg_usb("A:%x,WrD:%x\n",(usb3_xHC_EventRing_Segment_table_addr+0x8), usb3_read_data);
	usb3_read_data = 0x0000;
	writel( usb3_read_data,(usb3_xHC_EventRing_Segment_table_addr+0xc));
	dbg_usb("A:%x,WrD:%x\n",(usb3_xHC_EventRing_Segment_table_addr+0xc), usb3_read_data);
	// Note: The upper 32-bit field is Reserved.
} //usb3_xHC_eventring_datastructure_init
//****************************************************************************************//
//Initializing the xHC interrupter registers
void usb3_xHC_interrupter_regs_init(void)
{
	dbg_usb("*** Initializing the USB3 xHC Controller host interrupter ****\n");
	//Program ERST size(ERSTSZ) Register
	//usb3_read_data[31:16] = 0;         // Reserved
	//usb3_read_data[15:0]  = 1;         // Number of Event Ring segment
	usb3_read_data = 0x0001;
	writel( usb3_read_data,(USB3p0_CFG_BASE_ADDR+usb3_Runtime_Base_addr+0x028)); // Only has one Event Ring, Interrupter=0 
	dbg_usb("USB3CONTROLLER ERSTSZ REG @ A:%x,WrD:%x\n",(USB3p0_CFG_BASE_ADDR+usb3_Runtime_Base_addr+0x028), usb3_read_data); 

	//Program Event Ring Dequeue Pointer(ERDP) Register
	//usb3_read_data[63:4]  = {32'h0, usb3_xHC_EventRing_Segment_addr[31:4]};     // Lo address (set Dequeue pointer = Segment address) 
	//usb3_read_data[3]     = 0;                             // Event Handler Busy
	//usb3_read_data[2:0]   = 0;                             // Dequeue ERST Segment Index
	usb3_read_data = ((usb3_xHC_EventRing_Segment_addr & 0xFFFFFFF0) | 0x0);
	writel( usb3_read_data,(USB3p0_CFG_BASE_ADDR+usb3_Runtime_Base_addr+0x038)); 
	dbg_usb("USB3CONTROLLER ERDP REG @ A:%x,WrD:%x\n",(USB3p0_CFG_BASE_ADDR+usb3_Runtime_Base_addr+0x038), usb3_read_data); 

	//Program Event Ring Segment table base address(ERSTBA)
	//usb3_read_data[63:4]  = {32'h0, usb3_xHC_EventRing_Segment_table_addr[31:4]};    // Lo address
	//usb3_read_data[3:0]   = 0;                                   // Reserved
	usb3_read_data = ((usb3_xHC_EventRing_Segment_table_addr & 0xFFFFFFF0) | 0x0);
	writel( usb3_read_data,(USB3p0_CFG_BASE_ADDR+usb3_Runtime_Base_addr+0x030)); //31:0
	dbg_usb("USB3CONTROLLER ERSTBA REG[31:0] @ A:%x,WrD:%x\n",(USB3p0_CFG_BASE_ADDR+usb3_Runtime_Base_addr+0x030), usb3_read_data); //31:0
	usb3_read_data = 0x0;
	writel( usb3_read_data,(USB3p0_CFG_BASE_ADDR+usb3_Runtime_Base_addr+0x034));  //63:32
	dbg_usb("USB3CONTROLLER ERSTBA REG[61:32] @ A:%x,WrD:%x\n",(USB3p0_CFG_BASE_ADDR+usb3_Runtime_Base_addr+0x034), usb3_read_data);  //63:32

	//Enable xHC interrupt(Interrupter Enable(INTE) bit 2 of USB Command Register(USBCMD) to 1)
	//usb3_read_data[31:12] = 0;       //Reserved
	//usb3_read_data[11]    = 0;       //Enable U3 MFINDEX Stop
	//usb3_read_data[10]    = 0;       //Enable Wrap Event
	//usb3_read_data[9]     = 0;       //Controller Restore State
	//usb3_read_data[8]     = 0;       //Controller Save States
	//usb3_read_data[7]     = 0;       //Light Host Controller Reset
	//usb3_read_data[6:4]   = 0;       //Reserved
	//usb3_read_data[3]     = 0;       //Host System Error Enable
	//usb3_read_data[2]     = 1;       //Interrupter Enable
	//usb3_read_data[1]     = 0;       //Host Controller Reset
	//usb3_read_data[0]     = 0;       //Run/Stop
	usb3_read_data = 0x00000004;
	writel( usb3_read_data,(USB3p0_CFG_BASE_ADDR+usb3_caplength));
	dbg_usb("USB3CONTROLLER USBCMD REG @ A:%x,WrD:%x\n",(USB3p0_CFG_BASE_ADDR+usb3_caplength), usb3_read_data);

	//Enable Interrupter0(Interrupt Enable(IE) bit 1 of Interrupter Management Register(IMAN) to 1)
	//usb3_read_data1[31:2] = 0;        //Reserved
	//usb3_read_data1[1]    = 1;        //Interrupt Enable
	//usb3_read_data1[0]    = 0;        //Interrupt Pending
	usb3_read_data1 = 0x00000002;
	writel( usb3_read_data1,(USB3p0_CFG_BASE_ADDR+usb3_Runtime_Base_addr+0x020)); 
	dbg_usb("USB3CONTROLLER IMAN REG A:%x,WrD:%x\n",(USB3p0_CFG_BASE_ADDR+usb3_Runtime_Base_addr+0x020), usb3_read_data1); 

	//Set interrupt moderation interval 
	//usb3_read_data1[15:0] = 500;        //Moderation interval
	//usb3_read_data1[31:16]  = 500;      //Moderation Counter
	usb3_read_data1 = 0x01F401F4;
	writel( usb3_read_data1,(USB3p0_CFG_BASE_ADDR+usb3_Runtime_Base_addr+0x024)); 
	dbg_usb("USB3CONTROLLER INTERRUPT MODERATION INTERVAL REG @ A:%x,WrD:%x\n",(USB3p0_CFG_BASE_ADDR+usb3_Runtime_Base_addr+0x024), usb3_read_data1); 
} //usb3_xHC_interrupter_regs_init
//****************************************************************************************//
//Enabling the RUN bit on xHC-USBCMD register to excecute outstanding commands
void usb3_xHC_enable_run_mode(void)
{
	dbg_usb("*** Enabling Run bit of USB3 xHC Controller ****\n");
	//Programming the USBCMD register
	//usb3_read_data[31:12] = 0;       //Reserved
	//usb3_read_data[11]    = 0;       //Enable U3 MFINDEX Stop
	//usb3_read_data[10]    = 0;       //Enable Wrap Event
	//usb3_read_data[9]     = 0;       //Controller Restore State
	//usb3_read_data[8]     = 0;       //Controller Save States
	//usb3_read_data[7]     = 0;       //Light Host Controller Reset
	//usb3_read_data[6:4]   = 0;       //Reserved
	//usb3_read_data[3]     = 0;       //Host System Error Enable
	//usb3_read_data[2]     = 1;       //Interrupter Enable
	//usb3_read_data[1]     = 0;       //Host Controller Reset
	//usb3_read_data[0]     = 0;       //Run/Stop
	usb3_read_data = 0x00000004;

	//Setting the Run/Stop(R/S) bit 0 of USB Command Register(USBCMD) to 1
	//usb3_read_data[0]     = 1;       //Run/Stop
	usb3_read_data = (usb3_read_data | 0x1);
	writel( usb3_read_data,(USB3p0_CFG_BASE_ADDR +usb3_caplength));
	dbg_usb("USB3CONTROLLER USBCMD REG @ A:%x,WrD:%x\n",(USB3p0_CFG_BASE_ADDR +usb3_caplength), usb3_read_data);

	dbg_usb("USB3:Run/Stop bit is set \n"); 
	usb3_read_data = readl((USB3p0_CFG_BASE_ADDR+usb3_caplength+0x04)); 
	dbg_usb("USB3CONTROLLER USBSTS REG @ A:%x,RdD:%x\n",(USB3p0_CFG_BASE_ADDR+usb3_caplength+0x04),usb3_read_data); 
	dbg_usb("USB3:HCH bit = %x \n", usb3_read_data); 
} //usb3_xHC_enable_run_mode
//****************************************************************************************//
void usb3_wait_untill_phy_vbus_is_high(void)
{
	u32 temp_rd_data=0;
	dbg_usb("*** Waiting for VBUS high from usb3 phy ****\n");
	//Reading USB3 PHY status register #0x904A_0030
	temp_rd_data= readl(0x904a0030);
	dbg_usb("USB3PHY STATUS REG @ A:%x,RdD:%x\n",0x904a0030,temp_rd_data);
	//Check for VBUS valid indicator at bit[6].
	//Loop untill VBUS goes high on Phy
	// while((temp_rd_data & 0x40) != 0x40) Rohit
	nop (10000);
	//Check for OTG Device Session Valid Indicator (Low Voltage)-bit[4]
	//Loop untill otgsessvld0 goes high on Phy
	while((temp_rd_data & 0x00) != 0x00)
	{
		temp_rd_data= readl(0x904a0030);
		dbg_usb("USB3PHY STATUS REG @ A:%x,RdD:%x\n",0x904a0030,temp_rd_data);
	}
	dbg_usb("USB3: VBUS is High on USB3 PHY\n");
	//Reading the USB3 Controller status register #904A_001C
	//Bit[3] represents VBUS states on superspeed port.
	//Bit[2] represents VBUS states on usb2 port.
	temp_rd_data=readl(0x904a001c);
	dbg_usb("USB3CONTROLLER STATUS REG @ A:%x,RdD:%x\n",0x904a001c,temp_rd_data);
	//Checking the usb3 controller hub_vbus_ctrl[1:0] status...
	if((temp_rd_data & 0x8) == 0x8)
	{
		dbg_usb("USB3:VBUS is ON on SS port\n");
	}
	else
	{
		dbg_usb("USB3:VBUS is OFF on SS port\n");
	}

	//while ((temp_rd_data & 0x4) != 0x4)
	//{
	//  dbg_usb("USB3:VBUS is OFF on USB2.0 port\n");
	//  temp_rd_data=readl(0x904a001c);
	//  dbg_usb("USB3CONTROLLER STATUS REG @ A:%x,RdD:%x\n",0x904a001c,temp_rd_data);
	//}
	//dbg_usb("USB3:VBUS is ON on USB2.0 port\n");
	//
	if((temp_rd_data & 0x4) != 0x4)
		dbg_usb("USB3:VBUS is OFF on USB2.0 port\n");
	else
		dbg_usb("USB3:VBUS is ON on USB2.0 port\n");

} //usb3_wait_untill_phy_vbus_is_high
//****************************************************************************************//
//Polling the GPIO register
int usb3_wait_for_interrupt(void)
{
	//-----------------------------------------
	pmu_intr_status=readl(0x9047015C);
	dbg_usb("GPIO REG @ A:%x,RdD:%x\n",0x9047015C,pmu_intr_status);
	dbg_usb("GPIO:Waiting for usb3p0 interrupt...\n");
	while((pmu_intr_status & 0x00100000) != 0x00100000)
	{
		pmu_intr_status=readl(0x9047015C);

		if (ctrlc())
			return -1;
	}
	//-----------------------------------------
	//a9_irq_bus_31_0_bits_status=readl(A9_IRQ_31_0_BITS_STATUS_ADDR_AT_IRAM);
	//dbg_usb("Waiting for usb3p0 interrupt...\n");
	//while((a9_irq_bus_31_0_bits_status & 0x400000) != 0x400000)
	//{
	//  a9_irq_bus_31_0_bits_status=readl(A9_IRQ_31_0_BITS_STATUS_ADDR_AT_IRAM);
	//}
	////dbg_usb("A:%x,RdD:%x\n",A9_IRQ_31_0_BITS_STATUS_ADDR_AT_IRAM,a9_irq_bus_31_0_bits_status);
	dbg_usb("USB3 INTERRUPT DETECTED.\n");
	return 0;
} //usb3_wait_for_interrupt
//****************************************************************************************//
void gpio_interrupt_status_register_clear(void)
{ 
	dbg_usb("*** Clearing the GPIO raw interrupt status register ****\n");
	pmu_intr_status=readl(0x9047015C);
	dbg_usb("GPIO REG @ A:%x,RdD:%x\n",0x9047015C,pmu_intr_status);
	writel(pmu_intr_status,0x90470160);
	dbg_usb("GPIO REG @ A:%x,WrD:%x\n",0x90470160,pmu_intr_status); 
	pmu_intr_status=readl(0x9047015C);
	dbg_usb("GPIO REG @ A:%x,RdD:%x\n",0x9047015C,pmu_intr_status);
	dbg_usb("GPIO:RAW INTERRUPT STATUS REGISTER CLEARED\n");
} //gpio_interrupt_status_register_clear
//****************************************************************************************//
//Clear the xHC event interrupts
void usb3_xHC_clear_event_interupt(void)
{
	dbg_usb("*** Clearing the event interrupt generated by USB3 xHC Controller ****\n");
	//Clear Event Interrupt in USBSTS & Halt the execution
	//usb3_read_data = 0;
	//usb3_read_data[3]     = 1;       //Run/Stop
	usb3_read_data = 0x8;
	writel( usb3_read_data,(USB3p0_CFG_BASE_ADDR+usb3_caplength+0x04));
	dbg_usb("USB3 CONTROLLER USBSTS REG @ A:%x,WrD:%x\n",(USB3p0_CFG_BASE_ADDR+usb3_caplength+0x04), usb3_read_data);

	//Clear the Interrupt Pending in Interrupter Managment Register(IMAN)
	//usb3_read_data = 0;
	//usb3_read_data[1:0] = 2'b11;
	usb3_read_data = 0x3;
	writel( usb3_read_data,(USB3p0_CFG_BASE_ADDR+usb3_Runtime_Base_addr+0x020));
	dbg_usb("USB3 CONTROLLER IMAN REG @ A:%x,WrD:%x\n",(USB3p0_CFG_BASE_ADDR+usb3_Runtime_Base_addr+0x020), usb3_read_data);

	// Update Event Ring Dequeue Pointer
	usb3_hst_event_count = usb3_hst_event_count + 0x10;
	nop(20);//Delay
	usb3_read_data = (usb3_xHC_EventRing_Segment_addr + usb3_hst_event_count);  
	//usb3_read_data[3:0]  = 4'b1000;
	usb3_read_data = ((usb3_read_data & 0xFFFFFFF0) | 0x8);
	writel( usb3_read_data,(USB3p0_CFG_BASE_ADDR+usb3_Runtime_Base_addr+0x038));
	dbg_usb("USB3 CONTROLLER ERDP REG[31:0] @ A:%x,WrD:%x\n",(USB3p0_CFG_BASE_ADDR+usb3_Runtime_Base_addr+0x038), usb3_read_data);
	usb3_read_data = 0x0;
	writel( usb3_read_data,(USB3p0_CFG_BASE_ADDR+usb3_Runtime_Base_addr+0x03c));
	dbg_usb("USB3 CONTROLLER ERDP REG[61:31] @ A:%x,WrD:%x\n",(USB3p0_CFG_BASE_ADDR+usb3_Runtime_Base_addr+0x03c), usb3_read_data);
} //usb3_xHC_clear_event_interupt
//****************************************************************************************//
//Programming the PortStatus & Controller Register bit
void usb3_xHC_portsc_reg_init(void)
{
	dbg_usb("*** Programming the Port Power ON for both SS & 20 port ****\n");
	//Port Power will be ON for both SS and 20 by default beging Rel 1.20a 
	//multiport: Remove Port Power on unactive ports so interrupt only generated for active port 
	//usb3_read_data     = 0;
	//usb3_read_data[9]  = 1'b0;
	usb3_read_data = 0;
	usb3_ss_port_addr  = (0x400 + (DWC_USB3_HOST_NUM_U2_ROOT_PORTS * 0x10));
	for(usb3_i=1; usb3_i<DWC_USB3_HOST_NUM_U3_ROOT_PORTS; usb3_i=usb3_i+1)
	{
		writel( usb3_read_data,(USB3p0_CFG_BASE_ADDR+usb3_caplength+usb3_ss_port_addr+(usb3_i*0x10)));
		dbg_usb("USB3CONTROLLER PORTSC REG @ A:%x,WrD:%x\n",(USB3p0_CFG_BASE_ADDR+usb3_caplength+usb3_ss_port_addr+(usb3_i*0x10)), usb3_read_data);
	}
} //usb3_xHC_portsc_reg_init
//****************************************************************************************//
//Initializing the Port reset sequence on USB3 (HS/FS) port and SS ports.
//HS/FS port is always 1
//1st SS port # = USB2.0 number of port +1 
//Ex: PORTSC 32'h400 = Port1, 32'h410 = Port2, etc.. 
void usb3_xHC_apply_portreset_sequence(void)
{
	//dbg_usb("HS:PORT RST\n");
	dbg_usb("*** USB3:Configuring the Host Port & resetting it for speed negotiation ****\n");
	//Note that software shall write a 1 to bit[4] of PORTSC register to transition a 
	//USB2 port from the Polling state to the Enabled state
	usb3_read_data = 0x0;
	usb3_hs_port_addr  = 0x3F0+(usb3_hs_port_number*0x10);

	usb3_read_data  = readl(USB3p0_CFG_BASE_ADDR + usb3_caplength + usb3_hs_port_addr);
	dbg_usb("USB3CONTROLLER PORTSC REG @ A:%x,RdD:%x\n",(USB3p0_CFG_BASE_ADDR + usb3_caplength + usb3_hs_port_addr),usb3_read_data);
	usb3_read_data  = (usb3_read_data | 0x00000010); // Current PORSTC before setting USB Reset
	writel( usb3_read_data,(USB3p0_CFG_BASE_ADDR + usb3_caplength + usb3_hs_port_addr));
	//dbg_usb("A:%x,WrD:%x\n",(USB3p0_CFG_BASE_ADDR + usb3_caplength + usb3_hs_port_addr),usb3_read_data);
	usb3_read_data  = readl(USB3p0_CFG_BASE_ADDR + usb3_caplength + usb3_hs_port_addr);
	//dbg_usb("A:%x,RdD:%x\n",(USB3p0_CFG_BASE_ADDR + usb3_caplength + usb3_hs_port_addr),usb3_read_data);
	while ((usb3_read_data & 0x10) != 0x10)
	{
		dbg_usb("USB3:HS Port is not in reset\n");
		usb3_read_data  = (usb3_read_data | 0x00000010); // Current PORSTC before setting USB Reset
		writel( usb3_read_data,(USB3p0_CFG_BASE_ADDR + usb3_caplength + usb3_hs_port_addr));
		//dbg_usb("A:%x,WrD:%x\n",(USB3p0_CFG_BASE_ADDR + usb3_caplength + usb3_hs_port_addr),usb3_read_data);
		usb3_read_data  = readl(USB3p0_CFG_BASE_ADDR + usb3_caplength + usb3_hs_port_addr);
		//dbg_usb("A:%x,RdD:%x\n",(USB3p0_CFG_BASE_ADDR + usb3_caplength + usb3_hs_port_addr),usb3_read_data);
	}
	dbg_usb("USB3CONTROLLER PORTSC REG @ A:%x,RdD:%x\n",(USB3p0_CFG_BASE_ADDR + usb3_caplength + usb3_hs_port_addr),usb3_read_data);
	dbg_usb("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
	dbg_usb("*** The USB3 HOST CONTROLLER HS(HIGH SPEED) PORT IS IN RESET ****\n");
	dbg_usb("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
} //usb3_xHC_apply_portreset_sequence
//****************************************************************************************//
void usb3_wait_till_host_port_enabled(void)
{
	usb3_read_data  = readl(USB3p0_CFG_BASE_ADDR + usb3_caplength + usb3_hs_port_addr);
	dbg_usb("USB3:PORTSC RdD=%x\n",usb3_read_data);
	while ((usb3_read_data & 0x2) != 0x2)
	{
		dbg_usb("USB3: Host Port not enabled\n");
		usb3_read_data  = readl(USB3p0_CFG_BASE_ADDR + usb3_caplength + usb3_hs_port_addr);
		dbg_usb("USB3:PORTSC RdD=%x\n",usb3_read_data);
	}
	dbg_usb("USB3: Host Port is Now Enabled\n");

}//usb3_wait_till_host_port_enabled
//****************************************************************************************//
void usb3_host_get_ss_port_status(void)
{
	usb3_read_data = readl(0x9f000430);
	dbg_usb("USB3 HOST PORTSC REG @ A:%x,RdD:%x\n",0x9f000430,usb3_read_data);
}

//****************************************************************************************//
//Acknowledge the port status change interrupt
int usb3_xHC_ack_port_status_change_interrupt(void)
{
	int temp_i=0;
	int temp_j=0;
	int temp_k=0;
	int err_count=0;
	//Read the Interrpter Register Set 0 for Event Ring Dequeue Pointer(ERDP)
	//usb3_read_data = readl((USB3p0_CFG_BASE_ADDR+usb3_Runtime_Base_addr+0x20)+0x24);
	//usb3_ERDP = usb3_read_data[31:4];
	//Read Port Status Change Event TRB for Port ID
	dbg_usb("*** Reading the usb3 controller host Port Status Change Event TRB ****\n");
	usb3_read_data = readl((usb3_xHC_EventRing_Segment_addr+usb3_hst_event_count));
	dbg_usb("A:%x,RdD:%x\n",(usb3_xHC_EventRing_Segment_addr+usb3_hst_event_count),usb3_read_data);
	usb3_PortID = ((usb3_read_data & 0xFF000000) >> 24);//usb3_read_data[31:24];
	dbg_usb("Port ID = %x \n", usb3_PortID);
	//Check TRB type, completion code and cycle bit status
	usb3_read_data = readl((usb3_xHC_EventRing_Segment_addr+usb3_hst_event_count+0x8));
	dbg_usb("A:%x,RdD:%x\n",(usb3_xHC_EventRing_Segment_addr+usb3_hst_event_count+0x8),usb3_read_data);
	while ((temp_i ==0) || (temp_j ==0) || (temp_k ==0))
	{
		if(ctrlc())
			return -1;

		if((usb3_read_data & 0xFF000000) != 0x01000000)
		{
			dbg_usb("FAILED : Completion status is not Success\n");
			err_count++;
		} 
		else if(((usb3_read_data & 0xFF000000) == 0x01000000) && (temp_i==0))
		{
			dbg_usb("Completion status is Success\n");
			temp_i++;
		}    
		usb3_read_data = readl((usb3_xHC_EventRing_Segment_addr+usb3_hst_event_count+0xc));
		dbg_usb("A:%x,RdD:%x\n",(usb3_xHC_EventRing_Segment_addr+usb3_hst_event_count+0xc),usb3_read_data);
		if ((usb3_read_data & 0x01) != 0x01)
		{
			dbg_usb("FAILED : Cycle bit (C) is not set to 1\n");
			err_count++;
		}
		else if (((usb3_read_data & 0x01) == 0x01) && (temp_j ==0))
		{
			dbg_usb("Cycle bit (C) is  set to 1\n");
			temp_j++;
		}
		if ((usb3_read_data & 0xFC00)  != 0x8800)
		{
			dbg_usb("FAILED : TRB type is not a Port Status Change Event\n");
			err_count++;
		}
		else if (((usb3_read_data & 0xFC00)  == 0x8800) && (temp_k==0))
		{
			dbg_usb("TRB type is a Port Status Change Event\n");
			temp_k++;
		}
	}//while

	if(err_count ==0)
	{
		dbg_usb("*********************************************\n");
		dbg_usb("*** COMMAND COMPLETION IS SUCESS ****\n");
		dbg_usb("*********************************************\n");
		device_connected_on_ss_port=1;
		return 0;
	}
	else
	{
		printf("*********************************************\n");
		printf("*** COMMAND COMPLETION IS FAILED ****\n");
		printf("*********************************************\n");
		device_connected_on_ss_port=0;
		return -1;
	}

} //usb3_xHC_ack_port_status_change_interrupt
//****************************************************************************************//
//Check xHC in Enabled State from Port Status and Control Register(PORTSC)
void usb3_xHC_state_check(void)
{
	dbg_usb("*** Reading the usb3 controller host PORTSC regsiter ****\n");
	usb3_read_data = readl((USB3p0_CFG_BASE_ADDR+usb3_caplength+0x400+(0x10*(usb3_PortID-1))));
	dbg_usb("A:%x,RdD:%x\n",(USB3p0_CFG_BASE_ADDR+usb3_caplength+0x400+(0x10*(usb3_PortID-1))),usb3_read_data);
	if ((usb3_read_data & 0x2) != 0x2)
	{
		dbg_usb("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n"); 
		dbg_usb("*** THE USB3 HOST CONTROLLER IS IN 'DISABLED' STATE ****\n");
		dbg_usb("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n"); 
	}
	else
	{
		dbg_usb("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n"); 
		dbg_usb("*** THE USB3 HOST CONTROLLER IS IN 'ENABLED' STATE ****\n");
		dbg_usb("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n"); 
	}
	//usb3_PortSpeed = usb3_read_data[13:10];
	usb3_PortSpeed = ((usb3_read_data & 0x3C00) >> 10);
	dbg_usb("USB3:Port Speed = %d\n", usb3_PortSpeed);
} //usb3_xHC_state_check
//****************************************************************************************//
// Enable Device Slot 
void usb3_xHC_enable_device_slot(void)
{ 
	//Issue Enable Slot Command
	dbg_usb("*** Issuing the Enable Slot Command from usb3 controller host ****\n"); 
	usb3_read_data = 0x0; //[31:0] 
	// Lower 12 bytes are Reserved 
	writel( 0x0,(usb3_xHC_CommandRing_Segment_addr+0x0));
	dbg_usb("A:%x,WrD:%x\n",usb3_xHC_CommandRing_Segment_addr, 0x0);
	writel( 0x0,(usb3_xHC_CommandRing_Segment_addr+0x4));
	dbg_usb("A:%x,WrD:%x\n",usb3_xHC_CommandRing_Segment_addr, 0x0);
	//usb3_read_data[31:0]  = 0;      // Reserved
	usb3_read_data = 0x0;
	writel( usb3_read_data,(usb3_xHC_CommandRing_Segment_addr+0x8));
	dbg_usb("A:%x,WrD:%x\n",(usb3_xHC_CommandRing_Segment_addr+0x8), usb3_read_data);
	//usb3_read_data[63:48] = 0;      // Reserved
	//usb3_read_data[47:42] = 9;      // TRB Type - Enable Slot Command
	//usb3_read_data[41:33] = 0;      // Reserved
	//usb3_read_data[32]    = 1;      // Cycle bit(C)
	usb3_read_data = 0x00002401; 
	writel( usb3_read_data,(usb3_xHC_CommandRing_Segment_addr+0xc));
	dbg_usb("A:%x,WrD:%x\n",(usb3_xHC_CommandRing_Segment_addr+0xc), usb3_read_data);

	usb3_read_data = readl((usb3_xHC_CommandRing_Segment_addr+0x0));
	dbg_usb("A:%x,RdD:%x\n",usb3_xHC_CommandRing_Segment_addr,usb3_read_data);
	usb3_read_data = readl((usb3_xHC_CommandRing_Segment_addr+0x4));
	dbg_usb("A:%x,RdD:%x\n",(usb3_xHC_CommandRing_Segment_addr+0x4),usb3_read_data);
	usb3_read_data1 = readl((usb3_xHC_CommandRing_Segment_addr+0x8));
	dbg_usb("A:%x,RdD:%x\n",(usb3_xHC_CommandRing_Segment_addr+0x8),usb3_read_data1);
	usb3_read_data1 = readl((usb3_xHC_CommandRing_Segment_addr+0xc));
	dbg_usb("A:%x,RdD:%x\n",(usb3_xHC_CommandRing_Segment_addr+0xc),usb3_read_data1);
} //usb3_xHC_enable_device_slot
//****************************************************************************************//
//Ring xHC Doorbell with "Host Controller Command"
void usb3_xHC_ring_doorbell_with_host_control_cmd(void)
{
	dbg_usb("*** Ringing the usb3 host Doorbell Register with host_control command ****\n");
	//Read Doorbell Array Base Address from Doorbell Offset Register(DBOFF)
	usb3_read_data = readl((USB3p0_CFG_BASE_ADDR+0x14));
	dbg_usb("USB3CONTROLLER DBOFF REG @ A:%x,RdD:%x\n",(USB3p0_CFG_BASE_ADDR+0x14),usb3_read_data);
	usb3_Doorbell_Array_Base_addr = usb3_read_data;  
	dbg_usb("usb3_Doorbell_Array_Base_addr = %x \n", usb3_Doorbell_Array_Base_addr);

	//Program Doorbell Register
	//usb3_read_data[31:16] = 0;       // DB Stream ID
	//usb3_read_data[15:8]  = 0;       // Reserved
	//usb3_read_data[7:0]   = 0;       // DB Target - Host Controller Command
	usb3_read_data = 0x0;
	//writel( usb3_read_data,usb3_Doorbell_Array_Base_addr);
	//dbg_usb("A:%x,WrD:%x\n",usb3_Doorbell_Array_Base_addr, usb3_read_data);
	writel( usb3_read_data,(USB3p0_CFG_BASE_ADDR+usb3_Doorbell_Array_Base_addr));
	// dbg_usb("A:%x,WrD:%x\n",(USB3p0_CFG_BASE_ADDR+usb3_Doorbell_Array_Base_addr), usb3_read_data);
}//usb3_xHC_ring_doorbell_with_host_control_cmd
//****************************************************************************************//
//Check for Device Slot availability
void usb3_xHC_check_for_device_slot(void)
{
	int err_count=0;
	dbg_usb("*** Checking the device slot availability from usb3 controller host ****\n");
	//Read Command Completion Event TRB for Slot ID
	usb3_read_data = readl((usb3_xHC_EventRing_Segment_addr+usb3_hst_event_count+0x8));
	dbg_usb("Read Command Completion Event TRB for Slot ID\n");
	dbg_usb("A:%x,RdD:%x\n",(usb3_xHC_EventRing_Segment_addr+usb3_hst_event_count+0x8),usb3_read_data);
	//if(usb3_read_data[31:24] !== 1) {
	if((usb3_read_data & 0xFF000000) != 0x01000000)
	{
		dbg_usb("^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
		dbg_usb(" FAIL: NO DEVICE Slots Available\n");
		dbg_usb("^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
		err_count++;
	}
	else
	{
		dbg_usb("^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
		dbg_usb(" DEVICE Slots Available\n");
		dbg_usb("^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
	}
	usb3_read_data = readl((usb3_xHC_EventRing_Segment_addr+usb3_hst_event_count+0xC));
	dbg_usb("A:%x,RdD:%x\n",(usb3_xHC_EventRing_Segment_addr+usb3_hst_event_count+0xC),usb3_read_data);
	usb3_SlotID = ((usb3_read_data & 0xFF000000) >>24);// usb3_read_data[31:24];
	dbg_usb("Slot ID = %d \n", usb3_SlotID);
	//Check TRB type, completion code and cycle bit status
	if((usb3_read_data & 0x1) != 0x1)
	{
		dbg_usb("FAIL:Cycle bit (C) is not set to 1\n");
		err_count++;
	}
	else
	{
		dbg_usb("Cycle bit (C) is set to 1\n");
	}

	//if(usb3_read_data[15:10] !== 33) {
	if((usb3_read_data & 0xFC00) != 0x8400) 
	{
		dbg_usb("FAIL:TRB type is not a Command Completion Event\n");
		err_count++;
	}
	else
	{
		dbg_usb("TRB type is a Command Completion Event\n");
	}

	if(err_count ==0)
	{
		dbg_usb("*********************************************\n");
		dbg_usb("*** COMMAND COMPLETION IS SUCESS ****\n");
		dbg_usb("*********************************************\n");
		device_slots_cmd_pass=1;
	}
	else
	{
		dbg_usb("*********************************************\n");
		dbg_usb("*** COMMAND COMPLETION IS FAILED ****\n");
		dbg_usb("*********************************************\n");
		device_slots_cmd_pass=0;
	}

} //usb3_xHC_check_for_device_slot
//****************************************************************************************//
// Issues an Address Device Command 
void usb3_xHC_issue_addr_device_cmd(void)
{
	dbg_usb("*** Issuing the 'set_address command' from usb3 controller host ****\n");
	//Initializes the Input Control Context with A0 and A1 to 1
	dbg_usb("USB3:Initializes the Input Context Data Structure\n");
	//usb3_read_data[63:32]  = 3;       // Add Context Flags
	//usb3_read_data[31:0]   = 0;       // Drop Context Flags
	usb3_read_data = 0x0;
	writel( usb3_read_data,(usb3_xHC_Input_Context_addr+0x0));
	dbg_usb("A:%x,WrD:%x\n",usb3_xHC_Input_Context_addr, usb3_read_data);
	usb3_read_data = 0x3;
	writel( usb3_read_data,(usb3_xHC_Input_Context_addr+0x4));
	dbg_usb("A:%x,WrD:%x\n",(usb3_xHC_Input_Context_addr+0x4), usb3_read_data);
	//usb3_read_data1[63:0]  = 0;       // Reserved
	usb3_read_data1 =0x0;
	writel(usb3_read_data1, (usb3_xHC_Input_Context_addr+0x8));//[31:0]
	dbg_usb("A:%x,WrD:%x\n",(usb3_xHC_Input_Context_addr+0x8), usb3_read_data1);//[31:0]);
	usb3_read_data1 = 0x0;
	writel(usb3_read_data1 ,(usb3_xHC_Input_Context_addr+0xc));//[63:32]
	dbg_usb("A:%x,WrD:%x\n",(usb3_xHC_Input_Context_addr+0xc), usb3_read_data1);//[63:32]);
	//usb3_read_data2[63:0]  = 0;       // Reserved
	usb3_read_data2 = 0x0;
	writel(usb3_read_data2, (usb3_xHC_Input_Context_addr+0x10));//[31:0]
	dbg_usb("A:%x,WrD:%x\n",(usb3_xHC_Input_Context_addr+0x10), usb3_read_data2);//[31:0]);
	usb3_read_data2 = 0x0;
	writel(usb3_read_data2, (usb3_xHC_Input_Context_addr+0x14));//[63:32]
	dbg_usb("A:%x,WrD:%x\n",(usb3_xHC_Input_Context_addr+0x14), usb3_read_data2);//[63:32]);
	//usb3_read_data3[63:0]  = 0;       // Reserved
	usb3_read_data3 = 0x0;
	writel(usb3_read_data3, (usb3_xHC_Input_Context_addr+0x18));//[31:0]
	dbg_usb("A:%x,WrD:%x\n",(usb3_xHC_Input_Context_addr+0x18), usb3_read_data3);//[31:0]);
	usb3_read_data3 = 0x0;
	writel(usb3_read_data3, (usb3_xHC_Input_Context_addr+0x1c));//[63:32]
	dbg_usb("A:%x,WrD:%x\n",(usb3_xHC_Input_Context_addr+0x1c), usb3_read_data3);//[63:32]);

	//Initializes Input Slot Context 
	//usb3_read_data[31:27]  = 1;            // Context Entries
	//usb3_read_data[26]     = 0;            // Hub
	//usb3_read_data[25]     = 0;            // Multi-TT
	//usb3_read_data[24]     = 0;            // Reserved
	//usb3_read_data[23:20]  = usb3_PortSpeed;    // Speed
	//usb3_read_data[23:20]  = 3;    // Speed
	//usb3_read_data[19:0]   = 0;            // Route String
	usb3_read_data = 0x08400000;
	//usb3_read_data1[31:24] = 0;            // Reserved
	//SS port = USB 2.0 number of port + 1
	//usb3_read_data1[23:16] = usb3_ss_port_number;            // Root Hub Port Number
	//usb3_read_data1[15:0]  = 0;            // Max Exit Latency
	usb3_read_data1 = ((usb3_ss_port_number <<16) & 0x00FF0000);
	//usb3_read_data2[31:22] = 0;            // Interrupter Targer
	//usb3_read_data2[21:16] = 0;            // Reserved
	//usb3_read_data2[15:8]  = 0;            // TT Port Number
	//usb3_read_data2[7:0]   = 0;            // TT Hub Slot ID
	usb3_read_data2 = 0x0;

	//usb3_read_data3[31:27] = 0;            // Slot State
	//usb3_read_data3[26:8]  = 0;            // Reserved
	//usb3_read_data3[7:0]   = 0;            // USB Device Address
	usb3_read_data3 = 0x0;

	// The remaining upper 16 bytes are Reserved
	writel( usb3_read_data,(usb3_xHC_Input_Context_addr+0x40));  
	dbg_usb("A:%x,WrD:%x\n",(usb3_xHC_Input_Context_addr+0x40), usb3_read_data);  
	writel( usb3_read_data1,(usb3_xHC_Input_Context_addr+0x44));  
	dbg_usb("A:%x,WrD:%x\n",(usb3_xHC_Input_Context_addr+0x44), usb3_read_data1);  
	writel( usb3_read_data2,(usb3_xHC_Input_Context_addr+0x48));  
	dbg_usb("A:%x,WrD:%x\n",(usb3_xHC_Input_Context_addr+0x48), usb3_read_data2);  
	writel( usb3_read_data3,(usb3_xHC_Input_Context_addr+0x4C));
	dbg_usb("A:%x,WrD:%x\n",(usb3_xHC_Input_Context_addr+0x4C), usb3_read_data3);

	//Initializes Input default Control Endpoint 0 Context
	//usb3_read_data[31:24]  = 0;             // Reserved
	//usb3_read_data[23:16]  = 0;             // Interval
	//usb3_read_data[15]     = 0;             // Linear Stream Array
	//usb3_read_data[14:10]  = 0;             // Max Primary Streams
	//usb3_read_data[9:8]    = 0;             // Mult
	//usb3_read_data[7:3]    = 0;             // Reserved
	//usb3_read_data[2:0]    = 0;             // Endpoint State
	usb3_read_data = 0x0;

	//usb3_read_data1[31:16] = 512;           // Maximum Packet Size
	//usb3_read_data1[15:8]  = 0;             // Maximum Burst Size
	//usb3_read_data1[7]     = 0;             // Host Initiate Disable
	//usb3_read_data1[6]     = 0;             // Reserved
	//usb3_read_data1[5:3]   = 4;             // Endpoint Type
	//usb3_read_data1[2:1]   = 3;             // Error Count 
	//usb3_read_data1[0]     = 0;             // Force Event
	usb3_read_data1 = 0x02000026;
	//usb3_read_data2[63:32] = 0;                               // TR Dequeue Pointer Address Hi
	//usb3_read_data2[31:4]  = usb3_xHC_TransferRing_Segment_addr[31:4];    // TR Dequeue Pointer Address Lo
	//usb3_read_data2[3:1]   = 0;                               // Reserved 
	//usb3_read_data2[0]     = 1;                               // Dequeue Cycle State
	usb3_read_data2 = ((usb3_xHC_TransferRing_Segment_addr & 0xFFFFFFF0) | 0x1);

	//usb3_read_data3[31:16] = 0;                               // Max Endpoint Service Time Interval Payload
	//usb3_read_data3[15:0]  = 512;                             // Average TRB Length
	usb3_read_data3 = 0x00000200;

	// The remaining upper 16 bytes are Reserved
	writel(usb3_read_data,(usb3_xHC_Input_Context_addr+0x80));  
	dbg_usb("A:%x,WrD:%x\n",(usb3_xHC_Input_Context_addr+0x80), usb3_read_data);  
	writel( usb3_read_data1,(usb3_xHC_Input_Context_addr+0x84));  
	dbg_usb("A:%x,WrD:%x\n",(usb3_xHC_Input_Context_addr+0x84), usb3_read_data1);  
	writel(usb3_read_data2,(usb3_xHC_Input_Context_addr+0x88));//[31:0]
	dbg_usb("A:%x,WrD:%x\n",(usb3_xHC_Input_Context_addr+0x88), usb3_read_data2);//[31:0]);
	usb3_read_data2 =0x0;
	writel(usb3_read_data2, (usb3_xHC_Input_Context_addr+0x8C));//[63:32]
	dbg_usb("A:%x,WrD:%x\n",(usb3_xHC_Input_Context_addr+0x8C), usb3_read_data2);//[63:32]);
	writel( usb3_read_data3,(usb3_xHC_Input_Context_addr+0x90));
	dbg_usb("A:%x,WrD:%x\n",(usb3_xHC_Input_Context_addr+0x90), usb3_read_data3);
	writel( 0x0,(usb3_xHC_Input_Context_addr+0x94));
	dbg_usb("A:%x,WrD:%x\n",(usb3_xHC_Input_Context_addr+0x94), 0x0);
	writel( 0x0,(usb3_xHC_Input_Context_addr+0x98));
	dbg_usb("A:%x,WrD:%x\n",(usb3_xHC_Input_Context_addr+0x98), 0x0);
	writel( 0x0,(usb3_xHC_Input_Context_addr+0x9c));
	dbg_usb("A:%x,WrD:%x\n",(usb3_xHC_Input_Context_addr+0x9c), 0x0);

	//Insert a pointer to DCBAA lookup table for the newly created Slot ID
	//usb3_read_data[63:32] = 0;                                // Hi Address
	//usb3_read_data[31:0]  = usb3_xHC_Device_Context_addr;          // Lo Address
	//usb3_read_data= usb3_xHC_Device_Context_addr;
	//writel( usb3_read_data,(usb3_xHC_DCBAR_addr+8)); // Slot ID 0 is for xHCI scratchpad 
	//dbg_usb("A:%x,WrD:%x\n",(usb3_xHC_DCBAR_addr+8), usb3_read_data); // Slot ID 0 is for xHCI scratchpad 

	//Issue Address Device Command
	dbg_usb("Issuing Address Device Command\n");
	//usb3_read_data[63:32] =0;                              // Hi Address
	//usb3_read_data[31:4]  = usb3_xHC_Input_Context_addr[31:4];  // Lo Address
	//usb3_read_data[3:0]   = 0;                             //Reserved 
	usb3_read_data = ((usb3_xHC_Input_Context_addr & 0xFFFFFFF0) | 0x0);

	//usb3_read_data1[31:24] = usb3_SlotID;      // Reserved
	//usb3_read_data1[23:16] = 0;           // Reserved
	//usb3_read_data1[15:10] = 11;          // TRB Type - Enable Slot Command
	//usb3_read_data1[9]     = 0;           // Block Set Address Request 
	//usb3_read_data1[8:1]   = 0;           // Reserved
	//usb3_read_data1[0]     = 1;           // Cycle bit(C)
	usb3_read_data1 = (((usb3_SlotID << 24) & 0xFF000000) | 0x00002C01);

	// 0B-08 is Reserved 
	writel(usb3_read_data,(usb3_xHC_CommandRing_Segment_addr+0x10));//[31:0]
	dbg_usb("A:%x,WrD:%x\n",(usb3_xHC_CommandRing_Segment_addr+0x10), usb3_read_data);//[31:0]);
	usb3_read_data = 0;
	writel(usb3_read_data, (usb3_xHC_CommandRing_Segment_addr+0x14));//[63:32]
	dbg_usb("A:%x,WrD:%x\n",(usb3_xHC_CommandRing_Segment_addr+0x14), usb3_read_data);//[63:32]);
	writel(usb3_read_data1,(usb3_xHC_CommandRing_Segment_addr+0x1C));
	dbg_usb("A:%x,WrD:%x\n",(usb3_xHC_CommandRing_Segment_addr+0x1C), usb3_read_data1);
	writel( 0x0,(usb3_xHC_CommandRing_Segment_addr+0x18));
	dbg_usb("A:%x,WrD:%x\n",(usb3_xHC_CommandRing_Segment_addr+0x18), 0x0);

	usb3_read_data1 = readl((usb3_xHC_CommandRing_Segment_addr+0x10));
	dbg_usb("A:%x,RdD:%x\n",(usb3_xHC_CommandRing_Segment_addr+0x10),usb3_read_data1);
	dbg_usb("usb3_xHC_Input_Context_addr Lo = %x \n", usb3_read_data1);
	usb3_read_data1 = readl((usb3_xHC_CommandRing_Segment_addr+0x14));
	dbg_usb("A:%x,RdD:%x\n",(usb3_xHC_CommandRing_Segment_addr+0x14),usb3_read_data1);
	dbg_usb("usb3_xHC_Input_Context_addr Hi = %x \n", usb3_read_data1);
	usb3_read_data = readl((usb3_xHC_CommandRing_Segment_addr+0x1C));
	dbg_usb("A:%x,RdD:%x\n",(usb3_xHC_CommandRing_Segment_addr+0x1C),usb3_read_data);
}//usb3_xHC_issue_addr_device_cmd
//****************************************************************************************//
//Check for command completion event after issuing host control command 
void usb3_xHC_check_cmd_completion_status(void)
{
	int err_count=0;
	dbg_usb("*** Checking the Command Completion Event TRB generated from usb3 host ****\n");
	usb3_read_data = readl((usb3_xHC_EventRing_Segment_addr+usb3_hst_event_count+0x8));
	dbg_usb("A:%x,RdD:%x\n",(usb3_xHC_EventRing_Segment_addr+usb3_hst_event_count+0x8),usb3_read_data);
	//if(usb3_read_data[31:24] !== 1) 
	if( (usb3_read_data & 0xFF000000) != 0x01000000)
	{ 
		dbg_usb("FAIL:Completion Code = NOT SUCCESS, %d\n", (usb3_read_data >> 24));
		err_count++;
	} 
	else
	{
		dbg_usb("Completion Code = SUCCESS\n");
	}
	usb3_read_data = readl((usb3_xHC_EventRing_Segment_addr+usb3_hst_event_count+0xc));
	dbg_usb("A:%x,WrD:%x\n",(usb3_xHC_EventRing_Segment_addr+usb3_hst_event_count+0xc),usb3_read_data);

	//if  (usb3_read_data[31:24] !== usb3_SlotID)
	if  (((usb3_read_data & 0xFF000000) >> 24) != usb3_SlotID)
	{
		dbg_usb("FAIL:Slot ID = %d is not equal to the value of command's Slot ID\n", usb3_SlotID);
		err_count++;
	}
	else
	{
		dbg_usb("Slot ID = %d is equal to the value of command's Slot ID\n", usb3_SlotID);
	}
	// Check TRB type, completion code and cycle bit status
	//if(usb3_read_data[0] !== 1) {
	if((usb3_read_data & 0x1) != 0x1)
	{
		dbg_usb("FAIL:Cycle bit (C) is not set to 1\n");
		err_count++;
	}
	else
	{
		dbg_usb("Cycle bit (C) is set to 1\n");
	}

	//if(usb3_read_data[15:10] !== 33) {
	if((usb3_read_data & 0x0000FC00) != 0x00008400)
	{
		dbg_usb("FAIL:TRB type is not a Command Completion Event\n");
		err_count++;
	}
	else
	{
		dbg_usb("TRB type is a  Command Completion Event\n");
	}

	if(err_count ==0)
	{
		dbg_usb("*********************************************\n");
		dbg_usb("*** COMMAND COMPLETION IS SUCESS ****\n");
		dbg_usb("*********************************************\n");
		set_addr_cmd_pass=1;
	}
	else
	{
		dbg_usb("*********************************************\n");
		dbg_usb("*** COMMAND COMPLETION IS FAILED ****\n");
		dbg_usb("*********************************************\n");
		set_addr_cmd_pass=0;
	}

}//usb3_xHC_check_cmd_completion_status   
//****************************************************************************************//
//Issues a Configure Endpoint Command 
void usb3_xHC_issue_configure_endpoint_cmd(void)
{
	dbg_usb("*** Issuing the 'configure end point command' from  usb3 controller host ****\n");
	//Set the Input Control Context with A2 to 1 for enabling EP Context 1 OUT
	dbg_usb("Initializes the Input Context Data Structure\n");
	//usb3_read_data[63:32]  = 0x4;//(M) 0xc;     // Add Context Flags
	//usb3_read_data[31:0]   = 0;       // Drop Context Flags

	//usb3_read_data1[63:0]  = 0;       // Reserved
	//usb3_read_data2[63:0]  = 0;       // Reserved
	//usb3_read_data3[63:0]  = 0;       // Reserved
	usb3_read_data1 = 0x0;
	usb3_read_data2 = 0x0;
	usb3_read_data3 = 0x0;

	usb3_read_data = 0x0;
	writel(usb3_read_data, (usb3_xHC_Input_Context_addr+0x0));//[31:0]
	dbg_usb("A:%x,WrD:%x\n",usb3_xHC_Input_Context_addr, usb3_read_data);//[31:0]);
	//(M) usb3_read_data = 0xC;
	usb3_read_data = 0x4; //Set the Input Control Context with A2 to 1 for enabling EP Context 1 OUT
	writel(usb3_read_data, (usb3_xHC_Input_Context_addr+0x4));//[63:32]
	dbg_usb("A:%x,WrD:%x\n",(usb3_xHC_Input_Context_addr+0x4), usb3_read_data);//[63:32]);
	for(usb3_i=0; usb3_i<14; usb3_i=usb3_i+1) 
	{
		writel( 0,(usb3_xHC_Input_Context_addr+0x8+(usb3_i*4)));
		dbg_usb("A:%x,WrD:%x\n",(usb3_xHC_Input_Context_addr+0x8+(usb3_i*4)), 0);
	}

	//Initializes Input Slot Context 
	//usb3_read_data[31:27]  = 2 ;//(M) 3;            // Context Entries
	//usb3_read_data[26]     = 0;            // Hub
	//usb3_read_data[25]     = 0;            // Multi-TT
	//usb3_read_data[24]     = 0;            // Reserved
	//usb3_read_data[23:20]  = usb3_PortSpeed;    // Speed
	//usb3_read_data[23:20]  = 4;    // Speed
	//usb3_read_data[19:0]   = 0;            // Route String
	//(M)usb3_read_data = 0x18400000;
	usb3_read_data = 0x10400000;

	//usb3_read_data1[31:24] = 0;            // Reserved
	//SS port = USB 2.0 number of port +1
	//usb3_read_data1[23:16] = usb3_ss_port_number;            // Root Hub Port Number
	//usb3_read_data1[15:0]  = 0;            // Max Exit Latency
	usb3_read_data1 = ((usb3_ss_port_number<<16) & 0x00FF0000);

	//usb3_read_data2[31:22] = 0;            // Interrupter Targer
	//usb3_read_data2[21:16] = 0;            // Reserved
	//usb3_read_data2[15:8]  = 0;            // TT Port Number
	//usb3_read_data2[7:0]   = 0;            // TT Hub Slot ID
	usb3_read_data2 = 0x0;

	//usb3_read_data3[31:27] = 0;            // Slot State
	//usb3_read_data3[26:8]  = 0;            // Reserved
	//usb3_read_data3[7:0]   = 0;            // USB Device Address
	usb3_read_data3 = 0x0;

	// The remaining upper 16 bytes are Reserved
	writel( usb3_read_data,(usb3_xHC_Input_Context_addr+0x40));
	dbg_usb("A:%x,WrD:%x\n",(usb3_xHC_Input_Context_addr+0x40), usb3_read_data);
	writel( usb3_read_data1,(usb3_xHC_Input_Context_addr+0x44));
	dbg_usb("A:%x,WrD:%x\n",(usb3_xHC_Input_Context_addr+0x44), usb3_read_data1);
	writel( usb3_read_data2,(usb3_xHC_Input_Context_addr+0x48));
	dbg_usb("A:%x,WrD:%x\n",(usb3_xHC_Input_Context_addr+0x48), usb3_read_data2);
	writel( usb3_read_data3,(usb3_xHC_Input_Context_addr+0x4C));
	dbg_usb("A:%x,WrD:%x\n",(usb3_xHC_Input_Context_addr+0x4C), usb3_read_data3);
	for(usb3_i=0; usb3_i<12; usb3_i=usb3_i+1) 
	{
		writel( 0,(usb3_xHC_Input_Context_addr+0x50+(usb3_i*4)));
		dbg_usb("A:%x,WrD:%x\n",(usb3_xHC_Input_Context_addr+0x50+(usb3_i*4)), 0);
	}

	//Initializes Input Bulk OUT Endpoint 1 Context
	dbg_usb("USB3:Initializes Input Bulk OUT Endpoint 1 Context\n");
	//usb3_read_data[31:24]  = 0;             // Reserved
	//usb3_read_data[23:16]  = 0;             // Interval
	//usb3_read_data[15]     = 0;             // Linear Stream Array
	//usb3_read_data[14:10]  = 0;             // Max Primary Streams
	//usb3_read_data[9:8]    = 0;             // Mult
	//usb3_read_data[7:3]    = 0;             // Reserved
	//usb3_read_data[2:0]    = 0;             // Endpoint State
	usb3_read_data = 0x0;

	//usb3_read_data1[31:16] = 1024;          // Maximum Packet Size
	//usb3_read_data1[15:8]  = 0;             // Maximum Burst Size
	//usb3_read_data1[7]     = 0;             // Host Initiate Disable
	//usb3_read_data1[6]     = 0;             // Reserved
	//usb3_read_data1[5:3]   = 2;             // Endpoint Type
	//usb3_read_data1[2:1]   = 3;             // Error Count
	//usb3_read_data1[0]     = 0;             // Force Event
	usb3_read_data1 = 0x04000016;

	//usb3_read_data2[63:32] = 0;                               // TR Dequeue Pointer Address Hi
	//usb3_read_data2[31:4]  = usb3_xHC_TransferRing_Segment_addr[31:4];    // TR Dequeue Pointer Address Lo
	//usb3_read_data2[3:1]   = 0;                               // Reserved 
	//usb3_read_data2[0]     = 1;                               // Dequeue Cycle State
	usb3_read_data2 = ((usb3_xHC_TransferRing_Segment_addr & 0xFFFFFFF0) | 0x1);

	//usb3_read_data3[31:16] = 0;                               // Max Endpoint Service Time Interval Payload
	//usb3_read_data3[15:0]  = 1024;                             // Average TRB Length
	usb3_read_data3 = 0x00000400;

	// The remaining upper 16 bytes are Reserved
	writel(usb3_read_data,(usb3_xHC_Input_Context_addr+0xC0));
	dbg_usb("A:%x,WrD:%x\n",(usb3_xHC_Input_Context_addr+0xC0), usb3_read_data);
	writel(usb3_read_data1,(usb3_xHC_Input_Context_addr+0xC4));
	dbg_usb("A:%x,WrD:%x\n",(usb3_xHC_Input_Context_addr+0xC4), usb3_read_data1);
	writel(usb3_read_data2, (usb3_xHC_Input_Context_addr+0xC8));//[31:0]
	dbg_usb("A:%x,WrD:%x\n",(usb3_xHC_Input_Context_addr+0xC8), usb3_read_data2);//[31:0]);
	usb3_read_data2=0x0;
	writel(usb3_read_data2,(usb3_xHC_Input_Context_addr+0xCC));//63:32]
	dbg_usb("A:%x,WrD:%x\n",(usb3_xHC_Input_Context_addr+0xCC), usb3_read_data2);//63:32]);
	writel( usb3_read_data3,(usb3_xHC_Input_Context_addr+0xD0));
	for(usb3_i=0; usb3_i<11; usb3_i=usb3_i+1)
	{
		writel( 0,(usb3_xHC_Input_Context_addr+0xd4+(usb3_i*4)));
		dbg_usb("A:%x,WrD:%x\n",(usb3_xHC_Input_Context_addr+0xd4+(usb3_i*4)), 0);
	}

	//Initializes Input Bulk IN Endpoint 1 Context
	/* dbg_usb("USB3:Initializes Input Bulk IN Endpoint 1 Context\n");
	//usb3_read_data[31:24]  = 0;             // Reserved
	//usb3_read_data[23:16]  = 0;             // Interval
	//usb3_read_data[15]     = 0;             // Linear Stream Array
	//usb3_read_data[14:10]  = 0;             // Max Primary Streams
	//usb3_read_data[9:8]    = 0;             // Mult
	//usb3_read_data[7:3]    = 0;             // Reserved
	//usb3_read_data[2:0]    = 0;             // Endpoint State
	usb3_read_data = 0;

	//usb3_read_data1[31:16] = 1024;          // Maximum Packet Size
	//usb3_read_data1[15:8]  = 0;             // Maximum Burst Size
	//usb3_read_data1[7]     = 0;             // Host Initiate Disable
	//usb3_read_data1[6]     = 0;             // Reserved
	//usb3_read_data1[5:3]   = 6;             // Endpoint Type
	//usb3_read_data1[2:1]   = 3;             // Error Count
	//usb3_read_data1[0]     = 0;             // Force Event
	usb3_read_data1 = 0x04000036;

	//usb3_read_data2[63:32] = 0;                               // TR Dequeue Pointer Address Hi
	//usb3_read_data2[31:4]  = usb3_xHC_TransferRing_Segment_addr[31:4];    // TR Dequeue Pointer Address Lo
	//usb3_read_data2[3:1]   = 0;                               // Reserved
	//usb3_read_data2[0]     = 1;                               // Dequeue Cycle State
	usb3_read_data2 = ((usb3_xHC_TransferRing_Segment_addr & 0xFFFFFFF0) | 0x1);

	//usb3_read_data3[31:16] = 0;                               // Max Endpoint Service Time Interval Payload
	//usb3_read_data3[15:0]  = 1024;                             // Average TRB Length
	usb3_read_data3 = 0x00000400;

	// The remaining upper 16 bytes are Reserved
	writel( usb3_read_data,(usb3_xHC_Input_Context_addr+0x100));
	dbg_usb("A:%x,WrD:%x\n",(usb3_xHC_Input_Context_addr+0x100), usb3_read_data);
	writel( usb3_read_data1,(usb3_xHC_Input_Context_addr+0x104));
	dbg_usb("A:%x,WrD:%x\n",(usb3_xHC_Input_Context_addr+0x104), usb3_read_data1);
	writel( usb3_read_data2);//[31:0],(usb3_xHC_Input_Context_addr+0x108));
	dbg_usb("A:%x,WrD:%x\n",(usb3_xHC_Input_Context_addr+0x108), usb3_read_data2);//[31:0]);
	usb3_read_data2=0x0;
	writel( usb3_read_data2);//[63:32],(usb3_xHC_Input_Context_addr+0x10C));
	dbg_usb("A:%x,WrD:%x\n",(usb3_xHC_Input_Context_addr+0x10C), usb3_read_data2);//[63:32]);
	writel( usb3_read_data3,(usb3_xHC_Input_Context_addr+0x110));
	dbg_usb("A:%x,WrD:%x\n",(usb3_xHC_Input_Context_addr+0x110), usb3_read_data3);
	for(usb3_i=0; usb3_i<11; usb3_i=usb3_i+1) 
	{
	writel( 0,(usb3_xHC_Input_Context_addr+0x114+(usb3_i*4)));
	dbg_usb("A:%x,WrD:%x\n",(usb3_xHC_Input_Context_addr+0x114+(usb3_i*4)), 0);
	}*/

	//Issue Configure Endpoint Command
	dbg_usb("USB3:Issue 'Configure Endpoint Command'\n");
	//usb3_read_data[63:32] =0;           // Hi Address
	//usb3_read_data[31:4]  = usb3_xHC_Input_Context_addr[31:4];  // Lo Address
	usb3_read_data = ((usb3_xHC_Input_Context_addr & 0xFFFFFFF0) | 0x0);

	//usb3_read_data1[31:24] = usb3_SlotID; // Reserved
	//usb3_read_data1[23:16] = 0;           // Reserved
	//usb3_read_data1[15:10] = 12;          // TRB Type - Enable Slot Command
	//usb3_read_data1[9]     = 0;           // Deconfigure 
	//usb3_read_data1[8:1]   = 0;           // Reserved
	//usb3_read_data1[0]     = 1;           // Cycle bit(C)
	usb3_read_data1 = (((usb3_SlotID << 24) & 0xFF000000) | 0x00003001);

	// 0B-08 is Reserved 
	writel( usb3_read_data,(usb3_xHC_CommandRing_Segment_addr+0x20));
	dbg_usb("A:%x,WrD:%x\n",(usb3_xHC_CommandRing_Segment_addr+0x20), usb3_read_data);
	writel( usb3_read_data1,(usb3_xHC_CommandRing_Segment_addr+0x2C));
	dbg_usb("A:%x,WrD:%x\n",(usb3_xHC_CommandRing_Segment_addr+0x2C), usb3_read_data1);
} //usb3_xHC_issue_configure_endpoint_cmd
//****************************************************************************************//
//Host start the Bulk IN transfer
void usb3_xHC_start_bulk_in_xfer(void)
{
	dbg_usb("*** Starting the 'BulkIN transfer' from usb3 host controller ****\n");
	// Setup TRB 
	usb3_read_data = usb3_xHC_Data_BulkIn_Buffer_addr;
	//usb3_read_data1 = 0;
	//usb3_read_data1[16:0] = usb3_host_num_of_bulkin_bytes_per_trb;
	usb3_read_data1 = usb3_host_num_of_bulkin_bytes_per_trb;

	//usb3_read_data2 = 0;
	//usb3_read_data2[0]  = 1; // Cycle
	//usb3_read_data2[2]  = 1; // ISP
	//usb3_read_data2[5]  = 1; // IOC
	//usb3_read_data2[15:10]  = 1; // IOC
	usb3_read_data2 = 0x425;  

	writel(usb3_read_data, (usb3_xHC_TransferRing_Segment_addr+0x00));//[31:0]
	dbg_usb("A:%x,WrD:%x\n",(usb3_xHC_TransferRing_Segment_addr+0x00), usb3_read_data);//[31:0]);
	usb3_read_data = 0x0;
	writel(usb3_read_data, (usb3_xHC_TransferRing_Segment_addr+0x04));//[63:32]
	dbg_usb("A:%x,WrD:%x\n",(usb3_xHC_TransferRing_Segment_addr+0x04), usb3_read_data);//[63:32]);
	writel( usb3_read_data1,(usb3_xHC_TransferRing_Segment_addr+0x08));
	dbg_usb("A:%x,WrD:%x\n",(usb3_xHC_TransferRing_Segment_addr+0x08), usb3_read_data1);
	writel( usb3_read_data2,(usb3_xHC_TransferRing_Segment_addr+0x0C));
	dbg_usb("A:%x,WrD:%x\n",(usb3_xHC_TransferRing_Segment_addr+0x0C), usb3_read_data2);
	//Ring doorbell 
	dbg_usb("Ring xHC Doorbell For Bulk IN Transfer \n");
	//usb3_read_data = 0;
	//usb3_read_data[7:0]= 3;       // DB Target - Host Controller Command
	usb3_read_data = 0x3;
	writel( usb3_read_data,(USB3p0_CFG_BASE_ADDR+usb3_Doorbell_Array_Base_addr+0x4));
	dbg_usb("A:%x,WrD:%x\n",(USB3p0_CFG_BASE_ADDR+usb3_Doorbell_Array_Base_addr+0x4), usb3_read_data);
} //usb3_xHC_start_bulk_in_xfer;
//****************************************************************************************//
//Host Start BULK OUT tranferusb3_test_func
void usb3_xHC_start_bulk_out_xfer(void)
{
	dbg_usb("*** Starting the 'BulkOUT transfer' from usb3 host controller ****\n");
	// Setup TRB  for Bulk OUTusb3_test_func
	usb3_read_data = usb3_xHC_Data_BulkOut_Buffer_addr;

	//usb3_read_data1 = 0;
	//usb3_read_data1[16:0] = usb3_host_num_of_bulkout_bytes_per_trb;
	usb3_read_data1 = usb3_host_num_of_bulkout_bytes_per_trb;

	//usb3_read_data2 = 0;
	//usb3_read_data2[0]  = 1; // Cycle
	//usb3_read_data2[2]  = 0; //(M) 1;// ISP
	//usb3_read_data2[5]  = 1; // IOC
	//usb3_read_data2[15:10]  = 1; // IOC
	usb3_read_data2 = 0x425;  
	//usb3_read_data2 = 0x421;  

	writel(usb3_read_data, (usb3_xHC_TransferRing_Segment_addr+0x00));//[31:0]
	dbg_usb("A:%x,WrD:%x\n",(usb3_xHC_TransferRing_Segment_addr+0x00), usb3_read_data);//[31:0]);
	usb3_read_data = 0x0;
	writel(usb3_read_data, (usb3_xHC_TransferRing_Segment_addr+0x04));//[63:32]
	dbg_usb("A:%x,WrD:%x\n",(usb3_xHC_TransferRing_Segment_addr+0x04), usb3_read_data);//[63:32]);
	writel( usb3_read_data1,(usb3_xHC_TransferRing_Segment_addr+0x08));
	dbg_usb("A:%x,WrD:%x\n",(usb3_xHC_TransferRing_Segment_addr+0x08), usb3_read_data1);
	writel( usb3_read_data2,(usb3_xHC_TransferRing_Segment_addr+0x0C));
	dbg_usb("A:%x,WrD:%x\n",(usb3_xHC_TransferRing_Segment_addr+0x0C), usb3_read_data2);

	//Ring doorbell 
	dbg_usb(" Ring xHC Doorbell For Bulk OUT Transfer \n");
	//usb3_read_data = 0;
	//usb3_read_data[7:0]   = 2;       // DB Target - Host Controller Command
	usb3_read_data = 0x2;
	writel( usb3_read_data,(USB3p0_CFG_BASE_ADDR+usb3_Doorbell_Array_Base_addr+0x4));
	dbg_usb("A:%x,WrD:%x\n",(USB3p0_CFG_BASE_ADDR+usb3_Doorbell_Array_Base_addr+0x4), usb3_read_data);
} //usb3_xHC_start_bulk_out_xfer
//****************************************************************************************//
//Check for bulkout command completion event after issuing host control command 
void usb3_xHC_check_bulkout_cmd_completion_status(void)
{
	int err_count=0;
	dbg_usb("*** Checking the Command Completion Event TRB generated from usb3 host ****\n");
	usb3_read_data = readl((usb3_xHC_EventRing_Segment_addr+usb3_hst_event_count+0x8));
	dbg_usb("A:%x,RdD:%x\n",(usb3_xHC_EventRing_Segment_addr+usb3_hst_event_count+0x8),usb3_read_data);
	//if(usb3_read_data[31:24] !== 1) 
	if( (usb3_read_data & 0xFF000000) != 0x01000000)
	{ 
		dbg_usb("FAIL:Completion Code = NOT SUCCESS, %d\n", (usb3_read_data >> 24));
		err_count++;
	} 
	else
	{
		dbg_usb("Completion Code = SUCCESS\n");
	}

	if(err_count ==0)
	{
		dbg_usb("*********************************************\n");
		dbg_usb("*** COMMAND COMPLETION IS SUCESS ****\n");
		dbg_usb("*********************************************\n");
	}
	else
	{
		dbg_usb("*********************************************\n");
		dbg_usb("*** COMMAND COMPLETION IS FAILED ****\n");
		dbg_usb("*********************************************\n");
	}

}//usb3_xHC_check_bulkout_cmd_completion_status   
//****************************************************************************************//
void check_data_intigrity_and_conclude_final_result(u32 source_addr,u32 dest_addr, u32 data_size)
{
	int err_count=0;
	u32 dest_rd_data =0;
	u32 src_rd_data =0;
	u32 temp_i=0;
	u32 temp_j=0;
	dbg_usb("***Reading the destination buffer location from %x to %x and comparing with source buffer location data ****\n",dest_addr,(dest_addr+data_size-0x4));
	for(temp_i=0;temp_j<(data_size-0x4);temp_i=temp_i+1)
	{
		temp_j= (temp_i*4);
		dest_rd_data = readl(dest_addr+(temp_i*4));
		src_rd_data = readl(source_addr+(temp_i*4));
		dbg_usb("Src:A:%x,RdD:%x || Dest:A:%x,RdD:%x \n",(source_addr+(temp_i*4)),src_rd_data,(dest_addr+(temp_i*4)),dest_rd_data);
		if(src_rd_data != dest_rd_data)
			err_count++;
	}

	if(err_count ==0)
	{
		dbg_usb("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
		dbg_usb("*** TEST PASSED ****\n");
		dbg_usb("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
	}
	else
	{
		dbg_usb("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
		dbg_usb("*** TEST FAILED ****\n");
		dbg_usb("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
	}
} //check_data_intigrity_and_conclude_final_result
//****************************************************************************************//

//****************************************************************************************//
//Iniatialize the Data buffer with increamenting number pattern based on buffer size &
//Base address arguements.
void data_buffer_init(u32 base_addr, u32 buf_size,int temp_k)
{
	u32 temp_i=0;
	u32 temp_j=0;
	dbg_usb("*** Initializing the shared memory location from %x to %x with fixed increamented pattern data ****\n",base_addr,(base_addr+buf_size));
	if(temp_k !=0)
	{
		for(temp_i=0;temp_j<buf_size;temp_i=temp_i+1)
		{
			temp_j= (temp_i*4);
			writel( (0xbaad0000|temp_j),(base_addr+(temp_i*4)));
			dbg_usb("A:%x,WrD:%x \n",(base_addr+(temp_i*4)),(0xbaad0000|temp_j));
		}
	}
	else
	{
		for(temp_i=0;temp_j<buf_size;temp_i=temp_i+1)
		{
			temp_j= (temp_i*4);
			writel( (0x900d0000|temp_j),(base_addr+(temp_i*4)));
			dbg_usb("A:%x,WrD:%x \n",(base_addr+(temp_i*4)),(0x900d0000|temp_j));
		}
	}
} //data_buffer_init
//****************************************************************************************//
//****************************************************************************************//
void gpio_wait_for_device_trigger(void)
{
	u32 temp=0;
	dbg_usb("*** USB3 Host waiting for syncup with device ****\n");
	temp = readl(0x90470010);
	dbg_usb("A:%x,RdD:%x\n",0x90470010,temp);
	while((temp & 0x300) != 0x300)
	{
		temp = readl(0x90470010);
		//dbg_usb("A:%x,RdD:%x\n",0x90470010,temp);
	}
	dbg_usb("A:%x,RdD:%x\n",0x90470010,temp);
	dbg_usb("*** Now usb3 host is in syncup with device ****\n");
}
//****************************************************************************************//
void usb3_start_device_side_program_execution(void)
{
	dbg_usb("*** Programming GPIO[5:4] ports to high,which will be polled in device side program to proceed ****\n");
	dbg_usb("*** Start executing the usb3 device code ****\n");
	writel(0x30,0x90470000);
	dbg_usb("A:%x,WrD:%x\n",0x90470000,0x30);
}
//****************************************************************************************//
void usb3_stop_device_side_program_execution(void)
{
	dbg_usb("*** Programming GPIO[5:4] ports to low,which will be polled in device side program to stop ****\n");
	dbg_usb("*** Stop executing the usb3 device code ****\n");
	writel(0x00,0x90470000);
	dbg_usb("A:%x,WrD:%x\n",0x90470000,0x00);
}
//****************************************************************************************//
int check_host_to_device_handshaking_result(void)
{
	if((device_connected_on_ss_port==1) && (device_slots_cmd_pass==1) && (set_addr_cmd_pass==1))
	{
		printf(">>> USB3 HOST TO USB3 DEVICE HANDSHAKING IS SUCCESSFULL <<<\n\n");
		return 0;
	}
	else
	{
		printf(">>>> USB3 HOST TO USB3 DEVICE HANDSHAKING IS FAILED <<<<\n\n");
		return -1;
	}
}
//****************************************************************************************//


