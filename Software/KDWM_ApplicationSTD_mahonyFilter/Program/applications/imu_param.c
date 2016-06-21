/*====================================================================================================*/
/*====================================================================================================*/
#include "drivers\stm32f4_system.h"
#include "drivers\stm32f4_flash.h"
#include "applications\imu_param.h"
/*====================================================================================================*/
/*====================================================================================================*/
#define FLASH_SAVE_ADDR     FLASH_ADDR_SECTOR_7               // 128 Kbytes
#define FLASH_SAVE_SECTOR   Flash_GetSector(FLASH_SAVE_ADDR)

#define IMU_PARAM_LENS      42
/*====================================================================================================*/
/*====================================================================================================*
**函數 : IMU_ParamClearSector
**功能 : Clear Sector
**輸入 : None
**輸出 : None
**使用 : IMU_ParamClearSector();
**====================================================================================================*/
/*====================================================================================================*/
void IMU_ParamClearSector( void )
{
  Flash_EraseSector(FLASH_SAVE_SECTOR);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : IMU_ParamWrite
**功能 : Write Parameter to FLASH
**輸入 : *pIMU
**輸出 : state
**使用 : state = IMU_ParamWrite(&IMU);
**====================================================================================================*/
/*====================================================================================================*/
int8_t IMU_ParamWrite( IMU_DataTypeDef *pIMU )
{
  float readFp[IMU_PARAM_LENS] = {0};
  float writeFp[IMU_PARAM_LENS] = {0};

  writeFp[0]  = pIMU->GyrOffset[0];
  writeFp[1]  = pIMU->GyrOffset[1];
  writeFp[2]  = pIMU->GyrOffset[2];
  writeFp[3]  = pIMU->AccOffset[0];
  writeFp[4]  = pIMU->AccOffset[1];
  writeFp[5]  = pIMU->AccOffset[2];
  writeFp[6]  = pIMU->MagOffset[0];
  writeFp[7]  = pIMU->MagOffset[1];
  writeFp[8]  = pIMU->MagOffset[2];
  writeFp[9]  = pIMU->TempOffset;
  writeFp[10] = pIMU->PresOffset;

  writeFp[11] = pIMU->GyrGain[0];
  writeFp[12] = pIMU->GyrGain[1];
  writeFp[13] = pIMU->GyrGain[2];
  writeFp[14] = pIMU->AccGain[0];
  writeFp[15] = pIMU->AccGain[1];
  writeFp[16] = pIMU->AccGain[2];
  writeFp[17] = pIMU->AccGain[3];
  writeFp[18] = pIMU->AccGain[4];
  writeFp[19] = pIMU->AccGain[5];
  writeFp[20] = pIMU->AccGain[6];
  writeFp[21] = pIMU->AccGain[7];
  writeFp[22] = pIMU->AccGain[8];
  writeFp[23] = pIMU->MagGain[0];
  writeFp[24] = pIMU->MagGain[1];
  writeFp[25] = pIMU->MagGain[2];
  writeFp[26] = pIMU->MagGain[3];
  writeFp[27] = pIMU->MagGain[4];
  writeFp[28] = pIMU->MagGain[5];
  writeFp[29] = pIMU->MagGain[6];
  writeFp[30] = pIMU->MagGain[7];
  writeFp[31] = pIMU->MagGain[8];
  writeFp[32] = pIMU->TempGain;
  writeFp[33] = pIMU->PresGain;

  writeFp[34] = pIMU->GyrSens;
  writeFp[35] = pIMU->AccSens;
  writeFp[36] = pIMU->ICTempSens;
  writeFp[37] = pIMU->MagSens;
  writeFp[38] = pIMU->TempSens;
  writeFp[39] = pIMU->PresSens;

  writeFp[40] = 0;
  for(uint8_t i = 0; i < IMU_PARAM_LENS - 2; i++) {
    writeFp[40] += writeFp[i];
  }
  writeFp[41] = 0;

  Flash_WritePageU32(FLASH_SAVE_ADDR, (uint32_t *)writeFp, IMU_PARAM_LENS);
  Flash_ReadPageU32(FLASH_SAVE_ADDR, (uint32_t *)readFp, IMU_PARAM_LENS);
  for(uint8_t i = 0; i < IMU_PARAM_LENS; i++) {
    if(readFp[i] != writeFp[i])
      return ERROR;
  }

  return SUCCESS;
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : IMU_ParamRead
**功能 : Read Parameter from FLASH
**輸入 : *pIMU
**輸出 : None
**使用 : state = IMU_ParamRead(&pIMU);
**====================================================================================================*/
/*====================================================================================================*/
int8_t IMU_ParamRead( IMU_DataTypeDef *pIMU )
{
  float checkSum = 0;
  float readFp[IMU_PARAM_LENS] = {0};

  Flash_ReadPageU32(FLASH_SAVE_ADDR, (uint32_t *)readFp, IMU_PARAM_LENS);
  for(uint8_t i = 0; i < IMU_PARAM_LENS - 2; i++) {
    checkSum += readFp[i];
  }
  if(checkSum == readFp[40]) {
    pIMU->GyrOffset[0] = readFp[0];
    pIMU->GyrOffset[1] = readFp[1];
    pIMU->GyrOffset[2] = readFp[2];
    pIMU->AccOffset[0] = readFp[3];
    pIMU->AccOffset[1] = readFp[4];
    pIMU->AccOffset[2] = readFp[5];
    pIMU->MagOffset[0] = readFp[6];
    pIMU->MagOffset[1] = readFp[7];
    pIMU->MagOffset[2] = readFp[8];
    pIMU->TempOffset   = readFp[9];
    pIMU->PresOffset   = readFp[10];

    pIMU->GyrGain[0]   = readFp[11];
    pIMU->GyrGain[1]   = readFp[12];
    pIMU->GyrGain[2]   = readFp[13];
    pIMU->AccGain[0]   = readFp[14];
    pIMU->AccGain[1]   = readFp[15];
    pIMU->AccGain[2]   = readFp[16];
    pIMU->AccGain[3]   = readFp[17];
    pIMU->AccGain[4]   = readFp[18];
    pIMU->AccGain[5]   = readFp[19];
    pIMU->AccGain[6]   = readFp[20];
    pIMU->AccGain[7]   = readFp[21];
    pIMU->AccGain[8]   = readFp[22];
    pIMU->MagGain[0]   = readFp[23];
    pIMU->MagGain[1]   = readFp[24];
    pIMU->MagGain[2]   = readFp[25];
    pIMU->MagGain[3]   = readFp[26];
    pIMU->MagGain[4]   = readFp[27];
    pIMU->MagGain[5]   = readFp[28];
    pIMU->MagGain[6]   = readFp[29];
    pIMU->MagGain[7]   = readFp[30];
    pIMU->MagGain[8]   = readFp[31];
    pIMU->TempGain     = readFp[32];
    pIMU->PresGain     = readFp[33];

    pIMU->GyrSens      = readFp[34];
    pIMU->AccSens      = readFp[35];
    pIMU->ICTempSens   = readFp[36];
    pIMU->MagSens      = readFp[37];
    pIMU->TempSens     = readFp[38];
    pIMU->PresSens     = readFp[39];
  }
  else {
    return ERROR;
  }

  return SUCCESS;
}
/*====================================================================================================*/
/*====================================================================================================*/
