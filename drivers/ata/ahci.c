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
 * with the reference on libata and ahci drvier in kernel and ahci 
 * driver from u-boot.
 *
 * @file ahci.c
 * @brief This C file will contain all required functions to initialize
 *        and access SATA Devices through Synopsys DWC AHCI controller.
 * @date 10/02/2011
 *
 */

#include <stdio.h>
#include <init.h>
#include <driver.h>
#include <types.h>
#include <ata.h>
#include <xfuncs.h>
#include <errno.h>
#include <string.h>
#include <linux/kernel.h>
#include <malloc.h>
#include <common.h>
#include <block.h>
#include <asm/io.h>
#include <asm/byteorder.h>
#include "./ahci.h"

#define writel_with_flush(a,b)  do { writel(a,b); readl(b); } while (0)

static struct ahci_cb *glbl_ahci = NULL;

#define ATA_BLOCKSIZE	512	/* bytes */

/* different kinds of FIS specified in Serial ATA Revision 3.0. */
typedef enum
{
        FIS_TYPE_REG_H2D        = 0x27,         /* Register FIS - host to device */
        FIS_TYPE_REG_D2H        = 0x34,         /* Register FIS - device to host */
        FIS_TYPE_DMA_ACT        = 0x39,         /* DMA activate FIS - device to host */
        FIS_TYPE_DMA_SETUP      = 0x41,         /* DMA setup FIS - bidirectional */
        FIS_TYPE_DATA           = 0x46,         /* Data FIS - bidirectional */
        FIS_TYPE_BIST           = 0x58,         /* BIST activate FIS - bidirectional */
        FIS_TYPE_PIO_SETUP      = 0x5F,         /* PIO setup FIS - device to host */
        FIS_TYPE_DEV_BITS       = 0xA1,         /* Set device bits FIS - device to host */
} FIS_TYPE;


#define msleep(a) mdelay(a)
#define ssleep(a) mdelay(a * 1000)

/** 
 * This function initializes, given command slot.
 * @param *pp 	Pointer to port control block.
 * @param opts	Command options.
 */

static void ahci_fill_cmd_slot(struct ahci_ioports *pp, u32 opts)
{
	pp->cmd_slot->opts = cpu_to_le32(opts);
	pp->cmd_slot->status = 0;
	pp->cmd_slot->tbl_addr = cpu_to_le32(pp->cmd_tbl & 0xffffffff);
	pp->cmd_slot->tbl_addr_hi = 0;
}


/**
 * This function polls for currect command completion.
 * @param *offset	Address of the register to be polled.
 * @param timeout_msec	Time out in milliseconds.
 * @param sign 	        Bit mask, to be set for command completion.   
 */ 

static int waiting_for_cmd_completed(volatile u8 *offset,
				     int timeout_msec,
				     u32 sign)
{
	int i;
	u32 status;

	for (i = 0; ((status = readl(offset)) & sign) && i < timeout_msec; i++)
		msleep(1);

	return (i < timeout_msec) ? 0 : -1;
}

#define MAX_DATA_BYTE_COUNT  (4*1024*1024)

/**
 * This functions fills given buffer in scatter gather descriptors of command PRDT.
 * @param port		Port number.
 * @param *buf		Pointer to buffer.
 * @param buf_len	Length of buffer.
 */ 

static int ahci_fill_sg(u8 port, unsigned char *buf, int buf_len)
{
	struct ahci_ioports *pp = &(glbl_ahci->port[port]);
	struct ahci_sg *ahci_sg = pp->cmd_tbl_sg;
	u32 sg_count;
	int i;

	sg_count = ((buf_len - 1) / MAX_DATA_BYTE_COUNT) + 1;
	if (sg_count > AHCI_MAX_SG) {
		printf("Error:Too much sg!\n");
		return -1;
	}

	for (i = 0; i < sg_count; i++) {
		ahci_sg->addr =
		    cpu_to_le32((u32) buf + i * MAX_DATA_BYTE_COUNT);
		ahci_sg->addr_hi = 0;
		ahci_sg->flags_size = cpu_to_le32(0x3fffff &
					  (buf_len < MAX_DATA_BYTE_COUNT
					   ? (buf_len - 1)
					   : (MAX_DATA_BYTE_COUNT - 1)));
		ahci_sg++;
		buf_len -= MAX_DATA_BYTE_COUNT;
	}

	return sg_count;
}

/**
 * This function prepares read/write FISes, prepare and issue 
 * the command and waits for command completion.
 * @param ahci		Pointer to ahci driver controlblock.
 * @param block		ATA device Block number to be read/write.	   
 * @param num_blocks	Number of blocks to be read/write.
 * @param *buf		Pointer to data buffer.
 * @param write 	Block read/write. 1-write, 0-read.
 */
