/*
 * Legerity driver for Mindspeed Comcerto.
 *
 *  Copyright (C) 2008 Mindspeed Technologies, Inc.
 *  All rights reserved
 */
#include "gpio.h"
#include <mach/comcerto_spi.h>
#include "legerity.h"
#include "legerity_event_hdlr.h"

#define __raw_readl(addr)   (*(volatile unsigned int *)(addr))
#define __raw_writel(b,addr)  ((*(volatile unsigned int *)(addr)) = (b))

#define GFP_KERNEL                    0

#define DEVICE_SPI "legerity0"

struct legerity_data *legerity;
struct legerity_slic_desc *slic1, *slic;
struct spi_adapter comcerto_spi_adapter;

int legerity_init(void);

/* define the module options for slic types */
static int slic_type = 14;

static int flash_time = 500;

/*
 *  legerity_run_tick - performs the API-II tick
*/
void legerity_run_tick (struct legerity_slic_desc * slic)
{
    bool event;

    VpApiTick(&slic->context, &event);
    if (event) {
        /* read out and handle the events */
        VpEventType pEvent;
        while (VpGetEvent(&slic->context, &pEvent)) legerity_event_handler(pEvent);
    }
    return;
}


/*
 *  initialize_slic - initializes a specific slic device
*/
static int initialize_slic (struct legerity_slic_desc *slic)
{
    int i, res;

    if ((res = VpMakeDeviceObject(slic->deviceType, slic->id, &slic->context, slic->object)) != VP_STATUS_SUCCESS) {
        printf ("Failed to make legerity device %d object (%d)\n", slic->id, res);
        return -1;
    } else {
        printf ("VpMakeDeviceObjectsucess to make legerity device %d object (%d)\n", slic->id, res);
    }

    for (i = 0; i < slic->slicMaxLines; i++) {
            res = VpMakeLineObject(slic->termType, i, &slic->line_list[i].context, slic->line_list[i].object, &slic->context);

        if (res != VP_STATUS_SUCCESS) {
            printf ("Failed to make legerity device %d line %d object (%d)\n", slic->id, i, res);
            return -1;
        } else {
            printf ("sucess to make legerity device %d line %d object (%d)\n", slic->id, i, res);
        }
    }

    slic->num_rings=0;

        res= VpInitDevice(&slic->context, DEV_PROFILE_LE88264_ABS_ZSI,
	AC_PROFILE_LE88264_ABS_ZSI, DC_PROFILE_LE88264_ABS_ZSI,
	RING_PROFILE_LE88264_ABS_ZSI, VP_PTABLE_NULL, VP_PTABLE_NULL);
        if (res != VP_STATUS_SUCCESS) {
            printf("FXS slicId %i VpInitDevice call failed condition:%d\n", slic->id, res);
            return -1;
        } else {
            printf("FXS slicId %i VpInitDevice success \n", slic->id);
        }
    return 0;
}


