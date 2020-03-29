/*
--|----------------------------------------------------------------------------|
--| FILE DESCRIPTION:
--|   PSP_GPIO.c provides the implementation for GPIO pin reading and writing.
--|  
--|----------------------------------------------------------------------------|
--| REFERENCES:
--|   BCM2837-ARM-Peripherals.pdf page 89
--|
--|----------------------------------------------------------------------------|
*/

/*
--|----------------------------------------------------------------------------|
--| INCLUDE FILES
--|----------------------------------------------------------------------------|
*/

#include "PSP_GPIO.h"
#include "PSP_REGS.h"

/*
--|----------------------------------------------------------------------------|
--| PRIVATE DEFINES
--|----------------------------------------------------------------------------|
*/

/*
--| NAME: NUM_GPIO_PINS
--| DESCRIPTION: The number of GPIO pins supported by the processor
--| TYPE: uint
*/
#define NUM_GPIO_PINS (54u)

/*
--| NAME: NUM_PINS_PER_GPFSEL
--| DESCRIPTION: The number of pins handled in a GPFSELn register
--| TYPE: uint
*/
#define NUM_PINS_PER_GPFSEL (10u)

/*
--| NAME: NUM_BITS_USED_PER_PIN_IN_GPFSEL
--| DESCRIPTION: The number of bits used in a GPFSELn register for a single pin
--| TYPE: uint
*/
#define NUM_BITS_USED_PER_PIN_IN_GPFSEL (3u)

/*
--| NAME: REGISTER_WIDTH
--| DESCRIPTION: The width of a register, in bits
--| TYPE: uint
*/
#define REGISTER_WIDTH (32u)

/*
--| NAME: HIGHEST_BIT_POSITION_IN_A_REGISTER
--| DESCRIPTION: The highest bit position in a reg, 1 less than the reg width
--| TYPE: uint
*/
#define HIGHEST_BIT_POSITION_IN_A_REGISTER (31u)

/*
--| NAME: GPIO
--| DESCRIPTION: pointer to the GPIO register structure
--| TYPE: GPIO_t *
*/
#define GPIO ((GPIO_t *)PSP_REGS_GPIO_BASE_ADDRESS)

/*
--|----------------------------------------------------------------------------|
--| PRIVATE TYPES
--|----------------------------------------------------------------------------|
*/

/*
--| NAME: GPIO_t
--| DESCRIPTION: structure for GPIO registers
*/
typedef struct GPIO_Type
{
    vuint32_t GPFSELn[6u];   // GPIO Function Select [0...5, rw]
    vuint32_t reserved_1;    // 
    vuint32_t GPSETn[2u];    // GPIO Pin Output Set [0...1, w]
    vuint32_t reserved_2;    // 
    vuint32_t GPCLRn[2u];    // GPIO Pin Output Clear [0...1, w]
    vuint32_t reserved_3;    // 
    vuint32_t GPLEVn[2u];    // GPIO Pin Level [0...1, r]
    vuint32_t reserved_4;    // 
    vuint32_t GPEDSn[2u];    // GPIO Pin Event Detect Status [0...1, rw]
    vuint32_t reserved_5;    // 
    vuint32_t GPRENn[2u];    // GPIO Pin Rising Edge Detect Enable [0...1, rw]
    vuint32_t reserved_6;    // 
    vuint32_t GPFENn[2u];    // GPIO Pin Falling Edge Detect Enable [0...1, rw]
    vuint32_t reserved_7;    // 
    vuint32_t GPHENn[2u];    // GPIO Pin High Detect Enable [0...1, rw]
    vuint32_t reserved_8;    // 
    vuint32_t GPLENn[2u];    // GPIO Pin Low Detect Enable [0...1, rw]
    vuint32_t reserved_9;    // 
    vuint32_t GPARENn[2u];   // GPIO Pin Async. Rising Edge Detect [0...1, rw]
    vuint32_t reserved_10;   // 
    vuint32_t GPAFENn[2u];   // GPIO Pin Async. Falling Edge Detect [0...1, rw]
    vuint32_t reserved_11;   // 
    vuint32_t GPPUD;         // GPIO Pin Pull-up/down Enable [rw]
    vuint32_t GPPUDCLKn[2u]; //  GPIO Pin Pull-up/down Enable Clock [0...1, rw]
    vuint32_t reserved_12;   // 
    vuint32_t test;          // unknown purpose, rw
} GPIO_t;

