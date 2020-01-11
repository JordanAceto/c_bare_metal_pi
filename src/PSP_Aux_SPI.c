
#include "PSP_Aux_SPI.h"
#include "PSP_REGS.h"
#include "PSP_GPIO.h"

/*------------------------------------------------------------------------------------------------
    Private PSP_Auxiliaries Defines
 -------------------------------------------------------------------------------------------------*/

// AUX Register Addresses
#define PSP_AUX_BASE_ADDRESS      (PSP_REGS_AUX_BASE_ADDRESS)

#define PSP_AUX_IRQ_A             (PSP_AUX_BASE_ADDRESS | 0x00000000u) // Auxiliary Interrupt status address
#define PSP_AUX_ENABLES_A         (PSP_AUX_BASE_ADDRESS | 0x00000004u) // Auxiliary enables address

#define PSP_AUX_SPI_1_CNTL0_REG_A (PSP_AUX_BASE_ADDRESS | 0x00000080u) // SPI 1 Control register 0 address
#define PSP_AUX_SPI_1_CNTL1_REG_A (PSP_AUX_BASE_ADDRESS | 0x00000084u) // SPI 1 Control register 1 address
#define PSP_AUX_SPI_1_STAT_REG_A  (PSP_AUX_BASE_ADDRESS | 0x00000088u) // SPI 1 Status address
#define PSP_AUX_SPI_1_IO_REG_A    (PSP_AUX_BASE_ADDRESS | 0x00000090u) // SPI 1 Data address
#define PSP_AUX_SPI_1_PEEK_REG_A  (PSP_AUX_BASE_ADDRESS | 0x00000094u) // SPI 1 Peek address

#define PSP_AUX_SPI_2_CNTL0_REG_A (PSP_AUX_BASE_ADDRESS | 0x000000C0u) // SPI 2 Control register 0 address
#define PSP_AUX_SPI_2_CNTL1_REG_A (PSP_AUX_BASE_ADDRESS | 0x000000C4u) // SPI 2 Control register address
#define PSP_AUX_SPI_2_STAT_REG_A  (PSP_AUX_BASE_ADDRESS | 0x000000C8u) // SPI 2 Status address
#define PSP_AUX_SPI_2_IO_REG_A    (PSP_AUX_BASE_ADDRESS | 0x000000D0u) // SPI 2 Data address
#define PSP_AUX_SPI_2_PEEK_REG_A  (PSP_AUX_BASE_ADDRESS | 0x000000D4u) // SPI 2 Peek address

// AUX Register Pointers
#define PSP_AUX_IRQ_R             (*((vuint32_t *)PSP_AUX_IRQ_A))             // Auxiliary Interrupt status register
#define PSP_AUX_ENABLES_R         (*((vuint32_t *)PSP_AUX_ENABLES_A))         // Auxiliary enables register

#define PSP_AUX_SPI_1_CNTL0_REG_R (*((vuint32_t *)PSP_AUX_SPI_1_CNTL0_REG_A)) // SPI 1 Control register 0 register
#define PSP_AUX_SPI_1_CNTL1_REG_R (*((vuint32_t *)PSP_AUX_SPI_1_CNTL1_REG_A)) // SPI 1 Control register 1 register
#define PSP_AUX_SPI_1_STAT_REG_R  (*((vuint32_t *)PSP_AUX_SPI_1_STAT_REG_A))  // SPI 1 Status register
#define PSP_AUX_SPI_1_IO_REG_R    (*((vuint32_t *)PSP_AUX_SPI_1_IO_REG_A))    // SPI 1 Data register
#define PSP_AUX_SPI_1_PEEK_REG_R  (*((vuint32_t *)PSP_AUX_SPI_1_PEEK_REG_A))  // SPI 1 Peek register

#define PSP_AUX_SPI_2_CNTL0_REG_R (*((vuint32_t *)PSP_AUX_SPI_2_CNTL0_REG_A)) // SPI 2 Control register 0 register
#define PSP_AUX_SPI_2_CNTL1_REG_R (*((vuint32_t *)PSP_AUX_SPI_2_CNTL1_REG_A)) // SPI 2 Control register register
#define PSP_AUX_SPI_2_STAT_REG_R  (*((vuint32_t *)PSP_AUX_SPI_2_STAT_REG_A))  // SPI 2 Status register
#define PSP_AUX_SPI_2_IO_REG_R    (*((vuint32_t *)PSP_AUX_SPI_2_IO_REG_A))    // SPI 2 Data address
#define PSP_AUX_SPI_2_PEEK_REG_R  (*((vuint32_t *)PSP_AUX_SPI_2_PEEK_REG_A))  // SPI 2 Peek address

