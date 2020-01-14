/**
 * DESCRIPTION:
 *      BSP_ILI9341_SPI_Display, just a start for driving cheap SPI screens
 * 
 * NOTES:
 *      The initialization sequence is confusing, and hard to find out why it 
 *      is the way it is in the datasheet. Several successful ILI9341 libraries
 *      were used as references to figure out the init sequence.
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
    Public BSP_ILI9341_SPI_Display Defines
 -------------------------------------------------------------------------------------------------*/

// display size properties
#define BSP_ILI9341_TFTWIDTH  240u
#define BSP_ILI9341_TFTHEIGHT 320u

// a few standard colors
#define BSP_ILI9341_BLACK       0x0000u
#define BSP_ILI9341_NAVY        0x000Fu
#define BSP_ILI9341_DARKGREEN   0x03E0u
#define BSP_ILI9341_DARKCYAN    0x03EFu
#define BSP_ILI9341_MAROON      0x7800u
#define BSP_ILI9341_PURPLE      0x780Fu
#define BSP_ILI9341_OLIVE       0x7BE0u
#define BSP_ILI9341_LIGHTGREY   0xC618u
#define BSP_ILI9341_DARKGREY    0x7BEFu
#define BSP_ILI9341_BLUE        0x001Fu
#define BSP_ILI9341_GREEN       0x07E0u
#define BSP_ILI9341_CYAN        0x07FFu
#define BSP_ILI9341_RED         0xF800u
#define BSP_ILI9341_MAGENTA     0xF81Fu
#define BSP_ILI9341_YELLOW      0xFFE0u
#define BSP_ILI9341_WHITE       0xFFFFu
#define BSP_ILI9341_ORANGE      0xFD20u
#define BSP_ILI9341_GREENYELLOW 0xAFE5u
#define BSP_ILI9341_PINK        0xF81Fu


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
    dc_pin_num: the pin number for the D/C pin

Returns:
    None

Error Handling:
    None

-------------------------------------------------------------------------------------------------*/
void BSP_ILI9341_SPI_Display_Init(uint32_t dc_pin_num);



/*-----------------------------------------------------------------------------------------------

Function Name:
    BSP_ILI9341_Set_Window

Function Description:
    Set the rectangular window to use for drawing.

Inputs:
    x0, y0, x1, y1: the corners of the rectangular window.

Returns:
    None

Error Handling:
    None

-------------------------------------------------------------------------------------------------*/
void BSP_ILI9341_Set_Window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);



/*-----------------------------------------------------------------------------------------------

Function Name:
    BSP_ILI9341_Draw_Pixel

Function Description:
    Draws a single pixel on the screen.

Inputs:
    x, y: the coordinates of the pixel.
    color: the 16 bit 5-6-5 color for the pixel.

Returns:
    None

Error Handling:
    None

-------------------------------------------------------------------------------------------------*/
void BSP_ILI9341_Draw_Pixel(uint32_t x, uint32_t y, uint16_t color);



/*-----------------------------------------------------------------------------------------------

Function Name:
    BSP_ILI9341_Draw_Horizontal_Line

Function Description:
    Draws a horizontal line on the screen.

    The line starts at the given starting coordinates and goes right.

Inputs:
    x, y: the starting coordinates.
    length: the length of the line in pixels. 
    color: the 16 bit 5-6-5 color for the line.

Returns:
    None

Error Handling:
    None

-------------------------------------------------------------------------------------------------*/
void BSP_ILI9341_Draw_Horizontal_Line(uint32_t x, uint32_t y, uint32_t length, uint16_t color);



/*-----------------------------------------------------------------------------------------------

Function Name:
    BSP_ILI9341_Draw_Vertical_Line

Function Description:
    Draws a vertical line on the screen.

    The line starts at the given starting coordinates and goes down.

Inputs:
    x, y: the starting coordinates.
    height: the height of the line in pixels. 
    color: the 16 bit 5-6-5 color for the line.

Returns:
    None

Error Handling:
    None

-------------------------------------------------------------------------------------------------*/
void BSP_ILI9341_Draw_Vertical_Line(uint32_t x, uint32_t y, uint32_t height, uint16_t color);



/*-----------------------------------------------------------------------------------------------

Function Name:
    BSP_ILI9341_Draw_Filled_Rectangle

Function Description:
    Draws a filled in rectangle on the screen.

    The given (x, y) coordinate is the upper left corner of the rectangle.

Inputs:
    x, y: the upper left coordinates of the rectangle.
    width: the width of the rectangle in pixels.
    height: the height of the rectangle in pixels. 
    color: the 16 bit 5-6-5 color for the rectangle.

Returns:
    None

Error Handling:
    None

-------------------------------------------------------------------------------------------------*/
void BSP_ILI9341_Draw_Filled_Rectangle(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint16_t color);



/*-----------------------------------------------------------------------------------------------

Function Name:
    BSP_ILI9341_Draw_Rectangle_Outline

Function Description:
    Draws a rectangular outline on the screen.

    The given (x, y) coordinate is the upper left corner of the rectangle.

Inputs:
    x, y: the upper left coordinates of the rectangle.
    width: the width of the rectangle in pixels.
    height: the height of the rectangle in pixels. 
    color: the 16 bit 5-6-5 color for the rectangle.

Returns:
    None

Error Handling:
    None

-------------------------------------------------------------------------------------------------*/
void BSP_ILI9341_Draw_Rectangle_Outline(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint16_t color);



#endif
