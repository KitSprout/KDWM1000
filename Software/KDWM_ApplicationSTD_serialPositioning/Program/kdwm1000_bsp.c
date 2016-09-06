/*====================================================================================================*/
/*====================================================================================================*/
#include "drivers\stm32f4_system.h"
#include "modules\serial.h"
#include "modules\dw1000.h"

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
void KDWM_DW1000_Config( void )
{
//  DW_ConfigTypeDef DW_ConfigStruct;
//  int8_t state = ERROR;

  DW1000_Config();
  delay_ms(50);

//  DW_ConfigStruct.loadCode        = DISABLE;
//  DW_ConfigStruct.DW_channel      = Channel_2;
//  DW_ConfigStruct.DW_dataRate     = DataRate_110K;
//  DW_ConfigStruct.DW_pulseRepFreq = PulseRepFreq_64M;
//  DW_ConfigStruct.DW_txPreambLen  = TxPreambLen_1024;
//  DW_ConfigStruct.DW_rxCode       = 9;
//  DW_ConfigStruct.DW_phrMode      = PhrMode_StandardFrame;
//  state = DW1000_Init(&DW_ConfigStruct);
//  while(state != SUCCESS) {
//    LED_R_Toggle();
//    delay_ms(100);
//  }
//  delay_ms(50);
}
/*====================================================================================================*/
/*====================================================================================================*/
