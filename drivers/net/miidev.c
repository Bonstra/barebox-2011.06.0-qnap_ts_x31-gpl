/*
 * miidev.c - generic phy abstraction
 *
 * Copyright (c) 2007 Sascha Hauer <s.hauer@pengutronix.de>, Pengutronix
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <common.h>
#include <driver.h>
#include <init.h>
#include <miidev.h>
#include <clock.h>
#include <net.h>
#include <malloc.h>

#define DEBUG_PORT_ADDRESS          29
#define DEBUG_PORT_DATA             30

static inline int phy_debug_read(struct mii_device *mdev, unsigned int phy_addr, unsigned int reg_addr)
{
        mdev->write(mdev, phy_addr, DEBUG_PORT_ADDRESS, reg_addr);
        return (unsigned int) mdev->read(mdev, phy_addr, DEBUG_PORT_DATA);
}

static inline void phy_debug_write(struct mii_device *mdev, unsigned int phy_addr, unsigned int reg_addr, unsigned int reg_val)
{
        mdev->write(mdev, phy_addr, DEBUG_PORT_ADDRESS, reg_addr);
        mdev->write(mdev, phy_addr, DEBUG_PORT_DATA, reg_val);
}


void miidev_enable_rgmii_rx_delay(struct mii_device *mdev)
{
	uint16_t val;
        //enable RxClk delay
        val = phy_debug_read(mdev, mdev->address, 0x0);
        val |= 0x8000;
        phy_debug_write(mdev, mdev->address, 0x0, val);
}

void miidev_enable_rgmii_tx_delay(struct mii_device *mdev)
{
        uint16_t val;
        //enable TxClk delay
        val = phy_debug_read(mdev, mdev->address, 0x5);
        val |= 0x100;
        phy_debug_write(mdev, mdev->address, 0x5, val);
}


int miidev_restart_aneg(struct mii_device *mdev)
{
	uint16_t status, btcr;
	int timeout;

	printf("%s for PHY%d\n", __func__, mdev->address);

	/*
	 * Reset PHY, then delay 300ns
	 */
	mii_write(mdev, mdev->address, MII_BMCR, BMCR_RESET);

	if (mdev->flags & MIIDEV_FORCE_LINK)
		return 0;

	udelay(1000);

	if (mdev->flags & MIIDEV_FORCE_10) {
		printf("Forcing 10 Mbps ethernet link... ");
		status = mii_read(mdev, mdev->address, MII_BMSR);
		mii_write(mdev, mdev->address, MII_BMCR, BMCR_FULLDPLX | BMCR_CTST);

		timeout = 20;
		do {	/* wait for link status to go down */
			udelay(10000);
			if ((timeout--) == 0) {
				debug("hmmm, should not have waited...");
				break;
			}
			status = mii_read(mdev, mdev->address, MII_BMSR);
		} while (status & BMSR_LSTATUS);

	} else {	/* MII100 */
		/*
		 * Set the auto-negotiation advertisement register bits
		 */
		status = mii_read(mdev, mdev->address, MII_ADVERTISE);
		status |= ADVERTISE_ALL;
		mii_write(mdev, mdev->address, MII_ADVERTISE, status);

		 if (miidev_supports_1000base_t(mdev)) {
			 btcr = mii_read(mdev, mdev->address, MII_CTRL1000);
			 btcr |= ADVERTISE_1000FULL | ADVERTISE_1000HALF;
			 mii_write(mdev, mdev->address, MII_CTRL1000, btcr);
		 }

		mii_write(mdev, mdev->address, MII_BMCR, BMCR_ANENABLE | BMCR_ANRESTART);
	}

	return 0;
}

int miidev_wait_aneg(struct mii_device *mdev)
{
	uint64_t start;
	int status;

	if (mdev->flags & MIIDEV_FORCE_LINK)
		return 0;

	/*
	 * Wait for AN completion
	 */
	start = get_time_ns();
	do {
		if (is_timeout(start, 5 * SECOND)) {
			printf("%s: Autonegotiation timeout\n", mdev->cdev.name);
			return -1;
		}

		status = mii_read(mdev, mdev->address, MII_BMSR);
		if (status < 0) {
			printf("%s: Autonegotiation failed. status: 0x%04x\n", mdev->cdev.name, status);
			return -1;
		}
	} while (!(status & BMSR_ANEGCOMPLETE));

	return 0;
}

