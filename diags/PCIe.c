
int reg_cmp (volatile U32 addr, volatile U32 mask, volatile U32 exp_data) {
  U32 r_data;
  U32 err_cnt=0;
  U32 data_temp;
  r_data = reg_rd(addr);
  data_temp = r_data & (mask); // '&&' is logical AND of values, whereas '&' does "bit-wise" ANDing, which is what is required here
  if(data_temp != exp_data)
  {
    printf("FAIL@ %x :Exp.Value=%x ,Act.Value=%x\n",addr,exp_data,data_temp);
    err_cnt=1;
  }
  else
    printf("PASS@ %x :Exp.Value=%x ,Act.Value=%x\n",addr,exp_data,data_temp);
  
  return(err_cnt) ;
} 

void axi_direct_1w1r (void) {
  U32 addr;                      /* Start address of each transfer */
  U32 data1, rd_data;                     /* Write and expected read data   */

  /******************************************/
  /*               W R I T E                */
  /******************************************/
  /* Initialize a start address */
  addr  = TRANS_START_ADDR;

    /* Initialize the write data.*/
    data1 = 0x87654321;

    /* printf("single write addr = 0x%08x \n", addr); */

   reg_wr(addr, data1); 

  /******************************************/
  /*                 R E A D                */
  /******************************************/
  /* Initialize a start address */
  addr  = TRANS_START_ADDR;


    /* Initialize the write data */
    data1 = 0x87654321;

      /* printf("single read addr = 0x%08x \n", addr); */
      rd_data = reg_rd(addr); 

      if(rd_data == data1) {
          printf(" Data Matched!\n");
      } else {
          printf(" Data ERR! addr = 0x%08x\n", addr);
          fail_ctl(1);
      }
   return;
}

U32 caxi_direct_1w1r (volatile U32 addr, volatile U32 data_val) {
  U32 data1, rd_data;                     /* Write and expected read data   */
  U32 fail_flag;
  /******************************************/
  /*               W R I T E                */
  /******************************************/
  /* Initialize a start address */

  /* Initialize the write data.*/
  data1 = data_val;

  /* printf("single write addr = 0x%08x \n", addr); */

   reg_wr(addr, data1); 

  /******************************************/
  /*                 R E A D                */
  /******************************************/

  /* printf("single read addr = 0x%08x \n", addr); */
  rd_data = reg_rd(addr); 

  if(rd_data == data1) {
      printf(" Data Matched!\n");
      fail_flag = 0;
  } else {
      printf(" Data ERR! addr = 0x%08x\n", addr);
      fail_ctl(1);
      fail_flag = 1;
  }

  return fail_flag;
}


void axi_direct_2w2r (void) {
  U32 addr;                      /* Start address of each transfer */
  U32 data1, rd_data;                     /* Write and expected read data   */
  int num;                    

  /******************************************/
  /*               W R I T E                */
  /******************************************/
  /* Initialize a start address */
  addr  = TRANS_START_ADDR;

    /* Initialize the write data.*/
    data1 = 0x87654321;

    /* num times loop */
    for(num = 0;num < 2;num++) {

      /* printf("single write addr = 0x%08x \n", addr); */

      reg_wr(addr, data1); 

      /* Prepare the next address */
      addr  = addr + 0x4;

      /* Prepare the next write data.*/
      data1++;
    } /* End of num loop */


  /******************************************/
  /*                 R E A D                */
  /******************************************/
  /* Initialize a start address */
  addr  = TRANS_START_ADDR;


    /* Initialize the write data */
    data1 = 0x87654321;

    /* num times loop */
    for(num = 0;num < 2;num++) {

      /* printf("single read addr = 0x%08x \n", addr); */
      rd_data = reg_rd(addr); 

      if(rd_data == data1) {
          printf("Matched=0x%08x\n", rd_data);
      } else {
          printf(" Data ERR! addr = 0x%08x\n", addr);
      }

      /* Prepare the next address and data */
      addr  = addr + 0x4;
      data1++;

    } /* End of num loop */
   return;
}

U32 axi_direct_single (void) {
  U32 addr;                      /* Start address of each transfer */
  U32 data1, rd_data;                     /* Write and expected read data   */
  int num;                    
  U32 fail_flag;

  /******************************************/
  /*               W R I T E                */
  /******************************************/
  /* Initialize a start address */
  addr  = TRANS_START_ADDR;

    /* Initialize the write data.*/
    data1 = 0x87654321;

    /* 16 times loop */
    for(num = 0;num < 256;num++) {

      /* printf("single write addr = 0x%08x \n", addr); */

      reg_wr(addr, data1); 

      /* Prepare the next address */
      addr  = addr + 0x4;

      /* Prepare the next write data.*/
      data1++;
    } /* End of num loop */


  /******************************************/
  /*                 R E A D                */
  /******************************************/
  /* Initialize a start address */
  addr  = TRANS_START_ADDR;


    /* Initialize the write data */
    data1 = 0x87654321;

    /* 16 times loop */
    for(num = 0;num < 256;num++) {

      /* printf("single read addr = 0x%08x \n", addr); */
      rd_data = reg_rd(addr); 

      if(rd_data == data1) {
         /* printf("Matched=0x%08x\n", rd_data); */
         fail_flag = 0;
      } else {
         printf(" Data ERR! addr = 0x%08x\n", addr);
         fail_flag = 1;
      }

      /* Prepare the next address and data */
      addr  = addr + 0x4;
      data1++;

    } /* End of num loop */
   return fail_flag;
}

void PCIE0_link_up_chk (void) {
 U32 rd_data;
 U32 check_bit;

 rd_data = reg_rd(DWC_STS_REG0);
 check_bit =  CHECK_BIT(rd_data, 16);  
 printf("O: A=0x%x d=0x%x chkbit=0x%x \n", DWC_STS_REG0, rd_data, check_bit);
 while(check_bit != 0x00010000) {
    rd_data = reg_rd(DWC_STS_REG0);
    check_bit =  CHECK_BIT(rd_data, 16);  
    printf("I: d=0x%x\n", rd_data);
 };

 return;
}

void PCIE1_link_up_chk (void) {
 U32 rd_data;
 U32 check_bit;

  rd_data = reg_rd(DWC_STS_REG3);
  check_bit =  CHECK_BIT(rd_data, 16);  
  printf("O: A=0x%x d=0x%x chkbit=0x%x \n", DWC_STS_REG3, rd_data, check_bit);
  while(check_bit != 0x00010000) {
     rd_data = reg_rd(DWC_STS_REG3);
     check_bit =  CHECK_BIT(rd_data, 16);  
     printf("I: d=0x%x\n", rd_data);
  };

 return;
}
