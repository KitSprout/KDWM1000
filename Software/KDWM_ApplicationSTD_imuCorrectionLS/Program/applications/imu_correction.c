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
double accCorrData[CORR_SAMPLE * 3] = {0};
double accCorrParam[12] = {0.0};

void IMU_CorrLeastSquares( double *pArray, double *pArrayParam )
{
  double accSum[9] = {0};

  for(uint8_t i = 0; i < 6; i++) {
//    printf(" %f\t%f\t%f\r\n", pArray[3 * i + 0], pArray[3 * i + 1], pArray[3 * i + 2]);
    accSum[0] += pArray[3 * i + 0] * pArray[3 * i + 0];   // X^2
    accSum[1] += pArray[3 * i + 1] * pArray[3 * i + 1];   // Y^2
    accSum[2] += pArray[3 * i + 2] * pArray[3 * i + 2];   // Z^2
    accSum[3] += pArray[3 * i + 0] * pArray[3 * i + 1];   // XY
    accSum[4] += pArray[3 * i + 1] * pArray[3 * i + 2];   // YZ
    accSum[5] += pArray[3 * i + 2] * pArray[3 * i + 0];   // ZX
    accSum[6] += pArray[3 * i + 0];                       // X
    accSum[7] += pArray[3 * i + 1];                       // Y
    accSum[8] += pArray[3 * i + 2];                       // Z
  }

//  printf("\r\n--- Print Matrix acc_sum\r\n");
//  for(uint8_t i = 0; i < 9; i++) {
//    printf("%f\r\n", accSum[i]);
//  }

//  printf("\r\n--- Print Matrix_Create matrixM\r\n");
  matrix_t *matrixM = Matrix_Create(4, 4);
  Matrix_SetData(matrixM, 0, 0, accSum[0]);
  Matrix_SetData(matrixM, 0, 1, accSum[3]);
  Matrix_SetData(matrixM, 0, 2, accSum[5]);
  Matrix_SetData(matrixM, 0, 3, accSum[6]);
  Matrix_SetData(matrixM, 1, 0, accSum[3]);
  Matrix_SetData(matrixM, 1, 1, accSum[1]);
  Matrix_SetData(matrixM, 1, 2, accSum[4]);
  Matrix_SetData(matrixM, 1, 3, accSum[7]);
  Matrix_SetData(matrixM, 2, 0, accSum[5]);
  Matrix_SetData(matrixM, 2, 1, accSum[4]);
  Matrix_SetData(matrixM, 2, 2, accSum[2]);
  Matrix_SetData(matrixM, 2, 3, accSum[8]);
  Matrix_SetData(matrixM, 3, 0, accSum[6]);
  Matrix_SetData(matrixM, 3, 1, accSum[7]);
  Matrix_SetData(matrixM, 3, 2, accSum[8]);
  Matrix_SetData(matrixM, 3, 3, 6);

//  printf("\r\n--- Print Matrix_Create matrixN\r\n");
  matrix_t *matrixN = Matrix_Create(4, 3);
  Matrix_SetData(matrixN, 0, 0, pArray[3 * 4 + 0] - pArray[3 * 5 + 0]);   // X5 - X6
  Matrix_SetData(matrixN, 0, 1, pArray[3 * 2 + 0] - pArray[3 * 3 + 0]);   // X3 - X4
  Matrix_SetData(matrixN, 0, 2, pArray[3 * 0 + 0] - pArray[3 * 1 + 0]);   // X1 - X2
  Matrix_SetData(matrixN, 1, 0, pArray[3 * 4 + 1] - pArray[3 * 5 + 1]);   // Y5 - Y6
  Matrix_SetData(matrixN, 1, 1, pArray[3 * 2 + 1] - pArray[3 * 3 + 1]);   // Y3 - Y4
  Matrix_SetData(matrixN, 1, 2, pArray[3 * 0 + 1] - pArray[3 * 1 + 1]);   // Y1 - Y2
  Matrix_SetData(matrixN, 2, 0, pArray[3 * 4 + 2] - pArray[3 * 5 + 2]);   // Z5 - Z6
  Matrix_SetData(matrixN, 2, 1, pArray[3 * 2 + 2] - pArray[3 * 3 + 2]);   // Z3 - Z4
  Matrix_SetData(matrixN, 2, 2, pArray[3 * 0 + 2] - pArray[3 * 1 + 2]);   // Z1 - Z2
  Matrix_SetData(matrixN, 3, 0, 0.0);
  Matrix_SetData(matrixN, 3, 1, 0.0);
  Matrix_SetData(matrixN, 3, 2, 0.0);
  Matrix_MulNumb(matrixN, matrixN, 8192);
  Matrix_Print(matrixN);

//  printf("\r\n--- Print Matrix_Inv\r\n");
  Matrix_Inv(matrixM, matrixM);
//  Matrix_Print(matrixM);

//  printf("\r\n--- Print Matrix_Mul\r\n");
  matrix_t *matrixP = Matrix_Create(4, 3);
  Matrix_Mul(matrixP, matrixM, matrixN);
//  Matrix_Print(matrixP);

  pArrayParam[0]  = Matrix_GetData(matrixP, 0, 0);  // S11
  pArrayParam[1]  = Matrix_GetData(matrixP, 1, 0);  // S12
  pArrayParam[2]  = Matrix_GetData(matrixP, 2, 0);  // S13
  pArrayParam[3]  = Matrix_GetData(matrixP, 0, 1);  // S21
  pArrayParam[4]  = Matrix_GetData(matrixP, 1, 1);  // S22
  pArrayParam[5]  = Matrix_GetData(matrixP, 2, 1);  // S23
  pArrayParam[6]  = Matrix_GetData(matrixP, 0, 2);  // S31
  pArrayParam[7]  = Matrix_GetData(matrixP, 1, 2);  // S22
  pArrayParam[8]  = Matrix_GetData(matrixP, 2, 2);  // S33
  pArrayParam[9]  = Matrix_GetData(matrixP, 3, 0);  // B1
  pArrayParam[10] = Matrix_GetData(matrixP, 3, 1);  // B2
  pArrayParam[11] = Matrix_GetData(matrixP, 3, 2);  // B3

  Matrix_Delete(matrixM);
  Matrix_Delete(matrixN);
  Matrix_Delete(matrixP);
}

