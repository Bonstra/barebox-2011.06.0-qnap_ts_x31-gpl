/** \file vp_hal.c
 * vp_hal.c
 *
 * This file contains the platform dependent code for the Hardware Abstraction
 * Layer (HAL). This implementation is for embedded Linux. This HAL
 * implementation assumes a Linux kernel mode driver and also an application
 * level interface to those kernel drivers.
 *
 * Copyright (c) 2005, Legerity Inc.
 * All rights reserved
 *
 * This software is the property of Legerity , Inc. Please refer to the
 * Non Disclosure Agreement (NDA) that you have signed for more information
 * on legal obligations in using, modifying or distributing this file.
 */

#include "vp_hal.h"

#include "../../../legerity.h"

extern struct legerity_data *legerity;

extern int spi_write(struct spi_device *spi, const void *buf, size_t len);
extern int spi_read(struct spi_device *spi, void *buf, size_t len);

/**
 * VpHalHbiInit(): Configures the HBI bus and glue logic (if any)
 *
 * This function performs any tasks necessary to prepare the system for
 * communicating through the HBI, including writing the HBI configuration
 * register.  The HBI read and write functions should work after HbiHalInit()
 * is successfully executed. HbiHalInit() should be well-behaved even if called
 * more than once between system resets. HbiHalInit() is called from
 * VpBootLoad() since VpBootLoad() is normally the first VoicePath function
 * that the host application will call.
 *
 * This function is called by VpBootLoad() before sending the DVP firmware
 * image through the HBI.
 *
 * Params:
 *  uint8 deviceId: Device Id (chip select ID)
 *
 * Returns:
 *  This function returns FALSE if some error occurred during HBI initialization
 *  or TRUE otherwise.
 */
bool VpHalHbiInit(
    VpDeviceIdType deviceId)
{
    /*
     * Note that Setting up the basic device should be handled by the
     * some board bring up function. That function should setup the
     * CPLD based on the line module that is plugged in. Those functions
     * would configure the CPLD so that basic communication to the part
     * can happen between the HAL and the line module.
     */

    /* Write the HBI configuration register. */
    return 0;
} /* VpHalHbiInit() */

/**
 * VpHalHbiCmd(): Sends a command word over the HBI, with no data words.
 *
 *  Accepts a uint16 HBI command which is little-endian or big-endian,
 * depending on the host architecture.  Command words on the HBI bus are always
 * big-endian. This function is responsible for byte-swapping if required. This
 * implementation for Linux does not need any byte swapping.
 *
 * Params:
 * uint8 deviceId: Device Id (chip select ID)
 * uint16 cmd: the command word to send
 *
 * Returns:
 *   TRUE on success, FALSE on failure
 */
bool VpHalHbiCmd(
    VpDeviceIdType deviceId,
    uint16 cmd)
{
    return 0;
} /* VpHalHbiCmdWr() */

/**
 * VpHalHbiWrite(): Sends a command word and up to 256 data words over the HBI.
 *
 *  Accepts a uint16 HBI command which is little-endian or big-endian, depending
 * on the host architecture.  Command words on the HBI bus are always big-
 * endian.  This function is responsible for byte-swapping the command word, if
 * required. Note that this Linux implementation does not need byte swapping.
 *
 *  Accepts an array of uint16 data words.  No byte-swapping is necessary on
 * data words in this function.  Instead, the HBI bus can be configured in
 * VpHalHbiInit() to match the endianness of the host platform.
 *
 * Params:
 *   uint8 deviceId: Device Id (chip select ID)
 *   uint16 cmd: the command word to send
 *   uint8 numwords: the number of data words to send, minus 1
 *   uint16p data: the data itself; use data = (uint16p)0 to send
 *      zeroes for all data words
 *
 * Returns:
 *   TRUE on success, FALSE on failure
 */
bool VpHalHbiWrite(
    VpDeviceIdType deviceId,
    uint16 cmd,
    uint8 numwords,
    uint16p data)
{
    return 0;
} /* VpHalHbiWrite() */

/**
 * VpHalHbiRead(): Sends a command, and receives up to 256 data words over the
 * HBI.
 *
 *  Accepts a uint16 HBI command which is little-endian or big-endian, depending
 * on the host architecture.  Command words on the HBI bus are always big-
 * endian.  This function is responsible for byte-swapping the command word, if
 * required.
 *
 * Retrieves an array of uint16 data words.  No byte-swapping is necessary on
 * data words in this function.  Instead, the HBI bus can be configured in
 * VpHalHbiInit() to match the endianness of the host platform.
 *
 * Params:
 *   uint8 deviceId: Device Id (chip select ID)
 *   uint8 numwords: the number of words to receive, minus 1
 *   uint16p data: where to put them
 *
 * Returns:
 *   TRUE on success, FALSE on failure
 */
