/*====================================================================================================*/
/*====================================================================================================*/
#include "drivers\stm32f4_system.h"
#include "modules\serial.h"

#include "algorithms\moveAve.h"
#include "algorithms\matrix.h"
#include "applications\imu_correction.h"

#include <string.h>
#include "kdwm1000.h"
/*====================================================================================================*/
/*====================================================================================================*/
#define CORR_SAMPLE 6
double accCorrData[CORR_SAMPLE * 4] = {
  0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
  0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
  0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
  1.0, 1.0, 1.0, 1.0, 1.0, 1.0
};
double accCorrParam[12] = {0.0};

void IMU_CorrLeastSquares( double *arrayA, double *pArrayParam, double scale )
{
  double arrayZ[CORR_SAMPLE * 3] = {
    0.0,  0.0,  0.0,  0.0,  1.0, -1.0,
    0.0,  0.0,  1.0, -1.0,  0.0,  0.0,
    1.0, -1.0,  0.0,  0.0,  0.0,  0.0
  };
  double arrayS[3 * 4] = {
    0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0
  };

  matrix_t *matrixZ = Matrix_CreatePtr(arrayZ, 3, CORR_SAMPLE);
  matrix_t *matrixA = Matrix_CreatePtr(arrayA, 4, CORR_SAMPLE);
  matrix_t *matrixS = Matrix_CreatePtr(arrayS, 3, 4);

  Matrix_MulNumb(matrixZ, matrixZ, scale);

  matrix_t *matrixA_tr = Matrix_CopyMatrix(matrixA);
  Matrix_Transpose(matrixA_tr, matrixA_tr);

  matrix_t *matrixM = Matrix_Create(3, 4);
  Matrix_Mul(matrixM, matrixZ, matrixA_tr);   // M = Z * A'
  
  matrix_t *matrixN = Matrix_Create(4, 4);
  Matrix_Mul(matrixN, matrixA, matrixA_tr);   // N = A * A'
  Matrix_Inv(matrixN, matrixN);               // N = inv(N) = inv(A * A')

  Matrix_Mul(matrixS, matrixM, matrixN);      // S = M * N -> S = (Z * A') * inv(A * A')

  pArrayParam[0]  = arrayS[0];
  pArrayParam[1]  = arrayS[1];
  pArrayParam[2]  = arrayS[2];
  pArrayParam[3]  = arrayS[4];
  pArrayParam[4]  = arrayS[5];
  pArrayParam[5]  = arrayS[6];
  pArrayParam[6]  = arrayS[8];
  pArrayParam[7]  = arrayS[9];
  pArrayParam[8]  = arrayS[10];
  pArrayParam[9]  = arrayS[3];
  pArrayParam[10] = arrayS[7];
  pArrayParam[11] = arrayS[11];
  Matrix_Print(matrixS);

  Matrix_Delete(matrixZ);
  Matrix_Delete(matrixA);
  Matrix_Delete(matrixS);
  Matrix_Delete(matrixA_tr);
  Matrix_Delete(matrixM);
  Matrix_Delete(matrixN);
}

#define MABUF_LENS        200
#define CORR_SAMPLE_TIME  4
int16_t *MABUF_X, *MABUF_Y, *MABUF_Z;
static CorrectState_TypeDef correctState = IMU_CORR_START;

