/**
 * DESCRIPTION:
 *      BSP_I2C provides an interface for using I2C.
 * 
 * NOTES:
 *      TODO: Writing data has been tested, but reading data has not. To do so,
 *      I'll need to set up some I2C device to talk back to the Pi and run some
 *      tests. Until then, consider reading data to be broken.
 * 
 *      TODO: Add functions to read/write more than one byte at a time.
 * 
 * REFERENCES:
 *      BCM2837-ARM-Peripherals.pdf page 28
 */

#ifndef BSP_I2C_H_INCLUDED
#define BSP_I2C_H_INCLUDED

#include "PSP_REGS.h"
#include "Fixed_Width_Ints.h"

/*-----------------------------------------------------------------------------------------------
    Public BSP_I2C Defines
 -------------------------------------------------------------------------------------------------*/

// I2C Register Addresses
#define BSP_I2C_BASE_A     (PSP_REGS_I2C_BASE_ADDRESS)
#define BSP_I2C_C_A        (BSP_I2C_BASE_A | 0x00000000u)           // control register address
#define BSP_I2C_S_A        (BSP_I2C_BASE_A | 0x00000004u)           // status register address
#define BSP_I2C_DLEN_A     (BSP_I2C_BASE_A | 0x00000008u)           // data length register address
#define BSP_I2C_SA_A       (BSP_I2C_BASE_A | 0x0000000Cu)           // slave address register address
#define BSP_I2C_FIFO_A     (BSP_I2C_BASE_A | 0x00000010u)           // data FIFO register address
#define BSP_I2C_DIV_A      (BSP_I2C_BASE_A | 0x00000014u)           // clock divider register address
#define BSP_I2C_DEL_A      (BSP_I2C_BASE_A | 0x00000018u)           // data delay register address
#define BSP_I2C_CLKT_A     (BSP_I2C_BASE_A | 0x0000001Cu)           // clock stretch timeout register address

// I2C Register Pointers
#define BSP_I2C_C_R        (*((volatile uint32_t *)BSP_I2C_C_A))    // control register
#define BSP_I2C_S_R        (*((volatile uint32_t *)BSP_I2C_S_A))    // status register
#define BSP_I2C_DLEN_R     (*((volatile uint32_t *)BSP_I2C_DLEN_A)) // data length register
#define BSP_I2C_SA_R       (*((volatile uint32_t *)BSP_I2C_SA_A))   // slave address register
#define BSP_I2C_FIFO_R     (*((volatile uint32_t *)BSP_I2C_FIFO_A)) // data FIFO register
#define BSP_I2C_DIV_R      (*((volatile uint32_t *)BSP_I2C_DIV_A))  // clock divider register
#define BSP_I2C_DEL_R      (*((volatile uint32_t *)BSP_I2C_DEL_A))  // data delay register
#define BSP_I2C_CLKT_R     (*((volatile uint32_t *)BSP_I2C_CLKT_A)) // clock stretch timeout register

// masks for I2C control register
#define I2C_C_I2CEN        0x00008000u // I2C Enable, 0 = disabled, 1 = enabled
#define I2C_C_INTR         0x00000400u // Interrupt on RX
#define I2C_C_INTT         0x00000200u // Interrupt on TX
#define I2C_C_INTD         0x00000100u // Interrupt on DONE
#define I2C_C_ST           0x00000080u // Start transfer, 1 = Start a new transfer
#define I2C_C_CLEAR_1      0x00000020u // Clear FIFO Clear
#define I2C_C_CLEAR_2      0x00000010u // Clear FIFO Clear
#define I2C_C_READ         0x00000001u // Read transfer

// masks for I2C status register
#define I2C_S_CLKT         0x00000200u // Clock stretch timeout
#define I2C_S_ERR          0x00000100u // 0 = No errors detected. 1 = Slave has not acknowledged its address.
#define I2C_S_RXF          0x00000080u // RXF FIFO full, 0 = FIFO is not full, 1 = FIFO is full
#define I2C_S_TXE          0x00000040u // TXE FIFO full, 0 = FIFO is not full, 1 = FIFO is full
#define I2C_S_RXD          0x00000020u // RXD FIFO contains data
#define I2C_S_TXD          0x00000010u // TXD FIFO can accept data
#define I2C_S_RXR          0x00000008u // RXR FIFO needs reading (full)
#define I2C_S_TXW          0x00000004u // TXW FIFO needs writing (full)
#define I2C_S_DONE         0x00000002u // Transfer DONE
#define I2C_S_TA           0x00000001u // Transfer Active

// pin constants for I2C
#define I2C_SDA_PIN        2u
#define I2C_SCL_PIN        3u



/*-----------------------------------------------------------------------------------------------
    Public BSP_I2C Function Declarations
 -------------------------------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------------------------

Function Name:
    BSP_I2C_Start

Function Description:
    Initialize I2C by setting GPIO pins 2 and 3 to alt mode 0.

Inputs:
    None

Returns:
    None

Error Handling:
    None

-------------------------------------------------------------------------------------------------*/
void BSP_I2C_Start(void);



/*-----------------------------------------------------------------------------------------------

Function Name:
    BSP_I2C_End

Function Description:
    Shut down I2C by setting GPIO pins 2 and 3 to inputs.

Inputs:
    None

Returns:
    None

Error Handling:
    None

-------------------------------------------------------------------------------------------------*/
void BSP_I2C_End(void);



/*-----------------------------------------------------------------------------------------------

Function Name:
    BSP_I2C_Set_Clock_Divider

Function Description:
    Sets the clock divider for I2C. This sets the clock speed.

Inputs:
    divider: This must be a power of 2. Only the lower 16 bits are used. Odd number are rounded down.

    SCL = core_clock / divider Where core_clock is nominally 150 MHz. If divider is set to 0, the divisor is 32768. 
    divider is always rounded down to an even number. The default value should result in a 100 kHz I2C clock
    frequency.

Returns:
    None

Error Handling:
    None

-------------------------------------------------------------------------------------------------*/
void BSP_I2C_Set_Clock_Divider(uint32_t divider);



/*-----------------------------------------------------------------------------------------------

Function Name:
    BSP_I2C_Set_Slave_Address

Function Description:
    Sets the I2C slave device address.

Inputs:
    r0: address, the 7 bit address of the device to communicate with. Uses only bits [0, 6].

Returns:
    None

Error Handling:
    None

-------------------------------------------------------------------------------------------------*/
void BSP_I2C_Set_Slave_Address(uint32_t address);



/*-----------------------------------------------------------------------------------------------

Function Name:
    BSP_I2C_Write_Byte

Function Description:
    Writes a single byte to the address in the I2C address register.

Inputs:
    val: the byte to write.

Returns:
    None (TODO: this should return some error/ok code in the future that specifies if the transmission was succesful)

Error Handling:
    None

-------------------------------------------------------------------------------------------------*/
void BSP_I2C_Write_Byte(uint8_t val);



#endif
