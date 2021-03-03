#!/bin/sh

gcc -c -g -Os -m32 -march=i686 -ffreestanding -Wall donut.c -o donut.o
ld -m elf_i386 -static -Tdonut.ld -nostdlib --nmagic -o donut.elf donut.o
objcopy -O binary donut.elf donut.bin
