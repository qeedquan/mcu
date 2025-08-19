IMAGE := kernel.elf

CROSS = arm-none-eabi
CC = ${CROSS}-gcc
AS = ${CROSS}-as
LD = ${CROSS}-ld

CFLAGS = -march=armv6 -Wall -pedantic -ggdb -g3 -nostdlib -ffreestanding

QEMU = qemu-system-arm

OBJS = arch.o main.o syscall.o gpio.o

all: $(IMAGE)

${IMAGE}: linker.ld ${OBJS}
	$(CC) -Wl,-T $^ -o $@

%.o : %.S
	$(CC) ${CFLAGS} -c $< -o $@

%.o : %.c
	$(CC) ${CFLAGS} -c $<

qemu: $(IMAGE)
	$(QEMU) -cpu cortex-a15 -machine versatilepb -serial stdio -display none -kernel $(IMAGE) \
		-d trace:pl061* #-d trace:memory_region_ops_read #-s -S

clean:
	rm -f *.elf *.o
