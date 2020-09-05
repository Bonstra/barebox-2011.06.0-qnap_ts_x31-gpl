/*************************************************************************************************
* Validation code for Fast UART block
*
* All tests cases come from DSPG / MSPD VLSI Team, and adapted for barebox validation framework
*
**************************************************************************************************/

#include "tests.h"
#include <diags.h>
#include "fastuart.h"
#include <mach/dma.h>
#include <common.h>
#include <asm/types.h>
#include <asm/io.h>

/* make sure following define reflects exactly the number of tests cases in fast_uart_test_list */
#define	FAST_UART_NUM_TESTS	4

extern void dma_configure(unsigned int source_add, unsigned int target_add, unsigned int data_len, unsigned int ch_no);

void fastuart_configure(void)
{
	writel(0x80, UART0_BASEADDR + UART_LCR); /* Enable Data latch to write divisor latch */
	writel(0x02, UART0_BASEADDR + UART_DLL); /* Divisor latch(low) is set to 2 for baud rate clock */
	writel(0x00, UART0_BASEADDR + UART_DLH); /* Divisor latch(high) is set to 0 for baud rate clock */
        writel(0x00, UART0_BASEADDR + UART_LCR); /* Disable data latch */
	writel(0x0B, UART0_BASEADDR + UART_LCR); /* Enable parity and Eight bits per character */
	writel(0x01, UART0_BASEADDR + UART_IER); /* Enable Rx Interrupt */
}

/* Fast UART test Without DMA */
void fast_uart_test1(void)
{
	int err_cnt = 0, read_data, i;
	int const_data[32] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x30, 0x31, 0x32 };

	writel(0xAA0000, GPIO_PIN_SELECT_REG);  //GPIO vs UART pins selection
	writel(0x0, GPIO_MISC_PIN_REG);  // DMA UART vs UART_s2

	fastuart_configure();

	writel(0x07, UART0_BASEADDR + UART_FCR); /* Reset Tx and Rx FIFOs; Enable FIFO mode; Set Rx FIFO threshold */

	/* write to GPIO to start the model */
	writel(0xF, GPIO_OUTPUT_REG);
	writel(0xF, GPIO_OE_REG);

	// Writing to UART0 Data Register
	writel(0xFF, UART0_BASEADDR + UART_THR);
	read_data = readl(UART0_BASEADDR + UART_LSR);
	while(read_data != 0x61) // Wait for Transmit Empty and receiver contains atleast one character
		read_data = readl(UART0_BASEADDR + UART_LSR);

	// Reading from UART0 Data Register
	read_data = readl(UART0_BASEADDR + UART_THR);
	if(read_data != 0xFF)
	{
		err_cnt++;
		printf("Error:: Mismatch on the received dataword\n");
	};

	// Writing to UART0 Data Register
	for ( i=0; i < 32; i++ )
		writel(const_data[i], UART0_BASEADDR + UART_THR);

	// Wait for Transmit Empty and receiver contains atleast one character
	read_data = readl(UART0_BASEADDR + UART_LSR);
	while(read_data != 0x61) // Wait for Transmit Empty and receiver contains atleast one character
		read_data = readl(UART0_BASEADDR + UART_LSR);

	// Reading from UART0 Data Register
	for ( i=0; i < 32; i++ )
	{
		read_data = readl(UART0_BASEADDR + UART_THR);
		if(read_data != (const_data[i] & 0xff))
			err_cnt++;
	}

	if(err_cnt > 0x0)
		printf("Fast UART TEST 1 FAILED \n");
	else
		printf("Fast UART TEST 1 PASSED \n");
}

