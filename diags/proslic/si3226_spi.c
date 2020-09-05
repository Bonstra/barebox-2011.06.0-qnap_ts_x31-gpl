/*
** $Id: si3226_spi.c,v 1.1.1.1 2014/03/12 12:17:40 wokes Exp $
**
** SPI driver implementation file
**
** Author(s):
** laj (lajordan@silabs.com)
**
** Distributed by:
** Silicon Laboratories, Inc
**
** File Description:
** This is the implementation file for the SPI driver used
** in the ProSLIC API. It calls the generic SPI driver
** provided my Mindspeed.
**
** Platform:
** Mindspeed FE Router
**
** Copyright (c) 2008 Silicon Laboratories, Inc
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
//#include "proslic_api/proslic_ctrl.h"
#include "proslic_api/si_voice_ctrl.h"


#define CNUM_TO_CID_QUAD(channelNumber)   (((channelNumber<<4)&0x10)|((channelNumber<<2)&0x8)|((channelNumber>>2)&0x2)|((channelNumber>>4)&0x1)|(channelNumber&0x4))

/*
** Function: spiGci_ResetWrapper
**
** Description:
** Sets the reset pin of the ProSLIC
*/
int ctrl_ResetWrapper (void *client, int status)
{
/* If this function is filled in daisyChain should be
 * reset to 0 when the ProSLIC is reset
 */
	//client->daisyChain = 0;
	return 0;
}

/*
** SPI/GCI register read
**
** Description:
** Reads a single ProSLIC register
**
** Return:
** none
*/
uInt8 ctrl_ReadRegisterWrapper (void *clientPtr, uInt8 ch, uInt8 reg)
{

	u8 val;
	char buf[1];
	struct spi_itf *spiInt = clientPtr;
	struct device_d *client = &spiInt->spi_client;
        unsigned long irqflags;
	if (ch <=31 )
		ch = CNUM_TO_CID_QUAD(ch)|0x60;
//        spin_lock_irqsave(&spiInt->lock, irqflags);
	buf[0] = ch;
      	spi_write((struct spi_device *)client->type_data, &buf, 1);
	buf[0] = reg;
	spi_write((struct spi_device *)client->type_data, &buf, 1);
	spi_read((struct spi_device *)client->type_data, &val, 1);
//        spin_unlock_irqrestore(&spiInt->lock, irqflags);
	//SI3215_debug("si3215: read  ctrl:%02x register:%0d value:%02x daisyChain %d\n", ch, reg, val,spiInt->daisyChain);

	return val;
}

/*
** Function: spiGci_WriteRegisterWrapper
**
** Description:
** Writes a single ProSLIC register
**
** Return:
** none
*/
int ctrl_WriteRegisterWrapper (void *clientPtr, uInt8 ch, uInt8 reg, uInt8 val)
{
	char buf[1];
	unsigned long irqflags;
	struct spi_itf *spiInt = clientPtr;
	struct device_d *client = &spiInt->spi_client;
	if (ch <=31 )
		ch = CNUM_TO_CID_QUAD(ch)|0x20;
	if (ch == 0xff) /*broadcast*/
		ch = 0x20 | 0x80;
//      SI3215_debug("Si3215: write ch:%x register:%d value:%x\n", ch, reg, val);
//        spin_lock_irqsave(&spiInt->lock, irqflags);
	buf[0] = ch;
	spi_write((struct spi_device *)client->type_data, &buf, 1);
	buf[0] = reg;
	spi_write((struct spi_device *)client->type_data, &buf, 1);
	buf[0] = val;
	spi_write((struct spi_device *)client->type_data, &buf, 1);

//        spin_unlock_irqrestore(&spiInt->lock, irqflags);

	return 0;
}

static void RAMwait (struct spi_itf *client, uInt8 channel)
{
	unsigned char regVal;

	do
	{
		regVal = ctrl_ReadRegisterWrapper (client,channel,4);
	}while (regVal&0x01);//wait for indirect registers. TODO: Add timeout?

}



/*
** Function: SPI_ReadRAMWrapper
**
** Description:
** Reads a single ProSLIC RAM location
**
** Return:
** none
*/

ramData ctrl_ReadRAMWrapper (void *client, uInt8 ch, uInt16 reg)
{

	uInt32 RegVal;

	RAMwait(client, ch);

        ctrl_WriteRegisterWrapper(client, ch, 5, (reg>>3)&0xE0);
	ctrl_WriteRegisterWrapper(client, ch, 10, reg);

	RAMwait(client, ch);

	reg=ctrl_ReadRegisterWrapper(client,ch,6);
	RegVal = reg>>3;
	reg=ctrl_ReadRegisterWrapper(client,ch,7);
	RegVal |= ((unsigned long)reg)<<5;
	reg=ctrl_ReadRegisterWrapper(client,ch,8);
	RegVal |= ((unsigned long)reg)<<13;
	reg=ctrl_ReadRegisterWrapper(client,ch,9);
	RegVal |= ((unsigned long)reg)<<21;
	return RegVal;
}


/*
** Function: SPI_WriteRAMWrapper
**
** Description:
** Writes a single ProSLIC RAM location
**
** Return:
** none
*/

int ctrl_WriteRAMWrapper (void *client, uInt8 ch, uInt16 reg, ramData val)
{


	RAMwait(client, ch);
	ctrl_WriteRegisterWrapper(client, ch, 5, (reg>>3)&0xE0);
        ctrl_WriteRegisterWrapper(client, ch, 6,(val<<3)&0xff);
	ctrl_WriteRegisterWrapper(client, ch, 7,(val>>5)&0xff);
	ctrl_WriteRegisterWrapper(client, ch, 8,(val>>13)&0xff);
	ctrl_WriteRegisterWrapper(client, ch, 9,(val>>21)&0xff);

	ctrl_WriteRegisterWrapper(client, ch, 10, reg);
	return 0;
}



/*
** $Log: si3226_spi.c,v $
** Revision 1.1.1.1  2014/03/12 12:17:40  wokes
** bootloader for MindSpeed
**
** Revision 1.1  2008/11/27 07:29:39  rajivkumar.vishwakarma
** LOG: ProSlic driver integrated to DiagTool for POTS testing
**
** Revision 1.1.2.1  2008/11/19 12:51:07  mouli.burla
** LOG: 42063
** Few compilation errors are removed for Si3226.
**
** Revision 1.1  2008/04/28 13:46:36  appalaraju.chapala
** proslic initial checkin from silabs ver5.00.1
**
**
*/
