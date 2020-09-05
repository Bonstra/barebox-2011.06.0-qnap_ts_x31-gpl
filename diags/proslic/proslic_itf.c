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

#include "proslic_mspd.h"

#if 0 //Rajiv
#include <linux/spi2/slic_itf.h>
struct slic_itf *proslic_itf;

/**
* proslic_handle_state -
*
*/
void proslic_handle_state(struct proslic_data *proslic, int pots, int prev_pots_state)
{
	if (!proslic->itf) {
		PROSLIC_err("proslic interface not registered yet pots(%d)\n", pots);
            return;
	}

	switch (proslic->pots[pots].state) {
	case POTS_STATE_ONHOOK:
            proslic->itf->disconnect(proslic->itf, pots);
            break;

	case POTS_STATE_OFFHOOK:
            if (prev_pots_state == POTS_STATE_RINGING) {
                proslic->itf->data = (unsigned long)NULL;
                proslic->itf->outgoing_call_ack(proslic->itf, pots);
                proslic->pots[pots].num_rings = 0;
            }
            else
                proslic->itf->incoming_call(proslic->itf, pots);
            
            break;
        case POTS_STATE_RINGING:
            /* at end of first ring, notify up the stack */
            proslic->pots[pots].num_rings++;
            proslic->itf->data = (unsigned long)&proslic->pots[pots].num_rings;
            proslic->itf->outgoing_call_ack(proslic->itf, pots);
            break;
        case POTS_STATE_FLASH:
            /* gotten a brief ONHOOK/OFFHOOK transition */
            proslic->itf->flash(proslic->itf, pots);
            proslic->pots[pots].state = POTS_STATE_OFFHOOK;
            break;
	default:
            break;
	}
}

/**
* slic_adapter_outgoing_call -
*
*/
int slic_adapter_outgoing_call(struct slic_itf *itf, int pots)
{
	struct proslic_data *proslic = (struct proslic_data *) itf->itf_data;

        PROSLIC_debug("proslic: slic_adapter_outgoing_call entry\n");
	if (proslic == NULL) {
		PROSLIC_err("proslic interface not registered yet pots(%d)\n", pots);
		goto err;
	}

	if (pots >= proslic->max_pots) {
		PROSLIC_err("pots(%d) out of range\n", pots);
		goto err;
	}

	switch (proslic->pots[pots].state) {
	case POTS_STATE_ONHOOK:
		proslic->pots[pots].state = POTS_STATE_RINGING;
		proslic->pots[pots].num_rings = 0;
		ProSLIC_RingStart (&(proslic->pots[pots].proslic_chan));
		break;

	case POTS_STATE_OFFHOOK:
		goto err;
		break;

	case POTS_STATE_RINGING:
		goto err;
		break;

	default:
		PROSLIC_err("unknown pots(%d) state\n", pots);
		goto err;
		break;
	}

	return 0;

  err:
	return -1;
}

/**
* slic_adapter_disconnect -
*
*/
int slic_adapter_disconnect(struct slic_itf *itf, int pots)
{
	struct proslic_data *proslic = (struct proslic_data *) itf->itf_data;

        PROSLIC_debug("proslic: slic_adapter_disconnect entry\n");
	if (proslic == NULL) {
		PROSLIC_err("proslic interface not registered yet pots(%d)\n", pots);
		goto err;
	}

	if (pots >= proslic->max_pots) {
		PROSLIC_err("pots(%d) out of range\n", pots);
		goto err;
	}

	switch (proslic->pots[pots].state) {
	case POTS_STATE_ONHOOK:
		goto err;
		break;

	case POTS_STATE_OFFHOOK:
		break;

	case POTS_STATE_RINGING:
		proslic->pots[pots].state = POTS_STATE_ONHOOK;
		proslic->pots[pots].num_rings = 0;
		ProSLIC_RingStop (&(proslic->pots[pots].proslic_chan));
		break;

	default:
		PROSLIC_err("unknown pots(%d) state\n", pots);
		goto err;
		break;
	}

	return 0;

  err:
	return -1;
}

/**
* slic_adapter_register -
*
*/
int slic_adapter_register(struct slic_itf *itf)
{
        PROSLIC_debug("proslic: slic_adapter_register entry\n");
	/* we support a single interface */
	if (proslic_itf) {
		PROSLIC_err("slic interface already registered\n");
		goto err;
	}

	/* keep track of the registered interface */
	proslic_itf = itf;

	/* if the proslic is already running associate it to the interface */
	if (proslic) {
		proslic->itf = itf;
		proslic->itf->itf_data = (unsigned long) proslic;
	}

	return 0;

  err:
	return -1;
}

/**
* slic_adapter_unregister -
*
*/
void slic_adapter_unregister(struct slic_itf *itf)
{
	struct proslic_data *proslic = (struct proslic_data *) itf->itf_data;
    PROSLIC_debug("proslic: slic_adapter_unregister entry\n");
	proslic_itf = NULL;

	if (proslic) {
		proslic->itf = NULL;
		itf->itf_data = (unsigned long) NULL;
	}
}

EXPORT_SYMBOL(slic_adapter_outgoing_call);
EXPORT_SYMBOL(slic_adapter_disconnect);
EXPORT_SYMBOL(slic_adapter_register);
EXPORT_SYMBOL(slic_adapter_unregister);
#endif //Rajiv
