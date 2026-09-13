#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/mount.h>
#include "ring_buffer.h"

#define SHARED_BASE_ADDR        0x7E000000
#define SHARED_TOTAL_SIZE       0x10000
#define BUFFER_SIZE             0x8000
#define LINUX_WRITE_ONLY_ADDR   0x7E000000
#define LINUX_READ_ONLY_ADDR    0x7E008000

int main(void)
{
    mount("proc", "/proc", "proc", 0, NULL);
    mount("sysfs", "/sys", "sysfs", 0, NULL);
    mount("devtmpfs", "/dev", "devtmpfs", 0, NULL);

    int console_fd = open("/dev/console", O_RDWR);
    if (console_fd >= 0)
    {
        dup2(console_fd, 0);
        dup2(console_fd, 1);
        dup2(console_fd, 2);
    }

    int fd = open("/dev/mem", O_RDWR | O_SYNC);
    if (fd < 0)
    {
        perror("open /dev/mem failed");
        while (1) { sleep(1); }
    }
    void *shared_base_addr = mmap(NULL, SHARED_TOTAL_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, SHARED_BASE_ADDR);
    if (shared_base_addr == MAP_FAILED)
    {
        perror("mmap failed");
        close(fd);
        while (1) { sleep(1); }
    }

    struct RingBuffer *linux_write_rb = (struct RingBuffer *)(shared_base_addr);
    struct RingBuffer *linux_read_rb = (struct RingBuffer *)((uint8_t *)shared_base_addr + BUFFER_SIZE);

    /* Linux가 쓰는 송신 링버퍼 초기화 */
    RingBufferInit(linux_write_rb);

    /*
        communicate with Core3..
    */
    int count = 0;
    char send_msg[64];
    while (1)
    {
        sprintf(send_msg, "Linux in Core0 sending count : %d\n", count++);
        RingBufferWrite(linux_write_rb, send_msg, strlen(send_msg));
        printf("Linux sent: %s", send_msg);
        sleep(1);
    }
    munmap(shared_base_addr, SHARED_TOTAL_SIZE);
    close(fd);

    return 0;
}