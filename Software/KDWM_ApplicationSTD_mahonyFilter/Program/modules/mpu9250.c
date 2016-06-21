/*====================================================================================================*/
/*====================================================================================================*/
#include "drivers\stm32f4_system.h"
#include "drivers\stm32f4_spi.h"
#include "modules\mpu9250.h"
/*====================================================================================================*/
/*====================================================================================================*/
#define SPIx                        SPI1
#define SPIx_CLK_ENABLE()           RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE)
#define SPIx_SPEED_HIGH             SPI_BaudRatePrescaler_2
#define SPIx_SPEED_LOW              SPI_BaudRatePrescaler_256

#define SPIx_CSM_PIN                GPIO_Pin_4
#define SPIx_CSM_GPIO_PORT          GPIOA
#define SPIx_CSM_H()                __GPIO_SET(SPIx_CSM_GPIO_PORT, SPIx_CSM_PIN)
#define SPIx_CSM_L()                __GPIO_RST(SPIx_CSM_GPIO_PORT, SPIx_CSM_PIN)

#define SPIx_SCK_PIN                GPIO_Pin_5
#define SPIx_SCK_GPIO_PORT          GPIOA
#define SPIx_SCK_AF                 GPIO_AF_SPI1
#define SPIx_SCK_SOURCE             GPIO_PinSource5

#define SPIx_SDO_PIN                GPIO_Pin_6
#define SPIx_SDO_GPIO_PORT          GPIOA
#define SPIx_SDO_AF                 GPIO_AF_SPI1
#define SPIx_SDO_SOURCE             GPIO_PinSource6

#define SPIx_SDI_PIN                GPIO_Pin_7
#define SPIx_SDI_GPIO_PORT          GPIOA
#define SPIx_SDI_AF                 GPIO_AF_SPI1
#define SPIx_SDI_SOURCE             GPIO_PinSource7

#define SPIx_INT_PIN                GPIO_Pin_1
#define SPIx_INT_GPIO_PORT          GPIOA
#define SPIx_INT_IRQ                EXTI1_IRQn
#define SPIx_INT_LINE               EXTI_Line1
#define SPIx_INT_SOURCE             GPIO_PinSource1

