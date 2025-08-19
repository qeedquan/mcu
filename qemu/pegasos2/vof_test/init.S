/*

r1: Stack provided by OpenFirmware
r5: OpenFirmware client entry point
r6: Arguments
r7: Arguments length

*/

	.global start
	.global reboot

	.text

start:
	// save registers for reboot
	// a real system will use these registers at some point
	// so the right way is to save it in a reserved memory location
	// but we can cheat here since this is a test
	mr %r20, %r1
	mr %r21, %r2 
	mr %r22, %r3
	mr %r23, %r4
	mr %r24, %r5
	mr %r25, %r6
	mr %r26, %r7
	bl vof_test

reboot:
	mr %r1, %r20
	mr %r2, %r21
	mr %r3, %r22
	mr %r4, %r23
	mr %r5, %r24
	mr %r6, %r25
	mr %r7, %r26
	b start
