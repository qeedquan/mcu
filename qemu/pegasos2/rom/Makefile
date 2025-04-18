ENTRY=0x410c

all:
	go run rom_entry.go -e $(ENTRY) pegasos2.rom pegasos2_relocated.rom
	powerpc-linux-objcopy --input-target=binary --output-target=elf32-powerpc pegasos2_relocated.rom pegasos2_relocated.elf

clean:
	rm -f pegasos2_relocated.*

