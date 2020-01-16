
#include "PSP_Time.h"
#include "PSP_REGS.h"

/*-----------------------------------------------------------------------------------------------
    Private PSP_Time Defines
 -------------------------------------------------------------------------------------------------*/

// Register Addresses
#define PSP_Time_BASE_ADDRESS (PSP_REGS_SYSCLK_BASE_ADDRESS)        // base address of System Timer Register

#define PSP_Time_CS_A         (PSP_Time_BASE_ADDRESS | 0x00000000u) // System Timer Control/Status address
#define PSP_Time_CLO_A        (PSP_Time_BASE_ADDRESS | 0x00000004u) // System Timer Counter Lower 32 bits address
#define PSP_Time_CHI_A        (PSP_Time_BASE_ADDRESS | 0x00000008u) // System Timer Counter Higher 32 bits address
#define PSP_Time_C0_A         (PSP_Time_BASE_ADDRESS | 0x0000000Cu) // System Timer Compare 0 address
#define PSP_Time_C1_A         (PSP_Time_BASE_ADDRESS | 0x00000010u) // System Timer Compare 1 address
#define PSP_Time_C2_A         (PSP_Time_BASE_ADDRESS | 0x00000014u) // System Timer Compare 2 address
#define PSP_Time_C3_A         (PSP_Time_BASE_ADDRESS | 0x00000018u) // System Timer Compare 3 address

// Register Pointers
#define PSP_Time_CS_R         (*((vuint32_t *)PSP_Time_CS_A))       // System Timer Control/Status register
#define PSP_Time_CLO_R        (*((vuint32_t *)PSP_Time_CLO_A))      // System Timer Counter Lower 32 bits register
#define PSP_Time_CHI_R        (*((vuint32_t *)PSP_Time_CHI_A))      // System Timer Counter Higher 32 bits register
#define PSP_Time_C0_R         (*((vuint32_t *)PSP_Time_C0_A))       // System Timer Compare 0 register
#define PSP_Time_C1_R         (*((vuint32_t *)PSP_Time_C1_A))       // System Timer Compare 1 register
#define PSP_Time_C2_R         (*((vuint32_t *)PSP_Time_C2_A))       // System Timer Compare 2 register
#define PSP_Time_C3_R         (*((vuint32_t *)PSP_Time_C3_A))       // System Timer Compare 3 register


/*-----------------------------------------------------------------------------------------------
    PSP_Time Function Definitions
 -------------------------------------------------------------------------------------------------*/

uint64_t PSP_Time_Get_Ticks(void)
{
    uint32_t CHI_reading = PSP_Time_CHI_R;
    uint32_t CLO_reading = PSP_Time_CLO_R;

    // if the upper 32 bit reading has changed, take new readings
    if (CHI_reading != PSP_Time_CHI_R)
    {
        CHI_reading = PSP_Time_CHI_R;
        CLO_reading = PSP_Time_CLO_R;
    }

    return (uint64_t)CHI_reading << 32u | CLO_reading;
}



void PSP_Time_Delay_Microseconds(const uint32_t delay_time_uSec)
{
    const uint64_t end_time = PSP_Time_Get_Ticks() + delay_time_uSec;

    while (PSP_Time_Get_Ticks() < end_time)
    {
        // wait
    }
}
