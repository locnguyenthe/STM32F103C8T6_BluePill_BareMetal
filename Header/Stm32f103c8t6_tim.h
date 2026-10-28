#ifndef _STM32F103C8T6_TIM_H_
#define _STM32F103C8T6_TIM_H_

#include "Stm32f103c8t6.h"
#include "Stm32f103c8t6_rcc.h"

// Advanced-control timer: TIM1
// General-purpose timers: TIM2 to TIM4

typedef struct {
    __vo uint32_t CR1;     // 0x00 Control register 1
    __vo uint32_t CR2;     // 0x04 Control register 2
    __vo uint32_t SMCR;    // 0x08 Slave mode control register 
    __vo uint32_t DIER;    // 0x0C DMA/Interrupt enable register
    __vo uint32_t SR;      // 0x10 Status register
    __vo uint32_t EGR;     // 0x14 Event generation register
    __vo uint32_t CCMR1;   // 0x18 Capture/compare mode register 1 
    __vo uint32_t CCMR2;   // 0x1C Capture/compare mode register 2
    __vo uint32_t CCER;    // 0x20 Capture/compare enable register 
    __vo uint32_t CNT;     // 0x24 Counter
    __vo uint32_t PSC;     // 0x28 Prescaler
    __vo uint32_t ARR;     // 0x2C Auto-reload register
    __vo uint32_t RCR;     // 0x30 Repetition counter register (TIM1 only)
    __vo uint32_t CCR1;    // 0x34 Capture/compare register 1 
    __vo uint32_t CCR2;    // 0x38 Capture/compare register 2 
    __vo uint32_t CCR3;    // 0x3C Capture/compare register 3
    __vo uint32_t CCR4;    // 0x40 Capture/compare register 4
    __vo uint32_t BDTR;    // 0x44 Break and dead-time register (TIM1 only)
    __vo uint32_t DCR;     // 0x48 DMA control register
    __vo uint32_t DMAR;    // 0x4C DMA address for full transfer
} TIM_RegDef_t;

typedef struct{
    uint16_t TIM_Prescaler;
    uint32_t TIM_Period;
    uint8_t  TIM_CounterMode;
    uint8_t  TIM_AutoReloadPreload;
    uint8_t  TIM_ClockSource;
    uint8_t  TIM_ExtTriggerSource;
    uint8_t  TIM_ExtTriggerPolarity;
}TIM_Config_t;

typedef struct{
    TIM_RegDef_t* pTIMx;
    TIM_Config_t  TIM_Config;
}TIM_Handle_t;

#define TIM1                        ((TIM_RegDef_t*)0x40012C00u)    // APB2 Bus
#define TIM2                        ((TIM_RegDef_t*)0x40000000u)    // APB1 Bus
#define TIM3                        ((TIM_RegDef_t*)0x40000400u)    // APB1 Bus
#define TIM4                        ((TIM_RegDef_t*)0x40000800u)    // APB1 Bus

// TIM_CounterMode
#define TIM_COUNTERMODE_UP          0
#define TIM_COUNTERMODE_DOWN        1
#define TIM_COUNTERMODE_CENTER1     2
#define TIM_COUNTERMODE_CENTER2     4
#define TIM_COUNTERMODE_CENTER3     6

// TIM_AutoReloadPreload
#define TIM_ARP_DISABLE             0
#define TIM_ARP_ENABLE              1

// TIM_ClockSource
#define TIM_CLKSRC_INTERNAL         0
#define TIM_CLKSRC_EXT_MODE1        1
#define TIM_CLKSRC_EXT_MODE2        2

// TIM_ExtTriggerSource (SMCR TS[2:0])
#define TIM_TS_ITR0                 0
#define TIM_TS_ITR1                 1
#define TIM_TS_ITR2                 2
#define TIM_TS_ITR3                 3
#define TIM_TS_TI1F_ED              4
#define TIM_TS_TI1FP1               5
#define TIM_TS_TI2FP2               6
#define TIM_TS_ETRF                 7

// TIM_ExtTriggerPolarity
#define TIM_EXTTRIG_POL_RISING      0
#define TIM_EXTTRIG_POL_FALLING     1

void TIM_CLK_Control(TIM_RegDef_t* pTIMx, uint8_t EnorDi);

void TIM_Init(TIM_Handle_t* pTIMHandle);
void TIM_Reset(TIM_RegDef_t* pTIMx);

void TIM_Start(TIM_RegDef_t* pTIMx, uint8_t EnorDi);
uint32_t TIM_ReadCounter(TIM_RegDef_t* pTIMx);
void TIM_WriteCounter(TIM_RegDef_t* pTIMx, uint32_t value);

uint8_t TIM_GetUpdateFlag(TIM_RegDef_t* pTIMx);
void TIM_ClearUpdateFlag(TIM_RegDef_t* pTIMx);

void TIM_IRQInterrupt_Enable(TIM_RegDef_t* pTIMx, uint8_t EnorDi);
void TIM_IRQInterrupt_Config(uint8_t IRQNumber, uint8_t EnorDi);
void TIM_IRQInterrupt_Priority(uint8_t IRQNumber, uint8_t IRQPriority);
void TIM_IRQInterrupt_Handling(TIM_RegDef_t* pTIMx);

#endif