int uart0_err(void)
{
	int read_data, err_cnt = 0,err_total =0, i=0, temp;
	int const_data[32] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x30, 0x31, 0x32 };

	writel(0xAA0000, GPIO_PIN_SELECT_REG);  // GPIO vs UART pins selection
	writel(0x0, GPIO_MISC_PIN_REG);  // DMA UART vs UART_s2

	fastuart_configure();

	writel(0xa7, UART0_BASEADDR + UART_FCR); // Reset Tx and Rx FIFOs; Enable FIFO mode; Set Rx FIFO threshold
	writel(0x22, UART0_BASEADDR + UART_MCR); // enable auto flow control and drive RTS

	// write to GPIO to start the model
	writel(0x1, GPIO_OUTPUT_REG); // UART0 model instance will be enabled now
	writel(0xF, GPIO_OE_REG);

	// Checking the parity error & recover
	// start reading data
	read_data = readl(UART0_BASEADDR + UART_LSR);
	while(read_data != 0x61) // Wait for Transmit Empty and receiver contains atleast one character
		read_data = readl(UART0_BASEADDR + UART_LSR);

	// wait till you have 4 data
	read_data = readl(UART0_BASEADDR + UART_RFL);  //RX FIFO level register
	while (read_data < 4)
	read_data = readl(UART0_BASEADDR + UART_RFL);

	for(i=0; i < 4; i++)
	{
		read_data = readl(UART0_BASEADDR + UART_RBR);
		if(read_data != const_data[i])
			err_cnt ++;
	}

	// wait till you have 4 data with parity error
	read_data = readl(UART0_BASEADDR + UART_RFL);  //RX FIFO level register
	while (read_data < 4)
		read_data = readl(UART0_BASEADDR + UART_RFL);

	for(i=0; i < 4; i++)
	{
		read_data = readl(UART0_BASEADDR + UART_LSR);
		if(read_data != 0xe5)
		{
			printf("PE Not detected\n");
			err_cnt ++;
		}

		read_data = readl(UART0_BASEADDR + UART_RBR);
		if(read_data != const_data[i])
			err_cnt ++;
	}

	read_data = readl(UART0_BASEADDR + UART_LSR);
	while(read_data != 0x61) // Wait for all parity error bits read
		read_data = readl(UART0_BASEADDR + UART_LSR);

	// wait till you have 4 data
	read_data = readl(UART0_BASEADDR + UART_RFL);  //RX FIFO level register
	while (read_data < 4)
		read_data = readl(UART0_BASEADDR + UART_RFL);

	for(i=0;i<4;i++)
	{
		read_data = readl(UART0_BASEADDR + UART_RBR);
		if(read_data != const_data[i])
			err_cnt ++;
	}

	if(err_cnt > 0)
		printf("parity failed\n");
	else
		printf("parity passed\n");

	err_total = err_total+ err_cnt;

	// check the Framing error (data with framing error already in FIFO)
	// wait till you have 4 data with parity error
	err_cnt =0;
	read_data = readl(UART0_BASEADDR + UART_RBR);  //RX FIFO level register
	while (read_data < 4)
		read_data = readl(UART0_BASEADDR + UART_RBR);

	for(i=0;i<4;i++)
	{
		read_data = readl(UART0_BASEADDR + UART_LSR);
		if(read_data != 0xe9)
		{
			printf("PE Not detected\n");
			err_cnt ++;
		}

		read_data = readl(UART0_BASEADDR + UART_RBR);
		temp = readl(UART0_BASEADDR + UART_RBR); // This is junk data 0xff from model
		if(read_data != const_data[i])
			err_cnt ++;
	}

	// read the data after frame error recovery
	// wait till you have 4 data
	read_data = readl(UART0_BASEADDR + UART_RBR);  //RX FIFO level register
	while (read_data < 4)
		read_data = readl(UART0_BASEADDR + UART_RBR);

	for(i=0; i < 4; i++)
	{
		read_data = readl(UART0_BASEADDR + UART_RBR);
		if(read_data != const_data[i])
			err_cnt ++;
	}

	if(err_cnt > 0)
		printf("framing failed\n");
	else
		printf("framing passed\n");

	err_total = err_total+ err_cnt;

	// check the overrun status
	// model is sending 34 packets which will cause overrun bit to set
	err_cnt = 0 ;

	read_data = readl(UART0_BASEADDR + UART_LSR);
	while(read_data != 0x63) // Wait for all overrun error bits read
		read_data = readl(UART0_BASEADDR + UART_LSR);

	read_data = readl(UART0_BASEADDR + UART_LSR);
	if (read_data == 0x61) // check for over run error clear
		printf ("OR Pass \n");
	else
		printf ("OR FAIL \n");

	// later insert ISR to read the data.
	for ( i=0; i < 36; i++)
		read_data = readl(UART0_BASEADDR + UART_RBR);

	err_total = err_total+ err_cnt;

	return err_total;
}

void fast_uart_test2(void)
{
	int err_value;

	err_value = uart0_err();
	if(err_value > 0)
		printf("Fast UART Test 2 Failed \n");
	else
		printf("Fast UART Test 2 Passed \n");
}

