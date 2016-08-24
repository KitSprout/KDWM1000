/* #include "algorithms\matrix_arm.h" */

#ifndef __MATRIX_ARM_H
#define __MATRIX_ARM_H

#include <stdlib.h>
#include "stm32f4xx.h"
#include "algorithms\mathUnit.h"
/*====================================================================================================*/
/*====================================================================================================*/
#define USE_MATRIX_BUFFER

typedef arm_matrix_instance_f32 matrix_t;
typedef arm_matrix_instance_f64 matrix64_t;

void       Matrix_Clear( matrix_t *pMatrix );
void       Matrix_Init( matrix_t *pMatrix, float32_t *pArray, uint16_t rows, uint16_t cols );
arm_status Matrix_Copy( matrix_t *pMatrix, const matrix_t *pMatrixC );
arm_status Matrix_SetData( matrix_t *pMatrix, uint16_t rows, uint16_t cols, float32_t data );
arm_status Matrix_GetData( const matrix_t *pMatrix, uint16_t rows, uint16_t cols, float32_t *data );
arm_status Matrix_SetDiag( matrix_t *pMatrix, float32_t data );
arm_status Matrix_GetDiag( matrix_t *pMatrix, matrix_t *pMatrixD );
arm_status Matrix_SetMatrix( matrix_t *pMatrix, matrix_t *pMatrixS, uint16_t rows_pos, uint16_t cols_pos );
arm_status Matrix_GetMatrix( matrix_t *pMatrixG, matrix_t *pMatrix, uint16_t rows_pos, uint16_t cols_pos );
arm_status Matrix_Add( matrix_t *pMatrix, const matrix_t *pMatrixA1, const matrix_t *pMatrixA2 );
arm_status Matrix_Sub( matrix_t *pMatrix, const matrix_t *pMatrixS1, const matrix_t *pMatrixS2 );
arm_status Matrix_Mult( matrix_t *pMatrix, const matrix_t *pMatrixM1, const matrix_t *pMatrixM2 );
arm_status Matrix_MultScale( matrix_t *pMatrix, const matrix_t *pMatrixM, float32_t scale );
arm_status Matrix_Trans( matrix_t *pMatrix, const matrix_t *pMatrixT );
arm_status Matrix_Inv( matrix_t *pMatrix, const matrix_t *pMatrixI );
arm_status Matrix_Inv64( matrix_t *pMatrix, const matrix_t *pMatrixI );
//matrix_float_t Matrix_Det( matrix_t *pMatrix );
void Matrix_Print( matrix_t *pMatrix );
void Matrix_Print64( matrix64_t *pMatrix );
void Matrix_PrintInfo( matrix_t *pMatrix );
/*====================================================================================================*/
/*====================================================================================================*/
#endif
