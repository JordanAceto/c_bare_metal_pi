/**
 * DESCRIPTION:
 *      PSP_GPIO interfaces with the GPIO registers. Functions are
 *      provided for setting the pin mode of GPIO pins, reading the
 *      level of GPIO pins, and setting the level of GPIO pins which
 *      are set to outputs.
 * 
 * NOTES:
 *      TODO: Add edge detection functionality.
 * 
 * REFERENCES:
 *      BCM2837-ARM-Peripherals.pdf page 89
 */

#ifndef PSP_GPIO_H_INCLUDED
#define PSP_GPIO_H_INCLUDED

#include "Fixed_Width_Ints.h"
#include "PSP_REGS.h"

/*-----------------------------------------------------------------------------------------------
    Public PSP_GPIO Defines
 -------------------------------------------------------------------------------------------------*/

// Register Addresses
#define GPIO_BASE_ADDRESS    (PSP_REGS_GPIO_BASE_ADDRESS)

#define PSP_GPIO_GPFSEL0_A   (GPIO_BASE_ADDRESS | 0x00000000u)  // GPIO Function Select 0 address
#define PSP_GPIO_GPFSEL1_A   (GPIO_BASE_ADDRESS | 0x00000004u)  // GPIO Function Select 1 address
#define PSP_GPIO_GPFSEL2_A   (GPIO_BASE_ADDRESS | 0x00000008u)  // GPIO Function Select 2 address
#define PSP_GPIO_GPFSEL3_A   (GPIO_BASE_ADDRESS | 0x0000000Cu)  // GPIO Function Select 3 address
#define PSP_GPIO_GPFSEL4_A   (GPIO_BASE_ADDRESS | 0x00000010u)  // GPIO Function Select 4 address
#define PSP_GPIO_GPFSEL5_A   (GPIO_BASE_ADDRESS | 0x00000014u)  // GPIO Function Select 5 address

#define PSP_GPIO_GPSET0_A    (GPIO_BASE_ADDRESS | 0x0000001Cu)  // GPIO Pin Output Set 0 address
#define PSP_GPIO_GPSET1_A    (GPIO_BASE_ADDRESS | 0x00000020u)  // GPIO Pin Output Set 1 address

#define PSP_GPIO_GPCLR0_A    (GPIO_BASE_ADDRESS | 0x00000028u)  // GPIO Pin Output Clear 0 address
#define PSP_GPIO_GPCLR1_A    (GPIO_BASE_ADDRESS | 0x0000002Cu)  // GPIO Pin Output Clear 1 address

#define PSP_GPIO_GPLEV0_A    (GPIO_BASE_ADDRESS | 0x00000034u)  // GPIO Pin Level 0 address
#define PSP_GPIO_GPLEV1_A    (GPIO_BASE_ADDRESS | 0x00000038u)  // GPIO Pin Level 1 address

#define PSP_GPIO_GPEDS0_A    (GPIO_BASE_ADDRESS | 0x00000040u)  // GPIO Pin Event Detect Status 0 address
#define PSP_GPIO_GPEDS1_A    (GPIO_BASE_ADDRESS | 0x00000044u)  // GPIO Pin Event Detect Status 1 address

#define PSP_GPIO_GPREN0_A    (GPIO_BASE_ADDRESS | 0x0000004Cu)  // GPIO Pin Rising Edge Detect Enable 0 address
#define PSP_GPIO_GPREN1_A    (GPIO_BASE_ADDRESS | 0x00000050u)  // GPIO Pin Rising Edge Detect Enable 1 address

#define PSP_GPIO_GPFEN0_A    (GPIO_BASE_ADDRESS | 0x00000058u)  // GPIO Pin Falling Edge Detect Enable 0 address
#define PSP_GPIO_GPFEN1_A    (GPIO_BASE_ADDRESS | 0x0000005Cu)  // GPIO Pin Falling Edge Detect Enable 1 address

#define PSP_GPIO_GPHEN0_A    (GPIO_BASE_ADDRESS | 0x00000064u)  // GPIO Pin High Detect Enable 0 address
#define PSP_GPIO_GPHEN1_A    (GPIO_BASE_ADDRESS | 0x00000068u)  // GPIO Pin High Detect Enable 1 address

#define PSP_GPIO_GPLEN0_A    (GPIO_BASE_ADDRESS | 0x00000070u)  // GPIO Pin Low Detect Enable 0 address
#define PSP_GPIO_GPLEN1_A    (GPIO_BASE_ADDRESS | 0x00000074u)  // GPIO Pin Low Detect Enable 1 address

#define PSP_GPIO_GPAREN0_A   (GPIO_BASE_ADDRESS | 0x0000007Cu)  // GPIO Pin Async. Rising Edge Detect 0 address
#define PSP_GPIO_GPAREN1_A   (GPIO_BASE_ADDRESS | 0x00000080u)  // GPIO Pin Async. Rising Edge Detect 1 address

