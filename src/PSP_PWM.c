
#include "PSP_PWM.h"
#include "PSP_REGS.h"
#include "PSP_GPIO.h"

/*------------------------------------------------------------------------------------------------
    Private PSP_PWM Defines
 -------------------------------------------------------------------------------------------------*/

// PWM Register Addresses
#define PSP_PWM_BASE_ADDRESS (PSP_REGS_PWM_BASE_ADDRESS)

#define PSP_PWM_CTL_A        (PSP_PWM_BASE_ADDRESS | 0x00000000u) // PWM control register address
#define PSP_PWM_STA_A        (PSP_PWM_BASE_ADDRESS | 0x00000004u) // PWM status register address
#define PSP_PWM_DMAC_A       (PSP_PWM_BASE_ADDRESS | 0x00000008u) // PWM DMA configuration register address
#define PSP_PWM_RNG1_A       (PSP_PWM_BASE_ADDRESS | 0x00000010u) // PWM channel 1 range address
#define PSP_PWM_DAT1_A       (PSP_PWM_BASE_ADDRESS | 0x00000014u) // PWM channel 1 data address
#define PSP_PWM_FIF1_A       (PSP_PWM_BASE_ADDRESS | 0x00000018u) // PWM FIFO input address
#define PSP_PWM_RNG2_A       (PSP_PWM_BASE_ADDRESS | 0x00000020u) // PWM channel 2 range address
#define PSP_PWM_DAT2_A       (PSP_PWM_BASE_ADDRESS | 0x00000024u) // PWM channel 2 data address

// PWM Register Pointers
#define PSP_PWM_CTL_R        (*((vuint32_t *)PSP_PWM_CTL_A))      // PWM control register
#define PSP_PWM_STA_R        (*((vuint32_t *)PSP_PWM_STA_A))      // PWM status register
#define PSP_PWM_DMAC_R       (*((vuint32_t *)PSP_PWM_DMAC_A))     // PWM DMA configuration register
#define PSP_PWM_RNG1_R       (*((vuint32_t *)PSP_PWM_RNG1_A))     // PWM channel 1 range register
#define PSP_PWM_DAT1_R       (*((vuint32_t *)PSP_PWM_DAT1_A))     // PWM channel 1 data register
#define PSP_PWM_FIF1_R       (*((vuint32_t *)PSP_PWM_FIF1_A))     // PWM FIFO input register
#define PSP_PWM_RNG2_R       (*((vuint32_t *)PSP_PWM_RNG2_A))     // PWM channel 2 range register
#define PSP_PWM_DAT2_R       (*((vuint32_t *)PSP_PWM_DAT2_A))     // PWM channel 2 data register

// PWM Control Register Masks
#define PWM_CTL_MSEN2        0x00008000u                          // Channel 2 M/S Enable
#define PWM_CTL_USEF2        0x00002000u                          // Channel 2 Use Fifo
#define PWM_CTL_POLA2        0x00001000u                          // Channel 2 Polarity
#define PWM_CTL_SBIT2        0x00000800u                          // Channel 2 Silence Bit 
#define PWM_CTL_RPT2         0x00000400u                          // Channel 2 Repeat Last Data
#define PWM_CTL_MODE2        0x00000200u                          // Channel 2 Mode
#define PWM_CTL_PWEN2        0x00000100u                          // Channel 2 Enable 

#define PWM_CTL_MSEN1        0x00000080u                          // Channel 1 M/S Enable
#define PWM_CTL_CLRF1        0x00000040u                          // Clear Fifo
#define PWM_CTL_USEF1        0x00000020u                          // Channel 1 Use Fifo
#define PWM_CTL_POLA1        0x00000010u                          // Channel 1 Polarity
#define PWM_CTL_SBIT1        0x00000008u                          // Channel 1 Silence Bit
#define PWM_CTL_RPTL1        0x00000004u                          // Repeat Last Data
#define PWM_CTL_MODE1        0x00000002u                          // Channel 1 Mode
#define PWM_CTL_PWEN1        0x00000001u                          // Channel 1 Enable

// PWM Status Register Masks
#define PWM_STA_STA4         0x00001000u                          // Channel 4 State
#define PWM_STA_STA3         0x00000800u                          // Channel 3 State
#define PWM_STA_STA2         0x00000400u                          // Channel 2 State
#define PWM_STA_STA1         0x00000200u                          // Channel 1 State

#define PWM_STA_BERR         0x00000100u                          // Bus Error Flag

#define PWM_STA_GAPO4        0x00000080u                          // Channel 4 Gap Occurred Flag
#define PWM_STA_GAPO3        0x00000040u                          // Channel 3 Gap Occurred Flag
#define PWM_STA_GAPO2        0x00000020u                          // Channel 2 Gap Occurred Flag
#define PWM_STA_GAPO1        0x00000010u                          // Channel 1 Gap Occurred Flag

