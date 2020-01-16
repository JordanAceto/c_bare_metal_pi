/**
 * DESCRIPTION:
 *      PSP_Time interfaces with the System Clock timer, providing 
 *      functions for getting the current time and delaying for a
 *      specified amount of time.
 * 
 * NOTES:
 *      TODO: Add milliseconds get/delay functions (only has microseconds for now)
 * 
 * REFERENCES:
 *      BCM2837-ARM-Peripherals.pdf page 172
 */

#ifndef PSP_TIME_H_INCLUDED
#define PSP_TIME_H_INCLUDED

#include "Fixed_Width_Ints.h"

/*-----------------------------------------------------------------------------------------------
    Public PSP_Time Function Declarations
 -------------------------------------------------------------------------------------------------*/



/*-----------------------------------------------------------------------------------------------

Function Name:
    PSP_Time_Get_Ticks

Function Description:
    Get the 64 bit contents of the System Timer Counter

Inputs:
    None

Returns:
    uint64_t: The 64 bits of the System Timer Counter

Error Handling:
    None

-------------------------------------------------------------------------------------------------*/
uint64_t PSP_Time_Get_Ticks(void);




/*-----------------------------------------------------------------------------------------------

Function Name:
    PSP_Time_Delay_Microseconds

Function Description:
    Wait for a specified number of microseconds

Inputs:
    delay_time_uSec: uint32_t time in uSec to wait

    the longest time available is 4,294,967,295 microseconds, about 71 minutes

Returns:
    None

Error Handling:
    None

-------------------------------------------------------------------------------------------------*/
void PSP_Time_Delay_Microseconds(const uint32_t delay_time_uSec);

#endif
