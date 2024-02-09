/*
 * c2k_flash.c
 *
 */

#include <malloc.h>
#include <diags.h>
#include <spi/spi.h>
#include <mach/comcerto_spi.h>
#include <common.h>
#include <linux/list.h>
#include <linux/types.h>
#include <command.h>
#include <config.h>
#include <libbb.h>

#define DEVICE_FAST_SPI	"S25FL064A0"

#define BUF_SIZE	4

extern int spi_write(struct spi_device *spi, const void *buf, size_t len);
extern int spi_read(struct spi_device *spi, void *buf, size_t len);
extern int spi_write_then_read(struct spi_device *spi,\
                const void *txbuf, unsigned n_tx,\
                void *rxbuf, unsigned n_rx);
extern int spi_eeprom_read(struct spi_device *spi,\
                const void *txbuf, unsigned n_tx,\
                void *rxbuf, unsigned n_rx);

static struct spi_device *ls_spi;
unsigned int erase_glob = 0;

#ifdef	DEBUG
void print_buffer(char *buf, int s)
{
	int i = 0;

	printk ("Buffer: ");
	while(i < s){
		printk ("0x%x:", *buf++);
		i++;
	}
	printk ("\n");
}
#endif

u8 read_rdsr(struct spi_device *spi)
{
        int ret = 0;
        uint8_t s25fl128_rdsr_cmd[4] = {
                0x05, 0xa, 0xb, 0xc
        };

        ret = spi_write_then_read(spi, s25fl128_rdsr_cmd, 2, s25fl128_rdsr_cmd+2, 2);
        if(ret)
        {
                printk ("%s:%d: RDSR failed.\n", __func__, __LINE__);
                return -1;
        }

        return s25fl128_rdsr_cmd[3];
}

#if !defined(CONFIG_COMCERTO_ULOADER) && !defined(CONFIG_COMCERTO_JTAG_ULOADER)
/* Erase sector_num sector */
int nor_s25fl128_sector_erase(struct spi_device *spi, int sector_num)
{
	/* S25FL128: wren_cmd: |1B_cmd| */
	uint8_t s25fl128_wren_cmd[BUF_SIZE] = {
		0x06, 0x00, 0x00, 0x00,
	};
 	/* S25FL128: erase_cmd: |1B_cmd|3B_addr| */
	uint8_t s25fl128_erase_cmd[BUF_SIZE] = {
	 	0xd8, 0x00, 0x00, 0x00,
	};

        int sector_addr = sector_num * SPI_FLASH_SECTOR_SIZE;
        u8 rdsr = 0x1;
        int ret = 0;

        s25fl128_erase_cmd[1] = ((sector_addr & 0xff0000)>>16);
        s25fl128_erase_cmd[2] = ((sector_addr & 0xff00)>>8);
        s25fl128_erase_cmd[3] = (sector_addr & 0xff);

        do {
                rdsr = read_rdsr(spi);
        }while (rdsr & 0x1);

        ret = spi_write(spi, s25fl128_wren_cmd, 1);
        if(ret)
        {
                printk ("%s:WREN failed.\n", __func__);
                return ret;
        }

        do {
                rdsr = read_rdsr(spi);
        }while (!(rdsr & 0x2));

	if (!erase_glob) {
		ret = spi_write(spi, s25fl128_erase_cmd, 4);
		if(ret)
		{
			printk ("%s:ERASE failed.\n", __func__);
			return ret;
		}

		do {
			rdsr = read_rdsr(spi);
		}while (rdsr & 0x1);

		printk ("Erasing Done.\n");

		return ret;
	}else
		return 0;

}

int spi_sector_erase(int sec, int num_sec)
{
	struct device_d *d_fast_spi = get_device_by_name(DEVICE_FAST_SPI);
	int ret = 0;

	num_sec += sec;

	ls_spi = d_fast_spi->type_data;

	while(sec < num_sec){
		printf ("%s: erase sector %d\n", __func__, sec);
		ret = nor_s25fl128_sector_erase(ls_spi, sec++);
		if(ret)
		{
			printk ("%s: failed.\n", __func__);
			return ret;
		}
	}

	return 0;
}
EXPORT_SYMBOL(spi_sector_erase);

#define	WRITE_BYTES	8
int write_bytes_page_addr(struct spi_device *spi, u8 *b, int num_buytes, u8 p, unsigned int a)
{
	/* S25FL128: pp_cmd: |1B_cmd|3B_addr|at_least_1B_data|erite_buf| */
	uint8_t s25fl128_pp_cmd[4+WRITE_BYTES] = {
		0x02, 0x00, 0x00, 0x00,
	};
	/* S25FL128: wren_cmd: |1B_cmd| */
	uint8_t s25fl128_wren_cmd[BUF_SIZE] = {
		0x06, 0x00, 0x00, 0x00,
	};

        int ret = 0;
        u8 rdsr = 0x1;

        int addr = (SPI_FLASH_SECTOR_SIZE * p)+a;
        s25fl128_pp_cmd[1] = ((addr & 0xff0000)>>16);
        s25fl128_pp_cmd[2] = ((addr & 0xff00)>>8);
        s25fl128_pp_cmd[3] = (addr & 0xff);

	memcpy(s25fl128_pp_cmd+4, b, num_buytes);

        do {
                rdsr = read_rdsr(spi);
        }while (rdsr & 0x1);

        ret = spi_write(spi, s25fl128_wren_cmd, 1);
        if(ret)
        {
                printk ("%s:WREN failed.\n", __func__);
                return ret;
        }

        do {
                rdsr = read_rdsr(spi);
        }while (!(rdsr & 0x2));

        ret = spi_write(spi, s25fl128_pp_cmd, 4+num_buytes);
        if(ret)
        {
                printk ("%s:PP failed.\n", __func__);
                return ret;
        }

        do {
                rdsr = read_rdsr(spi);
        }while (rdsr & 0x1);

        return ret;
}
#endif

