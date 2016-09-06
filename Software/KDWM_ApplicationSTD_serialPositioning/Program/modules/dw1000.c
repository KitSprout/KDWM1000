/*====================================================================================================*/
/*====================================================================================================*/
#include "drivers\stm32f4_system.h"
#include "drivers\stm32f4_spi.h"
#include "modules\dw1000.h"
/*====================================================================================================*/
/*====================================================================================================*/
#define SPIx                        SPI3
#define SPIx_CLK_ENABLE()           RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE)
#define SPIx_SPEED_HIGH             SPI_BaudRatePrescaler_4
#define SPIx_SPEED_LOW              SPI_BaudRatePrescaler_64

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
#define SPIx_SDI_SOURCE             GPIO_PinSource5

#define SPIx_IRQ_PIN                GPIO_Pin_8
#define SPIx_IRQ_GPIO_PORT          GPIOB

#define SPIx_RST_PIN                GPIO_Pin_9
#define SPIx_RST_GPIO_PORT          GPIOB
#define SPIx_RST_H()                __GPIO_SET(SPIx_RST_GPIO_PORT, SPIx_RST_PIN)
#define SPIx_RST_L()                __GPIO_RST(SPIx_RST_GPIO_PORT, SPIx_RST_PIN)
/*====================================================================================================*/
/*====================================================================================================*/
//static DW_DeviceTypeDef KDW;
/*====================================================================================================*/
/*====================================================================================================*
**函數 : DW1000_readData
**功能 : DW1000 Read Data
**輸入 : regAddr, subIndex, *readBuf, lens
**輸出 : None
**使用 : DW1000_readData(regAddr, subIndex, readBuf, lens);
**====================================================================================================*/
/*====================================================================================================*/
void DW1000_readData( uint16_t regAddr, uint16_t subIndex, uint8_t *readBuf, uint32_t lens )
{
  uint8_t header[3] = {0};
  uint32_t count = 0;

  if(subIndex == 0) {
    header[count++] = (uint8_t)regAddr;
  }
  else {
    header[count++] = 0x40 | (uint8_t)regAddr;
    if(subIndex < 0xF0) {   /* 7-bit, subIndex <= 0x7F */
      header[count++] = (uint8_t)subIndex;
    }
    else {                  /* 15-bit, subIndex <= 0x7FFF, extended address */
      header[count++] = 0x80 | (uint8_t)subIndex;
      header[count++] = (uint8_t)(subIndex >> 7);
    }
  }

  /* Read from SPI */
  SPIx_CSD_L();
  for(uint16_t i = 0; i < count; i++)
    readBuf[0] = SPI_RW(SPIx, header[i]);
  for(uint16_t i = 0; i < lens; i++)
    readBuf[i] = SPI_RW(SPIx, 0x00);
  SPIx_CSD_H();
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
void DW1000_writeData( uint16_t regAddr, uint16_t subIndex, const uint8_t *writeBuf, uint32_t lens )
{
  uint8_t header[3] = {0};
  uint32_t count = 0;

  if(subIndex == 0) {
    header[count++] = 0x80 | (uint8_t)regAddr;
  }
  else {
    header[count++] = 0xC0 | (uint8_t)regAddr;
    if(subIndex < 0xF0) {   /* 7-bit, subIndex <= 0x7F */
      header[count++] = (uint8_t)subIndex;
    }
    else {                  /* 15-bit, subIndex <= 0x7FFF, extended address */
      header[count++] = 0x80 | (uint8_t)subIndex;
      header[count++] = (uint8_t)(subIndex >> 7);
    }
  }

  /* Write to SPI */
  SPIx_CSD_L();
  for(uint16_t i = 0; i < count; i++)
    SPI_RW(SPIx, header[i]);
  for(uint16_t i = 0; i < lens; i++)
    SPI_RW(SPIx, writeBuf[i]);
  SPIx_CSD_H();

//  DW1000_Delay();
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : DW1000_readData16
**功能 : DW1000 Read 16-bit Data
**輸入 : regAddr, subIndex
**輸出 : readData
**使用 : readData = DW1000_readData16(regAddr, subIndex);
**====================================================================================================*/
/*====================================================================================================*/
uint16_t DW1000_readData16( int32_t regAddr, uint16_t subIndex )
{
  uint8_t readBuf[2] = {0};
  uint16_t *readData = (uint16_t*)readBuf;

  DW1000_readData(regAddr, subIndex, readBuf, 2);

  return *readData;
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : DW1000_writeData16
**功能 : DW1000 Write 16-bit Data
**輸入 : regAddr, subIndex, writeData
**輸出 : None
**使用 : DW1000_writeData16(regAddr, subIndex, writeData);
**====================================================================================================*/
/*====================================================================================================*/
void DW1000_writeData16( int32_t regAddr, uint16_t subIndex, uint16_t writeData )
{
  DW1000_writeData(regAddr, subIndex, (uint8_t *)&writeData, 2);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : DW1000_readData32
**功能 : DW1000 Read 32-bit Data
**輸入 : regAddr, subIndex
**輸出 : readData
**使用 : readData = DW1000_readData32(regAddr, subIndex);
**====================================================================================================*/
/*====================================================================================================*/
uint32_t DW1000_readData32( int32_t regAddr, uint16_t subIndex )
{
  uint8_t readBuf[4] = {0};
  uint32_t *readData = (uint32_t*)readBuf;

  DW1000_readData(regAddr, subIndex, readBuf, 4);

  return *readData;
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : DW1000_writeData32
**功能 : DW1000 Write 32-bit Data
**輸入 : regAddr, subIndex, writeData
**輸出 : None
**使用 : DW1000_writeData32(regAddr, subIndex, writeData);
**====================================================================================================*/
/*====================================================================================================*/
void DW1000_writeData32(int32_t regAddr, uint16_t subIndex, uint32_t writeData)
{
  DW1000_writeData(regAddr, subIndex, (uint8_t *)&writeData, 4);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : DW1000_ReadOPT
**功能 : DW1000 Read OPT Memory
**輸入 : memAddr
**輸出 : None
**使用 : DW1000_ReadOPT(memAddr);
**====================================================================================================*/
/*====================================================================================================*/
uint32_t DW1000_ReadOPT( uint16_t memAddr )
{
  uint8_t buf[4] = {0};
  uint32_t *readData = (uint32_t *)buf;

  buf[0] = (uint8_t)memAddr;
  buf[1] = (memAddr >> 8) & 0x07;

  /* Write OPT memory address[0:10] */
  DW1000_writeData(DW1000_OTP_IF, DW1000_SUB_OTP_ADDR, buf, 2);

  /* Into read mode */
  buf[0] = 0x03;  /* OTPRDEN = 1, OPTREAD = 1 */
  DW1000_writeData(DW1000_OTP_IF, DW1000_SUB_OTP_CTRL, buf, 1);
  buf[0] = 0x00;  /* Clear bit 0 */
  DW1000_writeData(DW1000_OTP_IF, DW1000_SUB_OTP_CTRL, buf, 1);

  /* Read OPT memory */
  DW1000_readData(DW1000_OTP_IF, DW1000_SUB_OTP_RDAT, buf, 4);

  return (*readData);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : DW1000_SetClock
**功能 : Set Clock
**輸入 : clock
**輸出 : None
**使用 : DW1000_SetClock(ENABLE_ALL_SEQ);
**====================================================================================================*/
/*====================================================================================================*/
void DW1000_SetClock( uint8_t clock )
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
**函數 : DW1000_SetCrystalTrim
**功能 : Set Crystal Trim
**輸入 : xtalt (0x00 to 0x1F, ~1.5ppm/LSB)
**輸出 : None
**使用 : DW1000_SetCrystalTrim(xtalt);
**====================================================================================================*/
/*====================================================================================================*/
void DW1000_SetCrystalTrim( uint8_t xtalt )
{
  uint8_t reg;

  DW1000_readData(DW1000_FS_CTRL, DW1000_SUB_FS_XTALT, &reg, 1);

  reg &= 0xE0;
  reg |= (xtalt & 0x1F);

  DW1000_writeData(DW1000_FS_CTRL, DW1000_SUB_FS_XTALT, &reg, 1);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : DW1000_GetDeviceID
**功能 : Get Device ID
**輸入 : None
**輸出 : deviceID
**使用 : deviceID = DW1000_GetDeviceID();
**====================================================================================================*/
/*====================================================================================================*/
uint32_t DW1000_GetDeviceID( void )
{
  return DW1000_readData32(DW1000_DEV_ID, 0);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : DW1000_SetSpeed
**功能 : Set SPI Speed
**輸入 : speedSel
**輸出 : None
**使用 : DW1000_SetSpeed(SPIx_SPEED_LOW);
**====================================================================================================*/
/*====================================================================================================*/
void DW1000_SetSpeed( uint8_t speedSel )
{
  SPI_InitTypeDef SPI_InitStruct;

  SPI_I2S_DeInit(SPIx);

  SPI_InitStruct.SPI_Mode              = SPI_Mode_Master;
  SPI_InitStruct.SPI_Direction         = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStruct.SPI_DataSize          = SPI_DataSize_8b;
  SPI_InitStruct.SPI_CPOL              = SPI_CPOL_Low;
  SPI_InitStruct.SPI_CPHA              = SPI_CPHA_1Edge;
  SPI_InitStruct.SPI_NSS               = SPI_NSS_Soft;
  SPI_InitStruct.SPI_BaudRatePrescaler = speedSel;
  SPI_InitStruct.SPI_FirstBit          = SPI_FirstBit_MSB;
  SPI_InitStruct.SPI_CRCPolynomial     = 7;
  SPI_Init(SPIx, &SPI_InitStruct);

  SPI_Cmd(SPIx, ENABLE); 
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

  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IN;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_DOWN;

  GPIO_InitStruct.GPIO_Pin   = SPIx_IRQ_PIN;
  GPIO_Init(SPIx_IRQ_GPIO_PORT, &GPIO_InitStruct);

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
typedef struct {
    uint32_t      deviceID ;
    uint32_t      partID ;
    uint32_t      lotID ;
    uint8_t       chan;               // Added channel here - used in the reading of accumulator
    uint8_t       longFrames ;        // Flag in non-standard long frame mode
    uint8_t       otprev ;            // OTP revision number (read during initialisation)
    uint32_t      txFCTRL ;           // Keep TX_FCTRL register config
    uint8_t       xtrim;              // XTAL trim value read from OTP
    uint8_t       dblbuffon;          // Double RX buffer mode flag
    uint32_t      sysCFGreg ;         // Local copy of system config register
    uint16_t      sleep_mode;         // Used for automatic reloading of LDO tune and microcode at wake-up

    dwt_callback_data_t cdata;      // Callback data structure

    uint8_t       wait4resp ;         // wait4response was set with last TX start command
    int         prfIndex ;

    void (*dwt_txcallback)(const dwt_callback_data_t *txd);
    void (*dwt_rxcallback)(const dwt_callback_data_t *rxd);

} dwt_local_data_t;
static dwt_local_data_t KDW;

int8_t DW1000_Init( uint16_t config )
{
  uint8_t reg[4] = {0};
  uint8_t tmp8 = 0;
  uint32_t tmp32 = 0;

//  DW1000_Reset();

  /* check device */
  KDW.deviceID = DW1000_GetDeviceID();
  if(KDW.deviceID != DW1000_DEVICE_ID)
    return ERROR;

  /* system clock - 19.2 MHz XTI */
  DW1000_SetClock(FORCE_SYS_XTI);

  /* PLLLDT = 1, PLL lock detect flags */
  tmp8 = 0x04;
  DW1000_writeData(DW1000_EXT_SYNC, DW1000_SUB_EC_CTRL, &tmp8, 1);

  /* load LDO tune from OTP */
  tmp32 = DW1000_ReadOPT(LDOTUNE_ADDR);
  if((tmp32 & 0xFF) != 0) {
    tmp8 = 0x02;  /* LDO_KICK = 1 */
    DW1000_writeData(DW1000_OTP_IF, DW1000_SUB_OTP_SF, &tmp8, 1);
    KDW.sleep_mode |= AON_WCFG_ONW_LLDO;
  }

  /* read device info */
  tmp32 = DW1000_ReadOPT(XTRIM_ADDR) & 0xFFFF;
  KDW.xtrim  = tmp32 & 0x1F;
  KDW.otprev = (tmp32 >> 8) & 0xFF;
  KDW.partID = DW1000_ReadOPT(PARTID_ADDR);
  KDW.lotID  = DW1000_ReadOPT(LOTID_ADDR);

  /* configure XTAL trim */
  if(KDW.xtrim == 0x00) {
    /* middle range */
    KDW.xtrim = 0x10;
  }
  DW1000_SetCrystalTrim(KDW.xtrim);

////printf("KDW.deviceID = %X\r\n", KDW.deviceID);
////printf("KDW.otpRev   = %X\r\n", KDW.otpRev);
////printf("KDW.partID   = %X\r\n", KDW.partID);
////printf("KDW.lotID    = %X\r\n", KDW.lotID);
////printf("KDW.xtrim    = %X\r\n", KDW.xtrim);

  /* Load leading edge(LDE) detect code
   * see dw1000_user_manual - 2.5.4 Default Receiver Configuration */
  if(config == DWT_LOADUCODE) {
    /* load microcode */
    reg[0] = 0x01;  /* SYSCLKS = 1 */
    reg[1] = 0x03;  /* TXCLKS = 11 */
    DW1000_writeData(DW1000_PMSC, DW1000_SUB_PMSC_CTRL0, reg, 2);
    reg[0] = 0x00;
    reg[1] = 0x80;  /* LDELOAD = 1 */
    DW1000_writeData(DW1000_OTP_IF, DW1000_SUB_OTP_CTRL, reg, 2);

    /* wait upload */
    delay_ms(1);

    DW1000_SetClock(ENABLE_ALL_SEQ);
    KDW.sleep_mode |= AON_WCFG_ONW_LLDE;
  }
  else {
    DW1000_readData(DW1000_PMSC, DW1000_SUB_PMSC_CTRL1 + 1, reg, 2);
    reg[1] &= 0xFD; // LDERUN = 0
    DW1000_writeData(DW1000_PMSC, DW1000_SUB_PMSC_CTRL1 + 1, reg, 2);
  }

  /* Enable clocks for sequencing */
  DW1000_SetClock(ENABLE_ALL_SEQ);
  delay_ms(5);

  /* Read sysconfig register */
  KDW.sysCFGreg = DW1000_readData32(DW1000_SYS_CFG, 0);

  DW1000_SetSpeed(SPIx_SPEED_HIGH);

  return SUCCESS;
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : DW1000_SetAntennaDelayRX
**功能 : Set RX Antenna Delay
**輸入 : delay
**輸出 : None
**使用 : DW1000_SetAntennaDelayRX(delay);
**====================================================================================================*/
/*====================================================================================================*/
void DW1000_SetAntennaDelayRX( uint16_t delay )
{
  /* Set the RX antenna delay for auto TX timestamp adjustment */
  DW1000_writeData16(DW1000_LDE_CTRL, DW1000_SUB_LDE_RXANTD, delay);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : DW1000_SetAntennaDelayTX
**功能 : Set RX Antenna Delay
**輸入 : delay
**輸出 : None
**使用 : DW1000_SetAntennaDelayRX(delay);
**====================================================================================================*/
/*====================================================================================================*/
void DW1000_SetAntennaDelayTX( uint16_t delay )
{
  /* Set the TX antenna delay for auto TX timestamp adjustment */
  DW1000_writeData16(DW1000_TX_ANTD, 0x0, delay);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : DW1000_SetAntennaDelayTX
**功能 : Set RX Antenna Delay
**輸入 : delay
**輸出 : None
**使用 : DW1000_SetAntennaDelayRX(delay);
**====================================================================================================*/
/*====================================================================================================*/
//void dwt_setrxaftertxdelay(uint32_t rxDelayTime)
//{
//    uint32_t val = DW1000_readData32(ACK_RESP_T_ID, 0) ; // Read ACK_RESP_T_ID register

//    val &= ~(ACK_RESP_T_W4R_TIM_MASK) ; // Clear the timer (19:0)

//    val |= (rxDelayTime & ACK_RESP_T_W4R_TIM_MASK) ; // In UWB microseconds (e.g. turn the receiver on 20uus after TX)

//    DW1000_writeData32(ACK_RESP_T_ID, 0, val) ;
//}
//void dwt_setrxtimeout(uint16_t time)
//{
//    uint8_t temp ;

//    DW1000_readData(SYS_CFG_ID,3,&temp,1) ; // Read register

//    if(time > 0)
//    {
//        DW1000_writeData16(DW1000_RX_FWTO, 0x0, time) ;

//        temp |= (uint8_t)(SYS_CFG_RXWTOE>>24);
//        // OR in 32bit value (1 bit set), I know this is in high byte.
//        KDW.sysCFGreg |= SYS_CFG_RXWTOE;

//        DW1000_writeData(SYS_CFG_ID,3,&temp,1) ;
//    }
//    else
//    {
//        temp &= ~((uint8_t)(SYS_CFG_RXWTOE>>24));
//        // AND in inverted 32bit value (1 bit clear), I know this is in high byte.
//        KDW.sysCFGreg &= ~(SYS_CFG_RXWTOE);

//        DW1000_writeData(SYS_CFG_ID,3,&temp,1) ;

//        //DW1000_writeData16(RX_FWTO_ID,0,0) ; // Clearing the time is not needed
//    }

//}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : 
**功能 : 
**輸入 : 
**輸出 : 
**使用 : 
**====================================================================================================*/
/*====================================================================================================*/
//void DW1000_EnableRX( int32_t delayed )
//{
//  uint8_t tmp8 = 0;
//  uint16_t tmp16 = 0;

//  dwt_syncrxbufptrs();

//  tmp16 = (uint16_t)0x00000100UL;

//  if(delayed)
//    tmp16 |= (uint16_t)0x00000200UL;

////  dwt_write16bitoffsetreg(SYS_CTRL_ID,0,tmp16);
//  DW1000_writeData(DW1000_SYS_CTRL, 0, (uint8_t *)&tmp16, 2);

//  if(delayed) { // Check for errors

//    //uint32 status1 = dwt_read32bitreg(SYS_STATUS_ID) ; // Read status register

////    dwt_readfromdevice(SYS_STATUS_ID,3,1,&tmp8);
//    DW1000_readData(DW1000_SYS_STATUS, 3, &tmp8, 1);

//    if(tmp8 & (0x08000000UL >> 24)) { // If delay has not passed do delayed else immediate RX on
//      dwt_forcetrxoff(); // Turn the delayed receive off, and do immediate receive, return warning indication
//      tmp16 = (uint16_t)0x00000100UL; // Clear the delay bit
////      dwt_write16bitoffsetreg(SYS_CTRL_ID,0,tmp16) ;
//      DW1000_writeData(DW1000_SYS_CTRL, 0, (uint8_t *)&tmp16, 2);
////      return DWT_ERROR;
//    }
//  }
//}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : 
**功能 : 
**輸入 : 
**輸出 : 
**使用 : 
**====================================================================================================*/
/*====================================================================================================*/
//int8_t DW1000_RXData( uint8_t *readBuf, uint32_t *lens )
//{
//  int8_t status = ERROR;
//  uint32_t frame_len = 0;
//  uint32_t status_reg = 0;

//  while(!((status_reg = dwt_read32bitreg(0x0F)) & 0x2427D000UL));

//  if(status_reg & 0x00004000UL) {
//    /* A frame has been received, copy it to our local buffer. */
//    frame_len = dwt_read32bitreg(0x10) & RX_FINFO_RXFL_MASK_1023;
//    if(frame_len <= *lens) {
//      *lens = frame_len;
//      dwt_readfromdevice(RX_BUFFER_ID, 0, *lens, readBuf);
//      status = SUCCESS;
//    }
//    else {
//      status = ERROR;
//    }

//    /* Clear good RX frame event in the DW1000 status register. */
//    dwt_write32bitreg(0x0F, SYS_STATUS_RXFCG);
//  }
//  else {
//    /* Clear RX error events in the DW1000 status register. */
//    dwt_write32bitreg(0x0F, SYS_STATUS_ALL_RX_ERR);
//  }

//  return status;
//}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : 
**功能 : 
**輸入 : 
**輸出 : 
**使用 : 
**====================================================================================================*/
/*====================================================================================================*/
//void DW1000_TXData( uint8_t *sendBuf, uint32_t lens )
//{
//  /* Write frame data to DW1000 and prepare transmission. See NOTE 3 below.*/
//  dwt_writetxdata(lens, sendBuf, 0);
//  dwt_writetxfctrl(lens, 0);

//  /* Start transmission. */
//  dwt_starttx(DWT_START_TX_IMMEDIATE);

//  /* Poll DW1000 until TX frame sent event set. See NOTE 4 below.
//   * STATUS register is 5 bytes long but, as the event we are looking at is in the first byte of the register, we can use this simplest API
//   * function to access it.*/
//  while(!(dwt_read32bitreg(SYS_STATUS_ID) & SYS_STATUS_TXFRS)) { };

//  /* Clear TX frame sent event. */
//  dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_TXFRS);
//}
/*==============================================================================================*/
/*==============================================================================================*/
