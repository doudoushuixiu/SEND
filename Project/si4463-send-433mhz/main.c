#include "bsp.h"
#include "stm8l10x.h"
#include "radio_config_Si446x_1k_26M.h"

#define INT8U unsigned char
#define  PACKET_LENGTH      8 //0-64, if = 0: variable mode, else: fixed mode

#define SI_CSN_LOW( )   GPIO_ResetBits( GPIOB, GPIO_Pin_4 );
#define SI_CSN_HIGH( )  GPIO_SetBits( GPIOB, GPIO_Pin_4 );

#define SI_SDN_LOW( )   GPIO_ResetBits( GPIOC, GPIO_Pin_4 );
#define SI_SDN_HIGH( )  GPIO_SetBits( GPIOC, GPIO_Pin_4);


const  INT8U config_table_1k[] = RADIO_CONFIGURATION_DATA_ARRAY;
INT8U  buffer[8] = {0x7,0x7,0x11,0x22,0x33,0x44,0x00,0x20};
INT8U  count = 0;

INT8U  i;
INT16U j = 0;
INT8U  timeflag = 0;


void SI446X_RESET( void )
{
    INT16U x = 255;
    SI_SDN_HIGH( );
    while( x-- );
    SI_SDN_LOW( );
    SI_CSN_HIGH( );
}

INT8U SPI_ExchangeByte( INT8U input )
{
	SPI_SendData( input );
	while (SPI_GetFlagStatus(SPI_FLAG_RXNE) == RESET); //wait for receiving a byte
	while (SPI_GetFlagStatus(SPI_FLAG_TXE) == RESET);  //wait for the transmitting finished
	return SPI_ReceiveData( );
}
void SI446X_WAIT_CTS( void )
{
    INT8U cts;

    timeflag++;
    if(timeflag == 2)
    {
       // halt();halt();halt();halt();halt();halt();halt();halt();halt();halt();halt();
        wfi(); wfi(); wfi(); wfi(); wfi(); wfi(); wfi(); wfi(); wfi(); wfi(); wfi(); 
    } 
    else
    {
        do
        {
            SI_CSN_LOW( );
            SPI_ExchangeByte( READ_CMD_BUFF );
            cts = SPI_ExchangeByte( 0xFF );
            SI_CSN_HIGH( );
        }while( cts != 0xFF );    
    }
}
void SI446X_CMD( INT8U *cmd, INT8U cmdsize )
{
    SI446X_WAIT_CTS( );   
    SI_CSN_LOW( );
    while( cmdsize -- )
    {
        SPI_ExchangeByte( *cmd++ );
    }
    SI_CSN_HIGH( );
}

void SI446X_CHANGE_STATE( INT8U NewState )
{
    INT8U cmd[2];
    cmd[0] = 0x34;
    cmd[1] = NewState;
    SI446X_CMD( cmd, 2 );
}

void SI446X_SET_PROPERTY_1( SI446X_PROPERTY GROUP_NUM, INT8U proirity )
{
    INT8U cmd[5];
    cmd[0] = SET_PROPERTY;
    cmd[1] = GROUP_NUM>>8;
    cmd[2] = 1;
    cmd[3] = GROUP_NUM;
    cmd[4] = proirity;
    SI446X_CMD( cmd, 5 );
}


void SI446X_CONFIG_INIT( void )
{

      const INT8U   *ptr = config_table_1k;
   // while( ( i = *(ptr+j) ) != 0 )
   // {
    
      i = *(ptr+j);  j++;  SI446X_CMD( (INT8U*)ptr + j, i );  j = j + i;
        
      i = *(ptr+j);  j++;  SI446X_CMD( (INT8U*)ptr + j, i );  j = j + i; 
        
      i = *(ptr+j);  j++;  SI446X_CMD( (INT8U*)ptr + j, i );  j = j + i;        
        
      i = *(ptr+j);  j++;  SI446X_CMD( (INT8U*)ptr + j, i );  j = j + i;

      i = *(ptr+j);  j++;  SI446X_CMD( (INT8U*)ptr + j, i );  j = j + i;

      i = *(ptr+j);  j++;  SI446X_CMD( (INT8U*)ptr + j, i );  j = j + i;        
        
      
        
   // }
    SI446X_SET_PROPERTY_1( PKT_FIELD_1_LENGTH_7_0, PACKET_LENGTH );
}

void SI446X_SET_POWER( INT8U Power_Level )
{
    SI446X_SET_PROPERTY_1( PA_PWR_LVL, Power_Level );
}

void SI446X_SEND_PACKET( INT8U *txbuffer, INT8U size, INT8U channel, INT8U condition )
{
    INT8U tx_len = size;

    SI446X_TX_FIFO_RESET( );

    SI446X_CHANGE_STATE( 2 );
    while( SI446X_GET_DEVICE_STATE( ) != 2 );
    
    SI446X_WAIT_CTS( );
    SI_CSN_LOW( );
    SPI_ExchangeByte( WRITE_TX_FIFO );

    while( size -- )    
    { SPI_ExchangeByte( *txbuffer++ ); }
    SI_CSN_HIGH( );
    
    SI446X_START_TX( channel, condition, tx_len );
}


void SI446X_START_TX( INT8U channel, INT8U condition, INT16U tx_len )
{
    INT8U cmd[6];

    SI446X_CHANGE_STATE( 2 );
    while( SI446X_GET_DEVICE_STATE( ) != 2 );
    SI446X_CHANGE_STATE( 5 );
    while( SI446X_GET_DEVICE_STATE( ) != 5 );

    cmd[0] = START_TX;
    cmd[1] = channel;
    cmd[2] = condition;
    cmd[3] = tx_len>>8;
    cmd[4] = tx_len;
		cmd[5] = 128;
    SI446X_CMD( cmd, 6 );
}


void SI446X_TX_FIFO_RESET( void )
{
    INT8U cmd[2];

    cmd[0] = FIFO_INFO;
    cmd[1] = 0x01;
    SI446X_CMD( cmd, 2 );
}
void SI446X_READ_RESPONSE( INT8U *buffer, INT8U size )
{
    SI446X_WAIT_CTS( );
    SI_CSN_LOW( );
	SPI_ExchangeByte( READ_CMD_BUFF );
	while( size -- )
    {
        *buffer++ = SPI_ExchangeByte( 0xFF );
    }
    SI_CSN_HIGH( );

}
INT8U SI446X_GET_DEVICE_STATE( void )
{
   INT8U cmd[3];
   cmd[0] = REQUEST_DEVICE_STATE;
   SI446X_CMD( cmd, 1 );
   SI446X_READ_RESPONSE( cmd, 3 );
   return cmd[1] & 0x0F;
}




void main( )
{   
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
    
   
    
//GPIO_SetBits(GPIOB, GPIO_Pin_0); 
    
    SI446X_RESET( ); 
    SI446X_CONFIG_INIT( );                         
    SI446X_SET_POWER( 0x40 ); 
    
//GPIO_ResetBits(GPIOB, GPIO_Pin_0);

    
    SI446X_SEND_PACKET( buffer, PACKET_LENGTH, 0, 0 );
    halt();halt();halt();halt();   
    SI446X_START_TX( 0, 0, PACKET_LENGTH );
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



















/*    
    CLK_PeripheralClockConfig(CLK_Peripheral_TIM4, ENABLE); 
    TIM4_DeInit();
    TIM4_TimeBaseInit(TIM4_Prescaler_1024, 0x2);
    TIM4_ITConfig(TIM4_IT_Update, ENABLE); 
    enableInterrupts(); 
    TIM4_Cmd(DISABLE);
*/