/*******************************************************************************
* File Name: SPI_1.c
* Version 1.10
*
* Description:
*  This file provides the source code to the API for the SCB Component.
*
* Note:
*
*******************************************************************************
* Copyright 2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "SPI_1_PVT.h"

#if(SPI_1_SCB_MODE_I2C_INC)
    #include "SPI_1_I2C_PVT.h"
#endif /* (SPI_1_SCB_MODE_I2C_INC) */

#if(SPI_1_SCB_MODE_EZI2C_INC)
    #include "SPI_1_EZI2C_PVT.h"
#endif /* (SPI_1_SCB_MODE_EZI2C_INC) */

#if(SPI_1_SCB_MODE_SPI_INC || SPI_1_SCB_MODE_UART_INC)
    #include "SPI_1_SPI_UART_PVT.h"
#endif /* (SPI_1_SCB_MODE_SPI_INC || SPI_1_SCB_MODE_UART_INC) */


/**********************************
*    Run Time Configuration Vars
**********************************/

/* Stores internal component configuration for unconfigured mode */
#if(SPI_1_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Common config vars */
    uint8 SPI_1_scbMode = SPI_1_SCB_MODE_UNCONFIG;
    uint8 SPI_1_scbEnableWake;
    uint8 SPI_1_scbEnableIntr;

    /* I2C config vars */
    uint8 SPI_1_mode;
    uint8 SPI_1_acceptAddr;

    /* SPI/UART config vars */
    volatile uint8 * SPI_1_rxBuffer;
    uint8  SPI_1_rxDataBits;
    uint32 SPI_1_rxBufferSize;

    volatile uint8 * SPI_1_txBuffer;
    uint8  SPI_1_txDataBits;
    uint32 SPI_1_txBufferSize;

    /* EZI2C config vars */
    uint8 SPI_1_numberOfAddr;
    uint8 SPI_1_subAddrSize;
#endif /* (SPI_1_SCB_MODE_UNCONFIG_CONST_CFG) */


/**********************************
*     Common SCB Vars
**********************************/

uint8 SPI_1_initVar = 0u;
cyisraddress SPI_1_customIntrHandler = NULL;


/***************************************
*    Private Function Prototypes
***************************************/

static void SPI_1_ScbEnableIntr(void);
static void SPI_1_ScbModeStop(void);


/*******************************************************************************
* Function Name: SPI_1_Init
********************************************************************************
*
* Summary:
*  Initializes SCB component to operate in one of selected configurations:
*  I2C, SPI, UART, EZI2C or EZSPI.
*  This function does not do any initialization when configuration is set to
*  Unconfigured SCB.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void SPI_1_Init(void)
{
#if(SPI_1_SCB_MODE_UNCONFIG_CONST_CFG)
    if(SPI_1_SCB_MODE_UNCONFIG_RUNTM_CFG)
    {
        SPI_1_initVar = 0u; /* Clear init var */
    }
    else
    {
        /* Initialization was done before call */
    }

#elif(SPI_1_SCB_MODE_I2C_CONST_CFG)
    SPI_1_I2CInit();

#elif(SPI_1_SCB_MODE_SPI_CONST_CFG)
    SPI_1_SpiInit();

#elif(SPI_1_SCB_MODE_UART_CONST_CFG)
    SPI_1_UartInit();

#elif(SPI_1_SCB_MODE_EZI2C_CONST_CFG)
    SPI_1_EzI2CInit();

