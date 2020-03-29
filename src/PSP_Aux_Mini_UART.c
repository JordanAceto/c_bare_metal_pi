/*
--|----------------------------------------------------------------------------|
--| FILE DESCRIPTION:
--|   PSP_Aux_Mini_UART.c provides the implementation for the aux mini UART.
--|  
--|----------------------------------------------------------------------------|
--| REFERENCES:
--|   BCM2837-ARM-Peripherals.pdf page 8
--|
--|----------------------------------------------------------------------------|
*/

/*
--|----------------------------------------------------------------------------|
--| INCLUDE FILES
--|----------------------------------------------------------------------------|
*/

#include "PSP_Aux_Mini_UART.h"
#include "PSP_GPIO.h"
#include "PSP_REGS.h"

/*
--|----------------------------------------------------------------------------|
--| PRIVATE DEFINES
--|----------------------------------------------------------------------------|
*/

/*
--| NAME: AUX
--| DESCRIPTION: pointer to the Aux Peripherals register structure
--| TYPE: Aux_Peripherals_t *
*/
#define AUX ((volatile Aux_Peripherals_t *)PSP_REGS_AUX_BASE_ADDRESS)

/*
--|----------------------------------------------------------------------------|
--| PRIVATE TYPES
--|----------------------------------------------------------------------------|
*/

/*
--| NAME: Aux_Peripherals_t
--| DESCRIPTION: structure for the Aux Peripherals registers
*/
typedef struct Aux_Peripherals_Type
{
    vuint32_t IRQ;            // Auxiliary Interrupt status
    vuint32_t ENABLES;        // Auxiliary enables
    vuint32_t RESERVED_0[14]; // 
    vuint32_t MU_IO;          // Mini Uart I/O Data
    vuint32_t MU_IER;         // Mini Uart Interrupt Enable
    vuint32_t MU_IIR;         // Mini Uart Interrupt Identify
    vuint32_t MU_LCR;         // Mini Uart Line Control
    vuint32_t MU_MCR;         // Mini Uart Modem Control
    vuint32_t MU_LSR;         // Mini Uart Line Status
    vuint32_t MU_MSR;         // Mini Uart Modem Status
    vuint32_t MU_SCRATCH;     // Mini Uart Scratch
    vuint32_t MU_CNTL;        // Mini Uart Extra Control
    vuint32_t MU_STAT;        // Mini Uart Extra Status        
    vuint32_t MU_BAUD;        // Mini Uart Baudrate
    vuint32_t RESERVED_1[5];  // double check me please
    vuint32_t SPI0_CNTL0;     // SPI 1 Control register 0
    vuint32_t SPI0_CNTL1;     // SPI 1 Control register 1
    vuint32_t SPI0_STAT;      // SPI 1 Status
    vuint32_t RESERVED_2[1];  // double check me please
    vuint32_t SPI0_IO;        // SPI 1 Data
    vuint32_t SPI0_PEEK;      // SPI 1 Peek
    vuint32_t RESERVED_3[10]; // double check me please
    vuint32_t SPI1_CNTL0;     // SPI 2 Control register 0
    vuint32_t SPI1_CNTL1;     // SPI 2 Control register 1   
    vuint32_t SPI1_STAT;      // SPI 2 Status
    vuint32_t RESERVED_4[1];  // double check me please
    vuint32_t SPI1_IO;        // SPI 2 Data
    vuint32_t SPI1_PEEK;      // SPI 2 Peek
} Aux_Peripherals_t;

/*
--| NAME: Aux_Peripherals_IRQ_Flags_enum
--| DESCRIPTION: Aux Peripherals IRQ register flags
*/
typedef enum Aux_Peripherals_IRQ_Flags_Enumeration
{
    Aux_Peripherals_IRQ_SPI_2_FLAG = (1u << 2u),  // If set the SPI2 module has an interrupt pending [r]
    Aux_Peripherals_IRQ_SPI_1_FLAG = (1u << 1u),  // If set the SPI1 module has an interrupt pending [r]
    Aux_Peripherals_IRQ_MUART_FLAG = (1u << 0u),  // If set the mini UART has an interrupt pending [r]
} Aux_Peripherals_IRQ_Flags_enum;

/*
--| NAME: Aux_Peripherals_ENABLES_Flags_enum
--| DESCRIPTION: Aux Peripherals ENABLES register flags
*/
typedef enum Aux_Peripherals_ENABLES_Flags_Enumeration
{
    Aux_Peripherals_ENABLES_SPI_2_FLAG = (1u << 2u),  // SPI2 enable [rw]
    Aux_Peripherals_ENABLES_SPI_1_FLAG = (1u << 1u),  // SPI1 enable [rw]
    Aux_Peripherals_ENABLES_MUART_FLAG = (1u << 0u),  // mini UART enable [rw]
} Aux_Peripherals_ENABLES_Flags_enum;

