/*====================================================================================================*/
/*====================================================================================================*/
#include "drivers\stm32f4_system.h"
#include "modules\serial.h"
#include "modules\imu.h"
#include "algorithms\moveAve.h"
#include "applications\mahonyFilter.h"

#include <stdlib.h>
#include <string.h>

#include "kdwm1000.h"
/*====================================================================================================*/
/*====================================================================================================*/
void IMU_EvenCallBack( void );

extern IMU_DataTypeDef IMU;

mahonyFilter_t mf = {
  .sampleRate = SAMPLE_RATE,
  .imu        = &IMU
};

void KDWM_Init( void )
{
  KDWM_GPIO_Config();
  KDWM_UART_Config(NULL);
  KDWM_IMU_Config(IMU_EvenCallBack);

  MahonyFilter_Init(&mf);
}
/*====================================================================================================*/
/*====================================================================================================*/
float32_t gyro[3]     = {0};
float32_t accel[3]    = {0};
float32_t magnet[3]   = {0};
float32_t ictemp      = 0;
float32_t attitude[3] = {0};

void KDWM_Loop( void )
{
  IMU_MergeScaleCalib(&IMU);
  IMU_PrintData(&IMU);

  TIM_Cmd(TIM4, ENABLE);

  while(1) {
    LED_G_Toggle();
    delay_ms(100);

    printf("GX:%.4f\tGY:%.4f\tGZ:%.4f\tAX:%.4f\tAY:%.4f\tAZ:%.4f\tMX:%.3f\tMY:%.3f\tMZ:%.3f\tT:%.3f\r\n",
      attitude[0], attitude[1], attitude[2],
//      gyro[0],     gyro[1],     gyro[2],
      accel[0],    accel[1],    accel[2],
      magnet[0],   magnet[1],   magnet[2],
      ictemp
    );
  }
}
/*====================================================================================================*/
/*====================================================================================================*/
void IMU_EvenCallBack( void )
{
  IMU_GetCalibData(mf.imu);
  MahonyFilter_Update(&mf);

  gyro[0]     = IMU.gyrData[0];
  gyro[1]     = IMU.gyrData[1];
  gyro[2]     = IMU.gyrData[2];
  accel[0]    = IMU.accData[0];
  accel[1]    = IMU.accData[1];
  accel[2]    = IMU.accData[2];
  magnet[0]   = IMU.magData[0];
  magnet[1]   = IMU.magData[1];
  magnet[2]   = IMU.magData[2];
  ictemp      = IMU.ictempData;
  attitude[0] = mf.angE.pitch;
  attitude[1] = mf.angE.roll;
  attitude[2] = mf.angE.yaw;
}
/*====================================================================================================*/
/*====================================================================================================*/
