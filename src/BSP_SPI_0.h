
#ifndef SPI_0_H_INCLUDED
#define SPI_0_H_INCLUDED

#include "PSP_REGS.h"
#include "Fixed_Width_Ints.h"


/*-----------------------------------------------------------------------------------------------
    Public BSP_SPI_0 Defines
 -------------------------------------------------------------------------------------------------*/

// SPI 0 Register Addresses
#define BSP_SPI_0_BASE_A      (PSP_REGS_SPI_0_BASE_ADDRESS) 
#define BSP_SPI_0_CS_A        (PSP_REGS_SPI_0_BASE_ADDRESS | 0x00000000u) // SPI Master Control and Status address
#define BSP_SPI_0_FIFO_A      (PSP_REGS_SPI_0_BASE_ADDRESS | 0x00000004u) // SPI Master TX and RX FIFOs address
#define BSP_SPI_0_CLK_A       (PSP_REGS_SPI_0_BASE_ADDRESS | 0x00000008u) // SPI Master Clock Divider address
#define BSP_SPI_0_DLEN_A      (PSP_REGS_SPI_0_BASE_ADDRESS | 0x0000000Cu) // SPI Master Data Length address
#define BSP_SPI_0_LTOH_A      (PSP_REGS_SPI_0_BASE_ADDRESS | 0x00000010u) // SPI LOSSI mode TOH address
#define BSP_SPI_0_DC_A        (PSP_REGS_SPI_0_BASE_ADDRESS | 0x00000014u) // SPI DMA DREQ Controls address

// SPI 0 Register Pointers
#define BSP_SPI_0_CS_R        (*((volatile uint32_t *)BSP_SPI_0_CS_A))    // SPI Master Control and Status register
#define BSP_SPI_0_FIFO_R      (*((volatile uint32_t *)BSP_SPI_0_FIFO_A))  // SPI Master TX and RX FIFOs register
#define BSP_SPI_0_CLK_R       (*((volatile uint32_t *)BSP_SPI_0_CLK_A))   // SPI Master Clock Divider register
#define BSP_SPI_0_DLEN_R      (*((volatile uint32_t *)BSP_SPI_0_DLEN_A))  // SPI Master Data Length register
#define BSP_SPI_0_LTOH_R      (*((volatile uint32_t *)BSP_SPI_0_LTOH_A))  // SPI LOSSI mode TOH register
#define BSP_SPI_0_DC_R        (*((volatile uint32_t *)BSP_SPI_0_DC_A))    // SPI DMA DREQ Controls register

// SPI 0 Control Register Masks
#define SPI_0_CS_LEN_LONG   0x02000000u  // Enable Long data word in Lossi mode if DMA_LEN is set
#define SPI_0_CS_DMA_LEN    0x01000000u  // Enable DMA mode in Lossi mode
#define SPI_0_CS_CSPOL2     0x00800000u  // Chip Select 2 Polarity
#define SPI_0_CS_CSPOL1     0x00400000u  // Chip Select 1 Polarity
#define SPI_0_CS_CSPOL0     0x00200000u  // Chip Select 0 Polarity
#define SPI_0_CS_RXF        0x00100000u  // RXF - RX FIFO Full
#define SPI_0_CS_RXR        0x00080000u  // RXR RX FIFO needs Reading ( full)
#define SPI_0_CS_TXD        0x00040000u  // TXD TX FIFO can accept Data
#define SPI_0_CS_RXD        0x00020000u  // RXD RX FIFO contains Data
#define SPI_0_CS_DONE       0x00010000u  // Done transfer Done
#define SPI_0_CS_TE_EN      0x00008000u  // Unused
#define SPI_0_CS_LMONO      0x00004000u  // Unused
#define SPI_0_CS_LEN        0x00002000u  // LEN LoSSI enable
#define SPI_0_CS_REN        0x00001000u  // REN Read Enable
#define SPI_0_CS_ADCS       0x00000800u  // ADCS Automatically Deassert Chip Select
#define SPI_0_CS_INTR       0x00000400u  // INTR Interrupt on RXR
#define SPI_0_CS_INTD       0x00000200u  // INTD Interrupt on Done
#define SPI_0_CS_DMAEN      0x00000100u  // DMAEN DMA Enable
#define SPI_0_CS_TA         0x00000080u  // Transfer Active
#define SPI_0_CS_CSPOL      0x00000040u  // Chip Select Polarity
#define SPI_0_CS_CLEAR1     0x00000020u  // CLEAR FIFO Clear 1
#define SPI_0_CS_CLEAR2     0x00000010u  // CLEAR FIFO Clear 2
#define SPI_0_CS_CPOL       0x00000008u  // Clock Polarity
#define SPI_0_CS_CPHA       0x00000004u  // Clock Phase
#define SPI_0_CS_CS1        0x00000002u  // Chip Select 1
#define SPI_0_CS_CS2        0x00000001u  // Chip Select 2

// SPI 0 GPIO pin numbers
#define BSP_SPI_0_CE1_PIN   7u
#define BSP_SPI_0_CE0_PIN   8u
#define BSP_SPI_0_MISO_PIN  9u
#define BSP_SPI_0_MOSI_PIN  10u
#define BSP_SPI_0_CLK_PIN   11u



/*------------------------------------------------------------------------------------------------
    Public BSP_SPI_0 Types
 -------------------------------------------------------------------------------------------------*/

