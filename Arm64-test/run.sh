#!/bin/bash
set -e
./build.sh

qemu-system-aarch64 -M virt -cpu cortex-a72 -nographic -kernel kernel.elf