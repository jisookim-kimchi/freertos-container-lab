CC = aarch64-linux-gnu-gcc
CFLAGS = -static -O2
PATH := $(HOME)/.local/bin:$(PATH)

all: initrd.img

rootfs/init: main.c ring_buffer.c ring_buffer.h
	@mkdir -p rootfs/bin rootfs/dev rootfs/proc rootfs/sys
	$(CC) $(CFLAGS) main.c ring_buffer.c -o rootfs/init

initrd.img: rootfs/init
	@cd rootfs && find . | cpio -o -H newc | gzip -9 > ../initrd.img

run: all
	qemu-system-aarch64 -M virt -cpu cortex-a72 -m 2G -object memory-backend-file,id=mb,size=2G,mem-path=/dev/shm/qemu_amp_shmem,share=on -numa node,memdev=mb -smp 2 -nographic -kernel Image -initrd initrd.img -append "console=ttyAMA0 earlycon mem=992M"

clean:
	rm -rf rootfs/init initrd.img