#include <common.h>
#include <i2c/i2c.h>
#include <errno.h>
#include <command.h>

static void i2c_eeprom_write(unsigned char *src, ulong offset, ulong count)
{
	struct i2c_adapter *adapter = NULL;
	struct i2c_client client;
	int i, ret, bus = 0;

	adapter = i2c_get_adapter(bus);
	if (!adapter) {
		printf("i2c bus %d not found\n", bus);
		return -ENODEV;
	}

	client.adapter = adapter;
	client.addr = CFG_I2C_EEPROM0_ADDR;

	for (i = 0; i < count + 1; ) {
		printf("#");
		ret = i2c_write_reg(&client, i | I2C_ADDR_16_BIT, (uint8_t *)src + i, I2C_EEPROM_PAGE_SZ);
		i += I2C_EEPROM_PAGE_SZ;
	}


}
int update_i2c_eeprom(unsigned char *src, ulong off, ulong count)
{
	i2c_eeprom_write(src, off, count);

        return 0;
}

EXPORT_SYMBOL(update_i2c_eeprom);

static int do_update_i2c(struct command *cmdtp, int argc, char *argv[])
{
        ulong   src, offset, count;

        if (argc != 4) {
                printf ("Usage:\n%s\n", cmdtp->usage);
                return 1;
        }

	printk ("Updating I2C EEPROM...\n");

        src = simple_strtoul(argv[1], NULL, 16);

        offset = simple_strtoul(argv[2], NULL, 16);

        count = simple_strtoul(argv[3], NULL, 16);

        if (count == 0) {
                puts ("Zero length ???\n");
                return -1;
        }

	if(update_i2c_eeprom((unsigned char*)src, offset, count) < 0)
	{
		printf("ERROR: I2C EEPROM Update FAIL !!\n");
		return -1;
	}

	return 0;
}

BAREBOX_CMD_HELP_START(update_i2c)
BAREBOX_CMD_HELP_USAGE("update_i2c\n")
BAREBOX_CMD_HELP_SHORT("Update I2C EEPROM .\n")
BAREBOX_CMD_HELP_END

BAREBOX_CMD_START(update_i2c)
        .cmd            = do_update_i2c,
        .usage          = "update_i2c <src> <off> <size>",
	BAREBOX_CMD_HELP(cmd_update_i2c_help)
BAREBOX_CMD_END
