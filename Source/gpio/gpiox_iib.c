#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "gpiox_lib.h"

#define PIO_BASE (0x01c20800)
#define rPA_CFG2 0x08
#define rPA_DAT 0x10

uint32_t gpio_map;

int init(void)
{
    int fd;
    unsigned int addr_start, addr_offset;
    unsigned int PageSize, PageMask;
    void *pc;

    fd = open("/dev/mem", O_RDWR);
    if (fd < 0)
    {
        return (-1);
    }

    PageSize = 4 * 1024;
    PageMask = (~(PageSize - 1));

    addr_start = PIO_BASE & PageMask;
    addr_offset = PIO_BASE & ~PageMask;

    pc = (void *)mmap(0, PageSize * 2, PROT_READ | PROT_WRITE, MAP_SHARED, fd, addr_start);
    if (pc == MAP_FAILED)
    {
        printf("mmap failed\n");
        return (-1);
    }

    gpio_map = (unsigned int)pc;
    gpio_map += addr_offset;

    close(fd);
    return 0;
}
int output(uint32_t pin, uint8_t mode)
{
    if (mode<0 | mode> 7)
    {
        printf("output--mode error!\n");
        return -1;
    }

    uint32_t bank = pin / 32;
    uint32_t index = pin % 32 / 8;
    uint32_t offset = pin % 32 % 8;

    struct PIO_BASE_REG *pio_base = &((struct PIO_REG *)gpio_map)->Pn[bank];

    *(&pio_base->Pn_CFG[index]) &= ~(0xf << offset * 4);
    *(&pio_base->Pn_CFG[index]) += (mode) << (offset * 4);

    return 0;
}