
#include "PSP_SPI_0.h"
#include "PSP_GPIO.h"

#include "PSP_REGS.h"


/*-----------------------------------------------------------------------------------------------
    Private PSP_SPI_0 Defines
 -------------------------------------------------------------------------------------------------*/

// SPI 0 Register Addresses
#define PSP_SPI_0_BASE_A      (PSP_REGS_SPI_0_BASE_ADDRESS) 
#define PSP_SPI_0_CS_A        (PSP_SPI_0_BASE_A | 0x00000000u)   // SPI Master Control and Status address
#define PSP_SPI_0_FIFO_A      (PSP_SPI_0_BASE_A | 0x00000004u)   // SPI Master TX and RX FIFOs address
#define PSP_SPI_0_CLK_A       (PSP_SPI_0_BASE_A | 0x00000008u)   // SPI Master Clock Divider address
#define PSP_SPI_0_DLEN_A      (PSP_SPI_0_BASE_A | 0x0000000Cu)   // SPI Master Data Length address
#define PSP_SPI_0_LTOH_A      (PSP_SPI_0_BASE_A | 0x00000010u)   // SPI LOSSI mode TOH address
#define PSP_SPI_0_DC_A        (PSP_SPI_0_BASE_A | 0x00000014u)   // SPI DMA DREQ Controls address

// SPI 0 Register Pointers
#define PSP_SPI_0_CS_R        (*((vuint32_t *)PSP_SPI_0_CS_A))   // SPI Master Control and Status register
#define PSP_SPI_0_FIFO_R      (*((vuint32_t *)PSP_SPI_0_FIFO_A)) // SPI Master TX and RX FIFOs register
#define PSP_SPI_0_CLK_R       (*((vuint32_t *)PSP_SPI_0_CLK_A))  // SPI Master Clock Divider register
#define PSP_SPI_0_DLEN_R      (*((vuint32_t *)PSP_SPI_0_DLEN_A)) // SPI Master Data Length register
#define PSP_SPI_0_LTOH_R      (*((vuint32_t *)PSP_SPI_0_LTOH_A)) // SPI LOSSI mode TOH register
#define PSP_SPI_0_DC_R        (*((vuint32_t *)PSP_SPI_0_DC_A))   // SPI DMA DREQ Controls register

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
    Private PSP_SPI_0 Types
 -------------------------------------------------------------------------------------------------*/

