	.global _start
	.global putchar

	.text
_start:
	bl setup_sio
	bl setup_stack
	bl main
	b forever

forever:
	b forever

/* Setup SIO to make serial usable */
setup_sio:
	/* Write to PCI configuration register */ 
	/* r1 = 0xf1000c78 */
	/* r2 = 0x80006050 */
	lis %r1, 0xf100
	ori %r1, %r1, 0xc78
	
	li %r2, 0x5060
	slwi %r2, %r2, 16
	ori %r2, %r2, 0x0080
	stw %r2, 0(%r1)
	
	/* r1 = 0xf1000c7c */
	/* r2 = 0x6 */
	
	lis %r1, 0xf100
	ori %r1, %r1, 0xc7c

	li %r2, 0x600
	slwi %r2, %r2, 16
	stw %r2, 0(%r1)
	
	/* Enable SIO */
	/* r1 = 0xfe0003f0 */
	/* r2 = 0xf4, r3 = 0xbe */
	lis %r1, -0x200
	ori %r1, %r1, 0x3f0

	li %r2, 0xf4
	li %r3, 0xbe
	stb %r2, 0(%r1)
	stb %r3, 1(%r1)

	/* r2 = 0xf2, r3 = 0x7 */
	li %r2, 0xf2
	li %r3, 0x7
	stb %r2, 0(%r1)
	stb %r3, 1(%r1)
	
	eieio
	blr

setup_stack:
	li %r1, 0x1000
	slwi %r1, %r1, 16
	blr

/* Print out a character */
putchar:
	/* r4 = 0xfe0002f8 */
	/* r3 = char */
	lis %r4, -0x200
	ori %r4, %r4, 0x2f8
	stb %r3, 0(%r4)
	blr

/* Read a bunch of register on the SOC */
test_read_soc:
	/* r1 = 0xf1000000 */
	xor %r1, %r1, %r1
	ori %r1, %r1, 0xf100
	slwi %r1, %r1, 16
	ori %r1, %r1, 0x0

	/* r2 = r1 + 0x10000 */
	xor %r2, %r2, %r2
	xor %r2, %r1, %r2
	addi %r2, %r2, 0x4000
	addi %r2, %r2, 0x4000
	addi %r2, %r2, 0x2000

1:
	lwz %r3, 0(%r1)
	addi %r1, %r1, 4
	/* cr0 = r1 == r2 */
	cmpw %cr0, %r1, %r2
	bne 1b
	blr

