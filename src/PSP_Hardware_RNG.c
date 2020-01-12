
#include "PSP_Hardware_RNG.h"
#include "PSP_REGS.h"

/*------------------------------------------------------------------------------------------------
    Private PSP_Hardware_RNG Defines
 -------------------------------------------------------------------------------------------------*/

#define PSP_Hardware_RNG_BASE_ADDRESS   (PSP_REGS_HARDWARE_RNG_BASE_ADDRESS)

// RNG Register Addresses
#define PSP_Hardware_RNG_CTRL_A         (PSP_Hardware_RNG_BASE_ADDRESS)
#define PSP_Hardware_RNG_STATUS_A       (PSP_Hardware_RNG_BASE_ADDRESS | 0x00000004u)
#define PSP_Hardware_RNG_DATA_A         (PSP_Hardware_RNG_BASE_ADDRESS | 0x00000008u)
#define PSP_Hardware_RNG_FF_THRESHOLD_A (PSP_Hardware_RNG_BASE_ADDRESS | 0x0000000Cu)
#define PSP_Hardware_RNG_INT_MASK_A     (PSP_Hardware_RNG_BASE_ADDRESS | 0x00000010u)

// RNG Register Pointers
#define PSP_Hardware_RNG_CTRL_R         (*((vuint32_t *)PSP_Hardware_RNG_CTRL_A))
#define PSP_Hardware_RNG_STATUS_R       (*((vuint32_t *)PSP_Hardware_RNG_STATUS_A))
#define PSP_Hardware_RNG_DATA_R         (*((vuint32_t *)PSP_Hardware_RNG_DATA_A))
#define PSP_Hardware_RNG_FF_THRESHOLD_R (*((vuint32_t *)PSP_Hardware_RNG_FF_THRESHOLD_A))
#define PSP_Hardware_RNG_INT_MASK_R     (*((vuint32_t *)PSP_Hardware_RNG_INT_MASK_A))


/*------------------------------------------------------------------------------------------------
    PSP_Hardware_RNG Function Definitions
 -------------------------------------------------------------------------------------------------*/


void PSP_Hardware_RNG_Init(void)
{
    PSP_Hardware_RNG_STATUS_R = 0x00040000u; // mystery initialization constant
    PSP_Hardware_RNG_INT_MASK_R |= 0x1u;     // mask interrupt
    PSP_Hardware_RNG_CTRL_R |= 0x1u;         // enable

    while (!(PSP_Hardware_RNG_STATUS_R >> 24u))
    {
        // wait for entropy to develop
    }
}


uint32_t PSP_Hardware_RNG_Get_Random(void)
{
    return (uint32_t)PSP_Hardware_RNG_DATA_R;
}
