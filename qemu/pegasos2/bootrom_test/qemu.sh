#!/bin/sh

BIOS="-bios bootrom.bin"
MACHINE="-M pegasos2"
TRACE="-d unimp,trace:mv64361_reg_write,trace:via_superio_write,trace:vof*"
DISPLAY="-display none -device VGA,romfile="
SERIAL="-serial stdio"
DEBUG="-s -S"

qemu-system-ppc $MACHINE $BIOS $DISPLAY $SERIAL $TRACE $DEBUG

