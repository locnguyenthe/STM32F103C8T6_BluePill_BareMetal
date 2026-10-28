#include "Stm32f103c8t6_usart.h"

void USART_CLK_Control(USART_RegDef_t* pUSARTx, uint8_t EnorDi){
    if (EnorDi == Enable){
        if (pUSARTx == USART1)          RCC->APB2ENR |= (1 << 14);
        else if (pUSARTx == USART2)     RCC->APB1ENR |= (1 << 17);
        else if (pUSARTx == USART3)     RCC->APB1ENR |= (1 << 18);
    }
    else{
        if (pUSARTx == USART1)          RCC->APB2ENR &= ~(1 << 14);
        else if (pUSARTx == USART2)     RCC->APB1ENR &= ~(1 << 17);
        else if (pUSARTx == USART3)     RCC->APB1ENR &= ~(1 << 18);
    }
}

void USART_Init(USART_Handle_t* pUSARTHandle){
    uint32_t tempreg = 0;
    USART_CLK_Control(pUSARTHandle->pUSARTx, Enable);

    // 1. Configure TX and RX Mode
    if (pUSARTHandle->USART_Config.USART_Mode == USART_MODE_ONLY_RX){
        tempreg |= (1 << 2);
    }
    else if (pUSARTHandle->USART_Config.USART_Mode == USART_MODE_ONLY_TX){
        tempreg |= (1 << 3);
    }
    else if (pUSARTHandle->USART_Config.USART_Mode == USART_MODE_TXRX){
        tempreg |= ((1 << 2) | (1 << 3));
    }

    // 2. Configure Word Length (M bit 12)
    tempreg |= (pUSARTHandle->USART_Config.USART_WordLength << 12);

    // 3. Configure Parity (PCE bit 10, PS bit 9)
    if (pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_EN_EVEN){
        tempreg |= (1 << 10);
    }
    else if (pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_EN_ODD){
        tempreg |= (1 << 10) | (1 << 9);
    }

    pUSARTHandle->pUSARTx->CR1 = tempreg;

    // 4. Configure Stop Bits (CR2 bits 13:12)
    pUSARTHandle->pUSARTx->CR2 &= ~(0x03 << 12);
    pUSARTHandle->pUSARTx->CR2 |=  ((pUSARTHandle->USART_Config.USART_NoOfStopBits & 0x03) << 12);

    // 5. Configure Hardware Flow Control (CR3 bits 9:8)
    tempreg = 0;
    if (pUSARTHandle->USART_Config.USART_HWFlowControl == USART_HW_FLOW_CTRL_CTS){
        tempreg |= (1 << 9);
    }
    else if (pUSARTHandle->USART_Config.USART_HWFlowControl == USART_HW_FLOW_CTRL_RTS){
        tempreg |= (1 << 8);
    }
    else if (pUSARTHandle->USART_Config.USART_HWFlowControl == USART_HW_FLOW_CTRL_CTS_RTS){
        tempreg |= (1 << 8) | (1 << 9);
    }
    pUSARTHandle->pUSARTx->CR3 = tempreg;

    // 6. Configure Baud Rate (F103 uses fixed 16x oversampling)
    uint32_t pclk = (pUSARTHandle->pUSARTx == USART1) ? RCC_GetPCLK2() : RCC_GetPCLK1();
    uint32_t usartdiv = (pclk + (pUSARTHandle->USART_Config.USART_Baud / 2)) / pUSARTHandle->USART_Config.USART_Baud;
    pUSARTHandle->pUSARTx->BRR = usartdiv;

    // 7. Enable USART (UE bit 13)
    pUSARTHandle->pUSARTx->CR1 |= (1 << 13);
}

void USART_Reset(USART_RegDef_t* pUSARTx){
    if (pUSARTx == USART1){
        RCC->APB2RSTR |=  (1 << 14);
        RCC->APB2RSTR &= ~(1 << 14);
    }
    else if (pUSARTx == USART2){
        RCC->APB1RSTR |=  (1 << 17);
        RCC->APB1RSTR &= ~(1 << 17);
    }
    else if (pUSARTx == USART3){
        RCC->APB1RSTR |=  (1 << 18);
        RCC->APB1RSTR &= ~(1 << 18);
    }
}

void USART_SendData(USART_RegDef_t* pUSARTx, uint8_t* pTxBuffer, uint32_t Len){
    for (uint32_t i = 0; i < Len; i++){
        while (!(pUSARTx->SR & (1 << 7))); // Wait until TXE is set
        pUSARTx->DR = (*pTxBuffer++ & 0xFF);
    }
    while (!(pUSARTx->SR & (1 << 6)));     // Wait until TC is set
}

void USART_ReceiveData(USART_RegDef_t* pUSARTx, uint8_t* pRxBuffer, uint32_t Len){
    for (uint32_t i = 0; i < Len; i++){
        while (!(pUSARTx->SR & (1 << 5))); // Wait until RXNE is set
        *pRxBuffer++ = (uint8_t)(pUSARTx->DR & 0xFF);
    }
}

void USART_IRQInterrupt_Config(uint8_t IRQNumber, uint8_t EnorDi){
    uint8_t reg = IRQNumber / 32;
    uint8_t reg_place = IRQNumber % 32;

    if (EnorDi == Enable){
        *(NVIC_ISER0 + reg) |= (1 << reg_place);
    }
    else{
        *(NVIC_ICER0 + reg) |= (1 << reg_place);
    }
}

void USART_IRQInterrupt_Priority(uint8_t IRQNumber, uint8_t IRQPriority){
    uint8_t reg = IRQNumber / 4;
    uint8_t reg_place = (IRQNumber % 4) * 8 + 4;

    *(NVIC_IPR0 + reg) &= ~(0xFF << ((IRQNumber % 4) * 8));
    *(NVIC_IPR0 + reg) |=  (IRQPriority << reg_place);
}