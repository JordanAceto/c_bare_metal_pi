#ifndef BPS_PWM_H_INCLUDED
#define BPS_PWM_H_INCLUDED

#include "Fixed_Width_Ints.h"
#include "PSP_REGS.h"

/*------------------------------------------------------------------------------------------------
    Public BSP_PWM Defines
 -------------------------------------------------------------------------------------------------*/

// PWM Register Addresses
#define BSP_PWM_BASE_ADDRESS (PSP_REGS_PWM_BASE_ADDRESS)

#define BSP_PWM_CTL_A        (BSP_PWM_BASE_ADDRESS | 0x00000000u)      // PWM control register address
#define BSP_PWM_STA_A        (BSP_PWM_BASE_ADDRESS | 0x00000004u)      // PWM status register address
#define BSP_PWM_DMAC_A       (BSP_PWM_BASE_ADDRESS | 0x00000008u)      // PWM DMA configuration register address
#define BSP_PWM_RNG1_A       (BSP_PWM_BASE_ADDRESS | 0x00000010u)      // PWM channel 1 range address
#define BSP_PWM_DAT1_A       (BSP_PWM_BASE_ADDRESS | 0x00000014u)      // PWM channel 1 data address
#define BSP_PWM_FIF1_A       (BSP_PWM_BASE_ADDRESS | 0x00000018u)      // PWM FIFO input address
#define BSP_PWM_RNG2_A       (BSP_PWM_BASE_ADDRESS | 0x00000020u)      // PWM channel 2 range address
#define BSP_PWM_DAT2_A       (BSP_PWM_BASE_ADDRESS | 0x00000024u)      // PWM channel 2 data address

// PWM Register Pointers
#define BSP_PWM_CTL_R        (*((volatile uint32_t *)BSP_PWM_CTL_A))   // PWM control register
#define BSP_PWM_STA_R        (*((volatile uint32_t *)BSP_PWM_STA_A))   // PWM status register
#define BSP_PWM_DMAC_R       (*((volatile uint32_t *)BSP_PWM_DMAC_A))  // PWM DMA configuration register
#define BSP_PWM_RNG1_R       (*((volatile uint32_t *)BSP_PWM_RNG1_A))  // PWM channel 1 range register
#define BSP_PWM_DAT1_R       (*((volatile uint32_t *)BSP_PWM_DAT1_A))  // PWM channel 1 data register
#define BSP_PWM_FIF1_R       (*((volatile uint32_t *)BSP_PWM_FIF1_A))  // PWM FIFO input register
#define BSP_PWM_RNG2_R       (*((volatile uint32_t *)BSP_PWM_RNG2_A))  // PWM channel 2 range register
#define BSP_PWM_DAT2_R       (*((volatile uint32_t *)BSP_PWM_DAT2_A))  // PWM channel 2 data register

// PWM Control Register Masks
#define PWM_CTL_MSEN2        0x00008000u                               // Channel 2 M/S Enable
#define PWM_CTL_USEF2        0x00002000u                               // Channel 2 Use Fifo
#define PWM_CTL_POLA2        0x00001000u                               // Channel 2 Polarity
#define PWM_CTL_SBIT2        0x00000800u                               // Channel 2 Silence Bit 
#define PWM_CTL_RPT2         0x00000400u                               // Channel 2 Repeat Last Data
#define PWM_CTL_MODE2        0x00000200u                               // Channel 2 Mode
#define PWM_CTL_PWEN2        0x00000100u                               // Channel 2 Enable 

#define PWM_CTL_MSEN1        0x00000080u                               // Channel 1 M/S Enable
#define PWM_CTL_CLRF1        0x00000040u                               // Clear Fifo
#define PWM_CTL_USEF1        0x00000020u                               // Channel 1 Use Fifo
#define PWM_CTL_POLA1        0x00000010u                               // Channel 1 Polarity
#define PWM_CTL_SBIT1        0x00000008u                               // Channel 1 Silence Bit
#define PWM_CTL_RPTL1        0x00000004u                               // Repeat Last Data
#define PWM_CTL_MODE1        0x00000002u                               // Channel 1 Mode
#define PWM_CTL_PWEN1        0x00000001u                               // Channel 1 Enable

// PWM Status Register Masks
#define PWM_STA_STA4         0x00001000u                               // Channel 4 State
#define PWM_STA_STA3         0x00000800u                               // Channel 3 State
#define PWM_STA_STA2         0x00000400u                               // Channel 2 State
#define PWM_STA_STA1         0x00000200u                               // Channel 1 State

#define PWM_STA_BERR         0x00000100u                               // Bus Error Flag

#define PWM_STA_GAPO4        0x00000080u                               // Channel 4 Gap Occurred Flag
#define PWM_STA_GAPO3        0x00000040u                               // Channel 3 Gap Occurred Flag
#define PWM_STA_GAPO2        0x00000020u                               // Channel 2 Gap Occurred Flag
#define PWM_STA_GAPO1        0x00000010u                               // Channel 1 Gap Occurred Flag

#define PWM_STA_RERR1        0x00000008u                               // Fifo Read Error Flag
#define PWM_STA_WERR1        0x00000004u                               // Fifo Write Error Flag

#define PWM_STA_EMPT1        0x00000002u                               // Fifo Empty Flag
#define PWM_STA_FULL1        0x00000001u                               // Fifo Full Flag

// PWM Clock Control Register Addresses
#define CM_BASE_ADDRESS      (0x3F1010A0u)
#define BSP_CM_PWMCTL_A      (CM_BASE_ADDRESS | 0x00000000u)           // PWM clock control register address
#define BSP_CM_PWMDIV_A      (CM_BASE_ADDRESS | 0x00000004u)           // PWM clock divider register address