// AUX IRQ Register Masks
#define AUX_SPI_1_IRQ        0b010u // If set the AUX SPI 1 module has an interrupt pending
#define AUX_SPI_2_IRQ        0b100u // If set the AUX SPI 2 module has an interrupt pending

// AUX Enable Register masks
#define AUX_SPI_1_ENABLE     0b010u // If set the AUX SPI 1 module is enabled
#define AUX_SPI_2_ENABLE     0b100u // If set the AUX SPI 2 module is enabled


/*-----------------------------------------------------------------------------------------------
    Private PSP_Aux_SPI Variables
 -------------------------------------------------------------------------------------------------*/

volatile PSP_Aux_SPI_CNTL0_REG_t * SPI_1_CNTL0 = (volatile PSP_Aux_SPI_CNTL0_REG_t *)(PSP_AUX_SPI_1_CNTL0_REG_A);
volatile PSP_Aux_SPI_CNTL1_REG_t * SPI_1_CNTL1 = (volatile PSP_Aux_SPI_CNTL1_REG_t *)(PSP_AUX_SPI_1_CNTL1_REG_A);
volatile PSP_Aux_SPI_STAT_REG_t  * SPI_2_STAT  = (volatile PSP_Aux_SPI_STAT_REG_t *)(PSP_AUX_SPI_1_STAT_REG_A);


/*-----------------------------------------------------------------------------------------------
    PSP_Aux_SPI Function Definitions
 -------------------------------------------------------------------------------------------------*/

void PSP_SPI1_Start(void)
{
    PSP_GPIO_Set_Pin_Mode(PSP_SPI_1_CE2_PIN,  PSP_GPIO_PINMODE_ALT4);
    PSP_GPIO_Set_Pin_Mode(PSP_SPI_1_CE1_PIN,  PSP_GPIO_PINMODE_ALT4);
    PSP_GPIO_Set_Pin_Mode(PSP_SPI_1_CE0_PIN,  PSP_GPIO_PINMODE_ALT4);
    PSP_GPIO_Set_Pin_Mode(PSP_SPI_1_MISO_PIN, PSP_GPIO_PINMODE_ALT4);
    PSP_GPIO_Set_Pin_Mode(PSP_SPI_1_MOSI_PIN, PSP_GPIO_PINMODE_ALT4);
    PSP_GPIO_Set_Pin_Mode(PSP_SPI_1_CLK_PIN,  PSP_GPIO_PINMODE_ALT4);

    // enable the mini uart
    PSP_AUX_ENABLES_R |= AUX_SPI_1_ENABLE;

    SPI_1_CNTL0->Enable = 1u;
    
    SPI_1_CNTL0->Clear_FIFOs = 1u;

    // SPI_1_CNTL0->Speed = 1280u; // rough guess to get 100k clock speed

    SPI_1_CNTL0->chip_selects = AUX_SPI_USE_CS_0;
}


void PSP_SPI1_End(void)
{
    PSP_GPIO_Set_Pin_Mode(PSP_SPI_1_CE2_PIN,  PSP_GPIO_PINMODE_INPUT);
    PSP_GPIO_Set_Pin_Mode(PSP_SPI_1_CE1_PIN,  PSP_GPIO_PINMODE_INPUT);
    PSP_GPIO_Set_Pin_Mode(PSP_SPI_1_CE0_PIN,  PSP_GPIO_PINMODE_INPUT);
    PSP_GPIO_Set_Pin_Mode(PSP_SPI_1_MISO_PIN, PSP_GPIO_PINMODE_INPUT);
    PSP_GPIO_Set_Pin_Mode(PSP_SPI_1_MOSI_PIN, PSP_GPIO_PINMODE_INPUT);
    PSP_GPIO_Set_Pin_Mode(PSP_SPI_1_CLK_PIN,  PSP_GPIO_PINMODE_INPUT);

    SPI_1_CNTL0->Enable = 0u;
}

// if I only knew what I was doing...
uint8_t PSP_SPI1_Transfer_Byte(uint8_t val)
{
    // clear the FIFOs
    SPI_1_CNTL0->Clear_FIFOs = 1u;
    SPI_1_CNTL0->Clear_FIFOs = 0u;

    SPI_1_CNTL0->Shift_length = 8u;

    // write to the FIFO
    PSP_AUX_SPI_1_IO_REG_R = val;

    while (!SPI_2_STAT->TX_Empty)
    {
        // wait for transfer to complete
    }
    
    return 0u;
}
