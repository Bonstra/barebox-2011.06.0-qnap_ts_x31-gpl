#include <common.h>
#include <command.h>
#include <errno.h>
#include <fs.h>

#ifdef CONFIG_COMCERTO_NAND_ERASE_FBB
#include <getopt.h>
#include <linux/stat.h>
#include <fcntl.h>
#include <ioctl.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/mtd-abi.h>
#endif /* CONFIG_COMCERTO_NAND_ERASE_FBB */

#define NANDDEVNAME "nand0"

#ifdef CONFIG_COMCERTO_NAND_ERASE_FBB
extern int erase_old_nand_fmt(struct mtd_info *mtd, u8 bb_old_layout);

static int do_erase_old_nand_fmt(struct command *cmdtp, int argc, char *argv[])
{
	struct mtd_info_user user;
	int fd, ret = 0;
	char *filename = NULL;
	struct stat s;
	unsigned long start = 0, size = ~0;

        if (argc != 2) {
                printf ("Usage:\n%s\n", cmdtp->usage);
		return COMMAND_ERROR_USAGE;
        }

	filename = argv[1];

	if (stat(filename, &s)) {
		printf("stat %s: %s\n", filename, errno_str());
		return 1;
	}

	size = s.st_size;

	if (!filename) {
		printf("missing filename\n");
		return 1;
	}

	fd = open(filename, O_RDWR);
	if (fd < 0) {
		printf("open %s: %s", filename, errno_str());
		return 1;
	}

	ret = ioctl(fd, MEMGETINFO, &user);

	/* Change ecc layout to HW BCH-8 */
	erase_old_nand_fmt(user.mtd, 0);

	if (erase(fd, size, start)) {
		perror("erase");
		ret = 1;
	}

	/* Change ecc layout to SW BCH */
	erase_old_nand_fmt(user.mtd, 1);

	if (erase(fd, size, start)) {
		perror("erase");
		ret = 1;
	}

	printf("\t\t\t\t!!Erasure of old NAND format completed!!\n");
	printf("\t\t\t\t!!!! Please reboot the system now !!!!\n");

	return 0;
}

static const __maybe_unused char cmd_erase_old_nand_fmt_help[] =
"Usage: erase_old_nand_fmt <mtddev>\n"
"Example: erase_old_nand_fmt /dev/nand0 \n"
"Erase old NAND format on <mtddev>\n";


BAREBOX_CMD_START(erase_old_nand_fmt)
        .cmd            = do_erase_old_nand_fmt,
        .usage          = "Erase old NAND format",
	BAREBOX_CMD_HELP(cmd_erase_old_nand_fmt_help)
BAREBOX_CMD_END

#endif /* CONFIG_COMCERTO_NAND_ERASE_FBB */

int erase_nand(ulong offset, ulong count)
{
        struct cdev *cdev;
        int err;

        cdev = cdev_by_name(NANDDEVNAME);

        printf("Erasing from offset 0x%lx count 0x%lx ...\n", offset, count);
        err = cdev->ops->erase(cdev, count, offset);
        if (err) {
		perror("erase failed\n");
                perror("erase");
                return -1;
        } else {
		printf("Erase completed\n");
	}

        printf("Done\n");

        return 0;
}

EXPORT_SYMBOL(erase_nand);

static int do_erase_nand(struct command *cmdtp, int argc, char *argv[])
{
        ulong offset, count;

        if (argc != 3) {
                printf ("Usage:\n%s\n", cmdtp->usage);
                return 1;
        }


        offset = simple_strtoul(argv[1], NULL, 16);

        count = simple_strtoul(argv[2], NULL, 16);

        if (count == 0) {
                puts ("Zero length ???\n");
                return 1;
        }

	if(erase_nand(offset, count) < 0)
	{
		printf("ERROR: NAND Erase FAIL !!\n");
		return -1;
	}

	return 0;
}

static const __maybe_unused char cmd_nand_erase_help[] =
"Usage: erase_nand  <nand offset> <count>\n"
"Erase memory at nand offset\n";

BAREBOX_CMD_START(erase_nand)
        .cmd            = do_erase_nand,
        .usage          = "Erase the NAND",
	BAREBOX_CMD_HELP(cmd_nand_erase_help)
BAREBOX_CMD_END

int update_nand(ulong src, ulong offset, ulong count)
{
        struct cdev *cdev;
        int err;

        cdev = cdev_by_name(NANDDEVNAME);

        printf("Erasing from offset 0x%lx count 0x%lx ...\n", offset, count);
        err = cdev->ops->erase(cdev, count, offset);
        if (err) {
		perror("erase failed\n");
                perror("erase");
                return -1;
        } else {
		printf("Erase completed\n");
	}

#if 1
        printf("Writing ...\n");
        err = cdev->ops->write(cdev, (char *)src, count, offset, 0);
        if(err == -1)
        {
		perror("write failed\n");
                perror("write");
                return -1;
        }
#endif
        printf("Done\n");

        return 0;
}

EXPORT_SYMBOL(update_nand);

static int do_update_nand(struct command *cmdtp, int argc, char *argv[])
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

	if(update_nand(src, offset, count) < 0)
	{
		printf("ERROR: NAND Update FAIL !!\n");
		return -1;
	}

	return 0;
}

static const __maybe_unused char cmd_nand_help[] =
"Usage: update_nand  <ddr src> <nand offset> <count>\n"
"Copy memory at <src> of <count> bytes to <dst>\n";


BAREBOX_CMD_START(update_nand)
        .cmd            = do_update_nand,
        .usage          = "Flash the NAND",
	BAREBOX_CMD_HELP(cmd_nand_help)
BAREBOX_CMD_END

int read_nand(ulong src, ulong offset, ulong count)
{
        struct cdev *cdev;
        int err;

        cdev = cdev_by_name(NANDDEVNAME);

        printf("Reading ...\n");
        err = cdev->ops->read(cdev, (char *)src, count, offset, 0);
        if(err < 0)
        {
                perror("read failed %d\n");
                return -1;
        }

        printf("Done...Bytes read 0x%x\n", err);

        return 0;
}

EXPORT_SYMBOL(read_nand);


static int do_read_nand(struct command *cmdtp, int argc, char *argv[])
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

        if(read_nand(src, offset, count) < 0)
        {
                printf("ERROR: NAND Read FAIL !!\n");
                return -1;
        }

        return 0;
}

static const __maybe_unused char cmd_nand_read_help[] =
"Usage: read_nand  <ddr src> <nand offset> <count>\n"
"Read NAND to <src> of <count> bytes from <offset>\n";


BAREBOX_CMD_START(read_nand)
        .cmd            = do_read_nand,
        .usage          = "Read the NAND",
        BAREBOX_CMD_HELP(cmd_nand_read_help)
BAREBOX_CMD_END

