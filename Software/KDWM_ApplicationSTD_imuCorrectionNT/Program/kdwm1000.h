/* #include "kdwm1000.h" */

#ifndef __KDWM1000_H
#define __KDWM1000_H

#include "stm32f4xx.h"
#include "kdwm1000_bsp.h"
/*====================================================================================================*/
/*====================================================================================================*/
#define __SAMPLE_RATE_FREQ500Hz_

#ifdef __SAMPLE_RATE_FREQ10Hz_
  #define SAMPLE_RATE_FREQ  ((uint16_t)10)      // 10Hz
  #define SAMPLE_RATE       ((float)0.1f)       // 100ms
  #define SAMPLE_RATE_HELF  ((float)0.05f)      // 50ms
#endif

#ifdef __SAMPLE_RATE_FREQ50Hz_
  #define SAMPLE_RATE_FREQ  ((uint16_t)50)      // 50Hz
  #define SAMPLE_RATE       ((float)0.02f)      // 20ms
  #define SAMPLE_RATE_HELF  ((float)0.01f)      // 10ms
#endif

#ifdef __SAMPLE_RATE_FREQ100Hz_
  #define SAMPLE_RATE_FREQ  ((uint16_t)100)     // 100Hz
  #define SAMPLE_RATE       ((float)0.01f)      // 10ms
  #define SAMPLE_RATE_HELF  ((float)0.005f)     // 5ms
#endif

#ifdef __SAMPLE_RATE_FREQ200Hz_
  #define SAMPLE_RATE_FREQ  ((uint16_t)200)     // 200Hz
  #define SAMPLE_RATE       ((float)0.005f)     // 5ms
  #define SAMPLE_RATE_HELF  ((float)0.0025f)    // 2.5ms
#endif

#ifdef __SAMPLE_RATE_FREQ400Hz_
  #define SAMPLE_RATE_FREQ  ((uint16_t)400)     // 400Hz
  #define SAMPLE_RATE       ((float)0.0025f)    // 2.5ms
  #define SAMPLE_RATE_HELF  ((float)0.00125f)   // 1.25ms
#endif

#ifdef __SAMPLE_RATE_FREQ500Hz_
  #define SAMPLE_RATE_FREQ  ((uint16_t)500)     // 500Hz
  #define SAMPLE_RATE       ((float)0.002f)     // 2.0ms
  #define SAMPLE_RATE_HELF  ((float)0.001f)     // 1.0ms
#endif

#ifdef __SAMPLE_RATE_FREQ1000Hz_
  #define SAMPLE_RATE_FREQ  ((uint16_t)1000)    // 1000Hz
  #define SAMPLE_RATE       ((float)0.001f)     // 1.0ms
  #define SAMPLE_RATE_HELF  ((float)0.0005f)    // 0.5ms
#endif

#ifdef __SAMPLE_RATE_FREQ2000Hz_
  #define SAMPLE_RATE_FREQ  ((uint16_t)2000)    // 2000Hz
  #define SAMPLE_RATE       ((float)0.0005f)    // 0.5ms
  #define SAMPLE_RATE_HELF  ((float)0.00025f)   // 0.25ms
#endif
  
typedef enum {
  SENSOR_START = 0,
  SENSOR_CORR_SEL,
  SENSOR_CORR,
  SENSOR_FLASH,
  SENSOR_ALG,
  SENSOR_DEBUG,
  SENSOR_END
} SensorState_TypeDef;
/*====================================================================================================*/
/*====================================================================================================*/
void KDWM_Init( void );
void KDWM_Loop( void );
/*====================================================================================================*/
/*====================================================================================================*/
#endif
