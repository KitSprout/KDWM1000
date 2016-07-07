/* #include "applications\imu_correction.h" */

#ifndef __IMU_CORRECTION_H
#define __IMU_CORRECTION_H

#include "modules\imu.h"
/*====================================================================================================*/
/*====================================================================================================*/
typedef enum {
  IMU_START = 0,
  IMU_CORR_START,
  IMU_CORR_GYRO,
  IMU_CORR_ACC_MAG,
  IMU_CORR_END,
  IMU_END
} CorrectState_TypeDef;

int8_t IMU_Correction( IMU_DataTypeDef *pIMU, const uint16_t sampleRateFreq );
/*====================================================================================================*/
/*====================================================================================================*/
#endif
