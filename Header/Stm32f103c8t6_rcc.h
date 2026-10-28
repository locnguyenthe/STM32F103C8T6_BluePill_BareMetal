#ifndef _STM32F103C8T6_RCC_H_
#define _STM32F103C8T6_RCC_H_

#include "Stm32f103c8t6.h"

// Crystal Frequency in STM32F103C8T6
#define HSI                         8000000u
#define HSE                         8000000u

extern uint16_t AHB_Prescaler[];
extern uint8_t  APB2_Prescaler[];
extern uint8_t  APB1_Prescaler[];

typedef struct{
    __vo uint32_t CR;
    __vo uint32_t CFGR;
    __vo uint32_t CIR;
    __vo uint32_t APB2RSTR;
    __vo uint32_t APB1RSTR;
    __vo uint32_t AHBENR;
    __vo uint32_t APB2ENR;
    __vo uint32_t APB1ENR;
    __vo uint32_t BDCR;
    __vo uint32_t CSR;
}RCC_RefDef_t;

typedef struct{
    uint8_t Clocksource;
    uint8_t AHBPrescaler;
    uint8_t APB1Prescaler;
    uint8_t APB2Prescaler;
}RCC_Config_t;

typedef struct{
    RCC_RefDef_t *pRCC;
    RCC_Config_t RCC_Config;
}RCC_Handle_t;

#define RCC                         ((RCC_RefDef_t*)0x40021000u)

// Clock source
#define HSI_Clocksource             0
#define HSE_Clocksource             1

// AHB Prescaler
#define AHB_DIV_1                   0
#define AHB_DIV_2                   8
#define AHB_DIV_4                   9
#define AHB_DIV_8                   10
#define AHB_DIV_16                  11
#define AHB_DIV_64                  12
#define AHB_DIV_128                 13
#define AHB_DIV_256                 14
#define AHB_DIV_512                 15

// APB2 Prescaler
#define APB2_DIV1                   0
#define APB2_DIV2                   4
#define APB2_DIV4                   5
#define APB2_DIV8                   6
#define APB2_DIV16                  7

// APB1 Prescaler
#define APB1_DIV1                   0
#define APB1_DIV2                   4
#define APB1_DIV4                   5
#define APB1_DIV8                   6
#define APB1_DIV16                  7

void RCC_init(RCC_Handle_t* pRCCHandle);
uint32_t RCC_GetSYSCLK(void);
uint32_t RCC_GetHCLK(void);
uint32_t RCC_GetPCLK2(void);
uint32_t RCC_GetPCLK1(void);

#endif