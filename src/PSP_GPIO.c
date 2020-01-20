
#include "PSP_GPIO.h"
#include "PSP_REGS.h"

/*-----------------------------------------------------------------------------------------------
    Private PSP_GPIO Defines
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
#define PSP_GPIO_GPFSEL0_R   (*((vuint32_t *)PSP_GPIO_GPFSEL0_A))   // GPIO Function Select 0 register
#define PSP_GPIO_GPFSEL1_R   (*((vuint32_t *)PSP_GPIO_GPFSEL1_A))   // GPIO Function Select 1 register
#define PSP_GPIO_GPFSEL2_R   (*((vuint32_t *)PSP_GPIO_GPFSEL2_A))   // GPIO Function Select 2 register
#define PSP_GPIO_GPFSEL3_R   (*((vuint32_t *)PSP_GPIO_GPFSEL3_A))   // GPIO Function Select 3 register
#define PSP_GPIO_GPFSEL4_R   (*((vuint32_t *)PSP_GPIO_GPFSEL4_A))   // GPIO Function Select 4 register
#define PSP_GPIO_GPFSEL5_R   (*((vuint32_t *)PSP_GPIO_GPFSEL5_A))   // GPIO Function Select 5 register

#define PSP_GPIO_GPSET0_R    (*((vuint32_t *)PSP_GPIO_GPSET0_A))    // GPIO Pin Output Set 0 register
#define PSP_GPIO_GPSET1_R    (*((vuint32_t *)PSP_GPIO_GPSET1_A))    // GPIO Pin Output Set 1 register

#define PSP_GPIO_GPCLR0_R    (*((vuint32_t *)PSP_GPIO_GPCLR0_A))    // GPIO Pin Output Clear 0 register
#define PSP_GPIO_GPCLR1_R    (*((vuint32_t *)PSP_GPIO_GPCLR1_A))    // GPIO Pin Output Clear 1 register

#define PSP_GPIO_GPLEV0_R    (*((vuint32_t *)PSP_GPIO_GPLEV0_A))    // GPIO Pin Level 0 register
#define PSP_GPIO_GPLEV1_R    (*((vuint32_t *)PSP_GPIO_GPLEV1_A))    // GPIO Pin Level 1 register

#define PSP_GPIO_GPEDS0_R    (*((vuint32_t *)PSP_GPIO_GPEDS0_A))    // GPIO Pin Event Detect Status 0 register
#define PSP_GPIO_GPEDS1_R    (*((vuint32_t *)PSP_GPIO_GPEDS1_A))    // GPIO Pin Event Detect Status 1 register

#define PSP_GPIO_GPREN0_R    (*((vuint32_t *)PSP_GPIO_GPREN0_A))    // GPIO Pin Rising Edge Detect Enable 0 register
#define PSP_GPIO_GPREN1_R    (*((vuint32_t *)PSP_GPIO_GPREN1_A))    // GPIO Pin Rising Edge Detect Enable 1 register

#define PSP_GPIO_GPFEN0_R    (*((vuint32_t *)PSP_GPIO_GPFEN0_A))    // GPIO Pin Falling Edge Detect Enable 0 register
#define PSP_GPIO_GPFEN1_R    (*((vuint32_t *)PSP_GPIO_GPFEN1_A))    // GPIO Pin Falling Edge Detect Enable 1 register

#define PSP_GPIO_GPHEN0_R    (*((vuint32_t *)PSP_GPIO_GPHEN0_A))    // GPIO Pin High Detect Enable 0 register
#define PSP_GPIO_GPHEN1_R    (*((vuint32_t *)PSP_GPIO_GPHEN1_A))    // GPIO Pin High Detect Enable 1 register

#define PSP_GPIO_GPLEN0_R    (*((vuint32_t *)PSP_GPIO_GPLEN0_A))    // GPIO Pin Low Detect Enable 0 register
#define PSP_GPIO_GPLEN1_R    (*((vuint32_t *)PSP_GPIO_GPLEN1_A))    // GPIO Pin Low Detect Enable 1 register

#define PSP_GPIO_GPAREN0_R   (*((vuint32_t *)PSP_GPIO_GPAREN0_A))   // GPIO Pin Async. Rising Edge Detect 0 register
#define PSP_GPIO_GPAREN1_R   (*((vuint32_t *)PSP_GPIO_GPAREN1_A))   // GPIO Pin Async. Rising Edge Detect 1 register

#define PSP_GPIO_GPAFEN0_R   (*((vuint32_t *)PSP_GPIO_GPAFEN0_A))   // GPIO Pin Async. Falling Edge Detect 0 register
#define PSP_GPIO_GPAFEN1_R   (*((vuint32_t *)PSP_GPIO_GPAFEN1_A))   // GPIO Pin Async. Falling Edge Detect 1 register

#define PSP_GPIO_GPPUD_R     (*((vuint32_t *)PSP_GPIO_GPPUD_A))     // GPIO Pin Pull-up/down Enable register

#define PSP_GPIO_GPPUDCLK0_R (*((vuint32_t *)PSP_GPIO_GPPUDCLK0_A)) // GPIO Pin Pull-up/down Enable Clock 0 register
#define PSP_GPIO_GPPUDCLK1_R (*((vuint32_t *)PSP_GPIO_GPPUDCLK1_A)) // GPIO Pin Pull-up/down Enable Clock 1 register

#define PSP_GPIO_NUM_GPIO_PINS  54u
#define PSP_GPIO_MAX_PINMODE_VALUE 0b111u

#define NUM_PINS_PER_GPFSEL_REG 10u
#define NUM_BITS_USED_IN_PINMODE 3u
#define HIGHEST_BIT_POSITION_IN_A_REGISTER 31u



/*-----------------------------------------------------------------------------------------------
    PSP_GPIO Function Definitions
 -------------------------------------------------------------------------------------------------*/

