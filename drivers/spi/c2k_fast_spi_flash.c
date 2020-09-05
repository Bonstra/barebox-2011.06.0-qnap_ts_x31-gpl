/*
 * c2k_fast_spi_flash.c
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

#define DEVICE_FAST_SPI	"s25fl2560"

static struct spi_device *fast_spi;

extern int spi_write(struct spi_device *spi, const void *buf, size_t len);
extern int spi_read(struct spi_device *spi, void *buf, size_t len);
extern int spi_write_then_read(struct spi_device *spi,\
                const void *txbuf, unsigned n_tx,\
                void *rxbuf, unsigned n_rx);
extern int spi_eeprom_read(struct spi_device *spi,\
                const void *txbuf, unsigned n_tx,\
                void *rxbuf, unsigned n_rx);

#define CMD_BUF_SIZE	4

/* S25FL256: wren_cmd: |1B_cmd| */
static uint8_t s25fl256_wren_cmd[CMD_BUF_SIZE] = {
	0x06, 0x00, 0x00, 0x00,
};

/* S25FL256: erase_cmd: |1B_cmd|3B_addr| */
static uint8_t s25fl256_erase_cmd[CMD_BUF_SIZE] = {
	0xd8, 0x00, 0x00, 0x00,
};

/* S25FL256: pp_cmd: |1B_cmd|3B_addr|at_least_1B_data|erite_buf| */
static uint8_t s25fl256_pp_cmd[CMD_BUF_SIZE+FAST_SPI_FLASH_PAGE_SIZE] = {
	0x02, 0x00, 0x00, 0x00,
};

/* S25FL256: rd_cmd: |1B_cmd|3B_addr|dummy_bytes| */
static uint8_t s25fl256_rd_cmd[CMD_BUF_SIZE] = {
	0x03, 0x00, 0x00, 0x00,
};

//#define	FAST_SPI_FLASH_DEBUG
#ifdef	FAST_SPI_FLASH_DEBUG
static void print_buffer(char *buf, int s)
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

static u8 read_rdsr(struct spi_device *spi)
{
        int ret = 0;
        uint8_t s25fl256_rdsr_cmd[4] = {
                0x05, 0xa, 0xb, 0xc
        };

        ret = spi_write_then_read(spi, s25fl256_rdsr_cmd, 2, s25fl256_rdsr_cmd+2, 2);
        if(ret)
        {
                printk ("%s:%d: RDSR failed.\n", __func__, __LINE__);
                return -1;
        }

        return s25fl256_rdsr_cmd[3];
}

/* Erase sector_num sector */
static int nor_s25fl256_sector_erase(struct spi_device *spi, int sector_num)
{
        int sector_addr = sector_num * FAST_SPI_FLASH_SECTOR_SIZE;
	u8 rdsr = 0x1;
        int ret = 0;

        s25fl256_erase_cmd[1] = ((sector_addr & 0xff0000)>>16);
        s25fl256_erase_cmd[2] = ((sector_addr & 0xff00)>>8);
        s25fl256_erase_cmd[3] = (sector_addr & 0xff);

#ifdef FAST_SPI_FLASH_DEBUG
        printk ("%s: Erasing sec %d using (cmd):address (0x%x):0x%x:0x%x:0x%x \n", __func__, sector_num, \
		s25fl256_erase_cmd[0], s25fl256_erase_cmd[1], s25fl256_erase_cmd[2],\
		s25fl256_erase_cmd[3]);
#endif
	do {
		rdsr = read_rdsr(spi);
	}while (rdsr & 0x1);

#ifdef FAST_SPI_FLASH_DEBUG
	printk ("%s: WREN sending...\n", __func__);
#endif
        ret = spi_write(spi, s25fl256_wren_cmd, 1);
        if(ret)
        {
                printk ("%s:%d: WREN failed ret=%d\n", __func__, __LINE__, ret);
                return ret;
        }

	do {
		rdsr = read_rdsr(spi);
	}while (!(rdsr & 0x2));

#ifdef FAST_SPI_FLASH_DEBUG
	printk ("%s: Erase sending...\n", __func__);
#endif
        ret = spi_write(spi, s25fl256_erase_cmd, 4);
        if(ret)
        {
                printk ("%s:%d: ERASE failed.\n", __func__, __LINE__);
                return ret;
        }

	do {
		rdsr = read_rdsr(spi);
	}while (rdsr & 0x1);

#ifdef FAST_SPI_FLASH_DEBUG
	printk ("%s: Erase done...\n", __func__);
#endif
        return ret;
}

