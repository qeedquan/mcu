.macro ventry label
.align  2
	b \label
.endm

.section .vectors
	.global _vectors

_vectors:
    ventry _start // 0x0  Reset
    ventry _start // 0x4  Undefined Instruction
    ventry _start // 0x8  Software Interrupt
    ventry _start // 0xC  Prefetch Abort
    ventry _start // 0x10 Data Abort
    ventry _start // 0x14 Reserved
    ventry _start // 0x18 IRQ
    ventry _start // 0x1C FIQ

.section .text
	.globl rdcntfreq
	.globl rdcntlpct
	.globl rdvbar
	.globl wrvbar
	.globl rdcpsr
	.globl wrcpsr

rdcntfreq:
	mrc p15, 0, r0, c14, c0, 0
	bx lr

rdcntlpct:
	mrrc p15, 0, r0, r1, c14
	bx lr

rdvbar:
	mrc p15, 0, r0, c12, c0, 0
	bx lr

wrvbar:
	mcr p15, 0, r0, c12, c0, 0
	bx lr

rdcpsr:
	mrs r0, cpsr
	bx lr

wrcpsr:
	msr cpsr, r0
	bx lr
