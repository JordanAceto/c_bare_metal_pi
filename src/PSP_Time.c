/*
--|----------------------------------------------------------------------------|
--| FILE DESCRIPTION:
--|   PSP_Time.c provides the register addresses and pointers for the system
--|   timer as well as the implementation for the various timer functions.
--|
--|----------------------------------------------------------------------------|
--| REFERENCES:
--|   BCM2837-ARM-Peripherals.pdf page 172
--|
--|----------------------------------------------------------------------------|
*/

/*
--|----------------------------------------------------------------------------|
--| INCLUDE FILES
--|----------------------------------------------------------------------------|
*/

#include "PSP_Time.h"
#include "PSP_REGS.h"

/*
--|----------------------------------------------------------------------------|
--| PRIVATE DEFINES
--|----------------------------------------------------------------------------|
*/

/*
--| NAME: System_Timer
--| DESCRIPTION: pointer to the 64 bit system timer
--| TYPE: PSP_Time_System_Timer_t *
*/
#define System_Timer ((volatile PSP_Time_System_Timer_t *)PSP_REGS_SYSCLK_BASE_ADDRESS)

/*
--|----------------------------------------------------------------------------|
--| PRIVATE TYPES
--|----------------------------------------------------------------------------|
*/

/*
--| NAME: PSP_Time_System_Timer_t
--| DESCRIPTION: structure for the 64 bit system timer registers
*/
typedef struct PSP_Time_System_Timer_Type
{
    vuint32_t CS;  // System Timer Control/Status
    vuint32_t CLO; // System Timer Counter Lower 32 bits
    vuint32_t CHI; // System Timer Counter Higher 32 bits
    vuint32_t C0;  // System Timer Compare 0
    vuint32_t C1;  // System Timer Compare 1
    vuint32_t C2;  // System Timer Compare 2
    vuint32_t C3;  // System Timer Compare 3
} PSP_Time_System_Timer_t;


/*
--|----------------------------------------------------------------------------|
--| PRIVATE CONSTANTS
--|----------------------------------------------------------------------------|
*/

/* None */

/*
--|----------------------------------------------------------------------------|
--| PRIVATE VARIABLES
--|----------------------------------------------------------------------------|
*/

/* None */

/*
--|----------------------------------------------------------------------------|
--| PRIVATE HELPER FUNCTION PROTOTYPES
--|----------------------------------------------------------------------------|
*/

/* None */

/*
--|----------------------------------------------------------------------------|
--| PUBLIC FUNCTION DEFINITIONS
--|----------------------------------------------------------------------------|
*/

uint64_t PSP_Time_Get_Ticks(void)
{
    volatile uint32_t CHI_reading = System_Timer->CHI;
    volatile uint32_t CLO_reading = System_Timer->CLO;

    // if the upper 32 bit reading has changed, take new readings
    if (CHI_reading != System_Timer->CHI)
    {
        CHI_reading = System_Timer->CHI;
        CLO_reading = System_Timer->CLO;
    }

    return (uint64_t)CHI_reading << 32u | CLO_reading;
}

void PSP_Time_Delay_Microseconds(uint32_t delay_time_uSec)
{
    const uint64_t end_time = PSP_Time_Get_Ticks() + delay_time_uSec;

    while (PSP_Time_Get_Ticks() < end_time)
    {
        // wait
    }
}

void PSP_Time_Initialize_Timer_Counter(PSP_Time_Periodic_Timer_t * pCounter)
{
    pCounter->last_timeout_time_uSec = PSP_Time_Get_Ticks();
}

uint32_t PSP_Time_Periodic_Timer_Timeout_Occured(PSP_Time_Periodic_Timer_t * pCounter)
{
    uint32_t retval = 0;

    if ((PSP_Time_Get_Ticks() - pCounter->last_timeout_time_uSec) > pCounter->timeout_period_uSec)
    {
        pCounter->last_timeout_time_uSec = PSP_Time_Get_Ticks();
        retval = 1u;
    }

    return retval;
}

/*
--|----------------------------------------------------------------------------|
--| PRIVATE HELPER FUNCTION DEFINITIONS
--|----------------------------------------------------------------------------|
*/

/* None */
