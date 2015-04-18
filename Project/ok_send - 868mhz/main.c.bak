#include "stm8l10x.h"

#define  INT8U       unsigned char 
#define  CSN_1       GPIO_SetBits(GPIOB,GPIO_Pin_3); 
#define  CSN_0       GPIO_ResetBits(GPIOB,GPIO_Pin_3);

INT8U ID_part[4] = {0x11,0x22,0x33,0x44};
INT8U PaTabel[8] = {0xC3 ,0xC3 ,0xC3 ,0xC3 ,0xC3 ,0xC3 ,0xC3 ,0xC3};   //10dBm     
void halSpiWriteReg(INT8U addr, INT8U value);
void halSpiWriteBurstReg(INT8U addr, INT8U *buffer, INT8U count);
void halSpiStrobe(INT8U strobe);
void halRfSendPacket(INT8U *txBuffer, INT8U size);


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
    INT8U i,j;
    INT8U rand_num=((ID_part[3]<<4)/3);  //随机数
    
    INT8U datatosend[7]={0x7,ID_part[0],ID_part[1],ID_part[2],ID_part[3],0x0,0x0}; 
      
    CLK->CKDIVR=0x03;  //CLK_MasterPrescalerConfig(CLK_MasterPrescaler_HSIDiv8);
    CLK->PCKENR=0x50;
    
    GPIO_Init(GPIOB, GPIO_Pin_7,GPIO_Mode_In_PU_No_IT);           //MISO_PIN
    GPIO_Init(GPIOB, GPIO_Pin_6,GPIO_Mode_Out_PP_High_Fast);      // MOSI_PIN
    GPIO_Init(GPIOB, GPIO_Pin_5,GPIO_Mode_Out_PP_High_Fast);      // SCK_PIN
    GPIO_Init(GPIOB, GPIO_Pin_3,GPIO_Mode_Out_PP_High_Fast);      //CSN_PIN
    
    SPI->CR2=0xC3;
    SPI->CR1=0x47;
    AWU->CSR=0x10;    //AWU_Cmd(ENABLE);AWU_Init(AWU_Timebase_8ms);
    AWU->TBR=0x03;
    AWU->APR=0x3E;
    
    
    halSpiWriteReg(0x07, 0x04);   //PKTCTRL1
    halSpiWriteReg(0x08, 0x05);   //PKTCTRL0  
    halSpiWriteReg(0x0D, 0x23);   //FREQ2
    halSpiWriteReg(0x0E, 0x31);   //FREQ1
    halSpiWriteReg(0x0F, 0x3B);   //FREQ0
    halSpiWriteReg(0x10, 0x2D);   //MDMCFG4     
    halSpiWriteReg(0x11, 0x3B);   //MDMCFG3  
    halSpiWriteReg(0x12, 0x13);   //MDMCFG2
    halSpiWriteReg(0x18, 0x18 );  //校准 MCSM0
    halSpiWriteBurstReg(0x3E, PaTabel, 8);   //配置cc1101功率
    
    
    
    halSpiWriteReg(0x0A, 0x14);     //CHANNR
    
    halSpiWriteReg(0x0B, 0x0C);     //FSCTRL1
    halSpiWriteReg(0x15, 0x62);     //DEVITAN
//    halSpiWriteReg(0x19, 0x1D);   //FOCCFG
 //   halSpiWriteReg(0x1A, 0x1C);   //BSCFG
  //  halSpiWriteReg(0x1B, 0xC7);   //AGCCTRL2
  //  halSpiWriteReg(0x1C, 0x00);   //AGCCTRL1
   // halSpiWriteReg(0x1D, 0xB0);   //AGCCTRL0
  //  halSpiWriteReg(0x20, 0xFB);   //WORCTRL
    
  //  halSpiWriteReg(0x21, 0xB6);   //FREND1
    halSpiWriteReg(0x23, 0xEA);     //FSCAL3
    halSpiWriteReg(0x24, 0x2A);     //FSCAL2
    halSpiWriteReg(0x25, 0x00);     //FSCAL1
    halSpiWriteReg(0x26, 0x1F);     //FSCAL0
 //   halSpiWriteReg(0x2E, 0x09);   //TEST0
     
    halRfSendPacket(datatosend,7);      //发送数据  
    __halt();                           //stm休眠
     
    for(i=0;i<rand_num;i++)             //随机延时
    {for(j=0; j<100; j++);}
 
    halSpiStrobe(0x3B);                 //清空发送区数据    
    halRfSendPacket(datatosend,7);  
    __halt();                           //stm休眠
    
    for(i=0;i<rand_num;i++)             //随机延时
    {for(j=0; j<100; j++);}    
     
    halSpiStrobe(0x3B);                 //清空发送区数据
    halRfSendPacket(datatosend,7);  
    
 //   AWU->CSR=0x00;                    //stm一直休眠
 //   halt(); 
    while(1);

}























#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line)
{
    while (1)
    {
    }
}
#endif
