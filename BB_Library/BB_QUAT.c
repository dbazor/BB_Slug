/*
 * File:   BB_I2C.c
 * Author: BB Team
 *
 * 
 */
// I2C Master utilities, 100 kHz, using polling rather than interrupts
// The functions must be callled in the correct order as per the I2C protocol
// Master will use I2C1 SDA1 (D9) and SCL1 (D10)
// Connect these through resistors to Vcc (3.3 V). 2.4k resistors recommended,
// but something close will do.
// Connect SDA1 to the SDA pin on the slave and SCL1 to the SCL pin on a slave

#include "BB_QUAT.h"
#include "BB_BOARD.h"
#include <plib.h>
#include <math.h>

/*******************************************************************************
 * PRIVATE #DEFINES                                                            *
 ******************************************************************************/

/*******************************************************************************
 * Private Support Functions                                                   *
 ******************************************************************************/

/*******************************************************************************
 * Public Functions                                                  *
 ******************************************************************************/

/**
 * Function: BB_QUAT_FILL()
 * @param   float w, float x, float y, float z, Quat *q
 * @return 
 * @brief   Fills r with the inverse of q.
 * @author Daniel Bazor */
void BB_QUAT_FILL(float w, float x, float y, float z, Quat *q)
{
    q->w = w;
    q->x = x;
    q->y = y;
    q->z = z;
}

/**
 * Function: BB_QUAT_INV()
 * @param   q, qInv
 * @return 
 * @brief   Fills r with the inverse of q.
 * @author Daniel Bazor */
void BB_QUAT_INV(const Quat *q, Quat *qInv)
{
    qInv->w = q->w;
    qInv->x = -q->x;
    qInv->y = -q->y;
    qInv->z = -q->z;
}

/**
 * Function: BB_QUAT_MULT()
 * @param    q, r, result
 * @return  None
 * @brief 
 * @author Daniel Bazor */
void BB_QUAT_MULT(const Quat *q, const Quat *r, Quat *result)
{
    result->w = r->w * q->w - r->x * q->x - r->y * q->y - r->z * q->z;
    result->x = r->w * q->x + r->x * q->w - r->y * q->z + r->z * q->y;
    result->y = r->w * q->y + r->x * q->z + r->y * q->w - r->z * q->x;
    result->z = r->w * q->z - r->x * q->y + r->y * q->x + r->z * q->w;
}

/**
 * Function: BB_QUAT_ROTATE()
 * @param   p, vector, result
 * @return  
 * @brief   
 * @author Daniel Bazor */
void BB_QUAT_ROTATE(const Quat *q, const Quat *vector, Quat *result)
{
    Quat qInv, temp;
    BB_QUAT_INV(q, &qInv);

    BB_QUAT_MULT(q, vector, &temp);
    BB_QUAT_MULT(&temp, &qInv, result);
}

/**
 * Function: BB_QUAT_TIP_VECTOR()
 * @param   q, result
 * @return  
 * @brief   Does quaternion rotation for special case of up-vector
 * @precond q must be a unit quaternion     
 * @author Daniel Bazor */
void BB_QUAT_TIP_VECTOR(const Quat *q, Quat *result)
{
    Quat qInv, temp, upVec;
    BB_QUAT_INV(q, &qInv); // fills q-inverse

    BB_QUAT_FILL(0, 0, 0, 1, &upVec); // fills up-vector
    BB_QUAT_FILL(-q->z, q->y, -q->x, q->w, &temp); // acts as first quat mult

    BB_QUAT_MULT(&temp, &qInv, result); // the normalizing quat mult
}

/**
 * Function: BB_QUAT_FIND_TIP_ANGLE_X()
 * @param   tipVect
 * @return  float tip angle in x
 * @brief   
 * @precond      
 * @author Daniel Bazor */
float BB_QUAT_FIND_TIP_ANGLE_X(const Quat *tipVect)
{
    return atan2(tipVect->x, tipVect->z);
}

/**
 * Function: BB_QUAT_FIND_TIP_ANGLE_Y()
 * @param   tipVect
 * @return  float tip angle in y
 * @brief   
 * @precond      
 * @author Daniel Bazor */
float BB_QUAT_FIND_TIP_ANGLE_Y(const Quat *tipVect)
{
    return atan2(tipVect->y, tipVect->z);
}