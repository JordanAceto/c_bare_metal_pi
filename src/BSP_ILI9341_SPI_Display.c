/*
--|----------------------------------------------------------------------------|
--| FILE DESCRIPTION:
--|   BSP_ILI9341_SPI_Display.c, just a start for driving cheap SPI screens.
--|  
--|----------------------------------------------------------------------------|
--| REFERENCES:
--|   see BSP_ILI9341_SPI_Display.h
--|
--|----------------------------------------------------------------------------|
*/

/*
--|----------------------------------------------------------------------------|
--| INCLUDE FILES
--|----------------------------------------------------------------------------|
*/

#include "BSP_ILI9341_SPI_Display.h"
#include "PSP_GPIO.h"
#include "PSP_SPI_0.h"
#include "PSP_Time.h"

/*
--|----------------------------------------------------------------------------|
--| PRIVATE DEFINES
--|----------------------------------------------------------------------------|
*/

// ILI9341 command set
#define BSP_ILI9341_NOP        0x00u // No-op command
#define BSP_ILI9341_SWRESET    0x01u // Software reset command
#define BSP_ILI9341_RDDID      0x04u // Read display identification information
#define BSP_ILI9341_RDDST      0x09u // Read Display Status

#define BSP_ILI9341_RDMODE     0x0Au // Read Display Power Mode
#define BSP_ILI9341_RDMADCTL   0x0Bu // Read Display MADCTL
#define BSP_ILI9341_RDPIXFMT   0x0Cu // Read Display Pixel Format
#define BSP_ILI9341_RDIMGFMT   0x0Du // Read Display Image Format
#define BSP_ILI9341_RDSELFDIAG 0x0Fu // Read Display Self-Diagnostic Result

#define BSP_ILI9341_SLPIN      0x10u // Enter Sleep Mode
#define BSP_ILI9341_SLPOUT     0x11u // Sleep Out
#define BSP_ILI9341_PTLON      0x12u // Partial Mode ON
#define BSP_ILI9341_NORON      0x13u // Normal Display Mode ON

#define BSP_ILI9341_INVOFF     0x20u // Display Inversion OFF
#define BSP_ILI9341_INVON      0x21u // Display Inversion ON
#define BSP_ILI9341_GAMMASET   0x26u // Gamma Set
#define BSP_ILI9341_DISPOFF    0x28u // Display OFF
#define BSP_ILI9341_DISPON     0x29u // Display ON

#define BSP_ILI9341_CASET      0x2Au // Column Address Set
#define BSP_ILI9341_PASET      0x2Bu // Page Address Set
#define BSP_ILI9341_RAMWR      0x2Cu // Memory Write
#define BSP_ILI9341_RAMRD      0x2Eu // Memory Read

#define BSP_ILI9341_PTLAR      0x30u // Partial Area
#define BSP_ILI9341_VSCRDEF    0x33u // Vertical Scrolling Definition
#define BSP_ILI9341_MADCTL     0x36u // Memory Access Control
#define BSP_ILI9341_VSCRSADD   0x37u // Vertical Scrolling Start Address
#define BSP_ILI9341_PIXFMT     0x3Au // COLMOD: Pixel Format Set

#define BSP_ILLI9341_FRMCTR1   0xB1u // Frame Rate Control (In Normal Mode/Full Colors)
#define BSP_ILI9341_FRMCTR2    0xB2u // Frame Rate Control (In Idle Mode/8 colors)
#define BSP_ILI9341_FRMCTR3    0xB3u // Frame Rate control (In Partial Mode/Full Colors)
#define BSP_ILI9341_INVCTR     0xB4u // Display Inversion Control
#define BSP_ILI9341_DFUNCTR    0xB6u // Display Function Control

#define BSP_ILI9341_PWCTR1     0xC0u // Power Control 1
#define BSP_ILI9341_PWCTR2     0xC1u // Power Control 2
#define BSP_ILI9341_PWCTR3     0xC2u // Power Control 3
#define BSP_ILI9341_PWCTR4     0xC3u // Power Control 4
#define BSP_ILI9341_PWCTR5     0xC4u // Power Control 5
#define BSP_ILI9341_VMCTR1     0xC5u // VCOM Control 1
#define BSP_ILI9341_VMCTR2     0xC7u // VCOM Control 2

