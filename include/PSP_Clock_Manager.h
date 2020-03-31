/*
--|----------------------------------------------------------------------------|
--| FILE DESCRIPTION:
--|   PSP_Clock_Manager.h provides types and interfaces for the General 
--|   Purpose GPIO/PWM clock managers.
--|  
--|----------------------------------------------------------------------------|
--| REFERENCES:
--|   BCM2837-ARM-Peripherals.pdf page 105
--|
--|   https://elinux.org/BCM2835_datasheet_errata#p105_table (more information 
--|   about the Clock Manager)
--|
--|----------------------------------------------------------------------------|
*/

#ifndef TEMPLATE_H_INCLUDED
#define TEMPLATE_H_INCLUDED

/*
--|----------------------------------------------------------------------------|
--| INCLUDE FILES
--|----------------------------------------------------------------------------|
*/

#include "Fixed_Width_Ints.h"
#include "PSP_REGS.h"

/*
--|----------------------------------------------------------------------------|
--| PUBLIC DEFINES
--|----------------------------------------------------------------------------|
*/

/*
--| NAME: CLOCK_MANAGER_PASSWORD
--| DESCRIPTION: Clock Manager password for CTL and DIV registers
--| TYPE: uint32_t
*/
#define CLOCK_MANAGER_PASSWORD (0x5A000000u)

/*
--| NAME: PWM_Clock_Manager
--| DESCRIPTION: pointer to the PWM Clock Manager structure
--| TYPE: Clock_Manager_t *
*/
#define PWM_Clock_Manager ((volatile Clock_Manager_t *)PSP_REGS_PWM_CLK_MAN_BASE_ADDRESS)

/*
--|----------------------------------------------------------------------------|
--| PUBLIC TYPES
--|----------------------------------------------------------------------------|
*/

/*
--| NAME: Clock_Manager_t
--| DESCRIPTION: structure for General Purpose Clock Manager registers
*/
typedef struct Clock_Manager_Type
{
    vuint32_t CTL; // Clock Manager General Purpose Clocks Control
    vuint32_t DIV; // Clock Manager General Purpose Clock Divisors
} Clock_Manager_t;

/*
--| NAME: Clock_Manager_CTL_Flags_enum
--| DESCRIPTION: Clock Manager Control register flags
*/
typedef enum Clock_Manager_CTL_Flags_Enumeration
{
    CLOCK_MANAGER_CTL_FLIP_FLAG = (1u << 8u), // Invert the clock generator output [rw]
    CLOCK_MANAGER_CTL_BUSY_FLAG = (1u << 7u), // Clock generator is running [r]
    CLOCK_MANAGER_CTL_KILL_FLAG = (1u << 5u), // Kill the clock generator [rw]
    CLOCK_MANAGER_CTL_ENAB_FLAG = (1u << 4u), // Enable the clock generator [rw]
} Clock_Manager_CTL_Flags_enum;

/*
--| NAME: Clock_Manager_MASH_Masks_Enumeration
--| DESCRIPTION: Clock Manager MASH masks [2 bits, rw]
*/
typedef enum Clock_Manager_MASH_Masks_Enumeration
{
    CLOCK_MANAGER_MASH_INTEGER_DIVISION = 0b00u, // integer division
    CLOCK_MANAGER_MASH_1_STAGE_MASH     = 0b01u, // 1-stage MASH (equivalent to non-MASH dividers)
    CLOCK_MANAGER_MASH_2_STAGE_MASH     = 0b10u, // 2-stage MASH
    CLOCK_MANAGER_MASH_3_STAGE_MASH     = 0b11u, // 3-stage MASH
    CLOCK_MANAGER_MASH_SHIFT_AMT        = 9u,    // position of MASH in PWM CMCTL
} Clock_Manager_MASH_Masks_enum;

/*
--| NAME: Clock_Manager_SRC_Masks_Enumeration
--| DESCRIPTION: Clock Manager SRC masks [4 bits, rw]
*/
typedef enum Clock_Manager_SRC_Masks_Enumeration
{
    CLOCK_MANAGER_SRC_GND        = 0b0000u, // GND
    CLOCK_MANAGER_SRC_OSCILLATOR = 0b0001u, // oscillator
    CLOCK_MANAGER_SRC_TESTDEBUG0 = 0b0010u, // testdebug0
    CLOCK_MANAGER_SRC_TESTDEBUG1 = 0b0011u, // testdebug1
    CLOCK_MANAGER_SRC_PLLA       = 0b0100u, // PLLA per
    CLOCK_MANAGER_SRC_PLLC       = 0b0101u, // PLLC per
    CLOCK_MANAGER_SRC_PLLD       = 0b0110u, // PLLD per
    CLOCK_MANAGER_SRC_HDMI_AUX   = 0b0111u, // HDMI auxiliary
    CLOCK_MANAGER_SRC_SHIFT_AMT  = 0u,      // position of SRC in PWM CMCTL
} Clock_Manager_SRC_Masks_enum;

/*
--| NAME: Clock_Manager_DIV_DIVI_Masks_Enumeration
--| DESCRIPTION: Clock Manager Divider register DIVI masks [12 bits, w]
*/
typedef enum Clock_Manager_DIVI_Masks_Enumeration
{
    CLOCK_MANAGER_DIV_DIVI_MASK      = 0xFFFu, // DIVI takes up 12 bits
    CLOCK_MANAGER_DIV_DIVI_SHIFT_AMT = 12u,    // position of DIVI in CMDIV
} Clock_Manager_DIV_DIVI_Masks_enum;

/*
--| NAME: Clock_Manager_DIV_DIVF_Masks_Enumeration
--| DESCRIPTION: Clock Manager Divider register DIVF masks [12 bits, w]
*/
typedef enum Clock_Manager_DIVF_Masks_Enumeration
{
    CLOCK_MANAGER_DIV_DIVF_MASK      = 0xFFFu, // DIVI takes up 12 bits
    CLOCK_MANAGER_DIV_DIVF_SHIFT_AMT = 0u,     // position of DIVF in CMDIV
} Clock_Manager_DIV_DIVF_Masks_enum;

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
