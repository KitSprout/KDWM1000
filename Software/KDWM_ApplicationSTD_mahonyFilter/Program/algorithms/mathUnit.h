/* #include "algorithms\mathUnit.h" */

#ifndef __MATHUNIT_H
#define __MATHUNIT_H

#include "stm32f4xx.h"
#include "arm_math.h"
/*====================================================================================================*/
/*====================================================================================================*/
#define invSqrt(__iSq)    (1.0 / sqrt(__iSq))
#define invSqrtf(__iSq)   (1.0f / sqrtf(__iSq))
#define squa(__Sq)        ((__Sq) * (__Sq))
#define toRad(__mathD)    ((__mathD) * 0.0174532925199432957692369076849f)
#define toDeg(__mathR)    ((__mathR) * 57.295779513082320876798154814096f)

typedef struct {
  float32_t pitch;
  float32_t roll;
  float32_t yaw;
} __attribute__((aligned(4))) eulerAngle_t;
/*====================================================================================================*/
/*====================================================================================================*/
#endif
