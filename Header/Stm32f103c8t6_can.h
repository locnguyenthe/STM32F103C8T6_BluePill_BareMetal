#ifndef _STM32F103C8T6_CAN_H_
#define _STM32F103C8T6_CAN_H_

#include "Stm32f103c8t6.h"
#include "Stm32f103c8t6_rcc.h"

// TX Mailbox Register Structure
typedef struct{
    __vo uint32_t TIR;          // TX mailbox identifier register
    __vo uint32_t TDTR;         // TX mailbox data length control and time stamp register
    __vo uint32_t TDLR;         // TX mailbox data low register
    __vo uint32_t TDHR;         // TX mailbox data high register
}CAN_TxMailBox_RegDef_t;

// RX FIFO Mailbox Register Structure
typedef struct{
    __vo uint32_t RIR;          // RX FIFO mailbox identifier register
    __vo uint32_t RDTR;         // RX FIFO mailbox data length control and time stamp register
    __vo uint32_t RDLR;         // RX FIFO mailbox data low register
    __vo uint32_t RDHR;         // RX FIFO mailbox data high register
}CAN_FIFOMailBox_RegDef_t;

// Filter Bank Register Structure
typedef struct{
    __vo uint32_t FR1;          // Filter bank register 1
    __vo uint32_t FR2;          // Filter bank register 2
}CAN_FilterBank_RegDef_t;

// Main bxCAN Register Definition
typedef struct{
    __vo uint32_t               MCR;                // 0x000 Master control register
    __vo uint32_t               MSR;                // 0x004 Master status register
    __vo uint32_t               TSR;                // 0x008 Transmit status register
    __vo uint32_t               RF0R;               // 0x00C Receive FIFO 0 register
    __vo uint32_t               RF1R;               // 0x010 Receive FIFO 1 register
    __vo uint32_t               IER;                // 0x014 Interrupt enable register
    __vo uint32_t               ESR;                // 0x018 Error status register
    __vo uint32_t               BTR;                // 0x01C Bit timing register
    uint32_t                    RESERVED0[88];      // 0x020 - 0x17F Reserved
    CAN_TxMailBox_RegDef_t      sTxMailBox[3];      // 0x180 - 0x1AF TX Mailboxes 0..2
    CAN_FIFOMailBox_RegDef_t    sFIFOMailBox[2];    // 0x1B0 - 0x1CF RX FIFOs 0..1
    uint32_t                    RESERVED1[12];      // 0x1D0 - 0x1FF Reserved
    __vo uint32_t               FMR;                // 0x200 Filter master register
    __vo uint32_t               FM1R;               // 0x204 Filter mode register
    uint32_t                    RESERVED2;          // 0x208 Reserved
    __vo uint32_t               FS1R;               // 0x20C Filter scale register
    uint32_t                    RESERVED3;          // 0x210 Reserved
    __vo uint32_t               FFA1R;              // 0x214 Filter FIFO assignment register
    uint32_t                    RESERVED4;          // 0x218 Reserved
    __vo uint32_t               FA1R;               // 0x21C Filter activation register
    uint32_t                    RESERVED5[8];       // 0x220 - 0x23F Reserved
    CAN_FilterBank_RegDef_t     sFilterRegister[14];// 0x240 - 0x2AF Filter banks 0..13 (F103 Medium-Density)
}CAN_RegDef_t;

typedef struct{
    uint8_t  CAN_Mode;              // Normal, Loopback, Silent, Silent-Loopback
    uint16_t CAN_Prescaler;         // 1 to 1024
    uint8_t  CAN_SJW;               // Resynchronization jump width (CAN_SJW_1TQ .. 4TQ)
    uint8_t  CAN_BS1;               // Time segment 1 (CAN_BS1_1TQ .. 16TQ)
    uint8_t  CAN_BS2;               // Time segment 2 (CAN_BS2_1TQ .. 8TQ)
    uint8_t  CAN_AutoBusOff;        // Enable or Disable ABOM
    uint8_t  CAN_AutoRetransmission;// Enable or Disable automatic retransmission (NART)
}CAN_Config_t;

typedef struct{
    CAN_RegDef_t* pCANx;
    CAN_Config_t  CAN_Config;
}CAN_Handle_t;

typedef struct{
    uint32_t StdId;                 // Standard Identifier (11-bit: 0 to 0x7FF)
    uint32_t ExtId;                 // Extended Identifier (29-bit: 0 to 0x1FFFFFFF)
    uint8_t  IDE;                   // CAN_ID_STD or CAN_ID_EXT
    uint8_t  RTR;                   // CAN_RTR_DATA or CAN_RTR_REMOTE
    uint8_t  DLC;                   // Data Length Code (0 to 8 bytes)
}CAN_TxHeader_t;

