/*====================================================================================================*/
/*====================================================================================================*/
#include "drivers\stm32f4_system.h"
#include "drivers\stm32f4_spi.h"
#include "modules\imu.h"
/*====================================================================================================*/
/*====================================================================================================*/
#define IMU_SPIx                    SPI1
#define IMU_SPIx_CLK_ENABLE()       RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE)
#define IMU_SPIx_SPEED_HIGH         SPI_BaudRatePrescaler_2
#define IMU_SPIx_SPEED_LOW          SPI_BaudRatePrescaler_256

#define IMU_CSM_PIN                 GPIO_Pin_4
#define IMU_CSM_GPIO_PORT           GPIOA
#define IMU_CSM_H()                 __GPIO_SET(IMU_CSM_GPIO_PORT, IMU_CSM_PIN)
#define IMU_CSM_L()                 __GPIO_RST(IMU_CSM_GPIO_PORT, IMU_CSM_PIN)

#ifdef __USE_BAROMETER
#define IMU_CSB_PIN                 GPIO_Pin_0
#define IMU_CSB_GPIO_PORT           GPIOB
#define IMU_CSB_H()                 __GPIO_SET(IMU_CSB_GPIO_PORT, IMU_CSB_PIN)
#define IMU_CSB_L()                 __GPIO_RST(IMU_CSB_GPIO_PORT, IMU_CSB_PIN)
#endif

#define IMU_SCK_PIN                 GPIO_Pin_5
#define IMU_SCK_GPIO_PORT           GPIOA
#define IMU_SCK_AF                  GPIO_AF_SPI1
#define IMU_SCK_SOURCE              GPIO_PinSource5

#define IMU_SDO_PIN                 GPIO_Pin_6
#define IMU_SDO_GPIO_PORT           GPIOA
#define IMU_SDO_AF                  GPIO_AF_SPI1
#define IMU_SDO_SOURCE              GPIO_PinSource6

#define IMU_SDI_PIN                 GPIO_Pin_7
#define IMU_SDI_GPIO_PORT           GPIOA
#define IMU_SDI_AF                  GPIO_AF_SPI1
#define IMU_SDI_SOURCE              GPIO_PinSource7

#define IMU_INTM_PIN                GPIO_Pin_1
#define IMU_INTM_GPIO_PORT          GPIOA
#define IMU_INTM_IRQ                EXTI1_IRQn
#define IMU_INTM_LINE               EXTI_Line1
#define IMU_INTM_SOURCE             GPIO_PinSource1