#define __USE_MAG_AK8963
#define MAG_READ_DELAY 256
/*====================================================================================================*/
/*====================================================================================================*
**函數 : MPU9250_WriteReg
**功能 : Write Reg
**輸入 : writeAddr, writeData
**輸出 : None
**使用 : MPU9250_WriteReg(writeAddr, writeData);
**====================================================================================================*/
/*====================================================================================================*/
void MPU9250_WriteReg( uint8_t writeAddr, uint8_t writeData )
{
  SPIx_CSM_L();
  SPI_RW(SPIx, writeAddr);
  SPI_RW(SPIx, writeData);
  SPIx_CSM_H();
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : MPU9250_WriteRegs
**功能 : Write Regs
**輸入 : writeAddr, *writeData, lens
**輸出 : None
**使用 : MPU9250_WriteRegs(writeAddr, writeData, lens);
**====================================================================================================*/
/*====================================================================================================*/
void MPU9250_WriteRegs( uint8_t writeAddr, uint8_t *writeData, uint8_t lens )
{
  SPIx_CSM_L();
  SPI_RW(SPIx, writeAddr);
  for(uint8_t i = 0; i < lens; i++)
    SPI_RW(SPIx, writeData[i]);
  SPIx_CSM_H();
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : MPU9250_ReadReg
**功能 : Read Reg
**輸入 : readAddr
**輸出 : readData
**使用 : readData = MPU9250_ReadReg(readAddr);
**====================================================================================================*/
/*====================================================================================================*/
uint8_t MPU9250_ReadReg( uint8_t readAddr )
{
  uint8_t readData = 0;

  SPIx_CSM_L();
  SPI_RW(SPIx, 0x80 | readAddr);
  readData = SPI_RW(SPIx, 0x00);
  SPIx_CSM_H();

  return readData;
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : MPU9250_ReadRegs
**功能 : Read Regs
**輸入 : readAddr, *readData, lens
**輸出 : None
**使用 : MPU9250_ReadRegs(MPU6500_ACCEL_XOUT_H, readData, 14);
**=====================================================================================================*/
/*=====================================================================================================*/
void MPU9250_ReadRegs( uint8_t readAddr, uint8_t *readData, uint8_t lens )
{
  SPIx_CSM_L();
  SPI_RW(SPIx, 0x80 | readAddr);
  for(uint8_t i = 0; i < lens; i++)
    readData[i] = SPI_RW(SPIx, 0x00);
  SPIx_CSM_H();
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : MPU9250_Mag_WriteReg
**功能 : AK8963 Write Reg
**輸入 : writeAddr, writeData
**輸出 : None
**使用 : MPU9250_Mag_WriteReg(AK8963_CNTL2, 0x01);
**=====================================================================================================*/
/*=====================================================================================================*/
void MPU9250_Mag_WriteReg( uint8_t writeAddr, uint8_t writeData )
{
  uint8_t  status = 0;
  uint32_t timeout = MAG_READ_DELAY;

  MPU9250_WriteReg(MPU6500_I2C_SLV4_ADDR, AK8963_I2C_ADDR);
  delay_ms(1);
  MPU9250_WriteReg(MPU6500_I2C_SLV4_REG, writeAddr);
  delay_ms(1);
  MPU9250_WriteReg(MPU6500_I2C_SLV4_DO, writeData);
  delay_ms(1);
  MPU9250_WriteReg(MPU6500_I2C_SLV4_CTRL, MPU6500_I2C_SLVx_EN);
  delay_ms(1);

  do {
    status = MPU9250_ReadReg(MPU6500_I2C_MST_STATUS);
    delay_ms(1);
  } while(((status & MPU6500_I2C_SLV4_DONE) == 0) && (timeout--));
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : MPU9250_Mag_WriteRegs
**功能 : AK8963 Write Regs
**輸入 : writeAddr, *writeData, lens
**輸出 : None
**使用 : MPU9250_Mag_WriteRegs(AK8963_CNTL2, 0x01);
**=====================================================================================================*/
/*=====================================================================================================*/
void MPU9250_Mag_WriteRegs( uint8_t writeAddr, uint8_t *writeData, uint8_t lens )
{
  uint8_t  status = 0;
  uint32_t timeout = MAG_READ_DELAY;

  MPU9250_WriteReg(MPU6500_I2C_SLV4_ADDR, AK8963_I2C_ADDR);
  delay_ms(1);
  for(uint8_t i = 0; i < lens; i++) {
    MPU9250_WriteReg(MPU6500_I2C_SLV4_REG, writeAddr + i);
    delay_ms(1);
    MPU9250_WriteReg(MPU6500_I2C_SLV4_DO, writeData[i]);
    delay_ms(1);
    MPU9250_WriteReg(MPU6500_I2C_SLV4_CTRL, MPU6500_I2C_SLVx_EN);
    delay_ms(1);

    do {
      status = MPU9250_ReadReg(MPU6500_I2C_MST_STATUS);
    } while(((status & MPU6500_I2C_SLV4_DONE) == 0) && (timeout--));
  }
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : MPU9250_Mag_ReadReg
**功能 : AK8963 Read Reg
**輸入 : readAddr
**輸出 : readData
**使用 : DeviceID = MPU9250_Mag_ReadReg(AK8963_WIA);
**=====================================================================================================*/
/*=====================================================================================================*/
uint8_t MPU9250_Mag_ReadReg( uint8_t readAddr )
{
  uint8_t status = 0;
  uint8_t readData = 0;
  uint32_t timeout = MAG_READ_DELAY;

  MPU9250_WriteReg(MPU6500_I2C_SLV4_ADDR, AK8963_I2C_ADDR | 0x80);
  delay_ms(1);
  MPU9250_WriteReg(MPU6500_I2C_SLV4_REG, readAddr);
  delay_ms(1);
  MPU9250_WriteReg(MPU6500_I2C_SLV4_CTRL, MPU6500_I2C_SLVx_EN);
  delay_ms(1);

  do {
    status = MPU9250_ReadReg(MPU6500_I2C_MST_STATUS);
    delay_ms(1);
  } while(((status & MPU6500_I2C_SLV4_DONE) == 0) && (timeout--));

  readData = MPU9250_ReadReg(MPU6500_I2C_SLV4_DI);

  return readData;
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : MPU9250_Mag_ReadRegs
**功能 : AK8963 Read Regs
**輸入 : readAddr, *readData, lens
**輸出 : None
**使用 : DeviceID = MPU9250_Mag_ReadRegs(AK8963_WIA);
**=====================================================================================================*/
/*=====================================================================================================*/
void MPU9250_Mag_ReadRegs( uint8_t readAddr, uint8_t *readData, uint8_t lens )
{
  uint8_t status = 0;
  uint32_t timeout = MAG_READ_DELAY;

  MPU9250_WriteReg(MPU6500_I2C_SLV4_ADDR, AK8963_I2C_ADDR | 0x80);
  delay_ms(1);
  for(uint8_t i = 0; i< lens; i++) {
    MPU9250_WriteReg(MPU6500_I2C_SLV4_REG, readAddr + i);
    delay_ms(1);
    MPU9250_WriteReg(MPU6500_I2C_SLV4_CTRL, MPU6500_I2C_SLVx_EN);
    delay_ms(1);

    do {
      status = MPU9250_ReadReg(MPU6500_I2C_MST_STATUS);
    } while(((status & MPU6500_I2C_SLV4_DONE) == 0) && (timeout--));

    readData[i] = MPU9250_ReadReg(MPU6500_I2C_SLV4_DI);
    delay_ms(1);
  }
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : MPU9250_SetSpeed
**功能 : Set SPI Speed
**輸入 : SpeedSel
**輸出 : None
**使用 : MPU9250_SetSpeed(SPIx_SPEED_LOW);
**====================================================================================================*/
/*====================================================================================================*/
void MPU9250_SetSpeed( uint8_t speedSel )
{
  SPI_InitTypeDef SPI_InitStruct;

  SPI_I2S_DeInit(SPIx);

  SPI_InitStruct.SPI_Mode              = SPI_Mode_Master;
  SPI_InitStruct.SPI_Direction         = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStruct.SPI_DataSize          = SPI_DataSize_8b;
  SPI_InitStruct.SPI_CPOL              = SPI_CPOL_High;
  SPI_InitStruct.SPI_CPHA              = SPI_CPHA_2Edge;
  SPI_InitStruct.SPI_NSS               = SPI_NSS_Soft;
  SPI_InitStruct.SPI_BaudRatePrescaler = speedSel;
  SPI_InitStruct.SPI_FirstBit          = SPI_FirstBit_MSB;
  SPI_InitStruct.SPI_CRCPolynomial     = 7;
  SPI_Init(SPIx, &SPI_InitStruct);

  SPI_Cmd(SPIx, ENABLE); 
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : MPU9250_Config
**功能 : SPI Config
**輸入 : None
**輸出 : None
**使用 : MPU9250_Config();
**====================================================================================================*/
/*====================================================================================================*/
void MPU9250_Config( void )
{
  GPIO_InitTypeDef GPIO_InitStruct;
  SPI_InitTypeDef SPI_InitStruct;
//  NVIC_InitTypeDef NVIC_InitStruct;
//  EXTI_InitTypeDef EXTI_InitStruct;

  /* SPI Clk ******************************************************************/
  SPIx_CLK_ENABLE();

  /* SPI AF *******************************************************************/
  GPIO_PinAFConfig(SPIx_SCK_GPIO_PORT, SPIx_SCK_SOURCE, SPIx_SCK_AF);
  GPIO_PinAFConfig(SPIx_SDO_GPIO_PORT, SPIx_SDO_SOURCE, SPIx_SDO_AF);    
  GPIO_PinAFConfig(SPIx_SDI_GPIO_PORT, SPIx_SDI_SOURCE, SPIx_SDI_AF);

  /* SPI Pin ******************************************************************/
  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;

  GPIO_InitStruct.GPIO_Pin   = SPIx_CSM_PIN;
  GPIO_Init(SPIx_CSM_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;

  GPIO_InitStruct.GPIO_Pin   = SPIx_SCK_PIN;
  GPIO_Init(SPIx_SCK_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.GPIO_Pin   = SPIx_SDO_PIN;
  GPIO_Init(SPIx_SDO_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.GPIO_Pin   = SPIx_SDI_PIN;
  GPIO_Init(SPIx_SDI_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IN;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;

  GPIO_InitStruct.GPIO_Pin   = SPIx_INT_PIN;
  GPIO_Init(SPIx_INT_GPIO_PORT, &GPIO_InitStruct);

  SPIx_CSM_H();  // LOW ENABLE

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
  SPI_InitStruct.SPI_BaudRatePrescaler = SPIx_SPEED_LOW;
  SPI_InitStruct.SPI_FirstBit          = SPI_FirstBit_MSB;
  SPI_InitStruct.SPI_CRCPolynomial     = 7;
  SPI_Init(SPIx, &SPI_InitStruct);

  SPI_Cmd(SPIx, ENABLE); 
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : MPU9250_Init
**功能 : Init MPU9250
**輸入 : None
**輸出 : state
**使用 : state = MPU9250_Init(&MPU_InitStruct);
**====================================================================================================*/
/*====================================================================================================*/
#define MPU6500_InitRegNum 11
int8_t MPU9250_Init( MPU_ConfigTypeDef *MPUx )
{
  int8_t status = ERROR;
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

  MPU6500_InitData[6][0] = MPUx->MPU_Gyr_FullScale;       // MPU6500_GYRO_CONFIG
  MPU6500_InitData[8][0] = MPUx->MPU_Gyr_LowPassFilter;   // MPU6500_CONFIG
  MPU6500_InitData[7][0] = MPUx->MPU_Acc_FullScale;       // MPU6500_ACCEL_CONFIG
  MPU6500_InitData[9][0] = MPUx->MPU_Acc_LowPassFilter;   // MPU6500_ACCEL_CONFIG_2

  for(uint8_t i = 0; i < MPU6500_InitRegNum; i++) {
    MPU9250_WriteReg(MPU6500_InitData[i][1], MPU6500_InitData[i][0]);
    delay_ms(1);
  }

  status = MPU9250_Check();
  if(status != SUCCESS)
    return ERROR;

  delay_ms(10);

#ifdef __USE_MAG_AK8963
  uint8_t tmp[3] = {0};

  MPU9250_Mag_WriteReg(AK8963_CNTL2, 0x01);     // Reset Device
  delay_ms(1);
  MPU9250_Mag_WriteReg(AK8963_CNTL1, 0x10);     // Power-down mode
  delay_ms(1);
  MPU9250_Mag_WriteReg(AK8963_CNTL1, 0x1F);     // Fuse ROM access mode
  delay_ms(1);
  MPU9250_Mag_ReadRegs(AK8963_ASAX, tmp, 3);    // Read sensitivity adjustment values
  delay_ms(1);
  MPU9250_Mag_WriteReg(AK8963_CNTL1, 0x10);     // Power-down mode
  delay_ms(1);

//	AK8963_ASA[0] = (int16_t)(tmp[0]) + 128;
//	AK8963_ASA[1] = (int16_t)(tmp[1]) + 128;
//	AK8963_ASA[2] = (int16_t)(tmp[2]) + 128;

  MPU9250_WriteReg(MPU6500_I2C_MST_CTRL, 0x5D);
  delay_ms(1);
  MPU9250_WriteReg(MPU6500_I2C_SLV0_ADDR, AK8963_I2C_ADDR | 0x80);
  delay_ms(1);
  MPU9250_WriteReg(MPU6500_I2C_SLV0_REG, AK8963_ST1);
  delay_ms(1);
  MPU9250_WriteReg(MPU6500_I2C_SLV0_CTRL, MPU6500_I2C_SLVx_EN | 8);
  delay_ms(1);

  MPU9250_Mag_WriteReg(AK8963_CNTL1, 0x16);       // Continuous measurement mode 2
  delay_ms(1);

	MPU9250_WriteReg(MPU6500_I2C_SLV4_CTRL, 0x09);
	delay_ms(1);
	MPU9250_WriteReg(MPU6500_I2C_MST_DELAY_CTRL, 0x81);
	delay_ms(100);
#endif

  MPU9250_SetSpeed(SPIx_SPEED_HIGH);
  delay_ms(10);

  return SUCCESS;
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : MPU9250_Check
**功能 : Check Device ID
**輸入 : None
**輸出 : status
**使用 : status = MPU9250_Check();
**====================================================================================================*/
/*====================================================================================================*/
int8_t MPU9250_Check( void )
{
  uint8_t deviceID = ERROR;

  deviceID = MPU9250_ReadReg(MPU6500_WHO_AM_I);
  if(deviceID != MPU6500_DeviceID)
    return ERROR;

#ifdef __USE_MAG_AK8963
  deviceID = MPU9250_Mag_ReadReg(AK8963_WIA);
  if(deviceID != AK8963_DeviceID)
    return ERROR;
#endif

  return SUCCESS;
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : MPU9250_getData
**功能 : Get IMU Data
**輸入 : *dataIMU
**輸出 : None
**使用 : MPU9250_getData(dataIMU);
**====================================================================================================*/
/*====================================================================================================*/
void MPU9250_getData( int16_t *pIMU )
{
  uint8_t tmpRead[22] = {0};

#ifdef __USE_MAG_AK8963
  MPU9250_ReadRegs(MPU6500_ACCEL_XOUT_H, tmpRead, 22);
#else
  MPU9250_ReadRegs(MPU6500_ACCEL_XOUT_H, tmpRead, 14);
#endif

  pIMU[0] = (Byte16(int16_t, tmpRead[6],  tmpRead[7]));    // Temp
  pIMU[1] = (Byte16(int16_t, tmpRead[0],  tmpRead[1]));    // Acc.X
  pIMU[2] = (Byte16(int16_t, tmpRead[2],  tmpRead[3]));    // Acc.Y
  pIMU[3] = (Byte16(int16_t, tmpRead[4],  tmpRead[5]));    // Acc.Z
  pIMU[4] = (Byte16(int16_t, tmpRead[8],  tmpRead[9]));    // Gyr.X
  pIMU[5] = (Byte16(int16_t, tmpRead[10], tmpRead[11]));   // Gyr.Y
  pIMU[6] = (Byte16(int16_t, tmpRead[12], tmpRead[13]));   // Gyr.Z
#ifdef __USE_MAG_AK8963
  if(!(tmpRead[14] & AK8963_STATUS_DRDY) || (tmpRead[14] & AK8963_STATUS_DOR) || (tmpRead[21] & AK8963_STATUS_HOFL))
    return;

  pIMU[7] = (Byte16(int16_t, tmpRead[16], tmpRead[15]));   // Mag.X
  pIMU[8] = (Byte16(int16_t, tmpRead[18], tmpRead[17]));   // Mag.Y
  pIMU[9] = (Byte16(int16_t, tmpRead[20], tmpRead[19]));   // Mag.Z

//  pIMU[7] = ((long)dataIMU[7] * AK8963_ASA[0]) >> 8;
//  pIMU[8] = ((long)dataIMU[8] * AK8963_ASA[1]) >> 8;
//  pIMU[9] = ((long)dataIMU[9] * AK8963_ASA[2]) >> 8;
#endif  
}
/*==============================================================================================*/
/*==============================================================================================*/
