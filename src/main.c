
#include "PSP_GPIO.h"
#include "PSP_Time.h"
#include "BSP_PWM.h"

#define LED_PIN 17u
#define SWITCH_PIN 21u
#define DELAY_TIME_uSec 1000u

int main()
{
    PSP_GPIO_Set_Pin_Mode(LED_PIN, PSP_GPIO_PINMODE_OUTPUT);
    PSP_GPIO_Set_Pin_Mode(SWITCH_PIN, PSP_GPIO_PINMODE_INPUT);

    BSP_PWM_Clock_Init();
    BSP_PWM_Ch1_Start();
    BSP_PWM_Ch1_Set_GPIO12_To_PWM_Mode();

    uint8_t pwm_val = 0;

    while(1)
    {
        if (PSP_GPIO_Read_Pin(SWITCH_PIN))
        {
            PSP_GPIO_Write_Pin(LED_PIN, PSP_GPIO_PIN_WRITE_HIGH);
            PSP_Time_Delay_Microseconds(DELAY_TIME_uSec);
            
            PSP_GPIO_Write_Pin(LED_PIN, PSP_GPIO_PIN_WRITE_LOW);
            PSP_Time_Delay_Microseconds(DELAY_TIME_uSec);

            BSP_PWM_Ch1_Write(pwm_val);
            pwm_val++;
        }
    }

    return 0;
}
