BIN=bootrom

CC = powerpc-linux-gnu-gcc
LD = powerpc-linux-ld
OBJCOPY = powerpc-linux-objcopy

CFLAGS = -ffreestanding -nostdlib -fno-exceptions -fno-asynchronous-unwind-tables 

all:
	$(CC) -c init.S
	$(CC) $(CFLAGS) -c main.c
	$(LD) -T link.ld init.o main.o -o $(BIN).bin
	$(OBJCOPY) --input-target=binary --output-target=elf32-powerpc $(BIN).bin $(BIN).elf

clean:
	rm -f *.elf *.bin *.o

