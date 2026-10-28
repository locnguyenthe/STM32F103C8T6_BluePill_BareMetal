#ifndef _STM32F103C8T6_GPIO_H_
#define _STM32F103C8T6_GPIO_H_

#include "Stm32f103c8t6.h"

typedef struct{
    __vo uint32_t CRL;
    __vo uint32_t CRH;
    __vo uint32_t IDR;
    __vo uint32_t ODR;
    __vo uint32_t BSRR;
    __vo uint32_t BRR;
    __vo uint32_t LCKR;
}GPIO_RegDef_t;

typedef struct{
    __vo uint32_t EVCR;
    __vo uint32_t MAPR;
    __vo uint32_t EXTICR[4];
    uint32_t      RESERVED0;
    __vo uint32_t MAPR2;
}AFIO_RegDef_t;

typedef struct{
    __vo uint32_t IMR;
    __vo uint32_t EMR;
    __vo uint32_t RTSR;
    __vo uint32_t FTSR;
    __vo uint32_t SWIER;
    __vo uint32_t PR;
}EXTI_RegDef_t;

typedef struct{
    uint8_t pinNumber;
    uint8_t pinMode;
    uint8_t pinSpeed;
    uint8_t pinPUPD;
    uint8_t pinOType;
}GPIO_Config_t;

typedef struct{
    GPIO_RegDef_t* pGPIO;
    GPIO_Config_t GPIO_Config;
}GPIO_Handle_t;

#define GPIOA                       ((GPIO_RegDef_t*)0x40010800u)
#define GPIOB                       ((GPIO_RegDef_t*)0x40010C00u)
#define GPIOC                       ((GPIO_RegDef_t*)0x40011000u)
#define GPIOD                       ((GPIO_RegDef_t*)0x40011400u)
#define GPIOE                       ((GPIO_RegDef_t*)0x40011800u)


#define AFIO                        ((AFIO_RegDef_t*)0x40010000u)
#define EXTI                        ((EXTI_RegDef_t*)0x40010400u)

// pinNumber
#define GPIO_PIN_0                  0
#define GPIO_PIN_1                  1
#define GPIO_PIN_2                  2
#define GPIO_PIN_3                  3
#define GPIO_PIN_4                  4
#define GPIO_PIN_5                  5
#define GPIO_PIN_6                  6
#define GPIO_PIN_7                  7
#define GPIO_PIN_8                  8
#define GPIO_PIN_9                  9
#define GPIO_PIN_10                 10
#define GPIO_PIN_11                 11
#define GPIO_PIN_12                 12
#define GPIO_PIN_13                 13
#define GPIO_PIN_14                 14
#define GPIO_PIN_15                 15

// pinMode
#define GPIO_PIN_IN                 0
#define GPIO_PIN_OUT                1
#define GPIO_PIN_AF                 2
#define GPIO_PIN_ANALOG             3
#define GPIO_PIN_IT_FT              4   // Falling edge trigger
#define GPIO_PIN_IT_RT              5   // Rising edge trigger
#define GPIO_PIN_IT_RFT             6   // Rising and Falling edge trigger

// GPIO_OutputType
#define GPIO_PushPull               0
#define GPIO_OpenDrain              1

// GPIO_PinSpeed (STM32F103 MODEy[1:0] bits: must be 1, 2, or 3 for Output)
#define GPIO_Speed_MEDIUM           1   // 10 MHz
#define GPIO_Speed_LOW              2   // 2 MHz
#define GPIO_Speed_HIGH             3   // 50 MHz

// GPIO_PinPUPD
#define GPIO_NO_Pullup_Pulldown     0
#define GPIO_Pullup                 1
#define GPIO_Pulldown               2

#define GPIO_PORT_CODE(x)           ((x == GPIOA) ? 0 : \
                                     (x == GPIOB) ? 1 : \
                                     (x == GPIOC) ? 2 : \
                                     (x == GPIOD) ? 3 : \
                                     (x == GPIOE) ? 4 : 0)

void GPIO_CLK_Control(GPIO_RegDef_t* pGPIOx, uint8_t EnorDi);

void GPIO_Init(GPIO_Handle_t* pGPIOHandle);
void GPIO_Reset(GPIO_RegDef_t* pGPIOx);

uint8_t GPIO_Read(GPIO_RegDef_t* pGPIOx, uint8_t PinNumber);
void GPIO_Write(GPIO_RegDef_t* pGPIOx, uint8_t PinNumber, uint8_t value);
void GPIO_WriteToggle(GPIO_RegDef_t* pGPIOx, uint8_t PinNumber);

void GPIO_IRQInterrupt_Config(uint8_t IRQNumber, uint8_t EnorDi);
void GPIO_IRQInterrupt_Priority(uint8_t IRQNumber, uint8_t IRQPriority);
void GPIO_IRQInterrupt_Handling(uint8_t PinNumber);

#endif