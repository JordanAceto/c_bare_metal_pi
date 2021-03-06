/*
--|----------------------------------------------------------------------------|
--| FILE DESCRIPTION:
--|   PSP_I2C provides an interface for using I2C.
--|  
--|----------------------------------------------------------------------------|
--| NOTES:
--|      TODO: Writing data has been tested, but reading data has not. To do so,
--|      I'll need to set up some I2C device to talk back to the Pi and run some
--|      tests. Until then, consider reading data to be broken.
--|
--|      TODO: Add functions to read/write more than one byte at a time.
--|  
--|----------------------------------------------------------------------------|
--| REFERENCES:
--|   BCM2837-ARM-Peripherals.pdf page 28
--|
--|----------------------------------------------------------------------------|
*/

#ifndef PSP_I2C_H_INCLUDED
#define PSP_I2C_H_INCLUDED

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
--| NAME: I2C_xxx_PIN
--| DESCRIPTION: GPIO pins used by I2C
--| TYPE: uint32_t
*/
#define I2C_SDA_PIN (2u)
#define I2C_SCL_PIN (3u)

/*
--|----------------------------------------------------------------------------|
--| PUBLIC TYPES
--|----------------------------------------------------------------------------|
*/

/* None */

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
    PSP_I2C_Start

Function Description:
    Initialize I2C by setting GPIO pins 2 and 3 to alt mode 0.

Inputs:
    None

Returns:
    None

Assumptions/Limitations:
    None
------------------------------------------------------------------------------*/
void PSP_I2C_Start(void);

/*------------------------------------------------------------------------------
Function Name:
    PSP_I2C_End

Function Description:
    Shut down I2C by setting GPIO pins 2 and 3 to inputs.

Inputs:
    None

Returns:
    None

Assumptions/Limitations:
    None
------------------------------------------------------------------------------*/
void PSP_I2C_End(void);

/*------------------------------------------------------------------------------
Function Name:
    PSP_I2C_Set_Clock_Divider

Function Description:
    Sets the clock divider for I2C. This sets the clock speed.

Inputs:
    divider: This must be a power of 2. Only the lower 16 bits are used. 
    Odd number are rounded down.

    SCL = core_clock / divider Where core_clock is nominally 150 MHz. 
    If divider is set to 0, the divisor is 32768. 
    The divider is always rounded down to an even number. 
    The default value should result in a 100 kHz I2C clock frequency.

Returns:
    None

Assumptions/Limitations:
    None
------------------------------------------------------------------------------*/
void PSP_I2C_Set_Clock_Divider(uint32_t divider);

/*------------------------------------------------------------------------------
Function Name:
    PSP_I2C_Set_Slave_Address

Function Description:
    Sets the I2C slave device address.

Inputs:
    r0: address, the 7 bit address of the device to communicate with. 
    Uses only bits [0, 6].

Returns:
    None

Assumptions/Limitations:
    None
------------------------------------------------------------------------------*/
void PSP_I2C_Set_Slave_Address(uint32_t address);

/*------------------------------------------------------------------------------
Function Name:
    PSP_I2C_Write_Byte

Function Description:
    Writes a single byte to the address in the I2C address register.

Inputs:
    val: the byte to write.

Returns:
    None (TODO: this should return some error/ok code in the future that
    specifies if the transmission was succesful)

Assumptions/Limitations:
    None
------------------------------------------------------------------------------*/
void PSP_I2C_Write_Byte(uint8_t val);

#endif
