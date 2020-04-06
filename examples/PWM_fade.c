/*
--|----------------------------------------------------------------------------|
--| FILE DESCRIPTION:
--|   PWM_fade.c provides a simple demo which fades an LED via PWM.
--|
--|   Following this example should give you a basic idea of how to use PWM
--|   to write a variable output signal.
--|  
--|----------------------------------------------------------------------------|
--| HARDWARE SETUP:
--|   Attach an LED to pin 12.
--|
--|----------------------------------------------------------------------------|
*/

/*
--|----------------------------------------------------------------------------|
--| INCLUDE FILES
--|----------------------------------------------------------------------------|
*/

#include "PSP_PWM.h"
#include "PSP_Time.h"

/*
--|----------------------------------------------------------------------------|
--| DEFINES
--|----------------------------------------------------------------------------|
*/

/*
--| NAME: DELAY_TIME_uSec
--| DESCRIPTION: delay time bewteen PWM writes
--| TYPE: uint32_t
*/
#define DELAY_TIME_uSec (1000u)

/*
--|----------------------------------------------------------------------------|
--| TYPES
--|----------------------------------------------------------------------------|
*/

/* None */

/*
--|----------------------------------------------------------------------------|
--| CONSTANTS
--|----------------------------------------------------------------------------|
*/

/* None */

/*
--|----------------------------------------------------------------------------|
--| VARIABLES
--|----------------------------------------------------------------------------|
*/

/* None */

/*
--|----------------------------------------------------------------------------|
--| FUNCTION PROTOTYPES
--|----------------------------------------------------------------------------|
*/

/*------------------------------------------------------------------------------
Function Name:
    main

Function Description:
    main application function which fades an LED on pin 12 in an endless loop. 

Parameters:
    None

Returns:
    int [return is never reached]

Assumptions/Limitations:
    None
------------------------------------------------------------------------------*/
int main(void);

/*
--|----------------------------------------------------------------------------|
--| FUNCTION DEFINITIONS
--|----------------------------------------------------------------------------|
*/

int main(void)
{
    // use the default settings for clock source and divider: 19.2MHz oscillator
    // and divider of 4 means base PWM frequency is 19.2 / 4 = 4.8MHz
    PSP_PWM_Clock_Init(PSP_PWM_Clock_Source_OSCILLATOR, PWM_DEFAULT_DIV);

    // start up channel 1 in mark-space mode with a resolution of 10 bits (1024 values)
    // base PWM frequency of 4.8MHz / 1024 values = final PWM frequency of about 4.7kHz
    PSP_PWM_Channel_Start(PSP_PWM_Channel_1, PSP_PWM_MARK_SPACE_MODE, PSP_PWM_RANGE_10_BITS);

    // set GPIO pin 12 to PWM mode
    PSP_PWM_Ch1_Set_GPIO12_To_PWM_Mode();

    uint32_t pwm_val = 0u;

    while(1)
    {
        // write a ramp wave via PWM
        PSP_PWM_Ch1_Write(pwm_val);
        pwm_val++;
        pwm_val %= PSP_PWM_RANGE_10_BITS;

        // delay so that the period is slowed down and predicatable
        PSP_Time_Delay_Microseconds(DELAY_TIME_uSec);
    }

    // never reached
    return 0;
}
