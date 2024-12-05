BIN=vof_test

CC = powerpc-linux-gnu-gcc
LD = powerpc-linux-ld

CFLAGS = -ffreestanding -nostdlib -fno-exceptions -fno-asynchronous-unwind-tables -ggdb -g3

all:
	$(CC) $(CFLAGS) -c *.S *.c
	$(LD) -T link.ld -o $(BIN).elf *.o

clean:
	rm -f *.elf *.bin *.o