int miidev_print_status(struct mii_device *mdev)
{
	int bmsr, bmcr, lpa;
	char *duplex;
	int speed;

	if (mdev->flags & MIIDEV_FORCE_LINK) {
		printf("Forcing link present...\n");
		return 0;
	}

	bmsr = mii_read(mdev, mdev->address, MII_BMSR);
	if (bmsr < 0)
		goto err_out;
	bmcr = mii_read(mdev, mdev->address, MII_BMCR);
	if (bmcr < 0)
		goto err_out;
	lpa = mii_read(mdev, mdev->address, MII_LPA);
	if (lpa < 0)
		goto err_out;

	printf("%s: Link is %s", mdev->cdev.name,
			bmsr & BMSR_LSTATUS ? "up" : "down");

	if (bmcr & BMCR_ANENABLE) {
		duplex = lpa & LPA_DUPLEX ? "Full" : "Half";
		speed = lpa & LPA_100 ? 100 : 10;
	} else {
		duplex = bmcr & BMCR_FULLDPLX ? "Full" : "Half";
		speed = bmcr & BMCR_SPEED100 ? 100 : 10;
	}

	printf(" - %d/%s\n", speed, duplex);

	return 0;
err_out:
	printf("%s: failed to read\n", mdev->cdev.name);
	return -1;
}

int miidev_supports_1000base_t(struct mii_device *mdev)
{
        unsigned short reg;

        reg = mii_read(mdev, mdev->address, MII_BMSR);
        if (reg < 0) {
                printf("PHY bmsr read failed, assuming no 1000bT support\n");
                return (0);
        }

        if (reg & BMSR_ESTATEN) {
				reg = mii_read(mdev, mdev->address, MII_ESTATUS);
                if (reg < 0) {
                        printf("PHY exsr read failed, assuming no 1000bT support\n");
                        return (0);
                }

                if (reg & (ESTATUS_1000_TFULL | ESTATUS_1000_THALF))
                        return (1);
        }

        return (0);
}

int miidev_speed_duplex(struct mii_device *mdev, int *speed, int *duplex)
{
        unsigned short bmcr, btcr, btsr, anlpar, anar;

        /* Check Basic Management Control Register first. */
        bmcr = mii_read(mdev, mdev->address, MII_BMCR);
        if (bmcr < 0) {
                printf("PHY bmcr read failed\n");
                return -1;
        }

        /* Check if auto-negotiation is on. */
        if (bmcr & BMCR_ANENABLE) {
                if (miidev_wait_aneg(mdev)) {
                        printf("PHY auto-negotiation error\n");
                        return -1;
                }

                if (miidev_supports_1000base_t(mdev)) {
						btcr = mii_read(mdev, mdev->address, MII_CTRL1000);
                        if (btcr < 0) {
                                printf("PHY btcr read failed\n");
                                return -1;
                        }

                        btsr = mii_read(mdev, mdev->address, MII_STAT1000);
                        if (btsr < 0) {
                                printf("PHY btsr read failed\n");
                                return -1;
                        }

                        if ((btcr & ADVERTISE_1000FULL) && (btsr & LPA_1000FULL)) {
                                *speed = MII_SPEED_1000M;
                                *duplex = MII_DUPLEX_FULL;
                                return 0;
                        }

                        if ((btcr & ADVERTISE_1000HALF) && (btsr & LPA_1000HALF)) {
                                *speed = MII_SPEED_1000M;
                                *duplex = MII_DUPLEX_HALF;
                                return 0;
                        }
                }

                /* Get link partner abilities results. */
                anlpar = mii_read(mdev, mdev->address, MII_LPA);
                if (anlpar < 0) {
                        printf("PHY anlpar read failed\n");
                        return -1;
                }

                /* Get advertised abilities. */
                anar = mii_read(mdev, mdev->address, MII_ADVERTISE);
                if (anar < 0) {
                        printf("PHY anar register read failed\n");
                        return -1;
                }

                if ((anlpar & anar & ADVERTISE_100FULL)) {
                        *speed = MII_SPEED_100M;
                        *duplex = MII_DUPLEX_FULL;
                } else if ((anlpar & anar & ADVERTISE_100HALF)) {
                        *speed = MII_SPEED_100M;
                        *duplex = MII_DUPLEX_HALF;
                } else if ((anlpar & anar & ADVERTISE_10FULL)) {
                        *speed = MII_SPEED_10M;
                        *duplex = MII_DUPLEX_FULL;
                } else {
                        *speed = MII_SPEED_10M;
                        *duplex = MII_DUPLEX_HALF;
                }

                return 0;
        }

        /* Get speed from basic control settings. */
        if ((bmcr & BMCR_SPEEDMASK) == BMCR_SPEED1000)
                *speed = MII_SPEED_1000M;
        else if ((bmcr & BMCR_SPEEDMASK) == BMCR_SPEED100)
                *speed = MII_SPEED_100M;
        else
                *speed = MII_SPEED_10M;

        if (bmcr & BMCR_FULLDPLX)
                *duplex = MII_DUPLEX_FULL;
        else
                *duplex = MII_DUPLEX_HALF;

        return 0;
}