#define MABUF_LENS 200

int16_t *MABUF_X, *MABUF_Y, *MABUF_Z;
static CorrectState_TypeDef correctState = IMU_CORR_START;

int8_t IMU_Correction( IMU_DataTypeDef *pIMU, const uint16_t sampleRateFreq )
{
  static uint32_t correctTimes = 0;

  uint16_t corrStep = 0;

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
      corrStep = (uint16_t)(correctTimes / sampleRateFreq);
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
      corrStep = (uint16_t)(correctTimes / (sampleRateFreq << 1));
      switch(corrStep) {
        case 0:   // Step 1, clear buffer
          memset(MABUF_X, 0, MABUF_LENS * sizeof(int16_t));
          memset(MABUF_Y, 0, MABUF_LENS * sizeof(int16_t));
          memset(MABUF_Z, 0, MABUF_LENS * sizeof(int16_t));
          correctTimes = (sampleRateFreq << 1);
          break;
        case 1:   // Step 2, simple moving average & wait to fill buffer
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
          if((correctTimes % (sampleRateFreq << 1)) == 0) {
            LED_B_Toggle();
            printf("[%2d]\r\n", corrStep);
          }
          accCorrData[3 * (corrStep - 1) + 0] = MoveAve_SMA(pIMU->Acc[0], MABUF_X, MABUF_LENS, MABUF_LENS >> 2);
          accCorrData[3 * (corrStep - 1) + 1] = MoveAve_SMA(pIMU->Acc[1], MABUF_Y, MABUF_LENS, MABUF_LENS >> 2);
          accCorrData[3 * (corrStep - 1) + 2] = MoveAve_SMA(pIMU->Acc[2], MABUF_Z, MABUF_LENS, MABUF_LENS >> 2);
          correctTimes++;
          break;
        case 7:   // Step 3, calculate parameter
          LED_R_Set();
          LED_G_Set();
          LED_B_Set();
          IMU_CorrLeastSquares(accCorrData, accCorrParam);
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
