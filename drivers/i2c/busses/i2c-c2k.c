#include <config.h>
#include <init.h>
#include <common.h>
#include <malloc.h>
#include <i2c/i2c.h>
#include <asm/io.h>
#include <mach/gpio.h>
#include <mach/comcerto-2000.h>
#include <mach/i2c.h>

/* Assuming that there is only one master on the bus (us) */

void i2c_init (struct device_d *pdev, int speed, int slaveaddr)
{
	unsigned int n, m, freq, margin, power;
	unsigned int actualN = 0, actualM = 0;
	unsigned int minMargin = 0xffffffff;
	unsigned int control, status;
	unsigned int tclk = CFG_TCLK;
	unsigned int i2cFreq = speed;	/* 100000 max. Fast mode not supported */

	dev_dbg(pdev, "i2c_init\n");

	for (n = 0; n < 8; n++) {
		for (m = 0; m < 16; m++) {
			power = 1 << n;	/* power = 2^(n) */
			freq = tclk / (10 * (m + 1) * power);
			if (i2cFreq > freq)
				margin = i2cFreq - freq;
			else
				margin = freq - i2cFreq;
			if (margin < minMargin) {
				minMargin = margin;
				actualN = n;
				actualM = m;
			}
		}
	}

	dev_dbg(pdev, "setup i2c bus\n");

	/* Setup bus */
        writel(0, pdev->map_base + I2C_SOFT_RESET);

	dev_dbg(pdev, "udelay...\n");

	udelay (I2C_DELAY);

	dev_dbg(pdev, "set baudrate\n");

	writel((actualM << 3) | actualN, pdev->map_base + I2C_STAT);
	writel(I2C_AAK | I2C_ENAB, pdev->map_base + I2C_CNTR);

	udelay (I2C_DELAY * 10);

	dev_dbg(pdev, "read control, baudrate\n");

	status = readl(pdev->map_base + I2C_STAT);
	control = readl(pdev->map_base + I2C_CNTR);
}

static uchar i2c_start (struct device_d *pdev)
{
	unsigned int control, status;
	int count = 0;

	/* Set the start bit */
	control = readl(pdev->map_base + I2C_CNTR);
	control |= I2C_STA;	/* generate the I2C_START_BIT */
	writel(control, pdev->map_base + I2C_CNTR);

	status = readl(pdev->map_base + I2C_STAT);

	count = 0;
	while ((status & 0xff) != I2C_START_TRANSMIT) {
		udelay (I2C_DELAY);
		if (count > 20) {
			writel(I2C_STP, pdev->map_base + I2C_CNTR);	/*stop */
			dev_dbg(pdev, "i2c_start status timeout: 0x%x\n", status);
			return (status);
		}
		status = readl(pdev->map_base + I2C_STAT);
		dev_dbg(pdev, "i2c_start status 0x%x\n", status);
		count++;
	}

	return (0);
}

static uchar i2c_select_device (struct device_d *pdev, uchar dev_addr, uchar read, int ten_bit)
{
	unsigned int status, data, bits = 7;
	int count = 0;

	/* Output slave address */
	if (ten_bit) {
		bits = 10;
	}

	data = (dev_addr << 1);
	/* set the read bit */
	data |= read;
	writel(data, pdev->map_base + I2C_DATA);
	/* assert the address */
	writel(readl(pdev->map_base + I2C_CNTR) & ~I2C_IFLG, pdev->map_base + I2C_CNTR);

	udelay (I2C_DELAY);

	status = readl(pdev->map_base + I2C_STAT);
	count = 0;
	while (((status & 0xff) != I2C_ADDRESS_R_ACK) && ((status & 0xff) != I2C_ADDRESS_W_ACK)) {
		udelay (I2C_DELAY);
		if (count > 20) {
			writel(I2C_STP, pdev->map_base + I2C_CNTR);	/*stop */
			dev_dbg(pdev, "i2c_select_device timeout status 0x%x\n",status);
			return (status);
		}
		status = readl(pdev->map_base + I2C_STAT);
		dev_dbg(pdev, "i2c_select_device status 0x%x\n",status);
		count++;
	}

	if (bits == 10) {
		dev_dbg(pdev, "10 bit I2C addressing not yet implemented\n");
		return (0xff);
	}

	return (0);
}

