#include "Stm32f103c8t6_can.h"

void CAN_CLK_Control(CAN_RegDef_t* pCANx, uint8_t EnorDi){
    if (EnorDi == Enable){
        if (pCANx == CAN1)      RCC->APB1ENR |= (1 << 25);
    }
    else{
        if (pCANx == CAN1)      RCC->APB1ENR &= ~(1 << 25);
    }
}

void CAN_Init(CAN_Handle_t* pCANHandle){
    CAN_CLK_Control(pCANHandle->pCANx, Enable);

    // 1. Exit Sleep mode (clear SLEEP bit 1) and request Initialization mode (set INRQ bit 0)
    pCANHandle->pCANx->MCR &= ~(1 << 1);
    pCANHandle->pCANx->MCR |=  (1 << 0);
    while (!(pCANHandle->pCANx->MSR & (1 << 0))); // Wait for INAK bit 0 to set

    // 2. Configure Auto Bus-Off Management (ABOM bit 6)
    if (pCANHandle->CAN_Config.CAN_AutoBusOff == Enable){
        pCANHandle->pCANx->MCR |= (1 << 6);
    }
    else{
        pCANHandle->pCANx->MCR &= ~(1 << 6);
    }

    // 3. Configure Automatic Retransmission (NART bit 4: 0 = enabled, 1 = disabled)
    if (pCANHandle->CAN_Config.CAN_AutoRetransmission == Enable){
        pCANHandle->pCANx->MCR &= ~(1 << 4);
    }
    else{
        pCANHandle->pCANx->MCR |= (1 << 4);
    }

    // 4. Configure Bit Timing and Operating Mode in BTR
    uint32_t btr_reg = 0;
    btr_reg |= ((pCANHandle->CAN_Config.CAN_Mode & 0x03) << 30);        // SILM (bit 31), LBKM (bit 30)
    btr_reg |= ((pCANHandle->CAN_Config.CAN_SJW  & 0x03) << 24);        // SJW (bits 25:24)
    btr_reg |= ((pCANHandle->CAN_Config.CAN_BS2  & 0x07) << 20);        // TS2 (bits 22:20)
    btr_reg |= ((pCANHandle->CAN_Config.CAN_BS1  & 0x0F) << 16);        // TS1 (bits 19:16)
    btr_reg |= ((pCANHandle->CAN_Config.CAN_Prescaler - 1) & 0x3FF);    // BRP (bits 9:0)

    pCANHandle->pCANx->BTR = btr_reg;
}

void CAN_Reset(CAN_RegDef_t* pCANx){
    if (pCANx == CAN1){
        RCC->APB1RSTR |=  (1 << 25);
        RCC->APB1RSTR &= ~(1 << 25);
    }
}

void CAN_Filter_Config(CAN_RegDef_t* pCANx, CAN_FilterConfig_t* pFilterConfig){
    uint8_t bank = pFilterConfig->FilterBank;

    // 1. Enter Filter Initialization Mode (FINIT bit 0)
    pCANx->FMR |= (1 << 0);

    // 2. Deactivate filter bank before modification
    pCANx->FA1R &= ~(1 << bank);

    // 3. Configure Filter Scale (FS1R)
    if (pFilterConfig->FilterScale == CAN_FILTERSCALE_32BIT){
        pCANx->FS1R |= (1 << bank);
    }
    else{
        pCANx->FS1R &= ~(1 << bank);
    }

    // 4. Configure Filter Mode (FM1R: 0 = Mask Mode, 1 = List Mode)
    if (pFilterConfig->FilterMode == CAN_FILTERMODE_IDLIST){
        pCANx->FM1R |= (1 << bank);
    }
    else{
        pCANx->FM1R &= ~(1 << bank);
    }

    // 5. Set Filter Bank Registers FR1 and FR2
    pCANx->sFilterRegister[bank].FR1 = pFilterConfig->FilterId;
    pCANx->sFilterRegister[bank].FR2 = pFilterConfig->FilterMaskId;

    // 6. Assign Filter to FIFO 0 or FIFO 1 (FFA1R)
    if (pFilterConfig->FilterFIFOAssignment == CAN_FILTER_FIFO1){
        pCANx->FFA1R |= (1 << bank);
    }
    else{
        pCANx->FFA1R &= ~(1 << bank);
    }

    // 7. Activate Filter Bank if enabled
    if (pFilterConfig->FilterActivation == Enable){
        pCANx->FA1R |= (1 << bank);
    }

    // 8. Leave Filter Initialization Mode
    pCANx->FMR &= ~(1 << 0);
}

void CAN_Start(CAN_RegDef_t* pCANx){
    // Clear INRQ bit 0 to leave Initialization mode and enter Normal operating mode
    pCANx->MCR &= ~(1 << 0);
    while (pCANx->MSR & (1 << 0)); // Wait until INAK bit 0 is cleared by hardware
}

