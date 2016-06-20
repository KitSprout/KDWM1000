/* #include "algorithms\moveAve.h" */

#ifndef __MOVEAVE_H
#define __MOVEAVE_H

#include "stm32f4xx.h"
/*====================================================================================================*/
/*====================================================================================================*/
float MoveAve_SMA( int16_t newData, int16_t *moveAveFIFO, uint16_t fifoLens, uint16_t windowLens );
float MoveAve_WMA( int16_t newData, int16_t *moveAveFIFO, int16_t *weighted, uint16_t fifoLens, uint16_t windowLens );
/*====================================================================================================*/
/*====================================================================================================*/
#endif	 
