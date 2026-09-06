#!/bin/bash
set -e

aarch64-linux-gnu-gcc -nostdlib -ffreestanding -g -T linker.ld Start.S main.c -o kernel.elf

qemu-system-aarch64 -M virt -cpu cortex-a53 -nographic -kernel kernel.elf
