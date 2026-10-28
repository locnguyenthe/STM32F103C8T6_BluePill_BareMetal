#include "Stm32f103c8t6_tim.h"

void TIM_CLK_Control(TIM_RegDef_t* pTIMx, uint8_t EnorDi){
    if (EnorDi == Enable){
        if (pTIMx == TIM1)          RCC->APB2ENR |= (1 << 11);
        else if (pTIMx == TIM2)     RCC->APB1ENR |= (1 << 0);
        else if (pTIMx == TIM3)     RCC->APB1ENR |= (1 << 1);
        else if (pTIMx == TIM4)     RCC->APB1ENR |= (1 << 2);
    }
    else{
        if (pTIMx == TIM1)          RCC->APB2ENR &= ~(1 << 11);
        else if (pTIMx == TIM2)     RCC->APB1ENR &= ~(1 << 0);
        else if (pTIMx == TIM3)     RCC->APB1ENR &= ~(1 << 1);
        else if (pTIMx == TIM4)     RCC->APB1ENR &= ~(1 << 2);
    }
}

void TIM_Init(TIM_Handle_t* pTIMHandle){
    TIM_CLK_Control(pTIMHandle->pTIMx, Enable);

    // 1. Configure Counter Mode (DIR bit 4, CMS bits 6:5)
    pTIMHandle->pTIMx->CR1 &= ~(0x07 << 4);
    pTIMHandle->pTIMx->CR1 |=  ((pTIMHandle->TIM_Config.TIM_CounterMode & 0x07) << 4);

    // 2. Configure Auto-Reload Preload (ARPE bit 7)
    if (pTIMHandle->TIM_Config.TIM_AutoReloadPreload == TIM_ARP_ENABLE){
        pTIMHandle->pTIMx->CR1 |= (1 << 7);
    }
    else{
        pTIMHandle->pTIMx->CR1 &= ~(1 << 7);
    }

    // 3. Configure Clock Source
    if (pTIMHandle->TIM_Config.TIM_ClockSource == TIM_CLKSRC_INTERNAL){
        pTIMHandle->pTIMx->SMCR &= ~(0x07 << 0); // Clear SMS bits (Internal Clock)
    }
    else if (pTIMHandle->TIM_Config.TIM_ClockSource == TIM_CLKSRC_EXT_MODE1){
        // Select Trigger Source (TS bits 6:4)
        pTIMHandle->pTIMx->SMCR &= ~(0x07 << 4);
        pTIMHandle->pTIMx->SMCR |=  ((pTIMHandle->TIM_Config.TIM_ExtTriggerSource & 0x07) << 4);

        // Select External Clock Mode 1 (SMS = 111)
        pTIMHandle->pTIMx->SMCR |=  (0x07 << 0);
    }
    else if (pTIMHandle->TIM_Config.TIM_ClockSource == TIM_CLKSRC_EXT_MODE2){
        // Configure External Trigger Polarity (ETP bit 15)
        if (pTIMHandle->TIM_Config.TIM_ExtTriggerPolarity == TIM_EXTTRIG_POL_FALLING){
            pTIMHandle->pTIMx->SMCR |= (1 << 15);
        }
        else{
            pTIMHandle->pTIMx->SMCR &= ~(1 << 15);
        }
        // Enable External Clock Mode 2 (ECE bit 14)
        pTIMHandle->pTIMx->SMCR |= (1 << 14);
    }

    // 4. Set Prescaler and Auto-Reload Register
    pTIMHandle->pTIMx->PSC = pTIMHandle->TIM_Config.TIM_Prescaler;
    pTIMHandle->pTIMx->ARR = pTIMHandle->TIM_Config.TIM_Period;

    // 5. Generate Update Event to reload PSC and ARR immediately, then clear UIF flag
    pTIMHandle->pTIMx->EGR |= (1 << 0);
    pTIMHandle->pTIMx->SR  &= ~(1 << 0);
}

void TIM_Reset(TIM_RegDef_t* pTIMx){
    if (pTIMx == TIM1){
        RCC->APB2RSTR |=  (1 << 11);
        RCC->APB2RSTR &= ~(1 << 11);
    }
    else if (pTIMx == TIM2){
        RCC->APB1RSTR |=  (1 << 0);
        RCC->APB1RSTR &= ~(1 << 0);
    }
    else if (pTIMx == TIM3){
        RCC->APB1RSTR |=  (1 << 1);
        RCC->APB1RSTR &= ~(1 << 1);
    }
    else if (pTIMx == TIM4){
        RCC->APB1RSTR |=  (1 << 2);
        RCC->APB1RSTR &= ~(1 << 2);
    }
}

void TIM_Start(TIM_RegDef_t* pTIMx, uint8_t EnorDi){
    if (EnorDi == Enable){
        pTIMx->CR1 |= (1 << 0);
    }
    else{
        pTIMx->CR1 &= ~(1 << 0);
    }
}

uint32_t TIM_ReadCounter(TIM_RegDef_t* pTIMx){
    return pTIMx->CNT;
}

void TIM_WriteCounter(TIM_RegDef_t* pTIMx, uint32_t value){
    pTIMx->CNT = value;
}

uint8_t TIM_GetUpdateFlag(TIM_RegDef_t* pTIMx){
    return (uint8_t)(pTIMx->SR & 0x01);
}

void TIM_ClearUpdateFlag(TIM_RegDef_t* pTIMx){
    pTIMx->SR &= ~(1 << 0);
}

void TIM_IRQInterrupt_Enable(TIM_RegDef_t* pTIMx, uint8_t EnorDi){
    if (EnorDi == Enable){
        pTIMx->DIER |= (1 << 0);    // Enable Update Interrupt (UIE)
    }
    else{
        pTIMx->DIER &= ~(1 << 0);   // Disable Update Interrupt (UIE)
    }
}

void TIM_IRQInterrupt_Config(uint8_t IRQNumber, uint8_t EnorDi){
    uint8_t reg = IRQNumber / 32;
    uint8_t reg_place = IRQNumber % 32;

    if (EnorDi == Enable){
        *(NVIC_ISER0 + reg) |= (1 << reg_place);
    }
    else{
        *(NVIC_ICER0 + reg) |= (1 << reg_place);
    }
}

void TIM_IRQInterrupt_Priority(uint8_t IRQNumber, uint8_t IRQPriority){
    uint8_t reg = IRQNumber / 4;
    uint8_t reg_place = (IRQNumber % 4) * 8 + 4;

    *(NVIC_IPR0 + reg) &= ~(0xFF << ((IRQNumber % 4) * 8));
    *(NVIC_IPR0 + reg) |=  (IRQPriority << reg_place);
}

void TIM_IRQInterrupt_Handling(TIM_RegDef_t* pTIMx){
    if (pTIMx->SR & (1 << 0)){
        pTIMx->SR &= ~(1 << 0);     // Clear UIF flag
    }
}