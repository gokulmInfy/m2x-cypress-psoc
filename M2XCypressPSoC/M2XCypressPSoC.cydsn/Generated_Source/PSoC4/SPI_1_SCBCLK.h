/*******************************************************************************
* File Name: SPI_1_SCBCLK.h
* Version 2.0
*
*  Description:
*   Provides the function and constant definitions for the clock component.
*
*  Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CLOCK_SPI_1_SCBCLK_H)
#define CY_CLOCK_SPI_1_SCBCLK_H

#include <cytypes.h>
#include <cyfitter.h>


/***************************************
*        Function Prototypes
***************************************/

void SPI_1_SCBCLK_Start(void);
void SPI_1_SCBCLK_Stop(void);

void SPI_1_SCBCLK_SetFractionalDividerRegister(uint16 clkDivider, uint8 clkFractional);

uint16 SPI_1_SCBCLK_GetDividerRegister(void);
uint8  SPI_1_SCBCLK_GetFractionalDividerRegister(void);

#define SPI_1_SCBCLK_Enable()                         SPI_1_SCBCLK_Start()
#define SPI_1_SCBCLK_Disable()                        SPI_1_SCBCLK_Stop()
#define SPI_1_SCBCLK_SetDividerRegister(clkDivider, reset)  \
                        SPI_1_SCBCLK_SetFractionalDividerRegister((clkDivider), 0)
#define SPI_1_SCBCLK_SetDivider(clkDivider)           SPI_1_SCBCLK_SetDividerRegister((clkDivider), 1)
#define SPI_1_SCBCLK_SetDividerValue(clkDivider)      SPI_1_SCBCLK_SetDividerRegister((clkDivider) - 1, 1)


/***************************************
*             Registers
***************************************/

#define SPI_1_SCBCLK_DIV_REG    (*(reg32 *)SPI_1_SCBCLK__REGISTER)
#define SPI_1_SCBCLK_ENABLE_REG SPI_1_SCBCLK_DIV_REG

#endif /* !defined(CY_CLOCK_SPI_1_SCBCLK_H) */

/* [] END OF FILE */