static uchar i2c_get_data (struct device_d *pdev, uchar *return_data, int len)
{

	unsigned int data, status = 0;
	int count = 0;
//Patch by QNAP: mark message
//	printf("\n*\n");
	while (len) {
//Patch by QNAP: mark message
//		if (!(len % 128))
//			printf("#");
		/* Get and return the data */
		writel(readl(pdev->map_base + I2C_CNTR) & ~I2C_IFLG, pdev->map_base + I2C_CNTR);

		udelay (I2C_DELAY * 5);

		status = readl(pdev->map_base + I2C_STAT);
		dev_dbg(pdev, "i2c_get_data len %d status 0x%x\n", len, status);
		count++;
		while ((status & 0xff) != I2C_DATA_RECEIVE_ACK) {
			udelay (I2C_DELAY * 10000);
			if (count > 40) {
				writel(I2C_STP, pdev->map_base + I2C_CNTR);	/*stop */
				dev_dbg(pdev, "i2c_get_data timeout status 0x%x\n", status);
				return 0;
			}
			status = readl(pdev->map_base + I2C_STAT);
			dev_dbg(pdev, "i2c_get_data status 0x%x\n", status);
			count++;
		}
		data = readl(pdev->map_base + I2C_DATA);
		len--;
		*return_data = (uchar) data;
		return_data++;
		dev_dbg(pdev, "i2c_get_data data 0x%x\n",data);
	}
//Patch by QNAP: mark message
//	printf("\n*\n");
	writel(readl(pdev->map_base + I2C_CNTR) & ~(I2C_AAK | I2C_IFLG), pdev->map_base + I2C_CNTR);
	while ((status & 0xff) != I2C_DATA_RECEIVE_NACK) {
		udelay (I2C_DELAY);
		if (count > 200) {
			writel(I2C_STP, pdev->map_base + I2C_CNTR);	/*stop */
			return (status);
		}
		status = readl(pdev->map_base + I2C_STAT);
		count++;
		dev_dbg(pdev, "i2c_get_data I2C_AAK status 0x%x\n", status);
	}
	writel(I2C_STP, pdev->map_base + I2C_CNTR);	/* stop */

	return (0);
}

static uchar i2c_write_data (struct device_d *pdev, unsigned int *data, int len)
{
	unsigned int status;
	int count = 0;
	unsigned int temp;
	unsigned int *temp_ptr = data;

	while (len) {
		temp = (unsigned int) (*temp_ptr);
		writel(temp, pdev->map_base + I2C_DATA);
		writel(readl(pdev->map_base + I2C_CNTR) & ~I2C_IFLG, pdev->map_base + I2C_CNTR);

		udelay (I2C_DELAY);

		status = readl(pdev->map_base + I2C_STAT);
		count++;
		while ((status & 0xff) != I2C_DATA_TRANSMIT_ACK) {
			udelay (I2C_DELAY * 2000);
			if (count > 20) {
				writel(I2C_STP, pdev->map_base + I2C_CNTR);	/*stop */
				dev_dbg(pdev, "i2c_write_data timeout  status 0x%x\n",status);
				return (status);
			}
			status = readl(pdev->map_base + I2C_STAT);
			dev_dbg(pdev, "i2c_write_data  status 0x%x\n",status);
			count++;
		}
		len--;
		temp_ptr++;
	}

/* Can't have the write issuing a stop command */
/* it's wrong to have a stop bit in read stream or write stream */
/* since we don't know if it's really the end of the command */
/* or whether we have just send the device address + offset */
/* we will push issuing the stop command off to the original */
/* calling function */
	/* set the interrupt bit in the control register */
	writel(I2C_IFLG, pdev->map_base + I2C_CNTR);
	udelay (I2C_DELAY * 10);
	return (0);
}

