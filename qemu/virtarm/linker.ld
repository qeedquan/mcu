ENTRY(_start)

START = 0x41000000;

STACK_START = 0x41000000;
STACK_END = 0x42000000;

HEAP_START = 0x42000000;
HEAP_END = 0x43000000;

SECTIONS
{
	. = START;
	.text :
	{
		*(.vectors)
		*(.text)
	}
	. = ALIGN(4096);

	.data :
	{
		*(.data)
	}
	. = ALIGN(4096);

	.bss :
	{
		__bss_start__ = . ;
		*(.bss)
		*(COMMON)
		__bss_end__ = . ;
	}
	. = ALIGN(4096);

	/* grows down */
	__stack_start__ = STACK_START;
	__stack_end__ = STACK_END;
	__stack = STACK_END;

	__heap_start__ = HEAP_START;
	__heap_end__ = HEAP_END;
}
