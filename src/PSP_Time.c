
#include "PSP_Time.h"


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

    return (uint64_t)CHI_reading << 32 | CLO_reading;
}



void PSP_Time_Delay_Microseconds(uint32_t delay_time_uSec)
{
    uint64_t start_time = PSP_Time_Get_Ticks();

    while (PSP_Time_Get_Ticks() < (start_time + delay_time_uSec))
    {
        // wait
    }
}
