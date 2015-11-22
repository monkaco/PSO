/******************************************************************************
* FILENAME:    pso_uart.c
*
* DESCRIPTION:
*       High-level functions for UART modules.
*
* FUNCTIONS:
*    uint8_t uartBatchWrite ( uint8_t, uint8_t*, uint8_t)
*    uint8_t uartBatchRead ( uint8_t, uint8_t, uint8_t*, uint8_t)
*
* NOTES:
*       Functions to implement high-level procedures of the I2C peripheral.
*
* REFERENCES:
*       Adapted from CH Robotics.
*
* START DATE:    04 Apr 2015
*
* CHANGES :
*
* VERSION DATE        WHO                    DETAIL
* 1.0     04 Apr 2015 Rogerio Lima         Start-up coding.
******************************************************************************/

/*
 * SECTION ORDER
 *     1. Comment block
 *     2. Include statements
 *     3. Definitions: data type, constants, macros
 *     4. Static data declarations
 *     5. Private functions prototypes
 *     6. Public function bodies
 *     7. Private function bodies
 *
 */

#include <stdint.h>
#include "hw_uart.h"
#include "hw_types.h"
#include "pso_uart.h"

uart_raw_data_t* g_uart0_data;
uint8_t* g_tx_buffer_uart;

/*******************************************************************************
* Function Name  : uartBatchWrite
* Input          : uint32_t ui32Base, uint8_t* txBuffer, uint8_t bytesToWrite
* Output         : uint8_t* status_flag
* Return         : 1 if success, 0 if fail
* Description    : Writes a pre-defined number of bytes (bytesToWrite) stored
*                  in (txBuffer) into the UART addressed by (ui32Base) address.
*******************************************************************************/
uint8_t uartBatchWrite (uint32_t ui32Base, uint16_t* txBuffer, const uint8_t bytesToWrite)
{
    uint16_t i;
    uint8_t returnval;
    uint8_t buffer[255];

/*
	for (i = 0; i < 2*bytesToWrite; i++)
	{
		if(i%2==0)
		{
			buffer[i] = txBuffer[i/2];
		}
		else
		{
			buffer[i] = txBuffer[i/2] >> 8;
		}
	}

    for (i = 0U; i < 2*bytesToWrite; i++)
    {
    	// Wait until space is available.
         while(HWREG(ui32Base + UART_O_FR) & UART_FR_TXFF)
         {
         }
         // Send the char.
         HWREG(ui32Base + UART_O_DR) =  buffer[i];



    }
*/

    for (i = 0U; i < bytesToWrite; i++)
    {
    	// Wait until space is available.
         while(HWREG(ui32Base + UART_O_FR) & UART_FR_TXFF)
         {
         }
         // Send the char.
         HWREG(ui32Base + UART_O_DR) =  txBuffer[i];
    }

    return returnval;
}


/*******************************************************************************
* Function Name  : i2cBatchRead
* Input          : uint8_t bytesToRead,
* Output         : uint8_t* rxBuffer
* Return         : 1 if success, 0 if fail
* Description    : Reads a pre-defined number of bytes (bytesToRead) stored
*                  in (rxBuffer).
*******************************************************************************/
uint8_t i2cBatchRead (uint32_t ui32Base, uint8_t* rxBuffer, uint8_t bytesToRead)
{
    static int16_t i;
    static uint8_t returnval;
/*

    I2caRegs.I2CSAR = address7;                     I2C slave address register

    I2caRegs.I2CCNT = 1;                            Enviar 1 byte

    // Send start address for read
    I2caRegs.I2CDXR = device_start_address;         Endereço do registrador

     1 = a previous cycle has completed (wait for access ready condition)
    while (I2caRegs.I2CSTR.bit.ARDY == 0U);

     0: ACK, 1: NACK
    returnval = I2caRegs.I2CSTR.bit.NACK;

      Send new start condition and initiate read
    I2caRegs.I2CCNT    = bytesToRead;
     Master-receiver, Start, Stop after all bytes trans
    I2caRegs.I2CMDR.all = I2caRegsShadow.I2CMDR.all;;

    for (i = 0; i < bytesToRead; i++)
    {
    	 Aguarda dado ficar disponível no registrador
        while (I2caRegs.I2CSTR.bit.RRDY == I2C_RRDY_I2CDRR_NOT_READY);
         Lê dado do buffer do DSP
        rxBuffer[i] = I2caRegs.I2CDRR;
    }

     Aguarda Stop condition
    while (I2caRegs.I2CSTR.bit.SCD == I2C_SCD_NOT_DETECTED);
     Write 1 to clear the flag
    I2caRegs.I2CSTR.bit.SCD = I2C_SCD_DETECTED;
*/

    return returnval;
}
