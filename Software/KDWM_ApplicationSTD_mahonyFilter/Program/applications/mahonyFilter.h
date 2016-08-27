/* #include "mahonyFilter.h" */

#ifndef __MAHONYFILTER_H
#define __MAHONYFILTER_H

#include "stm32f4xx.h"
#include "modules\imu.h"
#include "algorithms\quaternion.h"
/*====================================================================================================*/
/*====================================================================================================*/
typedef struct {
  float32_t sampleRate;
  float32_t sampleRateHelf;

  float32_t ki;
  float32_t kp;

  quaternion_t numQ;
  eulerAngle_t angE;

  IMU_DataTypeDef *imu;
} __attribute__((aligned)) mahonyFilter_t;
/*====================================================================================================*/
/*====================================================================================================*/
void MahonyFilter_Init( mahonyFilter_t *mf );
void MahonyFilter_Update( mahonyFilter_t *mf );
/*====================================================================================================*/
/*====================================================================================================*/
#endif	
