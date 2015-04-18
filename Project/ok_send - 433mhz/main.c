#include "stm8l10x.h"

#define  INT8U       unsigned char 
#define  CSN_1       GPIO_SetBits  (GPIOB,GPIO_Pin_0);          
#define  CSN_0       GPIO_ResetBits(GPIOB,GPIO_Pin_0);

INT8U ID_part[4] = {0x11,0x22,0x33,0x44};
INT8U PaTabel[8] = {0xC0 ,0xC0 ,0xC0 ,0xC0 ,0xC0 ,0xC0 ,0xC0 ,0xC0};       //10dBm     433Mhz

//INT8U PaTabel[8] = {0x00,0x1d,0x00,0x00,0x00,0x00,0x00,0x00};  //ASK


void halSpiWriteReg(INT8U addr, INT8U value);
void halSpiWriteBurstReg(INT8U addr, INT8U *buffer, INT8U count);
void halSpiStrobe(INT8U strobe);
void halRfSendPacket(INT8U *txBuffer, INT8U size);
INT8U halSpiReadReg(INT8U addr) ;

/************  SPI写寄存器  ******************/
void halSpiWriteReg(INT8U addr, INT8U value) 
{
    CSN_0;
    SPI->DR = addr;            	
    while(SPI_GetFlagStatus(SPI_FLAG_TXE)==RESET);    
    SPI->DR=value;		    
    while(SPI_GetFlagStatus(SPI_FLAG_TXE)==RESET);    
    CSN_1;
}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            

/************  SPI连续写配置寄存器  ******************/
void halSpiWriteBurstReg(INT8U addr, INT8U *buffer, INT8U count) 
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
void halSpiStrobe(INT8U strobe) 
{
    CSN_0;
    SPI->DR=strobe;		
    while(SPI_GetFlagStatus(SPI_FLAG_TXE)==RESET);
    CSN_1;
}

/************  CC1100发送一组数据 ******************/
void halRfSendPacket(INT8U *txBuffer, INT8U size) 
{
    halSpiWriteReg(0x3F, size);   
    halSpiWriteBurstReg(0x3F, txBuffer, size);	//写入要发送的数据    
    halSpiStrobe(0x35);                         //进入发送模式发送数据     
}

/************  主函数入口  ******************/
void main(void)
{       
    INT8U key_pushed = 0;
    INT8U scan_key = 2;
    INT8U datatosend[7]={0x7,ID_part[0],ID_part[1],ID_part[2],ID_part[3],0x00,0x00}; 
      
    CLK->CKDIVR=0x03;  
    CLK->PCKENR=0x50; 

    GPIOB->ODR = 0x61;
    GPIOB->DDR = 0x61;
    GPIOB->CR1 = 0xE1;  
    GPIOB->CR2 = 0x61;
    
    GPIO_Init(GPIOB,GPIO_Pin_1,GPIO_Mode_In_PU_No_IT);
    GPIO_Init(GPIOB,GPIO_Pin_2,GPIO_Mode_In_PU_No_IT);
    GPIO_Init(GPIOB,GPIO_Pin_3,GPIO_Mode_In_PU_No_IT);
    GPIO_Init(GPIOC,GPIO_Pin_2,GPIO_Mode_In_PU_No_IT);
    
    
    //配置空闲IO  GPIO_Mode_Out_PP_Low_Slow
         
    SPI->CR2=0xC3;
    SPI->CR1=0x47;
    AWU->CSR=0x10;    
    AWU->TBR=0x01;
    AWU->APR=0x3E;   
     
    halt();  

    halSpiWriteReg(0x08,0x05);          //PKTCTRL0    
    halSpiWriteReg(0x0D,0x10);          //FREQ2
    halSpiWriteReg(0x0E,0xB1);          //FREQ1
    halSpiWriteReg(0x0F,0x3B);          //FREQ0
    halSpiWriteReg(0x10,0x2D);          //MDMCFG4     250Kbps
    halSpiWriteReg(0x11,0x3B);          //MDMCFG3
    halSpiWriteReg(0x12,0x03);          //MDMCFG2     0x13 GFSK
    
  //  halSpiWriteReg(0x13,0x02);          //MDMCFG1   
    
    halSpiWriteReg(0x15,0x62);          //DEVIATN
    halSpiWriteReg(0x18,0x18);          //MCSM0   
    halSpiWriteReg(0x23,0xEA);          //FSCAL3
    halSpiWriteReg(0x24,0x2A);          //FSCAL3
    halSpiWriteReg(0x25,0x00);          //FSCAL3
    halSpiWriteReg(0x26,0x1F);          //FSCAL3   

    halSpiWriteBurstReg(0x3E, PaTabel, 8);   //配置cc1101功率
     
    
    
    
    datatosend[6] = 0x20;
    halRfSendPacket(datatosend,7);     //发送  
    while(1);
    
    
  //  while((scan_key > 0) &&(key_pushed == 0))
    while(1)
    {
        if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_2)   == 0)   //KEY1  PC.2
        {
            datatosend[6] = 0x20;
            halRfSendPacket(datatosend,7);     //发送          
            key_pushed = 1;
            halt(); halt(); halt(); halt();
            halSpiStrobe(0x3B);   //清空
            halRfSendPacket(datatosend,7);     //发送 
            halt(); halt(); halt(); halt();
            halSpiStrobe(0x3B);  
            halRfSendPacket(datatosend,7);     //发送 
            halt(); halt(); halt(); halt();            
        }
        
        if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)  == 0)   //KEY2  PB.1
        {
            datatosend[6] = 0x20;
            halRfSendPacket(datatosend,7);     //发送          
            key_pushed = 1;
            halt(); halt(); halt(); halt();
            halSpiStrobe(0x3B);   //清空
            halRfSendPacket(datatosend,7);     //发送 
            halt(); halt(); halt(); halt();
            halSpiStrobe(0x3B);  
            halRfSendPacket(datatosend,7);     //发送 
            halt(); halt(); halt(); halt(); 
        }        

        if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_2)  == 0)   //KEY3  PB.2
        {
            datatosend[6] = 0x20;
            halRfSendPacket(datatosend,7);     //发送          
            key_pushed = 1;
            halt(); halt(); halt(); halt();
            halSpiStrobe(0x3B);   //清空
            halRfSendPacket(datatosend,7);     //发送 
            halt(); halt(); halt(); halt();
            halSpiStrobe(0x3B);  
            halRfSendPacket(datatosend,7);     //发送 
            halt(); halt(); halt(); halt(); 
        }         
        
        if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_3)  == 0)   //KEY4  PB.3
        {
            datatosend[6] = 0x20;
            halRfSendPacket(datatosend,7);     //发送          
            key_pushed = 1;
            halt(); halt(); halt(); halt();
            halSpiStrobe(0x3B);   //清空
            halRfSendPacket(datatosend,7);     //发送 
            halt(); halt(); halt(); halt();
            halSpiStrobe(0x3B);  
            halRfSendPacket(datatosend,7);     //发送 
            halt(); halt(); halt(); halt(); 
        }      
        scan_key--;
    }
    
    /*
    if(key_pushed == 0)
    {
        datatosend[6] = 0x20;
        halRfSendPacket(datatosend,7);     //发送          
        key_pushed = 1;
        halt(); halt(); halt(); halt();
        halSpiStrobe(0x3B);   //清空
        halRfSendPacket(datatosend,7);     //发送 
        halt(); halt(); halt(); halt();
        halSpiStrobe(0x3B);  
        halRfSendPacket(datatosend,7);     //发送 
        halt(); halt(); halt(); halt();    
    } 
    
*/
    
    while(1)
    {
        halt();
    }
      

}







