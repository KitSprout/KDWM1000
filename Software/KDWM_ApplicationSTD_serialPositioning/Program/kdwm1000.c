/*====================================================================================================*/
/*====================================================================================================*/
#include "drivers\stm32f4_system.h"
#include "modules\serial.h"
#include "modules\dw1000.h"

#include "kdwm1000.h"

#include <string.h>
/*====================================================================================================*/
/*====================================================================================================*/

/********************************************************************
 *                                                                  *
 *                             SSTWR                                *
 *                                                                  *
 ********************************************************************/
#define ENABLE_SSTWR_INITIATOR
//#define ENABLE_SSTWR_RESPONDER
#define DESTINATION_ADDR  10
#define MAX_ANCHOR        4
/********************************************************************/

static dwt_config_t config = {
  2,               /* Channel number. */
  DWT_PRF_64M,     /* Pulse repetition frequency. */
  DWT_PLEN_128,    /* Preamble length. */
  DWT_PAC8,        /* Preamble acquisition chunk size. Used in RX only. */
  9,               /* TX preamble code. Used in TX only. */
  9,               /* RX preamble code. Used in RX only. */
  0,               /* Use non-standard SFD (Boolean) */
  DWT_BR_6M8,      /* Data rate. */
  DWT_PHRMODE_STD, /* PHY header mode. */
  (129 + 8 - 8)    /* SFD timeout (preamble length + 1 + SFD length - PAC size). Used in RX only. */
};

/* Default antenna delay values for 64 MHz PRF. See NOTE 2 below. */
#define TX_ANT_DLY 16436
#define RX_ANT_DLY 16436

/* Inter-ranging delay period, in milliseconds. */
#define RNG_DELAY_MS 200

void DEMO_SSTWR_INITIATOR( void );
void DEMO_SSTWR_RESPONDER( void );
/*====================================================================================================*/
/*====================================================================================================*/
void KDWM_Init( void )
{
  KDWM_GPIO_Config();
  KDWM_UART_Config(NULL);
  KDWM_DW1000_Config();

  DW1000_Reset();
  dwt_initialise(DWT_LOADUCODE);
  dwt_configure(&config);

  /* Apply default antenna delay value. See NOTE 2 below. */
  DW1000_SetAntennaDelayRX(RX_ANT_DLY);
  DW1000_SetAntennaDelayTX(TX_ANT_DLY);
}
/*====================================================================================================*/
/*====================================================================================================*/
void KDWM_Loop( void )
{
#if defined(ENABLE_SSTWR_INITIATOR) && defined(ENABLE_SSTWR_RESPONDER)
  #error "defined ENABLE_SSTWR_INITIATOR and ENABLE_SSTWR_RESPONDER"

#elif defined(ENABLE_SSTWR_INITIATOR)
  printf(" ---- INITIATOR ----\r\n");
  DEMO_SSTWR_INITIATOR();

#elif defined(ENABLE_SSTWR_RESPONDER)
  printf(" ---- RESPONDER ----\r\n");
  DEMO_SSTWR_RESPONDER();

#else
  #error "need to define ENABLE_SSTWR_INITIATOR or ENABLE_SSTWR_RESPONDER"

#endif
}
/*====================================================================================================*/
/*====================================================================================================*/
#define ALL_MSG_COMMON_LEN        20
#define RESP_MSG_POLL_RX_TS_IDX   20 // ALL_MSG_COMMON_LEN
#define RESP_MSG_RESP_TX_TS_IDX   24 // ALL_MSG_COMMON_LEN + 4
#define RESP_MSG_TS_LEN           4

#define UUS_TO_DWT_TIME 65536

#define INITIATOR_TX_BUF_LEN  22  // ALL_MSG_COMMON_LEN + 2
#define INITIATOR_RX_BUF_LEN  30  // ALL_MSG_COMMON_LEN + 10

static uint32_t status = 0;

int8_t checkPacket( uint8_t *pMsg1, uint8_t *pMsg2 )
{
  for(uint8_t i = 0; i < 8; i++) {
    if(pMsg1[i] != pMsg2[i]) {
      return -1;
    }
  }
  return 0;
}

#define SERIAL_LENS 22
void sendDistance( float32_t *pDistance )
{
  uint8_t sendBuf[SERIAL_LENS] = { 'S', '-', 'K', 'S', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '\r', '\n' };
  float32_t *pData = (float32_t*)&sendBuf[4];

  pData[0] = pDistance[0];
  pData[1] = pDistance[1];
  pData[2] = pDistance[2];
  pData[3] = pDistance[3];

  Serial_SendData(sendBuf, SERIAL_LENS);
}

#if defined(ENABLE_SSTWR_INITIATOR)
/********************************************************************
 *                                                                  *
 *                      ENABLE_SSTWR_INITIATOR                      *
 *                                                                  *
 ********************************************************************/

