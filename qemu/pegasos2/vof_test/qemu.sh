#!/bin/sh

KERNEL="-kernel vof_test.elf"
MACHINE="-M pegasos2"
TRACE="-d unimp,trace:mv64361_reg_write,trace:via_superio_write,trace:vof*,trace:rtas*"
DISPLAY="-display none -device VGA,romfile="
SERIAL="-serial stdio"
#DEBUG="-s -S"

qemu-system-ppc $MACHINE $KERNEL $DISPLAY $SERIAL $TRACE $DEBUG
#gdb --args qemu-system-ppc $MACHINE $KERNEL $DISPLAY $SERIAL $TRACE
