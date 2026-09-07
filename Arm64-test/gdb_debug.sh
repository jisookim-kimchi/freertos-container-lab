#!/bin/bash

qemu-system-aarch64 -M virt -cpu cortex-a72 -nographic -kernel kernel.elf -s -S