/* #include "algorithms\quaternion.h" */

#ifndef __QUATERNION_H
#define __QUATERNION_H

#include "stm32f4xx.h"
#include "algorithms\mathUnit.h"
/*====================================================================================================*/
/*====================================================================================================*/
typedef struct {
  float32_t q0;
  float32_t q1;
  float32_t q2;
  float32_t q3;
  float32_t rMat[3][3];
} __attribute__((aligned(4))) quaternion_t;
/*====================================================================================================*/
/*====================================================================================================*/
void Quaternion_Clear( quaternion_t *pNumQ );
void Quaternion_ToNumQ( quaternion_t *pNumQ, eulerAngle_t *pAngE );
void Quaternion_ToAngE( quaternion_t *pNumQ, eulerAngle_t *pAngE );
void Quaternion_UpdateRolMatrix( quaternion_t *pNumQ );
void Quaternion_Multiply( quaternion_t *newQ, quaternion_t *nowQ, quaternion_t *oldQ );
void Quaternion_Normalize( quaternion_t *pNumQ );
void Quaternion_RungeKutta( quaternion_t *pNumQ, float32_t *gyro, float32_t helfTime );
/*====================================================================================================*/
/*====================================================================================================*/
#endif
