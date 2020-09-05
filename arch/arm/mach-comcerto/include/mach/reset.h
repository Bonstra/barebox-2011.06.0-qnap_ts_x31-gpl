/*
 * Copyright (C) Mindspeed Technologies, Inc. 2011. All rights reserved.
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
 *
 * @file reset.h
 * @brief this header file will contain all required
 * information for  external Device GPIO reset
 * @date 06/08/2013
 */

#ifndef __COMCERTO_RESET_H__
#define __COMCERTO_RESET_H__


/* C2000 device blocks which are to be put
 * in out of reset(GPIO).
 */
typedef enum {
	COMPONENT_ATHEROS_SWITCH=0,
	COMPONENT_SLIC,
	COMPONENT_PCIE0,
	COMPONENT_PCIE1,
	COMPONENT_USB_HUB,
	COMPONENT_EXP_DAUGTHER_CARD,
	COMPONENT_RGMII0,
	COMPONENT_RGMII1
}C2000_GEN2_GPIO_RESET_COMPONENT;

void GPIO_reset_external_device(int block,int state);
#endif




