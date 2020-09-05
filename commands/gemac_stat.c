/*
 * (C) Copyright 2003
 *  Author : Laurent Brando (Mindspeed Technologies)
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

/**
 * @file
 * @brief Retrieve GEMAC Statistics
 */

#include <common.h>
#include <command.h>
#include <environment.h>
#include <driver.h>
#include <net.h>
#include <fs.h>
#include <errno.h>
#include <libbb.h>
#include <malloc.h>


#define ETH_GSTRING_LEN         32 /* from linux/include/ethtool.h */

static char stat_gstrings[][ETH_GSTRING_LEN] = {	
  	"tx- octets  (Lower 32-bits)",
  	"tx- octets  (Upper 16-bits)",
  	"tx- packets",
  	"tx- broadcast",
  	"tx- multicast",
  	"tx- pause",
  	"tx- 64 bytes packets",
  	"tx- 64 - 127 bytes packets",
  	"tx- 128 - 255 bytes packets",
  	"tx- 256 - 511 bytes packets",
 	"tx- 512 - 1023 bytes packets",
 	"tx- 1024 - 1518 bytes packets",
 	"tx- > 1518 bytes packets",
 	"tx- underruns  - errors",
 	"tx- single collision",
 	"tx- multi collision",
 	"tx- exces. collision  - errors",
 	"tx- late collision  - errors",
 	"tx- deferred",
 	"tx- carrier sense - errors",
  	"rx- octets (Lower 32-bits)",
  	"rx- octets (Upper 16-bits)",
  	"rx- packets",
  	"rx- broadcast",
  	"rx- multicast",
  	"rx- pause",
  	"rx- 64 bytes packets",
  	"rx- 64 - 127 bytes packets",
  	"rx- 128 - 255 bytes packets",
  	"rx- 256 - 511 bytes packets",
 	"rx- 512 - 1023 bytes packets",
 	"rx- 1024 - 1518 bytes packets",
 	"rx- > 1518 bytes packets",
	"rx- undersize -errors",
	"rx- oversize  - errors ",
	"rx- jabbers - errors",
 	"rx- fcs - errors",
 	"rx- length - errors",
 	"rx- symbol - errors",
 	"rx- align - errors",
 	"rx- ressource - errors",
 	"rx- overrun - errors",
 	"rx- IP cksum - errors",
	"rx- TCP cksum - errors",
 	"rx- UDP cksum - errors" 	
};

static int gemac_stats(struct command *cmdtp, int argc, char *argv[])
{
	struct eth_device *edev;
	u32 *pbuf, *pstat = NULL;
	u32 len = 0;
	int i;

	if (argc != 2)
		return COMMAND_ERROR_USAGE;

	edev = eth_get_byname(argv[1]);
	if (edev) 
	{
		len = edev->get_gemac_stats_len(edev);
		pbuf = malloc(len);
		if(pbuf)
		{  
			pstat = pbuf;
			edev->get_gemac_stats(edev, pstat);
			for (i=0; i<(len/4); i++)
				printf("%s %d\n", stat_gstrings[i], *pstat++);
			free(pbuf);
		} 
		else 
		{
			printf("out of memory\n");
			return 1;
		}
	} 
	else 
	{
		printf("no such net device: %s\n", argv[1]);
		return 1;
	}

	return 0;
}

static const __maybe_unused char cmd_gemac_stats_help[] =
"Usage: gemac_stat [ethx]\n";

BAREBOX_CMD_START(gemac_stats)
	.cmd		= gemac_stats,
	.usage		= "retrieve GEMAC statistics",
	BAREBOX_CMD_HELP(cmd_gemac_stats_help)
BAREBOX_CMD_END

