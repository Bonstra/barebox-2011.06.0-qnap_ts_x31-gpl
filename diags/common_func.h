
#define        U32     u32

#define CODE_PROGRESS_MARKER    0x8300FF00
#define PERIPH_BASE 	0xFFF00000
#define IC_DIST_BASE	0xFFF01000
#define IC_INT_BASE	0xFFF00100

void set_marker(int val)
{
  *(volatile int *)CODE_PROGRESS_MARKER = val;
}


void reg_wr (volatile U32 waddr, volatile U32 data)
{
	U32 *wloc;
	wloc = (U32 *)waddr;
	*wloc = data;
	return ;
}

#if 0
U64 reg_wr_64 (U32 REG_ADDR,U64 REG_VAL)
{
   volatile U64* WriteLocAddr;
   U64  WriteData;
   WriteLocAddr = (volatile U64*)REG_ADDR;
   //tb_printf("%x\n",WriteLocAddr); // printing the write address
   WriteData = REG_VAL;
   *WriteLocAddr =WriteData;
   return(1);
}
#endif

int reg_rd (volatile U32 raddr)
{
	U32 *rloc,r_data;
	rloc = (U32 *)raddr;
	r_data = *rloc;
	return (r_data);
}

// Procedure to test a specific Register by writing a specific Test Data Pattern to it and reading it back
// Input Parameters: reg_waddr - (Write) Address of Register
//                   testdata - Data to be written to and read back from the Register
//                   debug - Enables printing of debug information.
// Output: Boolean value if the Register Write/Read-back test passed or not
bool test_reg (volatile U32 reg_waddr, volatile U32 testdata, bool debug)
{
        U32 *reg_loc; // Pointer to Register Address
	U32 r_data; // 32-bit Unsigned Integer Variable to store the data read from the Register Address

	if(debug) {
	  r_data = reg_rd(reg_waddr); // Read the data in the Register at it's Address
	  //printf("read_data = %x & testdata = %x\n", r_data, testdata); // Issue if Register read has X's
          printf("testdata = %x\n", testdata);
        }
	reg_wr(reg_waddr, testdata); // Call the method to write the Test Data-Pattern to the specified Register
        if(debug) {
          printf("Wrote %x to %x\n", testdata, reg_waddr);
        }
        r_data = reg_rd(reg_waddr); // Read the data in the Register back from it's Address
        if(debug) {
          printf("testdata = %x data = %x\n", testdata, r_data);
        }
        return(r_data == testdata); // Return whether the data read-back from the Register was the same as the data written to it
}


// Procedure to test a specific 'Read-Only' Register by writing a specific Test Data Pattern to it, reading it back & making sure that it didn't get written
// Input Parameters: reg_waddr - (Write) Address of Register
//                   testdata - Data to be written to and read back from the Register
//                   debug - Enables printing of debug information.
// Output: Boolean value if the Register Write/Read-back test passed or not
bool test_reg_ro (volatile U32 reg_waddr, volatile U32 testdata, bool debug)
{
        U32 *reg_loc; // Pointer to Register Address
	U32 r_data, r_dataw; // 32-bit Unsigned Integer Variable to store the data read from the Register Address before and after writing to it respectively

	r_data = reg_rd(reg_waddr); // Read the data in the Register at it's Address
	if(debug) {
	    printf("Read data = %x & testdata = %x\n", r_data, testdata);
	  }
	reg_wr(reg_waddr, testdata); // Call the method to write the Test Data-Pattern to the specified Register
        if(debug) {
          printf("Wrote %x to %x\n", testdata, reg_waddr);
        }
        r_dataw = reg_rd(reg_waddr); // Read the data in the Register back from it's Address after performing the write
        if(debug) {
          printf("Old read = %x & read = %x\n", r_data, r_dataw);
        }
        return(r_data == r_dataw); // Return whether the data read-back from the Register was the same as it's original value before the write to it
}

