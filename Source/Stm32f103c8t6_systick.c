#include "Stm32f103c8t6_systick.h"

void SysTick_Init(uint32_t ticks, uint8_t clk_source, uint8_t int_enable){
    SYSTICK->CTRL = 0;
    SYSTICK->LOAD = (ticks & 0x00FFFFFFu) - 1;
    SYSTICK->VAL  = 0;

    if (clk_source == SYSTICK_CLK_AHB){
        SYSTICK->CTRL |= (1 << 2);
    }
    if (int_enable == Enable){
        SYSTICK->CTRL |= (1 << 1);
    }
    SYSTICK->CTRL |= (1 << 0);
}

void SysTick_DelayMs(uint32_t ms){
    uint32_t ticks_per_ms = RCC_GetHCLK() / 1000;
    SYSTICK->CTRL = 0;
    SYSTICK->LOAD = ticks_per_ms - 1;
    SYSTICK->VAL  = 0;
    SYSTICK->CTRL = (1 << 2) | (1 << 0); // Processor clock (AHB), Enable

    for (uint32_t i = 0; i < ms; i++){
        while (!(SYSTICK->CTRL & (1 << 16))); // Wait for COUNTFLAG
    }
    SYSTICK->CTRL = 0;
}

void SysTick_DelayUs(uint32_t us){
    uint32_t ticks_per_us = RCC_GetHCLK() / 1000000;
    SYSTICK->CTRL = 0;
    SYSTICK->LOAD = ticks_per_us - 1;
    SYSTICK->VAL  = 0;
    SYSTICK->CTRL = (1 << 2) | (1 << 0);

    for (uint32_t i = 0; i < us; i++){
        while (!(SYSTICK->CTRL & (1 << 16)));
    }
    SYSTICK->CTRL = 0;
}