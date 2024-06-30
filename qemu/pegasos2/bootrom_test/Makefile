BIN=bootrom_test

all:
	powerpc-linux-as $(BIN).s -o $(BIN).o
	powerpc-linux-ld -T $(BIN).ld $(BIN).o -o $(BIN).bin
	powerpc-linux-objcopy --input-target=binary --output-target=elf32-powerpc $(BIN).bin $(BIN).elf

clean:
	rm -f *.elf *.bin *.o

