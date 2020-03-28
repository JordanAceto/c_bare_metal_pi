/*
--|----------------------------------------------------------------------------|
--| FILE DESCRIPTION:
--|   PSP_PWM.c provides the implementation for Pulse Width Modulation.
--|  
--|----------------------------------------------------------------------------|
--| REFERENCES:
--|   see PSP_PWM.h
--|
--|----------------------------------------------------------------------------|
*/

/*
--|----------------------------------------------------------------------------|
--| INCLUDE FILES
--|----------------------------------------------------------------------------|
*/

#include "PSP_Clock_Manager.h"
#include "PSP_GPIO.h"
#include "PSP_PWM.h"
#include "PSP_REGS.h"

/*
--|----------------------------------------------------------------------------|
--| PRIVATE DEFINES
--|----------------------------------------------------------------------------|
*/

/*
--| NAME: PWM
--| DESCRIPTION: pointer to the PWM structure
--| TYPE: PWM_t *
*/
#define PWM ((volatile PWM_t *)PSP_REGS_PWM_BASE_ADDRESS)

/*
--|----------------------------------------------------------------------------|
--| PRIVATE TYPES
--|----------------------------------------------------------------------------|
*/

/*
--| NAME: PWM_t
--| DESCRIPTION: structure for PWM registers
*/
typedef struct PWM_Type
{
    vuint32_t CTL;  // PWM Control
    vuint32_t STA;  // PWM Status
    vuint32_t DMAC; // PWM DMA Configuration
     uint32_t RESERVED_1;
    vuint32_t RNG1; // PWM Channel 1 Range
    vuint32_t DAT1; // PWM Channel 1 Data
    vuint32_t FIF1; // PWM FIFO Input
     uint32_t RESERVED_2;
    vuint32_t RNG2; // PWM Channel 2 Range
    vuint32_t DAT2; // PWM Channel 2 Data
} PWM_t;

/*
--| NAME: PWM_CTL_Flags_enum
--| DESCRIPTION: PWM control register flags
*/
typedef enum PWM_CTL_Flags_Enumeration
{   
    PWM_CTL_MSEN2_FLAG = (1u << 15u), // Channel 2 M/S Enable [rw]
    PWM_CTL_USEF2_FLAG = (1u << 13u), // Channel 2 Use Fifo [rw]
    PWM_CTL_POLA2_FLAG = (1u << 12u), // Channel 2 Polarity [rw]
    PWM_CTL_SBIT2_FLAG = (1u << 11u), // Channel 2 Silence Bit [rw]
    PWM_CTL_RPTL2_FLAG = (1u << 10u), // Channel 2 Repeat Last Data [rw]
    PWM_CTL_MODE2_FLAG = (1u << 9u),  // Channel 2 Mode [rw]
    PWM_CTL_PWEN2_FLAG = (1u << 8u),  // Channel 2 Enable [rw]
    PWM_CTL_MSEN1_FLAG = (1u << 7u),  // Channel 1 M/S Enable [rw]
    PWM_CTL_CLRF1_FLAG = (1u << 6u),  // Clear Fifo [r0, w]
    PWM_CTL_USEF1_FLAG = (1u << 5u),  // Channel 1 Use Fifo [rw]
    PWM_CTL_POLA1_FLAG = (1u << 4u),  // Channel 1 Polarity [rw]
    PWM_CTL_SBIT1_FLAG = (1u << 3u),  // Channel 1 Silence Bit [rw]
    PWM_CTL_RPTL1_FLAG = (1u << 2u),  // Channel 1 Repeat Last Data [rw]
    PWM_CTL_MODE1_FLAG = (1u << 1u),  // Channel 1 Mode [rw]
    PWM_CTL_PWEN1_FLAG = (1u << 0u),  // Channel 1 Enable [rw]
} PWM_CTL_Flags_enum;

