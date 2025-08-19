ENTRY(_start)

START = 0x41000000;

STACK_START = START + 0x1000000;
STACK_END   = START + 0x2000000;

HEAP_START = START + 0x2000000;
HEAP_END   = START + 0x3000000;

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