#define BSP_ILI9341_RDID1      0xDAu // Read ID 1
#define BSP_ILI9341_RDID2      0xDBu // Read ID 2
#define BSP_ILI9341_RDID3      0xDCu // Read ID 3
#define BSP_ILI9341_RDID4      0xDDu // Read ID 4

#define BSP_ILI9341_GMCTRP1    0xE0u // Positive Gamma Correction
#define BSP_ILI9341_GMCTRN1    0xE1u // Negative Gamma Correction

#define ILI9341_DC_PIN_WRITE_COMMAND 0u
#define ILI9341_DC_PIN_WRITE_DATA    1u

#define FIFO_SEND_LIMIT 16u

/*
--|----------------------------------------------------------------------------|
--| PRIVATE TYPES
--|----------------------------------------------------------------------------|
*/

/* None */

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

static uint32_t DC_PIN;

/*
--|----------------------------------------------------------------------------|
--| PRIVATE HELPER FUNCTION PROTOTYPES
--|----------------------------------------------------------------------------|
*/

/*------------------------------------------------------------------------------
Function Name:
    ILI9341_Write_Command

Function Description:
    Write a command to the display. 

Parameters:
    command: the command to write
    
Returns:
    None

Assumptions/Limitations:
    SPI write commands are standalone one shot writes that are not part of a
    longer transfer. 
------------------------------------------------------------------------------*/
void ILI9341_Write_Command(uint8_t command);

/*------------------------------------------------------------------------------
Function Name:
    ILI9341_Send_Command

Function Description:
    Send a command to the display. 

Parameters:
    command: the command to send
    
Returns:
    None

Assumptions/Limitations:
    SPI send commands are part of a larger transfer, and it is assumed that
    this will be sandwiched between calls to PSP_SPI0_Begin_Transfer and 
    PSP_SPI0_End_Transfer.
------------------------------------------------------------------------------*/
void ILI9341_Send_Command(uint8_t command);

/*------------------------------------------------------------------------------
Function Name:
    BSP_ILI9341_Send_Address

Function Description:
    Send the coordinates of a window address to the display. 

Parameters:
    x0, y0, x1, y1: the coordinates of the rectangular window.
    
Returns:
    None

Assumptions/Limitations:
    SPI send commands are part of a larger transfer, and it is assumed that
    this will be sandwiched between calls to PSP_SPI0_Begin_Transfer and 
    PSP_SPI0_End_Transfer.
------------------------------------------------------------------------------*/
void BSP_ILI9341_Send_Address(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);

/*------------------------------------------------------------------------------
Function Name:
    BSP_ILI9341_Send_Pixel_Data

Function Description:
    Send the data for a single pixel. 

Parameters:
    x, y: the coordinates of the pixel.
    color: the color of the pixel.

Returns:
    None

Assumptions/Limitations:
    SPI send commands are part of a larger transfer, and it is assumed that
    this will be sandwiched between calls to PSP_SPI0_Begin_Transfer and 
    PSP_SPI0_End_Transfer.
------------------------------------------------------------------------------*/
void BSP_ILI9341_Send_Pixel_Data(uint16_t x, uint16_t y, uint16_t color);

/*
--|----------------------------------------------------------------------------|
--| PUBLIC FUNCTION DEFINITIONS
--|----------------------------------------------------------------------------|
*/

