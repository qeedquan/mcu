IMAGE := kernel.elf

CROSS = arm-none-eabi
CC = ${CROSS}-gcc
AS = ${CROSS}-as
LD = ${CROSS}-ld

CFLAGS = -march=armv7-a -Wall -pedantic -ggdb -g3 -nostdlib -ffreestanding

QEMU = qemu-system-arm

OBJS = arch.o main.o syscall.o gic.o

all: $(IMAGE)

${IMAGE}: linker.ld ${OBJS}
	$(CC) -Wl,-T $^ -o $@

%.o : %.S
	$(CC) ${CFLAGS} -c $< -o $@

%.o : %.c
	$(CC) ${CFLAGS} -c $<

qemu:
	$(QEMU) -cpu cortex-a15 -machine virt -m 2G -serial stdio -display none -kernel $(IMAGE) #-s -S

clean:
	rm -f *.elf *.o
