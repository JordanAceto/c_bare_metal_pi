/*
--|----------------------------------------------------------------------------|
--| FILE DESCRIPTION:
--|    PSP_Time interfaces with the System Clock timer, providing 
--|    functions for getting the current time and delaying for a
--|    specified amount of time.
--|  
--|----------------------------------------------------------------------------|
--| REFERENCES:
--|   BCM2837-ARM-Peripherals.pdf page 172
--|
--|----------------------------------------------------------------------------|
*/

#ifndef PSP_TIME_H_INCLUDED
#define PSP_TIME_H_INCLUDED

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
--| NAME: PSP_Time_Periodic_Timer_t
--| DESCRIPTION: 64 bit system timer counter structure
*/
typedef struct PSP_Time_Periodic_Timer_Type
{
    uint64_t timeout_period_uSec;    // the period of the timeout in mSec
    uint64_t last_timeout_time_uSec; // the time in mSec when the last timeout occured
} PSP_Time_Periodic_Timer_t;

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
    PSP_Time_Get_Ticks

Function Description:
    Get the 64 bit contents of the System Timer Counter.

Parameters:
    None

Returns:
    uint64_t: The 64 bit System Timer Counter value.

Assumptions/Limitations:
    None
------------------------------------------------------------------------------*/
uint64_t PSP_Time_Get_Ticks(void);

/*------------------------------------------------------------------------------
Function Name:
    PSP_Time_Delay_Microseconds

Function Description:
    Wait for a specified number of microseconds.

Inputs:
    delay_time_uSec: uint32_t time in uSec to wait.

    the longest time available is 4,294,967,295 microseconds, about 71 minutes.

Returns:
    None

Assumptions/Limitations:
    None
------------------------------------------------------------------------------*/
void PSP_Time_Delay_Microseconds(uint32_t delay_time_uSec);

/*------------------------------------------------------------------------------

Function Name:
    PSP_Time_Initialize_Timer_Counter

Function Description:
    Initialize a periodic timer before using.

Inputs:
    * pCounter: pointer to the counter to initialize.

Returns:
    None

Assumptions/Limitations:
    Assumes that the given periodic timer has a valid timeout period.
------------------------------------------------------------------------------*/
void PSP_Time_Initialize_Timer_Counter(PSP_Time_Periodic_Timer_t * pCounter);

/*------------------------------------------------------------------------------

Function Name:
    PSP_Time_Periodic_Timer_Timeout_Occured

Function Description:
    Poll a given periodic timer and return true if a timeout has occured, 
    otherwise return false.

    If a timeout has occured, the last timeout time in the given periodic 
    timer is set to the time now.

Inputs:
    * pCounter: pointer to the periodic timer.

Returns:
    true if a timeout occured, else false.

Assumptions/Limitations:
    Assumes that the data in the given periodic timer is sensible.
------------------------------------------------------------------------------*/
uint32_t PSP_Time_Periodic_Timer_Timeout_Occured(PSP_Time_Periodic_Timer_t * pCounter);

#endif
