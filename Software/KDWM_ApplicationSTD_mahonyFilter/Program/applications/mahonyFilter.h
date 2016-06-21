/* #include "mahonyFilter.h" */

#ifndef __MAHONYFILTER_H
#define __MAHONYFILTER_H

#include "stm32f4xx.h"
#include "modules\imu.h"
#include "algorithms\quaternion.h"
/*====================================================================================================*/
/*====================================================================================================*/
typedef struct {
  float sampleRate;
  float sampleRateHelf;

  float ki;
  float kp;

  float acc[3];
  float gyr[3];

  quaternion_t numQ;
  eulerAngle_t angE;

  IMU_DataTypeDef *imu;
} mahonyFilter_t;
/*====================================================================================================*/
/*====================================================================================================*/
void MahonyFilter_Init( mahonyFilter_t *mf );
void MahonyFilter_Update( mahonyFilter_t *mf );
/*====================================================================================================*/
/*====================================================================================================*/
#endif	
