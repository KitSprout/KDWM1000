/*====================================================================================================*/
/*====================================================================================================*/
#include "drivers\stm32f4_system.h"

#include "kdwm1000_bsp.h"
/*====================================================================================================*/
/*====================================================================================================*/
pFunc EXTI_Line8_RecvEven;
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
void KDWM_EXTI9_5_Config( pFunc pEXTIx )
{
  EXTI_InitTypeDef EXTI_InitStruct;
  GPIO_InitTypeDef GPIO_InitStruct;
  NVIC_InitTypeDef NVIC_InitStruct;

  EXTI_Line8_RecvEven = pEXTIx;

  /* EXTI Clk ******************************************************************/
  EXTIx_CLK_ENABLE();

  /* GPIO Pin ******************************************************************/
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStruct.GPIO_Pin  = EXTIx_PIN;
  GPIO_Init(EXTIx_GPIO_PORT, &GPIO_InitStruct);

  /* Connect EXTI **************************************************************/
  SYSCFG_EXTILineConfig(EXTIx_GPIO_PORT_SOURCE, EXTIx_PIN_SOURCE);

  /* EXTI Config ***************************************************************/
  EXTI_InitStruct.EXTI_Line    = EXTIx_LINE;
  EXTI_InitStruct.EXTI_Mode    = EXTIx_MODE;
  EXTI_InitStruct.EXTI_Trigger = EXTIx_TRIGGER;  
  EXTI_InitStruct.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStruct);

  /* NVIC Config ***************************************************************/
  NVIC_InitStruct.NVIC_IRQChannel                   = EXTIx_IRQ;
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x0F;
  NVIC_InitStruct.NVIC_IRQChannelSubPriority        = 0x0F;
  NVIC_InitStruct.NVIC_IRQChannelCmd                = ENABLE;
  NVIC_Init(&NVIC_InitStruct);
}
/*====================================================================================================*/
/*====================================================================================================*/
