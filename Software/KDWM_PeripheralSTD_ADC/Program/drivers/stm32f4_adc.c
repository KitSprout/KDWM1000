/*====================================================================================================*/
/*====================================================================================================*/
#include "stm32f4_system.h"
#include "stm32f4_adc.h"
/*====================================================================================================*/
/*====================================================================================================*/
#define ADC1_DR_ADDRESS   ((uint32_t)0x4001204C)

__IO uint16_t ADC_DMA_ConvBuf[ADC_BUF_SIZE][ADC_BUF_CHENNAL];
/*====================================================================================================*/
/*====================================================================================================*
**函數 : ADC_Config
**功能 : ADC Config
**輸入 : None
**輸出 : None
**使用 : ADC_Config();
**====================================================================================================*/
/*====================================================================================================*/
void ADC_Config( void )
{
  DMA_InitTypeDef DMA_InitStruct;
  GPIO_InitTypeDef GPIO_InitStruct;
  ADC_InitTypeDef ADC_InitStruct;
  ADC_CommonInitTypeDef ADC_CommonInitStruct;
  

  /* Enable clock ******************************************************************/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);

  /* DMA Config ********************************************************************/
  DMA_InitStruct.DMA_Channel            = DMA_Channel_0;  
  DMA_InitStruct.DMA_PeripheralBaseAddr = ADC1_DR_ADDRESS;
  DMA_InitStruct.DMA_Memory0BaseAddr    = (uint32_t)&ADC_DMA_ConvBuf;
  DMA_InitStruct.DMA_DIR                = DMA_DIR_PeripheralToMemory;
  DMA_InitStruct.DMA_BufferSize         = ADC_BUF_CHENNAL * ADC_BUF_SIZE;
  DMA_InitStruct.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
  DMA_InitStruct.DMA_MemoryInc          = DMA_MemoryInc_Enable;
  DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStruct.DMA_MemoryDataSize     = DMA_MemoryDataSize_HalfWord;
  DMA_InitStruct.DMA_Mode               = DMA_Mode_Circular;
  DMA_InitStruct.DMA_Priority           = DMA_Priority_Medium;
  DMA_InitStruct.DMA_FIFOMode           = DMA_FIFOMode_Disable;         
  DMA_InitStruct.DMA_FIFOThreshold      = DMA_FIFOThreshold_HalfFull;
  DMA_InitStruct.DMA_MemoryBurst        = DMA_MemoryBurst_Single;
  DMA_InitStruct.DMA_PeripheralBurst    = DMA_PeripheralBurst_Single;
  DMA_Init(DMA2_Stream4, &DMA_InitStruct);
  DMA_Cmd(DMA2_Stream4, ENABLE);

  /* GPIO Config *******************************************************************/
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;

  GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_2 | GPIO_Pin_3;
  GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_1;
  GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* ADC Common Config *************************************************************/
  ADC_CommonInitStruct.ADC_Mode             = ADC_Mode_Independent;
  ADC_CommonInitStruct.ADC_Prescaler        = ADC_Prescaler_Div2;
  ADC_CommonInitStruct.ADC_DMAAccessMode    = ADC_DMAAccessMode_Disabled;
  ADC_CommonInitStruct.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_10Cycles;
  ADC_CommonInit(&ADC_CommonInitStruct);

  /* ADC Config ********************************************************************/
  ADC_InitStruct.ADC_Resolution           = ADC_Resolution_12b;
  ADC_InitStruct.ADC_ScanConvMode         = ENABLE;
  ADC_InitStruct.ADC_ContinuousConvMode   = ENABLE;
  ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStruct.ADC_ExternalTrigConv     = ADC_ExternalTrigConv_T1_CC1;
  ADC_InitStruct.ADC_DataAlign            = ADC_DataAlign_Right;
  ADC_InitStruct.ADC_NbrOfConversion      = ADC_BUF_CHENNAL;
  ADC_Init(ADC1, &ADC_InitStruct);

  /* ADC Regular Config ************************************************************/
  ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 1, ADC_SampleTime_112Cycles);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 2, ADC_SampleTime_112Cycles);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 3, ADC_SampleTime_112Cycles);

  /* ADC Enable ********************************************************************/
  ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
  ADC_DMACmd(ADC1, ENABLE);
  ADC_Cmd(ADC1, ENABLE);
  ADC_SoftwareStartConv(ADC1);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : ADC_getData
**功能 : Get ADC Data
**輸入 : channel
**輸出 : None
**使用 : ADC_ConvVal = ADC_getData(1);
**====================================================================================================*/
/*====================================================================================================*/
uint16_t ADC_getData( uint8_t channel )
{
  uint8_t getChannel = channel - 1;
  return (getChannel  < ADC_BUF_CHENNAL) ? ADC_DMA_ConvBuf[0][getChannel] : 0;
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : ADC_getAverage
**功能 : Get Average
**輸入 : *pADC_data, adcSample
**輸出 : None
**使用 : ADC_getAverage(ADC_ConvVal, 64);
**====================================================================================================*/
/*====================================================================================================*/
void ADC_getAverage( uint16_t *pADC_data, uint8_t adcSample )
{
  uint32_t tmpData = 0;

  for(uint8_t i = 0; i < ADC_BUF_CHENNAL; i++) {
    tmpData = 0;
    for(uint8_t j = ADC_BUF_SIZE - adcSample; j < ADC_BUF_SIZE; j++)
      tmpData += ADC_DMA_ConvBuf[j][i];
    pADC_data[i] = (uint16_t)(tmpData / adcSample);
  }
}
/*====================================================================================================*/
/*====================================================================================================*/
