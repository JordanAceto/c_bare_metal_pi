
#include "BSP_Rotary_Encoder.h"
#include "PSP_GPIO.h"


 /*-----------------------------------------------------------------------------------------------
    Private BSP_Rotary_Encoder Variables
 -------------------------------------------------------------------------------------------------*/
/*
    To read a given encoder, its current pin A and B state as well as its last pin A and B states
    are maintained in a struct along with other information about the encoder.

    For any given transition of current/last pin states the encoder's counter should either
    count up, count down, or stay the same.

    The table of encoder-state to desired-counter-increment is below:

    (a0/b0 represent the last encoder state, and a1/b1 represent the current encoder state)

    a0  |  b0 |  a1 |  b1 |  result
    --------------------------------
     0  |  0  |  0  |  0  |    0
     0  |  0  |  0  |  1  |    1
     0  |  0  |  1  |  0  |   -1
     0  |  0  |  1  |  1  |    0

     0  |  1  |  0  |  0  |   -1
     0  |  1  |  0  |  1  |    0
     0  |  1  |  1  |  0  |    0
     0  |  1  |  1  |  1  |    1

     1  |  0  |  0  |  0  |    1
     1  |  0  |  0  |  1  |    0
     1  |  0  |  1  |  0  |    0
     1  |  0  |  1  |  1  |   -1

     1  |  1  |  0  |  0  |    0
     1  |  1  |  0  |  1  |   -1
     1  |  1  |  1  |  0  |    1
     1  |  1  |  1  |  1  |    0

    The information in the above table is stored in the STATE_TRANSITIONS array below,
    where the four-bit state of the encoder is used as the index into the array, and
    thre result is stored as the value at that index.

    Because the encoder state is stored in a union, we can use this to directly index
    into the below array, and retrieve the correct counter increment for the given
    encoder state.
*/
const int8_t STATE_TRANSITIONS[16u] = 
{
     0,  1, -1,  0,
    -1,  0,  0,  1,
    -1,  0,  0,  1,
     0, -1,  1,  0
};


 /*-----------------------------------------------------------------------------------------------
    BSP_Rotary_Encoder Function Definitions
 -------------------------------------------------------------------------------------------------*/

void BSP_Rotary_Encoder_Initialize(BSP_Rotary_Encoder_t * pEncoder)
{
    PSP_GPIO_Set_Pin_Mode(pEncoder->PIN_A, PSP_GPIO_PINMODE_INPUT);
    PSP_GPIO_Set_Pin_Mode(pEncoder->PIN_B, PSP_GPIO_PINMODE_INPUT);
    pEncoder->count = 0u;
    pEncoder->state.Raw_Data = 0u;
}


void BSP_Poll_Rotary_Encoder(BSP_Rotary_Encoder_t * pEncoder)
{
    // save the last state
    pEncoder->state.Last_Pin_A_State = pEncoder->state.Pin_A_State;
    pEncoder->state.Last_Pin_B_State = pEncoder->state.Pin_B_State;

    // update the new state
    pEncoder->state.Pin_A_State = PSP_GPIO_Read_Pin(pEncoder->PIN_A);
    pEncoder->state.Pin_B_State = PSP_GPIO_Read_Pin(pEncoder->PIN_B);

    // get the increment from the state transitions array and increment the counter
    pEncoder->count += STATE_TRANSITIONS[pEncoder->state.Raw_Data];
}
