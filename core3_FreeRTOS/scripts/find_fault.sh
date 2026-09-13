#!/bin/bash

qemu-system-aarch64 -M virt -cpu cortex-a72 -m 2G \
    -object memory-backend-file,id=mb,size=2G,mem-path=/dev/shm/qemu_amp_shmem,share=on \
    -numa node,memdev=mb \
    -nographic -kernel build/kernel.elf -d int,guest_errors -D qemu.log