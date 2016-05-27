/* #include "module_fontlib.h" */

#ifndef __MODULE_FONTLIB_H
#define __MODULE_FONTLIB_H

#include "stm32f4xx.h"
/*=====================================================================================================*/
/*=====================================================================================================*/
typedef enum {
  dirH,
  dirV
} eFontType;

typedef struct {    
  const uint8_t *table;
  uint8_t height;
  uint8_t width;
  eFontType type;
} sFONT;

extern sFONT Font8x6;
extern sFONT Font16x8;
/*=====================================================================================================*/
/*=====================================================================================================*/
#endif
