/** \file sys_service.c
 * sys_service.c
 *
 *  This file implements the required system services for the API-II using a
 * Linux OS running on the UVB.  The user should replace the functions provided
 * here with the equivalent based on their OS and hardware.
 *
 * Copyright (c) 2005, Legerity Inc.
 * All rights reserved
 *
 * This software is the property of Legerity , Inc. Please refer to the
 * Non Disclosure Agreement (NDA) that you have signed for more information
 * on legal obligations in using, modifying or distributing this file.
 */

#include "../../arch/uvb/sys_service.h"
#include <mach/comcerto-2000.h>
#include "../../../legerity.h"
#include <asm/io.h>
#include <mach/gpio.h>

extern struct legerity_data *legerity;

static struct _legerity_test_mem
{
    int used;
    VpTestHeapType *mem;
} testMemory[255];

#ifdef VXWORKS
static int lock_lvl;
#endif

/*
 * VpSysWait() function implementation is needed only for CSLAC devices
 * (880, 790). For other devices this function could be commented.
 */
void
VpSysWait(
    uint8 time)  /* Time specified in increments of 125uS (e.g. 4 = 500uS) */
{
#ifdef VXWORKS
    struct timespec tm;

    tm.tv_sec = 0;
    tm.tv_nsec = time * 125000;
    nanosleep(&tm, NULL);
#else
    udelay(125 * time);
#endif
}

/*
 * VpSysEnterCritical(), VpSysExitCritical():
 *
 *  These functions allow for disabling interrupts while executing nonreentrant
 * portions of VoicePath API code. Note that the following implementations of
 * enter/exit critical section functions are simple implementations. These
 * functions could be expanded (if required) to handle different critical
 * section types differently.
 *
 * Params:
 *  VpDeviceIdType deviceId: Device Id (chip select ID)
 *  VpCriticalSecType: Critical section type
 *
 * Return:
 *  Number of critical sections currently entered for the device.
 */
uint8
VpSysEnterCritical(     VpDeviceIdType deviceId,
    VpCriticalSecType criticalSecType)
{
    uint8 rc;

    legerity->slic_list->api2_lock_count++;

    rc = legerity->slic_list->api2_lock_count;

    return rc;
} /* VpSysEnterCritical() */

uint8
VpSysExitCritical(
    VpDeviceIdType deviceId,
    VpCriticalSecType criticalSecType)
{
    uint8 rc;

    rc = --legerity->slic_list->api2_lock_count;
    return rc;
} /* VpSysExitCritical() */

/**
 * VpSysDisableInt(), VpSysEnableInt(), and VpSysTestInt()
 *
 *  These functions are used by the CSLAC device family for interrupt driven
 * polling modes. These are called by the API to detect when a non-masked
 * device status has changed.  If using SIMPLE_POLL mode, these functions do not
 * require implementation.
 *
 * Preconditions:
 *  None. The implementation of these functions is architecture dependent.
 *
 * Postconditions:
 *  VpSysDisableInt() - The interrupt associated with the deviceId passed is
 * disabled.
 *
 * VpSysEnableInt() - The interrupt associated with the deviceId passed is
 * enabled.
 *
 * VpSysTestInt() - The return value is TRUE if an interrupt occurred, otherwise
 * return FALSE.
 *
 * These functions are needed only for CSLAC devices
 * (880, 790). For other devices these functions could be commented.
 *
 */
void
VpSysDisableInt(
    VpDeviceIdType deviceId)
{
    return;
}

void
VpSysEnableInt(
    VpDeviceIdType deviceId)
{
    return;
}

bool
VpSysTestInt(
    VpDeviceIdType deviceId)

{
    bool rc;
#ifdef VXWORKS
    rc = (!(readl(GPIO_INPUT_REG) & 0x1));
#else
    rc = (!(comcerto_gpio_read(0xff) & 1));
#endif
    return rc;
}

/**
 * VpSysDtmfDetEnable(), VpSysDtmfDetDisable()
 *
 *  These functions are used by the CSLAC device family for devices that do not
 * internally detect DTMF. It is used for Caller ID type-II and is provided to
 * enable external DTMF detection.
 *
 * Preconditions:
 *  None. The implementation of these functions is application dependent.
 *
 * Postconditions:
 *  VpSysDtmfDetEnable() - The device/channel resource for DTMF detection is
 * enabled.
 *
 *  VpSysDtmfDetDisable() - The device/channel resource for DTMF detection is
 * disabled.
 *
 * These functions are needed only for CSLAC devices
 * (880, 790). For other devices these functions could be commented.
 *
 */

void
VpSysDtmfDetEnable(
    VpDeviceIdType deviceId,
    uint8 channelId)
{
    return;
}

void
VpSysDtmfDetDisable(
    VpDeviceIdType deviceId,
    uint8 channelId)
{
    return;
}

/*  * The following functions VpSysTestHeapAcquire(),  VpSysTestHeapRelease()
 * VpSysPcmCollectAndProcess() and are needed only for CSLAC devices
 * (880). For other devices these functions could be commented.
 *
 */
void * VpSysTestHeapAcquire(
    uint8 *pHeapId)
{
    int i;

    for (i=0; i<256; i++) {
        if (!testMemory[i].used) {
            *pHeapId = i;
            testMemory[*pHeapId].mem = (VpTestHeapType *) MALLOC(sizeof (VpTestHeapType));
            testMemory[*pHeapId].used = 1;
            return (void *) testMemory[*pHeapId].mem;
        }
    }
       return NULL;
} /* VpSysTestHeapAcquire() */

bool VpSysTestHeapRelease(
    uint8 heapId)
{
   if (testMemory[heapId].used) {
           FREE(testMemory[heapId].mem);
           testMemory[heapId].used = 0;
   } else {
           PRINT (KERN_INFO "ERROR: Trying to free none allocated memory\n");
           return 1;
   }
   return 0;
} /* VpSysTestHeapRelease() */

 void VpSysPcmCollectAndProcess(
    void *pLineCtx,
    VpDeviceIdType deviceId,
    uint8 channelId,
    uint8 startTimeslot,
    uint16 operationTime,
    uint16 settlingTime,
    uint16 operationMask)
{
    return;
} /* VpSysPcmCollectAndProcess() */