/*
--|----------------------------------------------------------------------------|
--| PRIVATE CONSTANTS
--|----------------------------------------------------------------------------|
*/

/* None */

/*
--|----------------------------------------------------------------------------|
--| PRIVATE VARIABLES
--|----------------------------------------------------------------------------|
*/

/* None */

/*
--|----------------------------------------------------------------------------|
--| PRIVATE HELPER FUNCTION PROTOTYPES
--|----------------------------------------------------------------------------|
*/

/*------------------------------------------------------------------------------
Function Name:
    is_valid_GPIO_pin_number

Function Description:
    test a given pin number for validity

Parameters:
    pin_num: the pin num to test for validity

Returns:
    Boolean: true if the given pin number is valid, else false

Assumptions/Limitations:
    None
------------------------------------------------------------------------------*/
uint32_t is_valid_GPIO_pin_number(uint32_t pin_num);

/*------------------------------------------------------------------------------
Function Name:
    is_valid_GPIO_pin_mode

Function Description:
    test a given pin mode for validity

Parameters:
    pin_mode: the pin mode to test for validity

Returns:
    Boolean: true if the given pin mode is valid, else false

Assumptions/Limitations:
    None
------------------------------------------------------------------------------*/
uint32_t is_valid_GPIO_pin_mode(GPIO_Pin_Mode_enum pin_mode);

/*------------------------------------------------------------------------------
Function Name:
    is_valid_GPIO_edge_type

Function Description:
    test a given edge type for validity

Parameters:
    edge: the edge to test for validity

Returns:
    Boolean: true if the given edge is valid, else false

Assumptions/Limitations:
    None
------------------------------------------------------------------------------*/
uint32_t is_valid_GPIO_edge_type(PSP_GPIO_Edge_Detect_enum edge);

/*
--|----------------------------------------------------------------------------|
--| PUBLIC FUNCTION DEFINITIONS
--|----------------------------------------------------------------------------|
*/

void PSP_GPIO_Set_Pin_Mode(uint32_t pin_num, GPIO_Pin_Mode_enum pin_mode)
{
    if (is_valid_GPIO_pin_number(pin_num) && is_valid_GPIO_pin_mode(pin_mode))
    {
        const uint32_t FSEL_NUMBER = pin_num / NUM_PINS_PER_GPFSEL;

        const uint32_t SHIFT_AMT = (pin_num % NUM_PINS_PER_GPFSEL) * NUM_BITS_USED_PER_PIN_IN_GPFSEL;

        // clear out the old pin mode
        GPIO->GPFSELn[FSEL_NUMBER] &= ~(0b111u << SHIFT_AMT);

        // write the new pin mode
        GPIO->GPFSELn[FSEL_NUMBER] |= (pin_mode << SHIFT_AMT);
    }
    else
    {
        /* it was an invalid pin number and/or mode, do nothing */
    }
}

void PSP_GPIO_Write_Pin(uint32_t pin_num, GPIO_Pin_Output_Write_enum value)
{
    if (is_valid_GPIO_pin_number(pin_num))
    {
        // determine whether the pin is in GPSET/CLR[0] or GPSET/CLR[1]
        const uint32_t SET_OR_CLR_INDEX = pin_num / REGISTER_WIDTH;

        // find the position of the bit in the GPSET/CLR register that controls the pin
        const uint32_t PIN_POSITION = pin_num & HIGHEST_BIT_POSITION_IN_A_REGISTER;

        if (value == PSP_GPIO_PIN_WRITE_HIGH)
        {
            GPIO->GPSETn[SET_OR_CLR_INDEX] |= 1u << PIN_POSITION;
        }
        else /* it is write LOW */
        {
            GPIO->GPCLRn[SET_OR_CLR_INDEX] |= 1u << PIN_POSITION;
        }
    }
    else
    {
        /* it was an invalid pin number, do nothing */
    }
}