#define	READ_BYTES	8
int read_bytes_page_addr(struct spi_device *spi, u8 *b, int num_buytes, u8 p, unsigned int a)
{
        int ret = 0;
        u8 rdsr = 0x1;
	/* S25FL128: rd_cmd: |1B_cmd|3B_addr|dummy_bytes| */
	uint8_t s25fl128_rd_cmd[4+READ_BYTES] = {
		0x03, 0x00, 0x00, 0x00,
	};


        int addr = (SPI_FLASH_SECTOR_SIZE * p)+a;
        s25fl128_rd_cmd[1] = ((addr & 0xff0000)>>16);
        s25fl128_rd_cmd[2] = ((addr & 0xff00)>>8);
        s25fl128_rd_cmd[3] = (addr & 0xff);

        do {
                rdsr = read_rdsr(spi);
        }while (rdsr & 0x1);

        ret = spi_write_then_read(spi, s25fl128_rd_cmd, 4+num_buytes, b, 4+num_buytes);
        if(ret)
        {
                printk ("%s:Read failed.\n", __func__);
                return ret;
        }

        do {
                rdsr = read_rdsr(spi);
        }while (rdsr & 0x1);

        return ret;
}

#if !defined(CONFIG_COMCERTO_ULOADER) && !defined(CONFIG_COMCERTO_JTAG_ULOADER)
static int spi_copy_write(char *src, unsigned int len, unsigned int sec, unsigned int off)
{
	unsigned int l=0;
	int ret = 0;
	struct device_d *d_fast_spi = get_device_by_name(DEVICE_FAST_SPI);

	ls_spi = d_fast_spi->type_data;

	ret = nor_s25fl128_sector_erase(ls_spi, sec);
	if(ret)
	{
		printk ("%s: failed.\n", __func__);
		return ret;
	}

	while (l < (len/WRITE_BYTES))
	{
		ret = write_bytes_page_addr(ls_spi, src, WRITE_BYTES, sec, off);
		if(ret)
		{
			printk ("%s: failed.\n", __func__);
			return ret;
		}
		src += WRITE_BYTES;
		off += WRITE_BYTES;
		l++;
	}
	
	if(len%WRITE_BYTES)
	{
		l = len%WRITE_BYTES;

		ret = write_bytes_page_addr(ls_spi, src, l, sec, off);
		if(ret)
		{
			printk ("%s: failed.\n", __func__);
			return ret;
		}
	}

	return 0;
}
#endif

int spi_copy_read(char *dst, unsigned int len, unsigned int sec, unsigned int off)
{
	unsigned int l=0;
	int ret = 0;
	char tmp[4+READ_BYTES];
	struct device_d *d_fast_spi = get_device_by_name(DEVICE_FAST_SPI);

	ls_spi = d_fast_spi->type_data;

	while (l < (len/READ_BYTES))
	{
		ret = read_bytes_page_addr(ls_spi, tmp, READ_BYTES, sec, off);
		if(ret)
		{
			printk ("%s: failed.\n", __func__);
			return ret;
		}

		memcpy(dst, tmp+4, READ_BYTES);

		dst += READ_BYTES;
		off += READ_BYTES;
		l++;
	}
	
	if(len%READ_BYTES)
	{
		l = len%READ_BYTES;

		ret = read_bytes_page_addr(ls_spi, tmp, l, sec, off);
		if(ret)
		{
			printk ("%s: failed.\n", __func__);
			return ret;
		}
		memcpy(dst, tmp+4, l);
		dst += l;
		off += l;
	}

	return 0;
}
EXPORT_SYMBOL(spi_copy_read);

#if !defined(CONFIG_COMCERTO_ULOADER) && !defined(CONFIG_COMCERTO_JTAG_ULOADER)
static void spi_flash_write(unsigned char *src, ulong sec, ulong offset, ulong count)
{
	int wl = 0;
	unsigned int off = 0;
	unsigned int itr = SPI_FLASH_SECTOR_SIZE/SPI_FLASH_NOR_PAGE_SIZE;
	int write_bytes = SPI_FLASH_NOR_PAGE_SIZE;
	unsigned int *ptr_w;
	int s;
	ulong size;

	ptr_w = (unsigned int *)src;
	s = sec;
	size = count;
	off = offset;

	while(size)
	{
		itr = SPI_FLASH_SECTOR_SIZE/SPI_FLASH_NOR_PAGE_SIZE;
		erase_glob=0;

		if(size < SPI_FLASH_SECTOR_SIZE) {
			if(size%write_bytes)
				itr = size/write_bytes + 1;
			else
				itr = size/write_bytes;

			size = 0;
		} else {
			size -= SPI_FLASH_SECTOR_SIZE;
		}

		for (wl = 0 ; wl < itr ; wl++)
		{
			spi_copy_write((char*)ptr_w, write_bytes, s, off);

			erase_glob=1;
			ptr_w += 64;
			off += write_bytes;
		}
		printk ("Wrote %d bytes at sec %d in %d iterations.\n", \
				write_bytes*itr, s, itr);
		s++;
		off = 0;
	}
}

int update_spi_flash(unsigned char *src, ulong sec, ulong off, ulong count)
{
	spi_flash_write(src, sec, off, count);

        return 0;
}

EXPORT_SYMBOL(update_spi_flash);
#endif