/*
 *  fill_slic_params - initializes the legerity driver
*/
static void fill_slic_params(struct legerity_slic_desc *slic, int cs_no)
{
	int i;
	unsigned long nDelay1, nDelay2;
	uint32 spiClk_kHz = LEGERITY_SPI_SC_RATE / 1000;
	uint32 pClk_kHz = 4096;

	/* initialize structure */
	slic->id = cs_no;
	slic->type = slic_type;

        slic->deviceType = VP_DEV_880_SERIES;
        slic->slicMaxLines = SLIC_MAX_LINES_LE88264_ABS_ZSI;
        slic->termType = VP_TERM_FXS_GENERIC;
        slic->object = (void *) MALLOC(sizeof (Vp880DeviceObjectType));
        slic->line_list = (struct legerity_slic_line_desc *) MALLOC
	(sizeof(struct legerity_slic_line_desc) * slic->slicMaxLines);

        for (i=0; i < slic->slicMaxLines; i++)
		slic->line_list[i].object = (void *) MALLOC(sizeof (Vp880LineObjectType));

        /* determine slic profiles */
        slic->deviceProfile = DEV_PROFILE_LE88264_ABS_ZSI;
        slic->deviceProfileLt = NULL;
        slic->acProfile = AC_PROFILE_LE88264_ABS_ZSI;
        slic->dcProfile = DC_PROFILE_LE88264_ABS_ZSI;
        slic->ringProfile = RING_PROFILE_LE88264_ABS_ZSI;
        slic->ringCadence = RING_CAD_LE88264_ABS_ZSI;

/*
 * ZSI devices have a special chip select off time requirement
 * that is based on the SPI clock and the PCM clock.
 */
        /* make sure we dont divide by 0 */
        spiClk_kHz = (spiClk_kHz < 0) ? 1 : spiClk_kHz;
        pClk_kHz = (pClk_kHz < 0) ? 1 : pClk_kHz;

        nDelay1 = 2500 + (8000000 / pClk_kHz) - (7000000 / spiClk_kHz);
        nDelay2 = (11000000/pClk_kHz) + 122;

        /* cant delay by a negative value */
        nDelay1 = (nDelay1 < 0) ? 0 : nDelay1;
        nDelay2 = (nDelay2 < 0) ? 0 : nDelay2;

    return;
}


/*
 *  detect_slic - tries to detect and initialize slics
*/
static int detect_slic(struct spi_adapter *adapter, int cs_no)
{
    struct device_d *client = get_device_by_name(DEVICE_SPI);
    int i;

    /* initialize the slic structure */
    legerity->slic_list = (void *) MALLOC(sizeof (struct legerity_slic_desc));
    slic = legerity->slic_list;
    slic->spi_client = client;
    fill_slic_params(slic, cs_no);

    /* initialize the slic itself */
    if (!initialize_slic (slic))
        printk ("Detected slic type %d\n", slic->type);
    else
	goto err0;

  slic1 = slic;
  printf("-----------------------------------------------------\n");
  printf("Do On/Off Hook operations on the attached POTS phone:\n");
  printf("Press <Control + c> to quit.\n");
  while(1)
  {
    if (ctrlc())
    {
        printf("Exiting from POTS testing\n");
        break;
    }
	legerity_run_tick(slic);
  }
  printf("-----------------------------------------------------\n");
  return 0;

err0:
    for (i=0; i<slic->slicMaxLines; i++)
        FREE(slic->line_list[i].object);
    FREE(slic->line_list);
    FREE(slic->object);
    FREE(slic);
    legerity->slic_list = NULL;
    return -1;
}

/*
 *  legerity_attach_adapter - initializes the legerity driver
*/
int legerity_attach_adapter(struct spi_adapter *adapter)
{
        legerity->tdm_coding = 1;

    legerity->flash_time= flash_time;

    if(detect_slic(adapter, 3))
	return -1;

    legerity->running_timer = 1;

    return 0;
}

/**
 * legerity_init -
 */
static int is_legerity_init = 0;

int legerity_init(void)
{
        if(is_legerity_init)
        {
                printf("SLIC already initialized\n");
		printf("-----------------------------------------------------\n");
		printf("Do On/Off Hook operations on the attached POTS phone:\n");
		printf("Press <Control + c> to quit.\n");
		while(1)
		{
			if (ctrlc())
			{
				printf("Exiting from POTS testing\n");
				break;
			}
			legerity_run_tick(slic1);
		}
		printf("-----------------------------------------------------\n");
                return 0;
        }
        is_legerity_init = 1;
        printf("Initializing SLIC...\n");

	/* allocate legerity_data structure */
	legerity = (void *) MALLOC(sizeof (struct legerity_data));
	if (legerity == NULL)
	{
		printf ("Unable to allocate memory for legerity driver\n");
		return -1;
	}
	memset(legerity, 0, sizeof(struct legerity_data));

        if(legerity_attach_adapter(&comcerto_spi_adapter))
		return -1;

        return 0;
}

