/*
--|----------------------------------------------------------------------------|
--| FILE DESCRIPTION:
--|     BSP_Rotaty_Encoder provides types and interfaces for connecting
--|     and reading rotary encoders.
--|  
--|----------------------------------------------------------------------------|
--| NOTES:
--|     This module has been tested with inexpensive Bourns PEC11R series
--|     encoders using the example circuit in the datasheet. External resistors
--|     and capacitors were used.
--|
--|     Other encoders may or may not require modifications.
--|
--|     While testing, I just spun the encoder by hand. I don't know if this
--|     module would keep up with an encoder on a motor shaft or something.
--|  
--|----------------------------------------------------------------------------|
--| REFERENCES:
--|   https://www.bourns.com/docs/Product-Datasheets/PEC11R.pdf
--|
--|----------------------------------------------------------------------------|
*/

#ifndef BSP_ROTARY_ENCODER_H_INCLUDED
#define BSP_ROTARY_ENCODER_H_INCLUDED

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
--| NAME: BSP_Rotary_Encoder_State_t
--| DESCRIPTION: storage for the state of a rotary encoder
*/
typedef union Rotary_Encoder_State_Type
{
    struct
    {
        uint8_t Pin_A_State      : 1u;
        uint8_t Pin_B_State      : 1u;
        uint8_t Last_Pin_A_State : 1u;
        uint8_t Last_Pin_B_State : 1u;
        uint8_t UNUSED           : 4u;
    };
    uint8_t Raw_Data; // the encoder state is stored in the lower 4 bits
} BSP_Rotary_Encoder_State_t;

/*
--| NAME: BSP_Rotary_Encoder_t
--| DESCRIPTION: storage for a rotary encoder struct
*/
typedef struct Rotary_Encoder_Type
{
    const uint32_t PIN_A;
    const uint32_t PIN_B;

    BSP_Rotary_Encoder_State_t state;

    int32_t count;

} BSP_Rotary_Encoder_t;

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
    BSP_Rotary_Encoder_Initialize

Function Description:
    Initialize a rotary encoder by setting its A and B pins to inputs, 
    setting its count to zero, and setting its state to zero.

Inputs:
    pEncoder: pointer to the encoder to initialize.

Returns:
    None

Assumptions/Limitations:
    None

------------------------------------------------------------------------------*/
void BSP_Rotary_Encoder_Initialize(BSP_Rotary_Encoder_t * pEncoder);

/*------------------------------------------------------------------------------
Function Name:
    BSP_Poll_Rotary_Encoder

Function Description:
    Polls a rotary encoder and updates its state and count.

    Note that this function must be called as frequently as possible in order 
    to accurately poll the encoder.

Inputs:
    pEncoder: pointer to the encoder to poll. The state of this encoder will 
    be modified during the function call.

Returns:
    The updated encoder state and count in pEncoder.

Assumptions/Limitations:
    None

------------------------------------------------------------------------------*/
void BSP_Poll_Rotary_Encoder(BSP_Rotary_Encoder_t * pEncoder);

#endif
