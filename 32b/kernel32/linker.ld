ENTRY (_loader)


SECTIONS
{

	.text 0x100000:
	{
		*(.text)
			*(.rodata)
	}

	.data ALIGN (0x1000) :
	{
		*(.data)
	}

	.bss :
	{
		_sbss = .;
		*(COMMON)
			*(.bss)
			_ebss = .;
	}
}