/*
--| NAME: PWM_STA_Flags_enum
--| DESCRIPTION: PWM Status register flags
*/
typedef enum PWM_STA_Flags_Enumeration
{   
    PWM_STA_STA4_FLAG  = (1u << 12u), // Channel 4 State [rw]
    PWM_STA_STA3_FLAG  = (1u << 11u), // Channel 3 State [rw]
    PWM_STA_STA2_FLAG  = (1u << 10u), // Channel 2 State [rw]
    PWM_STA_STA1_FLAG  = (1u << 9u),  // Channel 1 State [rw]
    PWM_STA_BERR_FLAG  = (1u << 8u),  // Bus Error Flag [rw]
    PWM_STA_GAPO4_FLAG = (1u << 7u),  // Channel 4 Gap Occurred Flag [rw]
    PWM_STA_GAPO3_FLAG = (1u << 6u),  // Channel 3 Gap Occurred Flag [rw]
    PWM_STA_GAPO2_FLAG = (1u << 5u),  // Channel 2 Gap Occurred Flag [rw]
    PWM_STA_GAPO1_FLAG = (1u << 4u),  // Channel 1 Gap Occurred Flag [rw]
    PWM_STA_RERR1_FLAG = (1u << 3u),  // Fifo Read Error Flag [rw]
    PWM_STA_WERR1_FLAG = (1u << 2u),  // Fifo Write Error Flag [rw]
    PWM_STA_EMPT1_FLAG = (1u << 1u),  // Fifo Empty Flag [rw]
    PWM_STA_FULL1_FLAG = (1u << 0u),  // Fifo Full Flag [rw]
} PWM_STA_Flags_enum;

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

/* None */

/*
--|----------------------------------------------------------------------------|
--| PUBLIC FUNCTION DEFINITIONS
--|----------------------------------------------------------------------------|
*/

void PSP_PWM_Clock_Init_Default(void)
{
    PSP_PWM_Clock_Init(PWM_DEFAULT_CLOCK, PWM_DEFAULT_DIV);
}

void PSP_PWM_Clock_Init(PSP_PWM_Clock_Source_t clock_source, uint32_t divider)
{
    // request a clock stop
    PWM_Clock_Manager->CTL = CLOCK_MANAGER_PASSWORD |
                            (PWM_Clock_Manager->CTL & ~CLOCK_MANAGER_CTL_ENAB_FLAG);

    while (PWM_Clock_Manager->CTL & CLOCK_MANAGER_CTL_BUSY_FLAG)
    {
        // wait for the clock to stop
    }

    // set the integer part of divider
    PWM_Clock_Manager->DIV = CLOCK_MANAGER_PASSWORD | 
                            (divider << CLOCK_MANAGER_DIV_DIVI_SHIFT_AMT);

    // set the clock source
    PWM_Clock_Manager->CTL = CLOCK_MANAGER_PASSWORD |
                             clock_source;

    // request a clock start (datasheet says not to change the clock source and 
    // assert enable at the same time)
    PWM_Clock_Manager->CTL = CLOCK_MANAGER_PASSWORD | 
                             CLOCK_MANAGER_CTL_ENAB_FLAG | clock_source;

    while (!(PWM_Clock_Manager->CTL & CLOCK_MANAGER_CTL_BUSY_FLAG))
    {
        // wait for the clock to start
    }
}

void PSP_PWM_Channel_Start(PSP_PWM_Channel_t channel, PSP_PWM_Output_Mode_t mode, PSP_PWM_Range_t range)
{
    if (channel == PSP_PWM_Channel_1)
    {
        // shift the mode into the MSEN1 position and enable
        PWM->CTL |= (mode << 7u) | PWM_CTL_PWEN1_FLAG;
        PWM->RNG1 = range;
    }
    else
    {
        // shift the mode into the MSEN2 position and enable
        PWM->CTL |= (mode << 15u) | PWM_CTL_PWEN2_FLAG;
        PWM->RNG2 = range;   
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

/*
--|----------------------------------------------------------------------------|
--| PRIVATE HELPER FUNCTION DEFINITIONS
--|----------------------------------------------------------------------------|
*/

/* None */