static int ahci_issue_command( struct ahci_cb *ahci, int block, int num_blocks, void *buf, int write )
{
	int port, buf_len, sg_count, fis_len = 20;
	u32 opts, port_status;
	struct ahci_ioports *pp;
	unsigned char fis[20];
        volatile u8 *port_mmio;
	
	port = 0; //Fixme : Right now port is hardcoded to zero.
	pp = &(ahci->port[port]);
        
        port_mmio = (u8 *) pp->port_mmio;
	
	debug("Enter get_ahci_device_data: for port %d\n", port);

        if (port > ahci->n_ports) {
                printf("Invaild port number %d\n", port);
                return -1;
        }

        port_status = readl(port_mmio + PORT_SCR_STAT);
        if ((port_status & 0xf) != 0x03) {
                printf("No Link on port %d!\n", port);
                return -1;
        }
	
	buf_len = ATA_BLOCKSIZE * num_blocks;

	/* Build READ FIS */
	memset(fis, 0, 20);

	fis[0] = FIS_TYPE_REG_H2D;
	fis[1] = 1 << 7; /* Command FIS */
	
	if( write )
		fis[2] = ATA_CMD_WR_DMA;
	else
		fis[2] = ATA_CMD_RD_DMA;
		

	/* Input block number for this function is 32-bit */
	fis[5] = block & 0xff; 	 
	fis[6] = (block >> 7) & 0xff; 	 
	fis[7] = (block >> 15) & 0xff; 	 
	fis[9] = (block >> 24) & 0xff; 

	/* Block count */
	fis[12] = num_blocks & 0xff;	 
	fis[13] = (num_blocks >> 7) & 0xff;	 

	memcpy((unsigned char *)pp->cmd_tbl, fis, fis_len);

        sg_count = ahci_fill_sg(port, buf, buf_len);
	if( write )
        	opts = (fis_len >> 2) | (sg_count << 16) | AHCI_CMD_WRITE;
	else
        	opts = (fis_len >> 2) | (sg_count << 16);
		
        ahci_fill_cmd_slot(pp, opts);

        writel_with_flush(1, port_mmio + PORT_CMD_ISSUE);

        if (waiting_for_cmd_completed(port_mmio + PORT_CMD_ISSUE, 150, 0x1)) {
                printf("timeout exit!\n");
                return -1;
        }
        debug(" %d byte transferred.\n", pp->cmd_slot->status);

	return 0;
}

/**
 * This function writes given sectors to SATA device. This API is used by
 * generic ATA frame work.
 * @param *dev		pointer to ata device.
 * @param block		Starting block.
 * @param num_blocks	Number of blocks to write.
 * @param *buf		Pointer to data buffer.
 */   
int ahci_write_data(struct device_d *dev, int block, int num_blocks, void *buf)
{
        struct ahci_cb *ahci = glbl_ahci;
	
	ahci_issue_command( ahci, block, num_blocks, buf, 1 );
        
	return 0;
}

/**
 * This function reads given sectors from SATA device. This API is used by
 * generic ATA frame work.
 * @param *dev		pointer to ata device.
 * @param block		Starting block.
 * @param num_blocks	Number of blocks to read.
 * @param *buf		Pointer to data buffer.
 */
int ahci_read_data(struct device_d *dev, int block, int num_blocks, void *buf)
{
        struct ahci_cb *ahci = glbl_ahci;

	ahci_issue_command( ahci, block, num_blocks, buf, 0 );

        return 0;
}

static inline u32 ahci_port_base(u32 base, u32 port)
{
        return base + 0x100 + (port * 0x80);
}


static void ahci_setup_port(struct ahci_ioports *port, unsigned long base,
                            unsigned int port_idx)
{
        base = ahci_port_base(base, port_idx);

        port->cmd_addr = base;
        port->scr_addr = base + PORT_SCR;
}

/**
 * This function initilize ahci host controller.
 * @param ahci		Pointer to ahci driver controlblock.
 */
