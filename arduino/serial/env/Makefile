MCU=atmega328p
F_CPU=16000000
BAUD=38400
#MCU=attiny13
#F_CPU=1200000
PROGGY=arduino
PORT=/dev/ttyACM2
CC=avr-gcc
OBJCOPY=avr-objcopy
SIZE=avr-size
CFLAGS=-std=c99 -Wall -g -Os -mmcu=${MCU} -DF_CPU=${F_CPU} -DBAUD=${BAUD} -I.
TARGET=env
SRCS=env.c

all:
	${CC} ${CFLAGS} -o ${TARGET}.elf ${SRCS}
	${OBJCOPY} -j .text -j .data -O ihex ${TARGET}.elf ${TARGET}.hex
	${OBJCOPY} -I ihex -O binary ${TARGET}.hex ${TARGET}.bin
	${SIZE} ${TARGET}.elf

flash:
	avrdude -p ${MCU} -c ${PROGGY} -U flash:w:${TARGET}.hex:i -F -P ${PORT}

clean:
	rm -f *.bin *.hex *.elf
