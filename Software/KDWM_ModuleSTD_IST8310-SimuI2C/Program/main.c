/**
  ********************************************************************************************
  * @file    main.c
  * @author  KitSprout
  * @date    20-Oct-2016
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
/* Private define --------------------------------------------------------------------------*/
/* Private macro ---------------------------------------------------------------------------*/
/* Private variables -----------------------------------------------------------------------*/
/* Private function prototypes -------------------------------------------------------------*/
void KDWM_Init( void );
void KDWM_Loop( void );

/* Private functions -----------------------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main( void )
{
  HAL_InitTick();

  KDWM_Init();
  KDWM_Loop();
}

/**
  * @brief  KDWM_Init
  * @param  None
  * @retval None
  */
void KDWM_Init( void )
{
  KDWM_GPIO_Config();
  KDWM_UART_Config(NULL);
  KDWM_IST8310_Config();
}

/**
  * @brief  KDWM_Loop
  * @param  None
  * @retval None
  */
void KDWM_Loop( void )
{
//  float32_t sendBuf[4] = {0};

  int16_t data[3];

  while (1) {
    LED_G_Toggle();
    delay_ms(100);
    IST8310_GetRawData(data);

    printf("MX : %6i, MY : %6i, MZ : %6i\r\n", data[0], data[1], data[2]);

//    sendBuf[0] = 0;
//    sendBuf[1] = 0;
//    sendBuf[2] = 0;
//    sendBuf[3] = 0;
//    kSerial_sendData(sendBuf, 4, KS_FLOAT32);
  }
}

/*************************************** END OF FILE ****************************************/
