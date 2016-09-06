/* #include "modules\dw1000.h" */

#ifndef __DW1000_H
#define __DW1000_H

#include "stm32f4xx.h"

#include "deca_device.h"
#include "deca_params.h"
/*=====================================================================================================*/
/*=====================================================================================================*/
/* device ID */
#define DW1000_DEVICE_ID        ((uint32_t)0xDECA0130)

/* OPT memory address */
#define LDOTUNE_ADDR            ((uint16_t)0x0004)
#define PARTID_ADDR             ((uint16_t)0x0006)
#define LOTID_ADDR              ((uint16_t)0x0007)
#define VBAT_ADDR               ((uint16_t)0x0008)
#define VTEMP_ADDR              ((uint16_t)0x0009)
#define XTRIM_ADDR              ((uint16_t)0x001E)

/* dw1000 reg */
#define DW1000_SUB_NONE         ((uint8_t)0x00)

#define DW1000_DEV_ID           ((uint8_t)0x00)     // Lens = 4,    RO,  Device Identifier – includes device type and revision info
#define DW1000_EUI              ((uint8_t)0x01)     // Lens = 8,    RW,  Extended Unique Identifier
#define DW1000_PANADR           ((uint8_t)0x03)     // Lens = 4,    RW,  PAN Identifier and Short Address
#define DW1000_SYS_CFG          ((uint8_t)0x04)     // Lens = 4,    RW,  System Configuration bitmap
#define DW1000_SYS_TIME         ((uint8_t)0x06)     // Lens = 5,    RO,  System Time Counter (40-bit)
#define DW1000_TX_FCTRL         ((uint8_t)0x08)     // Lens = 5,    RW,  Transmit Frame Control
#define DW1000_TX_BUFFER        ((uint8_t)0x09)     // Lens = 1024, WO,  Transmit Data Buffer
#define DW1000_DX_TIME          ((uint8_t)0x0A)     // Lens = 5,    RW,  Delayed Send or Receive Time (40-bit)
#define DW1000_RX_FWTO          ((uint8_t)0x0C)     // Lens = 2,    RW,  Receive Frame Wait Timeout Period
#define DW1000_SYS_CTRL         ((uint8_t)0x0D)     // Lens = 4,    SRW, System Control Register
#define DW1000_SYS_MASK         ((uint8_t)0x0E)     // Lens = 4,    RW,  System Event Mask Register
#define DW1000_SYS_STATUS       ((uint8_t)0x0F)     // Lens = 5,    SRW, System Event Status Register
#define DW1000_RX_FINFO         ((uint8_t)0x10)     // Lens = 4,    ROD, RX Frame Information (in double buffer set)
#define DW1000_RX_BUFFER        ((uint8_t)0x11)     // Lens = 1024, ROD, Receive Data (in double buffer set)
#define DW1000_RX_FQUAL         ((uint8_t)0x12)     // Lens = 8,    ROD, Rx Frame Quality information (in double buffer set)
#define DW1000_RX_TTCKI         ((uint8_t)0x13)     // Lens = 4,    ROD, Receiver Time Tracking Interval (in double buffer set)
#define DW1000_RX_TTCKO         ((uint8_t)0x14)     // Lens = 5,    ROD, Receiver Time Tracking Offset (in double buffer set)
#define DW1000_RX_TIME          ((uint8_t)0x15)     // Lens = 14,   ROD, Receive Message Time of Arrival (in double buffer set)
#define DW1000_TX_TIME          ((uint8_t)0x17)     // Lens = 10,   RO,  Transmit Message Time of Sending
#define DW1000_TX_ANTD          ((uint8_t)0x18)     // Lens = 2,    RW,  16-bit Delay from Transmit to Antenna
#define DW1000_SYS_STATE        ((uint8_t)0x19)     // Lens = 5,    RO,  System State information
#define DW1000_ACK_RESP_T       ((uint8_t)0x1A)     // Lens = 4,    RW,  Acknowledgement Time and Response Time
#define DW1000_RX_SNIFF         ((uint8_t)0x1D)     // Lens = 4,    RW,  Pulsed Preamble Reception Configuration
#define DW1000_TX_POWER         ((uint8_t)0x1E)     // Lens = 4,    RW,  TX Power Control
#define DW1000_CHAN_CTRL        ((uint8_t)0x1F)     // Lens = 4,    RW,  Channel Control
#define DW1000_USR_SFD          ((uint8_t)0x21)     // Lens = 41,   RW,  User-specified short/long TX/RX SFD sequences

