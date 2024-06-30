#!/bin/sh

qemu-system-ppc -M pegasos2 -bios bootrom_test.bin -display none -serial stdio -d "unimp,trace:mv64361_reg_read,trace:mv64361_reg_write" -s -S

