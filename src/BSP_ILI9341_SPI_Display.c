
#include "BSP_ILI9341_SPI_Display.h"

#include "PSP_SPI_0.h"
#include "PSP_GPIO.h"
#include "PSP_Time.h"

 /*-----------------------------------------------------------------------------------------------
    Private BSP_ILI9341_SPI_Display Defines
 -------------------------------------------------------------------------------------------------*/

// ILI9341 command set
#define BSP_ILI9341_NOP        0x00u // No-op register
#define BSP_ILI9341_SWRESET    0x01u // Software reset register
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

// display size properties
#define BSP_ILI9341_TFTWIDTH  240u
#define BSP_ILI9341_TFTHEIGHT 320u

// a few standard colors
#define ILI9341_BLACK       0x0000u  //   0,   0,   0
#define ILI9341_NAVY        0x000Fu  //   0,   0, 128
#define ILI9341_DARKGREEN   0x03E0u  //   0, 128,   0
#define ILI9341_DARKCYAN    0x03EFu  //   0, 128, 128
#define ILI9341_MAROON      0x7800u  // 128,   0,   0
#define ILI9341_PURPLE      0x780Fu  // 128,   0, 128
#define ILI9341_OLIVE       0x7BE0u  // 128, 128,   0
#define ILI9341_LIGHTGREY   0xC618u  // 192, 192, 192
#define ILI9341_DARKGREY    0x7BEFu  // 128, 128, 128
#define ILI9341_BLUE        0x001Fu  //   0,   0, 255
#define ILI9341_GREEN       0x07E0u  //   0, 255,   0
#define ILI9341_CYAN        0x07FFu  //   0, 255, 255
#define ILI9341_RED         0xF800u  // 255,   0,   0
#define ILI9341_MAGENTA     0xF81Fu  // 255,   0, 255
#define ILI9341_YELLOW      0xFFE0u  // 255, 255,   0
#define ILI9341_WHITE       0xFFFFu  // 255, 255, 255
#define ILI9341_ORANGE      0xFD20u  // 255, 165,   0
#define ILI9341_GREENYELLOW 0xAFE5u  // 173, 255,  47
#define ILI9341_PINK        0xF81Fu

#define ILI9341_DC_PIN_WRITE_COMMAND 0u
#define ILI9341_DC_PIN_WRITE_DATA    1u

/*-----------------------------------------------------------------------------------------------
    Private BSP_ILI9341_SPI_Display Variables
 -------------------------------------------------------------------------------------------------*/

typedef struct ILI9341_SPI_Display_Type
{
    uint32_t DC_PIN;
    uint32_t RESET_PIN;

} BSP_ILI9341_SPI_Display_t;



/*-----------------------------------------------------------------------------------------------
    Private BSP_ILI9341_SPI_Display Variables
 -------------------------------------------------------------------------------------------------*/

BSP_ILI9341_SPI_Display_t display;


/*-----------------------------------------------------------------------------------------------
    Private BSP_ILI9341_SPI_Display Functions
 -------------------------------------------------------------------------------------------------*/

void ILI9341_Write_Command(uint8_t command)
{
    PSP_GPIO_Write_Pin(display.DC_PIN, ILI9341_DC_PIN_WRITE_COMMAND);
    PSP_SPI0_Transfer_Byte(command);
    PSP_GPIO_Write_Pin(display.DC_PIN, ILI9341_DC_PIN_WRITE_DATA);
}


/*-----------------------------------------------------------------------------------------------
    BSP_ILI9341_SPI_Display Function Definitions
 -------------------------------------------------------------------------------------------------*/

// note: init function is in a messy and silly state, just for testing currently
void BSP_ILI9341_SPI_Display_Init(uint32_t dc_pin_num, uint32_t reset_pin_num)
{
    PSP_Time_Delay_Microseconds(1000000u);

    // set up the d/c and reset pins
    display.DC_PIN = dc_pin_num;
    display.RESET_PIN = reset_pin_num;
    PSP_GPIO_Set_Pin_Mode(display.DC_PIN,    PSP_GPIO_PINMODE_OUTPUT);
    PSP_GPIO_Set_Pin_Mode(display.RESET_PIN, PSP_GPIO_PINMODE_OUTPUT);

    PSP_GPIO_Write_Pin(display.DC_PIN, ILI9341_DC_PIN_WRITE_COMMAND);

    // start SPI 0, use chip select pin 0
    PSP_SPI0_Start();
    PSP_SPI0_Set_Clock_Divider(PSP_SPI0_Clock_Divider_1024);
    PSP_SPI0_Set_Chip_Select(PSP_SPI_0_Chip_Select_0);

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

    // Main loop - empty the screen as a test.
    int tft_iter = 1u;
    int tft_on = 0u;

    // Set column range.
    ILI9341_Write_Command(BSP_ILI9341_CASET);
    PSP_SPI0_Transfer_16(0x0000u);
    PSP_SPI0_Transfer_16(239u);

    // Set row range.
    ILI9341_Write_Command(BSP_ILI9341_PASET);
    PSP_SPI0_Transfer_16(0x0000u);
    PSP_SPI0_Transfer_16(319u);
    
    // Set 'write to RAM'
    ILI9341_Write_Command(BSP_ILI9341_RAMWR);
    
    while (1) 
    {
        // Write 320 * 240 pixels.
        for (tft_iter = 0u; tft_iter < (320u * 240u); ++tft_iter) 
        {
            // Write a 16-bit color.
            if (tft_on) 
            {
                PSP_SPI0_Transfer_16(ILI9341_RED);
            }
            else 
            {
                PSP_SPI0_Transfer_16(ILI9341_BLUE);
            }
        }

        tft_on ^= 1u;
    }
}