static int write_bytes_page_addr(struct spi_device *spi, u8 *b, int num_buytes, u8 p, int a)
{
        int ret = 0;
	u8 rdsr = 0x1;
        int c = 0;

        int addr = (FAST_SPI_FLASH_SECTOR_SIZE * p)+a;
        s25fl256_pp_cmd[1] = ((addr & 0xff0000)>>16);
        s25fl256_pp_cmd[2] = ((addr & 0xff00)>>8);
        s25fl256_pp_cmd[3] = (addr & 0xff);

        while (c < num_buytes)
        {
                s25fl256_pp_cmd[4+c] = b[c];
                c++;
        }

#ifdef FAST_SPI_FLASH_DEBUG
	printk ("%s: Writing %d bytes at (cmd):addr (0x%x):0x%x:0x%x:0x%x 0x%x\n", __func__, num_buytes,\
                s25fl256_pp_cmd[0], s25fl256_pp_cmd[1], s25fl256_pp_cmd[2], s25fl256_pp_cmd[3], s25fl256_pp_cmd[4]);
#endif

	do {
		rdsr = read_rdsr(spi);
	}while (rdsr & 0x1);

        ret = spi_write(spi, s25fl256_wren_cmd, 1);
        if(ret)
        {
                printk ("%s:%d: WREN failed.\n", __func__, __LINE__);
                return ret;
        }

	do {
		rdsr = read_rdsr(spi);
	}while (!(rdsr & 0x2));

        ret = spi_write(spi, s25fl256_pp_cmd, 4+num_buytes);
        if(ret)
        {
                printk ("%s:%d: PageProgram failed.\n", __func__, __LINE__);
                return ret;
        }

	do {
		rdsr = read_rdsr(spi);
	}while (rdsr & 0x1);

        return ret;
}

#define	WRITE_CHUNKS	128

static int write_one_sector(struct spi_device *spi, u8 *b, int count, u8 s, u8 of)
{
	int cnt = 0;
	int bytes = count; //num bytes to write
	u8 sec = s; //sector
	int off = of; //offset
	u8 *wbuf = b; //buf to use
	int chunks = WRITE_CHUNKS;
	int ret = 0;

	cnt = bytes/chunks;

	if(count%chunks)
		cnt += 1;

	if (cnt == 1)
		chunks = bytes;

	printk("\nWriting %d bytes at sec %d and offset %d\n", count, sec, off);

	while(cnt) {
		printk(".");

		ret = write_bytes_page_addr(spi, wbuf, chunks, sec, off);

		off += chunks;
		wbuf += chunks;
		cnt--;

		bytes -= chunks;
		if(bytes <= WRITE_CHUNKS)
			chunks = bytes;
		else
			chunks = WRITE_CHUNKS;
	}

	printk ("\n");

	return ret;
}

static int fast_spi_flash_write(struct spi_device *spi, unsigned char *src, \
			ulong sec, ulong offset, ulong count)
{
	int bytes = count;
	int num_sec;
	int c = sec;
	int ret = 0;
	int off = offset;

	if(bytes < FAST_SPI_FLASH_SECTOR_SIZE){
		num_sec = 1;
	}else{
		num_sec = bytes/FAST_SPI_FLASH_SECTOR_SIZE;
		if(bytes % FAST_SPI_FLASH_SECTOR_SIZE){
			num_sec += 1;
		}
	}

	num_sec += c; //increasing count as per starting sector

	while(c < num_sec){
		ret = nor_s25fl256_sector_erase(spi, c);
		if(ret)
		{
			printk ("%s: nor_s25fl256_sector_erase failed. ret=%d\n", \
						__func__, ret);
			return ret;
		}

		if(count > FAST_SPI_FLASH_SECTOR_SIZE){
			bytes = FAST_SPI_FLASH_SECTOR_SIZE;
		}else{
			bytes = count;
		}

		ret = write_one_sector(spi, src, bytes, c, off);
		if(ret)
		{
			printk ("%s: write_one_sector failed. ret=%d\n", \
						__func__, ret);
			return ret;
		}

		c++;
		src += bytes;
		count -= bytes;//FAST_SPI_FLASH_SECTOR_SIZE;
		off = 0; //start new sector write at offset 0
	}

	return ret;
}

