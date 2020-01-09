/**
 * DESCRIPTION:
 *      BSP_Aux_Mini_UART provides an interface for the auxilliary mini uart.
 * 
 * NOTES:
 *      TODO: This module has just been started, most addresses have been defined, as well
 *      as some bit masks for the mini uart, but consider this just a rough start.
 * 
 *      Mini uart init, set baud rate, and write bytes are started, but not thoroughly 
 *      verified. In particular, need to check that the baud rate enums actually
 *      result in the proper baud rates.
 * 
 * REFERENCES:
 *      BCM2837-ARM-Peripherals.pdf page 8
 */

#ifndef BSP_AUX_MINI_UART_H_INCLUDED
#define BSP_AUX_MINI_UART_H_INCLUDED

#include "Fixed_Width_Ints.h"



/*-----------------------------------------------------------------------------------------------
    Public BSP_Aux_Mini_UART Defines
 -------------------------------------------------------------------------------------------------*/

// mini uart pins
#define BSP_AUX_MINI_UART_TX_PIN 14u
#define BSP_AUX_MINI_UART_RX_PIN 15u



/*-----------------------------------------------------------------------------------------------
    Public BSP_Aux_Mini_UART Types
 -------------------------------------------------------------------------------------------------*/

// mini uart baud rate enum
typedef enum
{
    BSP_AUX_Mini_Uart_Baud_Rate_9600   = 3254u, // sets the mini uart baud rate to 9600
    BSP_AUX_Mini_Uart_Baud_Rate_14400  = 2169u, // sets the mini uart baud rate to 14400
    BSP_AUX_Mini_Uart_Baud_Rate_19200  = 1627u, // sets the mini uart baud rate to 19200
    BSP_AUX_Mini_Uart_Baud_Rate_28800  = 1084u, // sets the mini uart baud rate to 28800
    BSP_AUX_Mini_Uart_Baud_Rate_38400  = 813u,  // sets the mini uart baud rate to 38400
    BSP_AUX_Mini_Uart_Baud_Rate_56000  = 557u,  // sets the mini uart baud rate to 56000
    BSP_AUX_Mini_Uart_Baud_Rate_57600  = 542u,  // sets the mini uart baud rate to 57600
    BSP_AUX_Mini_Uart_Baud_Rate_115200 = 270u   // sets the mini uart baud rate to 115200
} BSP_AUX_Mini_Uart_Baud_Rate_t;


/*-----------------------------------------------------------------------------------------------
    Public BSP_Aux_Mini_UART Function Declarations
 -------------------------------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------------------------

Function Name:
    BSP_AUX_Mini_Uart_Init

Function Description:
    Initialize the aux mini uart by setting GPIO pins 14 and 15 to alt function 5, disabling
    mini uart interrupts, enabling 8-bit mode, and setting the baud rate to the given parameter.

Inputs:
    baud_rate_enum: the initial baud rate to use.

Returns:
    None.

Error Handling:
    None

-------------------------------------------------------------------------------------------------*/
void BSP_AUX_Mini_Uart_Init(BSP_AUX_Mini_Uart_Baud_Rate_t baud_rate_enum);




/*-----------------------------------------------------------------------------------------------

Function Name:
    BSP_AUX_Mini_Uart_Set_Baud_Rate

Function Description:
    Set the baud rate of the aux mini uart with a given baud rate.

Inputs:
    baud_rate_enum: baud rate enum to use. 
    
    The formula used for baud rate is : baudrate = system_clock_freq / (8 * ( baudrate_reg + 1 )) 
    where system_clock_freq is 250MHz.

    The provided enums are precomputed to deliver the correct baud rate setting for typical baud rates.

Returns:
    None.

Error Handling:
    None

-------------------------------------------------------------------------------------------------*/
void BSP_AUX_Mini_Uart_Set_Baud_Rate(BSP_AUX_Mini_Uart_Baud_Rate_t baud_rate_enum);



/*-----------------------------------------------------------------------------------------------

Function Name:
    BSP_AUX_Mini_Uart_Send_Byte

Function Description:
    Send a byte of data via mini uart Tx.

Inputs:
    value: the value of the byte to send.

Returns:
    None.

Error Handling:
    None

-------------------------------------------------------------------------------------------------*/
void BSP_AUX_Mini_Uart_Send_Byte(uint8_t value);



/*-----------------------------------------------------------------------------------------------

Function Name:
    BSP_AUX_Mini_Uart_Send_String

Function Description:
    Send a C-String via mini uart Tx.

Inputs:
    c_string: the C_String to send.

Returns:
    None.

Error Handling:
    None

-------------------------------------------------------------------------------------------------*/
void BSP_AUX_Mini_Uart_Send_String(char* c_string);



#endif
