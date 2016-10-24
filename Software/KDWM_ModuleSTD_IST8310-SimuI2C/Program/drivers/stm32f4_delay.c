/**
  ********************************************************************************************
  * @file    stm32f4_delay.c
  * @author  KitSprout
  * @date    4-Oct-2016
  * @brief   
  ********************************************************************************************
  */

/* Includes --------------------------------------------------------------------------------*/
#include "stm32f4_system.h"
#include "stm32f4_delay.h"

/** @addtogroup STM32_Driver
  * @{
  */

/* Private typedef -------------------------------------------------------------------------*/
/* Private define --------------------------------------------------------------------------*/
/* Private macro ---------------------------------------------------------------------------*/
/* Private variables -----------------------------------------------------------------------*/
static __IO uint32_t uwTick;

/* Private function prototypes -------------------------------------------------------------*/
/* Private functions -----------------------------------------------------------------------*/

/**
  * @brief  HAL_InitTick.
  * @param  None
  * @retval None
  */
void HAL_InitTick( void )
{
  NVIC_InitTypeDef NVIC_InitStruct;

  if (SysTick_Config(SystemCoreClock / 1000)) {
    while (1)
    {
    }
  }

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
  NVIC_InitStruct.NVIC_IRQChannel = (uint8_t)SysTick_IRQn;
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x000F;
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStruct);
}

/**
  * @brief  HAL_IncTick.
  * @param  None
  * @retval None
  */
void HAL_IncTick( void )
{
  uwTick++;
}

/**
  * @brief  HAL_GetTick.
  * @param  None
  * @retval uwTick
  */
uint32_t HAL_GetTick( void )
{
  return uwTick;
}

/**
  * @brief  HAL_Delay.
  * @param  delay: delay ms times.
  * @retval None
  */
void HAL_Delay( __IO uint32_t delay )
{
  uint32_t tickstart = 0;
  tickstart = HAL_GetTick();
  while ((HAL_GetTick() - tickstart) < delay)
  {
  }
}

/**
  * @brief  delay_us.
  * @param  vCnt_us: delay us time.
  * @retval None
  */
void delay_us( __IO uint32_t vCnt_us )
{
  __IO uint32_t vCnt;
  while (vCnt_us--) {
    for (vCnt = 7; vCnt != 0; vCnt--)
    {
    }
  }
}

/*************************************** END OF FILE ****************************************/
