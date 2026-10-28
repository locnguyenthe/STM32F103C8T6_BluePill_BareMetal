#include "Stm32f103c8t6_adc.h"

void ADC_CLK_Control(ADC_RegDef_t* pADCx, uint8_t EnorDi){
    if (EnorDi == Enable){
        if (pADCx == ADC1)          RCC->APB2ENR |= (1 << 9);
        else if (pADCx == ADC2)     RCC->APB2ENR |= (1 << 10);
    }
    else{
        if (pADCx == ADC1)          RCC->APB2ENR &= ~(1 << 9);
        else if (pADCx == ADC2)     RCC->APB2ENR &= ~(1 << 10);
    }
}

void ADC_Init(ADC_Handle_t* pADCHandle){
    ADC_CLK_Control(pADCHandle->pADCx, Enable);

    // 1. Configure Data Alignment (ALIGN bit 11) and Continuous Mode (CONT bit 1)
    if (pADCHandle->ADC_Config.ADC_DataAlign == ADC_ALIGN_LEFT){
        pADCHandle->pADCx->CR2 |= (1 << 11);
    }
    else{
        pADCHandle->pADCx->CR2 &= ~(1 << 11);
    }

    if (pADCHandle->ADC_Config.ADC_ContinuousMode == Enable){
        pADCHandle->pADCx->CR2 |= (1 << 1);
    }
    else{
        pADCHandle->pADCx->CR2 &= ~(1 << 1);
    }

    // 2. Configure Sample Time for the selected channel
    uint8_t ch = pADCHandle->ADC_Config.ADC_Channel;
    if (ch <= 9){
        pADCHandle->pADCx->SMPR2 &= ~(0x07 << (ch * 3));
        pADCHandle->pADCx->SMPR2 |=  ((pADCHandle->ADC_Config.ADC_SampleTime & 0x07) << (ch * 3));
    }
    else{
        pADCHandle->pADCx->SMPR1 &= ~(0x07 << ((ch - 10) * 3));
        pADCHandle->pADCx->SMPR1 |=  ((pADCHandle->ADC_Config.ADC_SampleTime & 0x07) << ((ch - 10) * 3));
    }

    // 3. Power on ADC (ADON bit 0) and run hardware calibration
    pADCHandle->pADCx->CR2 |= (1 << 0);
    for (volatile int i = 0; i < 1000; i++); // Short stabilization delay

    pADCHandle->pADCx->CR2 |= (1 << 3);      // Reset calibration (RSTCAL)
    while (pADCHandle->pADCx->CR2 & (1 << 3));

    pADCHandle->pADCx->CR2 |= (1 << 2);      // Start calibration (CAL)
    while (pADCHandle->pADCx->CR2 & (1 << 2));
}

uint16_t ADC_Read(ADC_RegDef_t* pADCx, uint8_t channel){
    // Set sequence length to 1 conversion (L = 0000 in SQR1) and channel in SQ1
    pADCx->SQR1 &= ~(0x0F << 20);
    pADCx->SQR3  = (channel & 0x1F);

    // Trigger conversion by writing ADON = 1 a second time
    pADCx->CR2 |= (1 << 0);

    // Wait for End of Conversion (EOC bit 1)
    while (!(pADCx->SR & (1 << 1)));

    return (uint16_t)(pADCx->DR & 0xFFFF);
}