#include <common.h>
#include <asm/io.h>
#include "tests.h"
#include "diags.h"
#include <mach/gpio.h>
#include <mach/comcerto-2000.h>

extern unsigned int msif_data[512];

void Comcerto_msif_tdm_test(struct diags_test_param *p)
{
	unsigned int write_data_tx, write_addr_tx, write_addr_rx, write_data_rx;
	unsigned int count, i, indx;
	unsigned int err_cnt=0;
	unsigned int ok_cnt=0;
	unsigned int dst_data;

	printf("MSIF/TDM Diag Test...\n");

	//GPIO -- TDM I/F muxing: select MSIF
	writel(0x30, COMCERTO_GPIO_MISC_PIN_SELECT_REG);

	// writel(0x82, TDMNTG_REF_CLK_DIV_CNTRL);       //keep bypassing clock divider just for simulation
	writel(0x21, 0x904B0280);       //TDM_NTG_CLK_CNTRL - don't bypass NTG logic
	writel(0xefffffD1, 0x904B0284); //TDM_NTG_INCR
	writel(0xFE, 0x904B028C);       //TDM_FSYNC_LOW   (32TS => 32*8-2)
	writel(0x2, 0x904B0290);        //TDM_FSYNC_HIGHN (FS is 2 cycle wide)

	//Pause till slic clock is set before taking slic out of reset
	for (i = 0; i<= 0x10; i++);

	writel(0xC, 0x904B014C); //TDM_CNTRL -> take slic out of reset and enable tdm clk divider (slic/12)

	// TDMA
	writel(0x83, COMCERTO_TDMA_BASE + 0x0); // TDMa Memory segment -> pointer to IRAM
	writel(0x0, COMCERTO_TDMA_BASE + 0x10);  // TDMA - disable interrupts

	for (indx = 0, i = 0; msif_data[indx] != '\0'; indx++, i += 4)
		writel(msif_data[indx], 0x83003000 + i);

	//Initialize Tx Buffer in DDR
	write_addr_tx = 0x83003000;
	write_addr_rx = 0x83002000;
	write_data_tx = 0x03020100;

	for (count = 0; count < 128; count = count + 1) {
		writel(write_data_tx, write_addr_tx);
		writel(0x0, write_addr_rx);

		write_addr_rx = write_addr_rx + 0x4;
		write_addr_tx = write_addr_tx + 0x4;
		write_data_tx = write_data_tx + 0x04040404;
	};

	for (count = 0; count < 32; count = count + 1) {
		writel(0x0, write_addr_tx);
		writel(0x0, write_addr_rx);

		write_addr_rx = write_addr_rx + 0x4;
		write_addr_tx = write_addr_tx + 0x4;
	};

	// Tx and Rx Memory base address - configure 32 channels as width 0
	write_data_tx = 0x00003000; // Channel Width - 0 and Base address -  0x8300_3000
	write_data_rx = 0x00002000; // Channel Width - 0 and Base address -  0x8300_2000
	write_addr_rx = 0x90421000; // TDMA  Rx Memory base address register
	write_addr_tx = 0x90421200; // TDMA  Tx Memory base address register

	for (count = 0; count < 32; count++) {
		writel(write_data_tx, write_addr_tx);
		writel(write_data_rx, write_addr_rx);

		write_addr_tx = write_addr_tx + 0x4;
		write_addr_rx = write_addr_rx + 0x4;
		write_data_tx = write_data_tx + 0x10;
		write_data_rx = write_data_rx + 0x10;
	};

	//TDMA Rx Look up Table register
	write_data_rx = 0x0;        // Channel Width - 0
	write_addr_rx = 0x90421400; // TDMA Rx Look up Table register

	for (count = 0; count < 32; count++) {
		writel(write_data_rx, write_addr_rx);

		write_addr_rx = write_addr_rx + 0x4;
		write_data_rx = write_data_rx + 0x10;
	};

	//TDMA Tx Look up Table register (unlike Rx, RAM address must be 1 less than TS# - refer to 2.4.1.2.2 in HDD)
	write_data_tx = 0x0;        // Channel Width - 0
	write_addr_tx = 0x9042167C;

	writel(write_data_rx, write_addr_tx);

	write_data_tx = write_data_tx + 0x10;
	write_addr_tx = 0x90421600; // TDMA Tx Look up Table register

	for (count = 0; count < 31; count++) {
		writel(write_data_rx, write_addr_tx);

		write_addr_tx = write_addr_tx + 0x4;
		write_data_tx = write_data_tx + 0x10;
	};

	writel(0x00000000, 0x90420030); // Set TDM Max to ts128 and DMA every 4-bytes (32-bit ahb bus)
	writel(0xFFFFFFFF, 0x90420010); // TDMA - 0 IRQ2TDMA INT EN

	//Enable 1 channels
	writel(0xffffffff, 0x90420060); // TDMA Rx Channel enable(0-31)
	writel(0x00000000, 0x90420064); // TDMA Rx Channel enable(32-63)
	writel(0x00000000, 0x90420068); // TDMA Rx Channel enable(64-95)
	writel(0x00000000, 0x9042006C); // TDMA Rx Channel enable(96-127)
	writel(0xffffffff, 0x90420070); // TDMA Tx Channel enable(0-31)
	writel(0x00000000, 0x90420074); // TDMA Tx Channel enable(32-63)
	writel(0x00000000, 0x90420078); // TDMA Tx Channel enable(64-95)
	writel(0x00000000, 0x9042007C); // TDMA Tx Channel enable(96-127)

	writel(0x00000003, 0x90420020); // TDMA Block size - 4 words
	writel(0x00000002, 0x90420024); // TDMA Interrupt block size - 3 words

	writel(0x00000001, 0x90420004); // TDMA Rx DMA enable
	writel(0x00000001, 0x90420008); // TDMA Tx DMA enable

	// TDM
	//Internal Loopback is off ; Frame Edge is sampled @falling ; 32TS
	writel(0x000000ff, COMCERTO_TDM_BASE + 0x4);

	// Enable TX/RX active slots (ts0-ts127);
	// # set tx channels 0-127 activ
	writel(0xffffffff, COMCERTO_TDM_BASE + 0x014);
	writel(0x00000000, COMCERTO_TDM_BASE + 0x018);
	writel(0x00000000, COMCERTO_TDM_BASE + 0x01c);
	writel(0x00000000, COMCERTO_TDM_BASE + 0x020);

	//# set rx channels 0-127 active
	writel(0xffffffff, COMCERTO_TDM_BASE + 0x034);
	writel(0x00000000, COMCERTO_TDM_BASE + 0x038);
	writel(0x00000000, COMCERTO_TDM_BASE + 0x03c);
	writel(0x00000000, COMCERTO_TDM_BASE + 0x040);

	// Enable Transmitter.. Drive TDMDX (1 clk per data)
	writel(0x440ff, COMCERTO_TDM_BASE + 0x10);// # enable Tx, 1 clks per data, data is driven @rising ; delay data

	// Pause till start transmitting real data and not x
	for (i = 0; i<= 0x3400; i++);

	// Enable Receiver..(1 clk per data)
	writel(0x44001, COMCERTO_TDM_BASE + 0x30); // # enable Rx, 1 clks per data, data is received @rising ; 0x1 cycles delay from FS (since LPK is used)

	// Pause for at least 5 frames
	for (i = 0; i<= 0xb000; i++);

	//Disable Transmitter
	writel(0x0, COMCERTO_TDM_BASE + 0x010);

	//Disable Receiver
	writel(0x0, COMCERTO_TDM_BASE + 0x030);

	//Manual Data Integrity Check
	write_addr_rx = 0x83002000;
	write_data_tx = 0x03020100;

	for (count = 0; count < 128; count = count + 1) {
		dst_data = *((volatile unsigned int *) (write_addr_rx));

		if (dst_data == write_data_tx) {
			//printf("Manual Data Integrity Check OKAY - addr=0x%x, data=0x%x \n",write_addr_rx, dst_data);
			ok_cnt++;
		} else {
			printf("Manual Data Integrity Check MISMATCH - addr=0x%x, data=0x%x, expected=0x%x \n",write_addr_rx, dst_data, write_data_tx);
			err_cnt++;
		}

		write_addr_rx = write_addr_rx + 0x4;
		write_data_tx = write_data_tx + 0x04040404;
	};

	//Test Ends here ...
	if (err_cnt == 0)
		printf("Test Passed - checked 0x%x entries \n", ok_cnt);
	else
		printf("Test Failed with 0x%x Errors \n", err_cnt);
}

