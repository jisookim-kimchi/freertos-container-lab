#!/bin/bash
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
ROOT_DIR="$SCRIPT_DIR/.."

qemu-system-aarch64 -M virt -cpu cortex-a72 -nographic -kernel "$ROOT_DIR/build/kernel.elf"