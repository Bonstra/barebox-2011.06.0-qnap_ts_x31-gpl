/*
** Copyright © 2008
**
** Author(s):
** Lucy Jordan <lucy.jordan@silabs.com>, James Phillips <james.phillips@mindspeed.com>
**
** This file contains proprietary information.
** No dissemination allowed without prior written permission from
** Silicon Laboratories, Inc.
**
*/
#ifndef _PROSLIC_MSPD_H
#define _PROSLIC_MSPD_H

#include <mach/comcerto_spi.h>
#include "proslic_api/proslic.h"
#include "proslic_constants.h"

#ifdef SI32267
#define CHAN_PER_DEVICE 2
#else
#define CHAN_PER_DEVICE 1
#endif

#define PROSLIC_DRIVER_NAME_STR "si3215"

#define ALAW		1
#define ULAW		2

#define DEFAULT_TDM_CODING   ULAW
#define DEFAULT_FLASH_TIME    500
#define DEFAULT_SPI_CS          3
#define DEFAULT_RXGAIN     0x4000
#define DEFAULT_TXGAIN     0x4000

#define MAX_POTS		4

#define POTS_STATE_ONHOOK	1
#define POTS_STATE_OFFHOOK	2
#define POTS_STATE_RINGING	3
#define POTS_STATE_FLASH        4

#define PROSLIC_DEBUG
#ifdef PROSLIC_DEBUG
	#define PROSLIC_debug(str, args...) printf(str, ## args)
#else
	#define PROSLIC_debug(str, args...)
#endif
	#define PROSLIC_info(str, args...) printf(str, ## args)
	#define PROSLIC_err(str, args...) printf(str, ## args)

struct proslic_pots
{
	char state;
	unsigned long irq_pending;
//        struct timer_list flash_timer;
        unsigned long num_rings;
	/*ProSLIC API data*/
	proslicChanType proslic_chan;
	ProslicDeviceType proslic_device;
};

struct spi_itf
{
//	struct spi_client client;
	struct device_d *spi_client;
//	spinlock_t lock;
	int daisyChain;
};

#define IRQ_BIT_HOOK_IRQ   0
#define IRQ_BIT_CID_IRQ    1
#define IRQ_BIT_CID_TIMER  2

struct proslic_data
{
	struct spi_itf client;
//	struct timer_list timer;
	char max_pots;

	struct proslic_pots pots[MAX_POTS];

//	struct tasklet_struct soft_irq;
//      struct tasklet_struct callerid_softirq;

	struct slic_itf *itf;

	char irq;
	/*ProSLIC API Data*/
	controlInterfaceType proslic_spi;
};

extern struct proslic_data *proslic;
extern struct slic_itf *proslic_itf;

void proslic_handle_state(struct proslic_data *proslic, int pots, int prev_pots_state);



#endif /* _PROSLIC_MSPD_H */
