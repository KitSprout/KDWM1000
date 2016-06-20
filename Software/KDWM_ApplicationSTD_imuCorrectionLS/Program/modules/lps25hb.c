/*====================================================================================================*/
/*====================================================================================================*/
#include "drivers\stm32f4_system.h"
#include "drivers\stm32f4_spi.h"
#include "modules\lps25hb.h"
/*====================================================================================================*/
/*====================================================================================================*/
#define SPIx                        SPI1
#define SPIx_CLK_ENABLE()           RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE)
#define SPIx_SPEED_HIGH             SPI_BaudRatePrescaler_2
#define SPIx_SPEED_LOW              SPI_BaudRatePrescaler_256

#define SPIx_CSB_PIN                GPIO_Pin_0
#define SPIx_CSB_GPIO_PORT          GPIOB
#define SPIx_CSB_H()                __GPIO_SET(SPIx_CSB_GPIO_PORT, SPIx_CSB_PIN)
#define SPIx_CSB_L()                __GPIO_RST(SPIx_CSB_GPIO_PORT, SPIx_CSB_PIN)

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

#define SPIx_INT_PIN                GPIO_Pin_0
#define SPIx_INT_GPIO_PORT          GPIOA
#define SPIx_INT_IRQ                EXTI0_IRQn
#define SPIx_INT_LINE               EXTI_Line0
#define SPIx_INT_SOURCE             GPIO_PinSource0
/*====================================================================================================*/
/*====================================================================================================*
**函數 : LPS25HB_WriteReg
**功能 : Write Reg
**輸入 : writeAddr, writeData
**輸出 : None
**使用 : LPS25HB_WriteReg(writeAddr, writeData);
**====================================================================================================*/
/*====================================================================================================*/
void LPS25HB_WriteReg( uint8_t writeAddr, uint8_t writeData )
{
  SPIx_CSB_L();
  SPI_RW(SPIx, writeAddr);
  SPI_RW(SPIx, writeData);
  SPIx_CSB_H();
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : LPS25HB_WriteRegs
**功能 : Write Regs
**輸入 : writeAddr, writeData
**輸出 : None
**使用 : LPS25HB_WriteRegs(writeAddr, writeData);
**====================================================================================================*/
/*====================================================================================================*/
void LPS25HB_WriteRegs( uint8_t writeAddr, uint8_t *writeData, uint8_t lens )
{
  SPIx_CSB_L();
  SPI_RW(SPIx, writeAddr);
  for(uint8_t i = 0; i < lens; i++)
    SPI_RW(SPIx, writeData[i]);
  SPIx_CSB_H();
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : LPS25HB_ReadReg
**功能 : Read Reg
**輸入 : readAddr
**輸出 : readData
**使用 : DeviceID = LPS25HB_ReadReg(readAddr);
**====================================================================================================*/
/*====================================================================================================*/
uint8_t LPS25HB_ReadReg( uint8_t readAddr )
{
  uint8_t readData = 0;

  SPIx_CSB_L();
  SPI_RW(SPIx, 0x80 | readAddr);
  readData = SPI_RW(SPIx, 0x00);
  SPIx_CSB_H();

  return readData;
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : LPS25HB_ReadRegs
**功能 : Read Regs
**輸入 : readAddr
**輸出 : readData
**使用 : LPS25HB_ReadRegs(LPS25HB_REF_P_XL, readData, 3);
**====================================================================================================*/
/*====================================================================================================*/
void LPS25HB_ReadRegs( uint8_t readAddr, uint8_t *readData, uint8_t lens )
{
  SPIx_CSB_L();
  SPI_RW(SPIx, 0x80 | readAddr);
  for(uint8_t i = 0; i < lens; i++)
    readData[i] = SPI_RW(SPIx, 0x00);
  SPIx_CSB_H();
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : LPS25HB_SetSpeed
**功能 : Set SPI Speed
**輸入 : SpeedSel
**輸出 : None
**使用 : LPS25HB_SetSpeed(SPIx_SPEED_LOW);
**====================================================================================================*/
/*====================================================================================================*/
void LPS25HB_SetSpeed( uint8_t speedSel )
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
**函數 : LPS25HB_Config
**功能 : SPI Config
**輸入 : None
**輸出 : None
**使用 : LPS25HB_Config();
**====================================================================================================*/
/*====================================================================================================*/
void LPS25HB_Config( void )
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

  GPIO_InitStruct.GPIO_Pin   = SPIx_CSB_PIN;
  GPIO_Init(SPIx_CSB_GPIO_PORT, &GPIO_InitStruct);

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

  SPIx_CSB_H();  // LOW ENABLE

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
**函數 : LPS25HB_Init
**功能 : Init LPS25HB
**輸入 : None
**輸出 : state
**使用 : state = LPS25HB_Init(&LPS_InitStruct);
**====================================================================================================*/
/*====================================================================================================*/
#define DELAY 5
uint8_t LPS25HB_Init( LPS_ConfigTypeDef *lps )
{
  uint8_t state = ERROR;
  uint8_t tmpREG = 0;

  state = LPS25HB_Check();
  if(state != SUCCESS)
    return ERROR;

  delay_ms(10);

  // Software reset
  tmpREG = LPS25HB_ReadReg(LPS25HB_CTRL_REG2) & (~0x04);
  tmpREG |= 0x04;
  LPS25HB_WriteReg(LPS25HB_CTRL_REG2, tmpREG);
  delay_ms(50);

  // Reset autozero function
  tmpREG = LPS25HB_ReadReg(LPS25HB_CTRL_REG1) & (~0x02);
  tmpREG |= 0x02;
  LPS25HB_WriteReg(LPS25HB_CTRL_REG1, tmpREG);
  delay_ms(DELAY);

  // Autozero enabled
  tmpREG = LPS25HB_ReadReg(LPS25HB_CTRL_REG2) & (~0x02);
  tmpREG |= 0x02;
  LPS25HB_WriteReg(LPS25HB_CTRL_REG2, tmpREG);
  delay_ms(DELAY);

  // Set pressure and temperature resolution mode
  tmpREG = lps->LPS_TempAVG | lps->LPS_PresAVG;
  LPS25HB_WriteReg(LPS25HB_RES_CONF, tmpREG);
  delay_ms(DELAY);

  // Set output data rate
  tmpREG = LPS25HB_ReadReg(LPS25HB_CTRL_REG1) & (~0x70);
  tmpREG |= lps->LPS_ODR;
  LPS25HB_WriteReg(LPS25HB_CTRL_REG1, tmpREG);
  delay_ms(DELAY);

//  // Interrupt pin configuration
//  tmpREG = LPS25HB_ReadReg(LPS25HB_CTRL_REG4) & (~0x0F);
//  tmpREG |= 0x01;   // Data-ready interrupt
//  LPS25HB_WriteReg(LPS25HB_CTRL_REG4, tmpREG);
//  delay_ms(DELAY);

//  // FIFO enable
//  tmpREG = LPS25HB_ReadReg(LPS25HB_CTRL_REG2) & (~0x40);
//  tmpREG |= 0x40;
//  LPS25HB_WriteReg(LPS25HB_CTRL_REG2, tmpREG);
//  delay_ms(DELAY);

//  // FIFO watermark point
//  tmpREG = LPS25HB_ReadReg(LPS25HB_FIFO_CTRL) & (~0x1F);
//  tmpREG |= lps->LPS_FIFO_WTM;
//  LPS25HB_WriteReg(LPS25HB_FIFO_CTRL, tmpREG);
//  delay_ms(DELAY);

//  // Register address automatically incremented
//  tmpREG = LPS25HB_ReadReg(LPS25HB_CTRL_REG2) & (~0x10);
//  tmpREG |= 0x00;
//  LPS25HB_WriteReg(LPS25HB_CTRL_REG2, tmpREG);
//  delay_ms(DELAY);

//  // FIFO mode
//  tmpREG = LPS25HB_ReadReg(LPS25HB_FIFO_CTRL) & (~0xE0);
//  tmpREG |= lps->LPS_FIFO_MODE;
//  LPS25HB_WriteReg(LPS25HB_FIFO_CTRL, tmpREG);
//  delay_ms(DELAY);

  // Set block data update
  tmpREG = LPS25HB_ReadReg(LPS25HB_CTRL_REG1) & (~0x04);
  tmpREG |= lps->LPS_BDU;
  LPS25HB_WriteReg(LPS25HB_CTRL_REG1, tmpREG);
  delay_ms(DELAY);

  // Set SPI Serial Mode
  tmpREG = LPS25HB_ReadReg(LPS25HB_CTRL_REG1);
  tmpREG &= ~0x01;
  LPS25HB_WriteReg(LPS25HB_CTRL_REG1, tmpREG);

  // Active mode
  tmpREG = LPS25HB_ReadReg(LPS25HB_CTRL_REG1) & (~0x80);
  tmpREG |= 0x80;
  LPS25HB_WriteReg(LPS25HB_CTRL_REG1, tmpREG);
  delay_ms(DELAY);

//  LPS25HB_SetSpeed(SPIx_SPEED_HIGH);
//  delay_ms(10);

  return SUCCESS;
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : LPS25HB_Check
**功能 : Check Device ID
**輸入 : None
**輸出 : State
**使用 : State = LPS25HB_Check();
**====================================================================================================*/
/*====================================================================================================*/
uint8_t LPS25HB_Check( void )
{
  uint8_t deviceID = 0x00;

  deviceID = LPS25HB_ReadReg(LPS25HB_WHO_AM_I);
  if(deviceID != LPS25HB_DeviceID)
    return ERROR;

  return SUCCESS;
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : LPS25HB_GetDataState
**功能 : Get Data State
**輸入 : None
**輸出 : state
**使用 : state = LPS25HB_GetDataState();
**====================================================================================================*/
/*====================================================================================================*/
uint8_t LPS25HB_GetDataState( void )
{
  return LPS25HB_ReadReg(LPS25HB_STATUS_REG);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : LPS25HB_GetFIFOState
**功能 : Get FIFO State
**輸入 : None
**輸出 : state
**使用 : state = LPS25HB_GetFIFOState();
**====================================================================================================*/
/*====================================================================================================*/
uint8_t LPS25HB_GetFIFOState( void )
{
  return LPS25HB_ReadReg(LPS25HB_FIFO_STATUS);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : LPS25HB_GetRefP
**功能 : Get Reference Pressure
**輸入 : refP
**輸出 : None
**使用 : LPS25HB_GetRefP(&refP);
**====================================================================================================*/
/*====================================================================================================*/
//void LPS25HB_GetRefP( int32_t *refP )
//{
//  uint8_t tmpRead[3] = {0};
//  uint32_t tmpRefP = 0;

//  LPS25HB_ReadRegs(LPS25HB_REF_P_XL, tmpRead, 3);

//  tmpRefP = Byte32(uint32_t, 0, tmpRead[2], tmpRead[1], tmpRead[0]);
//  *refP = (int32_t)((tmpRefP & 0x00800000) ? (tmpRefP | 0xFF000000) : (tmpRefP));
//}
void LPS25HB_GetRefP( int32_t *refP )
{
  uint8_t tmpRead[3] = {0};
  uint32_t tmpRefP = 0;
  int32_t raw_press;

  LPS25HB_ReadRegs(LPS25HB_REF_P_XL, tmpRead, 3);

  /* Build the raw data */
  for(uint8_t i = 0; i < 3; i++)
    tmpRefP |= (((uint32_t)tmpRead[i]) << (8 * i));
  /* convert the 2's complement 24 bit to 2's complement 32 bit */
  if(tmpRefP & 0x00800000)
    tmpRefP |= 0xFF000000;

  raw_press = ((int32_t)tmpRefP);
  *refP = (raw_press * 100) / 4096;
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : LPS25HB_GetOffsetP
**功能 : Get Pressure Offset Value
**輸入 : offsetP
**輸出 : None
**使用 : LPS25HB_GetOffsetP(&offsetP);
**====================================================================================================*/
/*====================================================================================================*/
void LPS25HB_GetOffsetP( int16_t *offsetP )
{
  uint8_t tmpRead[2] = {0};

  LPS25HB_ReadRegs(LPS25HB_RPDS_L, tmpRead, 2);

  *offsetP = Byte16(int16_t, tmpRead[1], tmpRead[0]);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : LPS25HB_GetData
**功能 : Read Data
**輸入 : *readData
**輸出 : None
**使用 : LPS25HB_GetData(readData);
**====================================================================================================*/
/*====================================================================================================*/
void LPS25HB_Get_Temperature( int16_t* Tout )
{
  uint8_t buffer[2];
  int16_t raw_data;
  uint16_t tmp;

  LPS25HB_ReadRegs(LPS25HB_TEMP_OUT_L, buffer, 2);
  /* Build the raw tmp */
  tmp = (((uint16_t)buffer[1]) << 8) + (uint16_t)buffer[0];
  raw_data = ((int16_t)tmp);
    
  *Tout = raw_data / 48 + 425;
}

void LPS25HB_Get_Pressure( int32_t* Pout )
{
  uint8_t buffer[3];
  int32_t raw_press;
  uint32_t tmp = 0;

  LPS25HB_ReadRegs(LPS25HB_PRESS_POUT_XL, buffer, 3);

  /* Build the raw data */
  for(uint8_t i = 0; i < 3; i++)
    tmp |= (((uint32_t)buffer[i]) << (8 * i));
    
  /* convert the 2's complement 24 bit to 2's complement 32 bit */
  if(tmp & 0x00800000)
    tmp |= 0xFF000000;
    
  raw_press = ((int32_t)tmp);

  *Pout = (raw_press * 100) / 4096;
}

void LPS25HB_GetData( int16_t *temp, int32_t *pres )
{
  LPS25HB_Get_Temperature(temp);
  LPS25HB_Get_Pressure(pres);
}
//void LPS25HB_GetData( int16_t *temp, int32_t *pres )
//{
//  uint8_t tmpRead[5] = {0};
//  uint16_t tmpT = 0;
//  uint32_t tmpP = 0;

//  LPS25HB_ReadRegs(LPS25HB_PRESS_POUT_XL, tmpRead, 5);
//  tmpT = Byte16(uint16_t, tmpRead[4], tmpRead[3]);
//  tmpP = Byte32(uint32_t, 0, tmpRead[2], tmpRead[1], tmpRead[0]);

//  *temp = (int16_t)(tmpT);
//  *pres = (int32_t)((tmpP & 0x00800000) ? (tmpP | 0xFF000000) : (tmpP));
//}
/*==============================================================================================*/
/*==============================================================================================*/