static int read_bytes_page_addr(struct spi_device *spi, u8 *b, int num_buytes, u8 p, int a)
{
	int ret = 0;
	u8 rdsr = 0x1;

	int addr = (FAST_SPI_FLASH_SECTOR_SIZE * p)+a;
	s25fl256_rd_cmd[1] = ((addr & 0xff0000)>>16);
	s25fl256_rd_cmd[2] = ((addr & 0xff00)>>8);
	s25fl256_rd_cmd[3] = (addr & 0xff);

#ifdef FAST_SPI_FLASH_DEBUG
	printk ("%s: Reading %d bytes from (cmd):addr (0x%x):0x%x:0x%x:0x%x \n", __func__, num_buytes,\
	s25fl256_rd_cmd[0], s25fl256_rd_cmd[1], s25fl256_rd_cmd[2], s25fl256_rd_cmd[3]);
#endif
	do {
		rdsr = read_rdsr(spi);
	}while (rdsr & 0x1);

	if (!s25fl256_rd_cmd[0])
		s25fl256_rd_cmd[0] = 0x3;

	ret = spi_write_then_read(spi, s25fl256_rd_cmd, 4+num_buytes, b, 4+num_buytes);
	if(ret)
	{
		printk ("%s:%d: Read failed.\n", __func__, __LINE__);
		return ret;
	}

	memcpy(b, b+4, num_buytes);

	do {
		rdsr = read_rdsr(spi);
	}while (rdsr & 0x1);

	return ret;
}

#define	READ_CHUNKS	128

static int read_one_sector(struct spi_device *spi, u8 *b, int count, u8 s, u8 of)
{
	int cnt = 0;
	int bytes = count; //num bytes to read
	u8 sec = s; //sector
	int off = of; //offset
	u8 *rbuf = b; //buf to use
	int chunks = READ_CHUNKS;
	int ret = 0;

	cnt = bytes/chunks;

	if(count%chunks)
		cnt += 1;

	if (cnt == 1)
		chunks = bytes;

	printk (".");
	while(cnt) {

		ret = read_bytes_page_addr(spi, rbuf, chunks, sec, off);

		off += chunks;
		rbuf += chunks;
		cnt--;

		bytes -= chunks;
		if(bytes <= READ_CHUNKS)
			chunks = bytes;
		else
			chunks = READ_CHUNKS;
	}

	return ret;
}

static int fast_spi_flash_read(struct spi_device *spi, u8 *recbuf, \
		u8 sect, u8 offset, int count)
{
	int bytes = count;
	int num_sec;
	int c = sect;
	int ret = 0;

	if(bytes < FAST_SPI_FLASH_SECTOR_SIZE){
		num_sec = 1;
	}else{
		num_sec = bytes/FAST_SPI_FLASH_SECTOR_SIZE;
		if(bytes % FAST_SPI_FLASH_SECTOR_SIZE){
			num_sec += 1;
		}
	}

	while(c < num_sec){
                if(count > FAST_SPI_FLASH_SECTOR_SIZE){
                        bytes = FAST_SPI_FLASH_SECTOR_SIZE;
                }else{
                        bytes = count;
                }

		ret = read_one_sector(spi, recbuf, bytes, c, offset);
		if(ret)
		{
			printk ("%s: fast_spi_flash_read failed. ret=%d\n", \
					__func__, ret);
			return ret;
		}
		c++;
		recbuf += bytes;
		count -= bytes;
		offset = 0;
	}

	return ret;
}

int fast_spi_copy_read(char *recbuf, unsigned int len, unsigned int sec, unsigned int off)
{
	int ret = 0;
	struct device_d *fast_spi_dev = get_device_by_name(DEVICE_FAST_SPI);

	fast_spi = fast_spi_dev->type_data;

	ret = fast_spi_flash_read(fast_spi, recbuf, sec, off, len);
	if(ret)
	{
		printk ("%s: fast_spi_copy_read failed. ret=%d\n", \
				__func__, ret);
		return ret;
	}

	return ret;
}
EXPORT_SYMBOL(fast_spi_copy_read);

int update_fast_spi_flash(unsigned char *src, ulong sec, ulong off, ulong count)
{
	struct device_d *fast_spi_dev = get_device_by_name(DEVICE_FAST_SPI);

	fast_spi = fast_spi_dev->type_data;

#ifdef FAST_SPI_FLASH_DEBUG
	printk ("%s: \
		\nSector = 0x%x \
		\nOffset = 0x%x \
		\nBytes = %d \
		\nmax_speed_hz=%d \
		\nchip_select=0x%x \
		\nmodalias=%s\n", __func__, \
		(unsigned int)sec, (unsigned int)off, (int)count, fast_spi->max_speed_hz, \
		fast_spi->chip_select, fast_spi->modalias);
#endif

	fast_spi_flash_write(fast_spi, src, sec, off, count);


        return 0;
}
EXPORT_SYMBOL(update_fast_spi_flash);
