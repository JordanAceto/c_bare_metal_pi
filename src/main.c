
#include "PSP_GPIO.h"
#include "PSP_Time.h"
#include "BSP_PWM.h"
#include "BSP_SPI_0.h"

#define LED_PIN 17u
#define SWITCH_PIN 21u
#define DELAY_TIME_uSec 1000u
#define PWM_CLOCK_DIVIDER 2u

int main()
{
    PSP_GPIO_Set_Pin_Mode(LED_PIN, PSP_GPIO_PINMODE_OUTPUT);
    PSP_GPIO_Set_Pin_Mode(SWITCH_PIN, PSP_GPIO_PINMODE_INPUT);

    BSP_PWM_Clock_Init(BSP_PWM_Clock_Source_OSCILLATOR, PWM_CLOCK_DIVIDER);
    BSP_PWM_Channel_Start(BSP_PWM_Channel_1, BSP_PWM_MARK_SPACE_MODE, BSP_PWM_RANGE_10_BITS);
    BSP_PWM_Ch1_Set_GPIO12_To_PWM_Mode();

    BSP_SPI0_Start();
    BSP_SPI0_Set_Clock_Divider(BSP_SPI0_Clock_Divider_1024);

    uint32_t pwm_val = 128;

    while(1)
    {
        if (PSP_GPIO_Read_Pin(SWITCH_PIN))
        {
            PSP_GPIO_Write_Pin(LED_PIN, PSP_GPIO_PIN_WRITE_HIGH);
            PSP_Time_Delay_Microseconds(DELAY_TIME_uSec);
            
            PSP_GPIO_Write_Pin(LED_PIN, PSP_GPIO_PIN_WRITE_LOW);
            PSP_Time_Delay_Microseconds(DELAY_TIME_uSec);

            // write a ramp wave via PWM
            BSP_PWM_Ch1_Write(pwm_val);
            pwm_val++;
            pwm_val %= BSP_PWM_RANGE_10_BITS;
            
            BSP_SPI0_Transfer_Byte(0xBEu);
            BSP_SPI0_Transfer_Byte(0xEFu);

            BSP_SPI0_Transfer_Byte(0xF0u);
            BSP_SPI0_Transfer_Byte(0x0Du);
        }
    }

    return 0;
}
