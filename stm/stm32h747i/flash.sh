#!/bin/sh

set -e

if [ -z "$STDIR" ]; then
	echo "STDIR environment variable needs to be set"
	exit 1
fi

STP="$STDIR/STM32_Programmer.sh"
CHIP="STM32H747I"
MAIN_BOARD="-DISCO"
BOARD_ID="0"
EXT_LOADER="MT25TL01G_$CHIP$MAIN_BOARD"
FLASH_LOADER="$STDIR/ExternalLoader/$EXT_LOADER.stldr"

if [ $# -ne 1 ]; then
	echo "usage: <hexfile>"
	exit 1
fi

echo "Flashing file $1"
$STP -c port=SWD index=$BOARD_ID reset=HWrst -el $FLASH_LOADER -e all -d $1 -HardRst