/**
 * To set a pinmode, we need to do the following steps:
 * 
 * 1) determinie which GPFSEL register the pin belongs to
 * 2) clear out the three bits for that pin in the correct GPFSEL register 
 * 3) set the three bits for that pin in the correct GPFSEL register
 * 
 * example:
 *      we want to set GPIO17 to an output.
 * 
 *      we call the function like this: PSP_GPIO_Set_Pin_Mode(17, PSP_GPIO_PINMODE_OUTPUT);
 * 
 *      to determine the correct GPFSEL register, note that there are 10 GPIO pins per GPFSEL register,
 *      the first 10 pins go into GPFSEL0, therefore pin 17 goes in GPFSEL1: (17 / 10) = 1
 * 
 *      the pin mode for a given pin is stored in 3 bits in a GPFSEL register, to find where those bits
 *      start, note that pin 17 is the 7th pin in GPFSEL1, 7th pin times three bits is 21,
 *      the starting position for the three pin mode bits is ((17 % 10) * 3) = (7 * 3) = 21
 * 
 *      to clear out the three pin mode bits for pin 17 in GPFSEL1, AND it with 0b11111111000111111111111111111111
 *      to get this pattern, take 0b111, shift it left by the calculated value of 21, then invert it
 * 
 *      Then take 0b001 (the value for PSP_GPIO_PINMODE_OUTPUT), shift that left by the calculated value 
 *      of 21, and then OR it in to GPFSEL1
 * 
 *      GPIO17 is now an output, huzzah
 */
void PSP_GPIO_Set_Pin_Mode(uint32_t pin_num, uint32_t pin_mode)
{
    if (PSP_GPIO_NUM_GPIO_PINS <= pin_num || PSP_GPIO_MAX_PINMODE_VALUE < pin_mode)
    {
        // invalid pin number or pin mode, do nothing
        return;
    }
    else /* it is a valid pin and mode */
    {
        // start at GPFSEL0, we'll move into the correct GPFSEL register by adding an offset
        uint32_t GPFSEL_n_Addr = PSP_GPIO_GPFSEL0_A;
        
        // the offset is which GPFSEL register we need to move into
        const uint32_t GPFSEL_OFFSET = pin_num / NUM_PINS_PER_GPFSEL_REG;

        // add the offset to move to the correct GPSFSEL register for this pin ( << 2 because next register is 4 away)
        GPFSEL_n_Addr += (GPFSEL_OFFSET << 2);

        // each pin gets three bits in its GPFSEL register which set its mode
        const uint32_t PIN_POSITION = (pin_num % NUM_PINS_PER_GPFSEL_REG) * NUM_BITS_USED_IN_PINMODE;
        
        // clear the 3 bits that set the old pin mode in GPFSELn
        (*((vuint32_t *)GPFSEL_n_Addr)) &= ~(0b111 << PIN_POSITION);

        // set the 3 bits in the correct GPFSEL register to the new pin mode
        (*((vuint32_t *)GPFSEL_n_Addr)) |= (pin_mode << PIN_POSITION);
    }
}


