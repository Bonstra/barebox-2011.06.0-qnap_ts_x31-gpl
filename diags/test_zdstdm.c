#include "tests.h"
#include <diags.h>
#include <common.h>
#include <asm/io.h>
#include <mach/zds.h>

extern unsigned int zds_data[512];

static inline void delay(volatile u32 delay_count)
{
	while(delay_count--);
}

void tdm_init(void)
{
	printf("Initializing TDM..\n");

	// Set TDMCTS (TDM Block)
	writel(0x55555555, TDM0_CTSEN_A);
	writel(0x55555555, TDM0_CTSEN_B);
	writel(0x55555555, TDM0_CTSEN_C);
	writel(0x55555555, TDM0_CTSEN_D);

	// Set TDM Max to ts128 and DMA every 4-bytes (32-bit ahb bus)
	writel(0x0, TDMA0_BACKCOM); // ts128 on, dma 4bytes

	// Enable Internal Loopback - Set TDM Time-Slots: 128 ts/frame
	writel(0x000003ff, TDM0_NETWORK);

	// Enable Transmitter.. Drive TDMDX (1 clk per data)
	writel(0x40000, TDM0_TX);// enable receiver, 1 clks per data

	// Enable TX/RX active slots (ts0-ts127)
	writel(0xFFFFFFFF, TDM0_TX_TSEN_A);
	writel(0xFFFFFFFF, TDM0_TX_TSEN_B);
	writel(0xFFFFFFFF, TDM0_TX_TSEN_C);
	writel(0xFFFFFFFF, TDM0_TX_TSEN_D);

	writel(0xFFFFFFFF, TDM0_RX_TSEN_A);
	writel(0xFFFFFFFF, TDM0_RX_TSEN_B);
	writel(0xFFFFFFFF, TDM0_RX_TSEN_C);
	writel(0xFFFFFFFF, TDM0_RX_TSEN_D);

	// Pause till  both frame sync. (tdm_dfx & tdm_dfr) come out
	delay(0x1100);

	// Enable Receiver..(1 clk per data)
	writel(0x40001, TDM0_RX); // # enable Rx, 1 clks per data, 1 clk delay since 0 delay loopback

	// Pause for about (8 frames) hclk = 10.5 ns and tdm_clk = 30 ns
	delay(0x10000);

	//Disable Transmitter
	writel(0x0, TDM0_TX);

	//Disable Receiver
	writel(0x0, TDM0_RX);
}

void tdma_init(void)
{
	u32 write_data_tx, write_addr_tx, write_addr_rx, write_data_rx;
	u32 count, i, indx;

	printf("Initializing TDMA..\n");

	// TDM memory segment
	writel(0x83, COMCERTO_TDMA_BASE);
	writel(0x0, IRQ2TDMA_INTERRUPT_ENABLE);

	for (indx = 0, i = 0; zds_data[ indx ] != '\0'; indx++, i+=4)
		writel(zds_data[indx], (IRAM_CHKER_SRC_ADDR + 0x1000) + i);

	// TDMA - 0
	// Tx and Rx Memory base address
	write_data_tx = 0x00001000; // Channel Width - 0 and Base address -  0x8300_1000
	write_data_rx = 0x00002000; // Channel Width - 0 and Base address -  0x8300_2000
	write_addr_rx = TDMA0_RXBASE0; // TDMA  Rx Memory base address register
	write_addr_tx = TDMA0_TXBASE0; // TDMA  Tx Memory base address register

	for (count = 0; count < 128; count++) {
		writel(write_data_tx, write_addr_tx);
		writel(write_data_rx, write_addr_rx);
		write_addr_tx =+ 0x4;
		write_addr_rx =+ 0x4;
		write_data_tx =+ 0x4;
		write_data_rx =+ 0x4;
	};

	write_data_rx = 0x0;        // Channel Width - 0
	write_addr_rx = TDMA0_RXLKTB0; // TDMA Rx Look up Table register
	for (count = 0; count < 128; count++) {
		writel(write_data_rx, write_addr_rx);
		write_addr_rx =+ 0x4;
		write_data_rx =+ 0x10;
	};

	write_data_tx = 0x0;        // Channel Width - 0
	write_addr_tx = TDMA0_TXLKTB0; // TDMA Tx Look up Table register
	for (count = 0; count < 128; count++) {
		writel(write_data_tx, write_addr_tx);
		write_addr_tx =+ 0x4;
		write_data_tx =+ 0x10;
	};

	writel(0xFFFFFFFF, IRQ2TDMA_INTERRUPT_ENABLE); // TDMA Rx Channel enable(0-31)
	writel(0xFFFFFFFF, TDMA0_RXCHEN_A); // TDMA Rx Channel enable(0-31)
	writel(0xFFFFFFFF, TDMA0_RXCHEN_B); // TDMA Rx Channel enable(32-63)
	writel(0xFFFFFFFF, TDMA0_RXCHEN_C); // TDMA Rx Channel enable(64-95)
	writel(0xFFFFFFFF, TDMA0_RXCHEN_D); // TDMA Rx Channel enable(96-127)
	writel(0xFFFFFFFF, TDMA0_TXCHEN_A); // TDMA Tx Channel enable(0-31)
	writel(0xFFFFFFFF, TDMA0_TXCHEN_B); // TDMA Tx Channel enable(32-63)
	writel(0xFFFFFFFF, TDMA0_TXCHEN_C); // TDMA Tx Channel enable(64-95)
	writel(0xFFFFFFFF, TDMA0_TXCHEN_D); // TDMA Tx Channel enable(96-127)

	writel(0x00000003, TDM_BLOCK_SIZE); // TDMA Block size - 4 words
	writel(0x00000002, TDMA0_INTSIZE); // TDMA Interrupt block size - 3 words

	writel(0x00000001, TDM_RX_DMA_ENABLE); // TDMA Rx DMA enable
	writel(0x00000001, TDM_TX_DMA_ENABLE); // TDMA Tx DMA enable
	writel(0x00000000, TDMA0_BACKCOM); // DMA 8 bytes
}

