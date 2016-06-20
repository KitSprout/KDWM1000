/*====================================================================================================*/
/*====================================================================================================*/
#include "drivers\stm32f4_system.h"
#include "modules\serial.h"
#include "algorithms\matrix.h"

#include <string.h>
/*====================================================================================================*/
/*====================================================================================================*/
static void MATRIX_ERROR( matrix_t *pMatrix )
{
  printf("\r\nMATRIX ERROR...\r\n");
  Matrix_PrintInfo(pMatrix);
  while(1);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Matrix_Init
**功能 : Init Matrix
**輸入 : *pMatrix, *pArray, rows, cols
**輸出 : none
**使用 : Matrix_Init(matrix, array, rows, cols);
**====================================================================================================*/
/*====================================================================================================*/
void Matrix_Init( matrix_t *pMatrix, double *pArray, uint16_t rows, uint16_t cols )
{
  pMatrix->mType = MTYPE_NORMAL;
  pMatrix->rows  = rows;
  pMatrix->cols  = cols;
  pMatrix->arr   = pArray;
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Matrix_Clear
**功能 : Clear Matrix
**輸入 : *pMatrix
**輸出 : none
**使用 : Matrix_Clear(matrix);
**====================================================================================================*/
/*====================================================================================================*/
void Matrix_Clear( matrix_t *pMatrix )
{
  memset(pMatrix->arr, 0, sizeof(double) * pMatrix->rows * pMatrix->cols);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Matrix_Create
**功能 : Create Matrix
**輸入 : rows, cols
**輸出 : *matrix
**使用 : matrix = Matrix_Create(rows, cols);
**====================================================================================================*/
/*====================================================================================================*/
matrix_t *Matrix_Create( uint16_t rows, uint16_t cols )
{
  matrix_t *matrix = NULL;

  matrix = (matrix_t *)malloc(sizeof(matrix_t));
  matrix->mType = MTYPE_MALLOC;
  matrix->rows  = rows;
  matrix->cols  = cols;
  matrix->arr   = (double *)malloc(sizeof(double) * matrix->rows * matrix->cols);

  return matrix;
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Matrix_CreatePtr
**功能 : Create Matrix Point to Array
**輸入 : *pArray, rows, cols
**輸出 : *matrix
**使用 : matrix = Matrix_CreatePtr(array, rows, cols);
**====================================================================================================*/
/*====================================================================================================*/
matrix_t *Matrix_CreatePtr( double *pArray, uint16_t rows, uint16_t cols )
{
  matrix_t *matrix = NULL;

  matrix = (matrix_t *)malloc(sizeof(matrix_t));
  matrix->mType = MTYPE_POINTER;
  matrix->rows  = rows;
  matrix->cols  = cols;
  matrix->arr   = pArray;

  return matrix;
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Matrix_CreateDiag
**功能 : Create Diagonal Matrix
**輸入 : cols, data
**輸出 : *matrix
**使用 : matrix = Matrix_CreateDiag(cols, data);
**====================================================================================================*/
/*====================================================================================================*/
matrix_t *Matrix_CreateDiag( uint16_t cols, double data )
{
  matrix_t *matrix = Matrix_Create(cols, cols);

  Matrix_Clear(matrix);
  for(uint16_t i = 0; i < cols; i++)
    matrix->arr[i * matrix->cols + i] = data;

  return matrix;
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Matrix_Delete
**功能 : Delete Matrix
**輸入 : *matrix
**輸出 : none
**使用 : Matrix_Delete(matrix);
**====================================================================================================*/
/*====================================================================================================*/
void Matrix_Delete( matrix_t *matrix )
{
  if(matrix->mType == MTYPE_MALLOC) {
    free(matrix->arr);
  }
  free(matrix);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Matrix_Copy
**功能 : Copy Matrix
**輸入 : *pMatrixC1, *pMatrixC2
**輸出 : none
**使用 : Matrix_Copy(matrixC1, matrixC2);     // matrixC1 = matrixC2
**====================================================================================================*/
/*====================================================================================================*/
void Matrix_Copy( matrix_t *pMatrixC1, matrix_t *pMatrixC2 )
{
  if((pMatrixC1->rows != pMatrixC2->rows) || (pMatrixC1->cols != pMatrixC2->cols))
    MATRIX_ERROR(pMatrixC1);

  for(uint32_t i = 0; i < pMatrixC1->rows * pMatrixC1->cols; i++)
    pMatrixC1->arr[i] = pMatrixC2->arr[i];
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Matrix_CopyMatrix
**功能 : Copy & Create Matrix
**輸入 : *pMatrix
**輸出 : *matrix
**使用 : matrix = Matrix_CopyMatrix(matrixCpy);
**====================================================================================================*/
/*====================================================================================================*/
matrix_t *Matrix_CopyMatrix( matrix_t *pMatrix )
{
  matrix_t *matrix = Matrix_Create(pMatrix->rows, pMatrix->cols);

  for(uint32_t i = 0; i < matrix->rows * matrix->cols; i++)
    matrix->arr[i] = pMatrix->arr[i];

  return matrix;
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Matrix_CopyArray
**功能 : Copy Array to Matrix
**輸入 : *pArray, rows, cols
**輸出 : *matrix
**使用 : matrix = Matrix_CopyArray(array, row, col);
**====================================================================================================*/
/*====================================================================================================*/
matrix_t *Matrix_CopyArray( double *pArray, uint16_t rows, uint16_t cols )
{
  matrix_t *matrix = Matrix_Create(rows, cols);

  for(uint32_t i = 0; i < matrix->rows * matrix->cols; i++)
    matrix->arr[i] = pArray[i];

  return matrix;
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Matrix_Resize
**功能 : Resize Matrix
**輸入 : *pMatrix, rows, cols
**輸出 : none
**使用 : Matrix_Resize(matrix, row, col);
**====================================================================================================*/
/*====================================================================================================*/
void Matrix_Resize( matrix_t *pMatrix, uint16_t rows, uint16_t cols )
{
/*
  Matrix_TypeDef *matrix = Matrix_Create(rows, cols);

  Matrix_Clear(matrix);
  for(uint32_t i = 0; i < matrix->total; i++)
    matrix->arr[i] = array[i];

  return matrix;
*/
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Matrix_SetData
**功能 : Set Data
**輸入 : *pMatrix, rows, cols, data
**輸出 : None
**使用 : Matrix_SetData(matrix, rows, cols, data);
**====================================================================================================*/
/*====================================================================================================*/
void Matrix_SetData( matrix_t *pMatrix, uint16_t rows, uint16_t cols, double data )
{
  if((rows >= pMatrix->rows) && (cols >= pMatrix->cols))
    MATRIX_ERROR(pMatrix);

  pMatrix->arr[rows * pMatrix->cols + cols] = data;
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Matrix_GetData
**功能 : Get Data
**輸入 : *pMatrix, rows, cols
**輸出 : data
**使用 : data = Matrix_GetData(matrix, rows, cols);
**====================================================================================================*/
/*====================================================================================================*/
double Matrix_GetData( matrix_t *pMatrix, uint16_t rows, uint16_t cols )
{
  if((rows >= pMatrix->rows) && (cols >= pMatrix->cols))
    MATRIX_ERROR(pMatrix);

  return pMatrix->arr[rows * pMatrix->cols + cols];
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Matrix_SetDiag
**功能 : Set Diagonal Matrix
**輸入 : *pMatrix, data
**輸出 : none
**使用 : Matrix_SetDiag(matrix, data);
**====================================================================================================*/
/*====================================================================================================*/
void Matrix_SetDiag( matrix_t *pMatrix, double data )
{
  if(pMatrix->rows != pMatrix->cols)
    MATRIX_ERROR(pMatrix);

  for(uint16_t i = 0; i < pMatrix->cols; i++)
    pMatrix->arr[i * pMatrix->cols + i] = data;
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Matrix_GetDiag
**功能 : Get Diagonal
**輸入 : *pMatrix, pMatrixD
**輸出 : none
**使用 : Matrix_GetDiag(matrix, matrixD);
**====================================================================================================*/
/*====================================================================================================*/
void Matrix_GetDiag( matrix_t *pMatrix, matrix_t *pMatrixD )
{
  if((pMatrix->rows != pMatrix->cols) || (pMatrixD->rows != pMatrixD->cols) || (pMatrix->rows != pMatrixD->cols))
    MATRIX_ERROR(pMatrix);

  if(pMatrix != pMatrixD) {
    Matrix_Clear(pMatrix);
    for(uint16_t i = 0; i < pMatrix->rows; i++) {
      pMatrix->arr[i * pMatrix->cols + i] = pMatrixD->arr[i * pMatrixD->rows + i];
    }
  }
  else {
    matrix_t *matrix = Matrix_CopyMatrix(pMatrixD);
    for(uint16_t i = 0; i < pMatrix->rows; i++)
      pMatrix->arr[i * pMatrix->cols + i] = matrix->arr[i * matrix->rows + i];
  }
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Matrix_SetMatrix
**功能 : Set Matrix
**輸入 : *pMatrix, *pMatrixS, rows_pos, cols_pos
**輸出 : none
**使用 : Matrix_SetMatrix(matrix, matrixS, rows_pos, cols_pos);
**====================================================================================================*/
/*====================================================================================================*/
void Matrix_SetMatrix( matrix_t *pMatrix, matrix_t *pMatrixS, uint16_t rows_pos, uint16_t cols_pos )
{
  if(((rows_pos + pMatrixS->rows) > pMatrix->rows) || ((cols_pos + pMatrixS->cols) > pMatrix->cols))
    MATRIX_ERROR(pMatrix);

  uint32_t cnt = 0;
  for(uint16_t i = rows_pos; i < rows_pos + pMatrixS->rows; i++) {
    for(uint16_t j = cols_pos; j < cols_pos + pMatrixS->cols; j++) {
      pMatrix->arr[i * pMatrix->cols + j] = pMatrixS->arr[cnt++];
    }
  }
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Matrix_GetMatrix
**功能 : Get Matrix
**輸入 : *pMatrixG, *pMatrix, rows_pos, cols_pos
**輸出 : none
**使用 : Matrix_GetMatrix(matrixG, matrix, rows_pos, cols_pos);
**====================================================================================================*/
/*====================================================================================================*/
void Matrix_GetMatrix( matrix_t *pMatrixG, matrix_t *pMatrix, uint16_t rows_pos, uint16_t cols_pos )
{
  if(((rows_pos + pMatrixG->rows) > pMatrix->rows) || ((cols_pos + pMatrixG->cols) > pMatrix->cols))
    MATRIX_ERROR(pMatrix);

  uint32_t cnt = 0;
  for(uint16_t i = rows_pos; i < rows_pos + pMatrixG->rows; i++) {
    for(uint16_t j = cols_pos; j < cols_pos + pMatrixG->cols; j++) {
      pMatrixG->arr[cnt++] = pMatrix->arr[i * pMatrix->cols + j];
    }
  }
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Matrix_Add
**功能 : Matrix Addition
**輸入 : *pMatrix, *pMatrixA1, pMatrixA2
**輸出 : none
**使用 : Matrix_Add(matrix, matrixA1, matrixA2);
**====================================================================================================*/
/*====================================================================================================*/
void Matrix_Add( matrix_t *pMatrix, matrix_t *pMatrixA1, matrix_t *pMatrixA2 )
{
  if((pMatrix->rows != pMatrixA1->rows) || (pMatrix->cols != pMatrixA1->cols) ||
     (pMatrix->rows != pMatrixA2->rows) || (pMatrix->cols != pMatrixA2->cols))
    MATRIX_ERROR(pMatrix);

  for(uint16_t i = 0; i < pMatrix->rows; i++)
    for(uint16_t j = 0; j < pMatrix->cols; j++)
      pMatrix->arr[i * pMatrix->cols + j] = pMatrixA1->arr[i * pMatrixA1->cols + j] + pMatrixA2->arr[i * pMatrixA2->cols + j];
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Matrix_Sub
**功能 : Matrix Subtraction
**輸入 : *pMatrix, *pMatrixS1, *pMatrixS2
**輸出 : none
**使用 : Matrix_Sub(matrix, matrixS1, matrixS2);
**====================================================================================================*/
/*====================================================================================================*/
void Matrix_Sub( matrix_t *pMatrix, matrix_t *pMatrixS1, matrix_t *pMatrixS2 )
{
  if((pMatrix->rows != pMatrixS1->rows) || (pMatrix->cols != pMatrixS1->cols) ||
     (pMatrix->rows != pMatrixS2->rows) || (pMatrix->cols != pMatrixS2->cols))
    MATRIX_ERROR(pMatrix);

  for(uint16_t i = 0; i < pMatrix->rows; i++)
    for(uint16_t j = 0; j < pMatrix->cols; j++)
      pMatrix->arr[i * pMatrix->cols + j] = pMatrixS1->arr[i * pMatrixS1->cols + j] - pMatrixS2->arr[i * pMatrixS2->cols + j];
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Matrix_Mul
**功能 : Matrix Multiplication
**輸入 : *pMatrix, *pMatrixM1, *pMatrixM2
**輸出 : none
**使用 : Matrix_Mul(matrix, matrixM1, matrixM2);
**====================================================================================================*/
/*====================================================================================================*/
void Matrix_Mul( matrix_t *pMatrix, matrix_t *pMatrixM1, matrix_t *pMatrixM2 )
{
  if((pMatrixM1->cols != pMatrixM2->rows) || (pMatrix->rows != pMatrixM1->rows) || (pMatrix->cols != pMatrixM2->cols))
    MATRIX_ERROR(pMatrix);

  if(pMatrix == pMatrixM1) {
    matrix_t *matrix = Matrix_CopyMatrix(pMatrixM1);
    Matrix_Clear(pMatrix);
    for(uint16_t i = 0; i < matrix->rows; i++) {
      for(uint16_t j = 0; j < pMatrixM2->cols; j++) {
        for(uint16_t k = 0; k < matrix->cols; k++) {
          pMatrix->arr[i * pMatrix->cols + j] += matrix->arr[i * matrix->cols + k] * pMatrixM2->arr[k * pMatrixM2->cols + j];
        }
      }
    }
  }
  else if(pMatrix == pMatrixM2) {
    matrix_t *matrix = Matrix_CopyMatrix(pMatrixM2);
    Matrix_Clear(pMatrix);
    for(uint16_t i = 0; i < pMatrixM1->rows; i++) {
      for(uint16_t j = 0; j < matrix->cols; j++) {
        for(uint16_t k = 0; k < pMatrixM1->cols; k++) {
          pMatrix->arr[i * pMatrix->cols + j] += pMatrixM1->arr[i * pMatrixM1->cols + k] * matrix->arr[k * matrix->cols + j];
        }
      }
    }
  }
  else {
    Matrix_Clear(pMatrix);
    for(uint16_t i = 0; i < pMatrixM1->rows; i++) {
      for(uint16_t j = 0; j < pMatrixM2->cols; j++) {
        for(uint16_t k = 0; k < pMatrixM1->cols; k++) {
          pMatrix->arr[i * pMatrix->cols + j] += pMatrixM1->arr[i * pMatrixM1->cols + k] * pMatrixM2->arr[k * pMatrixM2->cols + j];
        }
      }
    }
  }
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Matrix_MulNumb
**功能 : Matrix Multiplication
**輸入 : *pMatrix, *pMatrixM1, number
**輸出 : none
**使用 : Matrix_MulNumb(matrix, matrixM1, number);
**====================================================================================================*/
/*====================================================================================================*/
void Matrix_MulNumb( matrix_t *pMatrix, matrix_t *pMatrixM1, double number )
{
  if((pMatrix->cols != pMatrixM1->cols) || (pMatrix->rows != pMatrixM1->rows))
    MATRIX_ERROR(pMatrix);

  for(uint32_t i = 0; i < pMatrix->rows * pMatrix->cols; i++) {
    pMatrix->arr[i] = pMatrixM1->arr[i] * number;
  }
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Matrix_Transpose
**功能 : Matrix Transpose
**輸入 : *pMatrix, *pMatrixT
**輸出 : none
**使用 : Matrix_Transpose(matrix, matrixT);
**====================================================================================================*/
/*====================================================================================================*/
void Matrix_Transpose( matrix_t *pMatrix, matrix_t *pMatrixT )
{
  if(pMatrix != pMatrixT) {
    pMatrix->rows = pMatrixT->cols;
    pMatrix->cols = pMatrixT->rows;
    for(uint16_t i = 0; i < pMatrixT->rows; i++)
      for(uint16_t j = 0; j < pMatrixT->cols; j++)
        pMatrix->arr[j * pMatrix->cols + i] = pMatrixT->arr[i * pMatrixT->cols + j];
  }
  else {
    matrix_t *matrix = Matrix_CopyMatrix(pMatrixT);
    pMatrix->rows = matrix->cols;
    pMatrix->cols = matrix->rows;
    for(uint16_t i = 0; i < matrix->rows; i++)
      for(uint16_t j = 0; j < matrix->cols; j++)
        pMatrix->arr[j * pMatrix->cols + i] = matrix->arr[i * matrix->cols + j];
    Matrix_Delete(matrix);
  }
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Matrix_Inv
**功能 : Inverse Matrix ( Gaussian Elimination )
**輸入 : *pMatrix, *pMatrixInv
**輸出 : none
**使用 : Matrix_Inv(matrix, matrixInv);
**====================================================================================================*/
/*====================================================================================================*/
void Matrix_Inv( matrix_t *pMatrix, matrix_t *pMatrixInv )
{
  if((pMatrix->rows != pMatrix->cols) || (pMatrix->rows != pMatrixInv->rows))
    MATRIX_ERROR(pMatrix);

  /* check nonsingular */
  // if matrix is nonsingular
  // return ERROR;

  matrix_t *matrix     = Matrix_Create(pMatrixInv->rows, pMatrixInv->rows << 1);
  matrix_t *matrixDiag = Matrix_CreateDiag(pMatrixInv->rows, 1.0);
  Matrix_SetMatrix(matrix, pMatrixInv, 0, 0);
  Matrix_SetMatrix(matrix, matrixDiag, 0, pMatrixInv->cols);
  Matrix_Delete(matrixDiag);

  Matrix_GaussianElimination(matrix, matrix);

  Matrix_GetMatrix(pMatrix, matrix, 0, pMatrixInv->cols);
  Matrix_Delete(matrix);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Matrix_GaussianElimination
**功能 : Gaussian Elimination
**輸入 : *pMatrix, *pMatrixGE
**輸出 : none
**使用 : Matrix_GaussianElimination(matrix, matrixGE);
**====================================================================================================*/
/*====================================================================================================*/
void Matrix_GaussianElimination( matrix_t *pMatrix, matrix_t *pMatrixGE )
{
  double tmp;

  if(pMatrix != pMatrixGE)
    Matrix_Copy(pMatrix, pMatrixGE);

  /* left-down to zero */
  for(uint16_t i = 0; i < pMatrix->rows - 1; i++) {
    for(uint16_t j = i + 1; j < pMatrix->rows; j++) {
      tmp = pMatrix->arr[j * pMatrix->cols + i] / pMatrix->arr[i * pMatrix->cols + i];
      pMatrix->arr[j * pMatrix->cols + i] = 0;
      for(uint16_t k = i + 1; k < pMatrix->cols; k++)
        pMatrix->arr[j * pMatrix->cols + k] -= tmp * pMatrix->arr[i * pMatrix->cols + k];
    }
  }

  /* diagonal to one */
  for(uint16_t  i = 0; i < pMatrix->rows; i++) {
    tmp = pMatrix->arr[i * pMatrix->cols + i];
    pMatrix->arr[i * pMatrix->cols + i] = 1.0;
    for(uint16_t  j = i + 1; j < pMatrix->cols; j++)
      pMatrix->arr[i * pMatrix->cols + j] = pMatrix->arr[i * pMatrix->cols + j] / tmp;
  }

  /* right-up to zero */
  for(int16_t i = pMatrix->rows - 1; i > 0; i--) {
    for(int16_t j = i - 1; j >= 0; j--) {
      tmp = pMatrix->arr[j * pMatrix->cols + i];
      pMatrix->arr[j * pMatrix->cols + i] = 0;
      for(uint16_t  k = pMatrix->rows; k < pMatrix->cols; k++)
        pMatrix->arr[j * pMatrix->cols + k] -= tmp * pMatrix->arr[i * pMatrix->cols + k];
    }
  }

}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Matrix_Det
**功能 : Determinant
**輸入 : *pMatrix
**輸出 : data
**使用 : data = Matrix_Det(matrix);
**====================================================================================================*/
/*====================================================================================================*/
double Matrix_Det( matrix_t *pMatrix )
{
  double det = 0;
/*
  if(matrix->cols != matrix->rows)
    return 0;

*/
  return det;
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Matrix_Cholesky
**功能 : cholesky
**輸入 : *pMatrix, pMatrixC
**輸出 : none
**使用 : Matrix_Cholesky(matrix, matrixC);
**====================================================================================================*/
/*====================================================================================================*/
void Matrix_Cholesky( matrix_t *pMatrix, matrix_t *pMatrixC )
{
/*
  if(matrixA->cols != matrixA->rows)
    return;

  double tempSum = 0;
  for(uint16_t j = 0; j < matrix->cols; j++) {
    for(uint16_t i = j; i < matrix->rows; i++) {
      tempSum = 0;
      if(i == j) {
        for(uint16_t k = 0; k < i - 1; k++)
          tempSum = tempSum + sqrt_matrix(i, k)^2;
        matrix->arr[i * matrix->cols + j] = sqrt(pdMatrix[i * matrix->cols + j] - tempSum);
      }
      else {
        for(uint16_t k = 0; k < j - 1; k++)
          tempSum = tempSum + sqrt_matrix(i, k) * sqrt_matrix(j, k);
        sqrt_matrix(i, j) = (pdMatrix(i, j) - tempSum) / sqrt_matrix(j, j);
      }
    }
  }
*/
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Matrix_Print
**功能 : Print Matrix
**輸入 : *pMatrix
**輸出 : none
**使用 : Matrix_Print(matrix);
**====================================================================================================*/
/*====================================================================================================*/
void Matrix_Print( matrix_t *pMatrix )
{
  for(uint16_t i = 0; i < pMatrix->rows; i++) {
    printf(" ");
    for(uint16_t j = 0; j < pMatrix->cols; j++) {
      if(pMatrix->arr[i * pMatrix->cols + j] < 0)
        printf("%.5f\t", pMatrix->arr[i * pMatrix->cols + j]);
      else
        printf("+%.5f\t", pMatrix->arr[i * pMatrix->cols + j]);
    }
    printf("\r\n");
  }
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Matrix_PrintInfo
**功能 : Print Matrix Information
**輸入 : *pMatrix
**輸出 : none
**使用 : Matrix_PrintInfo(matrix);
**====================================================================================================*/
/*====================================================================================================*/
void Matrix_PrintInfo( matrix_t *pMatrix )
{
  printf("matrix.rows  = %d\r\n", pMatrix->rows);
  printf("matrix.cols  = %d\r\n", pMatrix->cols);
  printf("matrix.arr   = %d\r\n", (uint32_t)pMatrix->arr);
  if(pMatrix->mType == MTYPE_NORMAL)
    printf("matrix.mType = NORMAL\r\n");
  else if(pMatrix->mType == MTYPE_MALLOC)
    printf("matrix.mType = MALLOC\r\n");
  else
    printf("matrix.mType = POINTER\r\n");
  printf("\r\n");
}
/*====================================================================================================*/
/*====================================================================================================*/