/* created this function to get the i2c_write() */
/* function working properly. */
/* function to write bytes out on the i2c bus */
/* this is identical to the function i2c_write_data() */
/* except that it requires a buffer that is an */
/* unsigned character array.  You can't use */
/* i2c_write_data() to send an array of unsigned characters */
/* since the byte of interest ends up on the wrong end of the bus */
/* aah, the joys of big endian versus little endian! */
/* */
/* returns 0 = success */
/*         anything other than zero is failure */
static uchar i2c_write_byte (struct device_d *pdev, unsigned char *data, int len)
{
	unsigned int status;
	int count = 0;
	unsigned int temp;
	unsigned char *temp_ptr = data;

	dev_dbg(pdev, "i2c_write_byte  data 0x%x\n", *temp_ptr);

	while (len) {
//Patch by QNAP: mark message
//		if (!(len % 128))
//			printf("#");
		/* Set and assert the data */
		temp = *temp_ptr;
		writel(temp, pdev->map_base + I2C_DATA);
		writel(readl(pdev->map_base + I2C_CNTR) & ~I2C_IFLG, pdev->map_base + I2C_CNTR);

		udelay (I2C_DELAY*2);

		status = readl(pdev->map_base + I2C_STAT);
		count++;
		while ((status & 0xff) != I2C_DATA_TRANSMIT_ACK) {
			udelay (I2C_DELAY*2000);
			if (count > 20) {
				writel(I2C_STP, pdev->map_base + I2C_CNTR);	/*stop */
				return (status);
			}
			status = readl(pdev->map_base + I2C_STAT);
			count++;
			dev_dbg(pdev, "i2c_write_byte  status 0x%x\n",status);
		}
		len--;
		temp_ptr++;
	}
/* Can't have the write issuing a stop command */
/* it's wrong to have a stop bit in read stream or write stream */
/* since we don't know if it's really the end of the command */
/* or whether we have just send the device address + offset */
/* we will push issuing the stop command off to the original */
/* calling function */
	/* set the interrupt bit in the control register */
	writel(I2C_IFLG, pdev->map_base + I2C_CNTR);
	udelay (I2C_DELAY * 10);

	return (0);
}

static uchar i2c_set_dev_offset (struct device_d *pdev, uchar dev_addr, struct i2c_msg *msgs, int ten_bit, int alen)
{
	uchar status;
	unsigned int table[2];

/* initialize the table of address offset bytes */
/* utilized for 2 byte address offsets */
/* NOTE: the order is high byte first! */
	if (alen == 2) {
		table[1] = msgs->buf[1];	/* low byte */
	}
		table[0] = msgs->buf[0];	/* single byte */


	status = i2c_select_device (pdev, dev_addr, 0, ten_bit);
	if (status) {
		dev_dbg(pdev, "Failed to select device setting offset: 0x%02x\n", status);
		return status;
	}
/* check the address offset length */
	if (alen == 0)
		/* no address offset */
		return (0);
	else if (alen == 1) {
		/* 1 byte address offset */
		status = i2c_write_data (pdev, table, 1);
		if (status) {
			dev_dbg(pdev, "Failed to write data: 0x%02x\n", status);
			return status;
		}
	} else if (alen == 2) {
		/* 2 bytes address offset */
		status = i2c_write_data (pdev, table, 2);
		if (status) {
			dev_dbg(pdev, "Failed to write data: 0x%02x\n", status);
			return status;
		}
	} else {
		/* address offset unknown or not supported */
		dev_dbg(pdev, "Address length offset %d is not supported\n", alen);
		return 1;
	}
	return 0;		/* sucessful completion */
}

uchar i2c_read (struct device_d *pdev, uchar dev_addr, struct i2c_msg *msgs, int alen, uchar * data, int len)
{
	uchar status = 0;
	struct i2c_platform_data *pdata;
	int i2cFreq;

	pdata = pdev->platform_data;
	if (pdata && pdata->bitrate)
		i2cFreq = pdata->bitrate;

	i2c_init (pdev, i2cFreq, 0);	/* set the i2c frequency */

	status = i2c_start (pdev);

	if (status) {
		dev_dbg(pdev, "Transaction start failed: 0x%02x\n", status);
		return status;
	}

	status = i2c_set_dev_offset (pdev, dev_addr, msgs, 0, alen);	/* send the slave address + offset */
	if (status) {
		dev_dbg(pdev, "Failed to set slave address & offset: 0x%02x\n", status);
		return status;
	}

	i2c_init (pdev, i2cFreq, 0);	/* set the i2c frequency again */

	status = i2c_start (pdev);
	if (status) {
		dev_dbg(pdev, "Transaction restart failed: 0x%02x\n", status);
		return status;
	}

	status = i2c_select_device (pdev, dev_addr, 1, 0);	/* send the slave address */
	if (status) {
		dev_dbg(pdev, "Address not acknowledged: 0x%02x\n", status);
		return status;
	}

	status = i2c_get_data (pdev, data, len);
	if (status) {
		dev_dbg(pdev, "Data not recieved: 0x%02x\n", status);
		return status;
	}

	return 0;
}

