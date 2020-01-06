
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
