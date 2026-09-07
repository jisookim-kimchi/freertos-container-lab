#!/bin/bash
set -e

aarch64-linux-gnu-gcc -nostdlib -ffreestanding -g -T linker.ld Start.S mmu.S ../container/memory.c main.c -o kernel.elf
