#include "Stm32f103c8t6_rcc.h"

uint16_t AHB_Prescaler[] = {2, 4, 8, 16, 64, 128, 256, 512};
uint8_t  APB2_Prescaler[] = {2, 4, 8, 16};
uint8_t  APB1_Prescaler[] = {2, 4, 8, 16};

void RCC_init(RCC_Handle_t* pRCCHandle){
    if(pRCCHandle->RCC_Config.Clocksource == HSI_Clocksource){
        pRCCHandle->pRCC->CR |= (1 << 0);                   // Enable HSI
        while(!(pRCCHandle->pRCC->CR & (1 << 1)));          // Wait for HSIRDY

        pRCCHandle->pRCC->CFGR &= ~(0x03 << 0);             // Select HSI as SYSCLK
    }
    else if(pRCCHandle->RCC_Config.Clocksource == HSE_Clocksource){
        pRCCHandle->pRCC->CR |= (1 << 16);                  // Enable HSE
        while(!(pRCCHandle->pRCC->CR & (1 << 17)));         // Wait for HSERDY

        pRCCHandle->pRCC->CFGR &= ~(0x03 << 0);
        pRCCHandle->pRCC->CFGR |=  (0x01 << 0);             // Select HSE as SYSCLK
    }

    // Configure AHB Prescaler (bits 7:4)
    pRCCHandle->pRCC->CFGR &= ~(0x0F << 4);
    pRCCHandle->pRCC->CFGR |=  ((pRCCHandle->RCC_Config.AHBPrescaler & 0x0F) << 4);

    // Configure APB1 Prescaler (bits 10:8)
    pRCCHandle->pRCC->CFGR &= ~(0x07 << 8);
    pRCCHandle->pRCC->CFGR |=  ((pRCCHandle->RCC_Config.APB1Prescaler & 0x07) << 8);

    // Configure APB2 Prescaler (bits 13:11)
    pRCCHandle->pRCC->CFGR &= ~(0x07 << 11);
    pRCCHandle->pRCC->CFGR |=  ((pRCCHandle->RCC_Config.APB2Prescaler & 0x07) << 11);
}

uint32_t RCC_GetSYSCLK(void){
    uint32_t temp = ((RCC->CFGR >> 2) & 0x03);
    if (temp == HSI_Clocksource){
        return HSI;
    }
    else if (temp == HSE_Clocksource){
        return HSE;
    }
    return HSI;
}

uint32_t RCC_GetHCLK(void){
    uint32_t sysclk = RCC_GetSYSCLK();
    uint8_t temp = ((RCC->CFGR >> 4) & 0x0F);
    uint16_t ahbprescaler = (temp < 8) ? 1 : AHB_Prescaler[temp - 8];

    return (sysclk / ahbprescaler);
}

uint32_t RCC_GetPCLK2(void){
    uint32_t hclk = RCC_GetHCLK();
    uint8_t temp = ((RCC->CFGR >> 11) & 0x07);
    uint8_t apb2prescaler = (temp < 4) ? 1 : APB2_Prescaler[temp - 4];

    return (hclk / apb2prescaler);
}

uint32_t RCC_GetPCLK1(void){
    uint32_t hclk = RCC_GetHCLK();
    uint8_t temp = ((RCC->CFGR >> 8) & 0x07);
    uint8_t apb1prescaler = (temp < 4) ? 1 : APB1_Prescaler[temp - 4];

    return (hclk / apb1prescaler);
}