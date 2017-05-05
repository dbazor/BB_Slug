#ifndef BB_QUAT_H
#define BB_QUAT_H

#include "BB_BOARD.h"

/*******************************************************************************
 * PUBLIC #DEFINES                                                             *
 ******************************************************************************/

/*******************************************************************************
 * PUBLIC Variables                                                            *
 ******************************************************************************/
typedef struct Quaternion {
    float w;
    float x;
    float y;
    float z;
} Quat;

/*******************************************************************************
 * PUBLIC FUNCTION PROTOTYPES                                                  *
 ******************************************************************************/

/**
 * Function: BB_QUAT_INV()
 * @param None
 * @return None
 * @brief Init function for I2C1. Sets clock and enables I2C1
 * @author Daniel Bazor */
void BB_QUAT_INV();

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

BOOL BB_I2C_Read_Multi(UINT8 s_addr, UINT8 r_addr, UINT8 len, UINT8 *dat);

#endif