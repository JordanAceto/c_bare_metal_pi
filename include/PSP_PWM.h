
/*
--|----------------------------------------------------------------------------|
--| FILE DESCRIPTION:
--|   PSP_PWM provides an interface for using the hardware PWM channels 1 and 2.
--|  
--|----------------------------------------------------------------------------|
--| NOTES:
--|   Only GPIO12 and GPIO18 are available as channel 1 PWM pins and only GPIO13 
--|   and GPIO19 are available as channel 2 PWM pins on the raspberry pi 3b+ 
--|   breakout board.
--|  
--|----------------------------------------------------------------------------|
--| REFERENCES:
--|   BCM2837-ARM-Peripherals.pdf page 138 (PWM)
--|
--|   BCM2837-ARM-Peripherals.pdf page 105 (Clock manager info, not directly 
--|   about PWM CM, but maps pretty well)
--|
--|   https://elinux.org/BCM2835_datasheet_errata#p105_table (more information 
--|   about the Clock Manager)
--|
--|----------------------------------------------------------------------------|
*/

#ifndef PSP_PWM_H_INCLUDED
#define PSP_PWM_H_INCLUDED

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
--| NAME: PWM_DEFAULT_RANGE
--| DESCRIPTION: default PWM range [0, 255]
--| TYPE: PWM_Range_Type
*/
#define PWM_DEFAULT_RANGE (PSP_PWM_RANGE_8_BITS)             

/*
--| NAME: PWM_DEFAULT_CLOCK
--| DESCRIPTION: default clock source, 19.2MHz internal osc
--| TYPE: PWM_Clock_Source_Type
*/
#define PWM_DEFAULT_CLOCK (PSP_PWM_Clock_Source_OSCILLATOR)

/*
--| NAME: PWM_DEFAULT_DIV
--| DESCRIPTION: default clock divider, divide 19.2MHz clock by 4 = 4.8MHz
--| TYPE: uint43_t
*/
#define PWM_DEFAULT_DIV (4u)                                    

/*
--|----------------------------------------------------------------------------|
--| PUBLIC TYPES
--|----------------------------------------------------------------------------|
*/

/*
--| NAME: PSP_PWM_Clock_Source_t
--| DESCRIPTION: enumeration of PWM clock sources
*/
typedef enum PWM_Clock_Source_Type
{
    PSP_PWM_Clock_Source_GND        = 0u, // clock off
    PSP_PWM_Clock_Source_OSCILLATOR = 1u, // 19.2MHz
    PSP_PWM_Clock_Source_PLL_C      = 5u, // 1,000MHz (changes with overclock settings)
    PSP_PWM_Clock_Source_PLL_D      = 6u, // 500MHz
    PSP_PWM_Clock_Source_HDMI_AUX   = 7u  // 216MHz
} PSP_PWM_Clock_Source_t;

/*
--| NAME: PSP_PWM_Channel_t
--| DESCRIPTION: enumeration of PWM channels
*/
typedef enum PWM_Channel_Type
{
    PSP_PWM_Channel_1,
    PSP_PWM_Channel_2
} PSP_PWM_Channel_t;

/*
--| NAME: PSP_PWM_Output_Mode_t
--| DESCRIPTION: enumeration of PWM output modes
*/
typedef enum PWM_Output_Mode_Type
{
    PSP_PWM_N_OVER_M_MODE   = 0u, // use the N/M mode
    PSP_PWM_MARK_SPACE_MODE = 1u  // use the M/S mode
} PSP_PWM_Output_Mode_t;

/*
--| NAME: PSP_PWM_Range_t
--| DESCRIPTION: enumeration of PWM range
*/
typedef enum PWM_Range_Type
{
    PSP_PWM_RANGE_8_BITS  =  0x100u, // [0, 255]
    PSP_PWM_RANGE_10_BITS =  0x400u, // [0, 1023]
    PSP_PWM_RANGE_12_BITS =  0x1000, // [0, 4095]
    PSP_PWM_RANGE_16_BITS = 0x10000, // [0, 65535]
} PSP_PWM_Range_t;

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
    PSP_PWM_Clock_Init_Default

