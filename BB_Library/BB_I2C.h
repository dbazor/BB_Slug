#ifndef BB_I2C_H
#define BB_I2C_H

#include "BB_BOARD.h"

/*******************************************************************************
 * PUBLIC #DEFINES                                                             *
 ******************************************************************************/
#define I2C_CLOCK_FREQ  (400000ul)    // 100 kHz change me to max imu speed
#define BNO55_I2C_BUS   I2C1

/*****************************************************************************/



/*******************************************************************************
 * PUBLIC FUNCTION PROTOTYPES                                                  *
 ******************************************************************************/

/*******************************************************************************
  Function:
    void BB_I2C_Init()

  Summary:
    Init function for I2C1

  Description:
    Sets clock and enables I2C1

  Precondition:
    None

  Parameters:
    None
    
  Returns:
    None
    
  Example:
    <code>
    BB_I2C_Init();
    </code>

  Remarks:
    Call this on startup
 *****************************************************************************/

/**
 * Function: BB_I2C_Init()
 * @param None
 * @return None
 * @brief Init function for I2C1. Sets clock and enables I2C1
 * @author Daniel Bazor */
void BB_I2C_Init();

/**
 * Function: BB_I2C_Write()
 * @param   s_addr  - Slave address
 *          r_addr  - Data address
 * @return  TRUE    - Data was sent successfully
 *          FALSE   - There was a problem with the transfer
 * @brief   Write one Byte to the I2C1 Bus. Writes to the slave at s_addr at 
 *      the location r_addr the data at *dat
 * @example <code>
 *              success = BB_I2C_Write(UINT8 s_addr, UINT8 r_addr, UINT8 &dat);
 *          </code>
 * @author Daniel Bazor */
BOOL BB_I2C_Write(UINT8 s_addr, UINT8 r_addr, UINT8 *dat);

/**
 * Function: BB_I2C_Read(UINT8 s_addr, UINT8 r_addr, UINT8 *dat)
 * @param   s_addr  - Slave address
 *          r_addr  - Data address
 *          *dat    - where to read data to
 * @return  TRUE    - Data was sent successfully
 *          FALSE   - There was a problem with the transfer
 * @brief   Read one Byte from the I2C1 Bus. Reads from the slave at s_addr 
 *          at the location r_addr the data at *dat
 * @example <code>
 *              success = BB_I2C_Read(UINT8 s_addr, UINT8 r_addr, UINT8 &dat);
 *          </code>
 * @author Daniel Bazor */
BOOL BB_I2C_Read(UINT8 s_addr, UINT8 r_addr, UINT8 *dat);

/**
 * Function: BB_I2C_Read_Multi(UINT8 s_addr, UINT8 r_addr, UINT8 *dat)
 * @param   s_addr  - Slave address
 *          r_addr  - Data address
 *          *dat    - where to read data to
 *          len     - length of data to read
 * @return  TRUE    - Data was sent successfully
 *          FALSE   - There was a problem with the transfer
 * @brief   Read multiple Byte from the I2C1 Bus Reads from the slave at s_addr 
 *          at the location r_addr the data at *dat
 * @example <code>
 *              success = BB_I2C_Read_Multi(UINT8 s_addr, UINT8 r_addr, len, UINT8 &dat);
 *          </code>
 * @author Daniel Bazor */

BOOL BB_I2C_Read_Multi(UINT8 s_addr, UINT8 r_addr, UINT8 len, BYTE *dat);

#endif