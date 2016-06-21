/* #include "modules\imu.h" */

#ifndef __IMU_H
#define __IMU_H

#include "stm32f4xx.h"

#include "modules\mpu9250.h"
#include "modules\lps25hb.h"
/*=====================================================================================================*/
/*=====================================================================================================*/
/*
|     |            ACCELEROMETER           |
| LPF |  BandW  |  Delay  | Noise | Sample |
+-----+---------+---------+-------+--------+
|  x  |  1130Hz |  0.75ms | 250Hz |  4kHz  |
|  0  |   460Hz |  1.94ms | 250Hz |  1kHz  |
|  1  |   184Hz |  5.80ms | 250Hz |  1kHz  |
|  2  |    92Hz |  7.80ms | 250Hz |  1kHz  |
|  3  |    41Hz | 11.80ms | 250Hz |  1kHz  |
|  4  |    20Hz | 19.80ms | 250Hz |  1kHz  |
|  5  |    10Hz | 35.70ms | 250Hz |  1kHz  |
|  6  |     5Hz | 66.96ms | 250Hz |  1kHz  |
|  7  |   460Hz |  1.94ms | 250Hz |  1kHz  |
*/
/*
|     |          GYROSCOPE         |    TEMPERATURE    |
| LPF |  BandW  |  Delay  | Sample |  BandW  |  Delay  |
+-----+---------+---------+--------+---------+---------+
|  x  |  8800Hz | 0.064ms |  32kHz |  4000Hz |  0.04ms |
|  x  |  3600Hz |  0.11ms |  32kHz |  4000Hz |  0.04ms |
|  0  |   250Hz |  0.97ms |   8kHz |  4000Hz |  0.04ms |
|  1  |   184Hz |   2.9ms |   1kHz |   188Hz |   1.9ms |
|  2  |    92Hz |   3.9ms |   1kHz |    98Hz |   2.8ms |
|  3  |    41Hz |   5.9ms |   1kHz |    42Hz |   4.8ms |
|  4  |    20Hz |   9.9ms |   1kHz |    20Hz |   8.3ms |
|  5  |    10Hz | 17.85ms |   1kHz |    10Hz |  13.4ms |
|  6  |     5Hz | 33.48ms |   1kHz |     5Hz |  18.6ms |
|  7  |  3600Hz |  0.17ms |   8kHz |  4000Hz |  0.04ms |
*/
/*
| ODR |  Pres  |  Temp  |
+-----+--------+--------+
|  0  | One-Shot Enable |
|  1  |    1Hz |    1Hz |
|  2  |    7Hz |    7Hz |
|  3  | 12.5Hz | 12.5Hz |
|  4  |   25Hz |   25Hz |
|  5  |     Reserved    |
*/

typedef enum {
  MPU_GyrAcc_ENABLE   = ENABLE,
  MPU_GyrAcc_DISABLE  = DISABLE
} MPU_GyrAccEN_TypeDef;

#ifdef __USE_MAGNETOMETER
typedef enum {
  MPU_Mag_ENABLE      = ENABLE,
  MPU_Mag_DISABLE     = DISABLE
} MPU_MagEN_TypeDef;
#endif

#ifdef __USE_BAROMETER
typedef enum {
  LPS_PresTemp_ENABLE  = ENABLE,
  LPS_PresTemp_DISABLE = DISABLE
} LPS_PresTempEN_TypeDef;
#endif

typedef __IO struct {
  MPU_GyrAccEN_TypeDef    MPU_GyrAcc_Enable;

  int16_t Gyr[3];
  int16_t Acc[3];
  int16_t ICTemp;

  float GyrOffset[3];
  float AccOffset[3];
  float GyrGain[3];
  float AccGain[9];

  float GyrSens;
  float AccSens;
  float ICTempSens;

  float GyrC[3];
  float AccC[3];

  float GyrF[3];
  float AccF[3];
  float ICTempF;

#ifdef __USE_MAGNETOMETER
  MPU_MagEN_TypeDef       MPU_Mag_Enable;

  int16_t Mag[3];
  int16_t MagASA[3];

  float MagOffset[3];
  float MagGain[9];

  float MagSens;

  float MagC[3];

  float MagF[3];
#endif

#ifdef __USE_BAROMETER
  LPS_PresTempEN_TypeDef  LPS_PresTemp_Enable;

  int16_t Temp;
  int32_t Pres;

  int16_t TempOffset;
  int32_t PresOffset;
  float TempGain;
  float PresGain;

  float TempSens;
  float PresSens;

  float TempF;
  float PresF;
#endif

} IMU_DataTypeDef;

typedef struct {
  MPU_ConfigTypeDef InitMPU;
#ifdef __USE_BAROMETER
  LPS_ConfigTypeDef InitLPS;
#endif
  IMU_DataTypeDef *pIMU;
} IMU_InitTypeDef;
/*=====================================================================================================*/
/*=====================================================================================================*/
void   IMU_SetSpeed( uint8_t speedSel );

void   IMU_ClaerData( IMU_DataTypeDef *pIMU );
void   IMU_SetSensitivity( IMU_InitTypeDef *IMUx );

void   IMU_Config( void );
int8_t IMU_Init( IMU_InitTypeDef *IMUx );
int8_t IMU_DeviceCheck( IMU_DataTypeDef *pIMU );

void   IMU_GetData( IMU_DataTypeDef *pIMU );
void   IMU_GetDataCorr( IMU_DataTypeDef *pIMU );
void   IMU_GetDataCorrF( IMU_DataTypeDef *pIMU );
void   IMU_ConvToPhy( IMU_DataTypeDef *pIMU );

void   IMU_PrintData( IMU_DataTypeDef *pIMU );
/*=====================================================================================================*/
/*=====================================================================================================*/
#endif