void reg_rmw (volatile U32 waddr, volatile U32 mask, volatile U32 data) {
	U32 *rloc;
	U32 data_temp, data_mod;
	rloc = (U32 *)waddr;
	data_temp = *rloc;
	data_temp = data_temp & (~mask); // '&&' is logical AND of values, whereas '&' does "bit-wise" ANDing, which is what is required here
	data_mod = data_temp | data; // || is logical OR of values, whereas | does "bit-wise" ORing, which is what is what is required here
	*rloc = data_mod;
	return ;
	}
//----------------------------------------------------------------
// These are added by Mohd on 120411
int reg_def_val_check (volatile U32 raddr, volatile U32 rst_val) {
  U32 r_data;
  U32 err_cnt=0;
  r_data = reg_rd(raddr);
  if(r_data != rst_val)
  {
    printf("FAIL@ %x :Exp.DefValue= %x,Act.DefValue=%x\n",raddr,rst_val,r_data);
    err_cnt=1;
  }
  return(err_cnt);
}
int reg_rw_check (volatile U32 addr, volatile U32 wr_data, volatile U32 cmp_val) {
  U32 r_data;
  U32 err_cnt=0;
  reg_wr(addr,wr_data);
  r_data = reg_rd(addr);
  if(r_data != cmp_val)
  {
    printf("FAIL@ %x WrD=%x :Exp.Value=%x ,Act.Value=%x\n",addr,wr_data,cmp_val,r_data);
    err_cnt=1;
  }
  return(err_cnt) ;
}

#if 0
//----------------------------------------------------------------
// This is added by Mohd on 140411
// This functions returns the A9-MP Core GIC- Interrupt Controller
// Distribuitors SPI status (IRQs status) can be read by following 2 ways
// [1]
// 0x304 -ICDABR0(Interrupt Controller Distribuitor Acitve Bit Register 0-[31:0]
// 0x308 -ICDABR0(Interrupt Controller Distribuitor Acitve Bit Register 1-[63:32]
// These 2 registers holds the active status of SPI interrupts IRQs[63:0]
// [2]
// 0xD04 -ICD SPI_status Register 0-[31:0]
// 0xD08 -ICD SPI_status Register 1-[63:32]
// These 2 registers holds status of SPI interrupts IRQs[63:0]
//----------------------------------------------------------------
U64 read_a9irqs_status()
{
  U32 irqs_31_0_status=0;
  U32 irqs_63_32_status=0;
  U64 irqs_63_0_status=0;
  irqs_31_0_status  = reg_rd (IC_DIST_BASE + 0xD04);
  //printf("irqs_31_0= %x\n",irqs_31_0_status);
  //printf("%x\n",irqs_31_0_status);
  irqs_63_32_status = reg_rd (IC_DIST_BASE + 0xD08);
  //printf("irqs_63_32= %x\n",irqs_63_32_status);
  //printf("%x\n",irqs_63_32_status);
  irqs_63_0_status  = (((U64)(irqs_63_32_status)<<32) | irqs_31_0_status);
  printf("A9-IRQs Status:%016llx\n",irqs_63_0_status);
  return(irqs_63_0_status);
}
#endif

//----------------------------------------------------------------
// Specify that the test has failed with the specified number of Errors
// Input Parameter: num_errors - Integer inidicating number of errors seen in the failed test
void fail_ctl(int num_errors) {
        U32 sim_ctl_addr = 0x85FF0000;  // Simulation Control Address (from sim_control.v)
        U32 *sim_ctl_ptr = (U32 *)sim_ctl_addr; // Pointer to the Simulation Control Address
        U32 sim_ctl_byte = 0x3; // Lowest byte of Write-data on AXI to Simulation Conrol Address that indicates that the Test passed or failed
        U32 fail_msg_wdata = ((U32)num_errors << 8) + sim_ctl_byte; // Append the number of Errors in the upper portion [23:8] of the word alongwith the control byte for test-fail indication
        *sim_ctl_ptr = fail_msg_wdata; // Write the Error-information message to the Simulation Control Address through the pointer-value
        return;
}

