
#include "BSP_PWM.h"

#include "PSP_GPIO.h"

void BSP_PWM_Clock_Init(void)
{
    // request a clock stop
    BSP_CM_PWMCTL_R = CM_PWMCTL_PASSWD | (BSP_CM_PWMCTL_R & (~CM_PWMCTL_ENAB));

    while (BSP_CM_PWMCTL_R & CM_PWMCTL_BUSY)
    {
        // wait for the clock to stop
    }
    
    // set the divider, left shift of 12 pushes the divider setting into the integer part of the divider register
    BSP_CM_PWMDIV_R = CM_PWMCTL_PASSWD | (PWM_DEFAULT_DIV << 12);

    // use internal oscillator clock source
    BSP_CM_PWMCTL_R = CM_PWMCTL_PASSWD | CM_PWMCTL_USE_OSC;

    // request a clock start (datasheet says not to change the clock source and assert enable at the same time)
    BSP_CM_PWMCTL_R = CM_PWMCTL_PASSWD | CM_PWMCTL_ENAB | CM_PWMCTL_USE_OSC;

    while (!(BSP_CM_PWMCTL_R & CM_PWMCTL_BUSY))
    {
        // wait for the clock to start
    }
}



void BSP_PWM_Ch1_Start(void)
{
    // set pwm control for channel 1 to mark/space mode and enable, leave channel 2 alone
    BSP_PWM_CTL_R = BSP_PWM_CTL_R | (PWM_CTL_MSEN1 | PWM_CTL_PWEN1);

    // set the range to the default value
    BSP_PWM_RNG1_R = PWM_DEFAULT_RANGE;
}

void BSP_PWM_Ch2_Start(void)
{
    // set pwm control for channel 2 to mark/space mode and enable, leave channel 1 alone
    BSP_PWM_CTL_R = BSP_PWM_CTL_R | (PWM_CTL_MSEN2 | PWM_CTL_PWEN2);

    // set the range to the default value
    BSP_PWM_RNG2_R = PWM_DEFAULT_RANGE;
}

void BSP_PWM_Ch1_Write(uint32_t value)
{
    BSP_PWM_DAT1_R = value & PWM_RANGE_MASK;
}

void BSP_PWM_Ch2_Write(uint32_t value)
{
    BSP_PWM_DAT2_R = value & PWM_RANGE_MASK;
}

void BSP_PWM_Ch1_Set_GPIO12_To_PWM_Mode(void)
{
    PSP_GPIO_Set_Pin_Mode(12u, PSP_GPIO_PINMODE_ALT0);
}

void BSP_PWM_Ch1_Set_GPIO18_To_PWM_Mode(void)
{
    PSP_GPIO_Set_Pin_Mode(18u, PSP_GPIO_PINMODE_ALT5);
}

void BSP_PWM_Ch2_Set_GPIO13_To_PWM_Mode(void)
{
    PSP_GPIO_Set_Pin_Mode(13u, PSP_GPIO_PINMODE_ALT0);
}

void BSP_PWM_Ch2_Set_GPIO19_To_PWM_Mode(void)
{
    PSP_GPIO_Set_Pin_Mode(19u, PSP_GPIO_PINMODE_ALT5); 
}
