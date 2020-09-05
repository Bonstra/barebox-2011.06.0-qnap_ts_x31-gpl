#!/bin/sh
ROM_IMAGE=tsx31_spi_loader_12_xxxx.rom
ULOADER_IMAGE=uloader_12.bin
BAREBOX_IMAGE=barebox.bin
if [ ! -e $ULOADER_IMAGE ]; then echo No $ULOADER_IMAGE; exit 1;fi
if [ ! -e $BAREBOX_IMAGE ]; then echo No $BAREBOX_IMAGE; exit 1;fi
if [ ! -e $ENV_IMAGE ]; then echo No $ENV_IMAGE; exit 1;fi
dd if=/dev/zero ibs=1k count=640 | tr "\000" "\377" > $ROM_IMAGE
dd if=$ULOADER_IMAGE of=$ROM_IMAGE conv=notrunc
dd if=$BAREBOX_IMAGE of=$ROM_IMAGE bs=1k seek=128 conv=notrunc
