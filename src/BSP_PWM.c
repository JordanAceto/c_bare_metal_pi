
#include "BSP_PWM.h"

#include "PSP_GPIO.h"



void BSP_PWM_Clock_Init_Default(void)
{
    BSP_PWM_Clock_Init(PWM_DEFAULT_CLOCK, PWM_DEFAULT_DIV);
}



void BSP_PWM_Clock_Init(BSP_PWM_Clock_Source_t clock_source, uint32_t divider)
{
    // request a clock stop
    BSP_CM_PWMCTL_R = CM_PWMCTL_PASSWD | (BSP_CM_PWMCTL_R & (~CM_PWMCTL_ENAB));

    while (BSP_CM_PWMCTL_R & CM_PWMCTL_BUSY)
    {
        // wait for the clock to stop
    }
    
    // set the divider, left shift of 12 pushes the divider setting into the integer part of the divider register
    BSP_CM_PWMDIV_R = CM_PWMCTL_PASSWD | (divider << 12);

    // use internal oscillator clock source
    BSP_CM_PWMCTL_R = CM_PWMCTL_PASSWD | clock_source;

    // request a clock start (datasheet says not to change the clock source and assert enable at the same time)
    BSP_CM_PWMCTL_R = CM_PWMCTL_PASSWD | CM_PWMCTL_ENAB | clock_source;

    while (!(BSP_CM_PWMCTL_R & CM_PWMCTL_BUSY))
    {
        // wait for the clock to start
    }
}



void BSP_PWM_Channel_Start(BSP_PWM_Channel_t channel, BSP_PWM_Output_Mode_t mode, BSP_PWM_Range_t range)
{
    if (channel == BSP_PWM_Channel_1)
    {
        // shift the mode into the MSEN1 position
        BSP_PWM_CTL_R = 0xFF00u | (mode << 7u) | PWM_CTL_PWEN1;
        BSP_PWM_RNG1_R = range;
    }
    else
    {
        // shift the mode into the MSEN2 position
        BSP_PWM_CTL_R = 0x00FFu | (mode << 15u) | PWM_CTL_PWEN2;
        BSP_PWM_RNG2_R = range;   
    }
}



void BSP_PWM_Ch1_Write(uint32_t value)
{
    BSP_PWM_DAT1_R = value;
}



void BSP_PWM_Ch2_Write(uint32_t value)
{
    BSP_PWM_DAT2_R = value;
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
