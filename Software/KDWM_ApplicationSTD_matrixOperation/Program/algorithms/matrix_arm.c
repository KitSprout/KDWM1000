/*====================================================================================================*/
/*====================================================================================================*/
#include "drivers\stm32f4_system.h"
#include "modules\serial.h"
#include "algorithms\matrix_arm.h"

#include <string.h>
/*====================================================================================================*/
/*====================================================================================================*/
#ifdef USE_MATRIX_BUFFER
#define MATRIX_MAX_DIMENSION  ((uint32_t)12)
#define MATRIX_MAX_SIZE       ((uint32_t)144)   /* MATRIX_MAX_DIMENSION ^ 2 */
#define MATRIX_BUF_SIZE       ((uint32_t)MATRIX_MAX_SIZE << 1)

static float64_t matrixBuf[MATRIX_BUF_SIZE] = {0};
#endif
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
  memset(pMatrix->pData, 0, sizeof(float32_t) * pMatrix->numRows * pMatrix->numCols);
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
void Matrix_Init( matrix_t *pMatrix, float32_t *pArray, uint16_t rows, uint16_t cols )
{
  pMatrix->numRows = rows;
  pMatrix->numCols = cols;
  pMatrix->pData   = pArray;
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Matrix_Copy
**功能 : Copy Matrix
**輸入 : *pMatrix, *pMatrixC
**輸出 : status
**使用 : status = Matrix_Copy(matrix, matrixC);    // matrix = matrixC;
**====================================================================================================*/
/*====================================================================================================*/
arm_status Matrix_Copy( matrix_t *pMatrix, const matrix_t *pMatrixC )
{
  uint32_t i = 0;

  if((pMatrix->numRows != pMatrixC->numRows) ||
     (pMatrix->numCols != pMatrixC->numCols)) {
    return ARM_MATH_SIZE_MISMATCH;
  }

  for(i = 0; i < pMatrix->numRows * pMatrix->numCols; i++) {
    pMatrix->pData[i] = pMatrixC->pData[i];
  }

  return ARM_MATH_SUCCESS;
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Matrix_SetData
**功能 : Set Data
**輸入 : *pMatrix, rows, cols, data
**輸出 : status
**使用 : status = Matrix_SetData(matrix, rows, cols, data);    // matrix(rows, cols) = data;
**====================================================================================================*/
/*====================================================================================================*/
arm_status Matrix_SetData( matrix_t *pMatrix, uint16_t rows, uint16_t cols, float32_t data )
{
  uint32_t index = ((rows - 1) * pMatrix->numCols + (cols - 1));

  if((rows > pMatrix->numRows) ||
     (cols > pMatrix->numCols)) {
    return ARM_MATH_LENGTH_ERROR; 
  }

  pMatrix->pData[index] = data;

  return ARM_MATH_SUCCESS;
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Matrix_GetData
**功能 : Get Data
**輸入 : *pMatrix, rows, cols, *data
**輸出 : status
**使用 : status = Matrix_GetData(matrix, rows, cols, &data);    // data = matrix(rows, cols);
**====================================================================================================*/
/*====================================================================================================*/
arm_status Matrix_GetData( const matrix_t *pMatrix, uint16_t rows, uint16_t cols, float32_t *data )
{
  uint32_t index = ((rows - 1) * pMatrix->numCols + (cols - 1));

  if((rows > pMatrix->numRows) ||
     (cols > pMatrix->numCols)) {
    return ARM_MATH_LENGTH_ERROR; 
  }

  *data = pMatrix->pData[index];

  return ARM_MATH_SUCCESS;
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Matrix_SetDiag
**功能 : Set Diagonal Matrix
**輸入 : *pMatrix, data
**輸出 : status
**使用 : status = Matrix_SetDiag(matrix, data);    // matrix = diag(data);
**====================================================================================================*/
/*====================================================================================================*/
arm_status Matrix_SetDiag( matrix_t *pMatrix, float32_t data )
{
  uint32_t i = 0;

  if(pMatrix->numRows != pMatrix->numCols) {
    return ARM_MATH_SIZE_MISMATCH;
  }

  for(i = 0; i < pMatrix->numRows; i++) {
    pMatrix->pData[i * pMatrix->numRows + i] = data;
  }

  return ARM_MATH_SUCCESS;
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Matrix_GetDiag
**功能 : Get Diagonal
**輸入 : *pMatrix, pMatrixD
**輸出 : status
**使用 : status = Matrix_GetDiag(matrix, matrixD);    // matrix = diag(matrixD);
**====================================================================================================*/
/*====================================================================================================*/
arm_status Matrix_GetDiag( matrix_t *pMatrix, matrix_t *pMatrixD )
{
  uint32_t i = 0, j = 0;

  if((pMatrix->numRows != pMatrixD->numRows) ||
     (pMatrix->numCols != pMatrixD->numCols) ||
     (pMatrix->numRows != pMatrix->numCols)) {
    return ARM_MATH_LENGTH_ERROR; 
  }

  if(pMatrix != pMatrixD) {
    Matrix_Clear(pMatrix);
    for(i = 0; i < pMatrix->numRows; i++) {
      pMatrix->pData[i * pMatrix->numRows + i] = pMatrixD->pData[i * pMatrixD->numRows + i];
    }
  }
  else {
    for(i = 0; i < pMatrix->numRows; i++) {
      for(j = 0; j < pMatrix->numCols; j++) {
        if(i != j) {
          pMatrix->pData[i * pMatrix->numCols + j] = 0.0f;
        }
      }
    }
  }

  return ARM_MATH_SUCCESS;
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Matrix_SetMatrix
**功能 : Set Matrix
**輸入 : *pMatrix, *pMatrixS, rows_pos, cols_pos
**輸出 : status
**使用 : status = Matrix_SetMatrix(matrix, matrixS, rows_pos, cols_pos);    // matrixS(rpos:i, cpos:j) = matrix;
**====================================================================================================*/
/*====================================================================================================*/
arm_status Matrix_SetMatrix( matrix_t *pMatrix, matrix_t *pMatrixS, uint16_t rows_pos, uint16_t cols_pos )
{
  uint32_t cnt = 0;
  uint32_t i = 0, j = 0;

  const uint32_t rpos_total = rows_pos + pMatrixS->numRows;
  const uint32_t cpos_total = cols_pos + pMatrixS->numCols;
  
  if(((rows_pos + pMatrixS->numRows) > pMatrix->numRows) ||
     ((cols_pos + pMatrixS->numCols) > pMatrix->numCols)) {
    return ARM_MATH_LENGTH_ERROR; 
  }

  for(i = rows_pos; i < rpos_total; i++) {
    for(j = cols_pos; j < cpos_total; j++) {
      pMatrix->pData[i * pMatrix->numCols + j] = pMatrixS->pData[cnt++];
    }
  }

  return ARM_MATH_SUCCESS;
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Matrix_GetMatrix
**功能 : Get Matrix
**輸入 : *pMatrixG, *pMatrix, rows_pos, cols_pos
**輸出 : status
**使用 : status = Matrix_GetMatrix(matrixG, matrix, rows_pos, cols_pos);    // matrix = matrixG(rpos:i, cpos:j);
**====================================================================================================*/
/*====================================================================================================*/
arm_status Matrix_GetMatrix( matrix_t *pMatrixG, matrix_t *pMatrix, uint16_t rows_pos, uint16_t cols_pos )
{
  uint32_t cnt = 0;
  uint32_t i = 0, j = 0;

  const uint32_t rpos_total = rows_pos + pMatrixG->numRows;
  const uint32_t cpos_total = cols_pos + pMatrixG->numCols;
  
  if(((rows_pos + pMatrixG->numRows) > pMatrix->numRows) ||
     ((cols_pos + pMatrixG->numCols) > pMatrix->numCols)) {
    return ARM_MATH_LENGTH_ERROR; 
  }

  for(i = rows_pos; i < rpos_total; i++) {
    for(j = cols_pos; j < cpos_total; j++) {
      pMatrixG->pData[cnt++] = pMatrix->pData[i * pMatrix->numCols + j];
    }
  }

  return ARM_MATH_SUCCESS;
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Matrix_Add
**功能 : Matrix Addition
**輸入 : *pMatrix, *pMatrixA1, *pMatrixA2
**輸出 : status
**使用 : status = Matrix_Add(matrix, matrixA1, matrixA2);    // matrix = matrixA1 + matrixA2;
**====================================================================================================*/
/*====================================================================================================*/
inline arm_status Matrix_Add( matrix_t *pMatrix, const matrix_t *pMatrixA1, const matrix_t *pMatrixA2 )
{
  return arm_mat_add_f32(pMatrixA1, pMatrixA2, pMatrix);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Matrix_Sub
**功能 : Matrix Subtraction
**輸入 : *pMatrix, *pMatrixS1, *pMatrixS2
**輸出 : status
**使用 : status = Matrix_Sub(matrix, matrixS1, matrixS2);    // matrix = matrixS1 - matrixS2;
**====================================================================================================*/
/*====================================================================================================*/
inline arm_status Matrix_Sub( matrix_t *pMatrix, const matrix_t *pMatrixS1, const matrix_t *pMatrixS2 )
{
  return arm_mat_sub_f32(pMatrixS1, pMatrixS2, pMatrix);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Matrix_Mult
**功能 : Matrix Multiplication
**輸入 : *pMatrix, *pMatrixM1, *pMatrixM1
**輸出 : status
**使用 : status = Matrix_Mult(matrix, matrixM1, matrixM2);    // matrix = matrixM1 * matrixM2;
**====================================================================================================*/
/*====================================================================================================*/
#ifdef USE_MATRIX_BUFFER
arm_status Matrix_Mult( matrix_t *pMatrix, const matrix_t *pMatrixM1, const matrix_t *pMatrixM2 )
{
  if((pMatrix->numRows   != pMatrixM1->numRows) ||
     (pMatrix->numCols   != pMatrixM2->numCols) ||
     (pMatrixM1->numCols != pMatrixM2->numRows) ) {
     return ARM_MATH_SIZE_MISMATCH;
  }

  if(pMatrix == pMatrixM1) {
    uint32_t i = 0;
    uint32_t total = pMatrix->numRows * pMatrix->numCols;

    matrix_t matrixM = {
      .numRows = pMatrixM1->numRows,
      .numCols = pMatrixM1->numCols,
      .pData   = (float32_t *)matrixBuf
    };
    for(i = 0; i < total; i++) {
      matrixM.pData[i] = pMatrixM1->pData[i];
    }
    return arm_mat_mult_f32(&matrixM, pMatrixM2, pMatrix);
  }
  else if(pMatrix == pMatrixM2) {
    uint32_t i = 0;
    uint32_t total = pMatrix->numRows * pMatrix->numCols;

    matrix_t matrixM = {
      .numRows = pMatrixM2->numRows,
      .numCols = pMatrixM2->numCols,
      .pData   = (float32_t *)matrixBuf
    };
    for(i = 0; i < total; i++) {
      matrixM.pData[i] = pMatrixM2->pData[i];
    }
    return arm_mat_mult_f32(pMatrixM1, &matrixM, pMatrix);
  }
  else {
    return arm_mat_mult_f32(pMatrixM1, pMatrixM2, pMatrix);
  }
}
#else
inline arm_status Matrix_Mult( matrix_t *pMatrix, const matrix_t *pMatrixM1, const matrix_t *pMatrixM2 )
{
  return arm_mat_mult_f32(pMatrixM1, pMatrixM2, pMatrix);
}
#endif
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Matrix_MultScale
**功能 : Matrix Multiplication scale
**輸入 : *pMatrix, *pMatrixM1, scale
**輸出 : status
**使用 : status = Matrix_MultScale(matrix, matrixM1, scale);    // matrix = matrixM1 * scale;
**====================================================================================================*/
/*====================================================================================================*/
inline arm_status Matrix_MultScale( matrix_t *pMatrix, const matrix_t *pMatrixM, float32_t scale )
{
  return arm_mat_scale_f32(pMatrixM, scale, pMatrix);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Matrix_Trans
**功能 : Matrix Transpose
**輸入 : *pMatrix, *pMatrixT
**輸出 : status
**使用 : status = Matrix_Trans(matrix, matrixT);    // matrix = matrixT';
**====================================================================================================*/
/*====================================================================================================*/
#ifdef USE_MATRIX_BUFFER
arm_status Matrix_Trans( matrix_t *pMatrix, const matrix_t *pMatrixT )
{
  if(pMatrix == pMatrixT) {
    uint32_t i = 0;
    uint32_t total = pMatrix->numRows * pMatrix->numCols;

    matrix_t matrixTr = {
      .numRows = pMatrixT->numRows,
      .numCols = pMatrixT->numCols,
      .pData   = (float32_t *)matrixBuf
    };
    for(i = 0; i < total; i++) {
      matrixTr.pData[i] = pMatrixT->pData[i];
    }

    pMatrix->numCols = matrixTr.numRows;
    pMatrix->numRows = matrixTr.numCols;

    return arm_mat_trans_f32(&matrixTr, pMatrix);
  }
  else {
    return arm_mat_trans_f32(pMatrixT, pMatrix);
  }
}
#else
inline arm_status Matrix_Trans( matrix_t *pMatrix, const matrix_t *pMatrixT )
{
  return arm_mat_trans_f32(pMatrixT, pMatrix);
}
#endif
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Matrix_Inv
**功能 : Matrix inverse
**輸入 : *pMatrix, *pMatrixI
**輸出 : status
**使用 : status = Matrix_Inv(matrix, matrixI);    // matrix = inv(matrixI);
**====================================================================================================*/
/*====================================================================================================*/
#ifdef USE_MATRIX_BUFFER
arm_status Matrix_Inv( matrix_t *pMatrix, const matrix_t *pMatrixI )
{
  if(pMatrix == pMatrixI) {
    uint32_t i = 0;
    uint32_t total = pMatrix->numRows * pMatrix->numCols;

    matrix_t matrixInv = {
      .numRows = pMatrix->numRows,
      .numCols = pMatrix->numCols,
      .pData   = (float32_t *)matrixBuf
    };

    for(i = 0; i < total; i++) {
      matrixInv.pData[i] = pMatrix->pData[i];
    }
    return arm_mat_inverse_f32(&matrixInv, pMatrix);
  }
  else {
    return arm_mat_inverse_f32(pMatrixI, pMatrix);
  }
}
#else
inline arm_status Matrix_Inv( matrix_t *pMatrix, const matrix_t *pMatrixI )
{
  return arm_mat_inverse_f32(pMatrixI, pMatrix);
}
#endif

/*====================================================================================================*/
/*====================================================================================================*
**函數 : Matrix_Inv64
**功能 : Inverse Matrix
**輸入 : *pMatrix, *pMatrixInv
**輸出 : status
**使用 : status = Matrix_Inv64(matrix, matrixI);    // matrix = (float32)inv64(matrixI);
**====================================================================================================*/
/*====================================================================================================*/
#ifdef USE_MATRIX_BUFFER
arm_status Matrix_Inv64( matrix_t *pMatrix, const matrix_t *pMatrixI )
{
  uint32_t i = 0;
  uint32_t total = pMatrix->numRows * pMatrix->numCols;

  arm_status status;

  matrix64_t matrixCov = {
    .numRows = pMatrix->numRows,
    .numCols = pMatrix->numCols,
    .pData   = matrixBuf
  };

  matrix64_t matrixInv = {
    .numRows = pMatrixI->numRows,
    .numCols = pMatrixI->numCols,
    .pData   = &matrixBuf[MATRIX_MAX_SIZE]
  };

  for(i = 0; i < total; i++) {
    matrixInv.pData[i] = (float64_t)(pMatrixI->pData[i]);
  }

  status = arm_mat_inverse_f64(&matrixInv, &matrixCov);

  for(i = 0; i < total; i++) {
    pMatrix->pData[i] = (float32_t)(matrixCov.pData[i]);
  }

  return (status);
}
#endif
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Matrix_Det
**功能 : Determinant
**輸入 : *pMatrix
**輸出 : data
**使用 : data = Matrix_Det(matrix);
**====================================================================================================*/
/*====================================================================================================*/
//matrix_float_t Matrix_Det( matrix_t *pMatrix )
//{
//}
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
  uint32_t i = 0, j = 0;

  for(i = 0; i < pMatrix->numRows; i++) {
    printf(" ");
    for(j = 0; j < pMatrix->numCols; j++) {
      if(pMatrix->pData[i * pMatrix->numCols + j] < 0)
        printf("%.5f\t", pMatrix->pData[i * pMatrix->numCols + j]);
      else
        printf("+%.5f\t", pMatrix->pData[i * pMatrix->numCols + j]);
    }
    printf("\r\n");
  }
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Matrix_Print64
**功能 : Print float64 Matrix
**輸入 : *pMatrix
**輸出 : none
**使用 : Matrix_Print64(matrix);
**====================================================================================================*/
/*====================================================================================================*/
void Matrix_Print64( matrix64_t *pMatrix )
{
  uint32_t i = 0, j = 0;

  for(i = 0; i < pMatrix->numRows; i++) {
    printf(" ");
    for(j = 0; j < pMatrix->numCols; j++) {
      if(pMatrix->pData[i * pMatrix->numCols + j] < 0)
        printf("%.5f\t", pMatrix->pData[i * pMatrix->numCols + j]);
      else
        printf("+%.5f\t", pMatrix->pData[i * pMatrix->numCols + j]);
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
  printf("matrix.rows = %d\r\n", pMatrix->numRows);
  printf("matrix.cols = %d\r\n", pMatrix->numCols);
  printf("matrix.data = %d\r\n", (uint32_t)pMatrix->pData);
  printf("\r\n");
}
/*====================================================================================================*/
/*====================================================================================================*/
