/*
--|----------------------------------------------------------------------------|
--| FILE DESCRIPTION:
--|   PSP_SPI_0.c provides the implementation for SPI 0 communication.
--|   
--|----------------------------------------------------------------------------|
--| REFERENCES:
--|   BCM2837-ARM-Peripherals.pdf page 148
--|
--|----------------------------------------------------------------------------|
*/

/*
--|----------------------------------------------------------------------------|
--| INCLUDE FILES
--|----------------------------------------------------------------------------|
*/

#include "PSP_GPIO.h"
#include "PSP_REGS.h"
#include "PSP_SPI_0.h"

/*
--|----------------------------------------------------------------------------|
--| PRIVATE DEFINES
--|----------------------------------------------------------------------------|
*/

/*
--| NAME: SPI_0
--| DESCRIPTION: pointer to the SPI 0 register structure
--| TYPE: SPI_0_t *
*/
#define SPI_0 ((volatile SPI_0_t *)PSP_REGS_SPI_0_BASE_ADDRESS)

/*
--|----------------------------------------------------------------------------|
--| PRIVATE TYPES
--|----------------------------------------------------------------------------|
*/

/*
--| NAME: SPI_0_t
--| DESCRIPTION: structure for the SPI 0 registers
*/
typedef struct SPI_0_Type
{
    vuint32_t CS;   // SPI Master Control and Status
    vuint32_t FIFO; // SPI Master TX and RX FIFOs
    vuint32_t CLK;  // SPI Master Clock Divider
    vuint32_t DLEN; // SPI Master Data Length
    vuint32_t LTOH; // SPI LOSSI mode TOH
    vuint32_t DC;   // SPI DMA DREQ Controls
} SPI_0_t;

/*
--| NAME: SPI_0_CS_Flags_enum
--| DESCRIPTION: SPI 0 Master Control and Status register flags
*/
typedef enum SPI_0_CS_Flags_Enumeration
{
    SPI_0_CS_LEN_LONG_FLAG = (1u << 25u),  // Enable Long data word in Lossi mode if DMA_LEN is set [rw]
    SPI_0_CS_DMA_LEN_FLAG  = (1u << 24u),  // Enable DMA mode in Lossi mode [rw]
    SPI_0_CS_CSPOL2_FLAG   = (1u << 23u),  // Chip Select 2 Polarity [rw]
    SPI_0_CS_CSPOL1_FLAG   = (1u << 22u),  // Chip Select 1 Polarity [rw]
    SPI_0_CS_CSPOL0_FLAG   = (1u << 21u),  // Chip Select 0 Polarity [rw]
    SPI_0_CS_RXF_FLAG      = (1u << 20u),  // RXF - RX FIFO Full [r0]
    SPI_0_CS_RXR_FLAG      = (1u << 19u),  // RXR RX FIFO needs Reading (full) [r0]
    SPI_0_CS_TXD_FLAG      = (1u << 18u),  // TXD TX FIFO can accept Data [r0]
    SPI_0_CS_RXD_FLAG      = (1u << 17u),  // RXD RX FIFO contains Data [r0]
    SPI_0_CS_DONE_FLAG     = (1u << 16u),  // Done transfer Done [r0]
    SPI_0_CS_TE_EN_FLAG    = (1u << 15u),  // Unused 
    SPI_0_CS_LMONO_FLAG    = (1u << 14u),  // Unused
    SPI_0_CS_LEN_FLAG      = (1u << 13u),  // LEN LoSSI enable [rw]
    SPI_0_CS_REN_FLAG      = (1u << 12u),  // REN Read Enable [rw]
    SPI_0_CS_ADCS_FLAG     = (1u << 11u),  // ADCS Automatically Deassert Chip Select [rw]
    SPI_0_CS_INTR_FLAG     = (1u << 10u),  // INTR Interrupt on RXR [rw]
    SPI_0_CS_INTD_FLAG     = (1u << 9u),   // INTD Interrupt on Done [rw]
    SPI_0_CS_DMAEN_FLAG    = (1u << 8u),   // DMAEN DMA Enable [rw]
    SPI_0_CS_TA_FLAG       = (1u << 7u),   // Transfer Active [rw]
    SPI_0_CS_CSPOL_FLAG    = (1u << 6u),   // Chip Select Polarity [rw]
    SPI_0_CS_CPOL_FLAG     = (1u << 3u),   // Clock Polarity [rw]
    SPI_0_CS_CPHA_FLAG     = (1u << 2u),   // Clock Phase [rw]
} SPI_0_CS_Flags_enum;