void zds_init(void)
{
	printf("Initializing ZDS..\n");

	// Select ZDS block program GPIO 0x60 register with value 0x10 for the ZDS mode selection
	writel(0x10, COMCERTO_GPIO_MISC_PIN_SELECT_REG);

	// Configure FSYNC to be Output and FCLK as well
	writel(0x1, COMCERTO_GPIO_TDM_MUX); // FSYNC
	writel(0x3000, COMCERTO_GPIO_BOOTSTRAP_OVERRIDE); // TDM clock

	writel(0x21, NTG_FREQ_SET_INT);
	writel(0x3254E6E5, NTG_FREQ_SET_FRA); //NTG 250Mhz
	writel(0x1FF, (COMCERTO_NTG_BASE + 0xC));
	writel(0x1, NTG_PHASE_ADJ_FREQ_INT);

	// Be in ZDS mode and configure the clocks
	writel(0x8C, CLOCK_RESET_SLIC_CONTROL);	// TDM_CLK = SLIC_CLK/12 , 49.152/12 = 4.096Mhz
	delay(100);

	// Configure the TDM block
	writel(0x0000003FF, TDM0_NETWORK);
	writel(0x00004C000, TDM0_TX);
	writel(0xFFFFFF00, TDM0_TX_TSEN_A);
	writel(0xFFFFFF00, TDMA_TDM0RX_BIST_CTRL);
	writel(0xFFFFFF00, TDMA_TDM0TX_BIST_CTRL);
	writel(0xFFFFFF00, TDM_BLOCK_SIZE);

	writel(0xFFFFFF00, TDM0_RX_TSEN_A);
	writel(0xFFFFFF00, TDM0_RX_TSEN_B);
	writel(0xFFFFFF00, TDM0_RX_TSEN_C);
	writel(0xFFFFFF00, TDM0_RX_TSEN_D);

	delay(0x1100);

	// enable Rx, 1 clks per data, 1 clk delay since 0 delay loopback
	writel(0x4C000, TDMA0_BACKCOM);
	delay(0x1100);

	// Configure the SPI block for CS_3 and generate clock as per external clock requirement
	writel(0x03, COMCERTO_SPI_BASE + COMCERTO_SPI_RXFTLR); // Rx FIFO threshold
	writel(0x7007, COMCERTO_SPI_BASE + COMCERTO_SPI_CTRLR0); // 08bit frame with CPH=0 and CPL=0
//	writel(0xCF, COMCERTO_SPI_BASE + COMCERTO_SPI_CTRLR0); // 16bit frame with CPH=1 and CPL=1
	writel(0x01, COMCERTO_SPI_BASE + COMCERTO_SPI_CTRLR1);
//	writel(0x03, COMCERTO_SPI_BASE + COMCERTO_SPI_CTRLR1); // No. of frames to be received continuously
	writel(0x02D, COMCERTO_SPI_BASE + COMCERTO_SPI_BAUDR);// SSI clock divider
//	writel(0x140, COMCERTO_SPI_BASE + COMCERTO_SPI_BAUDR);
	writel(0x00, COMCERTO_SPI_BASE + COMCERTO_SPI_IMR); // No interrupt is enabled
	writel(0x08, COMCERTO_SPI_BASE + COMCERTO_SPI_SER); // Enable slave-0
//	writel(0x01, COMCERTO_SPI_BASE + COMCERTO_SPI_SER);
	writel(0x01, COMCERTO_SPI_BASE + COMCERTO_SPI_SSIENR); // Enable SPI

	// SPI config done now write commands
	writel(0x0C, CLOCK_RESET_SLIC_CONTROL); // TDM_CLK = SLIC_CLK/12 , 49.152/12 = 4.096Mhz
	delay(1200);
	//delay(12000);
}

static u32 do_data_integrity_check(u32 src_adr,u32 dest_adr,u32 length)
{
	int count;

	for (count = 0; count < length; count++) {
		if (readl(src_adr) != readl(dest_adr)) {
			printf("Mismatch: src value=0X%02x, dest value=0X%02x \n", readl(src_adr), readl(dest_adr));
			return 1;
		}
		src_adr =+ 0x4;
		dest_adr =+ 0x4;
	}

	return 0;
}