typedef enum
{
    BSP_SPI0_Clock_Divider_2     =     2u, // sets SPI 0 clock to 125.0 MHz
    BSP_SPI0_Clock_Divider_4     =     4u, // sets SPI 0 clock to 62.5 MHz
    BSP_SPI0_Clock_Divider_8     =     8u, // sets SPI 0 clock to 31.2 MHz
    BSP_SPI0_Clock_Divider_16    =    16u, // sets SPI 0 clock to 15.6 MHz
    BSP_SPI0_Clock_Divider_32    =    32u, // sets SPI 0 clock to 7.8 MHz
    BSP_SPI0_Clock_Divider_64    =    64u, // sets SPI 0 clock to 3.9 MHz
    BSP_SPI0_Clock_Divider_128   =   128u, // sets SPI 0 clock to 1953 kHz
    BSP_SPI0_Clock_Divider_256   =   256u, // sets SPI 0 clock to 976 kHz
    BSP_SPI0_Clock_Divider_512   =   512u, // sets SPI 0 clock to 488 kHz
    BSP_SPI0_Clock_Divider_1024  =  1024u, // sets SPI 0 clock to 244 kHz
    BSP_SPI0_Clock_Divider_2048  =  2048u, // sets SPI 0 clock to 122 kHz
    BSP_SPI0_Clock_Divider_4096  =  4096u, // sets SPI 0 clock to 61 kHz
    BSP_SPI0_Clock_Divider_8192  =  8192u, // sets SPI 0 clock to 30.5 kHz
    BSP_SPI0_Clock_Divider_16384 = 16384u, // sets SPI 0 clock to 15.2 kHz
    BSP_SPI0_Clock_Divider_32768 = 32768u  // sets SPI 0 clock to 7629 Hz
} BSP_SPI_0_Clock_Divider_t;


typedef enum
{
    BSP_SPI_0_Chip_Select_0 = 0u,
    BSP_SPI_0_Chip_Select_1 = 1u
} BSP_SPI_0_Chip_Select_t;



/*-----------------------------------------------------------------------------------------------
    Public BSP_SPI_0 Function Declarations
 -------------------------------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------------------------

Function Name:
    BSP_SPI0_Start

Function Description:
    Initialize SPI0 by setting GPIO pins 7 through 11 to alt mode 0, zeroing the SPI0 status
    and control register, clearing the SPI0 Rx FIFO and Tx FIFO buffers, and setting the chip
    select pin to chip select 0.

Inputs:
    None

Returns:
    None

Error Handling:
    None

-------------------------------------------------------------------------------------------------*/
void BSP_SPI0_Start(void);



/*-----------------------------------------------------------------------------------------------

Function Name:
    BSP_SPI0_End

Function Description:
    Shuts down SPI0 by setting GPIO pins 7 through 11 to inputs.

Inputs:
    None

Returns:
    None

Error Handling:
    None

-------------------------------------------------------------------------------------------------*/
void BSP_SPI0_End(void);




/*-----------------------------------------------------------------------------------------------

Function Name:
    BSP_SPI0_Set_Clock_Divider

Function Description:
    Sets the clock divider for SPI0. This sets the speed for the SPI0 clock.

    If this function is never called, the divider will default to 65536.

Inputs:
    divider: The divider to use for the SPI 0 clock.
    
    The following are the the valid dividers and the resulting SPI 0 clock speeds:

       div    speed
        2  -  125.0 MHz
        4  -   62.5 MHz
        8  -   31.2 MHz
       16  -   15.6 MHz
       32  -    7.8 MHz
       64  -    3.9 MHz
      128  -   1953 kHz
      256  -    976 kHz
      512  -    488 kHz
     1024  -    244 kHz
     2048  -    122 kHz
     4096  -     61 kHz
     8192  -   30.5 kHz
    16384  -   15.2 kHz
    32768  -   7629 Hz

    (note that the fastest three speeds may not work, experimentation needed)

Returns:
    None

Error Handling:
    None

-------------------------------------------------------------------------------------------------*/
void BSP_SPI0_Set_Clock_Divider(BSP_SPI_0_Clock_Divider_t divider);



/*-----------------------------------------------------------------------------------------------

Function Name:
    BSP_SPI0_Transfer_Byte

Function Description:
    Write and read a single byte via SPI 0. Uses Polled transfer as described in section 10.6.1 of the datasheet.

Inputs:
    val: the byte to write via SPI 0.

Returns:
    uint8_t: the value read by SPI 0.

Error Handling:
    None

-------------------------------------------------------------------------------------------------*/
uint8_t BSP_SPI0_Transfer_Byte(uint8_t val);



/*-----------------------------------------------------------------------------------------------

Function Name:
    BSP_SPI0_Transfer_16

Function Description:
    Write and read a 16 bit word via SPI 0. Uses Polled transfer as described in section 10.6.1 of the datasheet.

Inputs:
    val: the 16 bit value to write via SPI 0.

Returns:
    uint16_t: the value read by SPI 0.

Error Handling:
    None

-------------------------------------------------------------------------------------------------*/
uint16_t BSP_SPI0_Transfer_16(uint16_t val);



/*-----------------------------------------------------------------------------------------------

Function Name:
    BSP_SPI0_Set_Chip_Select

Function Description:
    Choose which chip select line to assert when transferring data.

Inputs:
    chip_select: the chip select line to use, 0 or 1. Has no immediate effect, but the next
    time a data transfer occurs the new chip select line will be used.

Returns:
    None

Error Handling:
    None

-------------------------------------------------------------------------------------------------*/
void BSP_SPI0_Set_Chip_Select(BSP_SPI_0_Chip_Select_t chip_select);



#endif
