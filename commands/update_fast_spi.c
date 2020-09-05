/*
 * test_spi.c
 *
 */

#include <diags.h>
#include <common.h>
#include <command.h>

extern int update_fast_spi_flash(unsigned char *, ulong, ulong , ulong);

static int do_update_fast_spi(struct command *cmdtp, int argc, char *argv[])
{
        ulong   src, offset, count, sec;

        if (argc != 5) {
                printf ("Usage:\n%s\n", cmdtp->usage);
                return 1;
        }

	printk ("Updating Fast SPI-Flash...\n");

        src = simple_strtoul(argv[1], NULL, 16);

        sec = simple_strtoul(argv[2], NULL, 16);

        offset = simple_strtoul(argv[3], NULL, 16);

        count = simple_strtoul(argv[4], NULL, 16);

        if (count == 0) {
                puts ("Zero length ???\n");
                return -1;
        }

	if(update_fast_spi_flash((unsigned char*)src, sec, offset, count) < 0)
	{
		printf("ERROR: NOR Update FAIL !!\n");
		return -1;
	}

	return 0;
}

BAREBOX_CMD_HELP_START(update_fast_spi)
BAREBOX_CMD_HELP_USAGE("update_fspi\n")
BAREBOX_CMD_HELP_SHORT("Update Fast SPI flash.\n")
BAREBOX_CMD_HELP_END

BAREBOX_CMD_START(update_fast_spi)
        .cmd            = do_update_fast_spi,
        .usage          = "update_fspi <src> <sec> <off> <size>",
	BAREBOX_CMD_HELP(cmd_update_fast_spi_help)
BAREBOX_CMD_END
