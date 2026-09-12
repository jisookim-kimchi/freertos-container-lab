#!/bin/bash
set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
ROOT_DIR="$SCRIPT_DIR/.."
BUILD_DIR="$ROOT_DIR/build"

mkdir -p "$BUILD_DIR"

aarch64-linux-gnu-gcc -march=armv8.1-a -nostdlib -ffreestanding -g \
    -I"$ROOT_DIR" \
    -T "$ROOT_DIR/arch/ARM64/linker.ld" \
    "$ROOT_DIR/arch/ARM64/start.S" \
    "$ROOT_DIR/arch/ARM64/mmu.S" \
    "$ROOT_DIR/arch/ARM64/atomic_cas.S" \
    "$ROOT_DIR/arch/ARM64/cycle_counter.S" \
    "$ROOT_DIR/utils/print.c" \
    "$ROOT_DIR/ipc/memory.c" \
    "$ROOT_DIR/ipc/ring_buffer.c" \
    "$ROOT_DIR/ipc/ring_buffer.S" \
    "$ROOT_DIR/main/main.c" \
    -o "$BUILD_DIR/kernel.elf"

echo "[Build Complete] Output -> build/kernel.elf"