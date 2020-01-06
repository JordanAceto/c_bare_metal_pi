
#include "BSP_I2C.h"

#include "PSP_GPIO.h"

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