// Specify that the test has passed (owing to no Errors)
void pass_ctl() {
	 U32 sim_ctl_addr = 0x85FF0000;  // Simulation Control Address (from sim_control.v)
         U32 *sim_ctl_ptr = (U32 *)sim_ctl_addr; // Pointer to the Simulation Control Address
         U32 sim_ctl_byte = 0x3; // Lowest byte of Write-data on AXI to Simulation Conrol Address that indicates that the Test failed
         U32 pass_msg_wdata = sim_ctl_byte; // Since there is no error count to append to theupper portiom [23:8] of the word alongwith the control byte, it means the test passed
	 *sim_ctl_ptr = pass_msg_wdata; // Write the Pass-information message to the Simulation Control Address through the pointer-value
	 return;
}

void nop (volatile int no_of_nop){
  int temp, i;
  temp = 0;
  for(i = 0; i < no_of_nop; i++){
    temp = i;
  }
  return;
}


interrupt_init (void){
U32 i, temp,read_val;
	printf("INIT\n");
	temp = reg_rd(IC_DIST_BASE + 0x004); // ICDICTR Interrupt controller Type Register
	printf("Type 0x%x \n",temp );
  	reg_wr((IC_DIST_BASE + 0x000),0x0); // ICDDCR Distributor Control register

	for (i = 0; i<= 31; i++) {
	reg_wr((IC_DIST_BASE + (0x080 + i*4)),0x0); // Interrupt Security register
		}

	reg_wr((IC_DIST_BASE + 0x100),0xF000FFFF);
	for (i = 0; i<= 6; i++) {
	reg_wr((IC_DIST_BASE + (0x104 + i*4)),0xFFFFFFFF); // Interrrupt Set Enable Register ICDISERn
		}


	for (i = 0; i<= 57; i++) {
	reg_wr((IC_DIST_BASE + (0x800 + i*4)),0x01010101); // SPI Target register
	}


	reg_wr((IC_INT_BASE + 0x004),0xF8); // Priority mask CPU register
	temp = reg_rd(IC_INT_BASE + 0x004); // Priority mask CPU Register
	printf("Prio 0x%x \n",temp );

  	reg_wr((IC_INT_BASE + 0x000),0x1); // ICDDCR Distributor Control register
	temp = reg_rd(IC_INT_BASE + 0x000); // ICDICTR Interrupt controller Type Register
	printf("En 0x%x \n",temp );

	reg_wr((IC_DIST_BASE + 0x000),0x1); // ICDDCR Distributor Control register
	temp = reg_rd(IC_DIST_BASE + 0x000); // ICDICTR Interrupt controller Type Register
	printf("Di 0x%x \n",temp );


	return;

}