static uint8_t tx_poll_msg[INITIATOR_TX_BUF_LEN] = {0};
static uint8_t rx_resp_msg[INITIATOR_RX_BUF_LEN] = {0};
static uint8_t rx_buffer[INITIATOR_RX_BUF_LEN]   = {0};

static float64_t tof[4]        = {0};
static float64_t distance[4]   = {0};
static float32_t distance32[4] = {0};

#define POLL_TX_TO_RESP_RX_DLY_UUS  140       /* Delay between frames, in UWB microseconds. See NOTE 1 below. */
#define RESP_RX_TIMEOUT_UUS         210       /* Receive response timeout. See NOTE 5 below. */
#define SPEED_OF_LIGHT              299702547 /* Speed of light in air, in metres per second. */

static void resp_msg_get_ts( uint8_t *ts_field, uint32_t *ts )
{
  *ts = 0;
  for(uint32_t i = 0; i < RESP_MSG_TS_LEN; i++) {
    *ts += ts_field[i] << (i * 8);
  }
}

void DEMO_SSTWR_INITIATOR( void )
{
  tx_poll_msg[0] = 'K';
  tx_poll_msg[1] = 'S';
  tx_poll_msg[2] = 'D';
  tx_poll_msg[3] = 'W';
  tx_poll_msg[4] = 0; // source address
  tx_poll_msg[5] = 0; // source address
  tx_poll_msg[6] = DESTINATION_ADDR;
  tx_poll_msg[7] = 0;

  rx_resp_msg[0] = 'K';
  rx_resp_msg[1] = 'S';
  rx_resp_msg[2] = 'D';
  rx_resp_msg[3] = 'W';
  rx_resp_msg[4] = DESTINATION_ADDR;
  rx_resp_msg[5] = 0;
  rx_resp_msg[6] = 0;
  rx_resp_msg[7] = 0;

  dwt_setrxaftertxdelay(POLL_TX_TO_RESP_RX_DLY_UUS);
  dwt_setrxtimeout(RESP_RX_TIMEOUT_UUS);

  while(1) {
    for(uint8_t anchor = 0; anchor < MAX_ANCHOR; anchor++) {
      rx_resp_msg[4] = DESTINATION_ADDR + anchor;
      tx_poll_msg[6] = DESTINATION_ADDR + anchor;
      DW1000_writeData32(SYS_STATUS_ID, 0, SYS_STATUS_TXFRS);
      dwt_writetxdata(sizeof(tx_poll_msg), tx_poll_msg, 0);
      dwt_writetxfctrl(sizeof(tx_poll_msg), 0);
      dwt_starttx(DWT_START_TX_IMMEDIATE | DWT_RESPONSE_EXPECTED);

      do {
        status = DW1000_readData32(SYS_STATUS_ID, 0);
      } while(!(status & (SYS_STATUS_RXFCG | SYS_STATUS_ALL_RX_ERR)));

      if(status & SYS_STATUS_RXFCG) {
        uint32_t frame_len;
        DW1000_writeData32(SYS_STATUS_ID, 0, SYS_STATUS_RXFCG);
        frame_len = DW1000_readData32(RX_FINFO_ID, 0) & RX_FINFO_RXFLEN_MASK;
        if(frame_len <= INITIATOR_RX_BUF_LEN) {
          dwt_readrxdata(rx_buffer, frame_len, 0);
        }
        if(checkPacket(rx_buffer, rx_resp_msg) == 0) {
          uint32_t poll_tx_ts, resp_rx_ts, poll_rx_ts, resp_tx_ts;
          int32_t rtd_init, rtd_resp;

          poll_tx_ts = dwt_readtxtimestamplo32();
          resp_rx_ts = dwt_readrxtimestamplo32();

          resp_msg_get_ts(&rx_buffer[RESP_MSG_POLL_RX_TS_IDX], &poll_rx_ts);
          resp_msg_get_ts(&rx_buffer[RESP_MSG_RESP_TX_TS_IDX], &resp_tx_ts);

          rtd_init = resp_rx_ts - poll_tx_ts;
          rtd_resp = resp_tx_ts - poll_rx_ts;

          tof[anchor] = ((rtd_init - rtd_resp) / 2.0) * DWT_TIME_UNITS;
          distance[anchor] = tof[anchor] * SPEED_OF_LIGHT;

          LED_G_Toggle();
        }
        else {
          LED_B_Toggle();
        }
        delay_ms(1);
      }
      else {
        DW1000_writeData32(SYS_STATUS_ID, 0, SYS_STATUS_ALL_RX_ERR);
        LED_R_Toggle();
      }
    }

    for(uint8_t i = 0; i < MAX_ANCHOR; i++) {
      distance32[i] = distance[i];
//      printf(" [%2i] %10.7f m\r\n", DESTINATION_ADDR + i, distance[i]);
    }
//    printf("\r\n");
    sendDistance(distance32);
    LED_B_Toggle();
//    delay_ms(5);
  }
}



