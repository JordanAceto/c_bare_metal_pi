/**
 * DESCRIPTION:
 *      Hardware Demos provides a suite of simple demo functions for each of the peripherals
 *      as I get them working. The idea is to demo the hardware as simply as possible, to 
 *      verify that it is working.
 * 
 * NOTES:
 *      Each demo enters an infinite loop and does not return. This is so that I can run a demo 
 *      and probe various ports on the board while it is running to validate the functionality
 *      of the given peripheral module.
 * 
 *      TODO: As new peripheral interfaces are implemented, new demo functions will be added here.
 * 
 * REFERENCES:
 *      Pinouts: https://www.raspberrypi.org/documentation/usage/gpio/
 */

#ifndef HARDWARE_DEMOS_H_INCLUDED
#define HARDWARE_DEMOS_H_INCLUDED

#include "PSP_GPIO.h"
#include "PSP_Time.h"
#include "BSP_PWM.h"
#include "BSP_SPI_0.h"
#include "BSP_I2C.h"
#include "BSP_Aux_Mini_UART.h"



/**
 * Simple demo of using GPIO pins as inputs and outputs.
 * 
 * Reads a switch and blinks a LED if the switch is HIGH.
 * 
 * To verify: attach a switch to pin 21 and a LED to pin 17.
 */ 
void demo_GPIO()
{
    const uint32_t LED_PIN  = 17u;
    const uint32_t SWITCH_PIN = 21u;
    const uint32_t DELAY_TIME_uSec = 10000u;

    PSP_GPIO_Set_Pin_Mode(LED_PIN, PSP_GPIO_PINMODE_OUTPUT);
    PSP_GPIO_Set_Pin_Mode(SWITCH_PIN, PSP_GPIO_PINMODE_INPUT);

    while(1)
    {
        if (PSP_GPIO_Read_Pin(SWITCH_PIN))
        {
            PSP_GPIO_Write_Pin(LED_PIN, PSP_GPIO_PIN_WRITE_HIGH);
            PSP_Time_Delay_Microseconds(DELAY_TIME_uSec);
            
            PSP_GPIO_Write_Pin(LED_PIN, PSP_GPIO_PIN_WRITE_LOW);
            PSP_Time_Delay_Microseconds(DELAY_TIME_uSec);
        }
    }
}



/**
 * Simple demo of hardware PWM.
 * 
 * Writes a ramp wave via PWM.
 * 
 * To verify: attach a LED to pin 12.
 */ 
void demo_PWM()
{
    const uint32_t DELAY_TIME_uSec = 1000u;

    BSP_PWM_Clock_Init(BSP_PWM_Clock_Source_OSCILLATOR, PWM_DEFAULT_DIV);
    BSP_PWM_Channel_Start(BSP_PWM_Channel_1, BSP_PWM_MARK_SPACE_MODE, BSP_PWM_RANGE_10_BITS);
    BSP_PWM_Ch1_Set_GPIO12_To_PWM_Mode();

    uint32_t pwm_val = 0u;

    while(1)
    {
        // write a ramp wave via PWM
        BSP_PWM_Ch1_Write(pwm_val);
        pwm_val++;
        pwm_val %= BSP_PWM_RANGE_10_BITS;

        PSP_Time_Delay_Microseconds(DELAY_TIME_uSec);
    }
}



/**
 * Simple demo of SPI 0.
 * 
 * Writes a secret message in a buffer of bytes.
 * 
 * Read is ignored, and probably doesn't work yet.
 * 
 * To verify, you'll need a logic analyzer/scope/some device you can talk to.
 * 
 * In a pinch, put LEDs on pins 7 through 11.
 */ 
void demo_SPI_0()
{
    const uint32_t DELAY_TIME_uSec = 1000u;

    BSP_SPI0_Start();
    BSP_SPI0_Set_Clock_Divider(BSP_SPI0_Clock_Divider_1024);
    BSP_SPI0_Set_Chip_Select(BSP_SPI_0_Chip_Select_0);

    const uint32_t SPI_BUFFER_SIZE = 5u;

    uint8_t spi_data_out[SPI_BUFFER_SIZE];
    uint8_t spi_data_in[SPI_BUFFER_SIZE];

    spi_data_out[0] = 0xDAu;
    spi_data_out[1] = 0xDBu;
    spi_data_out[2] = 0x0Du;
    spi_data_out[3] = 0xBEu;
    spi_data_out[4] = 0xEFu;

    while(1)
    {
        BSP_SPI0_Buffer_Transfer(spi_data_out, spi_data_in, SPI_BUFFER_SIZE);
        
        PSP_Time_Delay_Microseconds(DELAY_TIME_uSec);
    }
}



/**
 * Simple demo of I2C bus.
 * 
 * Writes a few bytes via I2C.
 * 
 * To verify: you need a device to talk to, and then a logic analyzer or scope or some way of seeing
 * what you wrote. I used an LCD at address 0x27 that was lying around and a scope to decode the bytes
 * sent. 
 * 
 * If you don't have a I2C device handy with a known address, you're out of luck.
 */ 
void demo_I2C()
{
    const uint32_t DELAY_TIME_uSec = 1000u;

    const uint32_t SLAVE_ADDRESS = 0x27u;

    BSP_I2C_Start();
    // use default divider for ~100kHz clock speed
    BSP_I2C_Set_Slave_Address(SLAVE_ADDRESS);

    while (1)
    {
        BSP_I2C_Write_Byte(0xFEu);
        BSP_I2C_Write_Byte(0xEDu);
        BSP_I2C_Write_Byte(0xFAu);
        BSP_I2C_Write_Byte(0xCEu);

        PSP_Time_Delay_Microseconds(DELAY_TIME_uSec);    
    }
}


/**
 * Simple demo of Auxiliary Mini Uart.
 * 
 * Writes a little string via mini uart Tx.
 * 
 * To verify: you'll need a logic analyzer/scope/some device you can talk to.
 * 
 * In a pinch, put a LED on pin 14.
 */ 
void demo_Mini_Uart()
{
    const uint32_t DELAY_TIME_uSec = 10000u;

    BSP_AUX_Mini_Uart_Init(BSP_AUX_Mini_Uart_Baud_Rate_9600);

    while (1)
    {
        BSP_AUX_Mini_Uart_Send_String("quux");

        PSP_Time_Delay_Microseconds(DELAY_TIME_uSec); 
    }
}

#endif