// Method to write the specified pattern to the specified memory(IRAM/DDR) Start Address for the specified number of locations(size), or to Check that the test pattern is present correctly at the specified memory Start Address for the specified number of locations
// Input Parameters: start_addr - Start Address of the IRAM/DDR Memory location where we have to write the test pattern to/check the test pattern from
//                   size - Number of sequential (DWORD-aligned) memory addresses to be written/checked including/from the specified start address
//                   pattern_type - Type of test pattern to be written to/checked from the specified start address for the specified number of locations(size) [0 - Increment from 0x0, 1 - Decrement from 0xFFFF_FFFF, 2 - Rolling counter pattern of DWORD-size (0x03020100, 0x04030201, onwards), 3 - Reverse rolling counter pattern of DWORD-size (0x00010203, 0x01020304, onwards), 4 - Inverted rolling counter pattern of DWORD-size (0xFCFDFEFF, 0xFBFCFDFE, onwards) & 5 - Inverted reverse rolling counter pattern of DWORD-size (0xFFFEFDFC, 0xFEFDFCFB, onwards)]
//                   isWrite - Specifies if the specified test pattern is to be written to the memory (else it is used to check from memory)
//                   max_err_cnt - Specifies the number of errors in the Memory Pattern Check tolerates before the method stops further checking and returns [Negative values - Default maximum of 5 errors, 0 - Don't stop on hitting errors & go on until all specified memory locations have been checked]
//                   debug - Enables the printing of debug messages
//                   extra_debug - Enables the printing of extra debug messages (Enabling this automatically enables the 'debug' parameter)
void mem_pattern_gen_check(U32 start_addr, int size, int pattern_type, bool isWrite, int max_err_cnt, bool debug, bool extra_debug)
{
 int i;
 U32 addr, wr_data, rd_data;
 int err_cnt = 5; // Count of Maximum number of errors that are allowed during pattern check (Default: 5)

 // If extra debug messages are enabled, regular debug messages are also enabled
 if(extra_debug)
  debug = true;
 // Update the maximum error count with the value specified through the parameter
 if(max_err_cnt >= 0)
  err_cnt = max_err_cnt;

 if(debug)
 {
  if(isWrite) // Check if we are trying to write the test pattern to the memory address
   printf("Writing the test pattern type:%d to memory locations:0x%x to 0x%x...\n", pattern_type, start_addr, (start_addr + (size * 4)));
  else // Otherwise we are trying to check that a particular test pattern was received correctly at the specified address
   printf("Checking the test pattern type:%d at memory locations:0x%x to 0x%x...\n", pattern_type, start_addr, (start_addr + (size * 4)));
 }

 // Perform the Memory's Pattern-Write/Check for specified range
 for(i = 0; i < size; i++)
 {
  addr = (start_addr + (i * 4));
  switch(pattern_type)
  {
   case 0: wr_data = (i % 256);
           break;
   case 1: wr_data = (0xFFFFFFFF - (i % 256));
           break;
   case 2: wr_data = ((((i+3) % 256) << 24) + (((i+2) % 256) << 16) + (((i+1) % 256) << 8) + (i % 256));
           break;
   case 3: wr_data = (((i % 256) << 24) + (((i+1) % 256) << 16) + (((i+2) % 256) << 8) + ((i+3) % 256));
           break;
   case 4: wr_data = ((((252 - i) % 256) << 24) + (((253 - i) % 256) << 16) + (((254 - i) % 256) << 8) + ((255 - i) % 256));
           break;
   case 5: wr_data = ((((255 - i) % 256) << 24) + (((254 - i) % 256) << 16) + (((253 - i) % 256) << 8) + ((252 - i) % 256));
           break;
   default: printf("Error: mem_pattern_gen_check - Unsupported Test Pattern-type:%d specified for Write!\n", pattern_type);
            return;
  }
  // Check if we are trying to write the test pattern to the memory address
  if(isWrite)
  {
   reg_wr(addr, wr_data);
   if(extra_debug)
    printf("Wrote 0x%x = 0x%x\n", addr, wr_data);
  }
  else // Otherwise we are trying to check that a particular test pattern was received correctly at the specified address
  {
   rd_data = reg_rd(addr);
   if(extra_debug)
    printf("Read 0x%x = 0x%x\n", addr, rd_data);
   // Check if there is a pattern mismatch error
   if(rd_data != wr_data)
   {
    printf("Error: Mem Patt Check - Addr:0x%x, Read:0x%x, Expected: 0x%x!\n", addr, rd_data, wr_data);
    if(err_cnt != 0) // Check that we don't have unlimited perrmissible errors
    {
     if(err_cnt == 1) // Check if this was the last permissible error
     {
      printf("Max no. of errors hit stopping check!\n");
      return;
     }
     else
      err_cnt--; // Decrement the remaining number of errors permitted
    }
   }
  }
 }

 if(debug)
 {
  if(isWrite)
   printf("Completed test pattern writing to memory!\n");
  else
  {
   if(err_cnt == 0)
    printf("Completed memory pattern check!\n");
   else
   {
    if(((max_err_cnt > 0) && (err_cnt == max_err_cnt)) || ((max_err_cnt < 0) && (err_cnt == 5)))
     printf("Completed memory pattern check without errors!\n");
    else
     printf("Completed memory pattern check with %d errors!\n", (max_err_cnt - err_cnt));
   }
  }
 }
}

