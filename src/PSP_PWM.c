
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



/*------------------------------------------------------------------------------------------------
    Private PSP_PWM Types
 -------------------------------------------------------------------------------------------------*/


/* PWM Control Register Type */
typedef union PWM_Control_Register_Type
{
    struct
    {
        vuint32_t PWEN1 : 1u; // Channel 1 Enable. 0: Channel is disabled. 1: Channel is enabled
        vuint32_t MODE1 : 1u; // Channel 1 Mode. 0: PWM mode. 1: Serialiser mode
        vuint32_t RPTL1 : 1u; // Channel 1 Repeat Last Data
        vuint32_t SBIT1 : 1u; // Channel 1 Silence Bit. Defines the state of the output when no transmission takes place
        vuint32_t POLA1 : 1u; // Channel 1 Polarity. 0 : 0=low 1=high. 1: 1=low 0=high
        vuint32_t USEF1 : 1u; // Channel 1 Use Fifo. 0: Data register is transmitted. 1: Fifo is used for transmission
        vuint32_t CLRF1 : 1u; // Clear Fifo. 1: Clears FIFO. 0: Has no effect.
        vuint32_t MSEN1 : 1u; // Channel 1 M/S Enable. 0: PWM algorithm is used. 1: M/S transmission is used.
        vuint32_t PWEN2 : 1u; // Channel 2 Enable. 0: Channel is disabled. 1: Channel is enabled
        vuint32_t MODE2 : 1u; // Channel 2 Mode. 0: PWM mode. 1: Serialiser mode
        vuint32_t RPTL2 : 1u; // Channel 2 Repeat Last Data
        vuint32_t SBIT2 : 1u; // Channel 2 Silence Bit. Defines the state of the output when no transmission takes place
        vuint32_t POLA2 : 1u; // Channel 2 Polarity. 0 : 0=low 1=high. 1: 1=low 0=high
        vuint32_t USEF2 : 1u; // Channel 2 Use Fifo. 0: Data register is transmitted. 1: Fifo is used for transmission
        vuint32_t       : 1u; // Reserved - Write as 0, read as don't care
        vuint32_t MSEN2 : 1u; // Channel 2 M/S Enable. 0: PWM algorithm is used. 1: M/S transmission is used.
        vuint32_t       :16u; // Reserved - Write as 0, read as don't care
    };

    vuint32_t DATA; // Read/write the entire 32 bit word at once.
    
} PWM_Control_Register_t;



/* PWM Status Register Type */
typedef union PWM_Status_Register_Type
{
    struct
    {
        vuint32_t FULL1 :  1u; // Fifo Full Flag
        vuint32_t EMPT1 :  1u; // Fifo Empty Flag
        vuint32_t WERR1 :  1u; // Fifo Write Error Flag
        vuint32_t RERR1 :  1u; // Fifo Read Error Flag
        vuint32_t GAPO1 :  1u; // Channel 1 Gap Occurred Flag
        vuint32_t GAPO2 :  1u; // Channel 2 Gap Occurred Flag
        vuint32_t GAPO3 :  1u; // Channel 3 Gap Occurred Flag
        vuint32_t GAPO4 :  1u; // Channel 4 Gap Occurred Flag
        vuint32_t BERR  :  1u; // Bus Error Flag
        vuint32_t STA1  :  1u; // Channel 1 State
        vuint32_t STA2  :  1u; // Channel 2 State
        vuint32_t STA3  :  1u; // Channel 3 State
        vuint32_t STA4  :  1u; // Channel 4 State
        vuint32_t       : 19u; // Reserved - Write as 0, read as don't care
    };

    vuint32_t DATA; // Read/write the entire 32 bit word at once.

} PWM_Status_Register_t;



/* PWM DMAC Register Type */
typedef union PWM_DMAC_Register_Type
{
    struct
    {
        vuint32_t DREQ  :  8u; // DMA Threshold for DREQ signal
        vuint32_t PANIC :  8u; // DMA Threshold for PANIC signal
        vuint32_t       : 15u; // Reserved - Write as 0, read as don't care
        vuint32_t ENAB  :  1u; // DMA Enable. 0: DMA disabled. 1: DMA enabled
    };

    vuint32_t DATA; // Read/write the entire 32 bit word at once.

} PWM_DMAC_Register_t;