bool VpHalHbiRead(
    VpDeviceIdType deviceId,
    uint16 cmd,
    uint8 numwords,
    uint16p data)
{
    return 0;
} /* VpHalHbiRead() */

/**
 * VpHalHbiWrite8(): Sends a command word and up to 256 data words over the HBI.
 *
 *  Accepts a uint16 HBI command which is little-endian or big-endian, depending
 * on the host architecture.  Command words on the HBI bus are always big-
 * endian.  This function is responsible for byte-swapping the command word, if
 * required. Note that this Linux implementation does not need byte swapping.
 *
 *  Accepts an array of uint8 data words.  No byte-swapping is necessary on
 * data words in this function.  Instead, the HBI bus can be configured in
 * VpHalHbiInit() to match the endianness of the host platform.
 *
 * Params:
 *   uint8 deviceId: Device Id (chip select ID)
 *   uint16 cmd: the command word to send
 *   uint8 numwords: the number of data words to send, minus 1
 *   uint8p data: the data itself; use data = (uint8p)0 to send
 *   zeroes for all data words
 *
 * Returns:
 *   TRUE on success, FALSE on failure
 */
bool VpHalHbiWrite8(
    VpDeviceIdType deviceId,
    uint16 cmd,
    uint8 numwords,
    uint8p data)
{
    return 0;
} /* VpHalHbiWrite8() */

/**
 * VpHalHbiRead8(): Sends a command, and receives up to 256 data words over the
 * HBI.
 *
 *  Accepts a uint16 HBI command which is little-endian or big-endian, depending
 * on the host architecture.  Command words on the HBI bus are always big-
 * endian.  This function is responsible for byte-swapping the command word, if
 * required.
 *
 *  Retrieves an array of uint8 data bytes.  No byte-swapping is necessary on
 * data words in this function.  Instead, the HBI bus can be configured in
 * VpHalHbiInit() to match the endianness of the host platform.
 *
 * Params:
 *   uint8 deviceId: Device Id (chip select ID)
 *   uint8 numwords: the number of words to receive, minus 1
 *   uint8p data: where to put them
 *
 * Returns:
 *   TRUE on success, FALSE on failure
 */
bool VpHalHbiRead8(
    VpDeviceIdType deviceId,
    uint16 cmd,
    uint8 numwords,
    uint8p data)
{
    return 0;
} /* VpHalHbiRead8() */

/**
 * VpHalHbiBootWr():
 *
 *  This is used by the VpBootLoad() function to send the boot stream to the
 * DVP.  This function is separate from VpHalHbiWrite(), for the following
 * reasons:
 *
 *  1. This function does not accept a command word; only data words.
 *  2. This function accepts uint8 data, instead of uint16 data.  Be careful
 *     not to assume that this data is word-aligned in memory.
 *  3. The HBI must be configured for big-endian data words while the boot
 *     stream is being transmitted, regardless of the endianness of the host
 *     platform.  This is because the boot image is an opaque stream of HBI
 *     command words and data words.  Therefore, commands and data cannot be
 *     distinguished for separate treatment by this function.  Since HBI
 *     command words are always big-endian, data words have to be big-endian,
 *     too.  The boot stream is stored big-endian in memory, even on little-
 *     endian hosts.
 *        If VpHalHbiInit() configures the HBI for little-endian data words,
 *     then this function must temporarily change the configuration by calling
 *     VpHalHbiCmd(HBI_CMD_CONFIGURE(...)), and change it back before
 *     returning.  In such a case, this function will need to swap each pair
 *     of bytes in the boot stream before sending.
 *        Another possibility (as in the case of our test platform) is a
 *     little-endian host architecture, with the HBI bus configured for big-
 *     endian data words.  In this case, byte-swapping has to be done in
 *     VpHalHbiWrite() or in the glue logic between the host and the DVP.
 *     In these setups, VpHalHbiBootWr() does not need to reconfigure the
 *     HBI.
 *  4. This function takes a VpImagePtrType pointer to char data, which is a
 *     platform-dependent type defined in vp_hal.h.  In our system, a "_huge"
 *     pointer is needed for this function, since the boot stream is larger
 *     than 64 kB.
 *
 * Params
 *   uint8 deviceId: Device Id (chip select ID)
 *  'length' specifies the number of 16-bit words to write to the DVP.
 *  'pBuf' points into the boot stream.
 *
 * Returns
 *  HbiHalBootWr() returns TRUE on success, FALSE on failure.
 *
 * Notes
 *  THIS FUNCTION IS NOT REENTRANT!
 */
bool VpHalHbiBootWr(
    VpDeviceIdType deviceId,
    uint8 numwords,
    VpImagePtrType data)
{
    return 0;
} /* VpHalHbiBootWr() */