#define DW1000_AGC_CTRL         ((uint8_t)0x23)     // Lens = 32,   RW,  Automatic Gain Control configuration
#define DW1000_SUB_AGC_CTRL1    ((uint8_t)0x02)     // Lens = 2,    RW,  AGC Control #1
#define DW1000_SUB_AGC_TUNE1    ((uint8_t)0x04)     // Lens = 2,    RW,  AGC Tuning register 1
#define DW1000_SUB_AGC_TUNE2    ((uint8_t)0x0C)     // Lens = 4,    RW,  AGC Tuning register 2
#define DW1000_SUB_AGC_TUNE3    ((uint8_t)0x12)     // Lens = 2,    RW,  AGC Tuning register 3
#define DW1000_SUB_AGC_STAT1    ((uint8_t)0x1E)     // Lens = 3,    RO,  AGC Status

#define DW1000_EXT_SYNC         ((uint8_t)0x24)     // Lens = 12,   RW,  External synchronisation control
#define DW1000_SUB_EC_CTRL      ((uint8_t)0x00)     // Lens = 4,    RW,  External clock synchronisation counter configuration
#define DW1000_SUB_EC_RXTC      ((uint8_t)0x04)     // Lens = 4,    RO,  External clock counter captured on RMARKER
#define DW1000_SUB_EC_GOLP      ((uint8_t)0x08)     // Lens = 4,    RO,  External clock offset to first path 1 GHz counter

#define DW1000_ACC_MEM          ((uint8_t)0x25)     // Lens = 4064, RO,  Read access to accumulator data

#define DW1000_GPIO_CTRL        ((uint8_t)0x26)     // Lens = 44,   RW,  Peripheral register bus 1 access - GPIO control
#define DW1000_SUB_GPIO_MODE    ((uint8_t)0x00)     // Lens = 4,    RW,  GPIO Mode Control Register
#define DW1000_SUB_GPIO_DIR     ((uint8_t)0x08)     // Lens = 4,    RW,  GPIO Direction Control Register
#define DW1000_SUB_GPIO_DOUT    ((uint8_t)0x0C)     // Lens = 4,    RW,  GPIO Data Output register
#define DW1000_SUB_GPIO_IRQE    ((uint8_t)0x10)     // Lens = 4,    RW,  GPIO Interrupt Enable
#define DW1000_SUB_GPIO_ISEN    ((uint8_t)0x14)     // Lens = 4,    RW,  GPIO Interrupt Sense Selection
#define DW1000_SUB_GPIO_IMODE   ((uint8_t)0x18)     // Lens = 4,    RW,  GPIO Interrupt Mode (Level / Edge)
#define DW1000_SUB_GPIO_IBES    ((uint8_t)0x1C)     // Lens = 4,    RW,  GPIO Interrupt “Both Edge” Select
#define DW1000_SUB_GPIO_ICLR    ((uint8_t)0x20)     // Lens = 4,    RW,  GPIO Interrupt Latch Clear
#define DW1000_SUB_GPIO_IDBE    ((uint8_t)0x24)     // Lens = 4,    RW,  GPIO Interrupt De-bounce Enable
#define DW1000_SUB_GPIO_RAW     ((uint8_t)0x28)     // Lens = 4,    RO,  GPIO raw state

#define DW1000_DRX_CONF         ((uint8_t)0x27)     // Lens = 44,   RW,  Digital Receiver configuration
#define DW1000_SUB_DRX_TUNE0b   ((uint8_t)0x02)     // Lens = 2,    RW,  Digital Tuning Register 0b
#define DW1000_SUB_DRX_TUNE1a   ((uint8_t)0x04)     // Lens = 2,    RW,  Digital Tuning Register 1a
#define DW1000_SUB_DRX_TUNE1b   ((uint8_t)0x06)     // Lens = 2,    RW,  Digital Tuning Register 1b
#define DW1000_SUB_DRX_TUNE2    ((uint8_t)0x08)     // Lens = 4,    RW,  Digital Tuning Register 2
#define DW1000_SUB_DRX_SFDTOC   ((uint8_t)0x20)     // Lens = 2,    RW,  SFD timeout
#define DW1000_SUB_DRX_PRETOC   ((uint8_t)0x24)     // Lens = 2,    RW,  Preamble detection timeout
#define DW1000_SUB_DRX_TUNE4H   ((uint8_t)0x26)     // Lens = 2,    RW,  Digital Tuning Register 4H
#define DW1000_SUB_RXPACC_NOSAT ((uint8_t)0x2C)     // Lens = 2,    RO,  Unsaturated accumulated preamble symbols

