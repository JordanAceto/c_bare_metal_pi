/*
--|----------------------------------------------------------------------------|
--| FILE DESCRIPTION:
--|   simple_blink.c provides a simple demo which blinks the onboard LED using
--|   the system timer.
--|  
--|   This is the simplest possible demo, and is a good "hello world" program
--|   to check that you can compile/upload code to the pi.
--|
--|   Following this example should give you an idea of how to create and use
--|   basic periodic timers and do basic GPIO pin writing.
--|  
--|----------------------------------------------------------------------------|
--| HARDWARE SETUP:
--|   None. The pi3b+ has an onboard LED on GPIO pin 17. If you want to attach
--|   an LED to a different pin, just change the LED_PIN_NUMBER.
--|
--|----------------------------------------------------------------------------|
*/

/*
--|----------------------------------------------------------------------------|
--| INCLUDE FILES
--|----------------------------------------------------------------------------|
*/

#include "PSP_GPIO.h"
#include "PSP_Time.h"

/*
--|----------------------------------------------------------------------------|
--| DEFINES
--|----------------------------------------------------------------------------|
*/

/*
--| NAME: LED_BLINK_TIME_uSec
--| DESCRIPTION: blink time for the onboard LED in microseconds
--| TYPE: uint32_t
*/
#define LED_BLINK_TIME_uSec (1000000u)

/*
--| NAME: LED_PIN_NUMBER
--| DESCRIPTION: the pin number for the onboard LED
--| TYPE: uint32_t
*/
#define LED_PIN_NUMBER (17u)

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

/*
--| NAME: periodic_timer
--| DESCRIPTION: periodic timer structure for scheduling the LED blink
--| TYPE: PSP_Time_Periodic_Timer_t
*/
PSP_Time_Periodic_Timer_t periodic_timer;

/*
--|----------------------------------------------------------------------------|
--| FUNCTION PROTOTYPES
--|----------------------------------------------------------------------------|
*/

/*------------------------------------------------------------------------------
Function Name:
    main

Function Description:
    main application function which blinks the onboard LED in an endless loop. 

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
    periodic_timer.timeout_period_uSec = LED_BLINK_TIME_uSec;

    PSP_Time_Initialize_Timer_Counter(&periodic_timer);

    PSP_GPIO_Set_Pin_Mode(LED_PIN_NUMBER, PSP_GPIO_PINMODE_OUTPUT);

    // write the pin high right away, so you see the LED light up immediately
    PSP_GPIO_Write_Pin(LED_PIN_NUMBER, PSP_GPIO_PIN_WRITE_HIGH);

    uint32_t led_val = 1u;

    while (1)
    {
        if (PSP_Time_Periodic_Timer_Timeout_Occured(&periodic_timer))
		{
            led_val ^= 1u;
            PSP_GPIO_Write_Pin(LED_PIN_NUMBER, led_val);
		}
    }

    // never reached
    return 0;
}
