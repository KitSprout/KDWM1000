/*====================================================================================================*/
/*====================================================================================================*/
#include "stm32f4_system.h"
#include "stm32f4_tim_pwm.h"
/*====================================================================================================*/
/*====================================================================================================*/
#define TIMx_GPIO_PIN           GPIO_Pin_10
#define TIMx_GPIO_PORT          GPIOB
#define TIMx_GPIO_AF            GPIO_AF_TIM2
#define TIMx_GPIO_SOURCE        GPIO_PinSource10
/*====================================================================================================*/
/*====================================================================================================*
**函數 : TIM_PWM_Config
**功能 : TIM PWM Config
**輸入 : None
**輸出 : None
**使用 : TIM_PWM_Config();
**====================================================================================================*/
/*====================================================================================================*/
void TIM_PWM_Config( void )
{
  GPIO_InitTypeDef GPIO_InitStruct;
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;
  TIM_OCInitTypeDef TIM_OCInitStruct;

  /* TIMX Clk ******************************************************************/
  TIMx_CLK_ENABLE();

  /* GPIO AF Config ************************************************************/
  GPIO_PinAFConfig(TIMx_GPIO_PORT, TIMx_GPIO_SOURCE, TIMx_GPIO_AF);

  /* GPIO Config ***************************************************************/
  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;

  GPIO_InitStruct.GPIO_Pin   = TIMx_GPIO_PIN;
  GPIO_Init(TIMx_GPIO_PORT, &GPIO_InitStruct);

  /* TIM Base Config ************************************************************/
  TIM_TimeBaseStruct.TIM_Prescaler     = TIMx_PRES - 1;
  TIM_TimeBaseStruct.TIM_Period        = TIMx_PERIOD - 1;
  TIM_TimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStruct.TIM_CounterMode   = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStruct);

  /* TIM OC Config **************************************************************/
  TIM_OCInitStruct.TIM_OCMode      = TIM_OCMode_PWM1; 
  TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStruct.TIM_OCPolarity  = TIM_OCPolarity_Low;

  TIM_OCInitStruct.TIM_Pulse       = PWM_MIN;
  TIMx_OCxInit(TIMx, &TIM_OCInitStruct);

  /* TIM Enable *****************************************************************/
  TIM_Cmd(TIMx, ENABLE);
//  TIM_CtrlPWMOutputs(TIMx, ENABLE);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : TIM_PWM_setDuty
**功能 : TIM PWM Set Duty
**輸入 : duty
**輸出 : None
**使用 : TIM_PWM_setDuty(duty);
**====================================================================================================*/
/*====================================================================================================*/
void TIM_PWM_setDuty( uint16_t duty )
{
  TIMx_PWM_DUTY = (duty > PWM_MAX) ? (PWM_MAX) : (duty);
}
/*====================================================================================================*/
/*====================================================================================================*/