#elif defined(ENABLE_SSTWR_RESPONDER)
/********************************************************************
 *                                                                  *
 *                      ENABLE_SSTWR_RESPONDER                      *
 *                                                                  *
 ********************************************************************/

static uint8_t rx_poll_msg[INITIATOR_TX_BUF_LEN] = {0};
static uint8_t tx_resp_msg[INITIATOR_RX_BUF_LEN] = {0};
static uint8_t rx_buffer[INITIATOR_TX_BUF_LEN]   = {0};

#define POLL_RX_TO_RESP_TX_DLY_UUS    330

static uint64_t poll_rx_ts;
static uint64_t resp_tx_ts;

static uint64_t get_rx_timestamp_u64( void )
{
  uint8_t ts_tab[5];
  uint64_t ts = 0;

  dwt_readrxtimestamp(ts_tab);
  for(int32_t i = 4; i >= 0; i--) {
    ts <<= 8;
    ts |= ts_tab[i];
  }
  return ts;
}

static void resp_msg_set_ts( uint8_t *ts_field, const uint64_t ts )
{
  for(uint32_t i = 0; i < RESP_MSG_TS_LEN; i++) {
    ts_field[i] = (ts >> (i * 8)) & 0xFF;
  }
}

void DEMO_SSTWR_RESPONDER( void )
{
  rx_poll_msg[0] = 'K';
  rx_poll_msg[1] = 'S';
  rx_poll_msg[2] = 'D';
  rx_poll_msg[3] = 'W';
  rx_poll_msg[4] = 0;   // source address
  rx_poll_msg[5] = 0;   // source address
  rx_poll_msg[6] = DESTINATION_ADDR;
  rx_poll_msg[7] = 0;

  tx_resp_msg[0] = 'K';
  tx_resp_msg[1] = 'S';
  tx_resp_msg[2] = 'D';
  tx_resp_msg[3] = 'W';
  tx_resp_msg[4] = DESTINATION_ADDR;
  tx_resp_msg[5] = 0;
  tx_resp_msg[6] = 0;
  tx_resp_msg[7] = 0;

  while(1) {
    dwt_rxenable(0);

    do {
      status = DW1000_readData32(SYS_STATUS_ID, 0);
    } while(!(status & (SYS_STATUS_RXFCG | SYS_STATUS_ALL_RX_ERR)));

    if(status & SYS_STATUS_RXFCG) {
      uint32_t frame_len;

      DW1000_writeData32(SYS_STATUS_ID, 0, SYS_STATUS_RXFCG);

      frame_len = DW1000_readData32(RX_FINFO_ID, 0) & RX_FINFO_RXFL_MASK_1023;
      if(frame_len <= 1024) { // DW1000_RX_BUFFER lens = 1024
        dwt_readrxdata(rx_buffer, frame_len, 0);
      }

//      if(memcmp(rx_buffer, rx_poll_msg, ALL_MSG_COMMON_LEN) == 0) {
      if(checkPacket(rx_buffer, rx_poll_msg) == 0) {
        uint32_t resp_tx_time;

        poll_rx_ts = get_rx_timestamp_u64();

        resp_tx_time = (poll_rx_ts + (POLL_RX_TO_RESP_TX_DLY_UUS * UUS_TO_DWT_TIME)) >> 8;
        dwt_setdelayedtrxtime(resp_tx_time);

        resp_tx_ts = (((uint64_t)(resp_tx_time & 0xFFFFFFFE)) << 8) + TX_ANT_DLY;

        resp_msg_set_ts(&tx_resp_msg[RESP_MSG_POLL_RX_TS_IDX], poll_rx_ts);
        resp_msg_set_ts(&tx_resp_msg[RESP_MSG_RESP_TX_TS_IDX], resp_tx_ts);

        dwt_writetxdata(sizeof(tx_resp_msg), tx_resp_msg, 0);
        dwt_writetxfctrl(sizeof(tx_resp_msg), 0);
        dwt_starttx(DWT_START_TX_DELAYED);

        while(!(DW1000_readData32(SYS_STATUS_ID, 0) & SYS_STATUS_TXFRS))
        { };

        DW1000_writeData32(SYS_STATUS_ID, 0, SYS_STATUS_TXFRS);
        LED_B_Toggle();
      }
    }
    else {
      DW1000_writeData32(SYS_STATUS_ID, 0, SYS_STATUS_ALL_RX_ERR);
      LED_R_Toggle();
    }
  }
}

#endif
/*====================================================================================================*/
/*====================================================================================================*/
