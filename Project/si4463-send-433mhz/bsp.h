
#ifndef _BSP_H_
#define _BSP_H_

#include "STM8l10x_conf.h"
#include "mytypedef.h"
#include "si446x.h"



#define SI_CSN_LOW( )   GPIO_ResetBits( GPIOB, GPIO_Pin_4 );
#define SI_CSN_HIGH( )  GPIO_SetBits( GPIOB, GPIO_Pin_4 );

#define SI_SDN_LOW( )   GPIO_ResetBits( GPIOC, GPIO_Pin_4 );
#define SI_SDN_HIGH( )  GPIO_SetBits( GPIOC, GPIO_Pin_4);

#define OLED_CSN_H( )   GPIO_SetBits( GPIOB, GPIO_Pin_3)
#define OLED_CSN_L( )   GPIO_ResetBits( GPIOB, GPIO_Pin_3)

#define OLED_CMD_H( )   GPIO_SetBits( GPIOD, GPIO_Pin_0)
#define OLED_CMD_L( )   GPIO_ResetBits( GPIOD, GPIO_Pin_0)

#define OLED_RST_H( )   GPIO_SetBits( GPIOB, GPIO_Pin_2)
#define OLED_RST_L( )   GPIO_ResetBits( GPIOB, GPIO_Pin_2)

/*turn on the LED*/
#define LED_On( )       GPIO_ResetBits( GPIOB, GPIO_Pin_0 )

/*turn off the LED*/
#define LED_Off( )      GPIO_SetBits( GPIOB, GPIO_Pin_0 )

/*toggle the LED*/
#define LED_Toggle( )   GPIO_ToggleBits( GPIOB, GPIO_Pin_0 )




/*Exchange a byte via the SPI bus*/
INT8U SPI_ExchangeByte( INT8U input );

/*Read parameters from flash memory*/
void Par_Read( void );

/*Save parameters to flash memory*/
void Par_Save( void );

/*Get the status of the SET pin*/
INT8U Get_Pin_Set( void );

/*Initialize the USART*/
void USARTInitial( INT32U baudrate, INT8U Parity );

/*Config the independent watchdog*/
void IWDG_Config(void);

/*Set the AUX pin*/
void Set_Pin_Aux( INT8U x );

/*Get the status of the GPIO0 pin*/
INT8U Get_Pin_GPIO0( void );

/*Get the status of the GPIO1 pin*/
INT8U Get_Pin_GPIO1( void );

void BSP_LowPowerMode( void );

void BSP_Restore( void );


#endif //_BSP_H_
/*
=================================================================================
------------------------------------End of File----------------------------------
=================================================================================
*/

