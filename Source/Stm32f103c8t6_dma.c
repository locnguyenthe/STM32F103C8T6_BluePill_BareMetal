#include "Stm32f103c8t6_dma.h"

void DMA_Init(DMA_Handle_t* pDMAHandle){
    // Enable DMA1 Clock on AHB bus (bit 0)
    RCC->AHBENR |= (1 << 0);

    // Disable channel before configuration
    pDMAHandle->pDMAChannelx->CCR &= ~(1 << 0);

    uint32_t tempreg = 0;
    tempreg |= (pDMAHandle->DMA_Config.DMA_Direction << 4);
    tempreg |= (pDMAHandle->DMA_Config.DMA_CircularMode << 5);
    tempreg |= (pDMAHandle->DMA_Config.DMA_PeriphInc << 6);
    tempreg |= (pDMAHandle->DMA_Config.DMA_MemInc << 7);
    tempreg |= ((pDMAHandle->DMA_Config.DMA_PeriphDataSize & 0x03) << 8);
    tempreg |= ((pDMAHandle->DMA_Config.DMA_MemDataSize & 0x03) << 10);
    tempreg |= ((pDMAHandle->DMA_Config.DMA_Priority & 0x03) << 12);

    pDMAHandle->pDMAChannelx->CCR = tempreg;
}

void DMA_Start(DMA_Channel_RegDef_t* pDMAChannelx, uint32_t PeriphAddr, uint32_t MemAddr, uint16_t DataLength){
    pDMAChannelx->CCR &= ~(1 << 0); // Disable channel to load addresses
    pDMAChannelx->CPAR  = PeriphAddr;
    pDMAChannelx->CMAR  = MemAddr;
    pDMAChannelx->CNDTR = DataLength;
    pDMAChannelx->CCR |=  (1 << 0); // Enable channel
}