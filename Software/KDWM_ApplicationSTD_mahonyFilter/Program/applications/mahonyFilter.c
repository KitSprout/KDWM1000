/*====================================================================================================*/
/*====================================================================================================*/
#include "drivers\stm32f4_system.h"
#include "applications\mahonyFilter.h"

#include "kdwm1000.h"
/*====================================================================================================*/
/*====================================================================================================*/
#define MF_KP 1.0f
//#define MF_KI 0.001f

void MahonyFilter_Init( mahonyFilter_t *mf )
{
  mf->sampleRateHelf = mf->sampleRate / 2.0f;

  mf->kp = MF_KP;

#if defined(MF_KI)
  mf->ki = mf->sampleRate * MF_KI;

#else
  mf->ki = 0.0f;

#endif

  mf->angE.pitch = 0.0f;
  mf->angE.roll  = 0.0f;
  mf->angE.yaw   = 0.0f;

  Quaternion_Clear(&mf->numQ);
}
/*====================================================================================================*/
/*====================================================================================================*/
//#define MAHONY_FILTER_9DOF
void MahonyFilter_Update( mahonyFilter_t *mf )
{
  float gVect[3] = {0.0f};
  float err[3] = {0.0f}, gyr[3] = {0.0f}, acc[3] = {0.0f};
  float normAcc = invSqrtf(squa(mf->imu->accData[0]) + squa(mf->imu->accData[1]) + squa(mf->imu->accData[2]));

#if defined(MAHONY_FILTER_9DOF)
  float mag[3] = {0.0f};
  float hVect[2] = {0.0f}, bVect[2] = {0.0f}, wVect[3] = {0.0f};
  float normMag = invSqrtf(squa(mf->imu->MagF[0]) + squa(mf->imu->MagF[1]) + squa(mf->imu->MagF[2]));
#endif

#if defined(MF_KI)
  static float errInt[3] = {0.0f};
#endif

  /* Normalise accelerometer measurement */
  acc[0] = mf->imu->accData[0] * normAcc;
  acc[1] = mf->imu->accData[1] * normAcc;
  acc[2] = mf->imu->accData[2] * normAcc;

  /* Estimated direction of gravity */
  gVect[0] = mf->numQ.rMat[0][2];
  gVect[1] = mf->numQ.rMat[1][2];
  gVect[2] = mf->numQ.rMat[2][2];

#if defined(MAHONY_FILTER_9DOF)
  /* Normalise magnetometer measurement */
  mag[0] = mf->imu->MagF[0] * normMag;
  mag[1] = mf->imu->MagF[1] * normMag;
  mag[2] = mf->imu->MagF[2] * normMag;

  /* Reference direction of Earth's magnetic field */
  hVect[0] = mag[0] * mf->numQ.rMat[0][0] + mag[1] * mf->numQ.rMat[1][0] + mag[2] * mf->numQ.rMat[2][0];
  hVect[1] = mag[0] * mf->numQ.rMat[0][1] + mag[1] * mf->numQ.rMat[1][1] + mag[2] * mf->numQ.rMat[2][1];
  bVect[0] = sqrtf(squa(hVect[0]) + squa(hVect[1]));
  bVect[1] = mag[0] * mf->numQ.rMat[0][2] + mag[1] * mf->numQ.rMat[1][2] + mag[2] * mf->numQ.rMat[2][2];

  /* Estimated direction of magnetic field */
  wVect[0] = bVect[0] * mf->numQ.rMat[0][0] + bVect[1] * mf->numQ.rMat[0][2];
  wVect[1] = bVect[0] * mf->numQ.rMat[1][0] + bVect[1] * mf->numQ.rMat[1][2];
  wVect[2] = bVect[0] * mf->numQ.rMat[2][0] + bVect[1] * mf->numQ.rMat[2][2];

  /* Error is sum of cross product between estimated direction and measured direction of field vectors */
  err[0] = (acc[1] * gVect[2] - acc[2] * gVect[1]) + (mag[1] * wVect[2] - mag[2] * wVect[1]);
  err[1] = (acc[2] * gVect[0] - acc[0] * gVect[2]) + (mag[2] * wVect[0] - mag[0] * wVect[2]);
  err[2] = (acc[0] * gVect[1] - acc[1] * gVect[0]) + (mag[0] * wVect[1] - mag[1] * wVect[0]);

#else
  err[0] = (acc[1] * gVect[2] - acc[2] * gVect[1]);
  err[1] = (acc[2] * gVect[0] - acc[0] * gVect[2]);
  err[2] = (acc[0] * gVect[1] - acc[1] * gVect[0]);

#endif

#if defined(MF_KI)
  /* Compute and apply integral feedback */
  errInt[0] += mf->ki * err[0];
  errInt[1] += mf->ki * err[1];
  errInt[2] += mf->ki * err[2];

  /* Apply proportional feedback */
  gyr[0] = toRad(mf->imu->gyrData[0]) + mf->kp * err[0] + errInt[0];
  gyr[1] = toRad(mf->imu->gyrData[1]) + mf->kp * err[1] + errInt[1];
  gyr[2] = toRad(mf->imu->gyrData[2]) + mf->kp * err[2] + errInt[2];

#else
  gyr[0] = toRad(mf->imu->gyrData[0]) + mf->kp * err[0];
  gyr[1] = toRad(mf->imu->gyrData[1]) + mf->kp * err[1];
  gyr[2] = toRad(mf->imu->gyrData[2]) + mf->kp * err[2];

#endif

  /* Integrate rate of change of quaternion */
  Quaternion_RungeKutta(&mf->numQ, gyr, mf->sampleRateHelf);
  Quaternion_Normalize(&mf->numQ);
  Quaternion_UpdateRolMatrix(&mf->numQ);
  Quaternion_ToAngE(&mf->numQ, &mf->angE);

//  // Get Euler Angle */
//  mf->angE.pitch = -toDeg(mf->angE.pitch);
//  mf->angE.roll  = toDeg(mf->angE.roll);
//  mf->angE.yaw   = -toDeg(mf->angE.yaw);
}
/*====================================================================================================*/
/*====================================================================================================*/
