#include "stm8l10x.h"
#include "stm8l10x_spi.h"

#define  INT8U       unsigned char 
#define  CSN_1       GPIOB->ODR |= GPIO_Pin_3; 
#define  CSN_0       GPIOB->ODR &= (INT8U)(~GPIO_Pin_3);

INT8U ID_part[4] = {0x11,0x22,0x22,0x44};
INT8U PaTabel[8] = {0xC5 ,0xC5 ,0xC5 ,0xC5 ,0xC5 ,0xC5 ,0xC5 ,0xC5};   //10dBm 


unsigned char  count1 = 0;

void SpiWriteReg(INT8U addr, INT8U value);
void SpiWriteBurstReg(INT8U addr, INT8U *buffer, INT8U count);
void SpiStrobe(INT8U strobe);
void RfSendPacket(INT8U *txBuffer, INT8U size);
void delay(INT8U  s);

void delay(INT8U  s)
{
    while(s>0 )
    {s--;}
     
}

/************  SPI写寄存器  ******************/
void SpiWriteReg(INT8U addr, INT8U value) 
{
    CSN_0;
    SPI->DR = addr;            	
    while(SPI_GetFlagStatus(SPI_FLAG_TXE)==RESET);     
    SPI->DR = value;		    
    while(SPI_GetFlagStatus(SPI_FLAG_TXE)==RESET);     
    CSN_1;
}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            

/************  SPI连续写配置寄存器  ******************/
void SpiWriteBurstReg(INT8U addr, INT8U *buffer, INT8U count) 
{
    INT8U i, temp;
    temp = addr | 0x40;
    CSN_0;
    SPI->DR=temp; 
    while(SPI_GetFlagStatus(SPI_FLAG_TXE)==RESET); 
    for (i = 0; i < count; i++)
    {  
      SPI->DR=buffer[i];      
      while(SPI_GetFlagStatus(SPI_FLAG_TXE)==RESET);
    }
    CSN_1;
}

/************  SPI写命令  ******************/
void SpiStrobe(INT8U strobe) 
{
    CSN_0;
    SPI->DR=strobe;		
    while(SPI_GetFlagStatus(SPI_FLAG_TXE)==RESET);
    CSN_1;
}

/************  CC1100发送一组数据 ******************/
void RfSendPacket(INT8U *txBuffer, INT8U size) 
{
    SpiWriteReg(0x3F, size);   
    SpiWriteBurstReg(0x3F, txBuffer, size);	//写入要发送的数据    
    SpiStrobe(0x35);                            //进入发送模式发送数据
}

