/*
--|----------------------------------------------------------------------------|
--| FILE DESCRIPTION:
--|   PSP_I2C.c provides the implementation for I2C.
--|  
--|----------------------------------------------------------------------------|
--| REFERENCES:
--|   BCM2837-ARM-Peripherals.pdf page 28
--|
--|----------------------------------------------------------------------------|
*/

/*
--|----------------------------------------------------------------------------|
--| INCLUDE FILES
--|----------------------------------------------------------------------------|
*/

#include "PSP_GPIO.h"
#include "PSP_I2C.h"
#include "PSP_REGS.h"

/*
--|----------------------------------------------------------------------------|
--| PRIVATE DEFINES
--|----------------------------------------------------------------------------|
*/

/*
--| NAME: I2C
--| DESCRIPTION: pointer to the I2C structure
--| TYPE: I2C_t *
*/
#define I2C ((volatile I2C_t *)PSP_REGS_I2C_BASE_ADDRESS)

/*
--|----------------------------------------------------------------------------|
--| PRIVATE TYPES
--|----------------------------------------------------------------------------|
*/

/*
--| NAME: I2C_t
--| DESCRIPTION: structure for I2C registers
*/
typedef struct I2C_Type
{
    vuint32_t C;    // Control
    vuint32_t S;    // Status
    vuint32_t DLEN; // Data Length
    vuint32_t A;    // Slave Address
    vuint32_t FIFO; // Data FIFO
    vuint32_t DIV;  // Clock Divider
    vuint32_t DEL;  // Data Delay
    vuint32_t CLKT; // Clock Stretch Timeout
} I2C_t;

/*
--| NAME: I2C_C_Flags_enum
--| DESCRIPTION: I2C Control register flags
*/
typedef enum I2C_C_Flags_Enumeration
{
    I2C_C_I2CEN_FLAG = (1u << 15u), // I2C Enable [rw]
    I2C_C_INTR_FLAG  = (1u << 10u), // INTR Interrupt on RX [rw]
    I2C_C_INTT_FLAG  = (1u << 9u),  // INTT Interrupt on TX [rw]
    I2C_C_INTD_FLAG  = (1u << 8u),  // INTD Interrupt on DONE [rw]
    I2C_C_ST_FLAG    = (1u << 7u),  // ST Start Transfer [rw]
    I2C_C_READ_FLAG  = (1u << 0u),  // READ Read Transfer [rw]
} I2C_C_Flags_enum;

/*
--| NAME: I2C_C_CLEAR_Masks_enum
--| DESCRIPTION: I2C Control register CLEAR masks [2 bits, rw]
*/
typedef enum I2C_C_CLEAR_Masks_Enumeration
{
    I2C_C_CLEAR_NO_ACTION = 0b00u, // No action
    I2C_C_CLEAR_FIFO      = 0b01u, // Clear FIFO. One shot operation
    I2C_C_CLEAR_FIFO_2    = 0b10u, // Clear FIFO. (2 bits are used to maintain compatibility to previous version)
    I2C_C_CLEAR_SHIFT_AMT = 4u,    // position of CLEAR in SPI0 CS
} I2C_C_CLEAR_Masks_enum;

/*
--| NAME: I2C_S_Flags_enum
--| DESCRIPTION: I2C Status register flags
*/
typedef enum I2C_S_Flags_Enumeration
{
    I2C_S_CLKT_FLAG = (1u << 9u), // CLKT Clock Stretch Timeout [rw]
    I2C_S_ERR_FLAG  = (1u << 8u), // ERR ACK Error [rw]
    I2C_S_RXF_FLAG  = (1u << 7u), // RXF - FIFO Full [r0]
    I2C_S_TXE_FLAG  = (1u << 6u), // TXE - FIFO Empty [r0]
    I2C_S_RXD_FLAG  = (1u << 5u), // RXD - FIFO contains Data [r0]
    I2C_S_TXD_FLAG  = (1u << 4u), // TXD - FIFO can accept Data [r0]
    I2C_S_RXR_FLAG  = (1u << 3u), // RXR - FIFO needs Reading (full) [r0]
    I2C_S_TXW_FLAG  = (1u << 2u), // TXW - FIFO needs Writing (full) [r0]
    I2C_S_DONE_FLAG = (1u << 1u), // DONE Transfer Done [rw]
    I2C_S_TA_FLAG   = (1u << 0u), // TA Transfer Active [r0]
} I2C_S_Flags_enum;

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

void PSP_I2C_Start(void)
{
    PSP_GPIO_Set_Pin_Mode(I2C_SDA_PIN, PSP_GPIO_PINMODE_ALT0);
    PSP_GPIO_Set_Pin_Mode(I2C_SCL_PIN, PSP_GPIO_PINMODE_ALT0);
}

void PSP_I2C_End(void)
{
    PSP_GPIO_Set_Pin_Mode(I2C_SDA_PIN, PSP_GPIO_PINMODE_INPUT);
    PSP_GPIO_Set_Pin_Mode(I2C_SCL_PIN, PSP_GPIO_PINMODE_INPUT);
}

void PSP_I2C_Set_Clock_Divider(uint32_t divider)
{
    I2C->DIV = divider;
}

void PSP_I2C_Set_Slave_Address(uint32_t address)
{
    I2C->A = address;
}

void PSP_I2C_Write_Byte(uint8_t val)
{
    // clear the fifo
    I2C->C |= I2C_C_CLEAR_FIFO << I2C_C_CLEAR_SHIFT_AMT;

    // clear the clock stretch timeout, no acknowledge error, and transfer done status flags 
    // note that these flags are cleared by writing a 1
    I2C->S |= I2C_S_CLKT_FLAG | I2C_S_ERR_FLAG | I2C_S_DONE_FLAG;
    
    // set data length to 1 byte
    I2C->DLEN = 1u;

    // write the value to the fifo
    I2C->FIFO = val;
    
    // enable device and start transfer
    I2C->C |= I2C_C_I2CEN_FLAG | I2C_C_ST_FLAG;

    while (!(I2C->S & I2C_S_DONE_FLAG))
    {
        // wait for transfer to complete
    }

    // set the done flag inorder to clear it and end the transfer
    I2C->S |= I2C_S_DONE_FLAG;
}

/*
--|----------------------------------------------------------------------------|
--| PRIVATE HELPER FUNCTION DEFINITIONS
--|----------------------------------------------------------------------------|
*/

/* None */