/* SPI 0 Master Control and Status Register Type */
typedef union SPI_0_CS_Reg_Type
{
    struct
    {
        vuint32_t CHIP_SEL : 2u; // Chip Select, 00 = Chip select 0, 01 = Chip select 1
        vuint32_t CPHA     : 1u; // Clock Phase. 0 = First SCLK transition at middle of data bit, else beginning.
        vuint32_t CPOL     : 1u; // Clock Polarity. 0 = Rest state of clock = low, else high.
        vuint32_t CLR_FIFO : 2u; // Clear FIFOs. 00 = No action. x1 = Clear TX FIFO. 1x = Clear RX FIFO.
        vuint32_t CSPOL    : 1u; // Chip Select Polarity. 0 = Chip select lines are active low, else high.
        vuint32_t TA       : 1u; // Transfer Active. 0 = Transfer not active, 1 = Transfer active.
        vuint32_t DMAEN    : 1u; // DMAEN DMA Enable. 0 = No DMA requests will be issued, 1 = Enable DMA operation.
        vuint32_t INTD     : 1u; // INTD Interrupt on Done. 0 = Don t generate interrupt on transfer complete.
        vuint32_t INTR     : 1u; // INTR Interrupt on RXR. 0 = Don t generate interrupts on RX FIFO condition.
        vuint32_t ADCS     : 1u; // ADCS Automatically Deassert Chip Select
        vuint32_t REN      : 1u; // REN Read Enable. 0 = We intend to write to the SPI peripheral, 1 = read.
        vuint32_t LEN      : 1u; // LoSSI enable. 0 = The serial interface will behave as an SPI master, 1 = LoSSI master.
        vuint32_t LMONO    : 1u; // Unused
        vuint32_t TE_EN    : 1u; // Unused
        vuint32_t DONE     : 1u; // Transfer Done. 0 = Transfer is in progress (or not active TA = 0), 1 = Tr complete.
        vuint32_t RXD      : 1u; // RX FIFO contains Data. 0 = RX FIFO is empty, 1 = RX FIFO contains at least 1 byte.
        vuint32_t TXD      : 1u; // TX FIFO can accept Data. 0 = TX FIFO is full, 1 = TX FIFO has space for at least 1 byte.
        vuint32_t RXR      : 1u; // RX FIFO needs Reading. 0 = RX FIFO is less than full (or not active TA = 0), 1 = full.
        vuint32_t RXF      : 1u; // RX FIFO Full. 0 = RXFIFO is not full, 1 = RX FIFO is full.
        vuint32_t CSPOL0   : 1u; // Chip Select 0 Polarity. 0 = Chip select is active low, 1 = Chip select is active high.
        vuint32_t CSPOL1   : 1u; // Chip Select 1 Polarity. 0 = Chip select is active low, 1 = Chip select is active high.
        vuint32_t CSPOL2   : 1u; // Chip Select 2 Polarity. Unused.
        vuint32_t DMA_LEN  : 1u; // Enable DMA mode in Lossi mode.
        vuint32_t LEN_LONG : 1u; // Enable Long data word in Lossi mode if DMA_LEN is set. 0 = byte, 1 = 32 bit word.
        vuint32_t RESERVED : 6u; // Reserved - Write as 0, read as don't care
    };

    vuint32_t Data; // Read/write the entire 32 bit word at once.
    
} SPI_0_CS_Regsiter_t;



/* SPI 0 Master TX and RX FIFOs Register Type */
typedef union SPI_0_FIFO_Reg_Type
{
    vuint32_t DATA; // SPI Master TX and RX FIFOs

} SPI_0_FIFO_Regsiter_t;



/* SPI 0 Master Clock Divider Register Type */
typedef union SPI_0_CLK_Reg_Type
{
    struct
    {
        vuint32_t CDIV     : 16u; // Clock Divider. SCLK = Core Clock / CDIV. If CDIV is set to 0, the divisor is 65536
        vuint32_t RESERVED : 16u; // Reserved - Write as 0, read as don't care
    };
    
} SPI_0_CLK_Regsiter_t;



/* SPI 0 Master Data Length Register Type */
typedef union SPI_0_DLEN_Reg_Type
{
    struct
    {
        vuint32_t LEN      : 16u; // Data Length. The number of bytes to transfer.
        vuint32_t RESERVED : 16u; // Reserved - Write as 0, read as don't care
    };
    
} SPI_0_DLEN_Regsiter_t;



/* SPI 0 LOSSI mode TOH Register Type */
typedef union SPI_0_LTOH_Reg_Type
{
    struct
    {
        vuint32_t TOH      : 16u; // This sets the Output Hold delay in APB clocks. A value of 0 causes a 1 clock delay.
        vuint32_t RESERVED : 16u; // Reserved - Write as 0, read as don't care
    };
    
} SPI_0_LTOH_Regsiter_t;



/* SPI 0 DMA DREQ Controls Register Type */
typedef union SPI_0_DC_Reg_Type
{
    struct
    {
        vuint32_t TDREQ  : 8u; // DMA Write Request Threshold.
        vuint32_t TPANIC : 8u; // DMA Write Panic Threshold.
        vuint32_t RDREQ  : 8u; // DMA Read Request Threshold.
        vuint32_t RPANIC : 8u; // DMA Read Panic Threshold.
    };
    
    vuint32_t DATA; // Read/write the entire 32 bit word at once.
    
} SPI_0_DC_Regsiter_t;



