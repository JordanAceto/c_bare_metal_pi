/*
--|----------------------------------------------------------------------------|
--| FILE DESCRIPTION:
--|   PSP_REGS simply keeps a publicly available list of register addresses,
--|   as well as the peripheral base address.
--|  
--|----------------------------------------------------------------------------|
--| REFERENCES:
--|   BCM2837-ARM-Peripherals.pdf 
--|
--|----------------------------------------------------------------------------|
*/

#ifndef PSP_REGS_H_INCLUDED
#define PSP_REGS_H_INCLUDED

/*
--|----------------------------------------------------------------------------|
--| INCLUDE FILES
--|----------------------------------------------------------------------------|
*/

/* None */

/*
--|----------------------------------------------------------------------------|
--| PUBLIC DEFINES
--|----------------------------------------------------------------------------|
*/

/*
--| NAME: PSP_REGS_PERIPHERAL_BASE_ADDRESS
--| DESCRIPTION: peripheral base address
--| TYPE: uint32_t
*/
#define PSP_REGS_PERIPHERAL_BASE_ADDRESS (0x3F000000u)

/*
--| NAME: PSP_REGS_xxx_ADDRESS
--| DESCRIPTION: base address of a given peripheral
--| TYPE: uint32_t
*/
#define PSP_REGS_GPIO_BASE_ADDRESS         (PSP_REGS_PERIPHERAL_BASE_ADDRESS | 0x00200000u)
#define PSP_REGS_SYSCLK_BASE_ADDRESS       (PSP_REGS_PERIPHERAL_BASE_ADDRESS | 0x00003000u)
#define PSP_REGS_PWM_BASE_ADDRESS          (PSP_REGS_PERIPHERAL_BASE_ADDRESS | 0x0020C000u) 
#define PSP_REGS_SPI_0_BASE_ADDRESS        (PSP_REGS_PERIPHERAL_BASE_ADDRESS | 0x00204000u)
#define PSP_REGS_I2C_BASE_ADDRESS          (PSP_REGS_PERIPHERAL_BASE_ADDRESS | 0x00804000u)
#define PSP_REGS_AUX_BASE_ADDRESS          (PSP_REGS_PERIPHERAL_BASE_ADDRESS | 0x00215000u)
#define PSP_REGS_HARDWARE_RNG_BASE_ADDRESS (PSP_REGS_PERIPHERAL_BASE_ADDRESS | 0x00104000u)
#define PSP_REGS_PWM_CLK_MAN_BASE_ADDRESS  (PSP_REGS_PERIPHERAL_BASE_ADDRESS | 0x001010A0u)

/*
--|----------------------------------------------------------------------------|
--| PUBLIC TYPES
--|----------------------------------------------------------------------------|
*/

/* None */

/*
--|----------------------------------------------------------------------------|
--| PUBLIC CONSTANTS
--|----------------------------------------------------------------------------|
*/

/* None */

/*
--|----------------------------------------------------------------------------|
--| PUBLIC VARIABLES
--|----------------------------------------------------------------------------|
*/

/* None */

/*
--|----------------------------------------------------------------------------|
--| PUBLIC FUNCTION PROTOTYPES
--|----------------------------------------------------------------------------|
*/

/* None */

#endif
