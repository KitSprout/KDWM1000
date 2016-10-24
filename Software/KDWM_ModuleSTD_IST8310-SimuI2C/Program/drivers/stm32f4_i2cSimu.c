/**
  ********************************************************************************************
  * @file    stm32f4_i2cSimu.c
  * @author  KitSprout
  * @date    4-Oct-2016
  * @brief   
  ********************************************************************************************
  */

/* Includes --------------------------------------------------------------------------------*/
#include "stm32f4_system.h"
#include "stm32f4_i2cSimu.h"

/** @addtogroup STM32_Driver
  * @{
  */

/* Private typedef -------------------------------------------------------------------------*/
/* Private define --------------------------------------------------------------------------*/
#define I2CxS_SCL_PIN           SimuI2C_Init->PinSCL
#define I2CxS_SCL_GPIO_PORT     SimuI2C_Init->PortSCL
#define I2CxS_SCL_L()           __GPIO_RST(I2CxS_SCL_GPIO_PORT, I2CxS_SCL_PIN)
#define I2CxS_SCL_H()           __GPIO_SET(I2CxS_SCL_GPIO_PORT, I2CxS_SCL_PIN)
#define I2CxS_SCL_READ()        (__GPIO_READ(I2CxS_SCL_GPIO_PORT, I2CxS_SCL_PIN) == I2CxS_SCL_PIN)

#define I2CxS_SDA_PIN           SimuI2C_Init->PinSDA
#define I2CxS_SDA_GPIO_PORT     SimuI2C_Init->PortSDA
#define I2CxS_SDA_L()           __GPIO_RST(I2CxS_SDA_GPIO_PORT, I2CxS_SDA_PIN)
#define I2CxS_SDA_H()           __GPIO_SET(I2CxS_SDA_GPIO_PORT, I2CxS_SDA_PIN)
#define I2CxS_SDA_READ()        (__GPIO_READ(I2CxS_SDA_GPIO_PORT, I2CxS_SDA_PIN) == I2CxS_SDA_PIN)

/* Private macro ---------------------------------------------------------------------------*/
/* Private variables -----------------------------------------------------------------------*/
/* Private function prototypes -------------------------------------------------------------*/
/* Private functions -----------------------------------------------------------------------*/

/**
  * @brief  SimuI2C_Config
  * @param  SimuI2C_Init: 
  * @retval None
  */
void SimuI2C_Config( SimuI2C_InitTypeDef *SimuI2C_Init )
{
  GPIO_InitTypeDef GPIO_InitStruct; 

  /* GPIO Clk Init *************************************************************/
  GPIO_InitStruct.GPIO_Pin   = SimuI2C_Init->PinSCL;
  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(SimuI2C_Init->PortSCL, &GPIO_InitStruct);

  GPIO_InitStruct.GPIO_Pin   = SimuI2C_Init->PinSDA;
  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(SimuI2C_Init->PortSDA, &GPIO_InitStruct);

  I2CxS_SCL_H();
  I2CxS_SDA_H();
}

/**
  * @brief  SimuI2C_Delay
  * @param  SimuI2C_Init: 
  * @retval None
  */
static void SimuI2C_Delay( SimuI2C_InitTypeDef *SimuI2C_Init )
{
  uint32_t i = SimuI2C_Init->Freguency;

  while (i--) {
    __nop();
  }
}

/**
  * @brief  SimuI2C_Start
  * @param  SimuI2C_Init: 
  * @retval None
  */
void SimuI2C_Start( SimuI2C_InitTypeDef *SimuI2C_Init )
{
  I2CxS_SDA_H();
  I2CxS_SCL_H();
  SimuI2C_Delay(SimuI2C_Init);
  I2CxS_SDA_L();
  SimuI2C_Delay(SimuI2C_Init);
}

/**
  * @brief  SimuI2C_Stop
  * @param  SimuI2C_Init: 
  * @retval None
  */
void SimuI2C_Stop( SimuI2C_InitTypeDef *SimuI2C_Init )
{
  I2CxS_SDA_L();
  I2CxS_SCL_H();
  SimuI2C_Delay(SimuI2C_Init);
  I2CxS_SDA_H();
  SimuI2C_Delay(SimuI2C_Init);
}

/**
  * @brief  SimuI2C_SendACK
  * @param  SimuI2C_Init: 
  * @param  sendACK: 
  * @retval None
  */
