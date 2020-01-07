
#include "BSP_SPI_0.h"
#include "PSP_GPIO.h"

#include "PSP_REGS.h"


/*-----------------------------------------------------------------------------------------------
    Private BSP_SPI_0 Defines
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



/*-----------------------------------------------------------------------------------------------
    BSP_SPI_0 Function Definitions
 -------------------------------------------------------------------------------------------------*/


void BSP_SPI0_Start(void)
{
    // set all the SPI 0 pins to alt mode 0
    PSP_GPIO_Set_Pin_Mode(BSP_SPI_0_CE1_PIN,  PSP_GPIO_PINMODE_ALT0);
    PSP_GPIO_Set_Pin_Mode(BSP_SPI_0_CE0_PIN,  PSP_GPIO_PINMODE_ALT0);
    PSP_GPIO_Set_Pin_Mode(BSP_SPI_0_MISO_PIN, PSP_GPIO_PINMODE_ALT0);
    PSP_GPIO_Set_Pin_Mode(BSP_SPI_0_MOSI_PIN, PSP_GPIO_PINMODE_ALT0);
    PSP_GPIO_Set_Pin_Mode(BSP_SPI_0_CLK_PIN,  PSP_GPIO_PINMODE_ALT0);

    // zero out the control/status register
    BSP_SPI_0_CS_R = 0u;

    // clear the fifo
    BSP_SPI_0_CS_R = SPI_0_CS_CLEAR1 | SPI_0_CS_CLEAR2;

    // default to chip select 0
    BSP_SPI0_Set_Chip_Select(BSP_SPI_0_Chip_Select_0);
}



void BSP_SPI0_End(void)
{
    // set all the SPI 0 pins to inputs
    PSP_GPIO_Set_Pin_Mode(BSP_SPI_0_CE1_PIN,  PSP_GPIO_PINMODE_INPUT);
    PSP_GPIO_Set_Pin_Mode(BSP_SPI_0_CE0_PIN,  PSP_GPIO_PINMODE_INPUT);
    PSP_GPIO_Set_Pin_Mode(BSP_SPI_0_MISO_PIN, PSP_GPIO_PINMODE_INPUT);
    PSP_GPIO_Set_Pin_Mode(BSP_SPI_0_MOSI_PIN, PSP_GPIO_PINMODE_INPUT);
    PSP_GPIO_Set_Pin_Mode(BSP_SPI_0_CLK_PIN,  PSP_GPIO_PINMODE_INPUT);
}



void BSP_SPI0_Set_Clock_Divider(BSP_SPI_0_Clock_Divider_t divider)
{
    BSP_SPI_0_CLK_R = divider;
}



uint8_t BSP_SPI0_Transfer_Byte(uint8_t val)
{
    // clear the fifo
    BSP_SPI_0_CS_R |= SPI_0_CS_CLEAR1 | SPI_0_CS_CLEAR2;

    // set Transfer Active high to enable transfer
    BSP_SPI_0_CS_R |= SPI_0_CS_TA;

    while (!(BSP_SPI_0_CS_R & SPI_0_CS_TXD))
    {
        // wait for TX fifo to be ready to accept data
    }

    // write the value into the fifo
    BSP_SPI_0_FIFO_R = val;

    while (!(BSP_SPI_0_CS_R & SPI_0_CS_DONE))
    {
        // wait for the transfer to complete
    }

    // read the byte in the fifo
    uint8_t retval = BSP_SPI_0_FIFO_R;

    // set transfer active low to end the transfer
    BSP_SPI_0_CS_R &= ~(SPI_0_CS_TA);

    // return the return value
    return retval;
}



uint16_t BSP_SPI0_Transfer_16(uint16_t val)
{
    // clear the fifo
    BSP_SPI_0_CS_R |= SPI_0_CS_CLEAR1 | SPI_0_CS_CLEAR2;

    // set Transfer Active high to enable transfer
    BSP_SPI_0_CS_R |= SPI_0_CS_TA;

    while (!(BSP_SPI_0_CS_R & SPI_0_CS_TXD))
    {
        // wait for TX fifo to be ready to accept data
    }

    uint8_t high_byte = val >> 8u;
    uint8_t low_byte = val & 0xFFu;

    // write the value into the fifo, high byte first
    BSP_SPI_0_FIFO_R = high_byte;
    BSP_SPI_0_FIFO_R = low_byte;

    while (!(BSP_SPI_0_CS_R & SPI_0_CS_DONE))
    {
        // wait for the transfer to complete
    }

    // read the value in the fifo
    uint16_t retval = BSP_SPI_0_FIFO_R;

    // set transfer active low to end the transfer
    BSP_SPI_0_CS_R &= ~(SPI_0_CS_TA);

    // return the return value
    return retval;
}



void BSP_SPI0_Buffer_Transfer(uint8_t *p_Tx_buffer, uint8_t *p_Rx_buffer, uint32_t num_bytes)
{
    uint32_t num_bytes_written = 0u;
    uint32_t num_bytes_read = 0u;

    // clear the fifo
    BSP_SPI_0_CS_R |= SPI_0_CS_CLEAR1 | SPI_0_CS_CLEAR2;

    // set Transfer Active high to enable transfer
    BSP_SPI_0_CS_R |= SPI_0_CS_TA;

    // write and read the given number of bytes of data 
    while ((num_bytes_written < num_bytes) || (num_bytes_read < num_bytes))
    { 
        // the Tx fifo can accept data and there is data to write
        while((BSP_SPI_0_CS_R & SPI_0_CS_TXD) && (num_bytes_written < num_bytes))
        {
            BSP_SPI_0_FIFO_R = (uint8_t)p_Tx_buffer[num_bytes_written];
            num_bytes_written++;
        }

        // the Rx fifo has data in it and there is data to read
        while((BSP_SPI_0_CS_R & SPI_0_CS_RXD) && (num_bytes_read < num_bytes))
        {
            p_Rx_buffer[num_bytes_read] = (uint8_t)BSP_SPI_0_FIFO_R;
            num_bytes_read++;
        }
    }

    while (!(BSP_SPI_0_CS_R & SPI_0_CS_DONE))
    {
        // wait for the transfer to complete
    }

    // set transfer active low to end the transfer
    BSP_SPI_0_CS_R &= ~(SPI_0_CS_TA);
}



void BSP_SPI0_Set_Chip_Select(BSP_SPI_0_Chip_Select_t chip_select)
{
    BSP_SPI_0_CS_R = (BSP_SPI_0_CS_R & 0xFFFFFFFCu) | chip_select;
}
