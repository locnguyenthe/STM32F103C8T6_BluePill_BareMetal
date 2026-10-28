#ifndef _STM32F103C8T6_SYSTICK_H_
#define _STM32F103C8T6_SYSTICK_H_

#include "Stm32f103c8t6.h"
#include "Stm32f103c8t6_rcc.h"

typedef struct{
    __vo uint32_t CTRL;     // 0x00 Control and status register
    __vo uint32_t LOAD;     // 0x04 Reload value register
    __vo uint32_t VAL;      // 0x08 Current value register
    __vo uint32_t CALIB;    // 0x0C Calibration value register
}SysTick_RegDef_t;

#define SYSTICK                     ((SysTick_RegDef_t*)0xE000E010u)

#define SYSTICK_CLK_AHB_DIV8        0
#define SYSTICK_CLK_AHB             1

void SysTick_Init(uint32_t ticks, uint8_t clk_source, uint8_t int_enable);
void SysTick_DelayMs(uint32_t ms);
void SysTick_DelayUs(uint32_t us);

#endif