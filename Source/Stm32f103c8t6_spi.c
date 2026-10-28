#include "Stm32f103c8t6_spi.h"

void SPI_CLK_Control(SPI_RegDef_t* pSPIx, uint8_t EnorDi){
    if (EnorDi == Enable){
        if (pSPIx == SPI1)          RCC->APB2ENR |= (1 << 12);
        else if (pSPIx == SPI2)     RCC->APB1ENR |= (1 << 14);
    }
    else{
        if (pSPIx == SPI1)          RCC->APB2ENR &= ~(1 << 12);
        else if (pSPIx == SPI2)     RCC->APB1ENR &= ~(1 << 14);
    }
}

void SPI_Init(SPI_Handle_t* pSPIHandle){
    uint32_t tempreg = 0;
    SPI_CLK_Control(pSPIHandle->pSPIx, Enable);

    // 1. Configure Device Mode (MSTR bit 2)
    tempreg |= (pSPIHandle->SPI_Config.SPI_DeviceMode << 2);

    // 2. Configure Bus Config
    if (pSPIHandle->SPI_Config.SPI_BusConfig == SPI_BUS_CONFIG_FD){
        tempreg &= ~(1 << 15);
    }
    else if (pSPIHandle->SPI_Config.SPI_BusConfig == SPI_BUS_CONFIG_HD){
        tempreg |= (1 << 15);
    }
    else if (pSPIHandle->SPI_Config.SPI_BusConfig == SPI_BUS_CONFIG_SIMPLEX_RX){
        tempreg &= ~(1 << 15);
        tempreg |=  (1 << 10);
    }

    // 3. Configure Clock Speed (BR bits 5:3), DFF (bit 11), CPOL (bit 1), CPHA (bit 0)
    tempreg |= ((pSPIHandle->SPI_Config.SPI_SclkSpeed & 0x07) << 3);
    tempreg |= (pSPIHandle->SPI_Config.SPI_DFF << 11);
    tempreg |= (pSPIHandle->SPI_Config.SPI_CPOL << 1);
    tempreg |= (pSPIHandle->SPI_Config.SPI_CPHA << 0);

    // 4. Configure Software Slave Management (SSM bit 9, SSI bit 8)
    if (pSPIHandle->SPI_Config.SPI_SSM == SPI_SSM_EN){
        tempreg |= (1 << 9) | (1 << 8);
    }

    pSPIHandle->pSPIx->CR1 = tempreg;
}

void SPI_Reset(SPI_RegDef_t* pSPIx){
    if (pSPIx == SPI1){
        RCC->APB2RSTR |=  (1 << 12);
        RCC->APB2RSTR &= ~(1 << 12);
    }
    else if (pSPIx == SPI2){
        RCC->APB1RSTR |=  (1 << 14);
        RCC->APB1RSTR &= ~(1 << 14);
    }
}

void SPI_PeripheralControl(SPI_RegDef_t* pSPIx, uint8_t EnorDi){
    if (EnorDi == Enable){
        pSPIx->CR1 |= (1 << 6);     // SPE bit 6
    }
    else{
        while (pSPIx->SR & (1 << 7)); // Wait while BSY is set
        pSPIx->CR1 &= ~(1 << 6);
    }
}

void SPI_SendData(SPI_RegDef_t* pSPIx, uint8_t* pTxBuffer, uint32_t Len){
    while (Len > 0){
        while (!(pSPIx->SR & (1 << 1))); // Wait until TXE is set

        if (pSPIx->CR1 & (1 << 11)){     // 16-bit DFF
            pSPIx->DR = *((uint16_t*)pTxBuffer);
            Len -= 2;
            pTxBuffer += 2;
        }
        else{                            // 8-bit DFF
            pSPIx->DR = *pTxBuffer;
            Len--;
            pTxBuffer++;
        }
    }
    while (pSPIx->SR & (1 << 7));        // Wait until BSY is cleared
}

void SPI_ReceiveData(SPI_RegDef_t* pSPIx, uint8_t* pRxBuffer, uint32_t Len){
    while (Len > 0){
        while (!(pSPIx->SR & (1 << 0))); // Wait until RXNE is set

        if (pSPIx->CR1 & (1 << 11)){
            *((uint16_t*)pRxBuffer) = pSPIx->DR;
            Len -= 2;
            pRxBuffer += 2;
        }
        else{
            *pRxBuffer = pSPIx->DR;
            Len--;
            pRxBuffer++;
        }
    }
}