/* Function to set the I2C stop bit */
void i2c_stop (struct device_d *pdev)
{
	writel((0x1 << 4), pdev->map_base + I2C_CNTR);
}

/* I2C write function */
/* dev_addr = device address */
/* offset = address offset */
/* alen = length in bytes of the address offset */
/* data = pointer to buffer to read data into */
/* len = # of bytes to read */
/* */
/* returns 0 = succesful */
/*         anything but zero is failure */
uchar i2c_write (struct device_d *pdev, uchar dev_addr, struct i2c_msg *msgs, int alen, uchar * data, int len)
{
	uchar status = 0;
        struct i2c_platform_data *pdata;
	int i2cFreq;

	pdata = pdev->platform_data;
	if (pdata && pdata->bitrate)
		i2cFreq = pdata->bitrate;

	i2c_init (pdev, i2cFreq, 0);	/* set the i2c frequency */

	status = i2c_start (pdev);	/* send a start bit */

	if (status) {
		dev_dbg(pdev, "Transaction start failed: 0x%02x\n", status);
		return status;
	}

	status = i2c_set_dev_offset (pdev, dev_addr, msgs, 0, alen);	/* send the slave address + offset */
	if (status) {
		dev_dbg(pdev, "Failed to set slave address & offset: 0x%02x\n", status);
		return status;
	}

	data += alen;

	status = i2c_write_byte (pdev, data, len - alen);	/* write the data */
	if (status) {
		dev_dbg(pdev, "Data not written: 0x%02x\n", status);
		return status;
	}
	/* issue a stop bit */
	i2c_stop (pdev);
	return 0;
}

static int i2c_c2k_xfer(struct i2c_adapter *adapter, struct i2c_msg *msgs, int num)
{
        unsigned int i;
        int result;

        /* Start I2C transfer */
        result = i2c_start(adapter->dev);
        if (result)
		goto fail0;

        /* read/write data */
	i = num-1;
        if (msgs[i].flags & I2C_M_RD)
		result = i2c_read(adapter->dev, msgs[i].addr, msgs, 2, msgs[i].buf, msgs[i].len);
        else
	        result = i2c_write(adapter->dev, msgs->addr, msgs, 2, msgs->buf, msgs->len);

	if (result)
                goto fail0;

fail0:
        /* Stop I2C transfer */
	i2c_stop(adapter->dev);

	return (result < 0) ? result : num;
}

/* function to determine if an I2C device is present */
/* chip = device address of chip to check for */
/* */
/* returns 0 = sucessful, the device exists */
/*         anything other than zero is failure, no device */
static int i2c_probe (struct device_d *pdev)
{
	uchar status = 0;
	struct i2c_adapter *adapter;

	dev_dbg(pdev, "i2c_probe\n");

	adapter = kzalloc(sizeof(struct i2c_adapter), GFP_KERNEL);

	adapter->master_xfer = i2c_c2k_xfer;
	adapter->nr = pdev->id;
	adapter->dev = pdev;

	/* Add I2C adapter */
	status = i2c_add_numbered_adapter(adapter);
	if (status < 0)
	{
		dev_err(pdev, "registration failed\n");
		kfree(adapter);
		return status;
	}

	return 0;		/* successful completion */
}

static struct driver_d c2k_i2c_driver = {
        .name    = "c2k_i2c",
        .probe   = i2c_probe,
};

static int c2k_i2c_driverinit(void)
{
        return register_driver(&c2k_i2c_driver);
}

device_initcall(c2k_i2c_driverinit);

