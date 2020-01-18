
#include "PSP_I2C.h"
#include "PSP_GPIO.h"
#include "PSP_REGS.h"

/*-----------------------------------------------------------------------------------------------
    Private PSP_I2C Defines
 -------------------------------------------------------------------------------------------------*/

// I2C Register Addresses
#define PSP_I2C_BASE_A     (PSP_REGS_I2C_BASE_ADDRESS)
#define PSP_I2C_C_A        (PSP_I2C_BASE_A | 0x00000000u)   // control register address
#define PSP_I2C_S_A        (PSP_I2C_BASE_A | 0x00000004u)   // status register address
#define PSP_I2C_DLEN_A     (PSP_I2C_BASE_A | 0x00000008u)   // data length register address
#define PSP_I2C_SA_A       (PSP_I2C_BASE_A | 0x0000000Cu)   // slave address register address
#define PSP_I2C_FIFO_A     (PSP_I2C_BASE_A | 0x00000010u)   // data FIFO register address
#define PSP_I2C_DIV_A      (PSP_I2C_BASE_A | 0x00000014u)   // clock divider register address
#define PSP_I2C_DEL_A      (PSP_I2C_BASE_A | 0x00000018u)   // data delay register address
#define PSP_I2C_CLKT_A     (PSP_I2C_BASE_A | 0x0000001Cu)   // clock stretch timeout register address

// I2C Register Pointers
#define PSP_I2C_C_R        (*((vuint32_t *)PSP_I2C_C_A))    // control register
#define PSP_I2C_S_R        (*((vuint32_t *)PSP_I2C_S_A))    // status register
#define PSP_I2C_DLEN_R     (*((vuint32_t *)PSP_I2C_DLEN_A)) // data length register
#define PSP_I2C_SA_R       (*((vuint32_t *)PSP_I2C_SA_A))   // slave address register
#define PSP_I2C_FIFO_R     (*((vuint32_t *)PSP_I2C_FIFO_A)) // data FIFO register
#define PSP_I2C_DIV_R      (*((vuint32_t *)PSP_I2C_DIV_A))  // clock divider register
#define PSP_I2C_DEL_R      (*((vuint32_t *)PSP_I2C_DEL_A))  // data delay register
#define PSP_I2C_CLKT_R     (*((vuint32_t *)PSP_I2C_CLKT_A)) // clock stretch timeout register

// masks for I2C control register
#define I2C_C_I2CEN        0x00008000u // I2C Enable, 0 = disabled, 1 = enabled
#define I2C_C_INTR         0x00000400u // Interrupt on RX
#define I2C_C_INTT         0x00000200u // Interrupt on TX
#define I2C_C_INTD         0x00000100u // Interrupt on DONE
#define I2C_C_ST           0x00000080u // Start transfer, 1 = Start a new transfer
#define I2C_C_CLEAR_1      0x00000020u // Clear FIFO Clear
#define I2C_C_CLEAR_2      0x00000010u // Clear FIFO Clear
#define I2C_C_READ         0x00000001u // Read transfer

// masks for I2C status register
#define I2C_S_CLKT         0x00000200u // Clock stretch timeout
#define I2C_S_ERR          0x00000100u // 0 = No errors detected. 1 = Slave has not acknowledged its address.
#define I2C_S_RXF          0x00000080u // RXF FIFO full, 0 = FIFO is not full, 1 = FIFO is full
#define I2C_S_TXE          0x00000040u // TXE FIFO full, 0 = FIFO is not full, 1 = FIFO is full
#define I2C_S_RXD          0x00000020u // RXD FIFO contains data
#define I2C_S_TXD          0x00000010u // TXD FIFO can accept data
#define I2C_S_RXR          0x00000008u // RXR FIFO needs reading (full)
#define I2C_S_TXW          0x00000004u // TXW FIFO needs writing (full)
#define I2C_S_DONE         0x00000002u // Transfer DONE
#define I2C_S_TA           0x00000001u // Transfer Active