static int ahci_host_init( struct ahci_cb *ahci )
{
        volatile u8 *mmio = (volatile u8 *)ahci->mmio_base;
        u32 tmp, cap_save;
        int i, j;
        volatile u8 *port_mmio;

        cap_save = readl(mmio + HOST_CAP);
        cap_save &= ((1 << 28) | (1 << 17));
        cap_save |= (1 << 27);

        /* global controller reset */
        tmp = readl(mmio + HOST_CTL);
        if ((tmp & HOST_RESET) == 0)
                writel_with_flush(tmp | HOST_RESET, mmio + HOST_CTL);

        /* reset must complete within 1 second, or
 	 * the hardware should be considered fried.
         */
        ssleep(1);

        tmp = readl(mmio + HOST_CTL);
        if (tmp & HOST_RESET) {
                printf("controller reset failed (0x%x)\n", tmp);
                return -1;
        }

        writel_with_flush(HOST_AHCI_EN, mmio + HOST_CTL);
        writel(cap_save, mmio + HOST_CAP);
        writel_with_flush(0xf, mmio + HOST_PORTS_IMPL);

        ahci->cap = readl(mmio + HOST_CAP);
        ahci->port_map = readl(mmio + HOST_PORTS_IMPL);
        ahci->n_ports = (ahci->cap & 0x1f) + 1;

        printf("cap 0x%x  port_map 0x%x  n_ports %d\n",
              ahci->cap, ahci->port_map, ahci->n_ports);

        for (i = 0; i < ahci->n_ports; i++) {
                ahci->port[i].port_mmio = ahci_port_base((u32) mmio, i);
                port_mmio = (u8 *) ahci->port[i].port_mmio;
                ahci_setup_port(&ahci->port[i], (unsigned long)mmio, i);

                /* make sure port is not active */
                tmp = readl(port_mmio + PORT_CMD);
                if (tmp & (PORT_CMD_LIST_ON | PORT_CMD_FIS_ON |
                           PORT_CMD_FIS_RX | PORT_CMD_START)) {
                        tmp &= ~(PORT_CMD_LIST_ON | PORT_CMD_FIS_ON |
                                 PORT_CMD_FIS_RX | PORT_CMD_START);
                        writel_with_flush(tmp, port_mmio + PORT_CMD);

                        /* spec says 500 msecs for each bit, so
                         * this is slightly incorrect.
                         */
                        msleep(500);
                }

                writel(PORT_CMD_SPIN_UP, port_mmio + PORT_CMD);
 		j = 0;
                while (j < 100) {
                        msleep(10);
                        tmp = readl(port_mmio + PORT_SCR_STAT);
                        if ((tmp & 0xf) == 0x3)
                                break;
                        j++;
                }

                tmp = readl(port_mmio + PORT_SCR_ERR);
                printf("PORT_SCR_ERR 0x%x\n", tmp);
                writel(tmp, port_mmio + PORT_SCR_ERR);

                /* ack any pending irq events for this port */
                tmp = readl(port_mmio + PORT_IRQ_STAT);
                debug("PORT_IRQ_STAT 0x%x\n", tmp);
                if (tmp)
                        writel(tmp, port_mmio + PORT_IRQ_STAT);

                writel(1 << i, mmio + HOST_IRQ_STAT);

                /* set irq mask (enables interrupts) */
                writel(DEF_PORT_IRQ, port_mmio + PORT_IRQ_MASK);

                /*register linkup ports */
                tmp = readl(port_mmio + PORT_SCR_STAT);
                printf("Port %d status: 0x%x\n", i, tmp);
                if ((tmp & 0xf) == 0x03)
                        ahci->link_port_map |= (0x01 << i);
        }
  	tmp = readl(mmio + HOST_CTL);
        printf("HOST_CTL 0x%x\n", tmp);
        writel(tmp | HOST_IRQ_EN, mmio + HOST_CTL);
        tmp = readl(mmio + HOST_CTL);
        printf("HOST_CTL 0x%x\n", tmp);

        return 0;
}

/**
 * This function initailize and start the given port of ahci host controller
 * @param port	Port number to be initialized.
 */
static int ahci_port_start(u8 port)
{
	struct ahci_ioports *pp = &(glbl_ahci->port[port]);
        volatile u8 *port_mmio = (volatile u8 *)pp->port_mmio;
        u32 port_status;
        u32 mem;

        printf("Enter start port: %d\n", port);
        port_status = readl(port_mmio + PORT_SCR_STAT);
        printf("Port %d status: %x\n", port, port_status);
        if ((port_status & 0xf) != 0x03) {
                printf("No Link on this port!\n");
                return -1;
        }

        mem = (u32) malloc(AHCI_PORT_PRIV_DMA_SZ + 2048);
        if (!mem) {
                free(pp);
                printf("No mem for table!\n");
                return -ENOMEM;
        }

        mem = (mem + 0x800) & (~0x7ff); /* Aligned to 2048-bytes */
        memset((u8 *) mem, 0, AHCI_PORT_PRIV_DMA_SZ);

        /*
         * First item in chunk of DMA memory: 32-slot command table,
         * 32 bytes each in size
         */
        pp->cmd_slot = (struct ahci_cmd_hdr *)mem;
        printf("cmd_slot = 0x%x\n", pp->cmd_slot);
        mem += (AHCI_CMD_SLOT_SZ + 224);

        /*
         * Second item: Received-FIS area
         */
        pp->rx_fis = mem;
        mem += AHCI_RX_FIS_SZ;

	/*
         * Third item: data area for storing a single command
         * and its scatter-gather table
         */
        pp->cmd_tbl = mem;
        printf("cmd_tbl_dma = 0x%x\n", pp->cmd_tbl);

        mem += AHCI_CMD_TBL_HDR;
        pp->cmd_tbl_sg = (struct ahci_sg *)mem;

        writel_with_flush((u32) pp->cmd_slot, port_mmio + PORT_LST_ADDR);

        writel_with_flush(pp->rx_fis, port_mmio + PORT_FIS_ADDR);

        writel_with_flush(PORT_CMD_ICC_ACTIVE | PORT_CMD_FIS_RX |
                          PORT_CMD_POWER_ON | PORT_CMD_SPIN_UP |
                          PORT_CMD_START, port_mmio + PORT_CMD);

        printf("Exit start port %d\n", port);

        return 0;

	
} 