void miidev_print_speed_duplex(struct mii_device *mdev, int speed, int duplex)
{
	char *speed_str, *duplex_str;

	switch (speed) {
	case MII_SPEED_10M:
		speed_str = "10";
		break;
	case MII_SPEED_100M:
		speed_str = "100";
		break;
	case MII_SPEED_1000M:
		speed_str = "1000";
		break;
	case MII_SPEED_1000M_PCS:
		speed_str = "1000_PCS";
		break;
	default:
		speed_str ="UNKNOWN";
	}

	if (duplex == MII_DUPLEX_FULL)
		duplex_str = "FULL";
	else
		duplex_str = "HALF";

	printf ("%s: Link is %s/%s\n", mdev->cdev.name, speed_str, duplex_str);
}

static ssize_t miidev_read(struct cdev *cdev, void *_buf, size_t count, ulong offset, ulong flags)
{
	int i = count;
	uint16_t *buf = _buf;
	struct mii_device *mdev = cdev->priv;

	while (i > 1) {
		*buf = mii_read(mdev, mdev->address, offset);
		buf++;
		i -= 2;
		offset++;
	}

	return count;
}

static ssize_t miidev_write(struct cdev *cdev, const void *_buf, size_t count, ulong offset, ulong flags)
{
	int i = count;
	const uint16_t *buf = _buf;
	struct mii_device *mdev = cdev->priv;

	while (i > 1) {
		mii_write(mdev, mdev->address, offset, *buf);
		buf++;
		i -= 2;
		offset++;
	}

	return count;
}

static struct file_operations miidev_ops = {
	.read  = miidev_read,
	.write = miidev_write,
	.lseek = dev_lseek_default,
};

static int miidev_probe(struct device_d *dev)
{
	struct mii_device *mdev = dev->priv;

	mdev->cdev.name = asprintf("phy%d", dev->id);
	mdev->cdev.size = 64;
	mdev->cdev.ops = &miidev_ops;
	mdev->cdev.priv = mdev;
	mdev->cdev.dev = dev;
	devfs_create(&mdev->cdev);
	return 0;
}

static void miidev_remove(struct device_d *dev)
{
	struct mii_device *mdev = dev->priv;

	free(mdev->cdev.name);
	devfs_remove(&mdev->cdev);
}

static struct driver_d miidev_drv = {
        .name  = "miidev",
        .probe = miidev_probe,
	.remove = miidev_remove,
};

int mii_register(struct mii_device *mdev)
{
	mdev->dev.priv = mdev;
	mdev->dev.id = -1;
	strcpy(mdev->dev.name, "miidev");

	return register_device(&mdev->dev);
}

void mii_unregister(struct mii_device *mdev)
{
	unregister_device(&mdev->dev);
}

static int miidev_init(void)
{
	register_driver(&miidev_drv);
	return 0;
}

device_initcall(miidev_init);