// note: init function is in a messy and silly state, just for testing currently
void BSP_ILI9341_SPI_Display_Init(uint32_t dc_pin_num)
{
    // set up the d/c pin
    DC_PIN = dc_pin_num;
    PSP_GPIO_Set_Pin_Mode(DC_PIN, PSP_GPIO_PINMODE_OUTPUT);

    // start SPI 0, use chip select pin 0
    PSP_SPI0_Start();
    // clock div 8 = 32.2MHz, faster speeds seemed to have problems, TODO: investigate
    PSP_SPI0_Set_Clock_Divider(PSP_SPI0_Clock_Divider_8);
    PSP_SPI0_Set_Chip_Select(PSP_SPI_0_Chip_Select_0);

    // sw reset means we don't need to use up a pin for the reset line
    ILI9341_Write_Command(BSP_ILI9341_SWRESET);
    PSP_Time_Delay_Microseconds(150000u);

    // init sequence from various sources from the web
    ILI9341_Write_Command(0xEFu);
    PSP_SPI0_Transfer_Byte(0x03u);
    PSP_SPI0_Transfer_Byte(0x80u);
    PSP_SPI0_Transfer_Byte(0x02u);

    ILI9341_Write_Command(0xCFu);
    PSP_SPI0_Transfer_Byte(0x00u);
    PSP_SPI0_Transfer_Byte(0xC1u);
    PSP_SPI0_Transfer_Byte(0x30u);

    ILI9341_Write_Command(0xEDu);
    PSP_SPI0_Transfer_Byte(0x64u);
    PSP_SPI0_Transfer_Byte(0x03u);
    PSP_SPI0_Transfer_Byte(0x12u);
    PSP_SPI0_Transfer_Byte(0x81u);

    ILI9341_Write_Command(0xE8u);
    PSP_SPI0_Transfer_Byte(0x85u);
    PSP_SPI0_Transfer_Byte(0x00u);
    PSP_SPI0_Transfer_Byte(0x78u);

    ILI9341_Write_Command(0xCBu);
    PSP_SPI0_Transfer_Byte(0x39u);
    PSP_SPI0_Transfer_Byte(0x2Cu);
    PSP_SPI0_Transfer_Byte(0x00u);
    PSP_SPI0_Transfer_Byte(0x34u);
    PSP_SPI0_Transfer_Byte(0x02u);

    ILI9341_Write_Command(0xF7u);
    PSP_SPI0_Transfer_Byte(0x20u);

    ILI9341_Write_Command(0xEAu);
    PSP_SPI0_Transfer_Byte(0x00u);
    PSP_SPI0_Transfer_Byte(0x00u);

    ILI9341_Write_Command(BSP_ILI9341_PWCTR1);
    PSP_SPI0_Transfer_Byte(0x23u);

    ILI9341_Write_Command(BSP_ILI9341_PWCTR2);
    PSP_SPI0_Transfer_Byte(0x10u);

    ILI9341_Write_Command(BSP_ILI9341_VMCTR1);
    PSP_SPI0_Transfer_Byte(0x3Eu);
    PSP_SPI0_Transfer_Byte(0x28u);

    ILI9341_Write_Command(BSP_ILI9341_VMCTR2);
    PSP_SPI0_Transfer_Byte(0x86u);

    ILI9341_Write_Command(BSP_ILI9341_MADCTL);
    PSP_SPI0_Transfer_Byte(0x48u);

    ILI9341_Write_Command(BSP_ILI9341_VSCRSADD);
    PSP_SPI0_Transfer_Byte(0x00u);

    ILI9341_Write_Command(BSP_ILI9341_PIXFMT);
    PSP_SPI0_Transfer_Byte(0x55u);

    ILI9341_Write_Command(BSP_ILLI9341_FRMCTR1);
    PSP_SPI0_Transfer_Byte(0x00u);
    PSP_SPI0_Transfer_Byte(0x18u);

    ILI9341_Write_Command(BSP_ILI9341_DFUNCTR);
    PSP_SPI0_Transfer_Byte(0x08u);
    PSP_SPI0_Transfer_Byte(0x82u);
    PSP_SPI0_Transfer_Byte(0x27u);

    ILI9341_Write_Command(0xF2u);
    PSP_SPI0_Transfer_Byte(0x00u);

    ILI9341_Write_Command(BSP_ILI9341_GAMMASET);
    PSP_SPI0_Transfer_Byte(0x01u);

    ILI9341_Write_Command(BSP_ILI9341_GMCTRP1);
    PSP_SPI0_Transfer_Byte(0x0Fu);
    PSP_SPI0_Transfer_Byte(0x31u);
    PSP_SPI0_Transfer_Byte(0x2Bu);
    PSP_SPI0_Transfer_Byte(0x0Cu);
    PSP_SPI0_Transfer_Byte(0x0Eu);
    PSP_SPI0_Transfer_Byte(0x08u);
    PSP_SPI0_Transfer_Byte(0x4Eu);
    PSP_SPI0_Transfer_Byte(0xF1u);
    PSP_SPI0_Transfer_Byte(0x37u);
    PSP_SPI0_Transfer_Byte(0x07u);
    PSP_SPI0_Transfer_Byte(0x10u);
    PSP_SPI0_Transfer_Byte(0x03u);
    PSP_SPI0_Transfer_Byte(0x0Eu);
    PSP_SPI0_Transfer_Byte(0x09u);
    PSP_SPI0_Transfer_Byte(0x00u);

    ILI9341_Write_Command(BSP_ILI9341_GMCTRN1);
    PSP_SPI0_Transfer_Byte(0x00u);
    PSP_SPI0_Transfer_Byte(0x0Eu);
    PSP_SPI0_Transfer_Byte(0x14u);
    PSP_SPI0_Transfer_Byte(0x03u);
    PSP_SPI0_Transfer_Byte(0x11u);
    PSP_SPI0_Transfer_Byte(0x07u);
    PSP_SPI0_Transfer_Byte(0x31u);
    PSP_SPI0_Transfer_Byte(0xC1u);
    PSP_SPI0_Transfer_Byte(0x48u);
    PSP_SPI0_Transfer_Byte(0x08u);
    PSP_SPI0_Transfer_Byte(0x0Fu);
    PSP_SPI0_Transfer_Byte(0x0Cu);
    PSP_SPI0_Transfer_Byte(0x31u);
    PSP_SPI0_Transfer_Byte(0x36u);
    PSP_SPI0_Transfer_Byte(0x0Fu);

    ILI9341_Write_Command(BSP_ILI9341_SLPOUT);
    PSP_Time_Delay_Microseconds(200000u);

    ILI9341_Write_Command(BSP_ILI9341_DISPON);
    PSP_Time_Delay_Microseconds(200000u);

    ILI9341_Write_Command(BSP_ILI9341_NORON);
}