typedef struct SPI_0_Type
{
    SPI_0_CS_Regsiter_t   CS;   // SPI Master Control and Status Register
    SPI_0_FIFO_Regsiter_t FIFO; // SPI Master TX and RX FIFO Register
    SPI_0_CLK_Regsiter_t  CLK;  // SPI Master Clock Divider Register
    SPI_0_DLEN_Regsiter_t DLEN; // SPI Master Data Length Register
    SPI_0_LTOH_Regsiter_t LTOH; // SPI LOSSI mode TOH Register
    SPI_0_DC_Regsiter_t   DC;   // SPI DMA DREQ Controls Register
} SPI_0_t;



/*-----------------------------------------------------------------------------------------------
    Private PSP_SPI_0 Variables
 -------------------------------------------------------------------------------------------------*/

volatile SPI_0_t * SPI_0 = (volatile SPI_0_t *)PSP_SPI_0_BASE_A;



/*-----------------------------------------------------------------------------------------------
    PSP_SPI_0 Function Definitions
 -------------------------------------------------------------------------------------------------*/


void PSP_SPI0_Start(void)
{
    // set all the SPI 0 pins to alt mode 0
    PSP_GPIO_Set_Pin_Mode(PSP_SPI_0_CE1_PIN,  PSP_GPIO_PINMODE_ALT0);
    PSP_GPIO_Set_Pin_Mode(PSP_SPI_0_CE0_PIN,  PSP_GPIO_PINMODE_ALT0);
    PSP_GPIO_Set_Pin_Mode(PSP_SPI_0_MISO_PIN, PSP_GPIO_PINMODE_ALT0);
    PSP_GPIO_Set_Pin_Mode(PSP_SPI_0_MOSI_PIN, PSP_GPIO_PINMODE_ALT0);
    PSP_GPIO_Set_Pin_Mode(PSP_SPI_0_CLK_PIN,  PSP_GPIO_PINMODE_ALT0);

    // zero out the control/status register
    SPI_0->CS.Data = 0u;

    // clear the fifos
    SPI_0->CS.CLR_FIFO = 0b11u;

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
    SPI_0->CLK.CDIV = divider;
}



void PSP_SPI0_Begin_Transfer(void)
{
    // clear the fifos
    SPI_0->CS.CLR_FIFO = 0b11u;

    // set Transfer Active high to enable transfer
    SPI_0->CS.TA = 1u;
}



void PSP_SPI0_End_Transfer(void)
{
    while (SPI_0->CS.TA == 1u && SPI_0->CS.DONE == 0u)
    {
        // wait for the transfer to complete
    }

    // set transfer active low to end the transfer
    SPI_0->CS.TA = 0u;
}



void PSP_SPI0_Send_Byte(uint8_t val)
{
    while (SPI_0->CS.TXD == 0u)
    {
        // wait for TX fifo to be ready to accept data
    }

    // write the value into the fifo
    SPI_0->FIFO.DATA = val;
}



void PSP_SPI0_Send_16(uint16_t val)
{
    while (SPI_0->CS.TXD == 0u)
    {
        // wait for TX fifo to be ready to accept data
    }

    const uint8_t high_byte = val >> 8u;
    const uint8_t low_byte = val & 0xFFu;

    // write the value into the fifo, high byte first
    SPI_0->FIFO.DATA = high_byte;
    SPI_0->FIFO.DATA = low_byte;
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
        while(SPI_0->CS.TXD && (num_bytes_written < num_bytes))
        {
            SPI_0->FIFO.DATA = (uint8_t)p_Tx_buffer[num_bytes_written];
            num_bytes_written++;
        }

        // the Rx fifo has data in it and there is data to read
        while(SPI_0->CS.RXD && (num_bytes_read < num_bytes))
        {
            p_Rx_buffer[num_bytes_read] = (uint8_t)SPI_0->FIFO.DATA;
            num_bytes_read++;
        }
    }

    PSP_SPI0_End_Transfer();
}



void PSP_SPI0_Set_Chip_Select(PSP_SPI_0_Chip_Select_t chip_select)
{
    SPI_0->CS.CHIP_SEL = chip_select;
}
