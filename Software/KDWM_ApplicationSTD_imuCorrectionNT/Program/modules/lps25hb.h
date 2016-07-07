/* #include "modules\lps25hb.h" */

#ifndef __LPS25HB_H
#define __LPS25HB_H

#include "stm32f4xx.h"

#define __USE_BAROMETER
/*=====================================================================================================*/
/*=====================================================================================================*/
/*
| ODR |  Pres  |  Temp  |
+-----+--------+--------+
|  0  | One-Shot Enable |
|  1  |    1Hz |    1Hz |
|  2  |    7Hz |    7Hz |
|  3  | 12.5Hz | 12.5Hz |
|  4  |   25Hz |   25Hz |
|  5  |     Reserved    |
*/

typedef enum {
  LPS_TempAVG_8   = 0x00, // Internal average on   8 samples
  LPS_TempAVG_16  = 0x04, // Internal average on  16 samples
  LPS_TempAVG_32  = 0x08, // Internal average on  32 samples
  LPS_TempAVG_64  = 0x0C, // Internal average on  64 samples
} LPS_TempAVG_TypeDef;
typedef enum {
  LPS_PresAVG_8   = 0x00, // Internal average on   8 samples
  LPS_PresAVG_32  = 0x01, // Internal average on  32 samples
  LPS_PresAVG_128 = 0x02, // Internal average on 128 samples
  LPS_PresAVG_512 = 0x03, // Internal average on 256 samples
} LPS_PresAVG_TypeDef;
typedef enum {
  LPS_ODR_OneShot = 0x00, // one shot
  LPS_ODR_1Hz     = 0x10, // 1Hz
  LPS_ODR_7Hz     = 0x20, // 7Hz
  LPS_ODR_12P5Hz  = 0x30, // 12.5Hz
  LPS_ODR_25Hz    = 0x40, // 25Hz
} LPS_ODR_TypeDef;
typedef enum {
  LPS_BDU_ContinuousUpdate = 0x00, // Data updated continuously
  LPS_ODR_NoUpdate         = 0x04, // Data updated after a read operation
} LPS_BDU_TypeDef;

typedef enum {
  LPS_FIFO_MODE_BYPASS            = 0x00,
  LPS_FIFO_MODE_FIFO              = 0x20,
  LPS_FIFO_MODE_STREAM            = 0x40,
  LPS_FIFO_MODE_STREAM_TO_FIFO    = 0x60,
  LPS_FIFO_MODE_BYPASS_TO_STREAM  = 0x80,
  LPS_FIFO_MODE_NOT_AVAILABLE     = 0xA0,
  LPS_FIFO_MODE_FIFO_MEAN         = 0xC0,
  LPS_FIFO_MODE_BYPASS_TO_FIFO    = 0xE0,
} LPS_FIFO_MODE_TypeDef;
typedef enum {
  LPS_FIFO_WTM_2  = 0x01,
  LPS_FIFO_WTM_4  = 0x03,
  LPS_FIFO_WTM_8  = 0x07,
  LPS_FIFO_WTM_16 = 0x0F,
  LPS_FIFO_WTM_32 = 0x1F,
} LPS_FIFO_WTM_TypeDef;

typedef struct {
  LPS_TempAVG_TypeDef   LPS_TempAVG;
  LPS_PresAVG_TypeDef   LPS_PresAVG;
  LPS_ODR_TypeDef       LPS_ODR;
  LPS_BDU_TypeDef       LPS_BDU;
  LPS_FIFO_MODE_TypeDef LPS_FIFO_MODE;
  LPS_FIFO_WTM_TypeDef  LPS_FIFO_WTM;
} LPS_ConfigTypeDef;


/* ---- Sensitivity --------------------------------------------------------- */
#define LPS25HB_P_hPa         ((float)0.000244140625f)  // 0.000244140625f hPa/LSB
#define LPS25HB_T_degC        ((float)0.002083333333f)  // 0.002083333333f degC/LSB

#define LPS25HB_I2C_ADDR      ((uint8_t)0xB8) // 1011_10xb
#define LPS25HB_DeviceID      ((uint8_t)0xBD)

#define LPS25HB_REF_P_XL      ((uint8_t)0x08)
#define LPS25HB_REF_P_L       ((uint8_t)0x09)
#define LPS25HB_REF_P_H       ((uint8_t)0x0A)
#define LPS25HB_WHO_AM_I      ((uint8_t)0x0F)
#define LPS25HB_RES_CONF      ((uint8_t)0x10)
#define LPS25HB_CTRL_REG1     ((uint8_t)0x20)
#define LPS25HB_CTRL_REG2     ((uint8_t)0x21)
#define LPS25HB_CTRL_REG3     ((uint8_t)0x22)
#define LPS25HB_CTRL_REG4     ((uint8_t)0x23)
#define LPS25HB_INT_CFG       ((uint8_t)0x24)
#define LPS25HB_INT_SOURCE    ((uint8_t)0x25)
#define LPS25HB_STATUS_REG    ((uint8_t)0x27)
#define LPS25HB_PRESS_POUT_XL ((uint8_t)0x28)
#define LPS25HB_PRESS_OUT_L   ((uint8_t)0x29)
#define LPS25HB_PRESS_OUT_H   ((uint8_t)0x2A)
#define LPS25HB_TEMP_OUT_L    ((uint8_t)0x2B)
#define LPS25HB_TEMP_OUT_H    ((uint8_t)0x2C)
#define LPS25HB_FIFO_CTRL     ((uint8_t)0x2E)
#define LPS25HB_FIFO_STATUS   ((uint8_t)0x2F)
#define LPS25HB_THS_P_L       ((uint8_t)0x30)
#define LPS25HB_THS_P_H       ((uint8_t)0x31)
#define LPS25HB_RPDS_L        ((uint8_t)0x39)
#define LPS25HB_RPDS_H        ((uint8_t)0x3A)
/*=====================================================================================================*/
/*=====================================================================================================*/
void    LPS25HB_WriteReg( uint8_t writeAddr, uint8_t writeData );
void    LPS25HB_WriteRegs( uint8_t writeAddr, uint8_t *writeData, uint8_t lens );
uint8_t LPS25HB_ReadReg( uint8_t readAddr );
void    LPS25HB_ReadRegs( uint8_t readAddr, uint8_t *readData, uint8_t lens );
void    LPS25HB_SetSpeed( uint8_t speedSel );

void    LPS25HB_Config( void );
uint8_t LPS25HB_Init( LPS_ConfigTypeDef *lps );

uint8_t LPS25HB_Check( void );

uint8_t LPS25HB_GetDataState( void );
uint8_t LPS25HB_GetFIFOState( void );
void    LPS25HB_GetRefP( int32_t *refP );
void    LPS25HB_GetOffsetP( int16_t *offsetP );
void    LPS25HB_GetData( int16_t *temp, int32_t *pres );
/*=====================================================================================================*/
/*=====================================================================================================*/
#endif
