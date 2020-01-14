/**
 * DESCRIPTION:
 *      PSP_Hardware_PWM provides an interface for the built in hardware random 
 *      number generator.
 * 
 * NOTES:
 *      The hardware random number generator is not well documented. This module was
 *      developed with hints from the web.
 * 
 * REFERENCES:
 *      https://www.raspberrypi.org/forums/viewtopic.php?t=196015
 *      https://elinux.org/BCM2835_registers#RNG (wrong processor, but seems to work)
 */

#ifndef PSP_HARDWARE_RNG_H_INCLUDED
#define PSP_HARDWARE_RNG_H_INCLUDED

#include "Fixed_Width_Ints.h"

/*------------------------------------------------------------------------------------------------
    Public PSP_Hardware_RNG Function Declarations
 -------------------------------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------------------------

Function Name:
    PSP_Hardware_RNG_Init

Function Description:
    Initialize the hardware RNG source. The exact workings of the RNG source are not well
    documented or understood.

Inputs:
    None

Returns:
    None

Error Handling:
    None

-------------------------------------------------------------------------------------------------*/
void PSP_Hardware_RNG_Init(void);



/*-----------------------------------------------------------------------------------------------

Function Name:
    PSP_Hardware_RNG_Get_Random

Function Description:
    Get a random uint32_t from the RNG.

Inputs:
    None

Returns:
    A random uint32_t.

Error Handling:
    None, assumes that PSP_Hardware_RNG_Init has been called.

-------------------------------------------------------------------------------------------------*/
uint32_t PSP_Hardware_RNG_Get_Random(void);


#endif