typedef struct{
    uint32_t StdId;
    uint32_t ExtId;
    uint8_t  IDE;
    uint8_t  RTR;
    uint8_t  DLC;
}CAN_RxHeader_t;

typedef struct{
    uint8_t  FilterBank;            // 0 to 13 on STM32F103C8T6
    uint8_t  FilterMode;            // CAN_FILTERMODE_IDMASK or CAN_FILTERMODE_IDLIST
    uint8_t  FilterScale;           // CAN_FILTERSCALE_16BIT or CAN_FILTERSCALE_32BIT
    uint8_t  FilterFIFOAssignment;  // CAN_FILTER_FIFO0 or CAN_FILTER_FIFO1
    uint32_t FilterId;              // 32-bit ID register value
    uint32_t FilterMaskId;          // 32-bit Mask/ID2 register value
    uint8_t  FilterActivation;      // Enable or Disable
}CAN_FilterConfig_t;

#define CAN1                        ((CAN_RegDef_t*)0x40006400u) // APB1 Bus

// CAN_Mode
#define CAN_MODE_NORMAL             0
#define CAN_MODE_LOOPBACK           1
#define CAN_MODE_SILENT             2
#define CAN_MODE_SILENT_LOOPBACK    3

// CAN_SJW (0-based hardware encoding)
#define CAN_SJW_1TQ                 0
#define CAN_SJW_2TQ                 1
#define CAN_SJW_3TQ                 2
#define CAN_SJW_4TQ                 3

// CAN_BS1 (0-based hardware encoding: 0 = 1 TQ ... 15 = 16 TQ)
#define CAN_BS1_1TQ                 0
#define CAN_BS1_2TQ                 1
#define CAN_BS1_3TQ                 2
#define CAN_BS1_4TQ                 3
#define CAN_BS1_5TQ                 4
#define CAN_BS1_6TQ                 5
#define CAN_BS1_7TQ                 6
#define CAN_BS1_8TQ                 7
#define CAN_BS1_9TQ                 8
#define CAN_BS1_10TQ                9
#define CAN_BS1_11TQ                10
#define CAN_BS1_12TQ                11
#define CAN_BS1_13TQ                12
#define CAN_BS1_14TQ                13
#define CAN_BS1_15TQ                14
#define CAN_BS1_16TQ                15

// CAN_BS2 (0-based hardware encoding: 0 = 1 TQ ... 7 = 8 TQ)
#define CAN_BS2_1TQ                 0
#define CAN_BS2_2TQ                 1
#define CAN_BS2_3TQ                 2
#define CAN_BS2_4TQ                 3
#define CAN_BS2_5TQ                 4
#define CAN_BS2_6TQ                 5
#define CAN_BS2_7TQ                 6
#define CAN_BS2_8TQ                 7

// Identifier & Frame Types
#define CAN_ID_STD                  0
#define CAN_ID_EXT                  1
#define CAN_RTR_DATA                0
#define CAN_RTR_REMOTE              1

// Filter Macros
#define CAN_FILTERMODE_IDMASK       0
#define CAN_FILTERMODE_IDLIST       1
#define CAN_FILTERSCALE_16BIT       0
#define CAN_FILTERSCALE_32BIT       1
#define CAN_FILTER_FIFO0            0
#define CAN_FILTER_FIFO1            1

void CAN_CLK_Control(CAN_RegDef_t* pCANx, uint8_t EnorDi);
void CAN_Init(CAN_Handle_t* pCANHandle);
void CAN_Reset(CAN_RegDef_t* pCANx);
void CAN_Filter_Config(CAN_RegDef_t* pCANx, CAN_FilterConfig_t* pFilterConfig);

void CAN_Start(CAN_RegDef_t* pCANx);
uint8_t CAN_SendMessage(CAN_RegDef_t* pCANx, CAN_TxHeader_t* pTxHeader, uint8_t* pData);
uint8_t CAN_ReceiveMessage(CAN_RegDef_t* pCANx, uint8_t FIFONumber, CAN_RxHeader_t* pRxHeader, uint8_t* pData);

void CAN_IRQInterrupt_Config(uint8_t IRQNumber, uint8_t EnorDi);
void CAN_IRQInterrupt_Priority(uint8_t IRQNumber, uint8_t IRQPriority);

#endif