/*-----------------------------------------------------------------------------------------------
    Private PSP_I2C Types
 -------------------------------------------------------------------------------------------------*/


/* I2C Control Register Type */
typedef union I2C_Control_Reg_Type
{
    struct
    {
        vuint32_t READ  : 1u;  // Read Transfer. 0 = Write Packet Transfer. 1 = Read Packet Transfer.
        vuint32_t       : 3u;  // Reserved - Write as 0, read as don't care
        vuint32_t CLEAR : 2u;  // Clear FIFO. 00 = No action. x1 = Clear FIFO. 1x = Clear FIFO
        vuint32_t       : 1u;  // Reserved - Write as 0, read as don't care
        vuint32_t ST    : 1u;  // Start Transfer 0 = No action. 1 = Start a new transfer.
        vuint32_t INTD  : 1u;  // Interrupt on DONE. 0 = Don t generate interrupts on DONE condition. 1 = do.
        vuint32_t INTT  : 1u;  // Interrupt on TX. 0 = Don t generate interrupts on TXW condition. 1 = do.
        vuint32_t INTR  : 1u;  // Interrupt on RX. 0 = Don t generate interrupts on RXR condition. 1 = do.
        vuint32_t       : 4u;  // Reserved - Write as 0, read as don't care
        vuint32_t I2CEN : 1u;  // I2C Enable. 0 = BSC controller is disabled. 1 = BSC controller is enabled
        vuint32_t       : 16u; // Reserved - Write as 0, read as don't care
    };
    
    vuint32_t DATA; // Read/write the entire 32 bit word at once.

} I2C_Control_Register_t;



/* I2C Status Register Type */
typedef union I2C_Status_Reg_Type
{
    struct
    {
        vuint32_t TA   : 1u;  // Transfer Active. 0 = Transfer not active. 1 = Transfer active.
        vuint32_t DONE : 1u;  // Transfer Done. 0 = not done. 1 = done. Cleared by writing 1 to the field.
        vuint32_t TXW  : 1u;  // FIFO needs Writing.
        vuint32_t RXR  : 1u;  // FIFO needs Reading. 
        vuint32_t TXD  : 1u;  // FIFO can accept Data. 0 = FIFO is full. 1 = FIFO has space for at least 1 byte.
        vuint32_t RXD  : 1u;  // FIFO contains Data. 0 = FIFO is empty. 1 = FIFO contains at least 1 byte.
        vuint32_t TXE  : 1u;  // FIFO Empty. 0 = FIFO is not empty. 1 = FIFO is empty.
        vuint32_t RXF  : 1u;  // FIFO Full. 0 = FIFO is not full. 1 = FIFO is full.
        vuint32_t ERR  : 1u;  // ACK Error. 0 = No errors detected. 1 = Slave has not acknowledged its address.
        vuint32_t CLKT : 1u;  // Clock Stretch Timeout. 0 = No errors detected. 1 = clock stretch timeout error.
        vuint32_t      : 22u; // Reserved - Write as 0, read as don't care
    };
    
    vuint32_t DATA; // Read/write the entire 32 bit word at once.

} I2C_Status_Register_t;



/* I2C Data Length Register Type */
typedef union I2C_DLEN_Reg_Type
{
    struct
    {
        vuint32_t DATA     : 16u; // Data Length. The number of bytes to transfer.
        vuint32_t RESERVED : 16u; // Reserved - Write as 0, read as don't care
    };
    
} I2C_DLEN_Regsiter_t;



/* I2C Slave Address Register Type */
typedef union I2C_Address_Reg_Type
{
    struct
    {
        vuint32_t DATA     :  7u; // Slave Address.
        vuint32_t RESERVED : 25u; // Reserved - Write as 0, read as don't care
    };
    
} I2C_Address_Regsiter_t;



/* I2C FIFO Register Type */
typedef union I2C_FIFO_Reg_Type
{
    struct
    {
        vuint32_t DATA     :  8u; // 16 byte FIFO
        vuint32_t RESERVED : 24u; // Reserved - Write as 0, read as don't care
    };
    
} I2C_FIFO_Regsiter_t;