#define PSP_GPIO_GPAFEN0_A   (GPIO_BASE_ADDRESS | 0x00000088u)  // GPIO Pin Async. Falling Edge Detect 0 address
#define PSP_GPIO_GPAFEN1_A   (GPIO_BASE_ADDRESS | 0x0000008Cu)  // GPIO Pin Async. Falling Edge Detect 1 address

#define PSP_GPIO_GPPUD_A     (GPIO_BASE_ADDRESS | 0x00000094u)  // GPIO Pin Pull-up/down Enable address

#define PSP_GPIO_GPPUDCLK0_A (GPIO_BASE_ADDRESS | 0x00000098u)  // GPIO Pin Pull-up/down Enable Clock 0 address
#define PSP_GPIO_GPPUDCLK1_A (GPIO_BASE_ADDRESS | 0x0000009Cu)  // GPIO Pin Pull-up/down Enable Clock 1 address

// Register Pointers
#define PSP_GPIO_GPFSEL0_R   (*((volatile uint32_t *)PSP_GPIO_GPFSEL0_A))   // GPIO Function Select 0 register
#define PSP_GPIO_GPFSEL1_R   (*((volatile uint32_t *)PSP_GPIO_GPFSEL1_A))   // GPIO Function Select 1 register
#define PSP_GPIO_GPFSEL2_R   (*((volatile uint32_t *)PSP_GPIO_GPFSEL2_A))   // GPIO Function Select 2 register
#define PSP_GPIO_GPFSEL3_R   (*((volatile uint32_t *)PSP_GPIO_GPFSEL3_A))   // GPIO Function Select 3 register
#define PSP_GPIO_GPFSEL4_R   (*((volatile uint32_t *)PSP_GPIO_GPFSEL4_A))   // GPIO Function Select 4 register
#define PSP_GPIO_GPFSEL5_R   (*((volatile uint32_t *)PSP_GPIO_GPFSEL5_A))   // GPIO Function Select 5 register

#define PSP_GPIO_GPSET0_R    (*((volatile uint32_t *)PSP_GPIO_GPSET0_A))    // GPIO Pin Output Set 0 register
#define PSP_GPIO_GPSET1_R    (*((volatile uint32_t *)PSP_GPIO_GPSET1_A))    // GPIO Pin Output Set 1 register

#define PSP_GPIO_GPCLR0_R    (*((volatile uint32_t *)PSP_GPIO_GPCLR0_A))    // GPIO Pin Output Clear 0 register
#define PSP_GPIO_GPCLR1_R    (*((volatile uint32_t *)PSP_GPIO_GPCLR1_A))    // GPIO Pin Output Clear 1 register

#define PSP_GPIO_GPLEV0_R    (*((volatile uint32_t *)PSP_GPIO_GPLEV0_A))    // GPIO Pin Level 0 register
#define PSP_GPIO_GPLEV1_R    (*((volatile uint32_t *)PSP_GPIO_GPLEV1_A))    // GPIO Pin Level 1 register

#define PSP_GPIO_GPEDS0_R    (*((volatile uint32_t *)PSP_GPIO_GPEDS0_A))    // GPIO Pin Event Detect Status 0 register
#define PSP_GPIO_GPEDS1_R    (*((volatile uint32_t *)PSP_GPIO_GPEDS1_A))    // GPIO Pin Event Detect Status 1 register

#define PSP_GPIO_GPREN0_R    (*((volatile uint32_t *)PSP_GPIO_GPREN0_A))    // GPIO Pin Rising Edge Detect Enable 0 register
#define PSP_GPIO_GPREN1_R    (*((volatile uint32_t *)PSP_GPIO_GPREN1_A))    // GPIO Pin Rising Edge Detect Enable 1 register

#define PSP_GPIO_GPFEN0_R    (*((volatile uint32_t *)PSP_GPIO_GPFEN0_A))    // GPIO Pin Falling Edge Detect Enable 0 register
#define PSP_GPIO_GPFEN1_R    (*((volatile uint32_t *)PSP_GPIO_GPFEN1_A))    // GPIO Pin Falling Edge Detect Enable 1 register

#define PSP_GPIO_GPHEN0_R    (*((volatile uint32_t *)PSP_GPIO_GPHEN0_A))    // GPIO Pin High Detect Enable 0 register
#define PSP_GPIO_GPHEN1_R    (*((volatile uint32_t *)PSP_GPIO_GPHEN1_A))    // GPIO Pin High Detect Enable 1 register

