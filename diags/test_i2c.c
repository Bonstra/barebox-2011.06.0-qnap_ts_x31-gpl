
#include "tests.h"
#include <diags.h>
#include <common.h>
#include <asm/io.h>
#include <errno.h>
#include <malloc.h>
#include <i2c/i2c.h>
#include <mach/comcerto-2000.h>

#define DEBUG

uint8_t const I2C_TEST_PTRN_SEED = 0x00;

static void i2c_ddr_incr_pattern(uint8_t *test_address, unsigned long max_size)
{
        unsigned long mem_offset = 0;

       // Fill memory with incremental pattern.
        for (mem_offset = 0; mem_offset < max_size; mem_offset++)
        {
                *(test_address + mem_offset) = I2C_TEST_PTRN_SEED + mem_offset;
        }
}

static int i2c_eeprom_test(struct i2c_client *client, unsigned int count)
{
	u8 *test_ptrn_address;
	u8 *buf;
	int ret, i;

	test_ptrn_address = xmalloc(count);

	i2c_ddr_incr_pattern(test_ptrn_address, count);

	printf("\n#####################Starting I2C read####################\n");

	buf = xmalloc(count);

	ret = i2c_read_reg(client, 0 | I2C_ADDR_16_BIT, buf, count);

	if (ret != count) {
		printf("ERROR: I2C read has failed\n");
		return ret;
	} else {
		printf("Read %i bytes from i2cdev 0x%02x on bus %i, ret = 0x%x\n", count, client->addr, client->adapter->nr, ret);
#ifdef DEBUG
		for (i = 0; i < count; i++)
			printf("0x%02x ", *(buf + i));
		printf("\n");
#endif
	}

	printf("\n#####################Starting I2C write##################\n");

	for (i = 0; i < count + 1; ) {
		printf("#");
		ret = i2c_write_reg(client, i | I2C_ADDR_16_BIT, test_ptrn_address + i, I2C_EEPROM_PAGE_SZ);
		i += I2C_EEPROM_PAGE_SZ;
	}

	printf("\n#####################Starting I2C second read#############\n");

	/* Clear buf contents */
	memset(buf, 0x0, count);

	ret = i2c_read_reg(client, 0 | I2C_ADDR_16_BIT, buf, count);

	if (ret != count) {
		printf("ERROR: I2C read has failed\n");
		return ret;
	} else {
		printf("Read %i bytes from i2cdev 0x%02x on bus %i\n", count, client->addr, client->adapter->nr);
#ifdef DEBUG
		for (i = 0; i < count; i++)
			printf("0x%02x ", *(buf + i));
		printf("\n");
#endif
	}
	ret = memcmp(test_ptrn_address, buf, count);

	if (ret)
		printf("ERROR: Read Mismatch @ 0x%x!!\n", ret);
	else
		printf("I2C data written and read back successfully!!\n");


	free(buf);
	free(test_ptrn_address);

}

/* I2C data read/write test */
int Comcerto_i2c_test (struct diags_test_param *p)
{
	struct i2c_adapter *adapter = NULL;
	struct i2c_client client;
	int bus = 0;
	unsigned int len = I2C_EEPROM_SIZE - 1;

	adapter = i2c_get_adapter(bus);
	if (!adapter) {
		printf("i2c bus %d not found\n", bus);
		return -ENODEV;
	}

	client.adapter = adapter;
	client.addr = CFG_I2C_EEPROM0_ADDR;

#ifdef CONFIG_MACH_COMCERTO_C2K_ASIC
	len /= 2;
#endif
	i2c_eeprom_test(&client, len);

#ifdef CONFIG_MACH_COMCERTO_C2K_ASIC
	client.addr = CFG_I2C_EEPROM1_ADDR;
	i2c_eeprom_test(&client, len);
#endif
	return 0;
}