GPIO_Pin_Input_Read_enum PSP_GPIO_Read_Pin(uint32_t pin_num)
{
    GPIO_Pin_Input_Read_enum result;

    if (is_valid_GPIO_pin_number(pin_num))
    {
        // determine whether the pin is in GPLEV[0] or GPSLEV[1]
        const uint32_t LEV_REG_INDEX = pin_num / REGISTER_WIDTH;

        // find the position of the bit in the GPLEV register that contains the pin
        const uint32_t PIN_POSITION = pin_num & HIGHEST_BIT_POSITION_IN_A_REGISTER;

        // push the reading into the lsb and clear all but the lsb
        result = (GPIO->GPLEVn[LEV_REG_INDEX] >> PIN_POSITION) & 1u;
    }
    else /* it is an invalid pin number, return LOW as default */
    {
        result = PSP_GPIO_PIN_READ_LOW;
    }

    return result;
}

void PSP_GPIO_Pin_Enable_Edge_Detect(uint32_t pin_num, PSP_GPIO_Edge_Detect_enum edge)
{
    if (is_valid_GPIO_pin_number(pin_num) && is_valid_GPIO_edge_type(edge))
    {
        // determine whether the pin is in GPREN/FEN[0] or GPREN/FEN[1]
        const uint32_t REN_FEN_REG_INDEX = pin_num / REGISTER_WIDTH;

        // find the position of the bit in the GPREN/FEN register that contains the pin
        const uint32_t PIN_POSITION = pin_num & HIGHEST_BIT_POSITION_IN_A_REGISTER;
      
        if (edge == GPIO_EDGE_TYPE_RISING)
        {
            GPIO->GPRENn[REN_FEN_REG_INDEX] |=  (1u << PIN_POSITION); // set rising edge 
            GPIO->GPFENn[REN_FEN_REG_INDEX] &= ~(1u << PIN_POSITION); // clear falling edge
        }
        else if (edge == GPIO_EDGE_TYPE_FALLING)
        {
            GPIO->GPRENn[REN_FEN_REG_INDEX] &= ~(1u << PIN_POSITION); // clear rising edge
            GPIO->GPFENn[REN_FEN_REG_INDEX] |=  (1u << PIN_POSITION); // set falling edge
        }
        else if (edge == GPIO_EDGE_TYPE_CHANGING)
        {
            GPIO->GPRENn[REN_FEN_REG_INDEX] |= (1u << PIN_POSITION); // set rising edge
            GPIO->GPFENn[REN_FEN_REG_INDEX] |= (1u << PIN_POSITION); // set falling edge
        }
        else if (edge == GPIO_EDGE_TYPE_NO_EDGE)
        {
            GPIO->GPRENn[REN_FEN_REG_INDEX] &= ~(1u << PIN_POSITION); // clear rising edge
            GPIO->GPFENn[REN_FEN_REG_INDEX] &= ~(1u << PIN_POSITION); // clear falling edge
        }
    }
    else
    {
        /* invalid pin number, do nothing */
    }
}

PSP_GPIO_Edge_Detect_Reading_enum PSP_GPIO_Event_Detected(uint32_t pin_num)
{
    PSP_GPIO_Edge_Detect_Reading_enum result;

    if (is_valid_GPIO_pin_number(pin_num))
    {
        // determine whether the pin is in GPEDS[0] or GPEDS[1]
        const uint32_t GPEDS_REG_INDEX = pin_num / REGISTER_WIDTH;

        // find the position of the bit in the GPEDS register that contains the pin
        const uint32_t PIN_POSITION = pin_num & HIGHEST_BIT_POSITION_IN_A_REGISTER;

        result = (GPIO->GPEDSn[GPEDS_REG_INDEX] >> PIN_POSITION) & 1u;
    }
    else
    {
        result = GPIO_EDGE_NOT_DETECTED;
    }

    return result;
}

/*
--|----------------------------------------------------------------------------|
--| PRIVATE HELPER FUNCTION DEFINITIONS
--|----------------------------------------------------------------------------|
*/

uint32_t is_valid_GPIO_pin_number(uint32_t pin_num)
{
    return pin_num < NUM_GPIO_PINS; // unsigned, no need to check for less than zero
}

uint32_t is_valid_GPIO_pin_mode(GPIO_Pin_Mode_enum pin_mode)
{
    return 0 <= pin_mode && pin_mode <= PSP_GPIO_MAX_PINMODE_VAL;
}

uint32_t is_valid_GPIO_edge_type(PSP_GPIO_Edge_Detect_enum edge)
{
    return  0 <= edge && edge <= GPIO_MAX_EDGE_TYPE_VAL; 
}