/************  主函数入口  ******************/
void main(void)
{       
  //  INT8U rand_num=((ID_part[3]<<4)/3);  //随机数
    INT8U key_pushed = 0;
    INT8U scan_key = 2;
    INT8U datatosend[7]={0x7,ID_part[0],ID_part[1],ID_part[2],ID_part[3],0x00,0x70};
       
    CLK->CKDIVR=0x03;  //8分频  2mhz
    CLK->PCKENR=0x50;
    CLK_MasterPrescalerConfig(CLK_MasterPrescaler_HSIDiv4);
     
    GPIOB->ODR=0x68;
    GPIOB->DDR=0x78;
    GPIOB->CR1=0xFF;
    GPIOB->CR2=0x78;
    GPIOB->IDR=0xEF;
    
    GPIOD->ODR=0x00;
    GPIOD->DDR=0x00;
    GPIOD->CR1=0x01;
    GPIOD->CR2=0x00;
    GPIOD->IDR=0xFF;  
    
    GPIOA->ODR=0x00;
    GPIOA->DDR=0x0F;
    GPIOA->CR1=0x0F;
    GPIOA->CR2=0x0F;
    GPIOA->IDR=0x73;  
    
    GPIOC->ODR=0x00;
    GPIOC->DDR=0x1F;
    GPIOC->CR1=0x1F;
    GPIOC->CR2=0x1F;
    GPIOC->IDR=0x60;  

    SPI->CR2=0xC3;
    SPI->CR1=0x47;     
    AWU->CSR=0x10;                  
    AWU->TBR=0x01;
    AWU->APR=0x3E;    //2ms休眠
        
    halt();       
    CLK->PCKENR &= (uint8_t)(~(uint8_t)0x40);  //关AWU
    
    CSN_0;    
    SPI->DR = 0x08; while(SPI_GetFlagStatus(SPI_FLAG_TXE)==RESET);SPI->DR = 0x05; while(SPI_GetFlagStatus(SPI_FLAG_TXE)==RESET);           	             		         	        		    
    SPI->DR = 0x0D; while(SPI_GetFlagStatus(SPI_FLAG_TXE)==RESET);SPI->DR = 0x21; while(SPI_GetFlagStatus(SPI_FLAG_TXE)==RESET);           	            	
    SPI->DR = 0x0E; while(SPI_GetFlagStatus(SPI_FLAG_TXE)==RESET);SPI->DR = 0x62; while(SPI_GetFlagStatus(SPI_FLAG_TXE)==RESET);           	           		    
    SPI->DR = 0x0F; while(SPI_GetFlagStatus(SPI_FLAG_TXE)==RESET);SPI->DR = 0x76; while(SPI_GetFlagStatus(SPI_FLAG_TXE)==RESET);           	            		    
    SPI->DR = 0x10; while(SPI_GetFlagStatus(SPI_FLAG_TXE)==RESET);SPI->DR = 0x5B; while(SPI_GetFlagStatus(SPI_FLAG_TXE)==RESET);           	           		    
    SPI->DR = 0x11; while(SPI_GetFlagStatus(SPI_FLAG_TXE)==RESET);SPI->DR = 0xF8; while(SPI_GetFlagStatus(SPI_FLAG_TXE)==RESET);           	           		    
    SPI->DR = 0x12; while(SPI_GetFlagStatus(SPI_FLAG_TXE)==RESET);SPI->DR = 0x13; while(SPI_GetFlagStatus(SPI_FLAG_TXE)==RESET);            	          		    
    SPI->DR = 0x18; while(SPI_GetFlagStatus(SPI_FLAG_TXE)==RESET);SPI->DR = 0x18; while(SPI_GetFlagStatus(SPI_FLAG_TXE)==RESET);    
    SPI->DR = 0x23; while(SPI_GetFlagStatus(SPI_FLAG_TXE)==RESET);SPI->DR = 0xEA; while(SPI_GetFlagStatus(SPI_FLAG_TXE)==RESET);   
    SPI->DR = 0x24; while(SPI_GetFlagStatus(SPI_FLAG_TXE)==RESET);SPI->DR = 0x2A; while(SPI_GetFlagStatus(SPI_FLAG_TXE)==RESET);            	          		    
    SPI->DR = 0x25; while(SPI_GetFlagStatus(SPI_FLAG_TXE)==RESET);SPI->DR = 0x00; while(SPI_GetFlagStatus(SPI_FLAG_TXE)==RESET);    
    SPI->DR = 0x26; while(SPI_GetFlagStatus(SPI_FLAG_TXE)==RESET);SPI->DR = 0x1F; while(SPI_GetFlagStatus(SPI_FLAG_TXE)==RESET);        
    CSN_1;
   
    SpiWriteBurstReg(0x3E, PaTabel, 8);   //配置cc1101功率 
      
    //没有检测到按键按下
    if(key_pushed == 0)
    {
        datatosend[6] = 0x20;                  
        
        SpiStrobe(0x33);  //矫正时钟
        RfSendPacket(datatosend,7);                          
        CLK->PCKENR |= ((uint8_t)0x40);  //开AWU
        halt(); halt(); halt();     
        CLK->PCKENR &= (uint8_t)(~(uint8_t)0x40);
       
 //       SpiStrobe(0x3B);                       
  //      RfSendPacket(datatosend,7);
  //      CLK->PCKENR |= ((uint8_t)0x40);
 //       __halt();halt();halt();        
 //       CLK->PCKENR &= (uint8_t)(~(uint8_t)0x40);
           
   //     SpiStrobe(0x3B);                      
    //    RfSendPacket(datatosend,7);
    //    __halt();                 
    }   
    
        
    while((scan_key > 0) &&(key_pushed == 0))
    {
        if( (GPIOD->IDR & (uint8_t)0x01)  == 0 )    //key1   PD.0
        { 
            datatosend[6] = 0x10;
            RfSendPacket(datatosend,7);             
            key_pushed = 1;
            wfi();                    
        }      
          
        if( (GPIOB->IDR & (uint8_t)0x01)  == 0 )   //key2   PB.0
        { 
            datatosend[6] = 0x30;
            RfSendPacket(datatosend,7);           
            key_pushed = 1;
            __halt(); __halt();                      
        }        
          
        if( (GPIOB->IDR & (uint8_t)0x02)  == 0 )   //key3   PB.1
        { 
            datatosend[6] = 0x50;
            RfSendPacket(datatosend,7);           
            key_pushed = 1;
            __halt(); __halt();                      
        }        
           
        if( (GPIOB->IDR & (uint8_t)0x04)  == 0 )   //key4   PB.2
        { 
            datatosend[6] = 0x70;
            RfSendPacket(datatosend,7);            
            key_pushed = 1;
            __halt(); __halt();                      
        }      
        
        __halt();
        scan_key -- ;     
    }

    
    
    while(1);
}

