/*****************************************************************************
 * HAL functions for CSLAC devices. Not necessary for VCP and VPP
 ****************************************************************************/

/**
 * VpMpiCmd()
 *  This function executes a Device MPI command through the MPI port. It
 * executes both read and write commands. The read or write operation is
 * determined by the "cmd" argument (odd = read, even = write). The caller must
 * ensure that the data array is large enough to hold the data being collected.
 * Because this command used hardware resources, this procedure is not
 * re-entrant.
 *
 * Preconditions:
 *  The device must be initialized.
 *
 * Postconditions:
 *   The data pointed to by dataPtr, using the command "cmd", with length
 * "cmdLen" has been sent to the MPI bus via the chip select associated with
 * deviceId.
 */
void
VpMpiCmd(
    VpDeviceIdType deviceId,    /**< Chip select, connector and 3 or 4 wire
                                 * interface for command
                                 */
    uint8 ecVal,        /**< Value to write to the EC register */
    uint8 cmd,          /**< Command number */
    uint8 cmdLen,       /**< Number of bytes used by command (cmd) */
    uint8 *dataPtr)     /**< Pointer to the data location */
{
    int i;
    char buf[1];
    struct legerity_slic_desc *slic = legerity->slic_list;

    VpSysEnterCritical(deviceId, VP_MPI_CRITICAL_SEC);

    /*
     * write the ec register only if the cmd argument is
     * not the read ec register command
     */
    if (cmd != 0x4B) {
	buf[0] = 0x4A;
        spi_write((struct spi_device *)slic->spi_client->type_data, &buf, 1);
	buf[0] = ecVal;
        spi_write((struct spi_device *)slic->spi_client->type_data, &buf, 1);
    }

    buf[0] = cmd;
    spi_write((struct spi_device *)slic->spi_client->type_data, &buf, 1);

    if (cmd & 0x01) {
	    /*
	     * ZSI devices have a special read requirement that require
	     * the host to read cmdLen+1 data bytes and to discard the
	     * first byte. Escentially, we are priming the ZSI RTL
	     * block pump. We know that the ZSI RTL uses chip select
	     * number 3 i.e. deviceid 3.
	     * One other requirement of the ZSI RTL is that the data on
	     * the MOSI SPI signal be an unrecognized or benign command.
	     * Therefor we should really be using a spi_write_and_readb()
	     * of somekind. I need Mindspeed to add the function call
	     * necessary to make that happen here - Microsemi -S.H.
	     */
	    if (deviceId == 3) {
		    uint8 discardByte;
		    spi_read((struct spi_device *)slic->spi_client->type_data, &discardByte, 1);
	    }

	    for (i = 0; i < cmdLen; i++) {
		    spi_read((struct spi_device *)slic->spi_client->type_data, &dataPtr[i], 1);
	    }
    } else {
	    for (i = 0; i < cmdLen; i++) {
		    buf[0] = dataPtr[i];
		    spi_write((struct spi_device *)slic->spi_client->type_data, &buf, 1);
	    }
    }

#ifdef LEGERITY_DEBUG_HAL
    PRINT (KERN_INFO "Device(%d): ", deviceId);
    PRINT ("SELECT(%02x, %02x) - ", 0x4a, ecVal);
    if (cmd & 0x01) {
	    PRINT ("READ (");
    } else {
	    PRINT ("WRITE(");
    }
    PRINT("cmd=%02x", cmd);
    if (cmdLen > 0) {
	    int i;
	    PRINT(", dataPtr=");
	    for (i = 0; i < cmdLen; i++) {
		    PRINT("%02x", dataPtr[i]);
		    if (i < (cmdLen - 1)) PRINT(",");
	    }
    }
    PRINT(")\n");
#endif
    VpSysExitCritical(deviceId, VP_MPI_CRITICAL_SEC);
    return;
} /* End VpMpiCmd */


/**
 * VpMpiReset()
 *  This function executes a 16-bit MPI command with the CS low to cause a
 * reset of the device.
 *
 * Preconditions:
 *  None.
 *
 * Postconditions:
 *  CS for the device specified by deviceId on the MPI bus is high (after being
 * held low for 16 clock cycles).
 */
void
VpMpiReset(
		VpDeviceIdType deviceId,
		VpDeviceType deviceType)
{
	char buf[2] = { 0, 0 };
	VpSysEnterCritical(deviceId, VP_MPI_CRITICAL_SEC);
	spi_write((struct spi_device *)legerity->slic_list->spi_client->type_data, &buf, 2);

#ifdef LEGERITY_DEBUG
	PRINT (KERN_INFO "Device(%d): RESET)\n", deviceId);
#endif
	VpSysExitCritical(deviceId, VP_MPI_CRITICAL_SEC);
	return;
}