void BSP_ILI9341_Set_Window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    PSP_SPI0_Begin_Transfer();
    BSP_ILI9341_Send_Address(x0, y0, x1, y1);
    PSP_SPI0_End_Transfer();
}

void BSP_ILI9341_Draw_Pixel(uint32_t x, uint32_t y, uint16_t color)
{
    PSP_SPI0_Begin_Transfer();
    BSP_ILI9341_Send_Pixel_Data(x, y, color);
    PSP_SPI0_End_Transfer();
}

void BSP_ILI9341_Draw_Horizontal_Line(uint32_t x, 
                                      uint32_t y, 
                                      uint32_t length, 
                                      uint16_t color)
{
    PSP_SPI0_Begin_Transfer();

    BSP_ILI9341_Send_Address(x, y, x + length - 1u, y);

    for (int i = 0u; i < length; i++)
    {
        PSP_SPI0_Send_16(color);

        // break up the transfer if it gets long
        if (i % FIFO_SEND_LIMIT == 0u)
        {
            PSP_SPI0_End_Transfer();
            PSP_SPI0_Begin_Transfer();
        }
    }

    PSP_SPI0_End_Transfer();
}

void BSP_ILI9341_Draw_Vertical_Line(uint32_t x, 
                                    uint32_t y, 
                                    uint32_t height, 
                                    uint16_t color)
{
    PSP_SPI0_Begin_Transfer();

    BSP_ILI9341_Send_Address(x, y, x, y + height - 1u);

    for (int i = 0u; i < height; i++)
    {
        PSP_SPI0_Send_16(color);

        // break up the transfer if it gets long
        if (i % FIFO_SEND_LIMIT == 0u)
        {
            PSP_SPI0_End_Transfer();
            PSP_SPI0_Begin_Transfer();
        }
    }

    PSP_SPI0_End_Transfer();
}

