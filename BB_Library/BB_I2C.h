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
void BB_I2C_Init();


/*******************************************************************************
  Function:
    void BB_I2C_Write()

  Summary:
    Write one Byte to the I2C1 Bus

  Description:
    Writes to the slave at s_addr at the location r_addr the data at *dat

  Precondition:
    None

  Parameters:
    s_addr  - Slave address
    r_addr  - Data address
 *dat    - data to be written
    
  Returns:
    TRUE    - Data was sent successfully
    FALSE   - There was a problem with the transfer
    
  Example:
    <code>
    success = BB_I2C_Write(UINT8 s_addr, UINT8 r_addr, UINT8 &dat);
    </code>

  Remarks:
    None
 *****************************************************************************/
BOOL BB_I2C_Write(UINT8 s_addr, UINT8 r_addr, UINT8 *dat);


/*******************************************************************************
  Function:
    void BB_I2C_Read()

  Summary:
    Read one Byte from the I2C1 Bus

  Description:
    Reads from the slave at s_addr at the location r_addr the data at *dat

  Precondition:
    None

  Parameters:
    s_addr  - Slave address
    r_addr  - Data address
    *dat    - where to read data to
    
  Returns:
    TRUE    - Data was sent successfully
    FALSE   - There was a problem with the transfer
    
  Example:
    <code>
    success = BB_I2C_Read(UINT8 s_addr, UINT8 r_addr, UINT8 &dat);
    </code>

  Remarks:
    None
 *****************************************************************************/
BOOL BB_I2C_Read(UINT8 s_addr, UINT8 r_addr, UINT8 *dat);


/*******************************************************************************
  Function:
    void BB_I2C_Read_Multi()

  Summary:
    Read one Byte from the I2C1 Bus

  Description:
    Reads from the slave at s_addr at the location r_addr the data at *dat

  Precondition:
    None

  Parameters:
    s_addr  - Slave address
    r_addr  - Data address
    len     - length of data to read
    *dat    - where to read data to
    
  Returns:
    TRUE    - Data was sent successfully
    FALSE   - There was a problem with the transfer
    
  Example:
    <code>
    success = BB_I2C_Read_Multi(UINT8 s_addr, UINT8 r_addr, len, UINT8 &dat);
    </code>

  Remarks:
    None
 *****************************************************************************/
BOOL BB_I2C_Read_Multi(UINT8 s_addr, UINT8 r_addr, UINT8 len, UINT8 *dat);

#endif