/*
--| NAME: SPI_0_CS_CLEAR_Masks_enum
--| DESCRIPTION: SPI 0 CS CLEAR masks
*/
typedef enum SPI_0_CS_CLEAR_Masks_Enumeration
{
    SPI_0_CS_CLEAR_NO_ACTION      = 0b00u, // No action
    SPI_0_CS_CLEAR_TX_FIFO        = 0b01u, // Clear TX FIFO. One shot operation
    SPI_0_CS_CLEAR_RX_FIFO        = 0b10u, // Clear RX FIFO. One shot operation
    SPI_0_CS_CLEAR_RX_AND_TX_FIFO = 0b11u, // Clear both. One shot operation
    SPI_0_CS_CLEAR_SHIFT_AMT      = 4u,    // position of CLEAR in SPI0 CS
} SPI_0_CS_CLEAR_Masks_enum;

/*
--| NAME: SPI_0_CS_CHIP_SEL_Masks_enum
--| DESCRIPTION: SPI 0 CS CHIP_SEL masks
*/
typedef enum SPI_0_CS_CHIP_SEL_Masks_Enumeration
{
    SPI_0_CS_CHIP_SEL_0         = 0b00u, // Chip select 0
    SPI_0_CS_CHIP_SEL_1         = 0b01u, // Chip select 1
    SPI_0_CS_CHIP_SEL_2         = 0b10u, // Chip select 2
    SPI_0_CS_CHIP_SEL_RESERVED  = 0b11u, // reserved
    SPI_0_CS_CHIP_SEL_SHIFT_AMT = 0u,    // position of CHIP_SEL in SPI0 CS
} SPI_0_CS_CHIP_SEL_Masks_enum;

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

void PSP_SPI0_Start(void)
{
    // set all the SPI 0 pins to alt mode 0
    PSP_GPIO_Set_Pin_Mode(PSP_SPI_0_CE1_PIN,  PSP_GPIO_PINMODE_ALT0);
    PSP_GPIO_Set_Pin_Mode(PSP_SPI_0_CE0_PIN,  PSP_GPIO_PINMODE_ALT0);
    PSP_GPIO_Set_Pin_Mode(PSP_SPI_0_MISO_PIN, PSP_GPIO_PINMODE_ALT0);
    PSP_GPIO_Set_Pin_Mode(PSP_SPI_0_MOSI_PIN, PSP_GPIO_PINMODE_ALT0);
    PSP_GPIO_Set_Pin_Mode(PSP_SPI_0_CLK_PIN,  PSP_GPIO_PINMODE_ALT0);

    // zero out the control/status register
    SPI_0->CS = 0u;

    // clear the fifos
    SPI_0->CS |= SPI_0_CS_CLEAR_RX_AND_TX_FIFO << SPI_0_CS_CLEAR_SHIFT_AMT;

    // default to chip select 0
    PSP_SPI0_Set_Chip_Select(PSP_SPI_0_Chip_Select_0);
}

void PSP_SPI0_End(void)
{
    // set all the SPI 0 pins to inputs
    PSP_GPIO_Set_Pin_Mode(PSP_SPI_0_CE1_PIN,  PSP_GPIO_PINMODE_INPUT);
    PSP_GPIO_Set_Pin_Mode(PSP_SPI_0_CE0_PIN,  PSP_GPIO_PINMODE_INPUT);
    PSP_GPIO_Set_Pin_Mode(PSP_SPI_0_MISO_PIN, PSP_GPIO_PINMODE_INPUT);
    PSP_GPIO_Set_Pin_Mode(PSP_SPI_0_MOSI_PIN, PSP_GPIO_PINMODE_INPUT);
    PSP_GPIO_Set_Pin_Mode(PSP_SPI_0_CLK_PIN,  PSP_GPIO_PINMODE_INPUT);
}