/*
    halSpiStrobe(0x3B);  
    halRfSendPacket(datatosend,7); 
    halt(); halt(); halt(); halt();
    
    halSpiStrobe(0x3B);  
    halRfSendPacket(datatosend,7); 
    halt(); halt(); halt(); halt();    
       
*/



//    GPIO_Init(GPIOB, GPIO_Pin_7,GPIO_Mode_In_PU_No_IT);           //MISO_PIN
//    GPIO_Init(GPIOB, GPIO_Pin_6,GPIO_Mode_Out_PP_High_Fast);      // MOSI_PIN
 //   GPIO_Init(GPIOB, GPIO_Pin_5,GPIO_Mode_Out_PP_High_Fast);      // SCK_PIN
//    GPIO_Init(GPIOB, GPIO_Pin_0,GPIO_Mode_Out_PP_High_Fast);      //CSN_PIN  
    
    

    //CLK_MasterPrescalerConfig(CLK_MasterPrescaler_HSIDiv8); 
  //  GPIO_Init(GPIOB, GPIO_Pin_7,GPIO_Mode_In_PU_No_IT);           //MISO_PIN
  //  GPIO_Init(GPIOB, GPIO_Pin_6,GPIO_Mode_Out_PP_High_Fast);      // MOSI_PIN
  //  GPIO_Init(GPIOB, GPIO_Pin_5,GPIO_Mode_Out_PP_High_Fast);      // SCK_PIN
  //  GPIO_Init(GPIOB, GPIO_Pin_0,GPIO_Mode_Out_PP_High_Fast);      //CSN_PIN
   //AWU_Cmd(ENABLE);AWU_Init(AWU_Timebase_8ms);



 /* halSpiWriteReg(0x08,0x05);          //PKTCTRL0 
    halSpiWriteReg(0x07, 0x04);        //PKTCTRL1 
    halSpiWriteReg(0x0D,0x10);          //FREQ2
    halSpiWriteReg(0x0E,0xA7);          //FREQ1
    halSpiWriteReg(0x0F,0x62);          //FREQ0
    halSpiWriteReg(0x10,0x2B);          //MDMCFG4        100Kbps
    halSpiWriteReg(0x11,0xF8);          //MDMCFG3
    halSpiWriteReg(0x12,0x13);          //MDMCFG2
    halSpiWriteReg(0x18,0x18);        //MCSM0
   
*/ 



#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line)
{
    while (1)
    {
    }
}
#endif
