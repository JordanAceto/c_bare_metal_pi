/*
--|----------------------------------------------------------------------------|
--| FILE DESCRIPTION:
--|   PSP_SPI_0 provides an interface for using SPI 0.
--|  
--|----------------------------------------------------------------------------|
--| NOTES:
--|     TODO: Writing data has been tested, but reading data has not. To do so,
--|     I'll need to set up some SPI device to talk back to the Pi and run some
--|     tests. Until then, reading data does not work.
--|  
--|----------------------------------------------------------------------------|
--| REFERENCES:
--|   BCM2837-ARM-Peripherals.pdf page 148
--|
--|----------------------------------------------------------------------------|
*/

#ifndef SPI_0_H_INCLUDED
#define SPI_0_H_INCLUDED

/*
--|----------------------------------------------------------------------------|
--| INCLUDE FILES
--|----------------------------------------------------------------------------|
*/

#include "Fixed_Width_Ints.h"

/*
--|----------------------------------------------------------------------------|
--| PUBLIC DEFINES
--|----------------------------------------------------------------------------|
*/

/*
--| NAME: PSP_SPI_0_xxx_PIN
--| DESCRIPTION: GPIO pin numbers used by SPI 0
--| TYPE: uint32_t
*/
#define PSP_SPI_0_CE1_PIN   (7u)
#define PSP_SPI_0_CE0_PIN   (8u)
#define PSP_SPI_0_MISO_PIN  (9u)
#define PSP_SPI_0_MOSI_PIN  (10u)
#define PSP_SPI_0_CLK_PIN   (11u)

/*
--|----------------------------------------------------------------------------|
--| PUBLIC TYPES
--|----------------------------------------------------------------------------|
*/

/*
--| NAME: PSP_SPI_0_Clock_Divider_t
--| DESCRIPTION: SPI 0 clock divider settings
*/
typedef enum SPI_0_Clock_Divider_Type
{
    PSP_SPI0_Clock_Divider_2     =     2u, // sets SPI 0 clock to 125.0 MHz
    PSP_SPI0_Clock_Divider_4     =     4u, // sets SPI 0 clock to 62.5 MHz
    PSP_SPI0_Clock_Divider_8     =     8u, // sets SPI 0 clock to 31.2 MHz
    PSP_SPI0_Clock_Divider_16    =    16u, // sets SPI 0 clock to 15.6 MHz
    PSP_SPI0_Clock_Divider_32    =    32u, // sets SPI 0 clock to 7.8 MHz
    PSP_SPI0_Clock_Divider_64    =    64u, // sets SPI 0 clock to 3.9 MHz
    PSP_SPI0_Clock_Divider_128   =   128u, // sets SPI 0 clock to 1953 kHz
    PSP_SPI0_Clock_Divider_256   =   256u, // sets SPI 0 clock to 976 kHz
    PSP_SPI0_Clock_Divider_512   =   512u, // sets SPI 0 clock to 488 kHz
    PSP_SPI0_Clock_Divider_1024  =  1024u, // sets SPI 0 clock to 244 kHz
    PSP_SPI0_Clock_Divider_2048  =  2048u, // sets SPI 0 clock to 122 kHz
    PSP_SPI0_Clock_Divider_4096  =  4096u, // sets SPI 0 clock to 61 kHz
    PSP_SPI0_Clock_Divider_8192  =  8192u, // sets SPI 0 clock to 30.5 kHz
    PSP_SPI0_Clock_Divider_16384 = 16384u, // sets SPI 0 clock to 15.2 kHz
    PSP_SPI0_Clock_Divider_32768 = 32768u  // sets SPI 0 clock to 7629 Hz
} PSP_SPI_0_Clock_Divider_t;

/*
--| NAME: PSP_SPI_0_Chip_Select_t
--| DESCRIPTION: SPI 0 active chip select setting
*/
typedef enum SPI_0_Chip_Select_Type
{
    PSP_SPI_0_Chip_Select_0 = 0u,
    PSP_SPI_0_Chip_Select_1 = 1u
} PSP_SPI_0_Chip_Select_t;

/*
--|----------------------------------------------------------------------------|
--| PUBLIC CONSTANTS
--|----------------------------------------------------------------------------|
*/

/* None */

/*
--|----------------------------------------------------------------------------|
--| PUBLIC VARIABLES
--|----------------------------------------------------------------------------|
*/

/* None */

/*
--|----------------------------------------------------------------------------|
--| PUBLIC FUNCTION PROTOTYPES
--|----------------------------------------------------------------------------|
*/

/*------------------------------------------------------------------------------
Function Name:
    PSP_SPI0_Start

Function Description:
    Initialize SPI0 by setting GPIO pins 7 through 11 to alt mode 0, zeroing 
    the SPI0 status and control register, clearing the SPI0 Rx FIFO and Tx 
    FIFO buffers, and setting the chip select pin to chip select 0.

Inputs:
    None

Returns:
    None

Assumptions/Limitations:
    None
------------------------------------------------------------------------------*/
void PSP_SPI0_Start(void);

/*------------------------------------------------------------------------------
Function Name:
    PSP_SPI0_End

Function Description:
    Shuts down SPI0 by setting GPIO pins 7 through 11 to inputs.

Inputs:
    None

Returns:
    None

Assumptions/Limitations:
    None
------------------------------------------------------------------------------*/
void PSP_SPI0_End(void);

