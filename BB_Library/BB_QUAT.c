/*
 * File:   BB_QUAT.c
 * Author: BB Team
 * 
 */

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
 * Public Functions                                                            *
 ******************************************************************************/

/**
 * Function: BB_QUAT_FILL()
 * @param   double w, double x, double y, double z, Quat *q
 * @return 
 * @brief   Fills r with the inverse of q.
 * @author Daniel Bazor */
void BB_Quat_Fill(double w, double x, double y, double z, Quat *q)
{
    q->w = w;
    q->x = x;
    q->y = y;
    q->z = z;
}

/**
 * Function: BB_Quat_Normalize()
 * @param   q, qInv
 * @return 
 * @brief   Fills r with the inverse of q.
 * @author Daniel Bazor */
void BB_Quat_Normalize(Quat *q)
{
    double w = q->w;
    double x = q->x;
    double y = q->y;
    double z = q->z;
    
    double normInv = 1/sqrt(w*w + x*x + y*y + z*z);
    
    q->w = w * normInv;
    q->x = x * normInv;
    q->y = y * normInv;
    q->z = z * normInv;
}

/**
 * Function: BB_QUAT_INV()
 * @param   q, qInv
 * @return 
 * @brief   Fills r with the inverse of q.
 * @author Daniel Bazor */
void BB_Quat_Inv(const Quat *q, Quat *qInv)
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
void BB_Quat_Mult(const Quat *q, const Quat *r, Quat *result)
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
void BB_Quat_Rotate(const Quat *q, const Quat *vector, Quat *result)
{
    Quat qInv, temp;
    BB_Quat_Inv(q, &qInv);

    BB_Quat_Mult(q, vector, &temp);
    BB_Quat_Mult(&temp, &qInv, result);
}

/**
 * Function: BB_QUAT_TIP_VECTOR()
 * @param   q, result
 * @return  
 * @brief   Does quaternion rotation for special case of up-vector
 * @precond q must be a unit quaternion     
 * @author Daniel Bazor */
void BB_Quat_Tip_Vector(const Quat *q, Quat *result)
{
    Quat qInv, temp, upVec;
    BB_Quat_Inv(q, &qInv); // fills q-inverse

    BB_Quat_Fill(0, 0, 0, 1, &upVec); // fills up-vector
    BB_Quat_Fill(-q->z, q->y, -q->x, q->w, &temp); // acts as first quat mult

    BB_Quat_Mult(&temp, &qInv, result); // the normalizing quat mult
}

/**
 * Function: BB_QUAT_FIND_TIP_ANGLE_X()
 * @param   tipVect
 * @return  double tip angle in x
 * @brief   
 * @precond      
 * @author Daniel Bazor */
double BB_Quat_Find_Tip_Angle_X(const Quat *tipVect)
{
    return RadiansToDegrees(atan2(tipVect->x, tipVect->z));
    //return atan(tipVect->x / tipVect->z);
}

/**
 * Function: BB_QUAT_FIND_TIP_ANGLE_Y()
 * @param   tipVect
 * @return  double tip angle in y
 * @brief   
 * @precond      
 * @author Daniel Bazor */
double BB_Quat_Find_Tip_Angle_Y(const Quat *tipVect)
{
    return RadiansToDegrees(atan2(tipVect->y, tipVect->z)); // negative to match our coordinate convention
    //return atan(tipVect->y / tipVect->z);
}

/**
 * Function: BB_QUAT_Print()
 * @param   q
 * @return  
 * @brief   Prints the quaternion passed by reference followed by a newLine in format [w x y z]'
 * @author Daniel Bazor */
double BB_Quat_Print(const Quat *q){
        printf("[%f %f %f %f]'\n",
            q->w,
            q->x,
            q->y,
            q->z);
}