Function Description:
    Initialize the PWM clock source to defauts of internal oscillator divided 
    down to 4.8MHz, and start the PWM clock.

    A clock init function must be called before starting PWM channel 1 or 2, 
    setting any GPIO pins to PWM mode or  writing any pins via PWM.

Inputs:
    None

Returns:
    None

Assumptions/Limitations:
    None
------------------------------------------------------------------------------*/
void PSP_PWM_Clock_Init_Default(void);

/*------------------------------------------------------------------------------
Function Name:
    PSP_PWM_Clock_Init

Function Description:
    Initialize the PWM clock source to a given clock source and divider.

    A clock init  function must be called before starting PWM channel 1 or 2, 
    setting any GPIO pins to PWM mode or  writing any pins via PWM.

Inputs:
    clock source: the clock source to use
    divider: the value to divide the base clock frequency by

Returns:
    None

Assumptions/Limitations:
    None
------------------------------------------------------------------------------*/
void PSP_PWM_Clock_Init(PSP_PWM_Clock_Source_t clock_source, uint32_t divider);

/*------------------------------------------------------------------------------
Function Name:
    PSP_PWM_Channel_Start

Function Description:
    Initialize a given PWM channel by setting the ouput mode and range.
    
    Note that only GPIO12 and GPIO18 are available as channel 1 PWM pins and 
    only GPIO13 and GPIO19 are available as channel 2 PWM pins on the raspberry 
    pi 3b+ breakout board.

Inputs:
    channel: the channel to initialize, channel 1 or 2
    mode: the output mode to use, PWM algorithm or M/S
    range: the range of the PWM channel to use, 8, 10, 12, or 16 bits

Returns:
    None

Assumptions/Limitations:
    None
------------------------------------------------------------------------------*/
void PSP_PWM_Channel_Start(PSP_PWM_Channel_t channel, 
                           PSP_PWM_Output_Mode_t mode, 
                           PSP_PWM_Range_t range);

/*------------------------------------------------------------------------------
Function Name:
    PSP_PWM_Ch1_Write

Function Description:
    Writes a value to PWM Channel 1.

Inputs:
    value: the value to write.

Returns:
    None

Assumptions/Limitations:
    None
------------------------------------------------------------------------------*/
void PSP_PWM_Ch1_Write(uint32_t value);

/*------------------------------------------------------------------------------
Function Name:
    PSP_PWM_Ch2_Write

Function Description:
    Writes a value to PWM Channel 2.

Inputs:
    value: the value to write.

Returns:
    None

Assumptions/Limitations:
    None
------------------------------------------------------------------------------*/
void PSP_PWM_Ch2_Write(uint32_t value);

/*------------------------------------------------------------------------------
Function Name:
    PSP_PWM_Ch1_Set_GPIO12_To_PWM_Mode

Function Description:
    Sets the pin mode of GPIO12 to PWM Channel 1

Inputs:
    None

Returns:
    None

Assumptions/Limitations:
    None
------------------------------------------------------------------------------*/
void PSP_PWM_Ch1_Set_GPIO12_To_PWM_Mode(void);

/*------------------------------------------------------------------------------
Function Name:
    PSP_PWM_Ch1_Set_GPIO18_To_PWM_Mode

Function Description:
    Sets the pin mode of GPIO18 to PWM Channel 1

Inputs:
    None

Returns:
    None

Assumptions/Limitations:
    None
------------------------------------------------------------------------------*/
void PSP_PWM_Ch1_Set_GPIO18_To_PWM_Mode(void);

/*------------------------------------------------------------------------------
Function Name:
    PSP_PWM_Ch2_Set_GPIO13_To_PWM_Mode

Function Description:
    Sets the pin mode of GPIO13 to PWM Channel 2

Inputs:
    None

Returns:
    None

Assumptions/Limitations:
    None
------------------------------------------------------------------------------*/
void PSP_PWM_Ch2_Set_GPIO13_To_PWM_Mode(void);

/*------------------------------------------------------------------------------
Function Name:
    PSP_PWM_Ch2_Set_GPIO19_To_PWM_Mode

Function Description:
    Sets the pin mode of GPIO19 to PWM Channel 2

Inputs:
    None

Returns:
    None

Assumptions/Limitations:
    None
------------------------------------------------------------------------------*/
void PSP_PWM_Ch2_Set_GPIO19_To_PWM_Mode(void);

#endif
