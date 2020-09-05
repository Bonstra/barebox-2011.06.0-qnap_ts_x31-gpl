#include <boot.h>
#include <common.h>
#include <command.h>
#include <init.h>
#include <clock.h>
#include <config.h>
#include <mach/comcerto-2000.h>
#include <mach/gpio.h>
#include <asm/io.h>

#define ULOADER_PART_SIZE 0x20000 /* 128 KB */
#define BAREBOX_PART_SIZE 0x80000 /* 256 KB */
#define BAREBOX_LODING_ADDR (COMCERTO_AXI_DDR_BASE + 0x1000000)

#ifdef CONFIG_COMCERTO_NAND_ULOADER
extern int read_nand(ulong src, ulong offset, ulong count);
#endif

#if 0 
static int bb_timeout(int timeout)
{
	int ret = 1;
	int countdown;
	uint64_t start, second;
	timeout = 3;

	start = get_time_ns();
	second = start;

	countdown = timeout;

	printf("\n%2d", countdown--);

	do {
		if (tstc()) {
			getc();
			ret = 1;
			goto  out;
		}

		if (is_timeout(second, SECOND)) {
			printf("\b\b%2d", countdown--);
			second += SECOND;
		}
	} while (!is_timeout(start, timeout * SECOND));

	ret = 0;
out:
	printf("\n");

	return ret;
}
#endif

static void bb_go(void *addr)
{
	int	(*func)(void);

	printf("## Starting Barebox at 0x%p ...\n", addr);

	console_flush();

	func = addr;

	shutdown_barebox();
	func();

	/*
	 * The application returned. Since we have shutdown barebox and
	 * we know nothing about the state of the cpu/memory we can't
	 * do anything here.
	 */
	while (1);
}

#ifndef CONFIG_COMCERTO_NAND_ULOADER
extern int spi_copy_read(char *, unsigned int , unsigned int , unsigned int );
#endif

#ifndef CONFIG_COMCERTO_NAND_ULOADER
static void copy_bb_from_spi_flash(char *dst, int count)
{
        unsigned int l=0;
        unsigned int sec=2;
        unsigned int size=0;

	size = count;

	while (l < (count/SPI_FLASH_SECTOR_SIZE))
        {	
		spi_copy_read((char*)dst, SPI_FLASH_SECTOR_SIZE, sec, 0);

		size -= SPI_FLASH_SECTOR_SIZE;
		dst += SPI_FLASH_SECTOR_SIZE;
		sec += 1;
		l++;
	}

	if(size)
        {
		spi_copy_read((char*)dst, size, sec, 0);
	}

        printf("BB Copying Done\n");

        return;
}

#if !defined(CONFIG_MACH_COMCERTO_C2K_EVM) && !defined(CONFIG_MACH_COMCERTO_C2K_MFCNEVM)
static void copy_bb_from_fast_spi_flash(char *dst, int count)
{
	unsigned int sec = 0x2; // Barebox is stored at sec 0x2 in Fast SPI flash
	unsigned int offset = 0x0; // Barebox is stored at offset 0x0
	unsigned int size = count;

	fast_spi_copy_read((char*)dst, size, sec, offset);

	printf("BB Copying Done\n");

	return;
}
#endif
#endif

static int do_bootb_barebox(void)
{
	volatile u32 *src = (u32 *)(COMCERTO_AXI_EXP_BASE + ULOADER_PART_SIZE); /* Start of NOR + uLdr size(128K) */
	volatile u32 *dst = (u32*)BAREBOX_LODING_ADDR;
	int count = BAREBOX_PART_SIZE; /* 256K for Barebox */
	u32 bootopt;
#if 0
	int timeout = 3;

	if(bb_timeout(timeout))
		return 0;
#endif

#ifdef CONFIG_COMCERTO_NAND_ULOADER
	/* this option is used when uloader is in NOR flash or I2C EEPROM and 
	barebox is in NAND */
	printf("\nCopying Bb from NAND\n");
	read_nand(BAREBOX_LODING_ADDR, 0x0, BAREBOX_PART_SIZE);
#else

	bootopt = ((readl(COMCERTO_GPIO_SYSTEM_CONFIG) >>  BOOTSTRAP_BOOT_OPT_SHIFT) & BOOTSTRAP_BOOT_OPT_MASK);
	
	switch(bootopt){
		case BOOT_LS_SPI:
			//With SPI boot, the barebox will also reside in SPI flash
			printf("\nCopying Barebox from SPI Flash(bootopt=%d)\n", \
					BOOT_LS_SPI);
			copy_bb_from_spi_flash((char*)BAREBOX_LODING_ADDR, count);
			break;
#if !defined(CONFIG_MACH_COMCERTO_C2K_EVM) && !defined(CONFIG_MACH_COMCERTO_C2K_MFCNEVM)
		case BOOT_HS_SPI:
			//With Fast SPI boot, the barebox will also reside in Fast SPI flash
			printf("\nCopying Barebox from Fast SPI Flash(bootopt=%d)\n", \
					BOOT_HS_SPI);
			copy_bb_from_fast_spi_flash((char*)BAREBOX_LODING_ADDR, count);
			break;
#endif
		default:
			/*
			   -With NOR boot the barebox will be loaded from NOR flash.
			   -With I2C boot the barebox will be loaded either from NOR flash or NAND. 
			   If NAND then this part of code won't be compiled as CONFIG_COMCERTO_NAND_ULOADER 
			   will be selected.
			   -With other boot option like UART boot, barebox will be loaded from NOR by default
			 */
			printf("\nCopying Barebox from NOR Flash(bootopt=%d)\n", \
					BOOT_16BIT_NOR);
			memcpy((void *)dst, (void *)src, count);
			break;
	}
#endif

	bb_go((void*)BAREBOX_LODING_ADDR);

	return -1;
}

late_initcall(do_bootb_barebox);

