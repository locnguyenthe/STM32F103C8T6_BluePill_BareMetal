#include "Stm32f103c8t6_i2c.h"

void I2C_CLK_Control(I2C_RegDef_t* pI2Cx, uint8_t EnorDi){
    if (EnorDi == Enable){
        if (pI2Cx == I2C1)          RCC->APB1ENR |= (1 << 21);
        else if (pI2Cx == I2C2)     RCC->APB1ENR |= (1 << 22);
    }
    else{
        if (pI2Cx == I2C1)          RCC->APB1ENR &= ~(1 << 21);
        else if (pI2Cx == I2C2)     RCC->APB1ENR &= ~(1 << 22);
    }
}

void I2C_Init(I2C_Handle_t* pI2CHandle){
    I2C_CLK_Control(pI2CHandle->pI2Cx, Enable);

    // 1. Configure FREQ field in CR2
    uint32_t pclk1 = RCC_GetPCLK1();
    pI2CHandle->pI2Cx->CR2 = (pclk1 / 1000000u) & 0x3F;

    // 2. Configure Own Address in OAR1 (bit 14 should be kept at 1)
    pI2CHandle->pI2Cx->OAR1 = (1 << 14) | (pI2CHandle->I2C_Config.I2C_DeviceAddress << 1);

    // 3. Configure CCR and TRISE
    uint16_t ccr_value = 0;
    if (pI2CHandle->I2C_Config.I2C_SCLSpeed <= I2C_SCL_SPEED_SM){
        ccr_value = (pclk1 / (2 * pI2CHandle->I2C_Config.I2C_SCLSpeed)) & 0xFFF;
        pI2CHandle->pI2Cx->CCR = ccr_value;
        pI2CHandle->pI2Cx->TRISE = ((pclk1 / 1000000u) + 1) & 0x3F;
    }
    else{
        uint16_t tempreg = (1 << 15); // Fast Mode
        tempreg |= (pI2CHandle->I2C_Config.I2C_FMDutyCycle << 14);
        if (pI2CHandle->I2C_Config.I2C_FMDutyCycle == I2C_FM_DUTY_2){
            ccr_value = (pclk1 / (3 * pI2CHandle->I2C_Config.I2C_SCLSpeed)) & 0xFFF;
        }
        else{
            ccr_value = (pclk1 / (25 * pI2CHandle->I2C_Config.I2C_SCLSpeed)) & 0xFFF;
        }
        pI2CHandle->pI2Cx->CCR = tempreg | ccr_value;
        pI2CHandle->pI2Cx->TRISE = (((pclk1 * 300) / 1000000000u) + 1) & 0x3F;
    }

    // 4. Enable Peripheral (PE bit 0) and ACK (bit 10)
    pI2CHandle->pI2Cx->CR1 |= (1 << 0);
    if (pI2CHandle->I2C_Config.I2C_ACKControl == I2C_ACK_ENABLE){
        pI2CHandle->pI2Cx->CR1 |= (1 << 10);
    }
}

void I2C_Reset(I2C_RegDef_t* pI2Cx){
    if (pI2Cx == I2C1){
        RCC->APB1RSTR |=  (1 << 21);
        RCC->APB1RSTR &= ~(1 << 21);
    }
    else if (pI2Cx == I2C2){
        RCC->APB1RSTR |=  (1 << 22);
        RCC->APB1RSTR &= ~(1 << 22);
    }
}

void I2C_MasterSendData(I2C_RegDef_t* pI2Cx, uint8_t* pTxBuffer, uint32_t Len, uint8_t SlaveAddr){
    // 1. Generate START condition and wait for SB flag
    pI2Cx->CR1 |= (1 << 8);
    while (!(pI2Cx->SR1 & (1 << 0)));

    // 2. Send Slave Address with Write bit (0) and wait for ADDR flag
    pI2Cx->DR = (SlaveAddr << 1) & ~1;
    while (!(pI2Cx->SR1 & (1 << 1)));
    (void)pI2Cx->SR1;
    (void)pI2Cx->SR2; // Clear ADDR flag

    // 3. Send Data bytes
    while (Len > 0){
        while (!(pI2Cx->SR1 & (1 << 7))); // Wait for TXE
        pI2Cx->DR = *pTxBuffer++;
        Len--;
    }

    // 4. Wait for TXE and BTF, then generate STOP condition
    while (!(pI2Cx->SR1 & (1 << 7)));
    while (!(pI2Cx->SR1 & (1 << 2)));
    pI2Cx->CR1 |= (1 << 9);
}

void I2C_MasterReceiveData(I2C_RegDef_t* pI2Cx, uint8_t* pRxBuffer, uint32_t Len, uint8_t SlaveAddr){
    pI2Cx->CR1 |= (1 << 10); // Enable ACK
    pI2Cx->CR1 |= (1 << 8);  // Generate START
    while (!(pI2Cx->SR1 & (1 << 0)));

    pI2Cx->DR = (SlaveAddr << 1) | 1; // Address + Read bit (1)
    while (!(pI2Cx->SR1 & (1 << 1)));

    if (Len == 1){
        pI2Cx->CR1 &= ~(1 << 10); // Disable ACK
        (void)pI2Cx->SR1;
        (void)pI2Cx->SR2;         // Clear ADDR
        pI2Cx->CR1 |= (1 << 9);   // Generate STOP
        while (!(pI2Cx->SR1 & (1 << 6))); // Wait for RXNE
        *pRxBuffer = (uint8_t)pI2Cx->DR;
    }
    else{
        (void)pI2Cx->SR1;
        (void)pI2Cx->SR2;
        for (uint32_t i = Len; i > 0; i--){
            while (!(pI2Cx->SR1 & (1 << 6))); // Wait for RXNE
            if (i == 2){
                pI2Cx->CR1 &= ~(1 << 10); // Disable ACK
                pI2Cx->CR1 |=  (1 << 9);  // Generate STOP
            }
            *pRxBuffer++ = (uint8_t)pI2Cx->DR;
        }
    }
}