#define DW1000_RF_CONF          ((uint8_t)0x28)     // Lens = 58,   RW,  Analog RF Configuration
#define DW1000_SUB_RF_CONF      ((uint8_t)0x00)     // Lens = 4,    RW,  RF Configuration Register
#define DW1000_SUB_RF_RXCTRLH   ((uint8_t)0x0B)     // Lens = 1,    RW,  Analog RX Control Register
#define DW1000_SUB_RF_TXCTRL    ((uint8_t)0x0C)     // Lens = 4,    RW,  Analog TX Control Register
#define DW1000_SUB_RF_STATUS    ((uint8_t)0x2C)     // Lens = 4,    RO,  RF Status Register
#define DW1000_SUB_LDOTUNE      ((uint8_t)0x30)     // Lens = 5,    RW,  LDO voltage tuning

#define DW1000_TX_CAL           ((uint8_t)0x2A)     // Lens = 52,   RW,  Transmitter calibration block
#define DW1000_SUB_TC_SARC      ((uint8_t)0x00)     // Lens = 2,    RW,  Transmitter Calibration – SAR control
#define DW1000_SUB_TC_SARL      ((uint8_t)0x03)     // Lens = 3,    RO,  Transmitter Calibration – Latest SAR readings
#define DW1000_SUB_TC_SARW      ((uint8_t)0x06)     // Lens = 2,    RO,  Transmitter Calibration – SAR readings at last Wake-Up
#define DW1000_SUB_TC_PGDELAY   ((uint8_t)0x0B)     // Lens = 1,    RW,  Transmitter Calibration – Pulse Generator Delay
#define DW1000_SUB_TC_PGTEST    ((uint8_t)0x0C)     // Lens = 1,    RW,  Transmitter Calibration – Pulse Generator Test

#define DW1000_FS_CTRL          ((uint8_t)0x2B)     // Lens = 21,   RW,  Frequency synthesiser control block
#define DW1000_SUB_FS_PLLCFG    ((uint8_t)0x07)     // Lens = 4,    RW,  Frequency synthesiser – PLL configuration
#define DW1000_SUB_FS_PLLTUNE   ((uint8_t)0x0B)     // Lens = 1,    RW,  Frequency synthesiser – PLL Tuning
#define DW1000_SUB_FS_XTALT     ((uint8_t)0x0E)     // Lens = 1,    RW,  Frequency synthesiser – Crystal trim

#define DW1000_AON              ((uint8_t)0x2C)     // Lens = 12,   RW,  Always-On register set
#define DW1000_SUB_AON_WCFG     ((uint8_t)0x00)     // Lens = 2,    RW,  AON Wakeup Configuration Register
#define DW1000_SUB_AON_CTRL     ((uint8_t)0x02)     // Lens = 1,    RW,  AON Control Register
#define DW1000_SUB_AON_RDAT     ((uint8_t)0x03)     // Lens = 1,    RW,  AON Direct Access Read Data Result
#define DW1000_SUB_AON_ADDR     ((uint8_t)0x04)     // Lens = 1,    RW,  AON Direct Access Address
#define DW1000_SUB_AON_CFG0     ((uint8_t)0x06)     // Lens = 4,    RW,  AON Configuration Register 0
#define DW1000_SUB_AON_CFG1     ((uint8_t)0x0A)     // Lens = 2,    RW,  AON Configuration Register 1

#define DW1000_OTP_IF           ((uint8_t)0x2D)     // Lens = 18,   RW,  One Time Programmable Memory Interface
#define DW1000_SUB_OTP_WDAT     ((uint8_t)0x00)     // Lens = 4,    RW,  OTP Write Data
#define DW1000_SUB_OTP_ADDR     ((uint8_t)0x04)     // Lens = 2,    RW,  OTP Address
#define DW1000_SUB_OTP_CTRL     ((uint8_t)0x06)     // Lens = 2,    RW,  OTP Control
#define DW1000_SUB_OTP_STAT     ((uint8_t)0x08)     // Lens = 2,    RW,  OTP Status
#define DW1000_SUB_OTP_RDAT     ((uint8_t)0x0A)     // Lens = 4,    R,   OTP Read Data
#define DW1000_SUB_OTP_SRDAT    ((uint8_t)0x0E)     // Lens = 4,    RW,  OTP SR Read Data
#define DW1000_SUB_OTP_SF       ((uint8_t)0x12)     // Lens = 1,    RW,  OTP Special Function

