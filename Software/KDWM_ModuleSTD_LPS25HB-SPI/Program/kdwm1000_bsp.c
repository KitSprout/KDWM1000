/*====================================================================================================*/
/*====================================================================================================*/
#include "drivers\stm32f4_system.h"
#include "modules\module_serial.h"
#include "modules\module_lps25hb.h"

#include "kdwm1000_bsp.h"
/*====================================================================================================*/
/*====================================================================================================*/
pFunc UART1_irqEven = NULL;
/*====================================================================================================*/
/*====================================================================================================*/
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
/*====================================================================================================*/
/*====================================================================================================*/
void KDWM_UART_Config( pFunc pUARTx )
{
  UART1_irqEven = pUARTx;

  Serial_Config();
  printf("\r\nHello World!\r\n\r\n");
}
/*====================================================================================================*/
/*====================================================================================================*/
void KDWM_LPS25HB_Config( void )
{
  LPS_ConfigTypeDef LPS_ConfigStruct;

  LPS25HB_Config();
  delay_ms(50);

  LPS_ConfigStruct.LPS_PresAVG   = LPS_PresAVG_512;
  LPS_ConfigStruct.LPS_TempAVG   = LPS_TempAVG_64;
  LPS_ConfigStruct.LPS_ODR       = LPS_ODR_25Hz;
  LPS_ConfigStruct.LPS_BDU       = LPS_BDU_ContinuousUpdate;
  LPS_ConfigStruct.LPS_FIFO_MODE = LPS_FIFO_MODE_BYPASS;
  LPS_ConfigStruct.LPS_FIFO_WTM  = LPS_FIFO_WTM_2;
  while(LPS25HB_Init(&LPS_ConfigStruct) != SUCCESS) {
    LED_R_Toggle();
    delay_ms(100);
  }
  delay_ms(50);
}
/*====================================================================================================*/
/*====================================================================================================*/
