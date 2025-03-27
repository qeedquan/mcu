void putchar(int);

void
main()
{
	int i;
	
	for (i = 0; i < 26; i++) {
		putchar('A' + i);
		putchar('\n');
	}
}
