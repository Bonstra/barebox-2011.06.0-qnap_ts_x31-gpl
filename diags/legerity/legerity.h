/*
 * Legerity driver for Mindspeed Comcerto.
 *
 *  Copyright (C) 2008 Mindspeed Technologies, Inc.
 *  All rights reserved
 */

#ifndef LEGERITY_H
#define LEGERITY_H

#include <mach/comcerto_spi.h>
#include <asm/hardware.h>
//#include <malloc.h>

#define PRINT printf
#define MALLOC(a) malloc(a)
#define FREE kfree

/* Fix to avoid compile error of API-II lib */
#ifdef __KERNEL__
#undef current
#endif

#include "api-II/api_lib/includes/vp_api.h"
#ifdef LINE_TESTING_ENABLED
#include "lt_api.h"
#endif

#include "profiles8-le88264_abs_zsi.h"

#define SPI_DEVICE_LEGERITY 1

#define	SLIC_TYPE_LE88264_ABS_ZSI	14
#define	SLIC_MAX_LINES_LE88264_ABS_ZSI	2
typedef enum {
    SLIC_TYPE_NONE = 0,
    SLIC_TYPE_RESERVED,
    SLIC_TYPE_LE88231 = 2,
    SLIC_TYPE_LE88241,
    SLIC_TYPE_LE88231MEGAMOMBASA,
    SLIC_TYPE_LE88211,
    SLIC_TYPE_LE88221,
    SLIC_TYPE_LE88010, // fXo implementation FXO  value 7
    SLIC_TYPE_VE8820_INVERT_BOOST,
    SLIC_TYPE_VE8820_FLYBACK,
    SLIC_TYPE_LE88266,
    SLIC_TYPE_LE89010, // fXo implementation FXO  value 11
    SLIC_TYPE_LE88111_BUCK_BOOST
} SlicType;

typedef enum {
    SLIC_MAX_LINES_LE88231 = 2,
    SLIC_MAX_LINES_LE88241 = 2,
    SLIC_MAX_LINES_LE88231MEGAMOMBASA = 2,
    SLIC_MAX_LINES_LE88211 = 2,
    SLIC_MAX_LINES_LE88221 = 2,
    SLIC_MAX_LINES_LE88010 = 1,// fXo implementation FXO :/desc
    SLIC_MAX_LINES_VE8820_INVERT_BOOST = 2,
    SLIC_MAX_LINES_VE8820_FLYBACK = 2,
    SLIC_MAX_LINES_LE88266 = 2,
    SLIC_MAX_LINES_LE89010 = 1, // fXo implementation FXO :/desc
    SLIC_MAX_LINES_LE88111_BUCK_BOOST = 1

} SlicMaxLines;

#define LEGERITY_DRIVER_NAME "legerity"
#define LEGERITY_DRIVER_VERSION "2.17"
#define LEGERITY_TIMER_TICK 10000
#define LEGERITY_LT_TIMER_TICK 2000
#define LEGERITY_MAX_SLICS 22
#define LEGERITY_MAX_LINES 4
#define LEGERITY_SPI_SC_RATE 4000000
#define LEGERITY_SPI_CS_DELAY 8

typedef enum {
    POTS_STATE_ONHOOK = 0,
    POTS_STATE_OFFHOOK,
    POTS_STATE_RINGING,
} PotsStateType;

typedef enum {
    LT_IDLE = 0,
    LT_INIT_SLIC,
    LT_INIT_SLIC_COMPLETE,
    LT_RUNNING,
    LT_FINISHED,
    LT_REBOOT_SLIC,
    LT_REBOOT_SLIC_COMPLETE,
} LtStatus;

struct legerity_slic_line_desc {
    VpLineCtxType context;
    void * object;
    PotsStateType state;
};

struct legerity_slic_desc {
    SlicType type;
    VpDeviceType deviceType;
    SlicMaxLines slicMaxLines;
    VpTermType termType;
    VpProfilePtrType deviceProfile;
    VpProfilePtrType deviceProfileLt;
    VpProfilePtrType acProfile;
    VpProfilePtrType dcProfile;
    VpProfilePtrType ringProfile;
    VpProfilePtrType ringCadence;
        VpProfilePtrType FxoCfgProfile;// fXo implementation FXO

    struct device_d *spi_client;

    int id;
    void *object;
    VpDevCtxType context;

    VpOptionDeviceIoType io_type;
    VpDeviceIoAccessDataType io;

    struct legerity_slic_line_desc * line_list;

    unsigned long driver_lock_flags;

    unsigned long api2_lock_flags;
    int api2_lock_count;

    int num_rings;
};

struct slic_itf
{
        void (*disconnect)(struct slic_itf *itf, uint8 pots);
        void (*incoming_call)(struct slic_itf *itf, uint8 pots);
        void (*outgoing_call_ack)(struct slic_itf *itf, uint8 pots);
        void (*flash)(struct slic_itf *itf, uint8 pots);
        void (*fxo_incoming_call)( uint8 pots,int state);

        unsigned long data;

        unsigned long itf_data;
};

int slic_adapter_outgoing_call(struct slic_itf *itf, int pots);
int slic_adapter_disconnect(struct slic_itf *itf, int pots);
int slic_adapter_register(struct slic_itf *itf);
void slic_adapter_unregister(struct slic_itf *itf);

struct legerity_data {
    /* module options */
    VpOptionCodecType tdm_coding;
    int flash_time;

    /* interrupt */
    uint8 irq;
    bool owned_irq;

    /* driver background tick */
    int tick_handler_counter;
    bool running_timer;
    int tick_count;

    int number_of_slics;
    struct legerity_slic_desc * slic_list;

    /* line testing */

    /* interface to wan module */
    struct slic_itf *itf;
};

int legerity_slicline_to_pots_id(int slic_id, int line_id);
int legerity_pots_to_slic_id(int pots);
int legerity_pots_to_line_id(int pots);

#endif