typedef struct PWM_Type
{
    PWM_Control_Register_t CTL;        // PWM Control Register
    PWM_Status_Register_t  STA;        // PWM Status Register
    PWM_DMAC_Register_t    DMAC;       // PWM DMA Configuration Register
    vuint32_t              RESERVED_1; // Unused
    vuint32_t              RNG1;       // PWM Channel 1 Range Register
    vuint32_t              DAT1;       // PWM Channel 1 Data Register
    vuint32_t              FIF1;       // PWM FIFO Input Register
    vuint32_t              RESERVED_2; // Unused
    vuint32_t              RNG2;       // PWM Channel 2 Range Register
    vuint32_t              DAT2;       // PWM Channel 2 Data Register
} PWM_t;



/* PWM Clock Manager Clock Control Type */
typedef union CM_PWMCTL_Type
{
    struct 
    {
        vuint32_t SRC    :  4u; // Clock source
        vuint32_t ENAB   :  1u; // Enable the clock generator
        vuint32_t KILL   :  1u; // Kill the clock generator
        vuint32_t        :  1u; // Unused
        vuint32_t BUSY   :  1u; // Clock generator is running
        vuint32_t FLIP   :  1u; // Invert the clock generator output
        vuint32_t MASH   :  2u; // MASH control
        vuint32_t        : 13u; // Unused
        vuint32_t PASSWD :  8u; // Clock Manager password “5a”
    };
    
    vuint32_t DATA; // Read/write the entire 32 bit word at once.

} CM_PWMCTL_t;




/* PWM Clock Manager Clock Divider Type */
typedef union  CM_PWMDIV_Type
{
    struct
    {
        vuint32_t DIVF   : 12u; // Fractional part of divisor
        vuint32_t DIVI   : 12u; // Integer part of divisor
        vuint32_t PASSWD :  8u; // Clock Manager password “5a”
    };

    vuint32_t DATA; // Read/write the entire 32 bit word at once.
    
} CM_PWMDIV_t;



typedef struct PWM_Clock_Manager_Type
{
    CM_PWMCTL_t PWMCTL; // Clock Manager Control Register
    CM_PWMDIV_t PWMDIV; // Clock Manager Divider Register
} PWM_Clock_Manager_t;



/*------------------------------------------------------------------------------------------------
    Private PSP_PWM Variables
 -------------------------------------------------------------------------------------------------*/


volatile PWM_t * PWM = (volatile PWM_t *)PSP_PWM_BASE_ADDRESS;

volatile PWM_Clock_Manager_t * PWM_Clock_Manager = (volatile PWM_Clock_Manager_t *)CM_BASE_ADDRESS;



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
    PWM_Clock_Manager->PWMCTL.DATA = CM_PWMCTL_PASSWD | (PSP_CM_PWMCTL_R & (~CM_PWMCTL_ENAB));

    while (PWM_Clock_Manager->PWMCTL.BUSY)
    {
        // wait for the clock to stop
    }
    
    // set the divider, left shift of 12 pushes the divider setting into the integer part of the divider register
    PWM_Clock_Manager->PWMDIV.DATA = CM_PWMCTL_PASSWD | (divider << 12);

    // set the clock source
    PWM_Clock_Manager->PWMCTL.DATA = CM_PWMCTL_PASSWD | clock_source;

    // request a clock start (datasheet says not to change the clock source and assert enable at the same time)
    PWM_Clock_Manager->PWMCTL.DATA = CM_PWMCTL_PASSWD | CM_PWMCTL_ENAB | clock_source;

    while (PWM_Clock_Manager->PWMCTL.BUSY == 0u)
    {
        // wait for the clock to start
    }
}



void PSP_PWM_Channel_Start(PSP_PWM_Channel_t channel, PSP_PWM_Output_Mode_t mode, PSP_PWM_Range_t range)
{
    if (channel == PSP_PWM_Channel_1)
    {
        PWM->CTL.PWEN1 = 1u;
        PWM->CTL.MSEN1 = mode;
        PWM->RNG1      = range;
    }
    else
    {
        PWM->CTL.PWEN2 = 1u;
        PWM->CTL.MSEN2 = mode;
        PWM->RNG2      = range; 
    }
}



void PSP_PWM_Ch1_Write(uint32_t value)
{
    PWM->DAT1 = value;
}



void PSP_PWM_Ch2_Write(uint32_t value)
{
    PWM->DAT2 = value;
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
