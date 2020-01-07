
#include "BSP_I2C.h"
#include "PSP_GPIO.h"
#include "PSP_REGS.h"

/*-----------------------------------------------------------------------------------------------
    Private BSP_I2C Defines
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



/*-----------------------------------------------------------------------------------------------
    BSP_I2C Function Definitions
 -------------------------------------------------------------------------------------------------*/


void BSP_I2C_Start(void)
{
    PSP_GPIO_Set_Pin_Mode(I2C_SDA_PIN, PSP_GPIO_PINMODE_ALT0);
    PSP_GPIO_Set_Pin_Mode(I2C_SCL_PIN, PSP_GPIO_PINMODE_ALT0);
}



 void BSP_I2C_End(void)
 {
    PSP_GPIO_Set_Pin_Mode(I2C_SDA_PIN, PSP_GPIO_PINMODE_INPUT);
    PSP_GPIO_Set_Pin_Mode(I2C_SCL_PIN, PSP_GPIO_PINMODE_INPUT);
 }



void BSP_I2C_Set_Clock_Divider(uint32_t divider)
{
    BSP_I2C_DIV_R = divider;
}



void BSP_I2C_Set_Slave_Address(uint32_t address)
{
    BSP_I2C_SA_R = address;
}



void BSP_I2C_Write_Byte(uint8_t val)
{
    // clear the fifo
    BSP_I2C_C_R |= I2C_C_CLEAR_1;

    // clear the clock stretch timeout, no acknowledge error, and transfer done status flags 
    // note that these flags are cleared by writing a 1
    BSP_I2C_S_R |= I2C_S_CLKT | I2C_S_ERR | I2C_S_DONE;
    
    // set data length to 1 byte
    BSP_I2C_DLEN_R = 1u;

    // write the value to the fifo
    BSP_I2C_FIFO_R = val;
    
    // enable device and start transfer
    BSP_I2C_C_R |= I2C_C_I2CEN | I2C_C_ST;

    while (!(BSP_I2C_S_R & I2C_S_DONE))
    {
        // wait for transfer to complete
    }

    // set the done flag inorder to clear it and end the transfer
    BSP_I2C_S_R |= I2C_S_DONE;
}
