/**
  ********************************************************************************************
  * @file    string.h
  * @author  KitSprout
  * @date    4-Oct-2016
  * @brief   
  ********************************************************************************************
  */

/* Define to prevent recursive inclusion ---------------------------------------------------*/
#ifndef __STRING_H
#define __STRING_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include <string.h>

/* Exported types --------------------------------------------------------------------------*/
typedef enum {
  S_BIN   = 2,   /* unsigned binary      */
  S_OCT   = 8,   /* unsigned octal       */
  S_DEC   = 10,  /* unsigned decimal     */
  S_HEX   = 16,  /* unsigned hexadecimal */
  S_INT   = 0,   /* signed decimal       */
  S_FLOAT = 1,   /* float point          */
} StringType_t;

/* Exported constants ----------------------------------------------------------------------*/
/* Exported functions ----------------------------------------------------------------------*/  
void     num2Str( StringType_t type, uint8_t lens, char *pStr, int32_t number );
uint16_t lenOfStr( char *pStr );

#ifdef __cplusplus
}
#endif

#endif

/*************************************** END OF FILE ****************************************/
