/*******************************************************************************
* File Name: .h
* Version 1.10
*
* Description:
*  This private file provides constants and parameter values for the
*  SCB Component in I2C mode.
*  Please do not use this file or its content in your project.
*
* Note:
*
********************************************************************************
* Copyright 2013, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_SCB_PVT_SPI_1_H)
#define CY_SCB_PVT_SPI_1_H

#include "SPI_1.h"


/***************************************
*     Private Function Prototypes
***************************************/

/* APIs to service INTR_I2C_EC register */
#define SPI_1_SetI2CExtClkInterruptMode(interruptMask) SPI_1_WRITE_INTR_I2C_EC_MASK(interruptMask)
#define SPI_1_ClearI2CExtClkInterruptSource(interruptMask) SPI_1_CLEAR_INTR_I2C_EC(interruptMask)
#define SPI_1_GetI2CExtClkInterruptSource()                (SPI_1_INTR_I2C_EC_REG)
#define SPI_1_GetI2CExtClkInterruptMode()                  (SPI_1_INTR_I2C_EC_MASK_REG)
#define SPI_1_GetI2CExtClkInterruptSourceMasked()          (SPI_1_INTR_I2C_EC_MASKED_REG)

/* APIs to service INTR_SPI_EC register */
#define SPI_1_SetSpiExtClkInterruptMode(interruptMask) SPI_1_WRITE_INTR_SPI_EC_MASK(interruptMask)
#define SPI_1_ClearSpiExtClkInterruptSource(interruptMask) SPI_1_CLEAR_INTR_SPI_EC(interruptMask)
#define SPI_1_GetExtSpiClkInterruptSource()                 (SPI_1_INTR_SPI_EC_REG)
#define SPI_1_GetExtSpiClkInterruptMode()                   (SPI_1_INTR_SPI_EC_MASK_REG)
#define SPI_1_GetExtSpiClkInterruptSourceMasked()           (SPI_1_INTR_SPI_EC_MASKED_REG)

#if(SPI_1_SCB_MODE_UNCONFIG_CONST_CFG)
    extern void SPI_1_SetPins(uint32 mode, uint32 subMode, uint32 uartTxRx);
#endif /* (SPI_1_SCB_MODE_UNCONFIG_CONST_CFG) */

void SPI_1_DisableTxPinsInputBuffer(void);
void SPI_1_EnableTxPinsInputBuffer(void);


/**********************************
*     Vars with External Linkage
**********************************/

extern cyisraddress SPI_1_customIntrHandler;
extern SPI_1_BACKUP_STRUCT SPI_1_backup;

#if(SPI_1_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Common config vars */
    extern uint8 SPI_1_scbMode;
    extern uint8 SPI_1_scbEnableWake;
    extern uint8 SPI_1_scbEnableIntr;

    /* I2C config vars */
    extern uint8 SPI_1_mode;
    extern uint8 SPI_1_acceptAddr;

    /* SPI/UART config vars */
    extern volatile uint8 * SPI_1_rxBuffer;
    extern uint8   SPI_1_rxDataBits;
    extern uint32  SPI_1_rxBufferSize;

    extern volatile uint8 * SPI_1_txBuffer;
    extern uint8   SPI_1_txDataBits;
    extern uint32  SPI_1_txBufferSize;

    /* EZI2C config vars */
    extern uint8 SPI_1_numberOfAddr;
    extern uint8 SPI_1_subAddrSize;
#endif /* (SPI_1_SCB_MODE_UNCONFIG_CONST_CFG) */

#endif /* (CY_SCB_PVT_SPI_1_H) */


/* [] END OF FILE */
