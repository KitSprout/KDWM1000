/**
  ********************************************************************************************
  * @file    stm32f4_delay.h
  * @author  KitSprout
  * @date    4-Oct-2016
  * @brief   
  ********************************************************************************************
  */

/* Define to prevent recursive inclusion ---------------------------------------------------*/
#ifndef __STM32F4_DELAY_H
#define __STM32F4_DELAY_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------*/
#include "stm32f4xx.h"

/* Exported types --------------------------------------------------------------------------*/
/* Exported constants ----------------------------------------------------------------------*/
#define delay_ms(__ms)    HAL_Delay(__ms)

/* Exported functions ----------------------------------------------------------------------*/  
void     HAL_InitTick( void );
void     HAL_IncTick( void );
uint32_t HAL_GetTick( void );
void     HAL_Delay( __IO uint32_t Delay );

void     delay_us( __IO uint32_t vCnt_us );

#ifdef __cplusplus
}
#endif

#endif

/*************************************** END OF FILE ****************************************/