/*
--| NAME: Aux_Peripherals_MU_IER_MS8_Masks_enum
--| DESCRIPTION: Aux Peripherals Mini UART register IER MS 8 bits masks [8 bits, rw]
*/
typedef enum Aux_Peripherals_MU_IER_MS8_Masks_Enumeration
{
    Aux_Peripherals_MU_IER_MS8_MASK      = 0xFFu, // Access to the MS 8 bits of the 16-bit baudrate reg
    Aux_Peripherals_MU_IER_MS8_SHIFT_AMT = 8u,    // position of MS8 in AUX MU IER 
} Aux_Peripherals_MU_IER_MS8_Masks_enum;

/*
--| NAME: Aux_Peripherals_MU_IER_Flags_enum
--| DESCRIPTION: Aux Peripherals IER register flags
*/
typedef enum Aux_Peripherals_MU_IER_Flags_Enumeration
{
    Aux_Peripherals_MU_IER_ENAB_TRANS_INTS_FLAG = (1u << 1u),  // enable transmit interrupts [rw]
    Aux_Peripherals_MU_IER_ENAB_REC_INTS_FLAG   = (1u << 0u),  // enable recieve interrupts [rw]
} Aux_Peripherals_MU_IER_Flags_enum;

/*
--| NAME: Aux_Peripherals_MU_IIR_READ_Masks_enum
--| DESCRIPTION: Aux Peripherals Mini UART IIR register READ masks [2 bits, rw]
*/
typedef enum Aux_Peripherals_MU_IIR_READ_Masks_Enumeration
{
    Aux_Peripherals_MU_IIR_READ_NO_INTS    = 0b00u, // No interrupts
    Aux_Peripherals_MU_IIR_READ_TRANS_EMPT = 0b01u, // Transmit holding register empty
    Aux_Peripherals_MU_IIR_READ_REC_VALID  = 0b10u, // Receiver holds valid byte
    Aux_Peripherals_MU_IIR_READ_SHIFT_AMT  = 1u,    // position of MS8 in AUX MU IER 
} Aux_Peripherals_MU_IIR_READ_Masks_enum;

/*
--| NAME: Aux_Peripherals_MU_IIR_Flags_enum
--| DESCRIPTION: Aux Peripherals IIR register flags
*/
typedef enum Aux_Peripherals_MU_IIR_Flags_Enumeration
{
    Aux_Peripherals_MU_IIR_INT_PENDING_FLAG = (1u << 0u),  // This bit is clear whenever an interrupt is pending [r]
} Aux_Peripherals_MU_IIR_Flags_enum;

/*
--| NAME: Aux_Peripherals_MU_LCR_Flags_enum
--| DESCRIPTION: Aux Peripherals LCR register flags
*/
typedef enum Aux_Peripherals_MU_LCR_Flags_Enumeration
{
    Aux_Peripherals_MU_LCR_DLAB_FLAG  = (1u << 7u), // give access the the Baudrate register [rw]
    Aux_Peripherals_MU_LCR_BREAK_FLAG = (1u << 6u), // UART1_TX line is pulled low continuously [rw]
} Aux_Peripherals_MU_LCR_Flags_enum;

/*
--| NAME: Aux_Peripherals_MU_LCR_DAT_SIZE_Masks_enum
--| DESCRIPTION: Aux Peripherals Mini UART LCR register data size masks [2 bits, rw]
*/
typedef enum Aux_Peripherals_MU_LCR_DAT_SIZE_Masks_Enumeration
{
    Aux_Peripherals_MU_LCR_DAT_SIZE_7_BIT     = 0b00u, // the UART works in 7-bit mode
    Aux_Peripherals_MU_LCR_DAT_SIZE_8_BIT     = 0b11u, // the UART works in 8-bit mode
    Aux_Peripherals_MU_LCR_DAT_SIZE_SHIFT_AMT = 0u,    // position of data size in AUX MU LCR 
} Aux_Peripherals_MU_LCR_DAT_SIZE_Masks_enum;

/*
--| NAME: Aux_Peripherals_MU_MCR_Flags_enum
--| DESCRIPTION: Aux Peripherals MCR register flags
*/
typedef enum Aux_Peripherals_MU_MCR_Flags_Enumeration
{
    Aux_Peripherals_MU_MCR_RTX_FLAG = (1u << 1u),  // If clear the UART1_RTS line is high [rw]
} Aux_Peripherals_MU_MCR_Flags_enum;

