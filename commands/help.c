/*
 * (C) Copyright 2000-2003
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#include <command.h>

/*
 * Use puts() instead of printf() to avoid printf buffer overflow
 * for long help messages
 */
static int do_help(struct command * cmdtp, int argc, char *argv[])
{
	if (argc == 1) {	/* show list of commands */
		for_each_command(cmdtp) {
			if (!cmdtp->usage)
				continue;
			printf("%10s - %s\n", cmdtp->name, cmdtp->usage);
		}
		return 0;
	}

	/* command help (long version) */
	if ((cmdtp = find_cmd(argv[1])) != NULL) {
		barebox_cmd_usage(cmdtp);
		return 0;
	} else {
		printf ("Unknown command '%s' - try 'help'"
			" without arguments for list of all"
			" known commands\n\n", argv[1]
				);
		return 1;
	}
}

static const __maybe_unused char cmd_help_help[] =
"Show help information (for 'command')\n"
"'help' prints online help for the monitor commands.\n\n"
"Without arguments, it prints a short usage message for all commands.\n\n"
"To get detailed help information for specific commands you can type\n"
"'help' with one or more command names as arguments.\n";

static const char *help_aliases[] = { "?", NULL};

BAREBOX_CMD_START(help)
	.cmd		= do_help,
	.aliases	= help_aliases,
	.usage		= "print online help",
	BAREBOX_CMD_HELP(cmd_help_help)
BAREBOX_CMD_END

