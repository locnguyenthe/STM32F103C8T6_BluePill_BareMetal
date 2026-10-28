#ifndef _STM32F103C8T6_ADC_H_
#define _STM32F103C8T6_ADC_H_

#include "Stm32f103c8t6.h"
#include "Stm32f103c8t6_rcc.h"

typedef struct{
    __vo uint32_t SR;       // 0x00 Status register
    __vo uint32_t CR1;      // 0x04 Control register 1
    __vo uint32_t CR2;      // 0x08 Control register 2
    __vo uint32_t SMPR1;    // 0x0C Sample time register 1
    __vo uint32_t SMPR2;    // 0x10 Sample time register 2
    __vo uint32_t JOFR1;    // 0x14 Injected channel data offset register 1
    __vo uint32_t JOFR2;    // 0x18 Injected channel data offset register 2
    __vo uint32_t JOFR3;    // 0x1C Injected channel data offset register 3
    __vo uint32_t JOFR4;    // 0x20 Injected channel data offset register 4
    __vo uint32_t HTR;      // 0x24 Watchdog higher threshold register
    __vo uint32_t LTR;      // 0x28 Watchdog lower threshold register
    __vo uint32_t SQR1;     // 0x2C Regular sequence register 1
    __vo uint32_t SQR2;     // 0x30 Regular sequence register 2
    __vo uint32_t SQR3;     // 0x34 Regular sequence register 3
    __vo uint32_t JSQR;     // 0x38 Injected sequence register
    __vo uint32_t JDR1;     // 0x3C Injected data register 1
    __vo uint32_t JDR2;     // 0x40 Injected data register 2
    __vo uint32_t JDR3;     // 0x44 Injected data register 3
    __vo uint32_t JDR4;     // 0x48 Injected data register 4
    __vo uint32_t DR;       // 0x4C Regular data register
}ADC_RegDef_t;

typedef struct{
    uint8_t ADC_Channel;
    uint8_t ADC_ContinuousMode;
    uint8_t ADC_DataAlign;
    uint8_t ADC_SampleTime;
}ADC_Config_t;

typedef struct{
    ADC_RegDef_t* pADCx;
    ADC_Config_t  ADC_Config;
}ADC_Handle_t;

#define ADC1                        ((ADC_RegDef_t*)0x40012400u) // APB2
#define ADC2                        ((ADC_RegDef_t*)0x40012800u) // APB2

// ADC_DataAlign
#define ADC_ALIGN_RIGHT             0
#define ADC_ALIGN_LEFT              1

// ADC_SampleTime
#define ADC_SAMPLETIME_1_5          0
#define ADC_SAMPLETIME_7_5          1
#define ADC_SAMPLETIME_13_5         2
#define ADC_SAMPLETIME_28_5         3
#define ADC_SAMPLETIME_41_5         4
#define ADC_SAMPLETIME_55_5         5
#define ADC_SAMPLETIME_71_5         6
#define ADC_SAMPLETIME_239_5        7

void ADC_CLK_Control(ADC_RegDef_t* pADCx, uint8_t EnorDi);
void ADC_Init(ADC_Handle_t* pADCHandle);
uint16_t ADC_Read(ADC_RegDef_t* pADCx, uint8_t channel);

#endif