#!/bin/bash
set -e

aarch64-linux-gnu-gcc -nostdlib -ffreestanding -g -T linker.ld Start.s mmu.s ../container/memory.c ../container/ring_buffer.c ../container/ring_buffer.S main.c -o kernel.elf
