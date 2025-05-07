AS = powerpc64-linux-gnu-as
CC = powerpc64-linux-gnu-gcc
LD = powerpc64-linux-gnu-ld

CFLAGS = -ffreestanding -nostdlib
LDFLAGS = -Wl,-Ttext=200000,-e init

BIN=test.elf

all:
	$(CC) $(CFLAGS) -c *.c
	$(AS) -o init.o *.s
	$(CC) $(LDFLAGS) -o $(BIN) *.o

clean:
	rm -f *.elf *.o

qemu:
	qemu-system-ppc64 -cpu e6500 -nographic -m 1G -M ppce500 -kernel $(BIN) -serial mon:stdio -d in_asm,nochain -singlestep