#define DW1000_LDE_CTRL         ((uint8_t)0x2E)     // Lens =  -    RW,  Leading edge detection control block
#define DW1000_SUB_LDE_THRESH   ((uint16_t)0x0000)  // Lens = 2,    RO,  LDE Threshold report
#define DW1000_SUB_LDE_CFG1     ((uint16_t)0x0806)  // Lens = 1,    RW,  LDE Configuration Register 1
#define DW1000_SUB_LDE_PPINDX   ((uint16_t)0x1000)  // Lens = 2,    RO,  LDE Peak Path Index
#define DW1000_SUB_LDE_PPAMPL   ((uint16_t)0x1002)  // Lens = 2,    RO,  LDE Peak Path Amplitude
#define DW1000_SUB_LDE_RXANTD   ((uint16_t)0x1804)  // Lens = 2,    RW,  LDE Receive Antenna Delay configuration
#define DW1000_SUB_LDE_CFG2     ((uint16_t)0x1806)  // Lens = 2,    RW,  LDE Configuration Register 2
#define DW1000_SUB_LDE_REPC     ((uint16_t)0x2804)  // Lens = 2,    RW,  LDE Replica Coefficient configuration

#define DW1000_DIG_DIAG         ((uint8_t)0x2F)     // Lens = 41,   RW,  Digital Diagnostics Interface
#define DW1000_SUB_EVC_CTRL     ((uint8_t)0x00)     // Lens = 4,    SRW, Event Counter Control
#define DW1000_SUB_EVC_PHE      ((uint8_t)0x04)     // Lens = 2,    RO,  PHR Error Counter
#define DW1000_SUB_EVC_RSE      ((uint8_t)0x06)     // Lens = 2,    RO,  RSD Error Counter
#define DW1000_SUB_EVC_FCG      ((uint8_t)0x08)     // Lens = 2,    RO,  Frame Check Sequence Good Counter
#define DW1000_SUB_EVC_FCE      ((uint8_t)0x0A)     // Lens = 2,    RO,  Frame Check Sequence Error Counter
#define DW1000_SUB_EVC_FFR      ((uint8_t)0x0C)     // Lens = 2,    RO,  Frame Filter Rejection Counter
#define DW1000_SUB_EVC_OVR      ((uint8_t)0x0E)     // Lens = 2,    RO,  RX Overrun Error Counter
#define DW1000_SUB_EVC_STO      ((uint8_t)0x10)     // Lens = 2,    RO,  SFD Timeout Counter
#define DW1000_SUB_EVC_PTO      ((uint8_t)0x12)     // Lens = 2,    RO,  Preamble Timeout Counter
#define DW1000_SUB_EVC_FWTO     ((uint8_t)0x14)     // Lens = 2,    RO,  RX Frame Wait Timeout Counter
#define DW1000_SUB_EVC_TXFS     ((uint8_t)0x16)     // Lens = 2,    RO,  TX Frame Sent Counter
#define DW1000_SUB_EVC_HPW      ((uint8_t)0x18)     // Lens = 2,    RO,  Half Period Warning Counter
#define DW1000_SUB_EVC_TPW      ((uint8_t)0x1A)     // Lens = 2,    RO,  Transmitter Power-Up Warning Counter
#define DW1000_SUB_DIAG_TMC     ((uint8_t)0x24)     // Lens = 2,    RW,  Test Mode Control Register

#define DW1000_PMSC             ((uint8_t)0x36)     // Lens = 48,   RW,  Power Management System Control Block
#define DW1000_SUB_PMSC_CTRL0   ((uint8_t)0x00)     // Lens = 4,    RW,  PMSC Control Register 0
#define DW1000_SUB_PMSC_CTRL1   ((uint8_t)0x04)     // Lens = 4,    RW,  PMSC Control Register 1
#define DW1000_SUB_PMSC_SNOZT   ((uint8_t)0x0C)     // Lens = 1,    RW,  PMSC Snooze Time Register
#define DW1000_SUB_PMSC_TXFSEQ  ((uint8_t)0x26)     // Lens = 2,    RW,  PMSC fine grain TX sequencing control
#define DW1000_SUB_PMSC_LEDC    ((uint8_t)0x28)     // Lens = 4,    RW,  PMSC LED Control Register

/* clock config */
#define ENABLE_ALL_SEQ          ((uint8_t)0x00)
#define FORCE_SYS_XTI           ((uint8_t)0x01)
#define FORCE_SYS_PLL           ((uint8_t)0x02)
#define FORCE_TX_PLL            ((uint8_t)0x03)
#define FORCE_OTP_ON            ((uint8_t)0x04)
#define FORCE_OTP_OFF           ((uint8_t)0x05)
#define READ_ACC_ON             ((uint8_t)0x06)
#define READ_ACC_OFF            ((uint8_t)0x07)
/*=====================================================================================================*/
/*=====================================================================================================*/
typedef enum {
  Channel_1 = 0x01,
  Channel_2 = 0x02,
  Channel_3 = 0x03,
  Channel_4 = 0x04,
  Channel_5 = 0x05,
  Channel_7 = 0x07
} DW_Channel_TypeDef;

