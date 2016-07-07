/*====================================================================================================*/
/*====================================================================================================*/
#include "drivers\stm32f4_system.h"
#include "drivers\stm32f4_flash.h"
#include "applications\imu_param.h"
/*====================================================================================================*/
/*====================================================================================================*/
#define FLASH_SAVE_ADDR     FLASH_ADDR_SECTOR_7               // 128 Kbytes
#define FLASH_SAVE_SECTOR   Flash_GetSector(FLASH_SAVE_ADDR)

void IMU_ParamClearSector( void )
{
  Flash_EraseSector(FLASH_SAVE_SECTOR);
}
#define IMU_PARAM_LENS 36
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
  writeFp[20] = pIMU->MagGain[0];
  writeFp[21] = pIMU->MagGain[1];
  writeFp[22] = pIMU->MagGain[2];
  writeFp[23] = pIMU->MagGain[3];
  writeFp[24] = pIMU->MagGain[4];
  writeFp[25] = pIMU->MagGain[5];
  writeFp[26] = pIMU->TempGain;
  writeFp[27] = pIMU->PresGain;

  writeFp[28] = pIMU->GyrSens;
  writeFp[29] = pIMU->AccSens;
  writeFp[30] = pIMU->ICTempSens;
  writeFp[31] = pIMU->MagSens;
  writeFp[32] = pIMU->TempSens;
  writeFp[33] = pIMU->PresSens;

  writeFp[34] = 0;
  for(uint8_t i = 0; i < IMU_PARAM_LENS - 2; i++) {
    writeFp[34] += writeFp[i];
  }
  writeFp[35] = 0;

  Flash_WritePageU32(FLASH_SAVE_ADDR, (uint32_t *)writeFp, IMU_PARAM_LENS);
  Flash_ReadPageU32(FLASH_SAVE_ADDR, (uint32_t *)readFp, IMU_PARAM_LENS);
  for(uint8_t i = 0; i < IMU_PARAM_LENS; i++) {
    if(readFp[i] != writeFp[i])
      return ERROR;
  }

  return SUCCESS;
}

int8_t IMU_ParamRead( IMU_DataTypeDef *pIMU )
{
  float checkSum = 0;
  float readFp[IMU_PARAM_LENS] = {0};

  Flash_ReadPageU32(FLASH_SAVE_ADDR, (uint32_t *)readFp, IMU_PARAM_LENS);
  for(uint8_t i = 0; i < IMU_PARAM_LENS - 2; i++) {
    checkSum += readFp[i];
  }
  if(checkSum == readFp[34]) {
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
    pIMU->MagGain[0]   = readFp[20];
    pIMU->MagGain[1]   = readFp[21];
    pIMU->MagGain[2]   = readFp[22];
    pIMU->MagGain[3]   = readFp[23];
    pIMU->MagGain[4]   = readFp[24];
    pIMU->MagGain[5]   = readFp[25];
    pIMU->TempGain     = readFp[26];
    pIMU->PresGain     = readFp[27];

    pIMU->GyrSens      = readFp[28];
    pIMU->AccSens      = readFp[29];
    pIMU->ICTempSens   = readFp[30];
    pIMU->MagSens      = readFp[31];
    pIMU->TempSens     = readFp[32];
    pIMU->PresSens     = readFp[33];
  }
  else {
    return ERROR;
  }

  return SUCCESS;
}
/*====================================================================================================*/
/*====================================================================================================*/
