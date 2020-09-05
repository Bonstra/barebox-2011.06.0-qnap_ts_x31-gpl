
#define	U32	u32

//---------------------------------------
//--------------------------------------//
// PHY CR CONTROL REGISTER (0x0) ACCESS PROTOCOL
// Its a 20 bit register;
// 31:20 --RSVD
// 15:0 -- cr_data_in
// 16------cr_cap_addr
// 17------cr_read
// 18------cr_write
// 19------cr_cap_data
//--------------------------------------//
//--------------------------------------//
// PHY CR STATUS REGISTER (0x4) ACCESS PROTOCOL
// Its a 17 bit bus;
// 31:17 ----RSVD tied to '0'
// 16--------cr_ack
// 15:0------cr_data_out
//--------------------------------------//
//CR Write Operation sequence
//1.	Write cr_data_in[15:0] with required address
//2.	Set cr_cap_addr
//3.	Pull cr_ack till it is ‘1’
//4.	Clear cr_cap_addr
//5.	Pull cr_ack till it is ‘0’
//
//6.	Write cr_data_in[15:0] with required ‘write data’
//7.	Set cr_cap_data
//8.	Pull cr_ack till it is ‘1’
//9.	Clear cr_cap_data
//10.	Pull cr_ack till it is ‘0’
//
//11.	Set cr_cap_write
//12.	Pull cr_ack till it is ‘1’
//13.	Clear cr_cap_write
//14.	Pull cr_ack till it is ‘0’
//
//
//CR Read Operation sequence
//1.	Write cr_data_in[15:0] with required address
//2.	Set cr_cap_addr
//3.	Pull cr_ack till it is ‘1’
//4.	Clear cr_cap_addr
//5.	Pull cr_ack till it is ‘0’
//
//6.	Set cr_cap_read
//7.	Pull cr_ack till it is ‘1’
//8.	cr_data_out[15:0] holds the ‘read data’
//9.	Clear cr_cap_read
//10.	Pull cr_ack till it is ‘0’
//--------------------------------------//
//
//
//-------------------------------------------------
void phy_cr_write(U32 address, U32 data)
{
   U32 i=0x1;
   U32 write_data =0;
   U32 read_data =0;
   U32 ack=0;
   U32 data_in=data;
   printf("CR_W=A:%x,D:%x\n",address,data);
   //1.	Write cr_data_in[15:0] with required address
   //2.	Set cr_cap_addr
   write_data = (((U32)(i)<<16) | address);
   //printf("WrA=%x: WrD=%x\n",USB3p0_PHY_CR_ACC_CNTRL_REG_ADDR,write_data);
   writel(write_data, USB3p0_PHY_CR_ACC_CNTRL_REG_ADDR);
   //3.	Pull cr_ack till it is ‘1’
   read_data= readl(USB3p0_PHY_CR_ACC_STS_REG_ADDR);
   ack = (read_data & 0x10000);
   while(ack == 0x00000)
   {
      read_data= readl(USB3p0_PHY_CR_ACC_STS_REG_ADDR);
      ack = (read_data & 0x10000);
   }
   //4.	Clear cr_cap_addr
   write_data = (write_data & 0x0FFFF);
   writel(write_data, USB3p0_PHY_CR_ACC_CNTRL_REG_ADDR);
   //5.	Pull cr_ack till it is ‘0’
   read_data= readl(USB3p0_PHY_CR_ACC_STS_REG_ADDR);
   ack = (read_data & 0x10000);
   while(ack != 0x00000)
   {
      read_data= readl(USB3p0_PHY_CR_ACC_STS_REG_ADDR);
      ack = (read_data & 0x10000);
   }
   //6.	Write cr_data_in[15:0] with required ‘write data’
   //7.	Set cr_cap_data
   write_data = (((U32)(i)<<19) | data_in);
   //printf("WrA=%x: WrD=%x\n",USB3p0_PHY_CR_ACC_CNTRL_REG_ADDR,write_data);
   writel(write_data, USB3p0_PHY_CR_ACC_CNTRL_REG_ADDR);
   //8.	Pull cr_ack till it is ‘1’
   read_data= readl(USB3p0_PHY_CR_ACC_STS_REG_ADDR);
   ack = (read_data & 0x10000);
   while(ack == 0x00000)
   {
      read_data= readl(USB3p0_PHY_CR_ACC_STS_REG_ADDR);
      ack = (read_data & 0x10000);
   }
   //9.	Clear cr_cap_data
   write_data = (write_data & 0x0FFFF);
   writel(write_data, USB3p0_PHY_CR_ACC_CNTRL_REG_ADDR);
   //10.Pull cr_ack till it is ‘0’
   read_data= readl(USB3p0_PHY_CR_ACC_STS_REG_ADDR);
   ack = (read_data & 0x10000);
   while(ack != 0x00000)
   {
      read_data= readl(USB3p0_PHY_CR_ACC_STS_REG_ADDR);
      ack = (read_data & 0x10000);
   }
   //11.Set cr_cap_write
   write_data =  (((U32)(i)<<18) | 0x0);
   //printf("WrA=%x: WrD=%x\n",USB3p0_PHY_CR_ACC_CNTRL_REG_ADDR,write_data);
   writel(write_data, USB3p0_PHY_CR_ACC_CNTRL_REG_ADDR);
   //12.Pull cr_ack till it is ‘1’
   read_data= readl(USB3p0_PHY_CR_ACC_STS_REG_ADDR);
   ack = (read_data & 0x10000);
   while(ack == 0x00000)
   {
      read_data= readl(USB3p0_PHY_CR_ACC_STS_REG_ADDR);
      ack = (read_data & 0x10000);
   }
   //13.Clear cr_cap_write
   write_data = (write_data & 0x0FFFF);
   writel(write_data, USB3p0_PHY_CR_ACC_CNTRL_REG_ADDR);
   //14.Pull cr_ack till it is ‘0’
   read_data= readl(USB3p0_PHY_CR_ACC_STS_REG_ADDR);
   ack = (read_data & 0x10000);
   while(ack != 0x00000)
   {
      read_data= readl(USB3p0_PHY_CR_ACC_STS_REG_ADDR);
      ack = (read_data & 0x10000);
   }
}
//-------------------------------------------------
U32 phy_cr_read(U32 address)
{
   U32 i=0x1;
   U32 write_data =0;
   U32 read_data =0;
   U32 ack=0x0;
   U32 data_out=0;
   printf("CR_R=");
   //1.	Write cr_data_in[15:0] with required address
   //2.	Set cr_cap_addr
   write_data = (((U32)(i)<<16) | address);
   //printf("WrA=%x: WrD=%x\n",USB3p0_PHY_CR_ACC_CNTRL_REG_ADDR,write_data);
   writel(write_data, USB3p0_PHY_CR_ACC_CNTRL_REG_ADDR);
   //printf("1,2 done\n");
   //3.	Pull cr_ack till it is ‘1’
   read_data= readl(USB3p0_PHY_CR_ACC_STS_REG_ADDR);
   ack = (read_data & 0x10000);
   while(ack == 0x00000)
   {
      read_data= readl(USB3p0_PHY_CR_ACC_STS_REG_ADDR);
      ack = (read_data & 0x10000);
   }
   //4.	Clear cr_cap_addr
   write_data = (write_data & 0x0FFFF);
   writel(write_data, USB3p0_PHY_CR_ACC_CNTRL_REG_ADDR);
   //5.	Pull cr_ack till it is ‘0’
   read_data= readl(USB3p0_PHY_CR_ACC_STS_REG_ADDR);
   ack = (read_data & 0x10000);
   while(ack != 0x00000)
   {
      read_data= readl(USB3p0_PHY_CR_ACC_STS_REG_ADDR);
      ack = (read_data & 0x10000);
   }
   //6.	Set cr_cap_read
   write_data =  (((U32)(i)<<17) | 0x0);
   //printf("WrA=%x: WrD=%x\n",USB3p0_PHY_CR_ACC_CNTRL_REG_ADDR,write_data);
   writel(write_data, USB3p0_PHY_CR_ACC_CNTRL_REG_ADDR);
   //7.	Pull cr_ack till it is ‘1’
   read_data= readl(USB3p0_PHY_CR_ACC_STS_REG_ADDR);
   ack = (read_data & 0x10000);
   while(ack == 0x00000)
   {
      read_data= readl(USB3p0_PHY_CR_ACC_STS_REG_ADDR);
      ack = (read_data & 0x10000);
   }
  //8.	cr_data_out[15:0] holds the ‘read data’
  read_data= readl(USB3p0_PHY_CR_ACC_STS_REG_ADDR);
  data_out = (read_data & 0xFFFF);
  //9.	Clear cr_cap_read
  write_data = (write_data & 0x0FFFF);
  writel(write_data, USB3p0_PHY_CR_ACC_CNTRL_REG_ADDR);
  //10.	Pull cr_ack till it is ‘0’
  read_data= readl(USB3p0_PHY_CR_ACC_STS_REG_ADDR);
  ack = (read_data & 0x10000);
  while(ack != 0x00000)
  {
     read_data= readl(USB3p0_PHY_CR_ACC_STS_REG_ADDR);
     ack = (read_data & 0x10000);
  }
  printf("A:%x,D:%x\n",address,data_out);
  return(data_out);
}

int phy_cr_reg_def_val_check (volatile U32 raddr, volatile U32 rst_val) {
  U32 r_data;
  U32 err_cnt=0;
  printf("E_PHY_CR_DEF_VAL_CHECK_FUNC\n");
  r_data = phy_cr_read(raddr);
  if(r_data != rst_val)
  {
    printf("FAIL@ %x :Exp.DefValue= %x,Act.DefValue=%x\n",raddr,rst_val,r_data);
    err_cnt=1;
  }
  return(err_cnt);
}

