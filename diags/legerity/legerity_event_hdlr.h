/*
 * Legerity driiver for Mindspeed Comcerto.
 *
 *  Copyright (C) Mindspeed Technologies, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */

#ifndef LEGERITY_EVENT_HDLR_H
#define LEGERITY_EVENT_HDLR_H

#include "legerity.h"

#ifdef _LINUX_TYPES_H
#undef _LINUX_TYPES_H
#endif

typedef struct
{
    int deviceId;
    int channelId;
}Fxo_t;


int legerity_slicline_to_pots_id(int slic_id, int line_id);
int legerity_pots_to_slic_id(int pots);
int legerity_pots_to_line_id(int pots);

void legerity_event_handler(VpEventType pEvent);

#endif