void SimuI2C_SendACK( SimuI2C_InitTypeDef *SimuI2C_Init, uint8_t sendACK )
{
  I2CxS_SCL_L();
  if (sendACK) {
    I2CxS_SDA_H();
  }
  else {
    I2CxS_SDA_L();
  }
  SimuI2C_Delay(SimuI2C_Init);
  I2CxS_SCL_H();
  SimuI2C_Delay(SimuI2C_Init);
  I2CxS_SCL_L();
  SimuI2C_Delay(SimuI2C_Init);
}

/**
  * @brief  SimuI2C_RecvACK
  * @param  SimuI2C_Init: 
  * @retval None
  */
uint8_t SimuI2C_RecvACK( SimuI2C_InitTypeDef *SimuI2C_Init )
{
  uint8_t recvACK = 0;

  I2CxS_SCL_L();
  I2CxS_SDA_H();
  SimuI2C_Delay(SimuI2C_Init);
  I2CxS_SCL_H();
  SimuI2C_Delay(SimuI2C_Init);
  recvACK = I2CxS_SDA_READ();
  I2CxS_SCL_L();
  SimuI2C_Delay(SimuI2C_Init);

  return recvACK;
}

/**
  * @brief  SimuI2C_SendByte
  * @param  SimuI2C_Init: 
  * @param  sendByte: 
  * @retval None
  */
void SimuI2C_SendByte( SimuI2C_InitTypeDef *SimuI2C_Init, uint8_t sendByte )
{
  for (uint8_t i = 8; i > 0; i--) {
    I2CxS_SCL_L();
    SimuI2C_Delay(SimuI2C_Init);
    if(sendByte & 0x80) {   /* MSB First */
      I2CxS_SDA_H();
    }
    else {
      I2CxS_SDA_L();
    }
    sendByte = sendByte << 1;
    SimuI2C_Delay(SimuI2C_Init);
    I2CxS_SCL_H();
    SimuI2C_Delay(SimuI2C_Init);
  }
  I2CxS_SCL_L();
}

/**
  * @brief  SimuI2C_RecvByte
  * @param  SimuI2C_Init: 
  * @retval receive byte
  */
uint8_t SimuI2C_RecvByte( SimuI2C_InitTypeDef *SimuI2C_Init )
{
  uint8_t recvByte = 0;

  I2CxS_SDA_H();
  for (uint8_t i = 8; i > 0; i--) {
    recvByte = recvByte << 1;
    I2CxS_SCL_L();
    SimuI2C_Delay(SimuI2C_Init);
    I2CxS_SCL_H();
    SimuI2C_Delay(SimuI2C_Init);
    if(I2CxS_SDA_READ()) {
      recvByte |= 0x01;
    }
  }
  I2CxS_SCL_L();

  return recvByte;
}

/**
  * @brief  SimuI2C_WriteData
  * @param  SimuI2C_Init: 
  * @param  slaveAddr: 
  * @param  writeData: 
  * @param  lens: 
  * @retval None
  */
void SimuI2C_WriteData( SimuI2C_InitTypeDef *SimuI2C_Init, uint8_t slaveAddr, uint8_t *writeData, uint8_t lens )
{
  SimuI2C_Start(SimuI2C_Init);
  SimuI2C_SendByte(SimuI2C_Init, slaveAddr);
  SimuI2C_RecvACK(SimuI2C_Init);
  for (uint8_t i = 0; i < lens; i++) {
    SimuI2C_SendByte(SimuI2C_Init, writeData[i]);
    SimuI2C_RecvACK(SimuI2C_Init);
  }
  SimuI2C_Stop(SimuI2C_Init);
}

/**
  * @brief  SimuI2C_ReadData
  * @param  SimuI2C_Init: 
  * @param  slaveAddr: 
  * @param  readData: 
  * @param  lens: 
  * @retval None
  */
void SimuI2C_ReadData( SimuI2C_InitTypeDef *SimuI2C_Init, uint8_t slaveAddr, uint8_t *readData, uint8_t lens )
{
  SimuI2C_Start(SimuI2C_Init);
  SimuI2C_SendByte(SimuI2C_Init, slaveAddr | 0x01);
  SimuI2C_RecvACK(SimuI2C_Init);
  for (uint8_t i = 0; i < lens; i++) {
    readData[i] = SimuI2C_RecvByte(SimuI2C_Init);
    if (i == (lens - 1)) {
      SimuI2C_SendACK(SimuI2C_Init, 1);
    }
    else {
      SimuI2C_SendACK(SimuI2C_Init, 0);
    }
  }
  SimuI2C_Stop(SimuI2C_Init);
}