/*------------------------------------------------------------------------------
Function Name:
    PSP_SPI0_Set_Clock_Divider

Function Description:
    Sets the clock divider for SPI0. This sets the speed for the SPI0 clock.

    If this function is never called, the divider will default to 65536.

Inputs:
    divider: The divider to use for the SPI 0 clock.
    
    The following are the the valid dividers and the resulting SPI 0 clock speeds:

       div    speed
        2  -  125.0 MHz
        4  -   62.5 MHz
        8  -   31.2 MHz
       16  -   15.6 MHz
       32  -    7.8 MHz
       64  -    3.9 MHz
      128  -   1953 kHz
      256  -    976 kHz
      512  -    488 kHz
     1024  -    244 kHz
     2048  -    122 kHz
     4096  -     61 kHz
     8192  -   30.5 kHz
    16384  -   15.2 kHz
    32768  -   7629 Hz

    (note that the fastest three speeds may not work, experimentation needed)

Returns:
    None

Assumptions/Limitations:
    None
------------------------------------------------------------------------------*/
void PSP_SPI0_Set_Clock_Divider(PSP_SPI_0_Clock_Divider_t divider);

/*------------------------------------------------------------------------------
Function Name:
    PSP_SPI0_Begin_Transfer

Function Description:
    Begins an SPI 0 transfer. It is expected that various send/recieve 
    functions will be called after beginning an SPI 0 transfer.

Inputs:
    None.

Returns:
    None.

Assumptions/Limitations:
    None
------------------------------------------------------------------------------*/
void PSP_SPI0_Begin_Transfer(void);

/*------------------------------------------------------------------------------
Function Name:
    PSP_SPI0_End_Transfer

Function Description:
    Ends an SPI 0 transfer. It is expected that PSP_SPI0_Begin_Transfer, and 
    various send/recieve functions will be called before ending an SPI 0 transfer.

Inputs:
    None.

Returns:
    None.

Assumptions/Limitations:
    None
------------------------------------------------------------------------------*/
void PSP_SPI0_End_Transfer(void);

/*------------------------------------------------------------------------------
Function Name:
    PSP_SPI0_Send_Byte

Function Description:
    Send a single byte as part of an SPI 0 transfer.

Inputs:
    val: the byte to write via SPI 0.

Returns:
    None.

Assumptions/Limitations:
    Expects that PSP_SPI0_Begin_Transfer was called before calling this 
    function and that PSP_SPI0_End_Transfer will be called at the end of the
    transfer.
------------------------------------------------------------------------------*/
void PSP_SPI0_Send_Byte(uint8_t val);

/*------------------------------------------------------------------------------
Function Name:
    PSP_SPI0_Send_16

Function Description:
    Send a 16 bit word as part of an SPI 0 transfer.

Inputs:
    val: the 16 bit value to write via SPI 0.

Returns:
    None.

Assumptions/Limitations:
    Expects that PSP_SPI0_Begin_Transfer was called before calling this
    function and that PSP_SPI0_End_Transfer will be called at the end of 
    the transfer.
------------------------------------------------------------------------------*/
void PSP_SPI0_Send_16(uint16_t val);

/*------------------------------------------------------------------------------
Function Name:
    PSP_SPI0_Transfer_Byte

Function Description:
    Write and read a single byte via SPI 0. Uses Polled transfer as described 
    in section 10.6.1 of the datasheet.

Inputs:
    val: the byte to write via SPI 0.

Returns:
    uint8_t: the value read by SPI 0.

Assumptions/Limitations:
    This method does not require 
    PSP_SPI0_Begin_Transfer/PSP_SPI0_End_Transfer bookends.
------------------------------------------------------------------------------*/
uint8_t PSP_SPI0_Transfer_Byte(uint8_t val);

/*-----------------------------------------------------------------------------------------------
Function Name:
    PSP_SPI0_Transfer_16

Function Description:
    Write and read a 16 bit word via SPI 0. Uses Polled transfer as described 
    in section 10.6.1 of the datasheet.

Inputs:
    val: the 16 bit value to write via SPI 0.

Returns:
    uint16_t: the value read by SPI 0.

Assumptions/Limitations:
    This method does not require 
    PSP_SPI0_Begin_Transfer/PSP_SPI0_End_Transfer bookends.
------------------------------------------------------------------------------*/
uint16_t PSP_SPI0_Transfer_16(uint16_t val);

/*------------------------------------------------------------------------------
Function Name:
    PSP_SPI0_Buffer_Transfer

Function Description:
    Write and read a given number of bytes via Tx and Rx buffers

Inputs:
    p_Tx_buffer: pointer to the buffer of bytes to write out via SPI 0.
    p_Rx_buffer: pointer to the buffer of bytes to read in via SPI 0.
    num_bytes: the number of bytes to write/read.

Returns:
    None. p_Rx_buffer will be filled with num_bytes of information if the 
    device connected to the MISO line sends any data during the transmission.

Assumptions/Limitations:
    note that the Tx and Rx buffers MUST both be at least num_bytes long.
    This method does not require PSP_SPI0_Begin_Transfer/PSP_SPI0_End_Transfer 
    bookends.
------------------------------------------------------------------------------*/
void PSP_SPI0_Buffer_Transfer(uint8_t *p_Tx_buffer, 
                              uint8_t *p_Rx_buffer, 
                              uint32_t num_bytes);

/*------------------------------------------------------------------------------

Function Name:
    PSP_SPI0_Set_Chip_Select

Function Description:
    Choose which chip select line to assert when transferring data.

Inputs:
    chip_select: the chip select line to use, 0 or 1. Has no immediate effect,
    but the next time a data transfer occurs the new chip select line will 
    be used.

Returns:
    None

Assumptions/Limitations:
    None
------------------------------------------------------------------------------*/
void PSP_SPI0_Set_Chip_Select(PSP_SPI_0_Chip_Select_t chip_select);

#endif
