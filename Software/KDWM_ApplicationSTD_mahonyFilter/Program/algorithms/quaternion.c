/*====================================================================================================*/
/*====================================================================================================*/
#include "drivers\stm32f4_system.h"
#include "algorithms\quaternion.h"
/*====================================================================================================*/
/*====================================================================================================*/
#define USE_ARM_MATH_LIB
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Quaternion_Clear
**功能 : Clear Quaternion Parameter
**輸入 : *pNumQ
**輸出 : None
**使用 : Quaternion_Clear(&NumQ);
**====================================================================================================*/
/*====================================================================================================*/
void Quaternion_Clear( quaternion_t *pNumQ )
{
  pNumQ->q0 = 1.0f;
  pNumQ->q1 = 0.0f;
  pNumQ->q2 = 0.0f;
  pNumQ->q3 = 0.0f;

  for(uint8_t i = 0; i < 3; i++) {
    for(uint8_t j = 0; j < 3; j++) {
      pNumQ->rMat[i][j] = 0.0f;
    }
  }
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Quaternion_ToNumQ
**功能 : Euler Angle to Quaternion
**輸入 : *pNumQ, *pAngE
**輸出 : None
**使用 : Quaternion_ToNumQ(&NumQ, &AngE);
**====================================================================================================*/
/*====================================================================================================*/
void Quaternion_ToNumQ( quaternion_t *pNumQ, eulerAngle_t *pAngE )
{
  float halfP = pAngE->pitch / 2.0f;
  float halfR = pAngE->roll / 2.0f;
  float halfY = pAngE->yaw / 2.0f;

#ifdef USE_ARM_MATH_LIB
  float sinP = arm_sin_f32(halfP);
  float cosP = arm_cos_f32(halfP);
  float sinR = arm_sin_f32(halfR);
  float cosR = arm_cos_f32(halfR);
  float sinY = arm_sin_f32(halfY);
  float cosY = arm_cos_f32(halfY);
#else
  float sinP = sinf(halfP);
  float cosP = cosf(halfP);
  float sinR = sinf(halfR);
  float cosR = cosf(halfR);
  float sinY = sinf(halfY);
  float cosY = cosf(halfY);
#endif

  pNumQ->q0 = cosY * cosR * cosP + sinY * sinR * sinP;
  pNumQ->q1 = cosY * cosR * sinP - sinY * sinR * cosP;
  pNumQ->q2 = cosY * sinR * cosP + sinY * cosR * sinP;
  pNumQ->q3 = sinY * cosR * cosP - cosY * sinR * sinP;
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Quaternion_ToAngE
**功能 : Quaternion to Euler Angle
**輸入 : *pNumQ, *pAngE
**輸出 : None
**使用 : Quaternion_ToAngE(&NumQ, &AngE);
**====================================================================================================*/
/*====================================================================================================*/
void Quaternion_ToAngE( quaternion_t *pNumQ, eulerAngle_t *pAngE )
{
#ifdef USE_ARM_MATH_LIB
  pAngE->pitch = -arm_sin_f32(pNumQ->rMat[0][2]);
#else
  pAngE->pitch = -asinf(pNumQ->rMat[0][2]);
#endif
  pAngE->roll  = atan2f(pNumQ->rMat[1][2], pNumQ->rMat[2][2]);
  pAngE->yaw   = atan2f(pNumQ->rMat[0][1], pNumQ->rMat[0][0]);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Quaternion_UpdateRolMatrix
**功能 : Update Rolation Matrix
**輸入 : *pNumQ
**輸出 : None
**使用 : Quaternion_UpdateRolMatrix(&NumQ);
**====================================================================================================*/
/*====================================================================================================*/
void Quaternion_UpdateRolMatrix( quaternion_t *pNumQ )
{
  float Mq1q1 = pNumQ->q1 * pNumQ->q1;
  float Mq2q2 = pNumQ->q2 * pNumQ->q2;
  float Mq3q3 = pNumQ->q3 * pNumQ->q3;
  float Mq0q1 = pNumQ->q0 * pNumQ->q1;
  float Mq0q2 = pNumQ->q0 * pNumQ->q2;
  float Mq0q3 = pNumQ->q0 * pNumQ->q3;
  float Mq1q2 = pNumQ->q1 * pNumQ->q2;
  float Mq1q3 = pNumQ->q1 * pNumQ->q3;
  float Mq2q3 = pNumQ->q2 * pNumQ->q3;

  pNumQ->rMat[0][0] = 1.0f - 2.0f * (Mq2q2 + Mq3q3);
  pNumQ->rMat[0][1] = 2.0f * ( Mq0q3 + Mq1q2);
  pNumQ->rMat[0][2] = 2.0f * (-Mq0q2 + Mq1q3);
  pNumQ->rMat[1][0] = 2.0f * (-Mq0q3 + Mq1q2);
  pNumQ->rMat[1][1] = 1.0f - 2.0f * (Mq1q1 + Mq3q3);
  pNumQ->rMat[1][2] = 2.0f * ( Mq0q1 + Mq2q3);
  pNumQ->rMat[2][0] = 2.0f * ( Mq0q2 + Mq1q3);
  pNumQ->rMat[2][1] = 2.0f * (-Mq0q1 + Mq2q3);
  pNumQ->rMat[2][2] = 1.0f - 2.0f * (Mq1q1 + Mq2q2);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Quaternion_Multiply
**功能 : Quaternion Multiply
**輸入 : newQ, nowQ, oldQ
**輸出 : None
**使用 : Quaternion_Multiply(newQ, nowQ, oldQ);
**====================================================================================================*/
/*====================================================================================================*/
void Quaternion_Multiply( quaternion_t *newQ, quaternion_t *nowQ, quaternion_t *oldQ )
{
  if(newQ == nowQ) {
    quaternion_t tmpQ;
    tmpQ.q0 = nowQ->q0;
    tmpQ.q1 = nowQ->q1;
    tmpQ.q2 = nowQ->q2;
    tmpQ.q3 = nowQ->q3;
    newQ->q0 = tmpQ.q0 * oldQ->q0 - tmpQ.q1 * oldQ->q1 - tmpQ.q2 * oldQ->q2 - tmpQ.q3 * oldQ->q3;
    newQ->q1 = tmpQ.q0 * oldQ->q1 + tmpQ.q1 * oldQ->q0 + tmpQ.q2 * oldQ->q3 - tmpQ.q3 * oldQ->q2;
    newQ->q2 = tmpQ.q0 * oldQ->q2 - tmpQ.q1 * oldQ->q3 + tmpQ.q2 * oldQ->q0 + tmpQ.q3 * oldQ->q1;
    newQ->q3 = tmpQ.q0 * oldQ->q3 + tmpQ.q1 * oldQ->q2 - tmpQ.q2 * oldQ->q1 + tmpQ.q3 * oldQ->q0;
  }
  else {
    newQ->q0 = nowQ->q0 * oldQ->q0 - nowQ->q1 * oldQ->q1 - nowQ->q2 * oldQ->q2 - nowQ->q3 * oldQ->q3;
    newQ->q1 = nowQ->q0 * oldQ->q1 + nowQ->q1 * oldQ->q0 + nowQ->q2 * oldQ->q3 - nowQ->q3 * oldQ->q2;
    newQ->q2 = nowQ->q0 * oldQ->q2 - nowQ->q1 * oldQ->q3 + nowQ->q2 * oldQ->q0 + nowQ->q3 * oldQ->q1;
    newQ->q3 = nowQ->q0 * oldQ->q3 + nowQ->q1 * oldQ->q2 - nowQ->q2 * oldQ->q1 + nowQ->q3 * oldQ->q0;
  }

  Quaternion_Normalize(newQ);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Quaternion_Normalize
**功能 : Quaternion Normalize
**輸入 : *pNumQ
**輸出 : None
**使用 : Quaternion_Normalize(&NewQ);
**====================================================================================================*/
/*====================================================================================================*/
void Quaternion_Normalize( quaternion_t *pNumQ )
{
  float norm = invSqrtf(squa(pNumQ->q0) + squa(pNumQ->q1) + squa(pNumQ->q2) + squa(pNumQ->q3));

  pNumQ->q0 = pNumQ->q0 * norm;
  pNumQ->q1 = pNumQ->q1 * norm;
  pNumQ->q2 = pNumQ->q2 * norm;
  pNumQ->q3 = pNumQ->q3 * norm;
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Quaternion_RungeKutta
**功能 : 1st Order Runge Kutta to Update Quaternion
**輸入 : *pNumQ, gyrX, gyrY, gyrZ, helfTimes
**輸出 : None
**使用 : Quaternion_RungeKutta(&NumQ, gyrX, gyrY, gyrZ, helfT);
**====================================================================================================*/
/*====================================================================================================*/
void Quaternion_RungeKutta( quaternion_t *pNumQ, float gyrX, float gyrY, float gyrZ, float helfTimes )
{
  float tq0 = pNumQ->q0;
  float tq1 = pNumQ->q1;
  float tq2 = pNumQ->q2;
  float tq3 = pNumQ->q3;

  pNumQ->q0 = pNumQ->q0 + (-tq1 * gyrX - tq2 * gyrY - tq3 * gyrZ) * helfTimes;
  pNumQ->q1 = pNumQ->q1 + ( tq0 * gyrX - tq3 * gyrY + tq2 * gyrZ) * helfTimes;
  pNumQ->q2 = pNumQ->q2 + ( tq3 * gyrX + tq0 * gyrY - tq1 * gyrZ) * helfTimes;
  pNumQ->q3 = pNumQ->q3 + (-tq2 * gyrX + tq1 * gyrY + tq0 * gyrZ) * helfTimes;
}
/*====================================================================================================*/
/*====================================================================================================*/
