#include "bsp.h"
#include "stm8l10x.h"

#define INT8U unsigned char
#define  PACKET_LENGTH      8 //0-64, if = 0: variable mode, else: fixed mode

INT8U SPI_ExchangeByte( INT8U input )
{
	SPI_SendData( input );
	while (SPI_GetFlagStatus(SPI_FLAG_RXNE) == RESET); //wait for receiving a byte
	while (SPI_GetFlagStatus(SPI_FLAG_TXE) == RESET);  //wait for the transmitting finished
	return SPI_ReceiveData( );
}

INT8U  buffer[8] = {0x7,0x7,0x11,0x22,0x33,0x44,0x00,0x20};

void main( )
{
    INT8U TxBuffer[10];
    
    CLK_MasterPrescalerConfig(CLK_MasterPrescaler_HSIDiv8);

    GPIOB->ODR = 0x11;
    GPIOB->IDR = 0x91;
    GPIOB->DDR = 0x71;
    GPIOB->CR1 = 0xF1;
    GPIOB->CR2 = 0x71;
    GPIO_SetBits( GPIOB, GPIO_Pin_4 );    
    GPIOC->ODR = 0x10;
    GPIOC->IDR = 0x70;
    GPIOC->DDR = 0x10;
    GPIOC->CR1 = 0x13;
    GPIOC->CR2 = 0x10;       
    GPIO_SetBits( GPIOC, GPIO_Pin_4 ); 
    CLK_PeripheralClockConfig(CLK_Peripheral_SPI, ENABLE); 
    CLK_PeripheralClockConfig(CLK_Peripheral_AWU, ENABLE);
    
    SPI->CR1 = 0x44;
    SPI->CR2 = 0x03;
    SPI->ICR = 0x00;
    SPI->SR  = 0x02;  
   
    AWU_Cmd(ENABLE);   
    AWU_Init(AWU_Timebase_2ms);
      
    GPIO_SetBits(GPIOB, GPIO_Pin_0);
    
    SI446X_RESET( );                                //SI446X 模块复位
    
    SI446X_CONFIG_INIT( );                          //寄存器初始化，寄存器来自WDS配置的头文件
    SI446X_SET_POWER( 0x40 );                       //将输出功率配置为最大

    
    GPIO_ResetBits(GPIOB, GPIO_Pin_0);
    
    
    SI446X_SEND_PACKET( buffer, PACKET_LENGTH, 0, 0 );
    halt();halt();halt();halt();
    SI446X_SEND_PACKET( buffer, PACKET_LENGTH, 0, 0 );
    halt();halt();halt();halt();
    
    while(1);
       
 
 
 /*   while( 1 )
    {
        do{   
            SI446X_INT_STATUS( TxBuffer );
        }while( !( TxBuffer[3] & ( 1<<5 ) ) );
    }
*/
}

