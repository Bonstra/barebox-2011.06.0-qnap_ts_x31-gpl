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
#include <common.h>
#include <config.h>
#include <mach/hardware.h>
#include <malloc.h>
#include <net.h>
#include <command.h>
#include <mach/comcerto_spi.h>

#include "proslic_mspd.h"

static int tdm_coding = DEFAULT_TDM_CODING;

static int txgain = DEFAULT_TXGAIN;

static int rxgain = DEFAULT_RXGAIN;

static int flash_time = DEFAULT_FLASH_TIME;

static int spi_cs = 0x0;//DEFAULT_SPI_CS;

extern struct spi_driver proslic_driver;
struct proslic_data *proslic;

extern int time_DelayWrapper (void *hTimer, int timeInMs);
extern uInt8 ctrl_ReadRegisterWrapper (void *client, uInt8 ch, uInt8 reg);
extern int ctrl_WriteRegisterWrapper (void *client, uInt8 ch, uInt8 reg, uInt8 val);
extern ramData ctrl_ReadRAMWrapper (void *client, uInt8 ch, uInt16 reg);
extern int ctrl_WriteRAMWrapper (void *client, uInt8 channel, uInt16 ramAddr, ramData data);
extern int ctrl_ResetWrapper (void *client, int status);

#define	SPI_DEVICE	"c2k_spi"

#define __raw_readl(addr)   (*(volatile unsigned int *)(addr))
#define __raw_writel(b,addr)  ((*(volatile unsigned int *)(addr)) = (b))
int proslic_hook_status(void);
struct spi_adapter comcerto_spi_adapter;
extern int spi_attach_client(struct device_d *client);
extern int spi_detach_client(struct device_d *client);

/* proslic_device_init */
static int proslic_device_init(struct proslic_data *proslic, int device)
{
	proslicChanType *ch;
	proslicChanType *chanArray[CHAN_PER_DEVICE];
	u8 pots; int i;

	u16 pcm_clock_slot;
	int error,chanEn;

	PROSLIC_debug("proslic_device_init(%#lx, %d)\n", (unsigned long) proslic, device);

        chanArray[0] = &(proslic->pots[device].proslic_chan);
	/* Change done by MSPD
         * chanArray[1] = &(proslic->pots[device+1].proslic_chan); */
	if (CHAN_PER_DEVICE == 2)
	{
	  chanArray[1] = &(proslic->pots[device+1].proslic_chan);
	}
        pots = device;     /* one POTS line per Si3215 chip */

        ProSLIC_Init(chanArray,CHAN_PER_DEVICE);
	PROSLIC_debug("proslic: initialized proslic device\n");
	for (i=0;i<CHAN_PER_DEVICE;i++)
	{
		ch = chanArray[i];
		ProSLIC_getErrorFlag (ch, &error);
		ProSLIC_getChannelEnable (ch, &chanEn);
		PROSLIC_debug("proslic: error %d\tchanEn %d\n", error, chanEn);
		if (error != NULL)
			goto err;

		/*configurations are stored in proslic_api_config.c/h*/
        	ProSLIC_RingSetup(ch, USA_DEFAULT_RING);
        	/*
          	PCM clock: value calculation
          	ch is the time slot number, *8 to have the value in number of bits
          	We want TDM samples to be sent on negative edge of the Fsync. Then we add 1
          	See Figure 27 page 46 of PROSLIC Datasheet Rev 0.92
         	*/
		ProSLIC_PCMSetup(ch, (tdm_coding==ALAW)?ALAW_PCM:ULAW_PCM);
        	pcm_clock_slot = ((device+i) * 8) + 1;
		ProSLIC_PCMTimeSlotSetup(ch, pcm_clock_slot,  pcm_clock_slot);
       		ProSLIC_PCMStart(ch);
        	ProSLIC_DCFeedSetup (ch, DEFAULT_DCFEED);
		ProSLIC_ZsynthSetup(ch, ZSYNTH_600);

		ProSLIC_TXAudioGainSetup(ch, DEFAULT_AUDIOGAIN);
        	ProSLIC_RXAudioGainSetup(ch, DEFAULT_AUDIOGAIN);
		//Rajiv
		//ProSLIC_EnableInterrupts(ch);
        	/* Set active Mode */
        	PROSLIC_debug("proslic: setting in active mode\n");
        	ProSLIC_SetLinefeedStatus( ch, LF_FWD_ACTIVE );
	}

	return 0;

  err:
	return -1;
}

/* proslic_device_probe */
static int proslic_device_probe(struct proslic_data *proslic)
{
	int i;
	int chanEn;
        i = 0;

#if 0
	do {
		proslic_device_init(proslic, i);
		ProSLIC_getChannelEnable (&(proslic->pots[i].proslic_chan), &chanEn);
		i+=CHAN_PER_DEVICE;

	} while ((chanEn) && (i<8));
#endif

	/* Changes done by MSPD */
	ProSLIC_getChannelEnable (&(proslic->pots[i].proslic_chan), &chanEn);
	while ((chanEn) && (i<MAX_POTS))
	{
		proslic_device_init(proslic, i);
		i+=CHAN_PER_DEVICE;
		ProSLIC_getChannelEnable (&(proslic->pots[i].proslic_chan), &chanEn);

	}

	proslic->max_pots = i;
        PROSLIC_debug("proslic: found %d proslic devices\n", proslic->max_pots/2);

	return 0;
}