void BSP_ILI9341_Draw_Filled_Rectangle(uint32_t x, 
                                       uint32_t y, 
                                       uint32_t width, 
                                       uint32_t height, 
                                       uint16_t color)
{
    PSP_SPI0_Begin_Transfer();

    BSP_ILI9341_Send_Address(x, y, x + width - 1u, y + height - 1u);

    const uint32_t num_pixels = width * height;

    for (uint32_t i = 0u; i < num_pixels; i++)
    {
        PSP_SPI0_Send_16(color);

        // break up the transfer if it gets long
        if (i % FIFO_SEND_LIMIT == 0u)
        {
            PSP_SPI0_End_Transfer();
            PSP_SPI0_Begin_Transfer();
        }
    }

    PSP_SPI0_End_Transfer();
}

void BSP_ILI9341_Draw_Rectangle_Outline(uint32_t x, 
                                        uint32_t y, 
                                        uint32_t width, 
                                        uint32_t height, 
                                        uint16_t color)
{
    BSP_ILI9341_Draw_Horizontal_Line(x, y, width, color);
    BSP_ILI9341_Draw_Horizontal_Line(x, y + height - 1u, width, color);

    BSP_ILI9341_Draw_Vertical_Line(x, y, height, color);
    BSP_ILI9341_Draw_Vertical_Line(x + width - 1u, y, height, color);
}

void BSP_ILI9341_Draw_Circle_Outline(uint32_t x, 
                                     uint32_t y, 
                                     uint32_t r, 
                                     uint16_t color)
{
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x_ = 0;
    int16_t y_ = r;

    BSP_ILI9341_Draw_Pixel(x    , y + r, color);
    BSP_ILI9341_Draw_Pixel(x    , y - r, color);
    BSP_ILI9341_Draw_Pixel(x + r, y    , color);
    BSP_ILI9341_Draw_Pixel(x - r, y    , color);

    while (x_ < y_) 
    {
        if (f >= 0) 
        {
            y_--;
            ddF_y += 2;
            f += ddF_y;
        }

        x_++;
        ddF_x += 2;
        f += ddF_x;

        BSP_ILI9341_Draw_Pixel(x + x_, y + y_, color);
        BSP_ILI9341_Draw_Pixel(x - x_, y + y_, color);
        BSP_ILI9341_Draw_Pixel(x + x_, y - y_, color);
        BSP_ILI9341_Draw_Pixel(x - x_, y - y_, color);
        BSP_ILI9341_Draw_Pixel(x + y_, y + x_, color);
        BSP_ILI9341_Draw_Pixel(x - y_, y + x_, color);
        BSP_ILI9341_Draw_Pixel(x + y_, y - x_, color);
        BSP_ILI9341_Draw_Pixel(x - y_, y - x_, color);
    }
}

/*
--|----------------------------------------------------------------------------|
--| PRIVATE HELPER FUNCTION DEFINITIONS
--|----------------------------------------------------------------------------|
*/

void ILI9341_Write_Command(uint8_t command)
{
    PSP_GPIO_Write_Pin(DC_PIN, ILI9341_DC_PIN_WRITE_COMMAND);
    PSP_SPI0_Transfer_Byte(command);
    PSP_GPIO_Write_Pin(DC_PIN, ILI9341_DC_PIN_WRITE_DATA);
}

void ILI9341_Send_Command(uint8_t command)
{
    PSP_GPIO_Write_Pin(DC_PIN, ILI9341_DC_PIN_WRITE_COMMAND);
    PSP_SPI0_Send_Byte(command);
    PSP_GPIO_Write_Pin(DC_PIN, ILI9341_DC_PIN_WRITE_DATA);
}

void BSP_ILI9341_Send_Address(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    // column set
    ILI9341_Send_Command(BSP_ILI9341_CASET);
    PSP_SPI0_Send_16(x0);
    PSP_SPI0_Send_16(x1);

    // row set
    ILI9341_Send_Command(BSP_ILI9341_PASET);
    PSP_SPI0_Send_16(y0);
    PSP_SPI0_Send_16(y1);
    
    // write to RAM
    ILI9341_Send_Command(BSP_ILI9341_RAMWR);
}

void BSP_ILI9341_Send_Pixel_Data(uint16_t x, uint16_t y, uint16_t color)
{
    BSP_ILI9341_Send_Address(x, y, x, y);
    PSP_SPI0_Send_16(color);
}
