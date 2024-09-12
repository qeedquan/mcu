/* OUTPUT_FORMAT(elf32-powerpc) */
OUTPUT_FORMAT(binary)

MEMORY 
{
	ROM1(rx): ORIGIN = 0x0000, LENGTH = 0x100
	ROM2(rx): ORIGIN = 0x0100, LENGTH = 0x7f00
}

SECTIONS
{
	.dummy :
	{
		BYTE(0);
		. = ORIGIN(ROM1) + LENGTH(ROM1);
	} >ROM1

	.text :
	{
		*(.text)
		. = ORIGIN(ROM2) + LENGTH(ROM2);
	} >ROM2

	/DISCARD/ : {
		*(.comment)
	}
}