#ifdef __USE_BAROMETER
#define IMU_INTB_PIN                GPIO_Pin_0
#define IMU_INTB_GPIO_PORT          GPIOA
#define IMU_INTB_IRQ                EXTI0_IRQn
#define IMU_INTB_LINE               EXTI_Line0
#define IMU_INTB_SOURCE             GPIO_PinSource0
#endif
/*====================================================================================================*/
/*====================================================================================================*
**函數 : IMU_SetSpeed
**功能 : Set SPI Speed
**輸入 : SpeedSel
**輸出 : None
**使用 : IMU_SetSpeed(SPIx_SPEED_LOW);
**====================================================================================================*/
/*====================================================================================================*/
void IMU_SetSpeed( uint8_t speedSel )
{
  SPI_InitTypeDef SPI_InitStruct;

  SPI_I2S_DeInit(IMU_SPIx);

  SPI_InitStruct.SPI_Mode              = SPI_Mode_Master;
  SPI_InitStruct.SPI_Direction         = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStruct.SPI_DataSize          = SPI_DataSize_8b;
  SPI_InitStruct.SPI_CPOL              = SPI_CPOL_High;
  SPI_InitStruct.SPI_CPHA              = SPI_CPHA_2Edge;
  SPI_InitStruct.SPI_NSS               = SPI_NSS_Soft;
  SPI_InitStruct.SPI_BaudRatePrescaler = speedSel;
  SPI_InitStruct.SPI_FirstBit          = SPI_FirstBit_MSB;
  SPI_InitStruct.SPI_CRCPolynomial     = 7;
  SPI_Init(IMU_SPIx, &SPI_InitStruct);

  SPI_Cmd(IMU_SPIx, ENABLE); 
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : IMU_ClaerData
**功能 : Clear Data
**輸入 : *pIMU
**輸出 : None
**使用 : IMU_ClaerData(IMU);
**====================================================================================================*/
/*====================================================================================================*/
void IMU_ClaerData( IMU_DataTypeDef *pIMU )
{
  for(uint8_t i = 0; i < 3; i++ ) {
    pIMU->Gyr[i] = 0;
    pIMU->Acc[i] = 0;
    pIMU->GyrOffset[i] = 0;
    pIMU->AccOffset[i] = 0;
    pIMU->GyrF[i] = 0.0f;
    pIMU->AccF[i] = 0.0f;
#ifdef __USE_MAGNETOMETER
    pIMU->Mag[i] = 0;
    pIMU->MagASA[i] = 0;
    pIMU->MagOffset[i] = 0;
    pIMU->MagF[i] = 0.0f;
#endif
  }

  pIMU->GyrGain[0] = 1.0f;
  pIMU->GyrGain[1] = 1.0f;
  pIMU->GyrGain[2] = 1.0f;

  pIMU->AccGain[0] = 1.0f;
  pIMU->AccGain[1] = 0.0f;
  pIMU->AccGain[2] = 0.0f;
  pIMU->AccGain[3] = 1.0f;
  pIMU->AccGain[4] = 0.0f;
  pIMU->AccGain[5] = 1.0f;

  pIMU->GyrSens = 0.0f;
  pIMU->AccSens = 0.0f;

#ifdef __USE_MAGNETOMETER
  pIMU->MagGain[0] = 1.0f;
  pIMU->MagGain[1] = 0.0f;
  pIMU->MagGain[2] = 0.0f;
  pIMU->MagGain[3] = 1.0f;
  pIMU->MagGain[4] = 0.0f;
  pIMU->MagGain[5] = 1.0f;

  pIMU->MagSens = 0.0f;
#endif

#ifdef __USE_BAROMETER
  pIMU->Temp = 0;
  pIMU->Pres = 0;
  pIMU->TempOffset = 0;
  pIMU->PresOffset = 0;
  pIMU->TempGain = 1.0f;
  pIMU->PresGain = 1.0f;

  pIMU->TempSens = 0.0f;
  pIMU->PresSens = 0.0f;
  pIMU->TempF = 0.0f;
  pIMU->PresF = 0.0f;
#endif
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : IMU_SetSensitivity
**功能 : Set Sensitivity
**輸入 : *IMUx
**輸出 : None
**使用 : IMU_SetSensitivity(IMUx);
**====================================================================================================*/
/*====================================================================================================*/
void IMU_SetSensitivity( IMU_InitTypeDef *IMUx )
{
  if(IMUx->pIMU->MPU_GyrAcc_Enable == MPU_GyrAcc_ENABLE) {
    switch(IMUx->InitMPU.MPU_Gyr_FullScale) { // dps/LSB
      case MPU_GyrFS_250dps:  IMUx->pIMU->GyrSens = MPU9250G_250dps;  break;
      case MPU_GyrFS_500dps:  IMUx->pIMU->GyrSens = MPU9250G_500dps;  break;
      case MPU_GyrFS_1000dps: IMUx->pIMU->GyrSens = MPU9250G_1000dps; break;
      case MPU_GyrFS_2000dps: IMUx->pIMU->GyrSens = MPU9250G_2000dps; break;
      default :               IMUx->pIMU->GyrSens = MPU9250G_250dps;  break;
    }
    switch(IMUx->InitMPU.MPU_Acc_FullScale) { // g/LSB
      case MPU_AccFS_2g:  IMUx->pIMU->AccSens = MPU9250A_2g;  break;
      case MPU_AccFS_4g:  IMUx->pIMU->AccSens = MPU9250A_4g;  break;
      case MPU_AccFS_8g:  IMUx->pIMU->AccSens = MPU9250A_8g;  break;
      case MPU_AccFS_16g: IMUx->pIMU->AccSens = MPU9250A_16g; break;
      default :           IMUx->pIMU->AccSens = MPU9250A_2g;  break;
    }
    IMUx->pIMU->ICTempSens = MPU9250T_85degC; // degC/LSB
  }

#ifdef __USE_MAGNETOMETER
  if(IMUx->pIMU->MPU_Mag_Enable == MPU_Mag_ENABLE) {
    IMUx->pIMU->MagSens    = MPU9250M_4800uT; // uT/LSB
  }
#endif

#ifdef __USE_BAROMETER
  if(IMUx->pIMU->LPS_PresTemp_Enable == LPS_PresTemp_ENABLE) {
    IMUx->pIMU->TempSens   = LPS25HB_T_degC;  // degC/LSB
    IMUx->pIMU->PresSens   = LPS25HB_P_hPa;   // hPa/LSB
  }
#endif
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : IMU_Config
**功能 : IMU Config
**輸入 : None
**輸出 : None
**使用 : IMU_Config();
**====================================================================================================*/
/*====================================================================================================*/
void IMU_Config( void )
{
  GPIO_InitTypeDef GPIO_InitStruct;
  SPI_InitTypeDef SPI_InitStruct;
//  NVIC_InitTypeDef NVIC_InitStruct;
//  EXTI_InitTypeDef EXTI_InitStruct;

  /* SPI Clk ******************************************************************/
  IMU_SPIx_CLK_ENABLE();

  /* SPI AF *******************************************************************/
  GPIO_PinAFConfig(IMU_SCK_GPIO_PORT, IMU_SCK_SOURCE, IMU_SCK_AF);
  GPIO_PinAFConfig(IMU_SDO_GPIO_PORT, IMU_SDO_SOURCE, IMU_SDO_AF);    
  GPIO_PinAFConfig(IMU_SDI_GPIO_PORT, IMU_SDI_SOURCE, IMU_SDI_AF);

  /* SPI Pin ******************************************************************/
  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;

  GPIO_InitStruct.GPIO_Pin   = IMU_CSM_PIN;
  GPIO_Init(IMU_CSM_GPIO_PORT, &GPIO_InitStruct);

#ifdef __USE_BAROMETER
  GPIO_InitStruct.GPIO_Pin   = IMU_CSB_PIN;
  GPIO_Init(IMU_CSB_GPIO_PORT, &GPIO_InitStruct);
#endif

  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;

  GPIO_InitStruct.GPIO_Pin   = IMU_SCK_PIN;
  GPIO_Init(IMU_SCK_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.GPIO_Pin   = IMU_SDO_PIN;
  GPIO_Init(IMU_SDO_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.GPIO_Pin   = IMU_SDI_PIN;
  GPIO_Init(IMU_SDI_GPIO_PORT, &GPIO_InitStruct);

//  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IN;
//  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
//  GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
//  GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;

//  GPIO_InitStruct.GPIO_Pin   = IMU_INTM_PIN;
//  GPIO_Init(IMU_INTM_GPIO_PORT, &GPIO_InitStruct);

//  GPIO_InitStruct.GPIO_Pin   = IMU_INTB_PIN;
//  GPIO_Init(IMU_INTB_GPIO_PORT, &GPIO_InitStruct);

  IMU_CSM_H();  // LOW ENABLE
#ifdef __USE_BAROMETER
  IMU_CSB_H();  // LOW ENABLE
#endif

  /* INT NVIC ****************************************************************/
//  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
//  NVIC_InitStruct.NVIC_IRQChannel                   = SPIx_INT_IRQ;
//  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
//  NVIC_InitStruct.NVIC_IRQChannelSubPriority        = 0;
//  NVIC_InitStruct.NVIC_IRQChannelCmd                = ENABLE;
//  NVIC_Init(&NVIC_InitStruct);

  /* INT EXTI ****************************************************************/
//  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, SPIx_INT_SOURCE);
//  EXTI_InitStruct.EXTI_Line    = SPIx_INT_LINE;
//  EXTI_InitStruct.EXTI_Mode    = EXTI_Mode_Interrupt;
//  EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;  
//  EXTI_InitStruct.EXTI_LineCmd = ENABLE;
//  EXTI_Init(&EXTI_InitStruct);

  /* SPI Init ****************************************************************/
  SPI_InitStruct.SPI_Mode              = SPI_Mode_Master;
  SPI_InitStruct.SPI_Direction         = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStruct.SPI_DataSize          = SPI_DataSize_8b;
  SPI_InitStruct.SPI_CPOL              = SPI_CPOL_High;
  SPI_InitStruct.SPI_CPHA              = SPI_CPHA_2Edge;
  SPI_InitStruct.SPI_NSS               = SPI_NSS_Soft;
  SPI_InitStruct.SPI_BaudRatePrescaler = IMU_SPIx_SPEED_LOW;
  SPI_InitStruct.SPI_FirstBit          = SPI_FirstBit_MSB;
  SPI_InitStruct.SPI_CRCPolynomial     = 7;
  SPI_Init(IMU_SPIx, &SPI_InitStruct);

  SPI_Cmd(IMU_SPIx, ENABLE); 
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : IMU_Init
**功能 : IMU Init
**輸入 : None
**輸出 : None
**使用 : status = IMU_Init(&IMUx_InitStruct);
**====================================================================================================*/
/*====================================================================================================*/
#define MPU6500_InitRegNum 11
#define LPS25H_InitRegNum  2
int8_t IMU_Init( IMU_InitTypeDef *IMUx )
{
  int8_t status = ERROR;

  uint8_t i = 0;
  uint8_t MPU6500_InitData[MPU6500_InitRegNum][2] = {
    {0x80, MPU6500_PWR_MGMT_1},     // [0]  Reset Device
    {0x04, MPU6500_PWR_MGMT_1},     // [1]  Clock Source
    {0x10, MPU6500_INT_PIN_CFG},    // [2]  Set INT_ANYRD_2CLEAR
    {0x01, MPU6500_INT_ENABLE},     // [3]  Set RAW_RDY_EN
    {0x00, MPU6500_PWR_MGMT_2},     // [4]  Enable Acc & Gyro
    {0x00, MPU6500_SMPLRT_DIV},     // [5]  Sample Rate Divider
    {0x00, MPU6500_GYRO_CONFIG},    // [6]  default : +-250dps
    {0x00, MPU6500_ACCEL_CONFIG},   // [7]  default : +-2G
    {0x00, MPU6500_CONFIG},         // [8]  default : GyrLPS_250Hz
    {0x00, MPU6500_ACCEL_CONFIG_2}, // [9]  default : AccLPS_460Hz
    {0x30, MPU6500_USER_CTRL},      // [10] Set I2C_MST_EN, I2C_IF_DIS
  };

  IMU_ClaerData(IMUx->pIMU);
  IMU_SetSensitivity(IMUx);

  MPU6500_InitData[6][0] = IMUx->InitMPU.MPU_Gyr_FullScale;       // MPU6500_GYRO_CONFIG
  MPU6500_InitData[7][0] = IMUx->InitMPU.MPU_Acc_FullScale;       // MPU6500_ACCEL_CONFIG
  MPU6500_InitData[8][0] = IMUx->InitMPU.MPU_Gyr_LowPassFilter;   // MPU6500_CONFIG
  MPU6500_InitData[9][0] = IMUx->InitMPU.MPU_Acc_LowPassFilter;   // MPU6500_ACCEL_CONFIG_2

  for(i = 0; i < MPU6500_InitRegNum; i++) {
    delay_ms(2);
    MPU9250_WriteReg(MPU6500_InitData[i][1], MPU6500_InitData[i][0]);
  }

  delay_ms(2);
  status = IMU_DeviceCheck(IMUx->pIMU);
  if(status != SUCCESS)
    return ERROR;

  delay_ms(10);

#ifdef __USE_MAGNETOMETER
  uint8_t tmp[3] = {0};

  if(IMUx->pIMU->MPU_Mag_Enable == MPU_Mag_ENABLE) {
    MPU9250_Mag_WriteReg(AK8963_CNTL2, 0x01);     // Reset Device
    delay_ms(2);
    MPU9250_Mag_WriteReg(AK8963_CNTL1, 0x10);     // Power-down mode
    delay_ms(2);
    MPU9250_Mag_WriteReg(AK8963_CNTL1, 0x1F);     // Fuse ROM access mode
    delay_ms(2);
    MPU9250_Mag_ReadRegs(AK8963_ASAX, tmp, 3);    // Read sensitivity adjustment values
    delay_ms(2);
    MPU9250_Mag_WriteReg(AK8963_CNTL1, 0x10);     // Power-down mode
    delay_ms(2);

    IMUx->pIMU->MagASA[0] = (int16_t)(tmp[0]) + 128;
    IMUx->pIMU->MagASA[1] = (int16_t)(tmp[1]) + 128;
    IMUx->pIMU->MagASA[2] = (int16_t)(tmp[2]) + 128;

    MPU9250_WriteReg(MPU6500_I2C_MST_CTRL, 0x5D);
    delay_ms(2);
    MPU9250_WriteReg(MPU6500_I2C_SLV0_ADDR, AK8963_I2C_ADDR | 0x80);
    delay_ms(2);
    MPU9250_WriteReg(MPU6500_I2C_SLV0_REG, AK8963_ST1);
    delay_ms(2);
    MPU9250_WriteReg(MPU6500_I2C_SLV0_CTRL, MPU6500_I2C_SLVx_EN | 8);
    delay_ms(2);

    MPU9250_Mag_WriteReg(AK8963_CNTL1, 0x16);       // Continuous measurement mode 2
    delay_ms(2);

    MPU9250_WriteReg(MPU6500_I2C_SLV4_CTRL, 0x09);
    delay_ms(2);
    MPU9250_WriteReg(MPU6500_I2C_MST_DELAY_CTRL, 0x81);
    delay_ms(100);
  }
#endif

#ifdef __USE_BAROMETER
  if(IMUx->pIMU->LPS_PresTemp_Enable == LPS_PresTemp_ENABLE) {
//    for(uint8_t i = 0; i < LPS25H_InitRegNum; i++) {
//      Delay_1ms(2);
//      LPS25H_WriteReg(LPS25H_InitData[i][1], LPS25H_InitData[i][0]);
//    }
  }
#endif

  IMU_SetSpeed(IMU_SPIx_SPEED_HIGH);
  delay_ms(10);

  return SUCCESS;
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : IMU_DeviceCheck
**功能 : Check Device ID
**輸入 : *pIMU
**輸出 : status
**使用 : status = IMU_DeviceCheck(pIMU);
**====================================================================================================*/
/*====================================================================================================*/
int8_t IMU_DeviceCheck( IMU_DataTypeDef *pIMU )
{
  uint8_t deviceID = 0x00;

  if(pIMU->MPU_GyrAcc_Enable == MPU_GyrAcc_ENABLE) {
    deviceID = MPU9250_ReadReg(MPU6500_WHO_AM_I);
    if(deviceID != MPU6500_DeviceID)
      return ERROR;
  }

#ifdef __USE_MAGNETOMETER
  if(pIMU->MPU_Mag_Enable == MPU_Mag_ENABLE) {
    deviceID = MPU9250_Mag_ReadReg(AK8963_WIA);
    if(deviceID != AK8963_DeviceID)
      return ERROR;
  }
#endif

#ifdef __USE_BAROMETER
  if(pIMU->LPS_PresTemp_Enable == LPS_PresTemp_ENABLE) {
    deviceID = LPS25HB_ReadReg(LPS25HB_WHO_AM_I);
    if(deviceID != LPS25HB_DeviceID)
      return ERROR;
  }
#endif

  return SUCCESS;
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : IMU_GetData
**功能 : Get IMU Data
**輸入 : *pIMU
**輸出 : None
**使用 : IMU_GetData(pIMU);
**====================================================================================================*/
/*====================================================================================================*/
void IMU_GetData( IMU_DataTypeDef *pIMU )
{
  uint8_t tmpRead[22] = {0};

#ifdef __USE_MAGNETOMETER
  if((pIMU->MPU_GyrAcc_Enable == MPU_GyrAcc_ENABLE) && (pIMU->MPU_Mag_Enable == MPU_Mag_ENABLE)) {
    MPU9250_ReadRegs(MPU6500_ACCEL_XOUT_H, tmpRead, 22);    // Read Gyr, Acc, Mag
  }
  else {
#endif

    if(pIMU->MPU_GyrAcc_Enable == MPU_GyrAcc_ENABLE) {
      MPU9250_ReadRegs(MPU6500_ACCEL_XOUT_H, tmpRead, 14);    // Read Gyr, Acc
    }

#ifdef __USE_MAGNETOMETER
    else if(pIMU->MPU_Mag_Enable == MPU_Mag_ENABLE) {
      MPU9250_ReadRegs(MPU6500_EXT_SENS_DATA_00, tmpRead + 14, 8); // Read Mag
    }
  }
#endif

  if(pIMU->MPU_GyrAcc_Enable == MPU_GyrAcc_ENABLE) {
    pIMU->Gyr[0] = (Byte16(int16_t, tmpRead[8],  tmpRead[9]));    // Gyr.X
    pIMU->Gyr[1] = (Byte16(int16_t, tmpRead[10], tmpRead[11]));   // Gyr.Y
    pIMU->Gyr[2] = (Byte16(int16_t, tmpRead[12], tmpRead[13]));   // Gyr.Z
    pIMU->Acc[0] = (Byte16(int16_t, tmpRead[0],  tmpRead[1]));    // Acc.X
    pIMU->Acc[1] = (Byte16(int16_t, tmpRead[2],  tmpRead[3]));    // Acc.Y
    pIMU->Acc[2] = (Byte16(int16_t, tmpRead[4],  tmpRead[5]));    // Acc.Z
    pIMU->ICTemp = (Byte16(int16_t, tmpRead[6],  tmpRead[7]));    // Temp
  }

#ifdef __USE_MAGNETOMETER
  if(pIMU->MPU_Mag_Enable == MPU_Mag_ENABLE) {
    if(!(!(tmpRead[14] & AK8963_STATUS_DRDY) || (tmpRead[14] & AK8963_STATUS_DOR) || (tmpRead[21] & AK8963_STATUS_HOFL))) {
      pIMU->Mag[0] = (Byte16(int16_t, tmpRead[16], tmpRead[15]));   // Mag.X
      pIMU->Mag[1] = (Byte16(int16_t, tmpRead[18], tmpRead[17]));   // Mag.Y
      pIMU->Mag[2] = (Byte16(int16_t, tmpRead[20], tmpRead[19]));   // Mag.Z
    }
  }
#endif

#ifdef __USE_BAROMETER
  if(pIMU->LPS_PresTemp_Enable == LPS_PresTemp_ENABLE) {
//    LPS25H_ReadReg(LPS25H_WHO_AM_I, );
  }
#endif
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : IMU_GetDataCorr
**功能 : Get Correction Data
**輸入 : *pIMU
**輸出 : None
**使用 : IMU_GetDataCorr(pIMU);
**====================================================================================================*/
/*====================================================================================================*/
void IMU_GetDataCorr( IMU_DataTypeDef *pIMU )
{
  float tmp[3] = {0};

  IMU_GetData(pIMU);

  if(pIMU->MPU_GyrAcc_Enable == MPU_GyrAcc_ENABLE) {
    pIMU->GyrC[0] = (pIMU->Gyr[0] - pIMU->GyrOffset[0]) * pIMU->GyrGain[0];    // Gyr.X
    pIMU->GyrC[1] = (pIMU->Gyr[1] - pIMU->GyrOffset[1]) * pIMU->GyrGain[1];    // Gyr.Y
    pIMU->GyrC[2] = (pIMU->Gyr[2] - pIMU->GyrOffset[2]) * pIMU->GyrGain[2];    // Gyr.Z

    tmp[0] = pIMU->Acc[0] - pIMU->AccOffset[0];   // Acc.X
    tmp[1] = pIMU->Acc[1] - pIMU->AccOffset[1];   // Acc.Y
    tmp[2] = pIMU->Acc[2] - pIMU->AccOffset[2];   // Acc.Z

    pIMU->AccC[0] = pIMU->AccGain[0] * tmp[0] + pIMU->AccGain[1] * tmp[1] + pIMU->AccGain[2] * tmp[2];
    pIMU->AccC[1] = pIMU->AccGain[1] * tmp[0] + pIMU->AccGain[3] * tmp[1] + pIMU->AccGain[4] * tmp[2];
    pIMU->AccC[2] = pIMU->AccGain[2] * tmp[0] + pIMU->AccGain[4] * tmp[1] + pIMU->AccGain[5] * tmp[2];
  }

#ifdef __USE_MAGNETOMETER
    tmp[0] = pIMU->Mag[0] - pIMU->MagOffset[0];   // Mag.X
    tmp[1] = pIMU->Mag[1] - pIMU->MagOffset[1];   // Mag.Y
    tmp[2] = pIMU->Mag[2] - pIMU->MagOffset[2];   // Mag.Z

    pIMU->MagC[0] = pIMU->MagGain[0] * tmp[0] + pIMU->MagGain[1] * tmp[1] + pIMU->MagGain[2] * tmp[2];
    pIMU->MagC[1] = pIMU->MagGain[1] * tmp[0] + pIMU->MagGain[3] * tmp[1] + pIMU->MagGain[4] * tmp[2];
    pIMU->MagC[2] = pIMU->MagGain[2] * tmp[0] + pIMU->MagGain[4] * tmp[1] + pIMU->MagGain[5] * tmp[2];
#endif

#ifdef __USE_BAROMETER
  if(pIMU->LPS_PresTemp_Enable == LPS_PresTemp_ENABLE) {
    pIMU->Temp -= pIMU->TempOffset;   // Temp
    pIMU->Pres -= pIMU->PresOffset;   // Pres
  }
#endif
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : IMU_GetDataCorrF
**功能 : Get Correction Data
**輸入 : *pIMU
**輸出 : None
**使用 : IMU_GetDataCorrF(pIMU);
**====================================================================================================*/
/*====================================================================================================*/
void IMU_GetDataCorrF( IMU_DataTypeDef *pIMU )
{
  float tmp[3] = {0};

  IMU_GetData(pIMU);

  if(pIMU->MPU_GyrAcc_Enable == MPU_GyrAcc_ENABLE) {
    pIMU->GyrF[0] = (pIMU->Gyr[0] - pIMU->GyrOffset[0]) * pIMU->GyrGain[0];   // Gyr.X
    pIMU->GyrF[1] = (pIMU->Gyr[1] - pIMU->GyrOffset[1]) * pIMU->GyrGain[1];   // Gyr.Y
    pIMU->GyrF[2] = (pIMU->Gyr[2] - pIMU->GyrOffset[2]) * pIMU->GyrGain[2];   // Gyr.Z

    tmp[0] = pIMU->Acc[0] - pIMU->AccOffset[0];   // Acc.X
    tmp[1] = pIMU->Acc[1] - pIMU->AccOffset[1];   // Acc.Y
    tmp[2] = pIMU->Acc[2] - pIMU->AccOffset[2];   // Acc.Z

    pIMU->AccF[0] = pIMU->AccGain[0] * tmp[0] + pIMU->AccGain[1] * tmp[1] + pIMU->AccGain[2] * tmp[2];
    pIMU->AccF[1] = pIMU->AccGain[1] * tmp[0] + pIMU->AccGain[3] * tmp[1] + pIMU->AccGain[4] * tmp[2];
    pIMU->AccF[2] = pIMU->AccGain[2] * tmp[0] + pIMU->AccGain[4] * tmp[1] + pIMU->AccGain[5] * tmp[2];

    pIMU->ICTempF = pIMU->ICTemp * pIMU->ICTempSens;
  }

#ifdef __USE_MAGNETOMETER
  if(pIMU->MPU_Mag_Enable == MPU_Mag_ENABLE) {
    tmp[0] = pIMU->Mag[0] - pIMU->MagOffset[0];   // Mag.X
    tmp[1] = pIMU->Mag[1] - pIMU->MagOffset[1];   // Mag.Y
    tmp[2] = pIMU->Mag[2] - pIMU->MagOffset[2];   // Mag.Z

    pIMU->MagF[0] = pIMU->MagGain[0] * tmp[0] + pIMU->MagGain[1] * tmp[1] + pIMU->MagGain[2] * tmp[2];
    pIMU->MagF[1] = pIMU->MagGain[1] * tmp[0] + pIMU->MagGain[3] * tmp[1] + pIMU->MagGain[4] * tmp[2];
    pIMU->MagF[2] = pIMU->MagGain[2] * tmp[0] + pIMU->MagGain[4] * tmp[1] + pIMU->MagGain[5] * tmp[2];
  }
#endif

#ifdef __USE_BAROMETER
  if(pIMU->LPS_PresTemp_Enable == LPS_PresTemp_ENABLE) {
    pIMU->Temp -= pIMU->TempOffset;   // Temp
    pIMU->Pres -= pIMU->PresOffset;   // Pres
  }
#endif
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : IMU_ConvToPhy
**功能 : Convert Data
**輸入 : *pIMU
**輸出 : None
**使用 : IMU_ConvToPhy(pIMU);
**====================================================================================================*/
/*====================================================================================================*/
void IMU_ConvToPhy( IMU_DataTypeDef *pIMU )
{
  if(pIMU->MPU_GyrAcc_Enable == MPU_GyrAcc_ENABLE) {
    pIMU->GyrF[0] = pIMU->Gyr[0] * pIMU->GyrSens;     // Gyr.X
    pIMU->GyrF[1] = pIMU->Gyr[1] * pIMU->GyrSens;     // Gyr.Y
    pIMU->GyrF[2] = pIMU->Gyr[2] * pIMU->GyrSens;     // Gyr.Z
    pIMU->AccF[0] = pIMU->Acc[0] * pIMU->AccSens;     // Acc.X
    pIMU->AccF[1] = pIMU->Acc[1] * pIMU->AccSens;     // Acc.Y
    pIMU->AccF[2] = pIMU->Acc[2] * pIMU->AccSens;     // Acc.Z
    pIMU->ICTempF = pIMU->ICTemp * pIMU->ICTempSens;  // ICTemp
  }

#ifdef __USE_MAGNETOMETER
  if(pIMU->MPU_Mag_Enable == MPU_Mag_ENABLE) {
    pIMU->MagF[0] = pIMU->Mag[0] * pIMU->MagSens;     // Mag.X
    pIMU->MagF[1] = pIMU->Mag[1] * pIMU->MagSens;     // Mag.Y
    pIMU->MagF[2] = pIMU->Mag[2] * pIMU->MagSens;     // Mag.Z
  }
#endif

#ifdef __USE_BAROMETER
  if(pIMU->LPS_PresTemp_Enable == LPS_PresTemp_ENABLE) {
    pIMU->TempF = pIMU->Temp * pIMU->TempSens;        // Temp
    pIMU->PresF = pIMU->Pres * pIMU->PresSens;        // Pres
  }
#endif
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : IMU_PrintData
**功能 : Print Data
**輸入 : *pIMU
**輸出 : None
**使用 : IMU_PrintData(IMU);
**====================================================================================================*/
/*====================================================================================================*/
#include "modules\serial.h"
void IMU_PrintData( IMU_DataTypeDef *pIMU )
{
  printf("\r\n");
  printf("G : %i, %i, %i\r\n", pIMU->Gyr[0], pIMU->Gyr[1], pIMU->Gyr[2]);
  printf("A : %i, %i, %i\r\n", pIMU->Acc[0], pIMU->Acc[1], pIMU->Acc[2]);
#ifdef __USE_MAGNETOMETER
  printf("M : %i, %i, %i\r\n", pIMU->Mag[0], pIMU->Mag[1], pIMU->Mag[2]);
#endif
#ifdef __USE_BAROMETER
  printf("TP: %i, %i\r\n", pIMU->Temp, pIMU->Pres);
#endif
  printf("\r\n");
  printf("G_offset : %i, %i, %i\r\n", pIMU->GyrOffset[0], pIMU->GyrOffset[1], pIMU->GyrOffset[2]);
  printf("A_offset : %i, %i, %i\r\n", pIMU->AccOffset[0], pIMU->AccOffset[1], pIMU->AccOffset[2]);
#ifdef __USE_MAGNETOMETER
  printf("M_offset : %i, %i, %i\r\n", pIMU->MagOffset[0], pIMU->MagOffset[1], pIMU->MagOffset[2]);
  printf("M_ASA    : %i, %i, %i\r\n", pIMU->MagASA[0], pIMU->MagASA[1], pIMU->MagASA[2]);
#endif
#ifdef __USE_BAROMETER
  printf("TP_offset: %i, %i\r\n", pIMU->TempOffset, pIMU->PresOffset);
#endif
  printf("\r\n");
  printf("G_f : %f, %f, %f\r\n", pIMU->GyrF[0], pIMU->GyrF[1], pIMU->GyrF[2]);
  printf("A_f : %f, %f, %f\r\n", pIMU->AccF[0], pIMU->AccF[1], pIMU->AccF[2]);
#ifdef __USE_MAGNETOMETER
  printf("M_f : %f, %f, %f\r\n", pIMU->MagF[0], pIMU->MagF[1], pIMU->MagF[2]);
#endif
#ifdef __USE_BAROMETER
  printf("TP_f: %f, %f\r\n", pIMU->TempF, pIMU->PresF);
#endif
  printf("\r\n");
  printf("G_gain : %f, %f, %f\r\n", pIMU->GyrGain[0], pIMU->GyrGain[1], pIMU->GyrGain[2]);
  printf("A_gain : %f, %f, %f, %f, %f, %f\r\n", pIMU->AccGain[0], pIMU->AccGain[1], pIMU->AccGain[2], pIMU->AccGain[3], pIMU->AccGain[4], pIMU->AccGain[5]);
#ifdef __USE_MAGNETOMETER
  printf("M_gain : %f, %f, %f, %f, %f, %f\r\n", pIMU->MagGain[0], pIMU->MagGain[1], pIMU->MagGain[2], pIMU->MagGain[3], pIMU->MagGain[4], pIMU->MagGain[5]);
#endif
#ifdef __USE_BAROMETER
  printf("TP_gain: %f, %f\r\n", pIMU->TempGain, pIMU->PresGain);
#endif
  printf("\r\n");
  printf("G_sens : %f\r\n", pIMU->GyrSens);
  printf("A_sens : %f\r\n", pIMU->AccSens);
#ifdef __USE_MAGNETOMETER
  printf("M_sens : %f\r\n", pIMU->MagSens);
#endif
#ifdef __USE_BAROMETER
  printf("TP_sens: %f, %f\r\n", pIMU->TempSens, pIMU->PresSens);
#endif

  printf("\r\n\r\n");
}
/*==============================================================================================*/
/*==============================================================================================*/
