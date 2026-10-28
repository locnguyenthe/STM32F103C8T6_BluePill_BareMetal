#ifndef _STM32F103C8T6_SPI_H_
#define _STM32F103C8T6_SPI_H_

#include "Stm32f103c8t6.h"
#include "Stm32f103c8t6_rcc.h"

typedef struct{
    __vo uint32_t CR1;      // 0x00 Control register 1
    __vo uint32_t CR2;      // 0x04 Control register 2
    __vo uint32_t SR;       // 0x08 Status register
    __vo uint32_t DR;       // 0x0C Data register
    __vo uint32_t CRCPR;    // 0x10 CRC polynomial register
    __vo uint32_t RXCRCR;   // 0x14 RX CRC register
    __vo uint32_t TXCRCR;   // 0x18 TX CRC register
    __vo uint32_t I2SCFGR;  // 0x1C I2S configuration register
    __vo uint32_t I2SPR;    // 0x20 I2S prescaler register
}SPI_RegDef_t;

typedef struct{
    uint8_t SPI_DeviceMode;
    uint8_t SPI_BusConfig;
    uint8_t SPI_SclkSpeed;
    uint8_t SPI_DFF;
    uint8_t SPI_CPOL;
    uint8_t SPI_CPHA;
    uint8_t SPI_SSM;
}SPI_Config_t;

typedef struct{
    SPI_RegDef_t* pSPIx;
    SPI_Config_t  SPI_Config;
}SPI_Handle_t;

#define SPI1                        ((SPI_RegDef_t*)0x40013000u) // APB2
#define SPI2                        ((SPI_RegDef_t*)0x40003800u) // APB1

// SPI_DeviceMode
#define SPI_DEVICE_MODE_SLAVE       0
#define SPI_DEVICE_MODE_MASTER      1

// SPI_BusConfig
#define SPI_BUS_CONFIG_FD           1
#define SPI_BUS_CONFIG_HD           2
#define SPI_BUS_CONFIG_SIMPLEX_RX   3

// SPI_SclkSpeed
#define SPI_SCLK_SPEED_DIV2         0
#define SPI_SCLK_SPEED_DIV4         1
#define SPI_SCLK_SPEED_DIV8         2
#define SPI_SCLK_SPEED_DIV16        3
#define SPI_SCLK_SPEED_DIV32        4
#define SPI_SCLK_SPEED_DIV64        5
#define SPI_SCLK_SPEED_DIV128       6
#define SPI_SCLK_SPEED_DIV256       7

// SPI_DFF
#define SPI_DFF_8BITS               0
#define SPI_DFF_16BITS              1

// SPI_CPOL & SPI_CPHA
#define SPI_CPOL_LOW                0
#define SPI_CPOL_HIGH               1
#define SPI_CPHA_LOW                0
#define SPI_CPHA_HIGH               1

// SPI_SSM
#define SPI_SSM_DI                  0
#define SPI_SSM_EN                  1

void SPI_CLK_Control(SPI_RegDef_t* pSPIx, uint8_t EnorDi);
void SPI_Init(SPI_Handle_t* pSPIHandle);
void SPI_Reset(SPI_RegDef_t* pSPIx);
void SPI_PeripheralControl(SPI_RegDef_t* pSPIx, uint8_t EnorDi);

void SPI_SendData(SPI_RegDef_t* pSPIx, uint8_t* pTxBuffer, uint32_t Len);
void SPI_ReceiveData(SPI_RegDef_t* pSPIx, uint8_t* pRxBuffer, uint32_t Len);

#endif