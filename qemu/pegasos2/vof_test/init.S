/*

r1: Stack provided by OpenFirmware
r5: OpenFirmware client entry point
r6: Arguments
r7: Arguments length

*/

	.global start

	.text

start:
	bl vof_test

