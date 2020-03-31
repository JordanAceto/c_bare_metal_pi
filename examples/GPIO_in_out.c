/*
--|----------------------------------------------------------------------------|
--| FILE DESCRIPTION:
--|   GPIO_in_out.c provides a simple demo which turns on the onboard LED 
--|    when a pin which has been set as an input is pulled high.
--|  
--|   Following this example should give you an idea of how to do basic 
--|   GPIO pin reading and writing.
--|  
--|----------------------------------------------------------------------------|
--| HARDWARE SETUP:
--|   Pull the SWITCH_PIN high to turn the onboard LED on, and pull it low
--|   to turn the LED off.
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
--| NAME: LED_PIN
--| DESCRIPTION: the pin number for the onboard LED
--| TYPE: uint32_t
*/
#define LED_PIN (17u)

/*
--| NAME: SWITCH_PIN
--| DESCRIPTION: the pin number for the switch
--| TYPE: uint32_t
*/
#define SWITCH_PIN (21u)

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
    main application function which uses a GPIO input pin to turn the 
    onboard LED on and off.

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
    PSP_GPIO_Set_Pin_Mode(LED_PIN, PSP_GPIO_PINMODE_OUTPUT);
    PSP_GPIO_Set_Pin_Mode(SWITCH_PIN, PSP_GPIO_PINMODE_INPUT);

    while (1)
    {
        PSP_GPIO_Write_Pin(LED_PIN, PSP_GPIO_Read_Pin(SWITCH_PIN));
        
        // below is a more longwinded way of doing the same thing:
        /*
        if (PSP_GPIO_Read_Pin(SWITCH_PIN))
        {
            PSP_GPIO_Write_Pin(LED_PIN, PSP_GPIO_PIN_WRITE_HIGH);
        }
        else
        {
            PSP_GPIO_Write_Pin(LED_PIN, PSP_GPIO_PIN_WRITE_LOW);
        }
        */
    }

    // never reached
    return 0;
}