#define PSP_GPIO_GPLEN0_R    (*((volatile uint32_t *)PSP_GPIO_GPLEN0_A))    // GPIO Pin Low Detect Enable 0 register
#define PSP_GPIO_GPLEN1_R    (*((volatile uint32_t *)PSP_GPIO_GPLEN1_A))    // GPIO Pin Low Detect Enable 1 register

#define PSP_GPIO_GPAREN0_R   (*((volatile uint32_t *)PSP_GPIO_GPAREN0_A))   // GPIO Pin Async. Rising Edge Detect 0 register
#define PSP_GPIO_GPAREN1_R   (*((volatile uint32_t *)PSP_GPIO_GPAREN1_A))   // GPIO Pin Async. Rising Edge Detect 1 register

#define PSP_GPIO_GPAFEN0_R   (*((volatile uint32_t *)PSP_GPIO_GPAFEN0_A))   // GPIO Pin Async. Falling Edge Detect 0 register
#define PSP_GPIO_GPAFEN1_R   (*((volatile uint32_t *)PSP_GPIO_GPAFEN1_A))   // GPIO Pin Async. Falling Edge Detect 1 register

#define PSP_GPIO_GPPUD_R     (*((volatile uint32_t *)PSP_GPIO_GPPUD_A))     // GPIO Pin Pull-up/down Enable register

#define PSP_GPIO_GPPUDCLK0_R (*((volatile uint32_t *)PSP_GPIO_GPPUDCLK0_A)) // GPIO Pin Pull-up/down Enable Clock 0 register
#define PSP_GPIO_GPPUDCLK1_R (*((volatile uint32_t *)PSP_GPIO_GPPUDCLK1_A)) // GPIO Pin Pull-up/down Enable Clock 1 register

// Pin Mode Defines
#define PSP_GPIO_PINMODE_INPUT  0b000u
#define PSP_GPIO_PINMODE_OUTPUT 0b001u
#define PSP_GPIO_PINMODE_ALT0   0b100u
#define PSP_GPIO_PINMODE_ALT1   0b101u
#define PSP_GPIO_PINMODE_ALT2   0b110u
#define PSP_GPIO_PINMODE_ALT3   0b111u
#define PSP_GPIO_PINMODE_ALT4   0b011u
#define PSP_GPIO_PINMODE_ALT5   0b010u

#define PSP_GPIO_NUM_GPIO_PINS  54u
#define PSP_GPIO_MAX_PINMODE_VALUE 0b111u

#define PSP_GPIO_PIN_WRITE_HIGH 1u
#define PSP_GPIO_PIN_WRITE_LOW  0u



/*-----------------------------------------------------------------------------------------------
    Public PSP_GPIO Function Declarations
 -------------------------------------------------------------------------------------------------*/



/*-----------------------------------------------------------------------------------------------

Function Name:
    PSP_GPIO_Set_Pin_Mode

Function Description:
    Set a GPIO pin mode to Input, Output, or Alternate Function 0... 5

Inputs:
    pin_num: the GPIO pin to set to a given mode

    pin_mode: the GPIO pin mode, as below:

        0b000 = GPIO Pin is an input
        0b001 = GPIO Pin is an output
        0b100 = GPIO Pin takes alternate function 0
        0b101 = GPIO Pin takes alternate function 1
        0b110 = GPIO Pin takes alternate function 2
        0b111 = GPIO Pin takes alternate function 3
        0b011 = GPIO Pin takes alternate function 4
        0b010 = GPIO Pin takes alternate function 5

Returns:
    None

Error Handling:
    Returns without having any effect if the pin number or pin mode are out of range

-------------------------------------------------------------------------------------------------*/
void PSP_GPIO_Set_Pin_Mode(uint32_t pin_num, uint32_t pin_mode);



/*-----------------------------------------------------------------------------------------------

Function Name:
    PSP_GPIO_Write_Pin

Function Description:
    Write a GPIO pin high or low

Inputs:
    pin_num: the GPIO pin number to set high or low
    value: GPIO pin value, 0 sets the pin Low, Non-zero sets the pin High

Returns:
    None

Error Handling:
    Returns without having any effect if the pin number is out of range.

    Has no effect if the GPIO pin is not set to Output. However, if the pin is subsequently 
    defined as an output then the bit will be set according to the last set/clear operation.

-------------------------------------------------------------------------------------------------*/
void PSP_GPIO_Write_Pin(uint32_t pin_num, uint32_t value);



/*-----------------------------------------------------------------------------------------------

Function Name:
    PSP_GPIO_Read_Pin

Function Description:
    Read a GPIO pin and return 0 if the pin is low and 1 if the pin is high

Inputs:
    pin_num: GPIO pin number to read

Returns:
    The value of the GPIO pin (0 or 1)

Error Handling:
    Returns 0 if the pin number is out of range

-------------------------------------------------------------------------------------------------*/
uint32_t PSP_GPIO_Read_Pin(uint32_t pin_num);

#endif
