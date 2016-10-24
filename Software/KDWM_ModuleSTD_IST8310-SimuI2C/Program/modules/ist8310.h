/**
  *      __            ____
  *     / /__ _  __   / __/                      __  
  *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
  *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
  *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
  *                    /_/   github.com/KitSprout    
  * 
  * @file    ist8310.h
  * @author  KitSprout
  * @date    20-Oct-2016
  * @brief   
  * 
  */

/* Define to prevent recursive inclusion ---------------------------------------------------*/
#ifndef __IST8310_H
#define __IST8310_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------*/
#include "stm32f4xx.h"

/* Exported types --------------------------------------------------------------------------*/
/* Exported constants ----------------------------------------------------------------------*/
#define IST8310_I2C_ADDR    ((uint8_t)0x18)
#define IST8310_DEVICD_ID   ((uint8_t)0x10)

#define IST8310_WAI         ((uint8_t)0x00)   /* Device ID */
#define IST8310_STAT1       ((uint8_t)0x02)   /* Data Status 1 */
#define IST8310_DATAXL      ((uint8_t)0x03)   /* Low  byte for X-axis data */
#define IST8310_DATAXH      ((uint8_t)0x04)   /* High byte for X-axis data */
#define IST8310_DATAYL      ((uint8_t)0x05)   /* Low  byte for Y-axis data */
#define IST8310_DATAYH      ((uint8_t)0x06)   /* High byte for Y-axis data */
#define IST8310_DATAZL      ((uint8_t)0x07)   /* Low  byte for Z-axis data */
#define IST8310_DATAZH      ((uint8_t)0x08)   /* High byte for Z-axis data */
#define IST8310_STAT2       ((uint8_t)0x09)   /* Data Status 2 */
#define IST8310_CNTL1       ((uint8_t)0x0A)   /* Control Register 1 */
#define IST8310_CNTL2       ((uint8_t)0x0B)   /* Control Register 2 */
#define IST8310_STR         ((uint8_t)0x0C)   /* Self-Test Register */
#define IST8310_TEMPL       ((uint8_t)0x1C)   /* Low  byte for Temperature data */
#define IST8310_TEMPH       ((uint8_t)0x1D)   /* High byte for Temperature data */
#define IST8310_AVGCNTL     ((uint8_t)0x41)   /* Average Control Register */
#define IST8310_PDCNTL      ((uint8_t)0x42)   /* Pulse Duration Control Register */

/* Exported functions ----------------------------------------------------------------------*/  
void    IST8310_WriteReg( uint8_t registerAddr, uint8_t writeData );
uint8_t IST8310_ReadReg( uint8_t registerAddr );
void    IST8310_ReadRegs( uint8_t registerAddr, uint8_t *readData, uint8_t lens );

void    IST8310_Config( void );
void    IST8310_Init( void );

void    IST8310_GetRawData( int16_t *data );

#ifdef __cplusplus
}
#endif

#endif

/*************************************** END OF FILE ****************************************/