typedef enum {
  DataRate_110K = 0x00,   // UWB bit rate 110 kbits/s
  DataRate_850K = 0x01,   // UWB bit rate 850 kbits/s
  DataRate_6M8  = 0x02    // UWB bit rate 6.8 Mbits/s
} DW_DataRate_TypeDef;

typedef enum {
  PulseRepFreq_16M = 0x01,  // UWB PRF 16 MHz
  PulseRepFreq_64M = 0x02,  // UWB PRF 64 MHz
} DW_PulseRepFreq_TypeDef;

typedef enum {
  TxPreambLen_64   = 0x04,  // Standard preamble length 64 symbols
  TxPreambLen_128  = 0x14,  // Non-standard preamble length 128 symbols
  TxPreambLen_256  = 0x24,  // Non-standard preamble length 256 symbols
  TxPreambLen_512  = 0x34,  // Non-standard preamble length 512 symbols
  TxPreambLen_1024 = 0x08,  // Standard preamble length 1024 symbols
  TxPreambLen_1536 = 0x18,  // Non-standard preamble length 1536 symbols
  TxPreambLen_2048 = 0x28,  // Non-standard preamble length 2048 symbols
  TxPreambLen_4096 = 0x0C,  // Standard preamble length 4096 symbols
} DW_TxPreambLen_TypeDef;

typedef enum {
  PhrMode_StandardFrame = 0x00,   // standard frame mode
  PhrMode_LongFrame     = 0x03,   // long frames mode
} DW_PhrMode_TypeDef;

typedef struct {
  uint8_t loadCode;
  uint8_t DW_rxCode;                        // RX preamble code

  DW_Channel_TypeDef      DW_channel;       // channel number 1, 2, 3, 4, 5, 7
  DW_DataRate_TypeDef     DW_dataRate;      // data rate
  DW_PulseRepFreq_TypeDef DW_pulseRepFreq;  // pulse repetition frequency
  DW_TxPreambLen_TypeDef  DW_txPreambLen;   // TX preamble length
  DW_PhrMode_TypeDef      DW_phrMode;       // PHR mode, see dw1000_user_manual - 10.4 PHY header
} DW_ConfigTypeDef;

typedef struct {
  // device info
  uint32_t deviceID;  // Device ID = 0xDECA0130
  uint32_t partID;    // Chip ID
  uint32_t lotID;     // Lot ID
  uint8_t otpRev;     // OTP Revision
  uint8_t xtrim;      // XTAL trim value read from OTP

uint32_t sysCFGreg;     //  ...

  DW_ConfigTypeDef config;  // Added channel here - used in the reading of accumulator

} DW_DeviceTypeDef;
/*=====================================================================================================*/
/*=====================================================================================================*/
void     DW1000_readData( uint16_t regAddr, uint16_t subIndex, uint8_t *readBuf, uint32_t lens );
void     DW1000_writeData( uint16_t regAddr, uint16_t subIndex, const uint8_t *writeBuf, uint32_t lens );
uint16_t DW1000_readData16( int32_t regAddr, uint16_t subIndex );
void     DW1000_writeData16( int32_t regAddr, uint16_t subIndex, uint16_t writeData );
uint32_t DW1000_readData32( int32_t regAddr, uint16_t subIndex );
void     DW1000_writeData32(int32_t regAddr, uint16_t subIndex, uint32_t writeData);

uint32_t DW1000_ReadOPT( uint16_t memAddr );
void     DW1000_SetClock( uint8_t clock );
void     DW1000_SetCrystalTrim( uint8_t xtalt );
uint32_t DW1000_GetDeviceID( void );

void     DW1000_SetSpeed( uint8_t speedSel );
void     DW1000_Config( void );
int8_t   DW1000_Init( uint16_t config );

void     DW1000_SetAntennaDelayRX( uint16_t delay );
void     DW1000_SetAntennaDelayTX( uint16_t delay );

void    DW1000_EnableRX( int32_t delayed );
int8_t  DW1000_RXData( uint8_t *readBuf, uint32_t *lens );
void    DW1000_TXData( uint8_t *sendBuf, uint32_t lens );

void DW1000_Reset( void );

/*=====================================================================================================*/
/*=====================================================================================================*/
#endif
