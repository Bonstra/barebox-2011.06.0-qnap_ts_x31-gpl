#include <common.h>
#include <command.h>
#include <errno.h>
#include <fs.h>

#define NORDEVNAME "nor0"

int update_nor(ulong src, ulong offset, ulong count)
{
        struct cdev *cdev;
        int err, prot = 0;

        cdev = cdev_by_name(NORDEVNAME);
        printf("Unprotecting %s...\n", cdev->name);
        err = cdev->ops->protect(cdev, count, offset, prot);
        if (err && err != -ENOSYS) {
                printf("Unprotecting failed\n");
		perror("protect");
                return -1;
        }

        err = cdev->ops->erase(cdev, count, offset);
        if (err) {
		perror("erase failed\n");
                perror("erase");
                return -1;
        }

        printf("Writing ...\n");
        err = cdev->ops->write(cdev, (char *)src, count, offset, 0);
        if(err == -1)
        {
		perror("write failed\n");
                perror("write");
                return -1;
        }

        prot = 1;
        printf("Protecting...\n");
        err = cdev->ops->protect(cdev, count, offset, prot);
        if (err && err != -ENOSYS) {
		perror("protect failed\n");
                perror("protect");
                return -1;
        }

        printf("Done");

        return 0;
}

EXPORT_SYMBOL(update_nor);

static int do_update_nor(struct command *cmdtp, int argc, char *argv[])
{
        ulong   src, offset, count;

        if (argc != 4) {
                printf ("Usage:\n%s\n", cmdtp->usage);
                return 1;
        }

        src = simple_strtoul(argv[1], NULL, 16);

        offset = simple_strtoul(argv[2], NULL, 16);

        count = simple_strtoul(argv[3], NULL, 16);

        if (count == 0) {
                puts ("Zero length ???\n");
                return 1;
        }

	if(update_nor(src, offset, count) < 0)
	{
		printf("ERROR: NOR Update FAIL !!\n");
		return -1;
	}

	return 0;
}

static const __maybe_unused char cmd_nor_help[] =
"Usage: update_nor  <ddr src> <nor offset> <count>\n"
"Copy memory at <src> of <count> bytes to <dst>\n";


BAREBOX_CMD_START(update_nor)
        .cmd            = do_update_nor,
        .usage          = "Flash the NOR",
	BAREBOX_CMD_HELP(cmd_nor_help)
BAREBOX_CMD_END

