/*====================================================================================================*/
/*====================================================================================================*/
#include "drivers\stm32f4_system.h"
#include "drivers\stm32f4_spi.h"

#include "module_dw1000.h"
/*====================================================================================================*/
/*====================================================================================================*/
/*====================================================================================================*/
#define SPIx                        SPI3
#define SPIx_CLK_ENABLE()           RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE)
#define SPIx_SPEED_HIGH             SPI_BaudRatePrescaler_4
#define SPIx_SPEED_LOW              SPI_BaudRatePrescaler_256

#define SPIx_CSD_PIN                GPIO_Pin_15
#define SPIx_CSD_GPIO_PORT          GPIOA
#define SPIx_CSD_H()                __GPIO_SET(SPIx_CSD_GPIO_PORT, SPIx_CSD_PIN)
#define SPIx_CSD_L()                __GPIO_RST(SPIx_CSD_GPIO_PORT, SPIx_CSD_PIN)

#define SPIx_SCK_PIN                GPIO_Pin_3
#define SPIx_SCK_GPIO_PORT          GPIOB
#define SPIx_SCK_AF                 GPIO_AF_SPI3
#define SPIx_SCK_SOURCE             GPIO_PinSource3

#define SPIx_SDO_PIN                GPIO_Pin_4
#define SPIx_SDO_GPIO_PORT          GPIOB
#define SPIx_SDO_AF                 GPIO_AF_SPI3
#define SPIx_SDO_SOURCE             GPIO_PinSource4

#define SPIx_SDI_PIN                GPIO_Pin_5
#define SPIx_SDI_GPIO_PORT          GPIOB
#define SPIx_SDI_AF                 GPIO_AF_SPI3
#define SPIx_SDI_SOURCE             GPIO_PinSource4

#define SPIx_IRQ_PIN                GPIO_Pin_8
#define SPIx_IRQ_GPIO_PORT          GPIOB