/*
--| NAME: Aux_Peripherals_MU_LSR_Flags_enum
--| DESCRIPTION: Aux Peripherals LSR register flags
*/
typedef enum Aux_Peripherals_MU_LSR_Flags_Enumeration
{
    Aux_Peripherals_MU_LSR_TRANS_IDLE_FLAG  = (1u << 6u),  // transmitter idle [r]
    Aux_Peripherals_MU_LSR_TRANS_EMPTY_FLAG = (1u << 5u),  // transmitter empty [r]
    Aux_Peripherals_MU_LSR_REC_OVERRUN_FLAG = (1u << 1u),  // reciever overrun [r/c]
    Aux_Peripherals_MU_LSR_DATA_READY_FLAG  = (1u << 0u),  // data ready [r]
} Aux_Peripherals_MU_LSR_Flags_enum;

/*
--| NAME: Aux_Peripherals_MU_MSR_Flags_enum
--| DESCRIPTION: Aux Peripherals MSR register flags
*/
typedef enum Aux_Peripherals_MU_MSR_Flags_Enumeration
{
    Aux_Peripherals_MU_MSR_CTS_FLAG = (1u << 5u),  // CTS status [r]
} Aux_Peripherals_MU_MSR_Flags_enum;

/*
--| NAME: Aux_Peripherals_MU_CNTL_Flags_enum
--| DESCRIPTION: Aux Peripherals CNTL register flags
*/
typedef enum Aux_Peripherals_MU_CNTL_Flags_Enumeration
{
    Aux_Peripherals_MU_CNTL_CTS_FLAG        = (1u << 7u), // CTS assert level [rw]
    Aux_Peripherals_MU_CNTL_RTS_FLAG        = (1u << 6u), // RTS assert level [rw]
    Aux_Peripherals_MU_CNTL_AUTO_FLOW_FLAG  = (1u << 2u), // enable recieve auto flow [rw]
    Aux_Peripherals_MU_CNTL_TRANS_ENAB_FLAG = (1u << 1u), // transmitter enable [rw]
    Aux_Peripherals_MU_CNTL_REC_ENAB_FLAG   = (1u << 0u), // reciever enable [rw]
} Aux_Peripherals_MU_CNTL_Flags_enum;

/*
--| NAME: Aux_Peripherals_MU_CNTL_RTS_AUTO_Masks_enum
--| DESCRIPTION: Aux Peripherals Mini UART CNTL register RTS AUTO flow level masks [2 bits, rw]
*/
typedef enum Aux_Peripherals_MU_CNTL_RTS_AUTO_Masks_Enumeration
{
    Aux_Peripherals_MU_CNTL_RTS_AUTO_3_EMPTY   = 0b00u, // De-assert RTS when rec FIFO has 3 spaces left
    Aux_Peripherals_MU_CNTL_RTS_AUTO_2_EMPTY   = 0b01u, // De-assert RTS when rec FIFO has 2 spaces left
    Aux_Peripherals_MU_CNTL_RTS_AUTO_1_EMPTY   = 0b10u, // De-assert RTS when rec FIFO has 1 spaces left
    Aux_Peripherals_MU_CNTL_RTS_AUTO_4_EMPTY   = 0b11u, // De-assert RTS when rec FIFO has 4 spaces left
    Aux_Peripherals_MU_CNTL_RTS_AUTO_SHIFT_AMT = 4u,    // position of RTS AUTO in AUX MU CNTL 
} Aux_Peripherals_MU_CNTL_RTS_AUTO_Masks_enum;

/*
--| NAME: Aux_Peripherals_MU_STAT_TX_FIFO_LEV_Masks_enum
--| DESCRIPTION: Aux Peripherals Mini UART STAT register TX FIFO fill level masks [4 bits, r]
*/
typedef enum Aux_Peripherals_MU_STAT_TX_FIFO_LEV_Masks_Enumeration
{
    Aux_Peripherals_MU_STAT_TX_FIFO_LEV_MASK      = 0xFu, // how many symbols stored in the transmit FIFO
    Aux_Peripherals_MU_STAT_TX_FIFO_LEV_SHIFT_AMT = 24u,  // position of TX FIFO fill lev in AUX MU STAT 
} Aux_Peripherals_MU_STAT_TX_FIFO_LEV_Masks_enum;