// PWM Clock Control Register Pointers
#define BSP_CM_PWMCTL_R      (*((volatile uint32_t *)BSP_CM_PWMCTL_A)) // PWM clock control register
#define BSP_CM_PWMDIV_R      (*((volatile uint32_t *)BSP_CM_PWMDIV_A)) // PWM clock divider register

// CM PWMCTL register masks
#define CM_PWMCTL_PASSWD     0x5A000000u                               // PWM clock password
#define CM_PWMCTL_ENAB       0x00000010u                               // CM PWMCLT enable
#define CM_PWMCTL_BUSY       0x00000080u                               // CM PWMCTL Busy flag
#define CM_PWMCTL_USE_OSC    0x00000001u                               // CM PWMCTL use internal oscillator

// PWM constants
#define PWM_DEFAULT_RANGE    256u                                      // default PWM range [0, 255]
#define PWM_RANGE_MASK       0xFFu                                     // used to get (val % PWM_DEFAULT_RANGE)

#define PWM_DEFAULT_DIV      4u                                        // default clock divider, divide 19.2MHz clock by 4 = 4.8MHz



/*------------------------------------------------------------------------------------------------
    Public BSP_PWM Function Declarations
 -------------------------------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------------------------

Function Name:
    BSP_PWM_Clock_Init

Function Description:
    Initialize the PWM clock source to internal oscillator divided down to 4.8MHz and start the PWM clock.

    This function must be called before starting PWM channel 1 or 2, setting any GPIO pins to PWM mode or 
    writing any pins via PWM.

Inputs:
    None

Returns:
    None

Error Handling:
    None

-------------------------------------------------------------------------------------------------*/
void BSP_PWM_Clock_Init(void);



/*-----------------------------------------------------------------------------------------------

Function Name:
    BSP_PWM_Ch1_Start

Function Description:
    Initialize PWM Channel 1. Resolution is set to 8 bits at 18.75kHz.
    
    Note that only GPIO12 and GPIO18 are available as channel 1 PWM pins on the raspberry pi 3b+ breakout board.

Inputs:
    None

Returns:
    None

Error Handling:
    None

-------------------------------------------------------------------------------------------------*/
void BSP_PWM_Ch1_Start(void);



/*-----------------------------------------------------------------------------------------------

Function Name:
    BSP_PWM_Ch2_Start

Function Description:
    Initialize PWM Channel 2. Resolution is set to 8 bits at 18.75kHz.
    
    Note that only GPIO13 and GPIO19 are available as channel 2 PWM pins on the raspberry pi 3b+ breakout board.

Inputs:
    None

Returns:
    None

Error Handling:
    None

-------------------------------------------------------------------------------------------------*/
void BSP_PWM_Ch2_Start(void);


/*-----------------------------------------------------------------------------------------------

Function Name:
    BSP_PWM_Ch1_Write

Function Description:
    Writes a value to PWM Channel 1. Range is 8 bits.

Inputs:
    value: Value to write. Range [0,255]

Returns:
    None

Error Handling:
    Values greater than 255 will be treated as (val % 256)

-------------------------------------------------------------------------------------------------*/
void BSP_PWM_Ch1_Write(uint32_t value);



/*-----------------------------------------------------------------------------------------------

Function Name:
    BSP_PWM_Ch2_Write

Function Description:
    Writes a value to PWM Channel 2. Range is 8 bits.

Inputs:
    r0: Value to write. Range [0,255]

Returns:
    None

Error Handling:
    Values greater than 255 will be treated as (val % 256)

-------------------------------------------------------------------------------------------------*/
void BSP_PWM_Ch2_Write(uint32_t value);



/*-----------------------------------------------------------------------------------------------

Function Name:
    BSP_PWM_Ch1_Set_GPIO12_To_PWM_Mode

Function Description:
    Sets the pin mode of GPIO12 to PWM Channel 1

Inputs:
    None

Returns:
    None

Error Handling:
    None

-------------------------------------------------------------------------------------------------*/
void BSP_PWM_Ch1_Set_GPIO12_To_PWM_Mode(void);



/*-----------------------------------------------------------------------------------------------

Function Name:
    BSP_PWM_Ch1_Set_GPIO18_To_PWM_Mode

Function Description:
    Sets the pin mode of GPIO18 to PWM Channel 1

Inputs:
    None

Returns:
    None

Error Handling:
    None

-------------------------------------------------------------------------------------------------*/
void BSP_PWM_Ch1_Set_GPIO18_To_PWM_Mode(void);



/*-----------------------------------------------------------------------------------------------

Function Name:
    BSP_PWM_Ch2_Set_GPIO13_To_PWM_Mode

Function Description:
    Sets the pin mode of GPIO13 to PWM Channel 2

Inputs:
    None

Returns:
    None

Error Handling:
    None

-------------------------------------------------------------------------------------------------*/
void BSP_PWM_Ch2_Set_GPIO13_To_PWM_Mode(void);



/*-----------------------------------------------------------------------------------------------

Function Name:
    BSP_PWM_Ch2_Set_GPIO19_To_PWM_Mode

Function Description:
    Sets the pin mode of GPIO19 to PWM Channel 2

Inputs:
    None

Returns:
    None

Error Handling:
    None

-------------------------------------------------------------------------------------------------*/
void BSP_PWM_Ch2_Set_GPIO19_To_PWM_Mode(void);



#endif