/*    
   
    SpiWriteReg(0x08,0x05);          //PKTCTRL0     
    SpiWriteReg(0x0D,0x21);          //FREQ2
    SpiWriteReg(0x0E,0x62);          //FREQ1
    SpiWriteReg(0x0F,0x76);          //FREQ0
    SpiWriteReg(0x10,0x5B);          //MDMCFG4 100Kbps
    SpiWriteReg(0x11,0xF8);          //MDMCFG3
    SpiWriteReg(0x12,0x13);           //MDMCFG2  
    SpiWriteReg(0x18,0x18);          //MCSM0
    SpiWriteReg(0x23,0xEA);          //FSCAL3
    SpiWriteReg(0x24,0x2A);          //FSCAL2    
    SpiWriteReg(0x25,0x00);          //FSCAL1
    SpiWriteReg(0x26,0x1F);          //FSCAL0 
/*            
    for(i=0;i<rand_num;i++)               //随机延时
    {for(j=0; j<100; j++);}     
    SpiStrobe(0x3B);                      //清空发送区数据   
    RfSendPacket(datatosend,7);  
    __halt();                             //stm休眠   
    
    for(i=0;i<rand_num;i++)               //随机延时
    {for(j=0; j<100; j++);}         
    SpiStrobe(0x3B);                      //清空发送区数据    
    RfSendPacket(datatosend,7); 

    CLK_PeripheralClockConfig(CLK_Peripheral_TIM4, ENABLE);
    TIM4_DeInit();
  //  TIM4_TimeBaseInit(TIM4_Prescaler_16, 125);    //1ms 
TIM4_TimeBaseInit(TIM4_Prescaler_4, 25);
    
    TIM4_ITConfig(TIM4_IT_Update, ENABLE);
    enableInterrupts();
    TIM4_Cmd(ENABLE);
    
 */  




    //按键
  //  GPIO_Init(GPIOB, GPIO_Pin_0,GPIO_Mode_In_PU_No_IT);
  //  GPIO_Init(GPIOB, GPIO_Pin_1,GPIO_Mode_In_PU_No_IT);
   // GPIO_Init(GPIOB, GPIO_Pin_2,GPIO_Mode_In_PU_No_IT);
   // GPIO_Init(GPIOD, GPIO_Pin_0,GPIO_Mode_In_PU_No_IT);
//  GPIO_Init(GPIOB, GPIO_Pin_7,GPIO_Mode_In_PU_No_IT);           //MISO_PIN
//  GPIO_Init(GPIOB, GPIO_Pin_6,GPIO_Mode_Out_PP_High_Fast);      //MOSI_PIN
//  GPIO_Init(GPIOB, GPIO_Pin_5,GPIO_Mode_Out_PP_High_Fast);      //SCK_PIN
//  GPIO_Init(GPIOB, GPIO_Pin_3,GPIO_Mode_Out_PP_High_Fast);      //CSN_PIN





   // SpiWriteReg(0x1B,0xC7);          //AGCCTRL2
  //  SpiWriteReg(0x1C,0x00);          //AGCCTRL1
  //  SpiWriteReg(0x1D,0xB0);          //AGCCTRL0  


  //  SpiWriteReg(0x0C,0x00);          //FSCTRL0    
  //  SpiWriteReg(0x06,0xFF);          //PKTLEN   
  //  SpiWriteReg(0x07,0x04);          //PKTCTRL1   
  //  SpiWriteReg(0x13,0x22);          //MDMCFG1     //
  //  SpiWriteReg(0x14,0xF8);          //MDMCFG0     //  
  //  SpiWriteReg(0x22,0x10);          //FREND0      // 

//  AWU_Cmd(ENABLE);AWU_Init(AWU_Timebase_8ms);AWU_Timebase_250us

//  CLK_MasterPrescalerConfig(CLK_MasterPrescaler_HSIDiv8);
















#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line)
{
    while (1)
    {
    }
}
#endif