/*
--| NAME: Aux_Peripherals_MU_STAT_RX_FIFO_LEV_Masks_enum
--| DESCRIPTION: Aux Peripherals Mini UART STAT register RX FIFO fill level masks [4 bits, r]
*/
typedef enum Aux_Peripherals_MU_STAT_RX_FIFO_LEV_Masks_Enumeration
{
    Aux_Peripherals_MU_STAT_RX_FIFO_LEV_MASK      = 0xFu, // how many symbols stored in the transmit FIFO
    Aux_Peripherals_MU_STAT_RX_FIFO_LEV_SHIFT_AMT = 16u,  // position of RX FIFO fill lev in AUX MU STAT 
} Aux_Peripherals_MU_STAT_RX_FIFO_LEV_Masks_enum;

/*
--| NAME: Aux_Peripherals_MU_STAT_Flags_enum
--| DESCRIPTION: Aux Peripherals STAT register flags
*/
typedef enum Aux_Peripherals_MU_STAT_Flags_Enumeration
{
    Aux_Peripherals_MU_STAT_TRANS_DONE_FLAG    = (1u << 9u), // transmitter done [r]
    Aux_Peripherals_MU_STAT_TX_FIFO_EMPTY_FLAG = (1u << 8u), // transmit FIFO is empty [r]
    Aux_Peripherals_MU_STAT_CTS_LINE_FLAG      = (1u << 7u), // CTS line [r]
    Aux_Peripherals_MU_STAT_RTS_STATUS_FLAG    = (1u << 6u), // RTS status [r]
    Aux_Peripherals_MU_STAT_TX_FIFO_FULL_FLAG  = (1u << 5u), // transmit FIFO is full [r]
    Aux_Peripherals_MU_STAT_REC_OVERRUN_FLAG   = (1u << 4u), // reciever overrun [r]
    Aux_Peripherals_MU_STAT_TRANS_IDLE_FLAG    = (1u << 3u), // transmitter is idle [r]
    Aux_Peripherals_MU_STAT_REC_IDLE_FLAG      = (1u << 2u), // reciever is idle [r]
    Aux_Peripherals_MU_STAT_SPACE_AVAIL_FLAG   = (1u << 1u), // space available [r]
    Aux_Peripherals_MU_STAT_SYMBOL_AVAIL_FLAG  = (1u << 0u), // symbol available [r]
} Aux_Peripherals_MU_STAT_Flags_enum;

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

void PSP_AUX_Mini_Uart_Init(PSP_AUX_Mini_Uart_Baud_Rate_t baud_rate_enum)
{
    // set pins 14 and 15 to alt pin mode 5 for mini uart
    PSP_GPIO_Set_Pin_Mode(PSP_AUX_MINI_UART_TX_PIN, PSP_GPIO_PINMODE_ALT5);
    PSP_GPIO_Set_Pin_Mode(PSP_AUX_MINI_UART_RX_PIN, PSP_GPIO_PINMODE_ALT5);

    // enable the mini uart
    AUX->ENABLES |= Aux_Peripherals_ENABLES_MUART_FLAG;

    // disable mini uart interrupts
    AUX->MU_IER = 0u;

    // zero out the control register
    AUX->MU_CNTL = 0u;

    // enable 8 bit mode
    AUX->MU_LCR = Aux_Peripherals_MU_LCR_DAT_SIZE_8_BIT;

    // set RTS line to be high
    AUX->MU_MCR = 0u;

    // set the baud rate
    PSP_AUX_Mini_Uart_Set_Baud_Rate(baud_rate_enum);

    // enable transmitter and reciever
    AUX->MU_CNTL |= Aux_Peripherals_MU_CNTL_TRANS_ENAB_FLAG | Aux_Peripherals_MU_CNTL_REC_ENAB_FLAG;
}

void PSP_AUX_Mini_Uart_Set_Baud_Rate(PSP_AUX_Mini_Uart_Baud_Rate_t baud_rate_enum)
{
    AUX->MU_BAUD = baud_rate_enum;
}

void PSP_AUX_Mini_Uart_Send_Byte(uint8_t value)
{
    while (!(AUX->MU_LSR & Aux_Peripherals_MU_LSR_TRANS_EMPTY_FLAG))
    {
        // wait until the transmitter can accept data
    }

    // write the value to the I/O register
    AUX->MU_IO = value;
}

void PSP_AUX_Mini_Uart_Send_String(char * c_string)
{
    
    for (int i = 0u; c_string[i] != '\0'; i++)
    {
        PSP_AUX_Mini_Uart_Send_Byte(c_string[i]);
    }
}

/*
--|----------------------------------------------------------------------------|
--| PRIVATE HELPER FUNCTION DEFINITIONS
--|----------------------------------------------------------------------------|
*/

/* None */