int proslic_attach_adapter(struct spi_adapter *adapter)
{
	int i;
	uInt8 hookStat;
	int onhookStat[2];
	int offhookStat[2];
	struct device_d *client = get_device_by_name(SPI_DEVICE);

	proslicChanType_ptr hProslic;

        PROSLIC_debug("proslic: proslic_attach_adapter\n");
	proslic = kmalloc(sizeof (struct proslic_data), GFP_KERNEL);
	if (proslic == NULL)
		goto err0;

	memset(proslic, 0, sizeof(struct proslic_data));
	proslic->client.spi_client = client;
	PROSLIC_debug("proslic: Initializing ProSLIC API\n");

	for (i = 0; i< MAX_POTS; i++)
	{
#if (CHAN_PER_DEVICE == 2)
		ProSLIC_SWInitChan (&(proslic->pots[i].proslic_chan),i,SI3226_TYPE,&(proslic->pots[i/2].proslic_device), &(proslic->proslic_spi));
#else
		ProSLIC_SWInitChan (&(proslic->pots[i].proslic_chan),i,SI321X_TYPE,&(proslic->pots[i].proslic_device), &(proslic->proslic_spi));
#endif
#ifdef PROSLIC_DEBUG
		ProSLIC_setSWDebugMode (&(proslic->pots[i].proslic_chan), 1);
#endif
	}
	ProSLIC_setControlInterfaceCtrlObj (&(proslic->proslic_spi), client);
	ProSLIC_setControlInterfaceReset (&(proslic->proslic_spi), ctrl_ResetWrapper);
	ProSLIC_setControlInterfaceWriteRegister (&(proslic->proslic_spi), ctrl_WriteRegisterWrapper);
	ProSLIC_setControlInterfaceReadRegister (&(proslic->proslic_spi), ctrl_ReadRegisterWrapper);
	ProSLIC_setControlInterfaceWriteRAM (&(proslic->proslic_spi), ctrl_WriteRAMWrapper);
	ProSLIC_setControlInterfaceReadRAM (&(proslic->proslic_spi), ctrl_ReadRAMWrapper);
	ProSLIC_setControlInterfaceTimerObj (&(proslic->proslic_spi), NULL);
	ProSLIC_setControlInterfaceDelay (&(proslic->proslic_spi), time_DelayWrapper);
//	ProSLIC_setControlInterfaceTimeElapsed proslic->proslic_spi, time_TimeElapsedWrapper); //not used (pulse dial)
//	ProSLIC_setControlInterfaceGetTime (proslic->proslic_spi, time_GetTimeWrapper);

	ProSLIC_Reset(&(proslic->pots[0].proslic_chan));

	PROSLIC_debug("proslic: Initializing ProSLIC(s)\n");
	if (proslic_device_probe(proslic))
		goto err1;

	printf("Done.\n");
	return 0;

  err1:
	kfree(proslic);

  err0:
	return -1;
}

int proslic_hook_status(void)
{
        int i;
        uInt8 hookStat;
        int onhookStat[MAX_POTS];
        int offhookStat[MAX_POTS];
        proslicChanType_ptr hProslic;

	for (i = 0; i < proslic->max_pots; i++)
	{
		onhookStat[i] = 1;
		offhookStat[i] = 0;
	}

	printf("Do On/Off Hook operations on the attached POTS phone:\n");
	printf("Press <Control + c> to quit.\n");
	printf("================================\n");

	while(1)
	{
		for (i = 0; i < proslic->max_pots; i++)
		{
			ProSLIC_ReadHookStatus (&(proslic->pots[i].proslic_chan),&hookStat);
			hProslic = &(proslic->pots[i].proslic_chan);
			if (hookStat == ONHOOK)
			{
				proslic->pots[i].state = POTS_STATE_ONHOOK;
				if(onhookStat[i]==1 && offhookStat[i]==0)
				{
					printf("SLIC(%d) POTS %d State: ON-HOOK\n",hProslic->deviceId->chipType,i);
					onhookStat[i]=0;
					offhookStat[i]=1;
				}
			}
			else
			{
				proslic->pots[i].state = POTS_STATE_OFFHOOK;
				if(onhookStat[i]==0 && offhookStat[i]==1)
				{
					printf("SLIC(%d) POTS %d State: OFF-HOOK\n",hProslic->deviceId->chipType,i);
					onhookStat[i]=1;
					offhookStat[i]=0;
				}
			}
			if (ctrlc())
			{
				printf("================================\n");
				printf("Exiting from POTS testing\n");
				printf("================================\n");
				return 0;
			}
		}
	}
}

static int is_proslic_init = 0;

int proslic_init(void)
{
	if(is_proslic_init)
	{
		printf("SLIC already initialized\n");
		return 0;
	}
	is_proslic_init = 1;
	printf("Initializing SLIC...\n");

	proslic_attach_adapter(&comcerto_spi_adapter);

	return 0;
}

