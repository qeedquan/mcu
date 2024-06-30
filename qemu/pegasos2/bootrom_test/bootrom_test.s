	.global main

	.text
main:
	bl test_read_soc
	bl test_serial
	b forever

forever:
	b forever

/* Print 'A's */
test_serial:
	/* r6 = 0xfe0002f8 */
	/* r5 = 'a' */
	lis %r6, -0x200
	ori %r6, %r6, 0x2f8
	li %r5, 'A'
	stb %r5, 0(%r6)
	stb %r5, 0(%r6)
	stb %r5, 0(%r6)
	stb %r5, 0(%r6)
	stb %r5, 0(%r6)
	stb %r5, 0(%r6)
	stb %r5, 0(%r6)
	stb %r5, 0(%r6)
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

