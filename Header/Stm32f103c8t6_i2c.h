#ifndef _STM32F103C8T6_I2C_H_
#define _STM32F103C8T6_I2C_H_

#include "Stm32f103c8t6.h"
#include "Stm32f103c8t6_rcc.h"

typedef struct{
    __vo uint32_t CR1;      // 0x00 Control register 1
    __vo uint32_t CR2;      // 0x04 Control register 2
    __vo uint32_t OAR1;     // 0x08 Own address register 1
    __vo uint32_t OAR2;     // 0x0C Own address register 2
    __vo uint32_t DR;       // 0x10 Data register
    __vo uint32_t SR1;      // 0x14 Status register 1
    __vo uint32_t SR2;      // 0x18 Status register 2
    __vo uint32_t CCR;      // 0x1C Clock control register
    __vo uint32_t TRISE;    // 0x20 TRISE register
}I2C_RegDef_t;

typedef struct{
    uint32_t I2C_SCLSpeed;
    uint8_t  I2C_DeviceAddress;
    uint8_t  I2C_ACKControl;
    uint16_t I2C_FMDutyCycle;
}I2C_Config_t;

typedef struct{
    I2C_RegDef_t* pI2Cx;
    I2C_Config_t  I2C_Config;
}I2C_Handle_t;

#define I2C1                        ((I2C_RegDef_t*)0x40005400u) // APB1
#define I2C2                        ((I2C_RegDef_t*)0x40005800u) // APB1

// I2C_SCLSpeed
#define I2C_SCL_SPEED_SM            100000u
#define I2C_SCL_SPEED_FM4K          400000u

// I2C_ACKControl
#define I2C_ACK_DISABLE             0
#define I2C_ACK_ENABLE              1

// I2C_FMDutyCycle
#define I2C_FM_DUTY_2               0
#define I2C_FM_DUTY_16_9            1

void I2C_CLK_Control(I2C_RegDef_t* pI2Cx, uint8_t EnorDi);
void I2C_Init(I2C_Handle_t* pI2CHandle);
void I2C_Reset(I2C_RegDef_t* pI2Cx);

void I2C_MasterSendData(I2C_RegDef_t* pI2Cx, uint8_t* pTxBuffer, uint32_t Len, uint8_t SlaveAddr);
void I2C_MasterReceiveData(I2C_RegDef_t* pI2Cx, uint8_t* pRxBuffer, uint32_t Len, uint8_t SlaveAddr);

#endif