/**
 * to write to a GPIO pin, we need to do the following steps:
 * 
 * 1) determine to write to GPSETn, or GPCLRn
 * 2) determine if the pin is in the first or second batch of GPSETn/GPCLRn registers
 * 3) write to the correct register at the correct position for the pin
 *      if the pin goes in GPSET/CLR0, the position is 2^pin_num
 *      if the pin goes int GPSET/CLR1, the position is 2^(pin_num & 31)
 *      either way 2^(pin_num & 31) works, so we'll do that every time
 * 
 * example:
 *      we want to set GPIO 35 HIGH
 * 
 *      we call the function like this: PSP_GPIO_Write_Pin(35, PSP_GPIO_PIN_WRITE_HIGH);
 *      
 *      to determine if we want to write to the SET or CLR register, check the value, it is not zero,
 *      therefore set our base address to PSP_GPIO_GPSET0_A
 * 
 *      to determine if we can stay in GPSET0, or need to move into GPSET1, check
 *      the pin number. pins 0...31 go in GPSET0, our pin num of 35 is higher than that, so
 *      it goes in GPSET1. incrementing the address of GPSET0 gets us there.
 * 
 *      now we need to find the position of the bit in GPSET1 for our pin. the first 31 pins go in GPSET0,
 *      GPSET1 starts at pin 32, 33, 34, 35. therefore pin 35 occupies bit number 3 in GPSET1
 * 
 *      the way to get this bit position mathematically is to take (pin_num & 31), like this:
 * 
 *        0b00000000000000000000000000100011 (35, pin_num)
 *      & 0b00000000000000000000000000011111 (31, num pins per register - 1)
 *      -------------------------------------
 *      = 0b00000000000000000000000000000011 (3, the desired position in GPSET1)    
 * 
 *      now to get the actual bit position in GPSET1, take 2^3 = 8. in c we get this by (1 << 3) 
 *      = 0b00000000000000000000000000001000 = 2^3 = 8
 * 
 *      now we can take our calculated address of GPSET1, dereference it, and set it to our calculated
 *      pin position, and pin 35 will go high (assuming it has previously been set to an output)
 * 
 *      the nice thing about having separate SET and CLR registers, is that we don't need to worry 
 *      about overwriting other pin's information when we set/clear a pin. in the above example, we
 *      set the whole GPSET1 register to 8, which is zeros in every position except for pin 35, but
 *      the other pins in GPSET1 are not effected, how nice.
 */ 
void PSP_GPIO_Write_Pin(uint32_t pin_num, uint32_t value)
{
    if (PSP_GPIO_NUM_GPIO_PINS <= pin_num)
    {
        // invalid pin number, do nothing
        return;
    }
    else /* it is a valid pin */
    {
        // a non-zero value means we want to write to the SET register, zero means CLR register
        uint32_t GPIO_SET_OR_CLR_ADDR = value ? PSP_GPIO_GPSET0_A : PSP_GPIO_GPCLR0_A;

        // if the pin is in the second batch of SET/CLR register, increment the address to SET1/CLR1
        if (pin_num > HIGHEST_BIT_POSITION_IN_A_REGISTER)
        {
            GPIO_SET_OR_CLR_ADDR += 4; // next register is 4 bytes away
        }

        // find the position of the bit in the SET/CLR register that controls the pin
        const uint32_t PIN_POSITION = pin_num & HIGHEST_BIT_POSITION_IN_A_REGISTER;

        // get a pointer to the memory location
        vuint32_t * GPIO_SET_OR_CLR_REG = ((vuint32_t *)(GPIO_SET_OR_CLR_ADDR));

        // write to the register
        (*GPIO_SET_OR_CLR_REG) = (1 << PIN_POSITION);
    }
}



uint32_t PSP_GPIO_Read_Pin(uint32_t pin_num)
{
    uint32_t result;

    if (PSP_GPIO_NUM_GPIO_PINS <= pin_num)
    {
        // invalid pin number, return 0
        result = 0u;
    }
    else /* it is a valid pin */
    {
        // find the position of the bit in the GPLEV register that contains the pin reading
        const uint32_t PIN_POSITION = pin_num & HIGHEST_BIT_POSITION_IN_A_REGISTER;

        // shift the correct GPLEV register right until the bit of interest is in the lsb
        if (pin_num <= HIGHEST_BIT_POSITION_IN_A_REGISTER)
        {
            result = PSP_GPIO_GPLEV0_R >> PIN_POSITION;
        }
        else /* the pin goes in the second bank of pin registers */
        {
            result = PSP_GPIO_GPLEV1_R >> PIN_POSITION;
        }

        result &= 1u; // clear out any bits besides the lsb, the result is now either 0 or 1
    }

    return result;
}



