/**
  *      __            ____
  *     / /__ _  __   / __/                      __  
  *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
  *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
  *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
  *                    /_/   github.com/KitSprout    
  * 
  * @file    ist8310.c
  * @author  KitSprout
  * @date    20-Oct-2016
  * @brief   
  * 
  */

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\stm32f4_system.h"
#include "drivers\stm32f4_i2cSimu.h"
#include "modules\ist8310.h"

/** @addtogroup STM32_Module
  * @{
  */

/* Private typedef -------------------------------------------------------------------------*/
static SimuI2C_InitTypeDef SHT_SimuI2C_InitStruct;

/* Private define --------------------------------------------------------------------------*/
#define IST8310_SCL_PIN         GPIO_Pin_14
#define IST8310_SCL_GPIO_PORT   GPIOB
#define IST8310_SDA_PIN         GPIO_Pin_15
#define IST8310_SDA_GPIO_PORT   GPIOB
#define IST8310_FREQUENCY       100

/* Private macro ---------------------------------------------------------------------------*/
/* Private variables -----------------------------------------------------------------------*/
/* Private function prototypes -------------------------------------------------------------*/
/* Private functions -----------------------------------------------------------------------*/

/**
  * @brief  IST8310_WriteReg
  * @param  registerAddr: 
  * @param  writeData: 
  * @retval None
  */
void IST8310_WriteReg( uint8_t registerAddr, uint8_t writeData )
{
  SimuI2C_WriteReg(&SHT_SimuI2C_InitStruct, IST8310_I2C_ADDR, registerAddr, writeData);
}

/**
  * @brief  IST8310_ReadReg
  * @param  registerAddr: 
  * @retval readData:
  */
uint8_t IST8310_ReadReg( uint8_t registerAddr )
{
  uint8_t readData;
  SimuI2C_ReadReg(&SHT_SimuI2C_InitStruct, IST8310_I2C_ADDR, registerAddr, &readData);
  return readData;
}

/**
  * @brief  IST8310_ReadRegs
  * @param  registerAddr: 
  * @param  readData: 
  * @param  lens: 
  * @retval None
  */
void IST8310_ReadRegs( uint8_t registerAddr, uint8_t *readData, uint8_t lens )
{
  SimuI2C_ReadRegs(&SHT_SimuI2C_InitStruct, IST8310_I2C_ADDR, registerAddr, readData, lens);
}

void IST8310_Config( void )
{
  SHT_SimuI2C_InitStruct.PinSCL    = IST8310_SCL_PIN;
  SHT_SimuI2C_InitStruct.PortSCL   = IST8310_SCL_GPIO_PORT;
  SHT_SimuI2C_InitStruct.PinSDA    = IST8310_SDA_PIN;
  SHT_SimuI2C_InitStruct.PortSDA   = IST8310_SDA_GPIO_PORT;
  SHT_SimuI2C_InitStruct.Freguency = IST8310_FREQUENCY;
  SimuI2C_Config(&SHT_SimuI2C_InitStruct);
}

/**
  * @brief  IST8310_Init
  * @param  None
  * @retval None
  */
void IST8310_Init( void )
{
  delay_ms(50);
  IST8310_WriteReg(IST8310_AVGCNTL, 0x24); /* x, y, z average by 16 times */
  IST8310_WriteReg(IST8310_PDCNTL, 0xC0);
  delay_ms(10);
  IST8310_WriteReg(IST8310_CNTL1, 0x01);
  delay_ms(10);
}

/**
  * @brief  IST8310_GetRawData
  * @param  data:
  * @retval None
  */
void IST8310_GetRawData( int16_t *data )
{
  if ((IST8310_ReadReg(IST8310_STAT1) & 0x01) == 0x01) {
    uint8_t raw[6];
    IST8310_ReadRegs(IST8310_DATAXL, raw, 6);
    IST8310_WriteReg(IST8310_CNTL1, 0x01);

    data[0] = (((int16_t)raw[1]) << 8) | raw[0];
    data[1] = (((int16_t)raw[3]) << 8) | raw[2];
    data[2] = (((int16_t)raw[5]) << 8) | raw[4];
  }
}

/*************************************** END OF FILE ****************************************/
