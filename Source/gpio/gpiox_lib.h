#ifndef _GPIOX_LIB_H_
#define _GPIOX_LIB_H_

#define PIO_BASE (0x01c20800)
#define rPA_CFG2 0x08
#define rPA_DAT 0x10

struct PIO_BASE_REG{
    uint32_t Pn_CFG[4];
    uint32_t Pn_DAT;
    uint32_t Pn_DRV[2];
    uint32_t Pn_PUL;
};

struct PIO_INT_REG{
    uint32_t Pn_INT_CFG[3];
    uint16_t Pn_INT_CFG4;
    uint32_t Pn_INT_CTL;
    uint32_t Pn_INT_STA;
    uint32_t Pn_INT_DEB;
};

struct PIO_REG{
    struct PIO_BASE_REG Pn[7];
    uint8_t offset[0x104];
    struct PIO_INT_REG Pn_INT[2];
};

#endif