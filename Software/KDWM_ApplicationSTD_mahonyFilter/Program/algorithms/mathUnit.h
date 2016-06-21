/* #include "algorithms\mathUnit.h" */

#ifndef __MATHUNIT_H
#define __MATHUNIT_H

#include "stm32f4xx.h"
#include "arm_math.h"
/*====================================================================================================*/
/*====================================================================================================*/
#define invSqrt(__iSq)    (1.0f / sqrt(__iSq))
#define invSqrtf(__iSq)   (1.0f / sqrtf(__iSq))
#define squa(__Sq)        ((__Sq) * (__Sq))
#define toRad(__mathD)    (__mathD * 0.0174532925f)
#define toDeg(__mathR)    (__mathR * 57.2957795f)

typedef struct {
  float pitch;
  float roll;
  float yaw;
} eulerAngle_t;
/*====================================================================================================*/
/*====================================================================================================*/
#endif