#define SPIx_RST_PIN                GPIO_Pin_9
#define SPIx_RST_GPIO_PORT          GPIOB
#define SPIx_RST_H()                __GPIO_SET(SPIx_RST_GPIO_PORT, SPIx_RST_PIN)
#define SPIx_RST_L()                __GPIO_RST(SPIx_RST_GPIO_PORT, SPIx_RST_PIN)
/*====================================================================================================*/
/*====================================================================================================*/
static DW_DeviceTypeDef KDW;
/*====================================================================================================*/
/*====================================================================================================*
**函數 : DW1000_Delay
**功能 : DW1000 Delay
**輸入 : None
**輸出 : None
**使用 : DW1000_Delay();
**====================================================================================================*/
/*====================================================================================================*/
static void DW1000_Delay( void )
{
  __IO uint32_t count = 32;
  while(count--);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : DW1000_readData
**功能 : DW1000 Read Data
**輸入 : regAddr, subIndex, *readBuf, lens
**輸出 : None
**使用 : DW1000_readData(regAddr, subIndex, readBuf, lens);
**====================================================================================================*/
/*====================================================================================================*/
static void DW1000_readData( uint16_t regAddr, uint16_t subIndex, uint8_t *readBuf, uint32_t lens )
{
  uint8_t header[3] = {0};
  uint32_t count = 0;

  if(subIndex == 0) {     // sub-index is not present
    header[count++] = (uint8_t)regAddr;
  }
  else {                  // sub-index is present
    header[count++] = (uint8_t)(0x40 | regAddr);
    if(subIndex <= 0x7F)  // 7-bit,  subIndex <= 0x7F
      header[count++] = (uint8_t)subIndex;
    else {                // 15-bit, subIndex <= 0x7FFF, extended address
      header[count++] = 0x80 | (uint8_t)subIndex;
      header[count++] = (uint8_t)(subIndex >> 7);
    }
  }

  // Read from SPI
  SPIx_CSD_L();
  for(uint8_t i = 0; i < count; i++)
    SPI_RW(SPIx, header[i]);
  for(uint8_t i = 0; i < lens; i++)
    readBuf[i] = SPI_RW(SPIx, 0x00);
  SPIx_CSD_H();

  DW1000_Delay();
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : DW1000_writeData
**功能 : DW1000 Write Data
**輸入 : regAddr, subIndex, *writeBuf, lens
**輸出 : None
**使用 : DW1000_writeData(regAddr, subIndex, writeBuf, lens);
**====================================================================================================*/
/*====================================================================================================*/
static void DW1000_writeData( uint16_t regAddr, uint16_t subIndex, const uint8_t *writeBuf, uint32_t lens )
{
  uint8_t header[3] = {0};
  uint32_t count = 0;

  if(subIndex == 0) {     // sub-index is not present
    header[count++] = 0x80 | (uint8_t)regAddr;
  }
  else {                  // sub-index is present
    header[count++] = (uint8_t)(0xC0 | regAddr);
    if(subIndex <= 0x7F)  // 7-bit,  subIndex <= 0x7F
      header[count++] = (uint8_t)subIndex;
    else {                // 15-bit, subIndex <= 0x7FFF, extended address
      header[count++] = 0x80 | (uint8_t)subIndex;
      header[count++] = (uint8_t)(subIndex >> 7);
    }
  }

  // Write to SPI
  SPIx_CSD_L();
  for(uint8_t i = 0; i < count; i++)
    SPI_RW(SPIx, header[i]);
  for(uint8_t i = 0; i < lens; i++)
    SPI_RW(SPIx, writeBuf[i]);
  SPIx_CSD_H();

  DW1000_Delay();
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : DW1000_readOPT
**功能 : DW1000 Read OPT Memory
**輸入 : memAddr
**輸出 : None
**使用 : DW1000_readOPT(memAddr);
**====================================================================================================*/
/*====================================================================================================*/
static uint32_t DW1000_readOPT( uint16_t memAddr )
{
  uint8_t buf[4] = {0};
  uint32_t *readData = (uint32_t *)buf;

  buf[0] = (uint8_t)memAddr;
  buf[1] = (memAddr >> 8) & 0x07;

  // Write OPT memory address[0:10]
  DW1000_writeData(DW1000_OTP_IF, DW1000_SUB_OTP_ADDR, buf, 2);

  // Into read mode
  buf[0] = 0x03;  // OTPRDEN = 1, OPTREAD = 1
  DW1000_writeData(DW1000_OTP_IF, DW1000_SUB_OTP_CTRL, buf, 1);
  buf[0] = 0x00;  // Clear bit 0
  DW1000_writeData(DW1000_OTP_IF, DW1000_SUB_OTP_CTRL, buf, 1);

  // Read OPT memory
  DW1000_readData(DW1000_OTP_IF, DW1000_SUB_OTP_RDAT, buf, 4);

  return (*readData);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : DW1000_setSpeed
**功能 : Set SPI Speed
**輸入 : speedSel
**輸出 : None
**使用 : DW1000_setSpeed(SPIx_SPEED_LOW);
**====================================================================================================*/
/*====================================================================================================*/
static void DW1000_setSpeed( uint8_t speedSel )
{
  SPI_InitTypeDef SPI_InitStruct;

  SPI_I2S_DeInit(SPIx);

  SPI_InitStruct.SPI_Mode              = SPI_Mode_Master;
  SPI_InitStruct.SPI_Direction         = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStruct.SPI_DataSize          = SPI_DataSize_8b;
  SPI_InitStruct.SPI_CPOL              = SPI_CPOL_High;
  SPI_InitStruct.SPI_CPHA              = SPI_CPHA_2Edge;
  SPI_InitStruct.SPI_NSS               = SPI_NSS_Soft;
  SPI_InitStruct.SPI_BaudRatePrescaler = speedSel;
  SPI_InitStruct.SPI_FirstBit          = SPI_FirstBit_MSB;
  SPI_InitStruct.SPI_CRCPolynomial     = 7;
  SPI_Init(SPIx, &SPI_InitStruct);

  SPI_Cmd(SPIx, ENABLE); 
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : DW1000_setClock
**功能 : Set Clock
**輸入 : clock
**輸出 : None
**使用 : DW1000_setClock(ENABLE_ALL_SEQ);
**====================================================================================================*/
/*====================================================================================================*/
static void DW1000_setClock( uint8_t clock )
{
  uint8_t reg[2] = {0};

  DW1000_readData(DW1000_PMSC, DW1000_SUB_PMSC_CTRL0, reg, 2);

  switch(clock) {

    case ENABLE_ALL_SEQ:
      reg[0] = 0x00;
      reg[1] = reg[1] & 0xFE;
      break;

    case FORCE_SYS_XTI:
      reg[0] = 0x01 | (reg[0] & 0xFC);
      break;

    case FORCE_SYS_PLL:
      reg[0] = 0x02 | (reg[0] & 0xFC);
      break;

    case FORCE_OTP_ON:
      reg[1] = 0x02 | reg[1];
      break;

    case FORCE_OTP_OFF:
      reg[1] = reg[1] & 0xFD;
      break;

    case READ_ACC_ON:
      reg[0] = 0x48 | (reg[0] & 0xB3);
      reg[1] = 0x80 | reg[1];
      break;

    case READ_ACC_OFF:
      reg[0] = reg[0] & 0xB3;
      reg[1] = 0x7F & reg[1];
      break;

    default:
      break;
  }

  DW1000_writeData(DW1000_PMSC, DW1000_SUB_PMSC_CTRL0, &reg[0], 1);
  DW1000_writeData(DW1000_PMSC, DW1000_SUB_PMSC_CTRL0 + 1, &reg[1], 1);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : DW1000_setCrystalTrim
**功能 : Set Crystal Trim
**輸入 : xtalt (0x00 to 0x1F, ~1.5ppm/LSB)
**輸出 : None
**使用 : DW1000_setCrystalTrim(xtalt);
**====================================================================================================*/
/*====================================================================================================*/
static void DW1000_setCrystalTrim( uint8_t xtalt )
{
  uint8_t reg;

  DW1000_readData(DW1000_FS_CTRL, DW1000_SUB_FS_XTALT, &reg,1);

  reg &= 0xE0;
  reg |= (xtalt & 0x1F);

  DW1000_writeData(DW1000_FS_CTRL, DW1000_SUB_FS_XTALT, &reg, 1);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : DW1000_getDeviceID
**功能 : Get Device ID
**輸入 : None
**輸出 : deviceID
**使用 : deviceID = DW1000_getDeviceID();
**====================================================================================================*/
/*====================================================================================================*/
static uint32_t DW1000_getDeviceID( void )
{
  uint8_t readBuf[4] = {0};
  uint32_t *ptrBuf = (uint32_t*)readBuf;

  DW1000_readData(DW1000_DEV_ID, 0, readBuf, 4);

  return *ptrBuf;
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : DW1000_Config
**功能 : DW1000 Config
**輸入 : None
**輸出 : None
**使用 : DW1000_Config();
**====================================================================================================*/
/*====================================================================================================*/
void DW1000_Config( void )
{
  GPIO_InitTypeDef GPIO_InitStruct;
  SPI_InitTypeDef SPI_InitStruct;

  /* SPI Clk ******************************************************************/
  SPIx_CLK_ENABLE();

  /* SPI AF *******************************************************************/
  GPIO_PinAFConfig(SPIx_SCK_GPIO_PORT, SPIx_SCK_SOURCE, SPIx_SCK_AF);
  GPIO_PinAFConfig(SPIx_SDO_GPIO_PORT, SPIx_SDO_SOURCE, SPIx_SDO_AF);    
  GPIO_PinAFConfig(SPIx_SDI_GPIO_PORT, SPIx_SDI_SOURCE, SPIx_SDI_AF);

  /* SPI Pin ******************************************************************/
  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;

  GPIO_InitStruct.GPIO_Pin   = SPIx_CSD_PIN;
  GPIO_Init(SPIx_CSD_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;

  GPIO_InitStruct.GPIO_Pin   = SPIx_SCK_PIN;
  GPIO_Init(SPIx_SCK_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.GPIO_Pin   = SPIx_SDO_PIN;
  GPIO_Init(SPIx_SDO_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.GPIO_Pin   = SPIx_SDI_PIN;
  GPIO_Init(SPIx_SDI_GPIO_PORT, &GPIO_InitStruct);

  SPIx_CSD_H();  // LOW ENABLE

  /* SPI Init ****************************************************************/
  SPI_InitStruct.SPI_Mode              = SPI_Mode_Master;
  SPI_InitStruct.SPI_Direction         = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStruct.SPI_DataSize          = SPI_DataSize_8b;
  SPI_InitStruct.SPI_CPOL              = SPI_CPOL_Low;
  SPI_InitStruct.SPI_CPHA              = SPI_CPHA_1Edge;
  SPI_InitStruct.SPI_NSS               = SPI_NSS_Soft;
  SPI_InitStruct.SPI_BaudRatePrescaler = SPIx_SPEED_LOW;
  SPI_InitStruct.SPI_FirstBit          = SPI_FirstBit_MSB;
  SPI_InitStruct.SPI_CRCPolynomial     = 7;
  SPI_Init(SPIx, &SPI_InitStruct);

  SPI_Cmd(SPIx, ENABLE); 
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : DW1000_Reset
**功能 : Reset DW1000
**輸入 : None
**輸出 : None
**使用 : DW1000_Reset();
**====================================================================================================*/
/*====================================================================================================*/
void DW1000_Reset( void )
{
  GPIO_InitTypeDef GPIO_InitStruct;

  GPIO_InitStruct.GPIO_Pin   = SPIx_RST_PIN;
  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(SPIx_RST_GPIO_PORT, &GPIO_InitStruct);

  SPIx_RST_L();

  GPIO_InitStruct.GPIO_Pin   = SPIx_RST_PIN;
  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AN;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(SPIx_RST_GPIO_PORT, &GPIO_InitStruct);

  delay_ms(5);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : DW1000_Init
**功能 : Init DW1000
**輸入 : None
**輸出 : None
**使用 : DW1000_Init(&KDW_config);
**====================================================================================================*/
/*====================================================================================================*/
int8_t DW1000_Init( DW_ConfigTypeDef *KDW_config )
{
  uint8_t reg[4] = {0};
  uint8_t tmp8 = 0;
//  uint16_t tmp16 = 0;
  uint32_t tmp32 = 0;

  DW1000_Reset();

  // check device
  KDW.deviceID = DW1000_getDeviceID();
  if(KDW.deviceID != DW1000_DEVICE_ID)
    return ERROR;

  // system clock - 19.2 MHz XTI
  DW1000_setClock(FORCE_SYS_XTI);

  // PLLLDT = 1, PLL lock detect flags
  tmp8 = 0x04;
  DW1000_writeData(DW1000_EXT_SYNC, DW1000_SUB_EC_CTRL, &tmp8, 1);

  // load LDO tune from OTP
  tmp32 = DW1000_readOPT(LDOTUNE_ADDR);
  if((tmp32 & 0xFF) != 0) {
    tmp8 = 0x02;  // LDO_KICK = 1
    DW1000_writeData(DW1000_OTP_IF, DW1000_SUB_OTP_SF, &tmp8, 1);
  }

  // read device info
  tmp32 = DW1000_readOPT(XTRIM_ADDR) & 0xFFFF;
  KDW.xtrim  = tmp32 & 0x1F;
  KDW.otpRev = (tmp32 >> 8) & 0xFF;
  KDW.partID = DW1000_readOPT(PARTID_ADDR);
  KDW.lotID  = DW1000_readOPT(LOTID_ADDR);

  // configure XTAL trim
  if(KDW.xtrim == 0x00)
    KDW.xtrim = 0x10;   // middle range
  DW1000_setCrystalTrim(KDW.xtrim);

//printf("KDW.deviceID = %X\r\n", KDW_device.deviceID);
//printf("KDW.otpRev   = %X\r\n", KDW_device.otpRev);
//printf("KDW.partID   = %X\r\n", KDW_device.partID);
//printf("KDW.lotID    = %X\r\n", KDW_device.lotID);
//printf("KDW.xtrim    = %X\r\n", KDW_device.xtrim);

  // Load leading edge(LDE) detect code
  // see dw1000_user_manual - 2.5.4 Default Receiver Configuration
  if(KDW_config->loadCode == ENABLE) {   // load microcode
    reg[0] = 0x01;  // SYSCLKS = 1
    reg[1] = 0x03;  // TXCLKS = 11
    DW1000_writeData(DW1000_PMSC, DW1000_SUB_PMSC_CTRL0, reg, 2);
    reg[0] = 0x00;
    reg[1] = 0x80;  // LDELOAD = 1
    DW1000_writeData(DW1000_OTP_IF, DW1000_SUB_OTP_CTRL, reg, 2);

    // wait upload
    delay_ms(1);

    DW1000_setClock(ENABLE_ALL_SEQ);
  }
  else {
    DW1000_readData(DW1000_PMSC, DW1000_SUB_PMSC_CTRL1 + 1, reg, 2);
    reg[1] &= 0xFD; // LDERUN = 0
    DW1000_writeData(DW1000_PMSC, DW1000_SUB_PMSC_CTRL1 + 1, reg, 2);
  }

  // Enable clocks for sequencing
  DW1000_setClock(ENABLE_ALL_SEQ);
  delay_ms(5);

  DW1000_setSpeed(SPIx_SPEED_HIGH);

  return SUCCESS;
}
/*==============================================================================================*/
/*==============================================================================================*/
