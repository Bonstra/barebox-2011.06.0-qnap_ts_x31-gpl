#include <common.h>
#include <driver.h>
#include <init.h>
#include <asm/io.h>
#include <mach/hardware.h>
#include <mach/otp.h>

void write_protect_unlock(void)
{
	writel(0xEBCF0000, OTP_CONFIG_LOCK_0);  /* config lock0 */
	writel(0xEBCF1111, OTP_CONFIG_LOCK_1); /* config lock1 */
	writel(0x0, OTP_CEB_INPUT);
}

/*
 ****************************************
 *   otp_write()
 *
 * PARAMETERS:
 *	s_addr -- Starting Address
 *	prog_data -- Source Data Buffer
 ****************************************
 */
void otp_write(u32 offset, u8 *prog_data, int size)
{
	int i;
	u32 read_data = 0, read_done = 0, exp_data = 0;

	if (NULL == prog_data)
		return RETCODE_ERROR;

	if (size <= 0)
		return RETCODE_ERROR;

	/* program the counters */
	writel(0x01900190, OTP_PGM2CPUMP_COUNTER);
	writel(0x04b004b0, OTP_CPUMP2WEB_COUNTER);
	writel(0x07d007d0, OTP_WEB_COUNTER);
	writel(0x03200320, OTP_WEB2CPUMP_COUNTER);
	writel(0x01900190, OTP_CPUMP2PGM_COUNTER);
	writel(0x1c, OTP_DATA_OUT_COUNTER);

	write_protect_unlock();

	/* rstb drive 0 */
	writel(0x0, OTP_RSTB_INPUT);
	/* rstb drive 1 to have pulse  */
	writel(0x1, OTP_RSTB_INPUT);
	udelay(OTP_DELAY);

	for(i = 0 ; i < size ; i++)
	{
		/* Drive the address now */
		writel(offset + i, OTP_ADDR_INPUT);

		/* Write data to the DATA_IN register */
		writel(prog_data[i], OTP_DATA_INPUT);

		/* DLE drive  "1" */
		writel(0x1, OTP_DLE_INPUT);
		/* WEB drive  "0" */
		writel(0x0, OTP_WEB_INPUT);
		/* WEB drive  "1" */
		writel(0x1, OTP_WEB_INPUT);
		/* DLE drive  "0" */
		writel(0x0, OTP_DLE_INPUT);

/* Write '1' to PGMEN to trigger the whole write and verify operation until PGMEN will be deasserted by HW */
		writel(0x1, OTP_PGMEN_INPUT);
		udelay(OTP_DELAY);
	}
}
EXPORT_SYMBOL(otp_program_verify);

/*
 ****************************************
 *   otp_read ()
 *
 * PARAMETERS:
 *	s_addr --Starting Address
 *	read_data --Destination Buffer
 *	size -- No of Bytes to read
 ****************************************
 */
int otp_read(u32 offset, u8 *read_data, int size)
{
	int i;
	u32 read_tmp = 0;

	if (NULL == read_data)
		return RETCODE_ERROR;

	if (size <= 0)
		return RETCODE_ERROR;

	/* configure the OTP_DATA_OUT_COUNTER for read operation.
	    70 nsec is needed except for blank check test, in which 1.5 usec is needed.*/
	writel(DOUT_COUNTER_VALUE, OTP_DATA_OUT_COUNTER);

	write_protect_unlock();

	/* rstb drive 0 */
	writel(0x0, OTP_RSTB_INPUT);
	/* Wait for at least 20nsec */
	udelay(OTP_DELAY);
	/* rstb drive 1 to have pulse  */
	writel(0x1, OTP_RSTB_INPUT);
	/* Wait for at least 1usec */
	udelay(OTP_DELAY);

	/* Write the desired address to the ADDR register */
	writel(offset, OTP_ADDR_INPUT);
	/* read_enable drive */
	writel(0x1, OTP_READEN_INPUT);
	/* Wait for at least 70nsec/1.5usec depends on operation type */
	udelay(OTP_DELAY);

	/* Read First Byte */
	read_tmp = readl(OTP_DATA_OUTPUT);
	*read_data = read_tmp & 0xFF;

	/* For consecutive read */
	for(i = 1 ; i < size ; i++)
	{
		offset = offset + 8;

		/* start reading from data out register */
		writel(offset, OTP_ADDR_INPUT);
		/* Wait for at least 70nsec/1.5usec depends on operation type */
		udelay(OTP_DELAY);

		read_tmp = readl(OTP_DATA_OUTPUT);
		*(read_data + i) = read_tmp & 0xFF;
	}

	/* reading is done make the read_enable low */
	writel(0x0, OTP_READEN_INPUT);

	/* lock CEB register, return to standby mode */
	writel(0x1, OTP_CEB_INPUT);

	return RETCODE_OK;
}
EXPORT_SYMBOL(otp_read);

static int c2k_otp_probe(struct device_d *pdev)
{
	printf("c2k_otp_probe.\n");
}

struct driver_d c2k_otp_driver = {
        .name    = "c2k_otp",
	.probe	 = c2k_otp_probe,
};

int c2k_otp_init(void)
{
	return register_driver(&c2k_otp_driver);
}

device_initcall(c2k_otp_init);
