/**
 * DESCRIPTION:
 *      BSP_ILI9341_SPI_Display, just a start for driving cheap SPI screens
 * 
 * NOTES:
 *      The initialization sequence is confusing, and hard to find out why it 
 *      is the way it is in the datasheet. Several successful ILI9341 libraries
 *      were used as references to figure out the init sequence.
 * 
 *      At the moment, calling the init function initializes the display
 *      and then fills the screen with alternating colors forever.
 * 
 *      This is just the very early testing phase. 
 * 
 * 
 * REFERENCES:
 *      ILI9341.pdf
 *      https://github.com/PaulStoffregen/ILI9341_t3
 *      https://github.com/adafruit/Adafruit_ILI9341
 *      https://github.com/mongoose-os-libs/ili9341-spi
 *      https://vivonomicon.com/2018/06/17/drawing-to-a-small-tft-display-the-ili9341-and-stm32/
 */

#ifndef BSP_ILI9341_SPI_DISPLAY_H_INCLUDED
#define BSP_ILI9341_SPI_DISPLAY_H_INCLUDED

#include "Fixed_Width_Ints.h"


/*-----------------------------------------------------------------------------------------------
    Public BSP_ILI9341_SPI_Display Function Declarations
 -------------------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------

Function Name:
    BSP_ILI9341_SPI_Display_Init

Function Description:
    Initialize a ILI9341 SPI display by initializing PSP_SPI_0, setting a GPIO pin to be used as
     and sending the appropriate 
    initialization sequence to the ILI9341.

Inputs:
    pEncoder: pointer to the encoder to initialize.

Returns:
    None

Error Handling:
    None

-------------------------------------------------------------------------------------------------*/
void BSP_ILI9341_SPI_Display_Init(uint32_t dc_pin_num, uint32_t reset_pin_num);


#endif
