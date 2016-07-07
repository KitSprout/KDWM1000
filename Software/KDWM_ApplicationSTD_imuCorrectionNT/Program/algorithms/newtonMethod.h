/* #include "algorithms\newtonMethod.h" */

#ifndef __NEWTONMETHOD_H
#define __NEWTONMETHOD_H

#include "stm32f4xx.h"
#include "algorithms\matrix.h"
/*====================================================================================================*/
/*====================================================================================================*/
void NewtonMethod( Matrix_TypeDef *pMatrixX, Matrix_TypeDef *pMatrixA, const double alpha, double refData );
/*====================================================================================================*/
/*====================================================================================================*/
#endif
