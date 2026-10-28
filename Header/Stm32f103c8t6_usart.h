#ifndef _STM32F103C8T6_USART_H_
#define _STM32F103C8T6_USART_H_

#include "Stm32f103c8t6.h"
#include "Stm32f103c8t6_rcc.h"

typedef struct{
    __vo uint32_t SR;       // 0x00 Status register
    __vo uint32_t DR;       // 0x04 Data register
    __vo uint32_t BRR;      // 0x08 Baud rate register
    __vo uint32_t CR1;      // 0x0C Control register 1
    __vo uint32_t CR2;      // 0x10 Control register 2
    __vo uint32_t CR3;      // 0x14 Control register 3
    __vo uint32_t GTPR;     // 0x18 Guard time and prescaler register
}USART_RegDef_t;

typedef struct{
    uint8_t  USART_Mode;
    uint32_t USART_Baud;
    uint8_t  USART_NoOfStopBits;
    uint8_t  USART_WordLength;
    uint8_t  USART_ParityControl;
    uint8_t  USART_HWFlowControl;
}USART_Config_t;

typedef struct{
    USART_RegDef_t* pUSARTx;
    USART_Config_t  USART_Config;
}USART_Handle_t;

#define USART1                      ((USART_RegDef_t*)0x40013800u) // APB2
#define USART2                      ((USART_RegDef_t*)0x40004400u) // APB1
#define USART3                      ((USART_RegDef_t*)0x40004800u) // APB1

// USART_Mode
#define USART_MODE_ONLY_TX          0
#define USART_MODE_ONLY_RX          1
#define USART_MODE_TXRX             2

// USART_WordLength
#define USART_WORDLEN_8BITS         0
#define USART_WORDLEN_9BITS         1

// USART_ParityControl
#define USART_PARITY_DISABLE        0
#define USART_PARITY_EN_EVEN        1
#define USART_PARITY_EN_ODD         2

// USART_NoOfStopBits
#define USART_STOPBITS_1            0
#define USART_STOPBITS_0_5          1
#define USART_STOPBITS_2            2
#define USART_STOPBITS_1_5          3

// USART_HWFlowControl
#define USART_HW_FLOW_CTRL_NONE     0
#define USART_HW_FLOW_CTRL_CTS      1
#define USART_HW_FLOW_CTRL_RTS      2
#define USART_HW_FLOW_CTRL_CTS_RTS  3

void USART_CLK_Control(USART_RegDef_t* pUSARTx, uint8_t EnorDi);
void USART_Init(USART_Handle_t* pUSARTHandle);
void USART_Reset(USART_RegDef_t* pUSARTx);

void USART_SendData(USART_RegDef_t* pUSARTx, uint8_t* pTxBuffer, uint32_t Len);
void USART_ReceiveData(USART_RegDef_t* pUSARTx, uint8_t* pRxBuffer, uint32_t Len);

void USART_IRQInterrupt_Config(uint8_t IRQNumber, uint8_t EnorDi);
void USART_IRQInterrupt_Priority(uint8_t IRQNumber, uint8_t IRQPriority);

#endif