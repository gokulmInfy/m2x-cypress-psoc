/*******************************************************************************
* File Name: SPI_1_PM.c
* Version 1.10
*
* Description:
*  This file provides the source code to the Power Management support for
*  the SCB Component.
*
* Note:
*
********************************************************************************
* Copyright 2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "SPI_1.h"
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


/***************************************
*   Backup Structure declaration
***************************************/

SPI_1_BACKUP_STRUCT SPI_1_backup =
{
    0u, /* enableState */
};


/*******************************************************************************
* Function Name: SPI_1_Sleep
********************************************************************************
*
* Summary:
*  Calls SaveConfig function fucntion for selected mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void SPI_1_Sleep(void)
{
#if(SPI_1_SCB_MODE_UNCONFIG_CONST_CFG)

    if(0u != SPI_1_scbEnableWake)
    {
        if(SPI_1_SCB_MODE_I2C_RUNTM_CFG)
        {
            SPI_1_I2CSaveConfig();
        }
        else if(SPI_1_SCB_MODE_SPI_RUNTM_CFG)
        {
            SPI_1_SpiSaveConfig();
        }
        else if(SPI_1_SCB_MODE_UART_RUNTM_CFG)
        {
            SPI_1_UartSaveConfig();
        }
        else if(SPI_1_SCB_MODE_EZI2C_RUNTM_CFG)
        {
            SPI_1_EzI2CSaveConfig();
        }
        else
        {
            /* Unknown mode: do nothing */
        }
    }
    else
    {
        SPI_1_backup.enableState = (uint8) SPI_1_GET_CTRL_ENABLED;
        
        if(0u != SPI_1_backup.enableState)
        {
            SPI_1_Stop();
        }
    }
    
    SPI_1_DisableTxPinsInputBuffer();
    
#else
    
    #if defined (SPI_1_I2C_WAKE_ENABLE_CONST) && (SPI_1_I2C_WAKE_ENABLE_CONST)
        SPI_1_I2CSaveConfig();
        
    #elif defined (SPI_1_SPI_WAKE_ENABLE_CONST) && (SPI_1_SPI_WAKE_ENABLE_CONST)
        SPI_1_SpiSaveConfig();
        
    #elif defined (SPI_1_UART_WAKE_ENABLE_CONST) && (SPI_1_UART_WAKE_ENABLE_CONST)
        SPI_1_UartSaveConfig();
        
    #elif defined (SPI_1_EZI2C_WAKE_ENABLE_CONST) && (SPI_1_EZI2C_WAKE_ENABLE_CONST)
        SPI_1_EzI2CSaveConfig();
    
    #else
        
        SPI_1_backup.enableState = (uint8) SPI_1_GET_CTRL_ENABLED;
        
        /* Check enable state */
        if(0u != SPI_1_backup.enableState)
        {
            SPI_1_Stop();
        }
        
    #endif /* defined (SPI_1_SCB_MODE_I2C_CONST_CFG) && (SPI_1_I2C_WAKE_ENABLE_CONST) */
    
    SPI_1_DisableTxPinsInputBuffer();
    