void fast_uart_test3(void)
{
	int err_cnt = 0, channel_status, access_addr, data_read, i = 0;
	int const_data[160] = { 0x04030201, 0x08070605,	0x12111009, 0x16151413, 0x20191817, 0x24232221,	0x28272625, 0x32313029,	0x36353433, 0x40393837, 0x44434241, 0x48474645, 0x52515049, 0x56555453, 0x60595857, 0x64636261, 0x68676665, 0x72717069, 0x76757473, 0x80797877, 0x84838281, 0x88878685, 0x92919089, 0x96959493,  0xa0999897, 0x04030201, 0x08070605, 0x12111009, 0x16151413, 0x20191817, 0x24232221, 0x28272625, 0x32313029, 0x36353433, 0x40393837, 0x44434241, 0x48474645, 0x52515049, 0x56555453, 0x60595857, 0x64636261, 0x68676665, 0x72717069, 0x76757473, 0x80797877, 0x84838281, 0x88878685, 0x92919089, 0x96959493, 0xa0999897, 0x04030201, 0x08070605, 0x12111009, 0x16151413, 0x20191817, 0x24232221, 0x28272625, 0x32313029, 0x36353433, 0x40393837, 0x44434241, 0x48474645, 0x52515049, 0x56555453, 0x60595857, 0x64636261, 0x68676665, 0x72717069, 0x76757473, 0x80797877, 0x84838281, 0x88878685, 0x92919089, 0x96959493, 0xa0999897, 0x04030201, 0x08070605, 0x12111009, 0x16151413, 0x20191817, 0x24232221, 0x28272625, 0x32313029, 0x36353433, 0x40393837, 0x44434241, 0x48474645, 0x52515049, 0x56555453, 0x60595857, 0x64636261, 0x68676665, 0x72717069, 0x76757473, 0x80797877, 0x84838281, 0x88878685, 0x92919089, 0x96959493, 0xa0999897, 0x04030201, 0x08070605, 0x12111009, 0x16151413, 0x20191817, 0x24232221, 0x28272625, 0x32313029, 0x36353433, 0x40393837, 0x44434241, 0x48474645, 0x52515049, 0x56555453, 0x60595857, 0x64636261, 0x68676665, 0x72717069, 0x76757473, 0x80797877, 0x84838281, 0x88878685, 0x92919089, 0x96959493, 0xa0999897, 0x04030201, 0x08070605, 0x12111009, 0x16151413, 0x20191817, 0x24232221, 0x28272625, 0x32313029, 0x36353433, 0x40393837, 0x44434241, 0x48474645, 0x52515049, 0x56555453, 0x60595857, 0x64636261, 0x68676665, 0x72717069, 0x76757473, 0x80797877, 0x84838281, 0x88878685, 0x92919089, 0x96959493, 0xa0999897 };

	writel(0xAA0000, GPIO_PIN_SELECT_REG);  //GPIO vs UART pins selection
	writel(0x0, GPIO_MISC_PIN_REG);  // DMA UART vs UART_s2

	fastuart_configure();

	writel(0x07, UART0_BASEADDR + UART_FCR); // Reset Tx and Rx FIFOs; Enable FIFO mode; Set Rx FIFO threshold
	writel(0x20, UART0_BASEADDR + UART_MCR); // enable auto flow control and drive RTS

	// write to GPIO to start the model
	writel(0xF, GPIO_OUTPUT_REG);
	writel(0xF, GPIO_OE_REG);

	writel(0x07, UART0_BASEADDR + UART_FCR); // For clearing the DMA request ( Need when we enable the DMA)

	// Now enabling the DMA to move data from DDR to UART0 on channel 0
	dma_configure(const_data, UART0_BASEADDR + UART_THR, 0x40, DMA_CHANNEL_0);

	// Wait here till channel is disabled by HW
	channel_status = 0xf;
	while(channel_status != 0x0)
	{
		channel_status = readl(DW_DMA_DMAC_CH_EN_REG);
		channel_status = channel_status & 0x00000001;
	}

	printf("TRANS DONE\n");

	err_cnt = 0;
	for(i = 0; i < 128; i++)
	{
		access_addr = 0x200000 + (i * 4);
		data_read = readl(access_addr);
		printf("data read = %x, data_expected = %x \n", data_read, const_data[i]);
		if(data_read != const_data[i])
			err_cnt++;
	}

	if(err_cnt > 0)
		printf("Fast UART Test 3 FAIL\n");
	else
		printf("Fast UART Test 3 Passed\n");
}

int Comcerto_fast_uart_test (struct diags_test_param *p)
{
	int test_case = p->params[0];

	switch(test_case)
	{
		case 1: fast_uart_test1();
			break;

		case 2: fast_uart_test2();
			break;

		case 3: fast_uart_test3();
			break;

		default: printf("Not a valid test case number\n");
			 break;
	}

	return 0;
}