/* I2C DIV Register Type */
typedef union I2C_DIV_Reg_Type
{
    struct
    {
        vuint32_t CDIV     : 16u; // Clock Divider. SCL = core clock / CDIV Where core_clk is nominally 150 MHz.
        vuint32_t RESERVED : 16u; // Reserved - Write as 0, read as don't care
    };
    
} I2C_DIV_Regsiter_t;



/* I2C DEL Register Type */
typedef union I2C_DEL_Reg_Type
{
    struct
    {
        vuint32_t REDL : 16u; // Falling Edge Delay. Num clock cycles to wait after rising edge of SCL to output data.
        vuint32_t FEDL : 16u; // Falling Edge Delay. Num clock cycles to wait after falling edge of SCL to output data.
    };
    
} I2C_DEL_Regsiter_t;



/* I2C CLKT Register Type */
typedef union I2C_CLKT_Reg_Type
{
    struct
    {
        vuint32_t TOUT     : 16u; // Clock Stretch Timeout Value.
        vuint32_t RESERVED : 16u; // Reserved - Write as 0, read as don't care
    };
    
} I2C_CLKT_Regsiter_t;



typedef struct I2C_Type
{
    I2C_Control_Register_t C;    // Control Register
    I2C_Status_Register_t S;     // Status Register
    I2C_DLEN_Regsiter_t DLEN;    // Data Length Register 
    I2C_Address_Regsiter_t ADDR; // Slave Address Register
    I2C_FIFO_Regsiter_t FIFO;    // FIFO Register
    I2C_DIV_Regsiter_t DIV;      // Clock Divider Register
    I2C_DEL_Regsiter_t DEL;      // Data Delay Register
    I2C_CLKT_Regsiter_t CLKT;    // Clock Stretch Timout Register
} I2C_t;



/*-----------------------------------------------------------------------------------------------
    Private PSP_I2C Variables
 -------------------------------------------------------------------------------------------------*/

volatile I2C_t * I2C = (volatile I2C_t *)PSP_I2C_BASE_A;



/*-----------------------------------------------------------------------------------------------
    PSP_I2C Function Definitions
 -------------------------------------------------------------------------------------------------*/


void PSP_I2C_Start(void)
{
    PSP_GPIO_Set_Pin_Mode(I2C_SDA_PIN, PSP_GPIO_PINMODE_ALT0);
    PSP_GPIO_Set_Pin_Mode(I2C_SCL_PIN, PSP_GPIO_PINMODE_ALT0);
}



 void PSP_I2C_End(void)
 {
    PSP_GPIO_Set_Pin_Mode(I2C_SDA_PIN, PSP_GPIO_PINMODE_INPUT);
    PSP_GPIO_Set_Pin_Mode(I2C_SCL_PIN, PSP_GPIO_PINMODE_INPUT);
 }



void PSP_I2C_Set_Clock_Divider(uint32_t divider)
{
    I2C->DIV.CDIV = divider;
}



void PSP_I2C_Set_Slave_Address(uint32_t address)
{
    I2C->ADDR.DATA = address;
}



void PSP_I2C_Write_Byte(uint8_t val)
{
    // clear the fifo
    I2C->C.CLEAR = 1u;

    // clear the clock stretch timeout, no acknowledge error, and transfer done status flags 
    // note that these flags are cleared by writing a 1
    I2C->S.CLKT = 1u;
    I2C->S.ERR  = 1u;
    I2C->S.DONE = 1u;
    
    // set data length to 1 byte
    I2C->DLEN.DATA = 1u;

    // write the value to the fifo
    I2C->FIFO.DATA = val;
    
    // enable device and start transfer
    I2C->C.I2CEN = 1u;
    I2C->C.ST    = 1u;

    while (I2C->S.DONE == 0u)
    {
        // wait for transfer to complete
    }

    // set the done flag inorder to clear it and end the transfer
    I2C->S.DONE = 1u; // is this necessary?
}