/**
  * @brief  SimuI2C_WriteReg
  * @param  SimuI2C_Init: 
  * @param  slaveAddr: 
  * @param  registerAddr: 
  * @param  writeData: 
  * @retval None
  */
void SimuI2C_WriteReg( SimuI2C_InitTypeDef *SimuI2C_Init, uint8_t slaveAddr, uint8_t registerAddr, uint8_t writeData )
{
  SimuI2C_Start(SimuI2C_Init);
  SimuI2C_SendByte(SimuI2C_Init, slaveAddr);
  SimuI2C_RecvACK(SimuI2C_Init);
  SimuI2C_SendByte(SimuI2C_Init, registerAddr);
  SimuI2C_RecvACK(SimuI2C_Init);
  SimuI2C_SendByte(SimuI2C_Init, writeData);
  SimuI2C_RecvACK(SimuI2C_Init);
  SimuI2C_Stop(SimuI2C_Init);
}

/**
  * @brief  SimuI2C_ReadReg
  * @param  SimuI2C_Init: 
  * @param  slaveAddr: 
  * @param  registerAddr: 
  * @param  readData: 
  * @retval None
  */
void SimuI2C_ReadReg( SimuI2C_InitTypeDef *SimuI2C_Init, uint8_t slaveAddr, uint8_t registerAddr, uint8_t *readData )
{
  SimuI2C_Start(SimuI2C_Init);
  SimuI2C_SendByte(SimuI2C_Init, slaveAddr);
  SimuI2C_RecvACK(SimuI2C_Init);
  SimuI2C_SendByte(SimuI2C_Init, registerAddr);
  SimuI2C_RecvACK(SimuI2C_Init);
  SimuI2C_Start(SimuI2C_Init);
  SimuI2C_SendByte(SimuI2C_Init, slaveAddr | 0x01);
  SimuI2C_RecvACK(SimuI2C_Init);
  *readData = SimuI2C_RecvByte(SimuI2C_Init);
  SimuI2C_SendACK(SimuI2C_Init, 1);
  SimuI2C_Stop(SimuI2C_Init);
}

/**
  * @brief  SimuI2C_WriteRegs
  * @param  SimuI2C_Init: 
  * @param  slaveAddr: 
  * @param  registerAddr: 
  * @param  writeData: 
  * @param  lens: 
  * @retval None
  */
void SimuI2C_WriteRegs( SimuI2C_InitTypeDef *SimuI2C_Init, uint8_t slaveAddr, uint8_t registerAddr, uint8_t *writeData, uint8_t lens )
{
  SimuI2C_Start(SimuI2C_Init);
  SimuI2C_SendByte(SimuI2C_Init, slaveAddr);
  SimuI2C_RecvACK(SimuI2C_Init);
  SimuI2C_SendByte(SimuI2C_Init, registerAddr);
  SimuI2C_RecvACK(SimuI2C_Init);
  for (uint8_t i = 0; i < lens; i++) {
    SimuI2C_SendByte(SimuI2C_Init, writeData[i]);
    SimuI2C_RecvACK(SimuI2C_Init);
  }
  SimuI2C_Stop(SimuI2C_Init);
}

/**
  * @brief  SimuI2C_ReadRegs
  * @param  SimuI2C_Init: 
  * @param  slaveAddr: 
  * @param  registerAddr: 
  * @param  readData: 
  * @param  lens: 
  * @retval None
  */
void SimuI2C_ReadRegs( SimuI2C_InitTypeDef *SimuI2C_Init, uint8_t slaveAddr, uint8_t registerAddr, uint8_t *readData, uint8_t lens )
{
  SimuI2C_Start(SimuI2C_Init);
  SimuI2C_SendByte(SimuI2C_Init, slaveAddr);
  SimuI2C_RecvACK(SimuI2C_Init);
  SimuI2C_SendByte(SimuI2C_Init, registerAddr);
  SimuI2C_RecvACK(SimuI2C_Init);
  SimuI2C_Start(SimuI2C_Init);
  SimuI2C_SendByte(SimuI2C_Init, slaveAddr | 0x01);
  SimuI2C_RecvACK(SimuI2C_Init);
  for (uint8_t i = 0; i < lens; i++) {
    readData[i] = SimuI2C_RecvByte(SimuI2C_Init);
    if(i == lens - 1) {
      SimuI2C_SendACK(SimuI2C_Init, 1);
    }
    else {
      SimuI2C_SendACK(SimuI2C_Init, 0);
    }
  }
  SimuI2C_Stop(SimuI2C_Init);
}

/*************************************** END OF FILE ****************************************/
