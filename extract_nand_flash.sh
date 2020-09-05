#!/bin/sh
ROM_IMAGE=tsx31_full_nand_flash_xxx.rom
KERNEL_IMAGE=uImage
ROOTFS2_IMAGE=rootfs2.ubi
CONFIG_IMAGE=config.ubi
if [ ! -e $ROM_IMAGE ]; then echo No $ROM_IMAGE; exit 1;fi
dd if=${ROM_IMAGE} of=$KERNEL_IMAGE bs=128k count=240
dd if=${ROM_IMAGE} of=$ROOTFS2_IMAGE bs=128k skip=256 count=1600
dd if=${ROM_IMAGE} of=$CONFIG_IMAGE bs=128k skip=3968 count=104

echo Prepare $KERNEL_IMAGE,$ROOTFS2_IMAGE,$CONFIG_IMAGE COMPLETE..
