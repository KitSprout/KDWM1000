/*====================================================================================================*/
/*====================================================================================================*/
#include "drivers\stm32f4_system.h"
#include "modules\serial.h"
#include "modules\imu.h"
#include "algorithms\moveAve.h"
#include "applications\imu_param.h"
#include "applications\imu_correction.h"
#include "applications\mahonyFilter.h"

#include <string.h>
#include "kdwm1000.h"
/*====================================================================================================*/
/*====================================================================================================*/
void IMU_EvenCallBack( void );

mahonyFilter_t mf;
IMU_DataTypeDef IMU;

__IO SensorState_TypeDef SensorState    = SENSOR_CORR_SEL;
__IO SensorState_TypeDef SensorStateOld = SENSOR_CORR_SEL;
/*====================================================================================================*/
/*====================================================================================================*/
void KDWM_Init( void )
{
  HAL_InitTick();

  KDWM_GPIO_Config();
  KDWM_UART_Config(NULL);
  KDWM_IMU_Config(IMU_EvenCallBack);

  mf.sampleRate = SAMPLE_RATE;
  mf.imu        = &IMU;
  MahonyFilter_Init(&mf);
}
/*====================================================================================================*/
/*====================================================================================================*/
IMU_DataTypeDef testIMU;
void KDWM_Loop( void )
{
  // wait to finish correct
  while(SensorState != SENSOR_ALG);
  LED_R_Set();
  LED_G_Set();
  LED_B_Set();

  // print imu data
  IMU_PrintData(&IMU);

  while(1) {
    LED_G_Toggle();
    delay_ms(100);

    IMU_GetDataCorr(mf.imu);
    printf("GX: %.4f\tGY: %.4f\tGZ: %.4f\tAX: %.4f\tAY: %.4f\tAZ: %.4f\tAP: %.4f\tAR: %.4f\tAY: %.4f\r\n",
      mf.imu->GyrF[0], mf.imu->GyrF[1], mf.imu->GyrF[2],
      mf.imu->AccF[0], mf.imu->AccF[1], mf.imu->AccF[2],
      mf.angE.pitch, mf.angE.roll, mf.angE.yaw
    );
  }
}
/*====================================================================================================*/
/*====================================================================================================*/
void IMU_EvenCallBack( void )
{
  switch(SensorState) {

    /************************** Correction Select *******************************/
    case SENSOR_CORR_SEL:
      SensorStateOld = SENSOR_CORR_SEL;
      SensorState    = (KEY_Read()) ? SENSOR_CORR : SENSOR_FLASH;
      break;

    /************************** Correction End **********************************/
    case SENSOR_CORR:
      if(IMU_Correction(&IMU, SAMPLE_RATE_FREQ) == SUCCESS) {
        SensorStateOld = SENSOR_CORR;
        SensorState    = SENSOR_FLASH;
      }
      break;

    /**************************** Flash *****************************************/
    case SENSOR_FLASH:
      if(SensorStateOld == SENSOR_CORR) {
        /* write to flash */
        IMU_ParamClearSector();
        if(IMU_ParamWrite(&IMU) != SUCCESS) {
          printf("IMU_ParamWrite ... ERROR\r\n");
        }
      }
      /* read from flash */
      if(IMU_ParamRead(&IMU) != SUCCESS) {
        printf("IMU_ParamRead ... ERROR\r\n");
      }
      SensorStateOld = SENSOR_FLASH;
      SensorState    = SENSOR_ALG;
      break;

    /************************** Algorithm ***************************************/
    case SENSOR_ALG:
      IMU_GetDataCorrF(mf.imu);
      MahonyFilter_Update(&mf);
      break;

    /************************** Debug *******************************************/
    case SENSOR_DEBUG:
//      IMU_getData(&IMU);
      break;

    /************************** Error *******************************************/
    default:
      printf("\fERROR!!!\r\n");
      while(1);
  }
}
/*====================================================================================================*/
/*====================================================================================================*/
