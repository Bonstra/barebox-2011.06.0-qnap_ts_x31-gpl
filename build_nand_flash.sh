#!/bin/sh
ROM_IMAGE=tsx31_full_nand_flash.rom
KERNEL_IMAGE=uImage
ROOTFS2_IMAGE=rootfs2.ubi
CONFIG_IMAGE=config.ubi
if [ ! -e $KERNEL_IMAGE ]; then echo No $KERNEL_IMAGE; exit 1;fi
if [ ! -e $ROOTFS2_IMAGE ]; then echo No $ROOTFS2_IMAGE; exit 1;fi
if [ ! -e $CONFIG_IMAGE ]; then echo No $CONFIG_IMAGE; exit 1;fi
dd if=/dev/zero ibs=128k count=4096 | tr "\000" "\377" > $ROM_IMAGE
dd if=$KERNEL_IMAGE of=$ROM_IMAGE bs=128k conv=notrunc
dd if=$ROOTFS2_IMAGE of=$ROM_IMAGE bs=128k seek=256 conv=notrunc
dd if=$KERNEL_IMAGE of=$ROM_IMAGE bs=128k seek=1984 conv=notrunc
dd if=$ROOTFS2_IMAGE of=$ROM_IMAGE bs=128k seek=2240 conv=notrunc
dd if=$CONFIG_IMAGE of=$ROM_IMAGE bs=128k seek=3968 conv=notrunc

echo Prepare : $ROM_IMAGE COMPLETE..