void PSP_SPI0_Set_Clock_Divider(PSP_SPI_0_Clock_Divider_t divider)
{
    SPI_0->CLK = divider;
}

void PSP_SPI0_Begin_Transfer(void)
{
    // clear the fifos
    SPI_0->CS |= SPI_0_CS_CLEAR_RX_AND_TX_FIFO << SPI_0_CS_CLEAR_SHIFT_AMT;

    // set Transfer Active high to enable transfer
    SPI_0->CS |= SPI_0_CS_TA_FLAG;
}

void PSP_SPI0_End_Transfer(void)
{
    while ((SPI_0->CS & SPI_0_CS_TA_FLAG) && !(SPI_0->CS & SPI_0_CS_DONE_FLAG))
    {
        // wait for the transfer to complete
    }

    // set transfer active low to end the transfer
    SPI_0->CS &= ~(SPI_0_CS_TA_FLAG);
}

void PSP_SPI0_Send_Byte(uint8_t val)
{
    while (!(SPI_0->CS & SPI_0_CS_TXD_FLAG))
    {
        // wait for TX fifo to be ready to accept data
    }

    // write the value into the fifo
    SPI_0->FIFO = val;
}

void PSP_SPI0_Send_16(uint16_t val)
{
    while (!(SPI_0->CS & SPI_0_CS_TXD_FLAG))
    {
        // wait for TX fifo to be ready to accept data
    }

    const uint8_t high_byte = val >> 8u;
    const uint8_t low_byte = val & 0xFFu;

    // write the value into the fifo, high byte first
    SPI_0->FIFO = high_byte;
    SPI_0->FIFO = low_byte;
}

uint8_t PSP_SPI0_Transfer_Byte(uint8_t val)
{
    PSP_SPI0_Begin_Transfer();
    PSP_SPI0_Send_Byte(val);
    PSP_SPI0_End_Transfer();
    return 0u;
}

uint16_t PSP_SPI0_Transfer_16(uint16_t val)
{
    PSP_SPI0_Begin_Transfer();
    PSP_SPI0_Send_16(val);
    PSP_SPI0_End_Transfer();
    return 0u;
}

void PSP_SPI0_Buffer_Transfer(uint8_t *p_Tx_buffer, uint8_t *p_Rx_buffer, uint32_t num_bytes)
{
    uint32_t num_bytes_written = 0u;
    uint32_t num_bytes_read = 0u;

    PSP_SPI0_Begin_Transfer();

    // write and read the given number of bytes of data 
    while ((num_bytes_written < num_bytes) || (num_bytes_read < num_bytes))
    { 
        // the Tx fifo can accept data and there is data to write
        while((SPI_0->CS & SPI_0_CS_TXD_FLAG) && (num_bytes_written < num_bytes))
        {
            SPI_0->FIFO = (uint8_t)p_Tx_buffer[num_bytes_written];
            num_bytes_written++;
        }

        // the Rx fifo has data in it and there is data to read
        while((SPI_0->CS & SPI_0_CS_RXD_FLAG) && (num_bytes_read < num_bytes))
        {
            p_Rx_buffer[num_bytes_read] = (uint8_t)SPI_0->FIFO;
            num_bytes_read++;
        }
    }

    PSP_SPI0_End_Transfer();
}

void PSP_SPI0_Set_Chip_Select(PSP_SPI_0_Chip_Select_t chip_select)
{
    SPI_0->CS &= ~(0b11u);
    SPI_0->CS |= chip_select;
}

/*
--|----------------------------------------------------------------------------|
--| PRIVATE HELPER FUNCTION DEFINITIONS
--|----------------------------------------------------------------------------|
*/

/* None */
