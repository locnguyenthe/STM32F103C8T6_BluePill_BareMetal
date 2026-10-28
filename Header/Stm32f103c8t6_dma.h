#ifndef _STM32F103C8T6_DMA_H_
#define _STM32F103C8T6_DMA_H_

#include "Stm32f103c8t6.h"
#include "Stm32f103c8t6_rcc.h"

typedef struct{
    __vo uint32_t ISR;      // 0x00 Interrupt status register
    __vo uint32_t IFCR;     // 0x04 Interrupt flag clear register
}DMA_RegDef_t;

typedef struct{
    __vo uint32_t CCR;      // 0x00 Channel configuration register
    __vo uint32_t CNDTR;    // 0x04 Number of data register
    __vo uint32_t CPAR;     // 0x08 Peripheral address register
    __vo uint32_t CMAR;     // 0x0C Memory address register
}DMA_Channel_RegDef_t;

typedef struct{
    uint8_t  DMA_Direction;
    uint8_t  DMA_CircularMode;
    uint8_t  DMA_PeriphInc;
    uint8_t  DMA_MemInc;
    uint8_t  DMA_PeriphDataSize;
    uint8_t  DMA_MemDataSize;
    uint8_t  DMA_Priority;
}DMA_Config_t;

typedef struct{
    DMA_Channel_RegDef_t* pDMAChannelx;
    DMA_Config_t          DMA_Config;
}DMA_Handle_t;

#define DMA1                        ((DMA_RegDef_t*)0x40020000u)
#define DMA1_Channel1               ((DMA_Channel_RegDef_t*)0x40020008u)
#define DMA1_Channel2               ((DMA_Channel_RegDef_t*)0x4002001Cu)
#define DMA1_Channel3               ((DMA_Channel_RegDef_t*)0x40020030u)
#define DMA1_Channel4               ((DMA_Channel_RegDef_t*)0x40020044u)
#define DMA1_Channel5               ((DMA_Channel_RegDef_t*)0x40020058u)
#define DMA1_Channel6               ((DMA_Channel_RegDef_t*)0x4002006Cu)
#define DMA1_Channel7               ((DMA_Channel_RegDef_t*)0x40020080u)

// DMA_Direction
#define DMA_DIR_PERIPH_TO_MEM       0
#define DMA_DIR_MEM_TO_PERIPH       1

// DMA_DataSize
#define DMA_DATASIZE_8BIT           0
#define DMA_DATASIZE_16BIT          1
#define DMA_DATASIZE_32BIT          2

// DMA_Priority
#define DMA_PRIORITY_LOW            0
#define DMA_PRIORITY_MEDIUM         1
#define DMA_PRIORITY_HIGH           2
#define DMA_PRIORITY_VERY_HIGH      3

void DMA_Init(DMA_Handle_t* pDMAHandle);
void DMA_Start(DMA_Channel_RegDef_t* pDMAChannelx, uint32_t PeriphAddr, uint32_t MemAddr, uint16_t DataLength);

#endif