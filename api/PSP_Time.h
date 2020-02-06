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
    Public PSP_Time Function Types
 -------------------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------

Struct Name:
    PSP_Time_Timeout_Counter

Members:
    timeout_period_uSec: the timeout period in microseconds
    timeout_time_uSec: the time stamp at which a timeout will occur

Usage:
    Intended to be passed to various init and poll methods in order to 
    create a periodic timout timer for scheduling events.

-------------------------------------------------------------------------------------------------*/
typedef struct Timeout_Counter
{
    uint64_t timeout_period_uSec;
    uint64_t timeout_time_uSec;
} PSP_Time_Timeout_Counter;



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


/*-----------------------------------------------------------------------------------------------

Function Name:
    PSP_Time_Initialize_Timer_Counter

Function Description:
    Initializes a given Timeout Counter by setting the timout time to be
    the current time plus the timout period

Inputs:
    * pCounter: a pointer to the counter to initialize

Returns:
    None

Error Handling:
    None

-------------------------------------------------------------------------------------------------*/
void PSP_Time_Initialize_Timer_Counter(PSP_Time_Timeout_Counter * pCounter);



/*-----------------------------------------------------------------------------------------------

Function Name:
    PSP_Time_Poll_Timeout_Counter

Function Description:
    Poll a given Timout Counter and return 1u if a timeout has occured, and a 0u if
    a timeout has not occured.

    Repeatedly calling this function will handle the data in pCounter and return 1u
    every time the timout period help by pCounter has been exceeded.

Inputs:
    * pCounter: a pointer to the counter to poll

Returns:
    None

Error Handling:
    None

-------------------------------------------------------------------------------------------------*/
uint32_t PSP_Time_Poll_Timeout_Counter(PSP_Time_Timeout_Counter * pCounter);



#endif