void PSP_GPIO_Pin_Enable_Edge_Detect(uint32_t pin_num, PSP_GPIO_Edge_Detect_t edge)
{
    if (PSP_GPIO_NUM_GPIO_PINS <= pin_num)
    {
        // invalid pin number, do nothing
        return;
    }
    else /* it is a valid pin */
    {
        const uint32_t PIN_POSITION = pin_num & HIGHEST_BIT_POSITION_IN_A_REGISTER;

        if (pin_num <= HIGHEST_BIT_POSITION_IN_A_REGISTER)
        {
            if (edge == GPIO_EDGE_TYPE_RISING)
            {
                PSP_GPIO_GPREN0_R |=  (1u << PIN_POSITION); // set rising edge 
                PSP_GPIO_GPFEN0_R &= ~(1u << PIN_POSITION); // clear falling edge
            }
            else if (edge == GPIO_EDGE_TYPE_FALLING)
            {
                PSP_GPIO_GPREN0_R &= ~(1u << PIN_POSITION); // clear rising edge
                PSP_GPIO_GPFEN0_R |=  (1u << PIN_POSITION); // set falling edge
            }
            else if (edge == GPIO_EDGE_TYPE_CHANGING)
            {
                PSP_GPIO_GPREN0_R |= (1u << PIN_POSITION); // set rising edge
                PSP_GPIO_GPFEN0_R |= (1u << PIN_POSITION); // set falling edge
            }
            else if (edge == GPIO_EDGE_TYPE_NO_EDGE)
            {
                PSP_GPIO_GPREN0_R &= ~(1u << PIN_POSITION); // clear rising edge
                PSP_GPIO_GPFEN0_R &= ~(1u << PIN_POSITION); // clear falling edge
            }
            
        }
        else /* the pin goes in the second bank of pin registers */
        {
            if (edge == GPIO_EDGE_TYPE_RISING)
            {
                PSP_GPIO_GPREN1_R |=  (1u << PIN_POSITION); // set rising edge 
                PSP_GPIO_GPFEN1_R &= ~(1u << PIN_POSITION); // clear falling edge
            }
            else if (edge == GPIO_EDGE_TYPE_FALLING)
            {
                PSP_GPIO_GPREN1_R &= ~(1u << PIN_POSITION); // clear rising edge
                PSP_GPIO_GPFEN1_R |=  (1u << PIN_POSITION); // set falling edge
            }
            else if (edge == GPIO_EDGE_TYPE_CHANGING)
            {
                PSP_GPIO_GPREN1_R |= (1u << PIN_POSITION); // set rising edge
                PSP_GPIO_GPFEN1_R |= (1u << PIN_POSITION); // set falling edge
            }
            else if (edge == GPIO_EDGE_TYPE_NO_EDGE)
            {
                PSP_GPIO_GPREN1_R &= ~(1u << PIN_POSITION); // clear rising edge
                PSP_GPIO_GPFEN1_R &= ~(1u << PIN_POSITION); // clear falling edge
            }
        }
    }
}



uint32_t PSP_GPIO_Event_Detected(uint32_t pin_num)
{
    uint32_t result;

    if (PSP_GPIO_NUM_GPIO_PINS <= pin_num)
    {
        // invalid pin number, return 0
        result = 0u;
    }
    else /* it is a valid pin */
    {
        // find the position of the bit in the GPEDS register that contains the edge detect status
        const uint32_t PIN_POSITION = pin_num & HIGHEST_BIT_POSITION_IN_A_REGISTER;

        // shift the correct GPEDS register right until the bit of interest is in the lsb
        if (pin_num <= HIGHEST_BIT_POSITION_IN_A_REGISTER)
        {
            // move the relevant bit into the lsb
            result = PSP_GPIO_GPEDS0_R >> PIN_POSITION;
            
            // clear the event
            PSP_GPIO_GPEDS0_R |= (1u << PIN_POSITION);
        }
        else /* the pin goes in the second bank of pin registers */
        {
            // move the relevant bit into the lsb
            result = PSP_GPIO_GPEDS1_R >> PIN_POSITION;
            
            // clear the event
            PSP_GPIO_GPEDS1_R |= (1u << PIN_POSITION);
        }

        result &= 1u; // clear out any bits besides the lsb, the result is now either 0 or 1
    }

    return result;
}
