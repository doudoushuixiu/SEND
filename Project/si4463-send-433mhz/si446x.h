

#ifndef _SI446X_H_
#define _SI446X_H_

#include "mytypedef.h"

#include "SI446X_defs.h"

/*
=================================================================================
-----------------------------External IMPORT functions---------------------------
=================================================================================
*/

#include "bsp.h"   //BSP里面包含了Si446X所用的相关函数。


/*
=================================================================================
------------------------------INTERNAL EXPORT APIs-------------------------------
=================================================================================
*/

#define  PACKET_LENGTH      8 //0-64, if = 0: variable mode, else: fixed mode



/*
=================================================================================
------------------------------INTERNAL EXPORT APIs-------------------------------
=================================================================================
*/




/*start TX command*/
void SI446X_START_TX( INT8U channel, INT8U condition, INT16U tx_len );

/*reset the RX FIFO of the device*/
void SI446X_RX_FIFO_RESET( void );

/*reset the TX FIFO of the device*/
void SI446X_TX_FIFO_RESET( void );

/*change a new state*/
void SI446X_CHANGE_STATE( INT8U NewState );

INT8U SI446X_GET_DEVICE_STATE( void );
/*
=================================================================================
----------------------------PROPERTY fast setting macros-------------------------
=================================================================================
*/
// GOLBAL(0x00)
#define GLOBAL_XO_TUNE( x )                 SI446X_SET_PROPERTY_1( 0x0000, x )
#define GLOBAL_CLK_CFG( x )                 SI446X_SET_PROPERTY_1( 0x0001, x )
#define GLOBAL_LOW_BATT_THRESH( x )         SI446X_SET_PROPERTY_1( 0x0002, x )
#define GLOBAL_CONFIG( x )                  SI446X_SET_PROPERTY_1( 0x0003, x )
#define GLOBAL_WUT_CONFIG( x )              SI446X_SET_PROPERTY_1( 0x0004, x )
#define GLOBAL_WUT_M_15_8( x )              SI446X_SET_PROPERTY_1( 0x0005, x )
#define GLOBAL_WUT_M_7_0( x )               SI446X_SET_PROPERTY_1( 0x0006, x )
#define GLOBAL_WUT_R( x )                   SI446X_SET_PROPERTY_1( 0x0007, x )
#define GLOBAL_WUT_LDC( x )                 SI446X_SET_PROPERTY_1( 0x0008, x )
#define GLOBAL_WUT_CAL( x )                 SI446X_SET_PROPERTY_1( 0x0009, x )

// INT_CTL(0x01)
#define INT_CTL_ENABLE( x )                 SI446X_SET_PROPERTY_1( 0x0100, x )
#define INT_CTL_PH_ENABLE( x )              SI446X_SET_PROPERTY_1( 0x0101, x )
#define INT_CTL_MODEM_ENABLE( x )           SI446X_SET_PROPERTY_1( 0x0102, x )
#define INT_CTL_CHIP_ENABLE( x )            SI446X_SET_PROPERTY_1( 0x0103, x )

//group 0x02, FRR_CTL
#define FRR_CTL_A_MODE( x )                 SI446X_SET_PROPERTY_1( 0x0200, x )
#define FRR_CTL_B_MODE( x )                 SI446X_SET_PROPERTY_1( 0x0201, x )
#define FRR_CTL_C_MODE( x )                 SI446X_SET_PROPERTY_1( 0x0202, x )
#define FRR_CTL_D_MODE( x )                 SI446X_SET_PROPERTY_1( 0x0203, x )

// PREAMBLE (0x10)









#endif //_SI446X_H_

/*
=================================================================================
------------------------------------End of FILE----------------------------------
=================================================================================
*/
