#ifndef BB_QUAT_H
#define BB_QUAT_H

/*******************************************************************************
 * #INCLUDES                                                                   *
 ******************************************************************************/
#include "BB_BOARD.h"
#include <math.h>

/*******************************************************************************
 * PUBLIC #DEFINES                                                             *
 ******************************************************************************/
#define DegreesToRadians(angleDegrees) (angleDegrees * M_PI / 180.0)
#define RadiansToDegrees(angleRadians) (angleRadians * 180.0 / M_PI)

/*******************************************************************************
 * PUBLIC Variables                                                            *
 ******************************************************************************/
typedef struct Quat {
    double w;
    double x;
    double y;
    double z;
} Quat;

/*******************************************************************************
 * PUBLIC FUNCTION PROTOTYPES                                                  *
 ******************************************************************************/

/**
 * Function: BB_QUAT_FILL()
 * @param   double w, double x, double y, double z, Quat *q
 * @return 
 * @brief   Fills r with the inverse of q.
 * @author Daniel Bazor */
void BB_Quat_Fill(double w, double x, double y, double z, Quat *q);


/**
 * Function: BB_QUAT_INV()
 * @param   q, qInv
 * @return 
 * @brief   Fills r with the inverse of q.
 * @author Daniel Bazor */
void BB_Quat_Inv(const Quat *q, Quat *qInv);

/**
 * Function: BB_QUAT_MULT()
 * @param    p, q, result
 * @return  None
 * @brief 
 * @author Daniel Bazor */
void BB_Quat_Mult(const Quat *p, const Quat *q, Quat *result);

/**
 * Function: BB_QUAT_ROTATE()
 * @param   p, vector, result
 * @return  
 * @brief   
 * @author Daniel Bazor */
void BB_Quat_Rotate(const Quat *q, const Quat *vector, Quat *result);

/**
 * Function: BB_QUAT_TIP_VECTOR()
 * @param   q, result
 * @return  
 * @brief   
 * @author Daniel Bazor */
void BB_Quat_Tip_Vector(const Quat *q, Quat *result);

/**
 * Function: BB_QUAT_FIND_TIP_ANGLE_X()
 * @param   tipVect
 * @return  double tip angle in x
 * @brief   
 * @precond      
 * @author Daniel Bazor */
double BB_Quat_Find_Tip_Angle_X(const Quat *tipVect);

/**
 * Function: BB_QUAT_FIND_TIP_ANGLE_Y()
 * @param   tipVect
 * @return  double tip angle in y
 * @brief   
 * @precond      
 * @author Daniel Bazor */
double BB_Quat_Find_Tip_Angle_Y(const Quat *tipVect);

/**
 * Function: BB_QUAT_Print()
 * @param   q
 * @return  
 * @brief   Prints the quaternion passed by reference   
 * @author Daniel Bazor */
double BB_Quat_Print(const Quat *q);
#endif