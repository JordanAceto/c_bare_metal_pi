/*
--|----------------------------------------------------------------------------|
--| FILE DESCRIPTION:
--|  PSP_GPIO interfaces with the GPIO registers. Functions are
--|  provided for setting the pin mode of GPIO pins, reading the
--|  level of GPIO pins, and setting the level of GPIO pins which
--|  are set to outputs.
--|  
--|----------------------------------------------------------------------------|
--| REFERENCES:
--|   BCM2837-ARM-Peripherals.pdf page 89
--|
--|----------------------------------------------------------------------------|
*/

#ifndef PSP_GPIO_H_INCLUDED
#define PSP_GPIO_H_INCLUDED

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

/* None */

/*
--|----------------------------------------------------------------------------|
--| PUBLIC TYPES
--|----------------------------------------------------------------------------|
*/

/*
--| NAME: GPIO_Pin_Mode_enum
--| DESCRIPTION: Enumeration for GPIO pin modes. These are the 3-bit wide values
--|   which are written to GPFSELn to determine the mode for a given pin
*/
typedef enum GPIO_Pin_Mode_Enumeration
{
    PSP_GPIO_PINMODE_INPUT   = 0b000u,
    PSP_GPIO_PINMODE_OUTPUT  = 0b001u,
    PSP_GPIO_PINMODE_ALT0    = 0b100u,
    PSP_GPIO_PINMODE_ALT1    = 0b101u,
    PSP_GPIO_PINMODE_ALT2    = 0b110u,
    PSP_GPIO_PINMODE_ALT3    = 0b111u,
    PSP_GPIO_PINMODE_ALT4    = 0b011u,
    PSP_GPIO_PINMODE_ALT5    = 0b010u,
    PSP_GPIO_MAX_PINMODE_VAL = PSP_GPIO_PINMODE_ALT3,
} GPIO_Pin_Mode_enum;

/*
--| NAME: PSP_GPIO_Edge_Detect_enum
--| DESCRIPTION: enumeration for GPIO edge detect types
*/
typedef enum PSP_GPIO_Edge_Detect_Enumeration
{
    GPIO_EDGE_TYPE_RISING   = 0u, // detects rising edge transitions
    GPIO_EDGE_TYPE_FALLING  = 1u, // detects falling edge transitions
    GPIO_EDGE_TYPE_CHANGING = 2u, // detects rising and falling edge transitions
    GPIO_EDGE_TYPE_NO_EDGE  = 3u, // disable edge detects
    GPIO_MAX_EDGE_TYPE_VAL  = 3u, // the larges value an edge detect type can take
} PSP_GPIO_Edge_Detect_enum;

/*
--| NAME: PSP_GPIO_Edge_Detect_Reading_enum
--| DESCRIPTION: enumeration for GPIO edge detect readings
*/
typedef enum PSP_GPIO_Edge_Detect_Reading_Enumeration
{
    GPIO_EDGE_NOT_DETECTED = 0u,
    GPIO_EDGE_DETECTED     = 1u,
} PSP_GPIO_Edge_Detect_Reading_enum;

/*
--| NAME: GPIO_Pin_Output_Write_enum
--| DESCRIPTION: set/clear signal for a given GPIO pin
*/
typedef enum GPIO_Pin_Output_Write_Enumeration
{
    PSP_GPIO_PIN_WRITE_LOW  = 0u,
    PSP_GPIO_PIN_WRITE_HIGH = 1u
} GPIO_Pin_Output_Write_enum;

/*
--| NAME: GPIO_Pin_Input_Read_enum
--| DESCRIPTION: value of a given GPIO pin reading
*/
typedef enum GPIO_Pin_Input_Read_Enumeration
{
    PSP_GPIO_PIN_READ_LOW  = 0u,
    PSP_GPIO_PIN_READ_HIGH = 1u
} GPIO_Pin_Input_Read_enum;

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

Assumptions/Limitations:
    Returns without having any effect if the pin number or pin mode are 
    out of range

------------------------------------------------------------------------------*/
void PSP_GPIO_Set_Pin_Mode(uint32_t pin_num, GPIO_Pin_Mode_enum pin_mode);

/*------------------------------------------------------------------------------
Function Name:
    PSP_GPIO_Write_Pin

Function Description:
    Write a GPIO pin high or low

Inputs:
    pin_num: the GPIO pin number to set high or low
    value: GPIO pin value, 0 sets the pin Low, Non-zero sets the pin High

Returns:
    None

Assumptions/Limitations:
    Returns without having any effect if the pin number is out of range.

    Has no effect if the GPIO pin is not set to Output. However, if the pin 
    is subsequently defined as an output then the bit will be set according 
    to the last set/clear operation.
------------------------------------------------------------------------------*/
void PSP_GPIO_Write_Pin(uint32_t pin_num, GPIO_Pin_Output_Write_enum value);

/*------------------------------------------------------------------------------
Function Name:
    PSP_GPIO_Read_Pin

Function Description:
    Read a GPIO pin and return 0 if the pin is low and 1 if the pin is high

Inputs:
    pin_num: GPIO pin number to read

Returns:
    The value of the GPIO pin (0 or 1)

Assumptions/Limitations:
    Returns 0 if the pin number is out of range
------------------------------------------------------------------------------*/
GPIO_Pin_Input_Read_enum PSP_GPIO_Read_Pin(uint32_t pin_num);

/*------------------------------------------------------------------------------
Function Name:
    PSP_GPIO_Pin_Enable_Edge_Detect

Function Description:
    Enable a given edge detect for a given GPIO pin.

Inputs:
    pin_num: GPIO pin number to enable the edge detect.
    edge: the edge to detect.

Returns:
    None.

Assumptions/Limitations:
    Returns without having any effect if the pin number is out of range or 
    edge type is invalid.
------------------------------------------------------------------------------*/
void PSP_GPIO_Pin_Enable_Edge_Detect(uint32_t pin_num, PSP_GPIO_Edge_Detect_enum edge);

/*------------------------------------------------------------------------------
Function Name:
    PSP_GPIO_Event_Detected

Function Description:
    Read a GPIO pin and return 0 if an event was not detected and 1 if an 
    event was detected.

    If an event is detected, it is cleared.

Inputs:
    pin_num: GPIO pin number to read.

Returns:
    Whether an event was detected or not (0 or 1).

Assumptions/Limitations:
    Returns 0 if the pin number is out of range.

    Assumes that PSP_GPIO_Pin_Enable_Edge_Detect previously assigned an edge 
    for this pin to detect.
------------------------------------------------------------------------------*/
PSP_GPIO_Edge_Detect_Reading_enum PSP_GPIO_Event_Detected(uint32_t pin_num);

#endif
