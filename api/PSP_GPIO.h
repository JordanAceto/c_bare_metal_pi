/**
 * DESCRIPTION:
 *      PSP_GPIO interfaces with the GPIO registers. Functions are
 *      provided for setting the pin mode of GPIO pins, reading the
 *      level of GPIO pins, and setting the level of GPIO pins which
 *      are set to outputs.
 * 
 * NOTES:
 *      TODO: Add edge detection functionality.
 * 
 * REFERENCES:
 *      BCM2837-ARM-Peripherals.pdf page 89
 */

#ifndef PSP_GPIO_H_INCLUDED
#define PSP_GPIO_H_INCLUDED

#include "Fixed_Width_Ints.h"

/*-----------------------------------------------------------------------------------------------
    Public PSP_GPIO Definitions
 -------------------------------------------------------------------------------------------------*/

// Pin Mode Defines
#define PSP_GPIO_PINMODE_INPUT  0b000u
#define PSP_GPIO_PINMODE_OUTPUT 0b001u
#define PSP_GPIO_PINMODE_ALT0   0b100u
#define PSP_GPIO_PINMODE_ALT1   0b101u
#define PSP_GPIO_PINMODE_ALT2   0b110u
#define PSP_GPIO_PINMODE_ALT3   0b111u
#define PSP_GPIO_PINMODE_ALT4   0b011u
#define PSP_GPIO_PINMODE_ALT5   0b010u

#define PSP_GPIO_PIN_WRITE_HIGH 1u
#define PSP_GPIO_PIN_WRITE_LOW  0u


/*-----------------------------------------------------------------------------------------------
    Public PSP_GPIO Function Declarations
 -------------------------------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------------------------

Function Name:
    PSP_GPIO_Set_Pin_Mode

Function Description:
    Set a GPIO pin mode to Input, Output, or Alternate Function 0... 5

Inputs:
    pin_num: the GPIO pin to set to a given mode

    pin_mode: the GPIO pin mode, as below:

        0b000 = GPIO Pin is an input
        0b001 = GPIO Pin is an output
        0b100 = GPIO Pin takes alternate function 0
        0b101 = GPIO Pin takes alternate function 1
        0b110 = GPIO Pin takes alternate function 2
        0b111 = GPIO Pin takes alternate function 3
        0b011 = GPIO Pin takes alternate function 4
        0b010 = GPIO Pin takes alternate function 5

Returns:
    None

Error Handling:
    Returns without having any effect if the pin number or pin mode are out of range

-------------------------------------------------------------------------------------------------*/
void PSP_GPIO_Set_Pin_Mode(uint32_t pin_num, uint32_t pin_mode);



/*-----------------------------------------------------------------------------------------------

Function Name:
    PSP_GPIO_Write_Pin

Function Description:
    Write a GPIO pin high or low

Inputs:
    pin_num: the GPIO pin number to set high or low
    value: GPIO pin value, 0 sets the pin Low, Non-zero sets the pin High

Returns:
    None

Error Handling:
    Returns without having any effect if the pin number is out of range.

    Has no effect if the GPIO pin is not set to Output. However, if the pin is subsequently 
    defined as an output then the bit will be set according to the last set/clear operation.

-------------------------------------------------------------------------------------------------*/
void PSP_GPIO_Write_Pin(uint32_t pin_num, uint32_t value);



/*-----------------------------------------------------------------------------------------------

Function Name:
    PSP_GPIO_Read_Pin

Function Description:
    Read a GPIO pin and return 0 if the pin is low and 1 if the pin is high

Inputs:
    pin_num: GPIO pin number to read

Returns:
    The value of the GPIO pin (0 or 1)

Error Handling:
    Returns 0 if the pin number is out of range

-------------------------------------------------------------------------------------------------*/
uint32_t PSP_GPIO_Read_Pin(uint32_t pin_num);

#endif
