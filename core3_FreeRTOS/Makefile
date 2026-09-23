CC = aarch64-linux-gnu-gcc
CFLAGS = -march=armv8.1-a -nostdlib -ffreestanding -g -I.
LDFLAGS = -T arch/ARM64/linker.ld

SRCS = arch/ARM64/start.S \
       arch/ARM64/mmu.S \
       arch/ARM64/atomic_cas.S \
       arch/ARM64/cycle_counter.S \
       interrupt/GIC.S \
       interrupt/GIC.c \
       interrupt/irq.c \
       interrupt/irq.S \
       interrupt/timer/timer.S \
       interrupt/timer/timer.c \
       exception/exception.S \
       exception/exception.c \
       exception/vector.S \
       utils/print.c \
       ipc/memory.c \
       ipc/ring_buffer.c \
       ipc/ring_buffer.S \
       ipc/ipc_interrupt_handler.c \
       main/main.c

BUILD_DIR = build
TARGET = $(BUILD_DIR)/kernel.elf

all: $(TARGET)

$(TARGET): $(SRCS)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRCS) -o $(TARGET)

run: all
	qemu-system-aarch64 -M virt -cpu cortex-a72 -m 2G -object memory-backend-file,id=mb,size=2G,mem-path=/dev/shm/qemu_amp_shmem,share=on -numa node,memdev=mb -nographic -kernel $(TARGET)

debug: all
	qemu-system-aarch64 -M virt -cpu cortex-a72 -m 2G -object memory-backend-file,id=mb,size=2G,mem-path=/dev/shm/qemu_amp_shmem,share=on -numa node,memdev=mb -nographic -kernel $(TARGET) -s -S

clean:
	rm -rf $(BUILD_DIR)