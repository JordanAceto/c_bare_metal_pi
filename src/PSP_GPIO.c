
#include "PSP_GPIO.h"

/*-----------------------------------------------------------------------------------------------
    Private PSP_GPIO Defines
 -------------------------------------------------------------------------------------------------*/
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
        return; // invalid pin number or pin mode, do nothing
    }
    else
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
        (*((volatile uint32_t *)GPFSEL_n_Addr)) &= ~(0b111 << PIN_POSITION);

        // set the 3 bits in the correct GPFSEL register to the new pin mode
        (*((volatile uint32_t *)GPFSEL_n_Addr)) |= (pin_mode << PIN_POSITION);
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
        return; // invalid pin number, do nothing
    }
    else
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
        volatile uint32_t * GPIO_SET_OR_CLR_REG = ((volatile uint32_t *)(GPIO_SET_OR_CLR_ADDR));

        // write to the register
        (*GPIO_SET_OR_CLR_REG) = (1 << PIN_POSITION);
    }
}



uint32_t PSP_GPIO_Read_Pin(uint32_t pin_num)
{
    uint32_t result;

    if (PSP_GPIO_NUM_GPIO_PINS <= pin_num)
    {
        result = 0u; // invalid pin number, return 0
    }
    else
    {
        // find the position of the bit in the GPLEV register that contains the pin reading
        const uint32_t PIN_POSITION = pin_num & HIGHEST_BIT_POSITION_IN_A_REGISTER;

        // shift the correct GPLEV register right until the bit of interest is in the lsb
        if (pin_num <= HIGHEST_BIT_POSITION_IN_A_REGISTER)
        {
            result = PSP_GPIO_GPLEV0_R >> PIN_POSITION;
        }
        else
        {
            result = PSP_GPIO_GPLEV1_R >> PIN_POSITION;
        }

        result &= 1u; // clear out any bits besides the lsb, the result is now either 0 or 1
    }

    return result;
}
