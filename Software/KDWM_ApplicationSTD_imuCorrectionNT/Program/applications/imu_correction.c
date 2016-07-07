/*====================================================================================================*/
/*====================================================================================================*/
#include "drivers\stm32f4_system.h"
#include "modules\serial.h"

#include "algorithms\moveAve.h"
#include "algorithms\newtonMethod.h"
#include "applications\imu_correction.h"

#include <string.h>
#include "kdwm1000.h"
/*====================================================================================================*/
/*====================================================================================================*/
//extern IMU_DataTypeDef IMU;

int16_t *MABUF_X, *MABUF_Y, *MABUF_Z;
int16_t *MABUF_X2, *MABUF_Y2, *MABUF_Z2;

#define CORR_SAMPLE 16
double accCorrData[CORR_SAMPLE * 3] = {0};
double magCorrData[CORR_SAMPLE * 3] = {0};

#define row_X 9
#define col_X 1
double arrayX[row_X * col_X] = { 1.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 0.0 };

Matrix_TypeDef matrixX   = { row_X, col_X, arrayX, MSTATE_NONE };
Matrix_TypeDef matrixAcc = { 3, CORR_SAMPLE, accCorrData, MSTATE_NONE };
Matrix_TypeDef matrixMag = { 3, CORR_SAMPLE, magCorrData, MSTATE_NONE };

static CorrectState_TypeDef correctState = IMU_CORR_START;

#define MABUF_LENS 200
int8_t IMU_Correction( IMU_DataTypeDef *pIMU, const uint16_t sampleRateFreq )
{
  static uint32_t correctTimes = 0;

  uint16_t corrStep = 0;

  switch(correctState) {

    /************************** Correct Start ********************************/
    case IMU_CORR_START:
      MABUF_X  = (int16_t*)malloc(MABUF_LENS * sizeof(int16_t));
      MABUF_Y  = (int16_t*)malloc(MABUF_LENS * sizeof(int16_t));
      MABUF_Z  = (int16_t*)malloc(MABUF_LENS * sizeof(int16_t));
      MABUF_X2 = (int16_t*)malloc(MABUF_LENS * sizeof(int16_t));
      MABUF_Y2 = (int16_t*)malloc(MABUF_LENS * sizeof(int16_t));
      MABUF_Z2 = (int16_t*)malloc(MABUF_LENS * sizeof(int16_t));
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
          memset(MABUF_X,  0, MABUF_LENS * sizeof(int16_t));
          memset(MABUF_Y,  0, MABUF_LENS * sizeof(int16_t));
          memset(MABUF_Z,  0, MABUF_LENS * sizeof(int16_t));
          memset(MABUF_X2, 0, MABUF_LENS * sizeof(int16_t));
          memset(MABUF_Y2, 0, MABUF_LENS * sizeof(int16_t));
          memset(MABUF_Z2, 0, MABUF_LENS * sizeof(int16_t));
          correctTimes = (sampleRateFreq << 1);
          break;
        case 1:   // Step 2, simple moving average & wait to fill buffer
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
          if((correctTimes % (sampleRateFreq << 1)) == 0) {
            LED_B_Toggle();
            printf("[%2d]\r\n", corrStep);
          }
          accCorrData[CORR_SAMPLE * 0 + corrStep - 1] = (int16_t)MoveAve_SMA(pIMU->Acc[0], MABUF_X,  MABUF_LENS, MABUF_LENS >> 2);
          accCorrData[CORR_SAMPLE * 1 + corrStep - 1] = (int16_t)MoveAve_SMA(pIMU->Acc[1], MABUF_Y,  MABUF_LENS, MABUF_LENS >> 2);
          accCorrData[CORR_SAMPLE * 2 + corrStep - 1] = (int16_t)MoveAve_SMA(pIMU->Acc[2], MABUF_Z,  MABUF_LENS, MABUF_LENS >> 2);
          magCorrData[CORR_SAMPLE * 0 + corrStep - 1] = (int16_t)MoveAve_SMA(pIMU->Mag[0], MABUF_X2, MABUF_LENS, MABUF_LENS >> 2);
          magCorrData[CORR_SAMPLE * 1 + corrStep - 1] = (int16_t)MoveAve_SMA(pIMU->Mag[1], MABUF_Y2, MABUF_LENS, MABUF_LENS >> 2);
          magCorrData[CORR_SAMPLE * 2 + corrStep - 1] = (int16_t)MoveAve_SMA(pIMU->Mag[2], MABUF_Z2, MABUF_LENS, MABUF_LENS >> 2);
          correctTimes++;
          break;
        case 17:  // Step 3, calculate parameter
          LED_R_Set();
          LED_G_Set();
          LED_B_Set();
          NewtonMethod(&matrixX, &matrixAcc, 0.4, 8192);
          Matrix_print(&matrixAcc);
          pIMU->AccGain[0]   = (float)arrayX[0];
          pIMU->AccGain[1]   = (float)arrayX[1];
          pIMU->AccGain[2]   = (float)arrayX[2];
          pIMU->AccGain[3]   = (float)arrayX[3];
          pIMU->AccGain[4]   = (float)arrayX[4];
          pIMU->AccGain[5]   = (float)arrayX[5];
          pIMU->AccOffset[0] = (int16_t)arrayX[6];
          pIMU->AccOffset[1] = (int16_t)arrayX[7];
          pIMU->AccOffset[2] = (int16_t)arrayX[8];
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
      free(MABUF_X2);
      free(MABUF_Y2);
      free(MABUF_Z2);
      printf("\r- arrayX --------\r\n");
      for(uint8_t i = 0; i < 9; i++)
        printf("%f\r\n", arrayX[i]);
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
