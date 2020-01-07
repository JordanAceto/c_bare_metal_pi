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
#include "PSP_REGS.h"

/*-----------------------------------------------------------------------------------------------
    Public PSP_Time Defines
 -------------------------------------------------------------------------------------------------*/

// Register Addresses
#define PSP_Time_BASE_ADDRESS (PSP_REGS_SYSCLK_BASE_ADDRESS)           // base address of System Timer Register

#define PSP_Time_CS_A         (PSP_Time_BASE_ADDRESS | 0x00000000u)    // System Timer Control/Status address
#define PSP_Time_CLO_A        (PSP_Time_BASE_ADDRESS | 0x00000004u)    // System Timer Counter Lower 32 bits address
#define PSP_Time_CHI_A        (PSP_Time_BASE_ADDRESS | 0x00000008u)    // System Timer Counter Higher 32 bits address
#define PSP_Time_C0_A         (PSP_Time_BASE_ADDRESS | 0x0000000Cu)    // System Timer Compare 0 address
#define PSP_Time_C1_A         (PSP_Time_BASE_ADDRESS | 0x00000010u)    // System Timer Compare 1 address
#define PSP_Time_C2_A         (PSP_Time_BASE_ADDRESS | 0x00000014u)    // System Timer Compare 2 address
#define PSP_Time_C3_A         (PSP_Time_BASE_ADDRESS | 0x00000018u)    // System Timer Compare 3 address

// Register Pointers
#define PSP_Time_CS_R         (*((volatile uint32_t *)PSP_Time_CS_A))  // System Timer Control/Status register
#define PSP_Time_CLO_R        (*((volatile uint32_t *)PSP_Time_CLO_A)) // System Timer Counter Lower 32 bits register
#define PSP_Time_CHI_R        (*((volatile uint32_t *)PSP_Time_CHI_A)) // System Timer Counter Higher 32 bits register
#define PSP_Time_C0_R         (*((volatile uint32_t *)PSP_Time_C0_A))  // System Timer Compare 0 register
#define PSP_Time_C1_R         (*((volatile uint32_t *)PSP_Time_C1_A))  // System Timer Compare 1 register
#define PSP_Time_C2_R         (*((volatile uint32_t *)PSP_Time_C2_A))  // System Timer Compare 2 register
#define PSP_Time_C3_R         (*((volatile uint32_t *)PSP_Time_C3_A))  // System Timer Compare 3 register



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
void PSP_Time_Delay_Microseconds(uint32_t delay_time_uSec);

#endif
