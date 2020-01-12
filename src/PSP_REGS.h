/**
 * DESCRIPTION:
 *      PSP_REGS simply keeps a publicly available list of register addresses,
 *      as well as the peripheral base address.
 * 
 * NOTES:
 *      TODO: For now this only works with the Raspberry Pi 3b+ and other Pis with 
 *      0x3F000000u as their peripheral base address. Add #ifdef statements
 *      so that it can work with other hardware.
 * 
 * REFERENCES:
 *      BCM2837-ARM-Peripherals.pdf 
 */

#ifndef PSP_REGS_H_INCLUDED
#define PSP_REGS_H_INCLUDED

/*------------------------------------------------------------------------------------------------
    Public PSP_REGS Defines
 -------------------------------------------------------------------------------------------------*/

// Register Base Addresses
#define PSP_REGS_PERIPHERAL_BASE_ADDRESS (0x3F000000u)

#define PSP_REGS_GPIO_BASE_ADDRESS         (PSP_REGS_PERIPHERAL_BASE_ADDRESS | 0x00200000u)
#define PSP_REGS_SYSCLK_BASE_ADDRESS       (PSP_REGS_PERIPHERAL_BASE_ADDRESS | 0x00003000u)
#define PSP_REGS_PWM_BASE_ADDRESS          (PSP_REGS_PERIPHERAL_BASE_ADDRESS | 0x0020C000u) 
#define PSP_REGS_SPI_0_BASE_ADDRESS        (PSP_REGS_PERIPHERAL_BASE_ADDRESS | 0x00204000u)
#define PSP_REGS_I2C_BASE_ADDRESS          (PSP_REGS_PERIPHERAL_BASE_ADDRESS | 0x00804000u)
#define PSP_REGS_AUX_BASE_ADDRESS          (PSP_REGS_PERIPHERAL_BASE_ADDRESS | 0x00215000u)
#define PSP_REGS_HARDWARE_RNG_BASE_ADDRESS (PSP_REGS_PERIPHERAL_BASE_ADDRESS | 0x00104000u)

#endif