uint8_t CAN_SendMessage(CAN_RegDef_t* pCANx, CAN_TxHeader_t* pTxHeader, uint8_t* pData){
    uint8_t mailbox = 0xFF;

    // 1. Find an empty Transmit Mailbox (TME0 bit 26, TME1 bit 27, TME2 bit 28)
    if (pCANx->TSR & (1 << 26))         mailbox = 0;
    else if (pCANx->TSR & (1 << 27))    mailbox = 1;
    else if (pCANx->TSR & (1 << 28))    mailbox = 2;
    else return 0xFF; // No empty mailbox available

    // 2. Clear and set Identifier and RTR in TIR
    uint32_t tir = 0;
    if (pTxHeader->IDE == CAN_ID_STD){
        tir |= ((pTxHeader->StdId & 0x7FFu) << 21);
    }
    else{
        tir |= ((pTxHeader->ExtId & 0x1FFFFFFFu) << 3) | (1 << 2);
    }

    if (pTxHeader->RTR == CAN_RTR_REMOTE){
        tir |= (1 << 1);
    }

    // 3. Set Data Length Code (DLC bits 3:0 in TDTR)
    pCANx->sTxMailBox[mailbox].TDTR = (pTxHeader->DLC & 0x0Fu);

    // 4. Load Payload Data into TDLR (bytes 0..3) and TDHR (bytes 4..7)
    pCANx->sTxMailBox[mailbox].TDLR = ((uint32_t)pData[3] << 24) |
                                      ((uint32_t)pData[2] << 16) |
                                      ((uint32_t)pData[1] << 8)  |
                                      ((uint32_t)pData[0]);

    pCANx->sTxMailBox[mailbox].TDHR = ((uint32_t)pData[7] << 24) |
                                      ((uint32_t)pData[6] << 16) |
                                      ((uint32_t)pData[5] << 8)  |
                                      ((uint32_t)pData[4]);

    // 5. Request transmission by setting TXRQ (bit 0)
    pCANx->sTxMailBox[mailbox].TIR = tir | (1 << 0);

    return mailbox;
}

uint8_t CAN_ReceiveMessage(CAN_RegDef_t* pCANx, uint8_t FIFONumber, CAN_RxHeader_t* pRxHeader, uint8_t* pData){
    // 1. Check if the selected FIFO has pending messages (FMP bits 1:0)
    if (FIFONumber == CAN_FILTER_FIFO0){
        if ((pCANx->RF0R & 0x03) == 0) return 0; // No message pending
    }
    else{
        if ((pCANx->RF1R & 0x03) == 0) return 0;
    }

    // 2. Read Identifier, IDE, and RTR from RIR
    uint32_t rir = pCANx->sFIFOMailBox[FIFONumber].RIR;
    pRxHeader->IDE = (rir >> 2) & 0x01;
    pRxHeader->RTR = (rir >> 1) & 0x01;

    if (pRxHeader->IDE == CAN_ID_STD){
        pRxHeader->StdId = (rir >> 21) & 0x7FFu;
    }
    else{
        pRxHeader->ExtId = (rir >> 3) & 0x1FFFFFFFu;
    }

    // 3. Read Data Length Code (DLC bits 3:0)
    pRxHeader->DLC = pCANx->sFIFOMailBox[FIFONumber].RDTR & 0x0Fu;

    // 4. Extract 8-byte payload from RDLR and RDHR
    uint32_t rdlr = pCANx->sFIFOMailBox[FIFONumber].RDLR;
    uint32_t rdhr = pCANx->sFIFOMailBox[FIFONumber].RDHR;

    pData[0] = (uint8_t)(rdlr & 0xFF);
    pData[1] = (uint8_t)((rdlr >> 8) & 0xFF);
    pData[2] = (uint8_t)((rdlr >> 16) & 0xFF);
    pData[3] = (uint8_t)((rdlr >> 24) & 0xFF);
    pData[4] = (uint8_t)(rdhr & 0xFF);
    pData[5] = (uint8_t)((rdhr >> 8) & 0xFF);
    pData[6] = (uint8_t)((rdhr >> 16) & 0xFF);
    pData[7] = (uint8_t)((rdhr >> 24) & 0xFF);

    // 5. Release the output mailbox (set RFOM bit 5)
    if (FIFONumber == CAN_FILTER_FIFO0){
        pCANx->RF0R |= (1 << 5);
    }
    else{
        pCANx->RF1R |= (1 << 5);
    }

    return 1;
}

void CAN_IRQInterrupt_Config(uint8_t IRQNumber, uint8_t EnorDi){
    uint8_t reg = IRQNumber / 32;
    uint8_t reg_place = IRQNumber % 32;

    if (EnorDi == Enable){
        *(NVIC_ISER0 + reg) |= (1 << reg_place);
    }
    else{
        *(NVIC_ICER0 + reg) |= (1 << reg_place);
    }
}

void CAN_IRQInterrupt_Priority(uint8_t IRQNumber, uint8_t IRQPriority){
    uint8_t reg = IRQNumber / 4;
    uint8_t reg_place = (IRQNumber % 4) * 8 + 4;

    *(NVIC_IPR0 + reg) &= ~(0xFF << ((IRQNumber % 4) * 8));
    *(NVIC_IPR0 + reg) |=  (IRQPriority << reg_place);
}