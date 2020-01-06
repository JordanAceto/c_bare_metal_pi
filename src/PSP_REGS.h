
#ifndef PSP_REGS_H_INCLUDED
#define PSP_REGS_H_INCLUDED

/*------------------------------------------------------------------------------------------------
    Public PSP_REGS Defines
 -------------------------------------------------------------------------------------------------*/

// Register Base Addresses
#define PSP_REGS_PERIPHERAL_BASE_ADDRESS (0x3F000000u)

#define PSP_REGS_GPIO_BASE_ADDRESS       (PSP_REGS_PERIPHERAL_BASE_ADDRESS | 0x00200000u)
#define PSP_REGS_SYSCLK_BASE_ADDRESS     (PSP_REGS_PERIPHERAL_BASE_ADDRESS | 0x00003000u)
#define PSP_REGS_PWM_BASE_ADDRESS        (PSP_REGS_PERIPHERAL_BASE_ADDRESS | 0x0020C000u) 
#define PSP_REGS_SPI_0_BASE_ADDRESS      (PSP_REGS_PERIPHERAL_BASE_ADDRESS | 0x00204000u)
#define PSP_REGS_I2C_BASE_ADDRESS        (PSP_REGS_PERIPHERAL_BASE_ADDRESS | 0x00804000u)

#endif