#define PWM_STA_RERR1        0x00000008u                          // Fifo Read Error Flag
#define PWM_STA_WERR1        0x00000004u                          // Fifo Write Error Flag

#define PWM_STA_EMPT1        0x00000002u                          // Fifo Empty Flag
#define PWM_STA_FULL1        0x00000001u                          // Fifo Full Flag

// PWM Clock Control Register Addresses
#define CM_BASE_ADDRESS      (0x3F1010A0u)
#define PSP_CM_PWMCTL_A      (CM_BASE_ADDRESS | 0x00000000u)      // PWM clock control register address
#define PSP_CM_PWMDIV_A      (CM_BASE_ADDRESS | 0x00000004u)      // PWM clock divider register address

// PWM Clock Control Register Pointers
#define PSP_CM_PWMCTL_R      (*((vuint32_t *)PSP_CM_PWMCTL_A))    // PWM clock control register
#define PSP_CM_PWMDIV_R      (*((vuint32_t *)PSP_CM_PWMDIV_A))    // PWM clock divider register

// CM PWMCTL register masks
#define CM_PWMCTL_PASSWD     0x5A000000u                          // PWM clock password
#define CM_PWMCTL_ENAB       0x00000010u                          // CM PWMCLT enable
#define CM_PWMCTL_BUSY       0x00000080u                          // CM PWMCTL Busy flag
#define CM_PWMCTL_USE_OSC    0x00000001u                          // CM PWMCTL use internal oscillator

#define CLOCK_RUNNING 1u
#define CLOCK_NOT_RUNNING 0u

/*------------------------------------------------------------------------------------------------
    PSP_PWM Function Definitions
 -------------------------------------------------------------------------------------------------*/


void PSP_PWM_Clock_Init_Default(void)
{
    PSP_PWM_Clock_Init(PWM_DEFAULT_CLOCK, PWM_DEFAULT_DIV);
}



void PSP_PWM_Clock_Init(PSP_PWM_Clock_Source_t clock_source, uint32_t divider)
{
    // request a clock stop
    PSP_CM_PWMCTL_R = CM_PWMCTL_PASSWD | (PSP_CM_PWMCTL_R & (~CM_PWMCTL_ENAB));

    while ((PSP_CM_PWMCTL_R & CM_PWMCTL_BUSY) == CLOCK_RUNNING)
    {
        // wait for the clock to stop
    }
    
    // set the divider, left shift of 12 pushes the divider setting into the integer part of the divider register
    PSP_CM_PWMDIV_R = CM_PWMCTL_PASSWD | (divider << 12);

    // set the clock source
    PSP_CM_PWMCTL_R = CM_PWMCTL_PASSWD | clock_source;

    // request a clock start (datasheet says not to change the clock source and assert enable at the same time)
    PSP_CM_PWMCTL_R = CM_PWMCTL_PASSWD | CM_PWMCTL_ENAB | clock_source;

    while ((PSP_CM_PWMCTL_R & CM_PWMCTL_BUSY) == CLOCK_NOT_RUNNING)
    {
        // wait for the clock to start
    }
}



void PSP_PWM_Channel_Start(PSP_PWM_Channel_t channel, PSP_PWM_Output_Mode_t mode, PSP_PWM_Range_t range)
{
    if (channel == PSP_PWM_Channel_1)
    {
        // shift the mode into the MSEN1 position and enable
        PSP_PWM_CTL_R |= (mode << 7u) | PWM_CTL_PWEN1;
        PSP_PWM_RNG1_R = range;
    }
    else
    {
        // shift the mode into the MSEN2 position and enable
        PSP_PWM_CTL_R |= (mode << 15u) | PWM_CTL_PWEN2;
        PSP_PWM_RNG2_R = range;   
    }
}



void PSP_PWM_Ch1_Write(uint32_t value)
{
    PSP_PWM_DAT1_R = value;
}



void PSP_PWM_Ch2_Write(uint32_t value)
{
    PSP_PWM_DAT2_R = value;
}



void PSP_PWM_Ch1_Set_GPIO12_To_PWM_Mode(void)
{
    PSP_GPIO_Set_Pin_Mode(12u, PSP_GPIO_PINMODE_ALT0);
}



void PSP_PWM_Ch1_Set_GPIO18_To_PWM_Mode(void)
{
    PSP_GPIO_Set_Pin_Mode(18u, PSP_GPIO_PINMODE_ALT5);
}



void PSP_PWM_Ch2_Set_GPIO13_To_PWM_Mode(void)
{
    PSP_GPIO_Set_Pin_Mode(13u, PSP_GPIO_PINMODE_ALT0);
}



void PSP_PWM_Ch2_Set_GPIO19_To_PWM_Mode(void)
{
    PSP_GPIO_Set_Pin_Mode(19u, PSP_GPIO_PINMODE_ALT5); 
}