#endif /* (SPI_1_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/*******************************************************************************
* Function Name: SPI_1_Enable
********************************************************************************
*
* Summary:
*  Enables SCB component operation.
*  The SCB configuration should be not changed when the component is enabled.
*  Any configuration changes should be made after disabling the component.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void SPI_1_Enable(void)
{
#if(SPI_1_SCB_MODE_UNCONFIG_CONST_CFG)
    if(!SPI_1_SCB_MODE_UNCONFIG_RUNTM_CFG)
    {
        /* Enable SCB block, only if already configured */
        SPI_1_CTRL_REG |= SPI_1_CTRL_ENABLED;
        
        /* Enable interrupt */
        SPI_1_ScbEnableIntr();
    }
#else
    SPI_1_CTRL_REG |= SPI_1_CTRL_ENABLED; /* Enable SCB block */
    
    SPI_1_ScbEnableIntr();
#endif /* (SPI_1_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/*******************************************************************************
* Function Name: SPI_1_Start
********************************************************************************
*
* Summary:
*  Invokes SCB_Init() and SCB_Enable().
*  After this function call the component is enabled and ready for operation.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  SPI_1_initVar - used to check initial configuration, modified
*  on first function call.
*
*******************************************************************************/
void SPI_1_Start(void)
{
    if(0u == SPI_1_initVar)
    {
        SPI_1_initVar = 1u; /* Component was initialized */
        SPI_1_Init();       /* Initialize component      */
    }

    SPI_1_Enable();
}


/*******************************************************************************
* Function Name: SPI_1_Stop
********************************************************************************
*
* Summary:
*  Disables the SCB component.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void SPI_1_Stop(void)
{
#if(SPI_1_SCB_IRQ_INTERNAL)
    SPI_1_SCB_IRQ_Disable();     /* Disable interrupt before block */
#endif /* (SPI_1_SCB_IRQ_INTERNAL) */

    SPI_1_CTRL_REG &= (uint32) ~SPI_1_CTRL_ENABLED;  /* Disable SCB block */

#if(SPI_1_SCB_IRQ_INTERNAL)
    SPI_1_SCB_IRQ_ClearPending(); /* Clear pending interrupt */
#endif /* (SPI_1_SCB_IRQ_INTERNAL) */
    
    SPI_1_ScbModeStop(); /* Calls scbMode specific Stop function */
}


/*******************************************************************************
* Function Name: SPI_1_SetCustomInterruptHandler
********************************************************************************
*
* Summary:
*  Registers a function to be called by the internal interrupt handler.
*  First the function that is registered is called, then the internal interrupt
*  handler performs any operations such as software buffer management functions
*  before the interrupt returns.  It is user's responsibility to not break the
*  software buffer operations. Only one custom handler is supported, which is
*  the function provided by the most recent call.
*  At initialization time no custom handler is registered.
*
* Parameters:
*  func: Pointer to the function to register.
*        The value NULL indicates to remove the current custom interrupt
*        handler.
*
* Return:
*  None
*
*******************************************************************************/
void SPI_1_SetCustomInterruptHandler(cyisraddress func)
{
    SPI_1_customIntrHandler = func; /* Register interrupt handler */
}


/*******************************************************************************
* Function Name: SPI_1_ScbModeEnableIntr
********************************************************************************
*
* Summary:
*  Enables interrupt for specific mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
static void SPI_1_ScbEnableIntr(void)
{
#if(SPI_1_SCB_IRQ_INTERNAL)
    #if(SPI_1_SCB_MODE_UNCONFIG_CONST_CFG)
        /* Enable interrupt source */
        if(0u != SPI_1_scbEnableIntr)
        {
            SPI_1_SCB_IRQ_Enable();
        }
    #else
        SPI_1_SCB_IRQ_Enable();
        
    #endif /* (SPI_1_SCB_MODE_UNCONFIG_CONST_CFG) */
#endif /* (SPI_1_SCB_IRQ_INTERNAL) */
}


/*******************************************************************************
* Function Name: SPI_1_ScbModeEnableIntr
********************************************************************************
*
* Summary:
*  Calls Stop function for specific operation mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
static void SPI_1_ScbModeStop(void)
{
#if(SPI_1_SCB_MODE_UNCONFIG_CONST_CFG)
    if(SPI_1_SCB_MODE_I2C_RUNTM_CFG)
    {
        SPI_1_I2CStop();
    }
    else if(SPI_1_SCB_MODE_EZI2C_RUNTM_CFG)
    {
        SPI_1_EzI2CStop();
    }
    else
    {
        /* None of modes above */
    }
#elif(SPI_1_SCB_MODE_I2C_CONST_CFG)
    SPI_1_I2CStop();

#elif(SPI_1_SCB_MODE_EZI2C_CONST_CFG)
    SPI_1_EzI2CStop();

#endif /* (SPI_1_SCB_MODE_UNCONFIG_CONST_CFG) */
}


