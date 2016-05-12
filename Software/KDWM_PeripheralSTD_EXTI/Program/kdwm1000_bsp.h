/* #include "smartimu_bsp.h" */

#ifndef __SMARTIMU_BSP_H
#define __SMARTIMU_BSP_H

#include "stm32f4xx.h"
/*====================================================================================================*/
/*====================================================================================================*/
#define LED_R_PIN                 GPIO_Pin_14
#define LED_R_GPIO_PORT           GPIOC
#define LED_R_Set()               __GPIO_SET(LED_R_GPIO_PORT, LED_R_PIN)
#define LED_R_Reset()             __GPIO_RST(LED_R_GPIO_PORT, LED_R_PIN)
#define LED_R_Toggle()            __GPIO_TOG(LED_R_GPIO_PORT, LED_R_PIN)

#define LED_G_PIN                 GPIO_Pin_13
#define LED_G_GPIO_PORT           GPIOC
#define LED_G_Set()               __GPIO_SET(LED_G_GPIO_PORT, LED_G_PIN)
#define LED_G_Reset()             __GPIO_RST(LED_G_GPIO_PORT, LED_G_PIN)
#define LED_G_Toggle()            __GPIO_TOG(LED_G_GPIO_PORT, LED_G_PIN)

#define LED_B_PIN                 GPIO_Pin_15
#define LED_B_GPIO_PORT           GPIOC
#define LED_B_Set()               __GPIO_SET(LED_B_GPIO_PORT, LED_B_PIN)
#define LED_B_Reset()             __GPIO_RST(LED_B_GPIO_PORT, LED_B_PIN)
#define LED_B_Toggle()            __GPIO_TOG(LED_B_GPIO_PORT, LED_B_PIN)

#define KEY_PIN                   GPIO_Pin_8
#define KEY_GPIO_PORT             GPIOA
#define KEY_Read()                (__GPIO_READ(KEY_GPIO_PORT, KEY_PIN) == KEY_PIN)

#define EXTIx_PIN                 GPIO_Pin_8
#define EXTIx_PIN_SOURCE          EXTI_PinSource8
#define EXTIx_LINE                EXTI_Line8
#define EXTIx_IRQ                 EXTI9_5_IRQn
#define EXTIx_GPIO_PORT           GPIOA
#define EXTIx_GPIO_PORT_SOURCE    EXTI_PortSourceGPIOA
#define EXTIx_CLK_ENABLE()        RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE)
#define EXTIx_MODE                EXTI_Mode_Interrupt
#define EXTIx_TRIGGER             EXTI_Trigger_Rising
/*====================================================================================================*/
/*====================================================================================================*/
typedef void (*pFunc)( void );

extern pFunc EXTI_Line8_RecvEven;

void KDWM_GPIO_Config( void );
void KDWM_EXTI9_5_Config( pFunc pEXTIx );
/*====================================================================================================*/
/*====================================================================================================*/
#endif