#endif /* (SPI_1_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/*******************************************************************************
* Function Name: SPI_1_Wakeup
********************************************************************************
*
* Summary:
*  Calls RestoreConfig function fucntion for selected mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void SPI_1_Wakeup(void)
{
#if(SPI_1_SCB_MODE_UNCONFIG_CONST_CFG)

    SPI_1_EnableTxPinsInputBuffer();
        
    if(0u != SPI_1_scbEnableWake)
    {
        if(SPI_1_SCB_MODE_I2C_RUNTM_CFG)
        {
            SPI_1_I2CRestoreConfig();
        }
        else if(SPI_1_SCB_MODE_SPI_RUNTM_CFG)
        {
            SPI_1_SpiRestoreConfig();
        }
        else if(SPI_1_SCB_MODE_UART_RUNTM_CFG)
        {
            SPI_1_UartRestoreConfig();
        }
        else if(SPI_1_SCB_MODE_EZI2C_RUNTM_CFG)
        {
            SPI_1_EzI2CRestoreConfig();
        }
        else
        {
            /* Unknown mode: do nothing */
        }
    }
    else
    {    
        /* Restore enable state */
        if(0u != SPI_1_backup.enableState)
        {
            SPI_1_Enable();
        }
    }

#else
    
    SPI_1_EnableTxPinsInputBuffer();
        
    #if defined (SPI_1_I2C_WAKE_ENABLE_CONST) && (SPI_1_I2C_WAKE_ENABLE_CONST)
        SPI_1_I2CRestoreConfig();
        
    #elif defined (SPI_1_SPI_WAKE_ENABLE_CONST) && (SPI_1_SPI_WAKE_ENABLE_CONST)
        SPI_1_SpiRestoreConfig();
        
    #elif defined (SPI_1_UART_WAKE_ENABLE_CONST) && (SPI_1_UART_WAKE_ENABLE_CONST)
        SPI_1_UartRestoreConfig();
        
    #elif defined (SPI_1_EZI2C_WAKE_ENABLE_CONST) && (SPI_1_EZI2C_WAKE_ENABLE_CONST)
        SPI_1_EzI2CRestoreConfig();
    
    #else
        /* Check enable state */
        if(0u != SPI_1_backup.enableState)
        {
            SPI_1_Enable();
        }
        
    #endif /* (SPI_1_I2C_WAKE_ENABLE_CONST) */

#endif /* (SPI_1_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/*******************************************************************************
* Function Name: SPI_1_DisableTxPinsInputBuffer
********************************************************************************
*
* Summary:
*  Disables input buffers for TX pins. This action removes leakage current while
*  low power mode (Cypress ID 149635).
*   SCB mode is I2C and EZI2C: bus is pulled-up. Leave pins as it is.
*   SCB mode SPI:
*     Slave  - disable input buffer for MISO pin.
*     Master - disable input buffer for all pins.
*   SCB mode SmartCard: 
*     Standard and IrDA - disable input buffer for TX pin.
*     SmartCard - RX_TX pin is pulled-up. Leave pin as it is.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void SPI_1_DisableTxPinsInputBuffer(void)
{
#if(SPI_1_SCB_MODE_UNCONFIG_CONST_CFG)
    
    /* SCB mode is I2C and EZI2C: bus is pulled-up. Does nothing */
       
    if(SPI_1_SCB_MODE_SPI_RUNTM_CFG)
    {
        if(0u != (SPI_1_SPI_CTRL_REG & SPI_1_SPI_CTRL_MASTER))
        /* SPI Master */
        {
        #if(SPI_1_MOSI_SCL_RX_WAKE_PIN)
            SPI_1_spi_mosi_i2c_scl_uart_rx_wake_INP_DIS |= \
                                                                SPI_1_spi_mosi_i2c_scl_uart_rx_wake_MASK;
        #endif /* (SPI_1_MOSI_SCL_RX_WAKE_PIN) */

        #if(SPI_1_MOSI_SCL_RX_PIN)
            SPI_1_spi_mosi_i2c_scl_uart_rx_INP_DIS |= SPI_1_spi_mosi_i2c_scl_uart_rx_MASK;
        #endif /* (SPI_1_MOSI_SCL_RX_PIN) */

        #if(SPI_1_MISO_SDA_TX_PIN)
            SPI_1_spi_miso_i2c_sda_uart_tx_INP_DIS |= SPI_1_spi_miso_i2c_sda_uart_tx_MASK;
        #endif /* (SPI_1_MISO_SDA_TX_PIN_PIN) */

        #if(SPI_1_SCLK_PIN)
            SPI_1_spi_sclk_INP_DIS |= SPI_1_spi_sclk_MASK;
        #endif /* (SPI_1_SCLK_PIN) */

        #if(SPI_1_SS0_PIN)
            SPI_1_spi_ss0_INP_DIS |= SPI_1_spi_ss0_MASK;
        #endif /* (SPI_1_SS1_PIN) */

        #if(SPI_1_SS1_PIN)
            SPI_1_spi_ss1_INP_DIS |= SPI_1_spi_ss1_MASK;
        #endif /* (SPI_1_SS1_PIN) */

        #if(SPI_1_SS2_PIN)
            SPI_1_spi_ss2_INP_DIS |= SPI_1_spi_ss2_MASK;
        #endif /* (SPI_1_SS2_PIN) */

        #if(SPI_1_SS3_PIN)
            SPI_1_spi_ss3_INP_DIS |= SPI_1_spi_ss3_MASK;
        #endif /* (SPI_1_SS3_PIN) */
        }
        else
        /* SPI Slave */
        {
        #if(SPI_1_MISO_SDA_TX_PIN)
            SPI_1_spi_miso_i2c_sda_uart_tx_INP_DIS |= SPI_1_spi_miso_i2c_sda_uart_tx_MASK;
        #endif /* (SPI_1_MISO_SDA_TX_PIN_PIN) */
        }
    }
    else if (SPI_1_SCB_MODE_UART_RUNTM_CFG)
    {
        if(SPI_1_UART_CTRL_MODE_UART_SMARTCARD != 
            (SPI_1_UART_CTRL_REG & SPI_1_UART_CTRL_MODE_MASK))
        /* UART Standard or IrDA */
        {
        #if(SPI_1_MISO_SDA_TX_PIN)
            SPI_1_spi_miso_i2c_sda_uart_tx_INP_DIS |= SPI_1_spi_miso_i2c_sda_uart_tx_MASK;
        #endif /* (SPI_1_MISO_SDA_TX_PIN_PIN) */
        }
    }
    else
    {
        /* Does nothing */
    }
    
#else
    
    /* SCB mode is I2C and EZI2C: bus is pulled-up. Does nothing */
        
    /* SCB mode is SPI Master */
    #if(SPI_1_SPI_MASTER_PINS)
        SPI_1_sclk_m_INP_DIS |= SPI_1_sclk_m_MASK;
        SPI_1_mosi_m_INP_DIS |= SPI_1_mosi_m_MASK;
        SPI_1_miso_m_INP_DIS |= SPI_1_miso_m_MASK;
    #endif /* (SPI_1_SPI_MASTER_PINS) */

    #if(SPI_1_SPI_MASTER_SS0_PIN)
        SPI_1_ss0_m_INP_DIS |= SPI_1_ss0_m_MASK;
    #endif /* (SPI_1_SPI_MASTER_SS0_PIN) */

    #if(SPI_1_SPI_MASTER_SS1_PIN)
        SPI_1_ss1_m_INP_DIS |= SPI_1_ss1_m_MASK;
    #endif /* (SPI_1_SPI_MASTER_SS1_PIN) */

    #if(SPI_1_SPI_MASTER_SS2_PIN)
        SPI_1_ss2_m_INP_DIS |= SPI_1_ss2_m_MASK;
    #endif /* (SPI_1_SPI_MASTER_SS2_PIN) */

    #if(SPI_1_SPI_MASTER_SS3_PIN)
        SPI_1_ss3_m_INP_DIS |= SPI_1_ss3_m_MASK;
    #endif /* (SPI_1_SPI_MASTER_SS3_PIN) */
    
    /* SCB mode is SPI Slave */
    #if(SPI_1_SPI_SLAVE_PINS)
        SPI_1_miso_s_INP_DIS |= SPI_1_miso_s_MASK;
    #endif /* (SPI_1_SPI_SLAVE_PINS) */

    /* SCB mode is UART */
    #if(SPI_1_UART_TX_PIN)
        SPI_1_tx_INP_DIS |= SPI_1_tx_MASK;
    #endif /* (SPI_1_UART_TX_PIN) */

#endif /* (SPI_1_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/*******************************************************************************
* Function Name: SPI_1_EnableTxPinsInputBuffer
********************************************************************************
*
* Summary:
*  Enables input buffers for TX pins. Restore changes done byte
*SPI_1_DisableTxPinsInputBuffer.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void SPI_1_EnableTxPinsInputBuffer(void)
{
#if(SPI_1_SCB_MODE_UNCONFIG_CONST_CFG)
    if(SPI_1_SCB_MODE_SPI_RUNTM_CFG)
    {
        if(0u != (SPI_1_SPI_CTRL_REG & SPI_1_SPI_CTRL_MASTER))
        /* SPI Master */
        {
        #if(SPI_1_MOSI_SCL_RX_WAKE_PIN)
            SPI_1_spi_mosi_i2c_scl_uart_rx_wake_INP_DIS &= \
                                            (uint32) ~((uint32) SPI_1_spi_mosi_i2c_scl_uart_rx_wake_MASK);
        #endif /* (SPI_1_MOSI_SCL_RX_WAKE_PIN) */

        #if(SPI_1_MOSI_SCL_RX_PIN)
            SPI_1_spi_mosi_i2c_scl_uart_rx_INP_DIS &= \
                                            (uint32) ~((uint32) SPI_1_spi_mosi_i2c_scl_uart_rx_MASK);
        #endif /* (SPI_1_MOSI_SCL_RX_PIN) */

        #if(SPI_1_MISO_SDA_TX_PIN)
            SPI_1_spi_miso_i2c_sda_uart_tx_INP_DIS &= \
                                            (uint32) ~((uint32) SPI_1_spi_miso_i2c_sda_uart_tx_MASK);
        #endif /* (SPI_1_MISO_SDA_TX_PIN_PIN) */

        #if(SPI_1_SCLK_PIN)
            SPI_1_spi_sclk_INP_DIS &= (uint32) ~((uint32) SPI_1_spi_sclk_MASK);
        #endif /* (SPI_1_SCLK_PIN) */

        #if(SPI_1_SS0_PIN)
            SPI_1_spi_ss0_INP_DIS &= (uint32) ~((uint32) SPI_1_spi_ss0_MASK);
        #endif /* (SPI_1_SS1_PIN) */

        #if(SPI_1_SS1_PIN)
            SPI_1_spi_ss1_INP_DIS &= (uint32) ~((uint32) SPI_1_spi_ss1_MASK);
        #endif /* (SPI_1_SS1_PIN) */

        #if(SPI_1_SS2_PIN)
            SPI_1_spi_ss2_INP_DIS &= (uint32) ~((uint32) SPI_1_spi_ss2_MASK);
        #endif /* (SPI_1_SS2_PIN) */

        #if(SPI_1_SS3_PIN)
            SPI_1_spi_ss3_INP_DIS &= (uint32) ~((uint32) SPI_1_spi_ss3_MASK);
        #endif /* (SPI_1_SS3_PIN) */
        }
        else
        /* SPI Slave */
        {
        #if(SPI_1_MISO_SDA_TX_PIN)
            SPI_1_spi_miso_i2c_sda_uart_tx_INP_DIS &= \
                                                (uint32) ~((uint32) SPI_1_spi_miso_i2c_sda_uart_tx_MASK);
        #endif /* (SPI_1_MISO_SDA_TX_PIN_PIN) */
        }
    }
    else if (SPI_1_SCB_MODE_UART_RUNTM_CFG)
    {
        if(SPI_1_UART_CTRL_MODE_UART_SMARTCARD != 
                (SPI_1_UART_CTRL_REG & SPI_1_UART_CTRL_MODE_MASK))
        /* UART Standard or IrDA */
        {
        #if(SPI_1_MISO_SDA_TX_PIN)
            SPI_1_spi_miso_i2c_sda_uart_tx_INP_DIS &= \
                                                (uint32) ~((uint32) SPI_1_spi_miso_i2c_sda_uart_tx_MASK);
        #endif /* (SPI_1_MISO_SDA_TX_PIN_PIN) */
        }
    }
    else
    {
        /* Does nothing */
    }
    
#else
        
    /* SCB mode is SPI Master */
    #if(SPI_1_SPI_MASTER_PINS)
        SPI_1_sclk_m_INP_DIS &= (uint32) ~((uint32) SPI_1_sclk_m_MASK);
        SPI_1_mosi_m_INP_DIS &= (uint32) ~((uint32) SPI_1_mosi_m_MASK);
        SPI_1_miso_m_INP_DIS &= (uint32) ~((uint32) SPI_1_miso_m_MASK);
    #endif /* (SPI_1_SPI_MASTER_PINS) */

    #if(SPI_1_SPI_MASTER_SS0_PIN)
        SPI_1_ss0_m_INP_DIS &= (uint32) ~((uint32) SPI_1_ss0_m_MASK);
    #endif /* (SPI_1_SPI_MASTER_SS0_PIN) */

    #if(SPI_1_SPI_MASTER_SS1_PIN)
        SPI_1_ss1_m_INP_DIS &= (uint32) ~((uint32) SPI_1_ss1_m_MASK);
    #endif /* (SPI_1_SPI_MASTER_SS1_PIN) */

    #if(SPI_1_SPI_MASTER_SS2_PIN)
        SPI_1_ss2_m_INP_DIS &= (uint32) ~((uint32) SPI_1_ss2_m_MASK);
    #endif /* (SPI_1_SPI_MASTER_SS2_PIN) */

    #if(SPI_1_SPI_MASTER_SS3_PIN)
        SPI_1_ss3_m_INP_DIS &= (uint32) ~((uint32) SPI_1_ss3_m_MASK);
    #endif /* (SPI_1_SPI_MASTER_SS3_PIN) */
    
    /* SCB mode is SPI Slave */
    #if(SPI_1_SPI_SLAVE_PINS)
        SPI_1_miso_s_INP_DIS &= (uint32) ~((uint32) SPI_1_miso_s_MASK);
    #endif /* (SPI_1_SPI_SLAVE_PINS) */

    /* SCB mode is UART */
    #if(SPI_1_UART_TX_PIN)
        SPI_1_tx_INP_DIS &= (uint32) ~((uint32) SPI_1_tx_MASK);
    #endif /* (SPI_1_UART_TX_PIN) */

#endif /* (SPI_1_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/* [] END OF FILE */
