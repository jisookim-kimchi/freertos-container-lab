#!/bin/bash

qemu-system-aarch64 -M virt -cpu cortex-a72 -machine dumpdtb=virt.dtb
dtc -I dtb -O dts -o virt.dts virt.dtb