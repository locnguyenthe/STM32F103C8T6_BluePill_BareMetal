#include "Stm32f103c8t6_gpio.h"
#include "Stm32f103c8t6_rcc.h"

void GPIO_CLK_Control(GPIO_RegDef_t* pGPIOx, uint8_t EnorDi){
    if (EnorDi){
        if (pGPIOx == GPIOA)        RCC->APB2ENR |= (1 << 2);
        else if(pGPIOx == GPIOB)    RCC->APB2ENR |= (1 << 3);
        else if(pGPIOx == GPIOC)    RCC->APB2ENR |= (1 << 4);
        else if(pGPIOx == GPIOD)    RCC->APB2ENR |= (1 << 5);
        else if(pGPIOx == GPIOE)    RCC->APB2ENR |= (1 << 6);
    }
    else{
        if (pGPIOx == GPIOA)        RCC->APB2ENR &= ~(1 << 2);
        else if(pGPIOx == GPIOB)    RCC->APB2ENR &= ~(1 << 3);
        else if(pGPIOx == GPIOC)    RCC->APB2ENR &= ~(1 << 4);
        else if(pGPIOx == GPIOD)    RCC->APB2ENR &= ~(1 << 5);
        else if(pGPIOx == GPIOE)    RCC->APB2ENR &= ~(1 << 6);
    }
}

void GPIO_Init(GPIO_Handle_t* pGPIOHandle){
    GPIO_CLK_Control(pGPIOHandle->pGPIO, Enable);

    uint8_t pin = pGPIOHandle->GPIO_Config.pinNumber;
    uint8_t shift = (pin % 8) * 4;
    uint8_t config_bits = 0;

    if (pGPIOHandle->GPIO_Config.pinMode == GPIO_PIN_IN) {
        if (pGPIOHandle->GPIO_Config.pinPUPD == GPIO_NO_Pullup_Pulldown){
            config_bits = 0x04; // Floating input (CNF=01, MODE=00)
        }
        else{
            config_bits = 0x08; // Input with PU/PD (CNF=10, MODE=00)
            if (pGPIOHandle->GPIO_Config.pinPUPD == GPIO_Pullup) {
                pGPIOHandle->pGPIO->ODR |= (1 << pin);
            } else if (pGPIOHandle->GPIO_Config.pinPUPD == GPIO_Pulldown) {
                pGPIOHandle->pGPIO->ODR &= ~(1 << pin);
            }
        }
    }
    else if (pGPIOHandle->GPIO_Config.pinMode == GPIO_PIN_OUT){
        uint8_t speed = (pGPIOHandle->GPIO_Config.pinSpeed == 0) ? GPIO_Speed_LOW : pGPIOHandle->GPIO_Config.pinSpeed;
        config_bits = (pGPIOHandle->GPIO_Config.pinOType << 2) | (speed & 0x03);
    }
    else if (pGPIOHandle->GPIO_Config.pinMode == GPIO_PIN_AF){
        uint8_t speed = (pGPIOHandle->GPIO_Config.pinSpeed == 0) ? GPIO_Speed_LOW : pGPIOHandle->GPIO_Config.pinSpeed;
        config_bits = ((0x02 | pGPIOHandle->GPIO_Config.pinOType) << 2) | (speed & 0x03);
    }
    else if (pGPIOHandle->GPIO_Config.pinMode == GPIO_PIN_ANALOG){
        config_bits = 0x00; // Analog mode (CNF=00, MODE=00)
    }
    else if (pGPIOHandle->GPIO_Config.pinMode >= GPIO_PIN_IT_FT){
        // 1. Configure pin as input (floating or pull-up/pull-down)
        if (pGPIOHandle->GPIO_Config.pinPUPD == GPIO_NO_Pullup_Pulldown){
            config_bits = 0x04;
        } else {
            config_bits = 0x08;
            if (pGPIOHandle->GPIO_Config.pinPUPD == GPIO_Pullup)
                pGPIOHandle->pGPIO->ODR |= (1 << pin);
            else
                pGPIOHandle->pGPIO->ODR &= ~(1 << pin);
        }

        // 2. Enable AFIO Clock and route Port to EXTI line
        RCC->APB2ENR |= (1 << 0);
        uint8_t exti_reg   = pin / 4;
        uint8_t exti_shift = (pin % 4) * 4;
        AFIO->EXTICR[exti_reg] &= ~(0x0F << exti_shift);
        AFIO->EXTICR[exti_reg] |=  (GPIO_PORT_CODE(pGPIOHandle->pGPIO) << exti_shift);

        // 3. Configure Edge Trigger (FTSR / RTSR)
        if (pGPIOHandle->GPIO_Config.pinMode == GPIO_PIN_IT_FT){
            EXTI->FTSR |=  (1 << pin);
            EXTI->RTSR &= ~(1 << pin);
        }
        else if (pGPIOHandle->GPIO_Config.pinMode == GPIO_PIN_IT_RT){
            EXTI->RTSR |=  (1 << pin);
            EXTI->FTSR &= ~(1 << pin);
        }
        else if (pGPIOHandle->GPIO_Config.pinMode == GPIO_PIN_IT_RFT){
            EXTI->FTSR |= (1 << pin);
            EXTI->RTSR |= (1 << pin);
        }

        // 4. Enable EXTI Interrupt Mask
        EXTI->IMR |= (1 << pin);
    }
    if (pin <= 7){
        pGPIOHandle->pGPIO->CRL &= ~(0x0F << shift);
        pGPIOHandle->pGPIO->CRL |=  (config_bits << shift);
    } 
    else{
        pGPIOHandle->pGPIO->CRH &= ~(0x0F << shift);
        pGPIOHandle->pGPIO->CRH |=  (config_bits << shift);
    }
}

