
#include "BSP_SPI_0.h"
#include "PSP_GPIO.h"



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
