/**
  ********************************************************************************************
  * @file    kdwm1000_bsp.c
  * @author  KitSprout
  * @date    4-Oct-2016
  * @brief   
  ********************************************************************************************
  */

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\stm32f4_system.h"
#include "modules\serial.h"
#include "modules\kSerial.h"
#include "modules\ist8310.h"
#include "kdwm1000_bsp.h"

/** @addtogroup STM32_Program
  * @{
  */

/* Private typedef -------------------------------------------------------------------------*/
pFunc IRQEven_UART1 = NULL;

/* Private define --------------------------------------------------------------------------*/
/* Private macro ---------------------------------------------------------------------------*/
/* Private variables -----------------------------------------------------------------------*/
/* Private function prototypes -------------------------------------------------------------*/
/* Private functions -----------------------------------------------------------------------*/

/**
  * @brief  KDWM_GPIO_Config
  * @param  None
  * @retval None
  */
void KDWM_GPIO_Config( void )
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /* Enable all GPIO Clk *******************************************************/
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

  /* GPIO all analog input *****************************************************/
  GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_All;
  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AN;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStruct);
  GPIO_Init(GPIOC, &GPIO_InitStruct);
  GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_All & (~(GPIO_Pin_13 | GPIO_Pin_14));
  GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* GPIO Pin ******************************************************************/
  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;

  GPIO_InitStruct.GPIO_Pin   = LED_R_PIN;
  GPIO_Init(LED_R_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.GPIO_Pin   = LED_G_PIN;
  GPIO_Init(LED_G_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.GPIO_Pin   = LED_B_PIN;
  GPIO_Init(LED_B_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IN;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;

  GPIO_InitStruct.GPIO_Pin   = KEY_PIN;
  GPIO_Init(KEY_GPIO_PORT, &GPIO_InitStruct);

  LED_R_Set();
  LED_G_Set();
  LED_B_Set();
}

/**
  * @brief  KDWM_UART_Config
  * @param  pUARTx: 
  * @retval None
  */
void KDWM_UART_Config( pFunc pUARTx )
{
  IRQEven_UART1 = pUARTx;

  Serial_Config();
  kSerial_config(Serial_SendByte);
  printf("\r\nHello World!\r\n\r\n");
}

/**
  * @brief  KDWM_IST8310_Config
  * @param  None
  * @retval None
  */
void KDWM_IST8310_Config( void )
{
  uint8_t deviceID;

  IST8310_Config();
  IST8310_Init();

  while(deviceID != IST8310_DEVICD_ID) {
    LED_R_Toggle();
    delay_ms(100);
    deviceID = IST8310_ReadReg(IST8310_WAI);
    printf("deviceID = 0x%2X\r\n", deviceID);
  }
  LED_R_Set();
}

/*************************************** END OF FILE ****************************************/