/**
 * This function prints complete host information.
 * @param ahci		Pointer to ahci driver controlblock.
 */
 
static void ahci_print_info(struct ahci_cb *ahci)
{
	volatile u8 *mmio = (volatile u8 *)ahci->mmio_base;
	u32 vers, cap, impl, speed;
	const char *speed_s;
	const char *scc_s;

	vers = readl(mmio + HOST_VERSION);
	cap = ahci->cap;
	impl = ahci->port_map;

	speed = (cap >> 20) & 0xf;
	if (speed == 1)
		speed_s = "1.5";
	else if (speed == 2)
		speed_s = "3";
	else
		speed_s = "?";

	scc_s = "SATA";
	printf("AHCI %02x%02x.%02x%02x "
	       "%u slots %u ports %s Gbps 0x%x impl %s mode\n",
	       (vers >> 24) & 0xff,
	       (vers >> 16) & 0xff,
	       (vers >> 8) & 0xff,
	       vers & 0xff,
	       ((cap >> 8) & 0x1f) + 1, (cap & 0x1f) + 1, speed_s, impl, scc_s);

	printf("flags: "
	       "%s%s%s%s%s%s"
	       "%s%s%s%s%s%s%s\n",
	       cap & (1 << 31) ? "64bit " : "",
	       cap & (1 << 30) ? "ncq " : "",
	       cap & (1 << 28) ? "ilck " : "",
	       cap & (1 << 27) ? "stag " : "",
	       cap & (1 << 26) ? "pm " : "",
	       cap & (1 << 25) ? "led " : "",
	       cap & (1 << 24) ? "clo " : "",
	       cap & (1 << 19) ? "nz " : "",
	       cap & (1 << 18) ? "only " : "",
	       cap & (1 << 17) ? "pmp " : "",
	       cap & (1 << 15) ? "pio " : "",
	       cap & (1 << 14) ? "slum " : "",
	       cap & (1 << 13) ? "part " : "");
}
 

/**
 * This is probe function registered with platform device driver framework.
 * @param *dev		Pointer to device.
 */
static int ahci_probe(struct device_d *dev)
{
	int i;

	/* Initialize ahci control block */ 
	glbl_ahci = (struct ahci_cb *)xmalloc(sizeof(struct ahci_cb));  

	if (!glbl_ahci)
	{	
		dev_err(dev, "Unable to allocate memory ahci control block\n");
                return -ENOMEM;
	}
	
	memset(glbl_ahci, 0, sizeof(struct ahci_cb));
	dev->priv = (void *) glbl_ahci;
	glbl_ahci->mmio_base = dev->map_base; 
 
	glbl_ahci->host_flags = ATA_FLAG_SATA
                                | ATA_FLAG_NO_LEGACY
                                | ATA_FLAG_MMIO
                                | ATA_FLAG_PIO_DMA
                                | ATA_FLAG_NO_ATAPI;
        
	glbl_ahci->pio_mask = 0x1f;
	glbl_ahci->udma_mask = 0x7f;    /*Fixme,assume to support UDMA6 */

        /* initialize adapter */
        if(!ahci_host_init(glbl_ahci))
                return -1;

        ahci_print_info(glbl_ahci);

	/* Start all the ports */
        for (i = 0; i < glbl_ahci->n_ports; i++) 
                ahci_port_start(i);



        return 0;
}

static struct driver_d ahci_driver = {
        .name   = "ahci",
        .probe  = ahci_probe,
};

/**
 * This is init function of ahci driver.
 */

static int ahci_init(void)
{
	register_driver(&ahci_driver);
	return 0;
}

device_initcall(ahci_init);