void GPIO_Reset(GPIO_RegDef_t* pGPIOx){
    if(pGPIOx == GPIOA){
        RCC->APB2RSTR |= (1 << 2);
        RCC->APB2RSTR &= ~(1 << 2);
    }
    else if(pGPIOx == GPIOB){
        RCC->APB2RSTR |= (1 << 3);
        RCC->APB2RSTR &= ~(1 << 3);
    }
    else if(pGPIOx == GPIOC){
        RCC->APB2RSTR |= (1 << 4);
        RCC->APB2RSTR &= ~(1 << 4);
    }
    else if(pGPIOx == GPIOD){
        RCC->APB2RSTR |= (1 << 5);
        RCC->APB2RSTR &= ~(1 << 5);
    }
    else if(pGPIOx == GPIOE){
        RCC->APB2RSTR |= (1 << 6);
        RCC->APB2RSTR &= ~(1 << 6);
    }
}

uint8_t GPIO_Read(GPIO_RegDef_t* pGPIOx, uint8_t PinNumber){
    uint8_t value = ((pGPIOx->IDR >> PinNumber) & 0x01);
    return value;
}

void GPIO_Write(GPIO_RegDef_t* pGPIOx, uint8_t PinNumber, uint8_t value){
    if (value == Enable){
        pGPIOx->ODR |= (1 << PinNumber);
    }
    else{
        pGPIOx->ODR &= ~(1 << PinNumber);
    }
}

void GPIO_WriteToggle(GPIO_RegDef_t* pGPIOx, uint8_t PinNumber){
    pGPIOx->ODR ^= (1 << PinNumber);
}

// Bottom of Stm32f103c8t6_gpio.c:
void GPIO_IRQInterrupt_Config(uint8_t IRQNumber, uint8_t EnorDi){
    uint8_t reg = IRQNumber / 32;
    uint8_t reg_place = IRQNumber % 32;

    if (EnorDi == Enable){
        *(NVIC_ISER0 + reg) |= (1 << reg_place);
    }
    else{
        *(NVIC_ICER0 + reg) |= (1 << reg_place);
    }
}

void GPIO_IRQInterrupt_Priority(uint8_t IRQNumber, uint8_t IRQPriority){
    uint8_t reg = IRQNumber / 4;
    uint8_t reg_place = (IRQNumber % 4) * 8 + 4;

    *(NVIC_IPR0 + reg) &= ~(0xFF << ((IRQNumber % 4) * 8));
    *(NVIC_IPR0 + reg) |=  (IRQPriority << reg_place);
}

void GPIO_IRQInterrupt_Handling(uint8_t PinNumber){
    if (EXTI->PR & (1 << PinNumber)){
        EXTI->PR |= (1 << PinNumber); // Clear pending bit by writing 1
    }
}