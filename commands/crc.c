/*
 * crc.c - Calculate a crc32 checksum of a memory area
 *
 * Copyright (c) 2007 Sascha Hauer <s.hauer@pengutronix.de>, Pengutronix
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <common.h>
#include <command.h>
#include <fs.h>
#include <getopt.h>
#include <fcntl.h>
#include <errno.h>
#include <xfuncs.h>
#include <malloc.h>
#include <linux/ctype.h>

static int file_crc(char* filename, ulong start, ulong size, ulong *crc,
		    ulong *total)
{
	int fd, now;
	int ret = 0;
	char *buf;

	*total = 0;
	*crc = 0;

	fd = open(filename, O_RDONLY);
	if (fd < 0) {
		printf("open %s: %s\n", filename, errno_str());
		return fd;
	}

	if (start > 0) {
		ret = lseek(fd, start, SEEK_SET);
		if (ret == -1) {
			perror("lseek");
			goto out;
		}
	}

	buf = xmalloc(4096);

	while (size) {
		now = min((ulong)4096, size);
		now = read(fd, buf, now);
		if (now < 0) {
			ret = now;
			perror("read");
			goto out_free;
		}
		if (!now)
			break;
		*crc = crc32(*crc, buf, now);
		size -= now;
		*total += now;
	}

	printf ("CRC32 for %s 0x%08lx ... 0x%08lx ==> 0x%08lx",
			filename, start, start + *total - 1, *crc);

out_free:
	free(buf);
out:
	close(fd);

	return ret;
}

static int do_crc(struct command *cmdtp, int argc, char *argv[])
{
	ulong start = 0, size = ~0, total = 0;
	ulong crc = 0, vcrc = 0;
	char *filename = "/dev/mem";
#ifdef CONFIG_CMD_CRC_CMP
	char *vfilename = NULL;
#endif
	int opt, err = 0, filegiven = 0, verify = 0;

	while((opt = getopt(argc, argv, "f:F:v:")) > 0) {
		switch(opt) {
		case 'f':
			filename = optarg;
			filegiven = 1;
			break;
#ifdef CONFIG_CMD_CRC_CMP
		case 'F':
			verify = 1;
			vfilename = optarg;
			break;
#endif
		case 'v':
			verify = 1;
			vcrc = simple_strtoul(optarg, NULL, 0);
			break;
		}
	}

	if (!filegiven && optind == argc)
		return COMMAND_ERROR_USAGE;

	if (optind < argc) {
		if (parse_area_spec(argv[optind], &start, &size)) {
			printf("could not parse area description: %s\n", argv[optind]);
			return 1;
		}
	}

	if (file_crc(filename, start, size, &crc, &total) < 0)
		return 1;

#ifdef CONFIG_CMD_CRC_CMP
	if (vfilename) {
		size = total;
		puts("\n");
		if (file_crc(vfilename, start, size, &vcrc, &total) < 0)
			return 1;
	}
#endif

	if (verify && crc != vcrc) {
		printf(" != 0x%08lx ** ERROR **", vcrc);
		err = 1;
	}

	printf("\n");

	return err;
}

BAREBOX_CMD_HELP_START(crc)
BAREBOX_CMD_HELP_USAGE("crc32 [OPTION] [AREA]\n")
BAREBOX_CMD_HELP_SHORT("Calculate a crc32 checksum of a memory area.\n")
BAREBOX_CMD_HELP_OPT  ("-f <file>", "Use file instead of memory.\n")
#ifdef CONFIG_CMD_CRC_CMP
BAREBOX_CMD_HELP_OPT  ("-F <file>", "Use file to compare.\n")
#endif
BAREBOX_CMD_HELP_OPT  ("-v <crc>",  "Verify\n")
BAREBOX_CMD_HELP_END

BAREBOX_CMD_START(crc32)
	.cmd		= do_crc,
	.usage		= "crc32 checksum calculation",
	BAREBOX_CMD_HELP(cmd_crc_help)
BAREBOX_CMD_END
