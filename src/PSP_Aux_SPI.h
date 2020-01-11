/**
 * DESCRIPTION:
 *      Just a start for Aux SPI 1 and 2, nothing works yet
 * 
 * NOTES:
 * 
 * REFERENCES:
 *      BCM2837-ARM-Peripherals.pdf page 8
 */

#ifndef PSP_AUX_SPI_H_INCLUDED
#define PSP_AUX_SPI_H_INCLUDED

#include "Fixed_Width_Ints.h"


/*------------------------------------------------------------------------------------------------
    Public PSP_Aux_SPI Defines
 -------------------------------------------------------------------------------------------------*/

// SPI 1 GPIO pin numbers
#define PSP_SPI_1_CE2_PIN   16u
#define PSP_SPI_1_CE1_PIN   17u
#define PSP_SPI_1_CE0_PIN   18u
#define PSP_SPI_1_MISO_PIN  19u
#define PSP_SPI_1_MOSI_PIN  20u
#define PSP_SPI_1_CLK_PIN   21u


/*------------------------------------------------------------------------------------------------
    Public PSP_Aux_SPI Types
 -------------------------------------------------------------------------------------------------*/


typedef enum Aux_SPI_CS_Type
{
    AUX_SPI_USE_CS_0 = 0b001u,
    AUX_SPI_USE_CS_1 = 0b010u,
    AUX_SPI_USE_CS_2 = 0b100u
} PSP_Aux_SPI_CS_t;

typedef union
{
    struct
    {
        vuint32_t Shift_length           : 6u;  // Specifies the number of bits to shift
        vuint32_t Shift_out_MS_bit_first : 1u;  // If 1 the data is shifted out starting with the MS bit, else LS bit
        vuint32_t Invert_SPI_CLK         : 1u;  // If 1 the 'idle' clock line state is high, else low
        vuint32_t Out_rising             : 1u;  // If 1 data is clocked out on the rising edge of the SPI clock, else falling
        vuint32_t Clear_FIFOs            : 1u;  // If 1 the receive and transmit FIFOs are held in reset (and thus flushed.)
        vuint32_t In_rising              : 1u;  // If 1 data is clocked in on the rising edge of the SPI clock, else falling
        vuint32_t Enable                 : 1u;  // Enables the SPI interface.
        vuint32_t DOUT_Hold_time         : 2u;  // Controls the extra DOUT hold time in system clock cycles.
        vuint32_t Variable_width         : 1u;  // If 1 the SPI takes the shift length and the data from the TX fifo
        vuint32_t Variable_CS            : 1u;  // If 1 the SPI takes the CS pattern and the data from TX fifo
        vuint32_t post_input_mode        : 1u;  // If set the SPI input works in post input mode.
        vuint32_t chip_selects           : 3u;  // The pattern output on the CS pins when active.
        vuint32_t Speed                  : 12u; // Sets the SPI clock speed. spi clk freq = system_clock_freq/2*(speed+1)
    };

    vuint32_t Control_Word;
    
} PSP_Aux_SPI_CNTL0_REG_t;

typedef union 
{
    struct 
    {
        vuint32_t Keep_input            : 1u;  // If 1 the receiver shift reg is NOT cleared. Thus new data is concatenated to old data.
        vuint32_t Shift_in_MS_bit_first : 1u;  // If 1 the data is shifted in starting with the MS bit, else LS bit
        vuint32_t                       : 4u;  // Reserved, write zero, read as don’t care
        vuint32_t Done_IRQ              : 1u;  // If 1 the interrupt line is high when the interface is idle
        vuint32_t TX_empty_IRQ          : 1u;  // If 1 the interrupt line is high when the transmit FIFO is empty
        vuint32_t CS_high_time          : 3u;  // Additional SPI clock cycles where the CS is high.
        vuint32_t                       : 14u; // Reserved, write zero, read as don’t care
    };

    vuint32_t Control_Word;
    
}PSP_Aux_SPI_CNTL1_REG_t;

typedef union 
{
    struct 
    {
        vuint32_t Bit_count     : 6u;  // The number of bits still to be processed.
        vuint32_t Busy          : 1u;  // Indicates the module is busy transferring data.
        vuint32_t RX_Empty      : 1u;  // If 1 the receiver FIFO is empty
        vuint32_t TX_Empty      : 1u;  // If 1 the transmit FIFO is empty
        vuint32_t TX_Full       : 1u;  // If 1 the transmit FIFO is full
        vuint32_t               : 7u;  // Reserved, write zero, read as don’t care
        vuint32_t RX_FIFO_level : 12u; // The number of data units in the receive data FIFO.
        vuint32_t TX_FIFO_level : 8u;  // The number of data units in the transmit data FIFO
    };

    vuint32_t Control_Word;
    
}PSP_Aux_SPI_STAT_REG_t;



/*------------------------------------------------------------------------------------------------
    Public PSP_Aux_SPI Function Declarations
 -------------------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------

Function Name:
    PSP_SPI1_Start

Function Description:
    Initialize SPI1 by setting GPIO pins 16 through 21 to alt mode 4, and setting the chip
    select pin to chip select 0.

Inputs:
    None

Returns:
    None

Error Handling:
    None

-------------------------------------------------------------------------------------------------*/
void PSP_SPI1_Start(void);


/*-----------------------------------------------------------------------------------------------

Function Name:
    PSP_SPI1_End

Function Description:
    Shuts down SPI1 by setting GPIO pins 16 through 21 to inputs.

Inputs:
    None

Returns:
    None

Error Handling:
    None

-------------------------------------------------------------------------------------------------*/
void PSP_SPI1_End(void);



/*-----------------------------------------------------------------------------------------------

Function Name:
    PSP_SPI1_Transfer_Byte

Function Description:
    Write and read a single byte via SPI 1.

Inputs:
    val: the byte to write via SPI 0.

Returns:
    uint8_t: the value read by SPI 0.

Error Handling:
    None

-------------------------------------------------------------------------------------------------*/
uint8_t PSP_SPI1_Transfer_Byte(uint8_t val);




#endif
