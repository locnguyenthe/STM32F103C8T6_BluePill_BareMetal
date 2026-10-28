#ifndef _STM32F103C8T6_H_
#define _STM32F103C8T6_H_

#include <stdint.h>

#define __vo                            volatile

// Memory Base Addresses
#define FLASH_BASE_Address              0x08000000u // Size: 64 KB, ending at 0x0800FFFF
#define SRAM_BASE_Address               0x20000000u // Size: 20 KB, ending at 0x20004FFF

// Bus Base Addresses
#define AHB_BASE_Address                0x40018000u // Max 72 MHz
#define APB2_BASE_Address               0x40010000u // Max 72 MHz
#define APB1_BASE_Address               0x40000000u // Max 36 MHz

// ARM Cortex-M3 NVIC Register Addresses
#define NVIC_ISER0                      ((__vo uint32_t*)0xE000E100u)
#define NVIC_ISER1                      ((__vo uint32_t*)0xE000E104u)
#define NVIC_ISER2                      ((__vo uint32_t*)0xE000E108u)

#define NVIC_ICER0                      ((__vo uint32_t*)0xE000E180u)
#define NVIC_ICER1                      ((__vo uint32_t*)0xE000E184u)
#define NVIC_ICER2                      ((__vo uint32_t*)0xE000E188u)

#define NVIC_IPR0                       ((__vo uint32_t*)0xE000E400u)

// IRQ Numbers for STM32F103C8T6
#define IRQ_EXTI0                       6
#define IRQ_EXTI1                       7
#define IRQ_EXTI2                       8
#define IRQ_EXTI3                       9
#define IRQ_EXTI4                       10
#define IRQ_EXTI9_5                     23
#define IRQ_TIM1_BRK                    24
#define IRQ_TIM1_UP                     25
#define IRQ_TIM1_TRG_COM                26
#define IRQ_TIM1_CC                     27
#define IRQ_TIM2                        28
#define IRQ_TIM3                        29
#define IRQ_TIM4                        30
#define IRQ_EXTI15_10                   40

// Generic Macros
#define SET                             1
#define RESET                           0
#define Enable                          SET
#define Disable                         RESET

#endif