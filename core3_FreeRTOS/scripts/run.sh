#!/bin/bash
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
ROOT_DIR="$SCRIPT_DIR/.."

qemu-system-aarch64 -M virt -cpu cortex-a72 -m 2G \
    -object memory-backend-file,id=mb,size=2G,mem-path=/dev/shm/qemu_amp_shmem,share=on \
    -numa node,memdev=mb \
    -nographic -kernel "$ROOT_DIR/build/kernel.elf"