#if(SPI_1_SCB_MODE_UNCONFIG_CONST_CFG)
    /*******************************************************************************
    * Function Name: SPI_1_SetPins
    ********************************************************************************
    *
    * Summary:
    *  Sets pins settings accordingly to selected operation mode.
    *  Only available in Unconfigured operation mode. The mode specific
    *  initialization function calls it.
    *  Pins configuration is set by PSoC Creator when specific mode of operation
    *  selected in design time.
    *
    * Parameters:
    *  mode:      Mode of SCB operation.
    *  subMode:   Submode of SCB operation. It is only required for SPI and UART
    *             modes.
    *  uartTxRx:  Direction for UART.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void SPI_1_SetPins(uint32 mode, uint32 subMode, uint32 uartTxRx)
    {
        uint32 hsiomSel[SPI_1_SCB_PINS_NUMBER];
        uint32 pinsDm  [SPI_1_SCB_PINS_NUMBER];
        uint32 i;

        /* Make all unused */
        for(i = 0u; i < SPI_1_SCB_PINS_NUMBER; i++)
        {
            hsiomSel[i] = SPI_1_HSIOM_DEF_SEL;
            pinsDm[i]   = SPI_1_PIN_DM_ALG_HIZ;
        }

        /* Choice the Dm and HSIOM */
        if((SPI_1_SCB_MODE_I2C   == mode) ||
           (SPI_1_SCB_MODE_EZI2C == mode))
        {
            hsiomSel[SPI_1_MOSI_SCL_RX_PIN_INDEX] = SPI_1_HSIOM_I2C_SEL;
            hsiomSel[SPI_1_MISO_SDA_TX_PIN_INDEX] = SPI_1_HSIOM_I2C_SEL;

            pinsDm[SPI_1_MOSI_SCL_RX_PIN_INDEX] = SPI_1_PIN_DM_OD_LO;
            pinsDm[SPI_1_MISO_SDA_TX_PIN_INDEX] = SPI_1_PIN_DM_OD_LO;
        }
        else if(SPI_1_SCB_MODE_SPI == mode)
        {
            hsiomSel[SPI_1_MOSI_SCL_RX_PIN_INDEX] = SPI_1_HSIOM_SPI_SEL;
            hsiomSel[SPI_1_MISO_SDA_TX_PIN_INDEX] = SPI_1_HSIOM_SPI_SEL;
            hsiomSel[SPI_1_SCLK_PIN_INDEX]        = SPI_1_HSIOM_SPI_SEL;

            if(SPI_1_SPI_SLAVE == subMode)
            {
                /* Slave */
                pinsDm[SPI_1_MOSI_SCL_RX_PIN_INDEX] = SPI_1_PIN_DM_DIG_HIZ;
                pinsDm[SPI_1_MISO_SDA_TX_PIN_INDEX] = SPI_1_PIN_DM_STRONG;
                pinsDm[SPI_1_SCLK_PIN_INDEX]        = SPI_1_PIN_DM_DIG_HIZ;

            #if(SPI_1_SS0_PIN)
                /* Only SS0 is valid choice for Slave */
                hsiomSel[SPI_1_SS0_PIN_INDEX] = SPI_1_HSIOM_SPI_SEL;
                pinsDm  [SPI_1_SS0_PIN_INDEX] = SPI_1_PIN_DM_DIG_HIZ;
            #endif /* (SPI_1_SS1_PIN) */
            }
            else /* (Master) */
            {
                pinsDm[SPI_1_MOSI_SCL_RX_PIN_INDEX] = SPI_1_PIN_DM_STRONG;
                pinsDm[SPI_1_MISO_SDA_TX_PIN_INDEX] = SPI_1_PIN_DM_DIG_HIZ;
                pinsDm[SPI_1_SCLK_PIN_INDEX]        = SPI_1_PIN_DM_STRONG;

            #if(SPI_1_SS0_PIN)
                hsiomSel[SPI_1_SS0_PIN_INDEX] = SPI_1_HSIOM_SPI_SEL;
                pinsDm  [SPI_1_SS0_PIN_INDEX] = SPI_1_PIN_DM_STRONG;
            #endif /* (SPI_1_SS0_PIN) */

            #if(SPI_1_SS1_PIN)
                hsiomSel[SPI_1_SS1_PIN_INDEX] = SPI_1_HSIOM_SPI_SEL;
                pinsDm  [SPI_1_SS1_PIN_INDEX] = SPI_1_PIN_DM_STRONG;
            #endif /* (SPI_1_SS1_PIN) */

            #if(SPI_1_SS2_PIN)
                hsiomSel[SPI_1_SS2_PIN_INDEX] = SPI_1_HSIOM_SPI_SEL;
                pinsDm  [SPI_1_SS2_PIN_INDEX] = SPI_1_PIN_DM_STRONG;
            #endif /* (SPI_1_SS2_PIN) */

            #if(SPI_1_SS3_PIN)
                hsiomSel[SPI_1_SS3_PIN_INDEX] = SPI_1_HSIOM_SPI_SEL;
                pinsDm  [SPI_1_SS3_PIN_INDEX] = SPI_1_PIN_DM_STRONG;
            #endif /* (SPI_1_SS2_PIN) */
            }
        }
        else /* UART */
        {
            if(SPI_1_UART_MODE_SMARTCARD == subMode)
            {
                /* SmartCard */
                hsiomSel[SPI_1_MISO_SDA_TX_PIN_INDEX] = SPI_1_HSIOM_UART_SEL;
                pinsDm  [SPI_1_MISO_SDA_TX_PIN_INDEX] = SPI_1_PIN_DM_OD_LO;
            }
            else /* Standard or IrDA */
            {
                if(0u != (SPI_1_UART_RX & uartTxRx))
                {
                    hsiomSel[SPI_1_MOSI_SCL_RX_PIN_INDEX] = SPI_1_HSIOM_UART_SEL;
                    pinsDm  [SPI_1_MOSI_SCL_RX_PIN_INDEX] = SPI_1_PIN_DM_DIG_HIZ;
                }

                if(0u != (SPI_1_UART_TX & uartTxRx))
                {
                    hsiomSel[SPI_1_MISO_SDA_TX_PIN_INDEX] = SPI_1_HSIOM_UART_SEL;
                    pinsDm  [SPI_1_MISO_SDA_TX_PIN_INDEX] = SPI_1_PIN_DM_STRONG;
                }
            }
        }

        /* Condfigure pins: set HSIOM and DM */
        /* Condfigure pins: DR registers configuration remains unchanged for cyfitter_cfg() */

    #if(SPI_1_MOSI_SCL_RX_PIN)
        SPI_1_SET_HSIOM_SEL(SPI_1_MOSI_SCL_RX_HSIOM_REG,
                                       SPI_1_MOSI_SCL_RX_HSIOM_MASK,
                                       SPI_1_MOSI_SCL_RX_HSIOM_POS,
                                       hsiomSel[SPI_1_MOSI_SCL_RX_PIN_INDEX]);
    #endif /* (SPI_1_MOSI_SCL_RX_PIN) */

    #if(SPI_1_MOSI_SCL_RX_WAKE_PIN)
        SPI_1_SET_HSIOM_SEL(SPI_1_MOSI_SCL_RX_WAKE_HSIOM_REG,
                                       SPI_1_MOSI_SCL_RX_WAKE_HSIOM_MASK,
                                       SPI_1_MOSI_SCL_RX_WAKE_HSIOM_POS,
                                       hsiomSel[SPI_1_MOSI_SCL_RX_WAKE_PIN_INDEX]);
    #endif /* (SPI_1_MOSI_SCL_RX_WAKE_PIN) */

    #if(SPI_1_MISO_SDA_TX_PIN)
        SPI_1_SET_HSIOM_SEL(SPI_1_MISO_SDA_TX_HSIOM_REG,
                                       SPI_1_MISO_SDA_TX_HSIOM_MASK,
                                       SPI_1_MISO_SDA_TX_HSIOM_POS,
                                       hsiomSel[SPI_1_MISO_SDA_TX_PIN_INDEX]);
    #endif /* (SPI_1_MOSI_SCL_RX_PIN) */

    #if(SPI_1_SCLK_PIN)
        SPI_1_SET_HSIOM_SEL(SPI_1_SCLK_HSIOM_REG, SPI_1_SCLK_HSIOM_MASK,
                                       SPI_1_SCLK_HSIOM_POS, hsiomSel[SPI_1_SCLK_PIN_INDEX]);
    #endif /* (SPI_1_SCLK_PIN) */

    #if(SPI_1_SS0_PIN)
        SPI_1_SET_HSIOM_SEL(SPI_1_SS0_HSIOM_REG, SPI_1_SS0_HSIOM_MASK,
                                       SPI_1_SS0_HSIOM_POS, hsiomSel[SPI_1_SS0_PIN_INDEX]);
    #endif /* (SPI_1_SS1_PIN) */

    #if(SPI_1_SS1_PIN)
        SPI_1_SET_HSIOM_SEL(SPI_1_SS1_HSIOM_REG, SPI_1_SS1_HSIOM_MASK,
                                       SPI_1_SS1_HSIOM_POS, hsiomSel[SPI_1_SS1_PIN_INDEX]);
    #endif /* (SPI_1_SS1_PIN) */

    #if(SPI_1_SS2_PIN)
        SPI_1_SET_HSIOM_SEL(SPI_1_SS2_HSIOM_REG, SPI_1_SS2_HSIOM_MASK,
                                       SPI_1_SS2_HSIOM_POS, hsiomSel[SPI_1_SS2_PIN_INDEX]);
    #endif /* (SPI_1_SS2_PIN) */

    #if(SPI_1_SS3_PIN)
        SPI_1_SET_HSIOM_SEL(SPI_1_SS3_HSIOM_REG,  SPI_1_SS3_HSIOM_MASK,
                                       SPI_1_SS3_HSIOM_POS, hsiomSel[SPI_1_SS3_PIN_INDEX]);
    #endif /* (SPI_1_SS3_PIN) */



    #if(SPI_1_MOSI_SCL_RX_PIN)
        SPI_1_spi_mosi_i2c_scl_uart_rx_SetDriveMode((uint8)
                                                               pinsDm[SPI_1_MOSI_SCL_RX_PIN_INDEX]);
    #endif /* (SPI_1_MOSI_SCL_RX_PIN) */

    #if(SPI_1_MOSI_SCL_RX_WAKE_PIN)
    SPI_1_spi_mosi_i2c_scl_uart_rx_wake_SetDriveMode((uint8)
                                                               pinsDm[SPI_1_MOSI_SCL_RX_WAKE_PIN_INDEX]);

    /* Set interrupt on rising edge */
    SPI_1_SET_INCFG_TYPE(SPI_1_MOSI_SCL_RX_WAKE_INTCFG_REG,
                                    SPI_1_MOSI_SCL_RX_WAKE_INTCFG_TYPE_MASK,
                                    SPI_1_MOSI_SCL_RX_WAKE_INTCFG_TYPE_POS,
                                    SPI_1_INTCFG_TYPE_FALLING_EDGE);

    #endif /* (SPI_1_MOSI_SCL_RX_WAKE_PIN) */

    #if(SPI_1_MISO_SDA_TX_PIN)
        SPI_1_spi_miso_i2c_sda_uart_tx_SetDriveMode((uint8)
                                                                    pinsDm[SPI_1_MISO_SDA_TX_PIN_INDEX]);
    #endif /* (SPI_1_MOSI_SCL_RX_PIN) */

    #if(SPI_1_SCLK_PIN)
        SPI_1_spi_sclk_SetDriveMode((uint8) pinsDm[SPI_1_SCLK_PIN_INDEX]);
    #endif /* (SPI_1_SCLK_PIN) */

    #if(SPI_1_SS0_PIN)
        SPI_1_spi_ss0_SetDriveMode((uint8) pinsDm[SPI_1_SS0_PIN_INDEX]);
    #endif /* (SPI_1_SS0_PIN) */

    #if(SPI_1_SS1_PIN)
        SPI_1_spi_ss1_SetDriveMode((uint8) pinsDm[SPI_1_SS1_PIN_INDEX]);
    #endif /* (SPI_1_SS1_PIN) */

    #if(SPI_1_SS2_PIN)
        SPI_1_spi_ss2_SetDriveMode((uint8) pinsDm[SPI_1_SS2_PIN_INDEX]);
    #endif /* (SPI_1_SS2_PIN) */

    #if(SPI_1_SS3_PIN)
        SPI_1_spi_ss3_SetDriveMode((uint8) pinsDm[SPI_1_SS3_PIN_INDEX]);
    #endif /* (SPI_1_SS3_PIN) */
    }

#endif /* (SPI_1_SCB_MODE_UNCONFIG_CONST_CFG) */


/* [] END OF FILE */