void zds_test_1(void)
{
	int read_data;

	printf("ZDS Test 1 started...\n");

	tdma_init();
	tdm_init();

	// slic loop back
	writel(0x70, COMCERTO_SPI_BASE + COMCERTO_SPI_DR);
	read_data = readl(COMCERTO_SPI_BASE + COMCERTO_SPI_SR);
	while (read_data != 0x0E)
		read_data = readl(COMCERTO_SPI_BASE + COMCERTO_SPI_SR);
	read_data = readl(COMCERTO_SPI_BASE + COMCERTO_SPI_DR);
	delay(250);

        writel(0x35, COMCERTO_SPI_BASE + COMCERTO_SPI_DR);
        read_data = readl(COMCERTO_SPI_BASE + COMCERTO_SPI_SR);
        while (read_data != 0x0E)
                read_data = readl(COMCERTO_SPI_BASE + COMCERTO_SPI_SR);
        read_data = readl(COMCERTO_SPI_BASE + COMCERTO_SPI_DR);
	delay(250);

	// Enable Transmitter.. Drive TDMDX (1 clk per data)
	writel(0x40000, TDM0_TX); // enable receiver, 1 clks per data

	// Enable Receiver..(1 clk per data)
	writel(0x40001, TDM0_RX); // # enable Rx, 1 clks per data, 1 clk delay since 0 delay loopback

	// Pause for about (8 frames) hclk = 10.5 ns and tdm_clk = 30 ns
	delay(0x10000);

	if(do_data_integrity_check(0x83001000, 0x83002000, 512) != 0)
		printf("ZDS Test 1 failed\n");
	else
		printf("ZDS Test 1 passed\n");

	//Disable Transmitter
        writel(0x0, TDM0_TX);

        //Disable Receiver
        writel(0x0, TDM0_RX);
}

void zds_test_2(void)
{
	int read_data;

	printf("ZDS Test 2 started...\n");

	// enable wideband mode
	writel(0x4a, COMCERTO_SPI_BASE + COMCERTO_SPI_DR);
	read_data = readl(COMCERTO_SPI_BASE + COMCERTO_SPI_SR);
	while (read_data != 0x0E)
		read_data = readl(COMCERTO_SPI_BASE + COMCERTO_SPI_SR);
	read_data = readl(COMCERTO_SPI_BASE + COMCERTO_SPI_DR);
	delay(250);

        writel(0x20, COMCERTO_SPI_BASE + COMCERTO_SPI_DR);
        read_data = readl(COMCERTO_SPI_BASE + COMCERTO_SPI_SR);
        while (read_data != 0x0E)
                read_data = readl(COMCERTO_SPI_BASE + COMCERTO_SPI_SR);
        read_data = readl(COMCERTO_SPI_BASE + COMCERTO_SPI_DR);
	delay(250);

	tdma_init();
	tdm_init();

	// slic loop back
	writel(0x70, COMCERTO_SPI_BASE + COMCERTO_SPI_DR);
	read_data = readl(COMCERTO_SPI_BASE + COMCERTO_SPI_SR);
	while (read_data != 0x0E)
		read_data = readl(COMCERTO_SPI_BASE + COMCERTO_SPI_SR);
	read_data = readl(COMCERTO_SPI_BASE + COMCERTO_SPI_DR);
	delay(250);

        writel(0x35, COMCERTO_SPI_BASE + COMCERTO_SPI_DR);
        read_data = readl(COMCERTO_SPI_BASE + COMCERTO_SPI_SR);
        while (read_data != 0x0E)
                read_data = readl(COMCERTO_SPI_BASE + COMCERTO_SPI_SR);
        read_data = readl(COMCERTO_SPI_BASE + COMCERTO_SPI_DR);
	delay(250);

        // Enable Transmitter.. Drive TDMDX (1 clk per data)
        writel(0x40000, TDM0_TX); // enable receiver, 1 clks per data

        // Enable Receiver..(1 clk per data)
	writel(0x40001, TDM0_RX); // # enable Rx, 1 clks per data, 1 clk delay since 0 delay loopback

        // Pause for about (8 frames) hclk = 10.5 ns and tdm_clk = 30 ns
        delay(0x10000);

	if(do_data_integrity_check(0x83001000, 0x83002000, 512) != 0)
		printf("ZDS Test 2 failed\n");
	else
		printf("ZDS Test 2 passed\n");

	//Disable Transmitter
	writel(0x0, TDM0_TX);

        //Disable Receiver
        writel(0x0, TDM0_RX);
}

int Comcerto_zds_tdm_test (struct diags_test_param *p)
{
	int test_case = p->params[0];

	switch(test_case)
	{
		case 1:
			zds_test_1();
			break;
		case 2:
			zds_test_2();
			break;
		default:
			printf("Enter correct test case number\n");
			break;
	}

	return 0;
}

