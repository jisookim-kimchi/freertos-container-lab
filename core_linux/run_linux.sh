#!/bin/bash

qemu-system-aarch64 \
    -M virt \
    -cpu cortex-a72 \
    -m 2G \
    -dtb ../virt.dtb \
    -object memory-backend-file,id=mb,size=2G,mem-path=/dev/shm/qemu_amp_shmem,share=on \
    -numa node,memdev=mb \
    -smp 2 \
    -nographic \
    -kernel Image \
    -initrd initrd.img \
    -append "console=ttyAMA0 earlycon mem=992M"