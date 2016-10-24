/**
  ********************************************************************************************
  * @file    kSerial.c
  * @author  KitSprout
  * @date    4-Oct-2016
  * @brief   
  ********************************************************************************************
  */

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\stm32f4_system.h"
#include "modules\kSerial.h"

/** @addtogroup STM32_Module
  * @{
  */

/* Private typedef -------------------------------------------------------------------------*/
static ptrSerial pSerialFunc = NULL;

/* Private define --------------------------------------------------------------------------*/
/* Private macro ---------------------------------------------------------------------------*/
/* Private variables -----------------------------------------------------------------------*/
/* Private function prototypes -------------------------------------------------------------*/
/* Private functions -----------------------------------------------------------------------*/

/**
  * @brief  kSerial_config.
  * @param  pSerial: 
  * @retval None
  */
void kSerial_config( ptrSerial pSerial )
{
  pSerialFunc = pSerial;
}

/**
  * @brief  getTypeSize.
  * @param  type: 
  * @retval data length
  */
static uint8_t getTypeSize( uint8_t type )
{
  switch (type) {
    case KS_INT8:     return 1;
    case KS_UINT8:    return 1;
    case KS_INT16:    return 2;
    case KS_UINT16:   return 2;
    case KS_INT32:    return 4;
    case KS_UINT32:   return 4;
    case KS_INT64:    return 8;
    case KS_UINT64:   return 8;
    case KS_FLOAT32:  return 4;
    case KS_FLOAT64:  return 8;
    default:          return 0;
  }
}

/**
  * @brief  kSerial_sendData.
  * @param  signalData: 
  * @param  lens: 
  * @param  typeL: 
  * @retval None
  */
void kSerial_sendData( void *signalData, uint8_t lens, uint8_t type )
{
  uint8_t packet[64] = {0};
  uint8_t *pPacket = packet;

  uint16_t dataBytes  = lens * getTypeSize(type);
  uint16_t packetSize = dataBytes + 6;
  uint16_t packetInfo = ((uint16_t)type << 8) | dataBytes;

  packet[0] = 'K';                                      /* header 'K'  */
  packet[1] = 'S';                                      /* header 'S'  */
  packet[2] = (packetInfo & 0x00FF);                    /* lens & type */
  packet[3] = (packetInfo & 0xFF00) >> 8;               /* lens & type */
  for (uint8_t count = 0; count < dataBytes; count++)   /* data        */
    packet[count + 4] = ((uint8_t*)signalData)[count];
  packet[packetSize - 2] = '\r';                        /* 'r'         */
  packet[packetSize - 1] = '\n';                        /* 'n'         */

  do {
    pSerialFunc(*pPacket++);
  } while (--packetSize);
}

/*************************************** END OF FILE ****************************************/
