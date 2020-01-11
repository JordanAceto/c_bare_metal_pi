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
#include "PSP_PWM.h"
#include "PSP_SPI_0.h"
#include "PSP_I2C.h"
#include "PSP_Aux_Mini_UART.h"
#include "BSP_Rotary_Encoder.h"



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

    PSP_PWM_Clock_Init(PSP_PWM_Clock_Source_OSCILLATOR, PWM_DEFAULT_DIV);
    PSP_PWM_Channel_Start(PSP_PWM_Channel_1, PSP_PWM_MARK_SPACE_MODE, PSP_PWM_RANGE_10_BITS);
    PSP_PWM_Ch1_Set_GPIO12_To_PWM_Mode();

    uint32_t pwm_val = 0u;

    while(1)
    {
        // write a ramp wave via PWM
        PSP_PWM_Ch1_Write(pwm_val);
        pwm_val++;
        pwm_val %= PSP_PWM_RANGE_10_BITS;

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

    PSP_SPI0_Start();
    PSP_SPI0_Set_Clock_Divider(PSP_SPI0_Clock_Divider_1024);
    PSP_SPI0_Set_Chip_Select(PSP_SPI_0_Chip_Select_0);

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
        PSP_SPI0_Buffer_Transfer(spi_data_out, spi_data_in, SPI_BUFFER_SIZE);
        
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

    PSP_I2C_Start();
    // use default divider for ~100kHz clock speed
    PSP_I2C_Set_Slave_Address(SLAVE_ADDRESS);

    while (1)
    {
        PSP_I2C_Write_Byte(0xFEu);
        PSP_I2C_Write_Byte(0xEDu);
        PSP_I2C_Write_Byte(0xFAu);
        PSP_I2C_Write_Byte(0xCEu);

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

    PSP_AUX_Mini_Uart_Init(PSP_AUX_Mini_Uart_Baud_Rate_9600);

    while (1)
    {
        PSP_AUX_Mini_Uart_Send_String("quux");

        PSP_Time_Delay_Microseconds(DELAY_TIME_uSec); 
    }
}


/*
    Simple demo of the Rotary Encoder module.

    Sets up an encoder on pins 5 and 6, polls is as
    fast as possible, and writes the encoder count
    to the uart periodically.

    To verify: you'll need an encoder set up according to 
    this datasheet: https://www.bourns.com/docs/Product-Datasheets/PEC11R.pdf

    And also something to read the uart.
*/
void demo_Rotary_Encoder()
{
    // make a new encoder on pins 5 and 6
    BSP_Rotary_Encoder_t encoder =
    {
        5u, // pin A
        6u, // pin B
    };

    // initialize the encoder
    BSP_Rotary_Encoder_Initialize(&encoder);

    // we'll print to the uart once every period
    const uint64_t TIMER_PERIOD_uSec = 10000u;
    
    uint64_t time_stamp = 0u;

    PSP_AUX_Mini_Uart_Init(PSP_AUX_Mini_Uart_Baud_Rate_9600);

    while(1)
    {
        BSP_Poll_Rotary_Encoder(&encoder);

        // write the encoder count to the uart periodically
        if (PSP_Time_Get_Ticks() > time_stamp)
        {
            time_stamp = PSP_Time_Get_Ticks() + TIMER_PERIOD_uSec;
            
            // when printing, divide the count down a bit to slow it down, 
            // this makes it easier to control the Bourns encoder I used
            PSP_AUX_Mini_Uart_Send_Byte(encoder.count >> 2u);
        }
    }
}


#endif
