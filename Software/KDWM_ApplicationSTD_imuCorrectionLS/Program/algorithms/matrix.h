/* #include "algorithms\matrix.h" */

#ifndef __MATRIX_H
#define __MATRIX_H

#include <stdlib.h>
#include "stm32f4xx.h"
/*====================================================================================================*/
/*====================================================================================================*/
typedef enum {
  MTYPE_NORMAL = 0,
  MTYPE_MALLOC,
  MTYPE_POINTER
} matrix_mType_t;

typedef struct {
  uint16_t rows;
  uint16_t cols;
  double   *arr;
  matrix_mType_t mType;
} matrix_t;
/*====================================================================================================*/
/*====================================================================================================*/
void      Matrix_Init( matrix_t *pMatrix, double *pArray, uint16_t rows, uint16_t cols );
void      Matrix_Clear( matrix_t *pMatrix );
matrix_t *Matrix_Create( uint16_t rows, uint16_t cols );
matrix_t *Matrix_CreatePtr( double *pArray, uint16_t rows, uint16_t cols );
matrix_t *Matrix_CreateDiag( uint16_t cols, double data );
void      Matrix_Delete( matrix_t *matrix );
void      Matrix_Copy( matrix_t *pMatrixC1, matrix_t *pMatrixC2 );
matrix_t *Matrix_CopyMatrix( matrix_t *pMatrix );
matrix_t *Matrix_CopyArray( double *pArray, uint16_t rows, uint16_t cols );
void      Matrix_Resize( matrix_t *pMatrix, uint16_t rows, uint16_t cols );
void      Matrix_SetData( matrix_t *pMatrix, uint16_t rows, uint16_t cols, double data );
double    Matrix_GetData( matrix_t *pMatrix, uint16_t rows, uint16_t cols );
void      Matrix_SetDiag( matrix_t *pMatrix, double data );
void      Matrix_GetDiag( matrix_t *pMatrix, matrix_t *pMatrixD );
void      Matrix_SetMatrix( matrix_t *pMatrix, matrix_t *pMatrixS, uint16_t rows_pos, uint16_t cols_pos );
void      Matrix_GetMatrix( matrix_t *pMatrixG, matrix_t *pMatrix, uint16_t rows_pos, uint16_t cols_pos );
void      Matrix_Add( matrix_t *pMatrix, matrix_t *pMatrixA1, matrix_t *pMatrixA2 );
void      Matrix_Sub( matrix_t *pMatrix, matrix_t *pMatrixS1, matrix_t *pMatrixS2 );
void      Matrix_Mul( matrix_t *pMatrix, matrix_t *pMatrixM1, matrix_t *pMatrixM2 );
void      Matrix_MulNumb( matrix_t *pMatrix, matrix_t *pMatrixM1, double number );
void      Matrix_Transpose( matrix_t *pMatrix, matrix_t *pMatrixT );
void      Matrix_Inv( matrix_t *pMatrix, matrix_t *pMatrixInv );
void      Matrix_GaussianElimination( matrix_t *pMatrix, matrix_t *pMatrixGE );
double    Matrix_Det( matrix_t *pMatrix );
void      Matrix_Cholesky( matrix_t *pMatrix, matrix_t *pMatrixC );
void      Matrix_Print( matrix_t *pMatrix );
void      Matrix_PrintInfo( matrix_t *pMatrix );
/*====================================================================================================*/
/*====================================================================================================*/
#endif