int8_t IMU_Correction( IMU_DataTypeDef *pIMU, const uint16_t sampleRateFreq )
{
  static uint32_t correctTimes = 0;

  uint32_t corrStep = 0;

  switch(correctState) {

    /************************** Correct Start ********************************/
    case IMU_CORR_START:
      MABUF_X = (int16_t*)malloc(MABUF_LENS * sizeof(int16_t));
      MABUF_Y = (int16_t*)malloc(MABUF_LENS * sizeof(int16_t));
      MABUF_Z = (int16_t*)malloc(MABUF_LENS * sizeof(int16_t));
      correctState = IMU_CORR_GYRO;
      break;

    /************************** Correct Gyro (RED) ***************************/
    case IMU_CORR_GYRO:
      IMU_GetData(pIMU);
      corrStep = (uint32_t)(correctTimes / sampleRateFreq);
      switch(corrStep) {
        case 0:   // Step 1, clear buffer
          LED_R_Reset();
          LED_G_Set();
          LED_B_Set();
          memset(MABUF_X, 0, MABUF_LENS * sizeof(int16_t));
          memset(MABUF_Y, 0, MABUF_LENS * sizeof(int16_t));
          memset(MABUF_Z, 0, MABUF_LENS * sizeof(int16_t));
          correctTimes = sampleRateFreq;
          break;
        case 1:   // Step 2, simple moving average & wait to fill buffer
        case 2:
          pIMU->Gyr[0] = MoveAve_SMA(pIMU->Gyr[0], MABUF_X, MABUF_LENS, MABUF_LENS >> 2);
          pIMU->Gyr[1] = MoveAve_SMA(pIMU->Gyr[1], MABUF_Y, MABUF_LENS, MABUF_LENS >> 2);
          pIMU->Gyr[2] = MoveAve_SMA(pIMU->Gyr[2], MABUF_Z, MABUF_LENS, MABUF_LENS >> 2);
          correctTimes++;
          break;
        case 3:   // Step 3, calculate offset
          LED_R_Set();
          LED_G_Set();
          LED_B_Set();
          pIMU->GyrOffset[0] = pIMU->Gyr[0];
          pIMU->GyrOffset[1] = pIMU->Gyr[1];
          pIMU->GyrOffset[2] = pIMU->Gyr[2];
          pIMU->GyrGain[0]   = pIMU->GyrSens;
          pIMU->GyrGain[1]   = pIMU->GyrSens;
          pIMU->GyrGain[2]   = pIMU->GyrSens;
          correctTimes = 0;
          correctState = IMU_CORR_ACC_MAG;
//          correctState = IMU_CORR_END;
          break;
      }
      break;

    /************************** Correct Acc & Mag (BLUE) *********************/
    case IMU_CORR_ACC_MAG:
      IMU_GetData(pIMU);
      corrStep = (uint32_t)(correctTimes / (sampleRateFreq * CORR_SAMPLE_TIME));
      switch(corrStep) {
        case 0:   // Step 1, clear buffer
          memset(MABUF_X, 0, MABUF_LENS * sizeof(int16_t));
          memset(MABUF_Y, 0, MABUF_LENS * sizeof(int16_t));
          memset(MABUF_Z, 0, MABUF_LENS * sizeof(int16_t));
          correctTimes = (sampleRateFreq * CORR_SAMPLE_TIME);
          break;
        case 1:   // Step 2, simple moving average & wait to fill buffer
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
          if((correctTimes % (sampleRateFreq * CORR_SAMPLE_TIME)) == 0) {
            LED_B_Toggle();
            printf("[%2d]\r\n", corrStep);
          }
          accCorrData[0 * CORR_SAMPLE + corrStep - 1] = MoveAve_SMA(pIMU->Acc[0], MABUF_X, MABUF_LENS, MABUF_LENS >> 2);
          accCorrData[1 * CORR_SAMPLE + corrStep - 1] = MoveAve_SMA(pIMU->Acc[1], MABUF_Y, MABUF_LENS, MABUF_LENS >> 2);
          accCorrData[2 * CORR_SAMPLE + corrStep - 1] = MoveAve_SMA(pIMU->Acc[2], MABUF_Z, MABUF_LENS, MABUF_LENS >> 2);
          correctTimes++;
          break;
        case 7:   // Step 3, calculate parameter
          LED_R_Set();
          LED_G_Set();
          LED_B_Set();
          IMU_CorrLeastSquares(accCorrData, accCorrParam, 1000);
          for(uint8_t i = 0; i < 9; i++)
            pIMU->AccGain[i] = accCorrParam[i];
          pIMU->AccOffset[0] = accCorrParam[9];
          pIMU->AccOffset[1] = accCorrParam[10];
          pIMU->AccOffset[2] = accCorrParam[11];
          correctTimes = 0;
          correctState = IMU_CORR_END;
          break;

        default:
          printf("STEP_ERROR!!!\r\n");
          break;
      }
      break;

    /************************** Correction End **********************************/
    case IMU_CORR_END:
      free(MABUF_X);
      free(MABUF_Y);
      free(MABUF_Z);
      printf("\r\n- accCorrParam --------\r\n");
      for(uint8_t i = 0; i < 12; i++)
        printf("%f\r\n", accCorrParam[i]);
      return SUCCESS;

    /************************** Error *******************************************/
    default:
      printf("\fERROR!!!\r\n");
      while(1);
  }
  return ERROR;
}
/*====================================================================================================*/
/*====================================================================================================*/
