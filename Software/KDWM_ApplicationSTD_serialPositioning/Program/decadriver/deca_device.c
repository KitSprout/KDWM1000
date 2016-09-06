
#include "deca_params.h"
#include "modules\dw1000.h"

// -------------------------------------------------------------------------------------------------------------------
// Internal functions for controlling and configuring the device
// -------------------------------------------------------------------------------------------------------------------

// Configure the ucode (FP algorithm) parameters
void _dwt_configlde(int prf);
// Program the non-volatile memory
uint32_t _dwt_otpprogword32(uint32_t data, uint16_t address);
// Upload the device configuration into always on memory
void _dwt_aonarrayupload(void);
// -------------------------------------------------------------------------------------------------------------------

/*!
 * Static data for DW1000 DecaWave Transceiver control
 */

// -------------------------------------------------------------------------------------------------------------------
// Structure to hold device data
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

} dwt_local_data_t ;

static dwt_local_data_t KDW; // Static local device data

int dwt_initialise( uint16_t config )
{
  uint8_t tmp8 = 0;
  uint8_t reg[4] = {0};
  uint32_t tmp32 = 0;

KDW.dblbuffon    = 0; // Double buffer mode off by default
KDW.prfIndex     = 0; // 16MHz
KDW.cdata.aatset = 0; // Auto ACK bit not set
KDW.wait4resp    = 0;
KDW.sleep_mode   = 0;

KDW.dwt_txcallback = NULL;
KDW.dwt_rxcallback = NULL;

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

  DW1000_SetSpeed(SPI_BaudRatePrescaler_4);

  return DWT_SUCCESS;
}

/*! ------------------------------------------------------------------------------------------------------------------
 * @fn dwt_configure()
 *
 * @brief This function provides the main API for the configuration of the
 * DW1000 and this low-level driver.  The input is a pointer to the data structure
 * of type dwt_config_t that holds all the configurable items.
 * The dwt_config_t structure shows which ones are supported
 *
 * input parameters
 * @param config    -   pointer to the configuration structure, which contains the device configuration data.
 *
 * output parameters
 *
 * returns DWT_SUCCESS for success, or DWT_ERROR for error
 */
int dwt_configure(dwt_config_t *config)
{
    uint8_t nsSfd_result  = 0;
    uint8_t useDWnsSFD = 0;
    uint8_t chan = config->chan ;
    uint32_t regval ;
    uint16_t reg16 = lde_replicaCoeff[config->rxCode];
    uint8_t prfIndex = KDW.prfIndex = config->prf - DWT_PRF_16M;
    uint8_t bw = ((chan == 4) || (chan == 7)) ? 1 : 0 ; // Select wide or narrow band

    KDW.chan = config->chan ;

    // For 110 kbps we need a special setup
    if(DWT_BR_110K == config->dataRate)
    {
        KDW.sysCFGreg |= SYS_CFG_RXM110K ;
        reg16 >>= 3; // lde_replicaCoeff must be divided by 8
    }
    else
    {
        KDW.sysCFGreg &= (~SYS_CFG_RXM110K) ;
    }

    KDW.longFrames = config->phrMode ;

    KDW.sysCFGreg |= (SYS_CFG_PHR_MODE_11 & (config->phrMode << 16)) ;

    DW1000_writeData32(SYS_CFG_ID, 0, KDW.sysCFGreg) ;
    // Set the lde_replicaCoeff
    DW1000_writeData16(LDE_IF_ID, LDE_REPC_OFFSET, reg16) ;

    _dwt_configlde(prfIndex);

    // Configure PLL2/RF PLL block CFG (for a given channel)
    DW1000_writeData(FS_CTRL_ID, FS_PLLCFG_OFFSET, &pll2_config[chan_idx[chan]][0], 5);

    // Configure RF RX blocks (for specified channel/bandwidth)
    DW1000_writeData(RF_CONF_ID, RF_RXCTRLH_OFFSET, &rx_config[bw], 1);

    // Configure RF TX blocks (for specified channel and PRF)
    // Configure RF TX control
    DW1000_writeData32(RF_CONF_ID, RF_TXCTRL_OFFSET, tx_config[chan_idx[chan]]);

    // Configure the baseband parameters (for specified PRF, bit rate, PAC, and SFD settings)
    // DTUNE0
    DW1000_writeData16(DRX_CONF_ID, DRX_TUNE0b_OFFSET, sftsh[config->dataRate][config->nsSFD]);

    // DTUNE1
    DW1000_writeData16(DRX_CONF_ID, DRX_TUNE1a_OFFSET, dtune1[prfIndex]);

    if(config->dataRate == DWT_BR_110K)
    {
        DW1000_writeData16(DRX_CONF_ID, DRX_TUNE1b_OFFSET, 0x64);
    }
    else
    {
        if(config->txPreambLength == DWT_PLEN_64)
        {
            uint8_t temp = 0x10;
            DW1000_writeData16(DRX_CONF_ID, DRX_TUNE1b_OFFSET, 0x10);
            DW1000_writeData(DRX_CONF_ID, 0x26, &temp, 1);
        }
        else
        {
            uint8_t temp = 0x28;
            DW1000_writeData16(DRX_CONF_ID, DRX_TUNE1b_OFFSET, 0x20);
            DW1000_writeData(DRX_CONF_ID, 0x26, &temp, 1);
        }
    }

    // DTUNE2
    DW1000_writeData32(DRX_CONF_ID, DRX_TUNE2_OFFSET, digital_bb_config[prfIndex][config->rxPAC]);

    // DTUNE3 (SFD timeout)
    // Don't allow 0 - SFD timeout will always be enabled
    if(config->sfdTO == 0)
    {
        config->sfdTO = DWT_SFDTOC_DEF;
    }
    DW1000_writeData16(DRX_CONF_ID, DRX_SFDTOC_OFFSET, config->sfdTO);

    // Configure AGC parameters
    DW1000_writeData32( AGC_CFG_STS_ID, 0xC, agc_config.lo32);
    DW1000_writeData16( AGC_CFG_STS_ID, 0x4, agc_config.target[prfIndex]);

    // Set (non-standard) user SFD for improved performance,
    if(config->nsSFD)
    {
         // Write non standard (DW) SFD length
         DW1000_writeData(DW1000_USR_SFD,0x00,&dwnsSFDlen[config->dataRate],1);
         nsSfd_result = 3 ;
         useDWnsSFD = 1 ;
    }
    regval =  (CHAN_CTRL_TX_CHAN_MASK & (chan << CHAN_CTRL_TX_CHAN_SHIFT)) | // Transmit Channel
              (CHAN_CTRL_RX_CHAN_MASK & (chan << CHAN_CTRL_RX_CHAN_SHIFT)) | // Receive Channel
              (CHAN_CTRL_RXFPRF_MASK & (config->prf << CHAN_CTRL_RXFPRF_SHIFT)) | // RX PRF
              ((CHAN_CTRL_TNSSFD|CHAN_CTRL_RNSSFD) & (nsSfd_result << CHAN_CTRL_TNSSFD_SHIFT)) | // nsSFD enable RX&TX
              (CHAN_CTRL_DWSFD & (useDWnsSFD << CHAN_CTRL_DWSFD_SHIFT)) | // Use DW nsSFD
              (CHAN_CTRL_TX_PCOD_MASK & (config->txCode << CHAN_CTRL_TX_PCOD_SHIFT)) | // TX Preamble Code
              (CHAN_CTRL_RX_PCOD_MASK & (config->rxCode << CHAN_CTRL_RX_PCOD_SHIFT)) ; // RX Preamble Code

    DW1000_writeData32(CHAN_CTRL_ID, 0, regval) ;

    // Set up TX Preamble Size and TX PRF
    // Set up TX Ranging Bit and Data Rate
    KDW.txFCTRL  = (config->txPreambLength | config->prf) << 16;
    KDW.txFCTRL |= (config->dataRate << 13) | 0x00008000; // Always set ranging bit !!!
    DW1000_writeData32(DW1000_TX_FCTRL, 0, KDW.txFCTRL) ;

    return DWT_SUCCESS ;
}
/*! ------------------------------------------------------------------------------------------------------------------
 * @fn dwt_setrxantennadelay()
 *
 * @brief This API function writes the antenna delay (in time units) to RX registers
 *
 * input parameters:
 * @param rxDelay - this is the total (RX) antenna delay value, which
 *                          will be programmed into the RX register
 *
 * output parameters
 *
 * no return value
 */
void dwt_setrxantennadelay(uint16_t rxDelay)
{
  // Set the RX antenna delay for auto TX timestamp adjustment
  DW1000_writeData16(LDE_IF_ID, LDE_RXANTD_OFFSET, rxDelay);
}

void dwt_settxantennadelay(uint16_t txDelay)
{
  // Set the TX antenna delay for auto TX timestamp adjustment
  DW1000_writeData16(DW1000_TX_ANTD, 0x0, txDelay);
}

/*! ------------------------------------------------------------------------------------------------------------------
 * @fn dwt_writetxdata()
 *
 * @brief This API function writes the supplied TX data into the DW1000's
 * TX buffer.  The input parameters are the data length in bytes and a pointer
 * to those data bytes.
 *
 * input parameters
 * @param txFrameLength  - This is the total frame length, including the two byte CRC.
 *                         Note: this is the length of TX message (including the 2 byte CRC) - max is 1023
 *                         standard PHR mode allows up to 127 bytes
 *                         if > 127 is programmed, DWT_PHRMODE_EXT needs to be set in the phrMode configuration
 *                         see dwt_configure function
 * @param txFrameBytes   - Pointer to the userӳ buffer containing the data to send.
 * @param txBufferOffset - This specifies an offset in the DW1000ӳ TX Buffer at which to start writing data.
 *
 * output parameters
 *
 * returns DWT_SUCCESS for success, or DWT_ERROR for error
 */
int dwt_writetxdata(uint16_t txFrameLength, uint8_t *txFrameBytes, uint16_t txBufferOffset)
{
    if ((txBufferOffset + txFrameLength) > 1024)
    {
        return DWT_ERROR ;
    }
    // Write the data to the IC TX buffer, (-2 bytes for auto generated CRC)
    DW1000_writeData( DW1000_TX_BUFFER, txBufferOffset, txFrameBytes, txFrameLength-2) ;

    return DWT_SUCCESS ;
} // end dwt_writetxdata()

/*! ------------------------------------------------------------------------------------------------------------------
 * @fn dwt_writetxfctrl()
 *
 * @brief This API function configures the TX frame control register before the transmission of a frame
 *
 * input parameters:
 * @param txFrameLength - this is the length of TX message (including the 2 byte CRC) - max is 1023
 *                              NOTE: standard PHR mode allows up to 127 bytes
 *                              if > 127 is programmed, DWT_PHRMODE_EXT needs to be set in the phrMode configuration
 *                              see dwt_configure function
 * @param txBufferOffset - the offset in the tx buffer to start writing the data
 *
 * output parameters
 *
 * returns DWT_SUCCESS for success, or DWT_ERROR for error
 */
int dwt_writetxfctrl(uint16_t txFrameLength, uint16_t txBufferOffset)
{
    // Write the frame length to the TX frame control register
    // dw1000local.txFCTRL has kept configured bit rate information
    uint32_t reg32 = KDW.txFCTRL | txFrameLength | (txBufferOffset << 22);
    DW1000_writeData32(DW1000_TX_FCTRL, 0, reg32) ;

    return DWT_SUCCESS ;

} // end dwt_writetxfctrl()


/*! ------------------------------------------------------------------------------------------------------------------
 * @fn dwt_readrxdata()
 *
 * @brief This is used to read the data from the RX buffer, from an offset location give by offset parameter
 *
 * input parameters
 * @param buffer - the buffer into which the data will be read
 * @param length - the length of data to read (in bytes)
 * @param rxBufferOffset - the offset in the rx buffer from which to read the data
 *
 * output parameters
 *
 * no return value
 */
void dwt_readrxdata(uint8_t *buffer, uint16_t length, uint16_t rxBufferOffset)
{
  DW1000_readData(DW1000_RX_BUFFER,rxBufferOffset,buffer,length) ;
}

/*! ------------------------------------------------------------------------------------------------------------------
 * @fn dwt_readtxtimestamplo32()
 *
 * @brief This is used to read the low 32-bits of the TX timestamp (adjusted with the programmed antenna delay)
 *
 * input parameters
 *
 * output parameters
 *
 * returns low 32-bits of TX timestamp
 */
uint32_t dwt_readtxtimestamplo32(void)
{
  return DW1000_readData32(DW1000_TX_TIME, 0);
}

/*! ------------------------------------------------------------------------------------------------------------------
 * @fn dwt_readrxtimestamp()
 *
 * @brief This is used to read the RX timestamp (adjusted time of arrival)
 *
 * input parameters
 * @param timestamp - a pointer to a 5-byte buffer which will store the read RX timestamp time
 *
 * output parameters - the timestamp buffer will contain the value after the function call
 *
 * no return value
 */
void dwt_readrxtimestamp(uint8_t * timestamp)
{
  DW1000_readData(DW1000_RX_TIME, 0, timestamp, 5) ; // Get the adjusted time of arrival
}

/*! ------------------------------------------------------------------------------------------------------------------
 * @fn dwt_readrxtimestamplo32()
 *
 * @brief This is used to read the low 32-bits of the RX timestamp (adjusted with the programmed antenna delay)
 *
 * input parameters
 *
 * output parameters
 *
 * returns low 32-bits of RX timestamp
 */
uint32_t dwt_readrxtimestamplo32(void)
{
  return DW1000_readData32(DW1000_RX_TIME, 0);
}

/*! ------------------------------------------------------------------------------------------------------------------
 * @fn _dwt_otpsetmrregs()
 *
 * @brief Configure the MR registers for initial programming (enable charge pump).
 * Read margin is used to stress the read back from the
 * programmed bit. In normal operation this is relaxed.
 *
 * input parameters
 * @param mode - "0" : Reset all to 0x0:           MRA=0x0000, MRB=0x0000, MR=0x0000
 *               "1" : Set for inital programming: MRA=0x9220, MRB=0x000E, MR=0x1024
 *               "2" : Set for soak programming:   MRA=0x9220, MRB=0x0003, MR=0x1824
 *               "3" : High Vpp:                   MRA=0x9220, MRB=0x004E, MR=0x1824
 *               "4" : Low Read Margin:            MRA=0x0000, MRB=0x0003, MR=0x0000
 *               "5" : Array Clean:                MRA=0x0049, MRB=0x0003, MR=0x0024
 *               "4" : Very Low Read Margin:       MRA=0x0000, MRB=0x0003, MR=0x0000
 *
 * output parameters
 *
 * returns DWT_SUCCESS for success, or DWT_ERROR for error
 */
uint32_t _dwt_otpsetmrregs(int mode)
{
    uint8_t rd_buf[4];
    uint8_t wr_buf[4];
    uint32_t mra=0,mrb=0,mr=0;
    //printf("OTP SET MR: Setting MR,MRa,MRb for mode %2x\n",mode);

    // PROGRAMME MRA
    // Set MRA, MODE_SEL
    wr_buf[0] = 0x03;
    DW1000_writeData(OTP_IF_ID, OTP_CTRL+1,wr_buf,1);

    // Load data
    switch(mode&0x0f) {
    case 0x0 :
        mr =0x0000;
        mra=0x0000;
        mrb=0x0000;
        break;
    case 0x1 :
        mr =0x1024;
        mra=0x9220; // Enable CPP mon
        mrb=0x000e;
        break;
    case 0x2 :
        mr =0x1824;
        mra=0x9220;
        mrb=0x0003;
        break;
    case 0x3 :
        mr =0x1824;
        mra=0x9220;
        mrb=0x004e;
        break;
    case 0x4 :
        mr =0x0000;
        mra=0x0000;
        mrb=0x0003;
        break;
    case 0x5 :
        mr =0x0024;
        mra=0x0000;
        mrb=0x0003;
        break;
    default :
    //  printf("OTP SET MR: ERROR : Invalid mode selected\n",mode);
        return (uint32_t)DWT_ERROR;
    }

    wr_buf[0] = mra & 0x00ff;
    wr_buf[1] = (mra & 0xff00)>>8;
    DW1000_writeData(OTP_IF_ID, OTP_WDAT,wr_buf,2);


    // Set WRITE_MR
    wr_buf[0] = 0x08;
    DW1000_writeData(OTP_IF_ID, OTP_CTRL,wr_buf,1);

    // Wait?

    // Set Clear Mode sel
    wr_buf[0] = 0x02;
    DW1000_writeData(OTP_IF_ID,OTP_CTRL+1,wr_buf,1);

    // Set AUX update, write MR
    wr_buf[0] = 0x88;
    DW1000_writeData(OTP_IF_ID, OTP_CTRL,wr_buf,1);
    // Clear write MR
    wr_buf[0] = 0x80;
    DW1000_writeData(OTP_IF_ID, OTP_CTRL,wr_buf,1);
    // Clear AUX update
    wr_buf[0] = 0x00;
    DW1000_writeData(OTP_IF_ID, OTP_CTRL,wr_buf,1);

    ///////////////////////////////////////////
    // PROGRAM MRB
    // Set SLOW, MRB, MODE_SEL
    wr_buf[0] = 0x05;
    DW1000_writeData(OTP_IF_ID,OTP_CTRL+1,wr_buf,1);

    wr_buf[0] = mrb & 0x00ff;
    wr_buf[1] = (mrb & 0xff00)>>8;
    DW1000_writeData(OTP_IF_ID, OTP_WDAT,wr_buf,2);

    // Set WRITE_MR
    wr_buf[0] = 0x08;
    DW1000_writeData(OTP_IF_ID, OTP_CTRL,wr_buf,1);

    // Wait?

    // Set Clear Mode sel
    wr_buf[0] = 0x04;
    DW1000_writeData(OTP_IF_ID,OTP_CTRL+1,wr_buf,1);

    // Set AUX update, write MR
    wr_buf[0] = 0x88;
    DW1000_writeData(OTP_IF_ID, OTP_CTRL,wr_buf,1);
    // Clear write MR
    wr_buf[0] = 0x80;
    DW1000_writeData(OTP_IF_ID, OTP_CTRL,wr_buf,1);
    // Clear AUX update
    wr_buf[0] = 0x00;
    DW1000_writeData(OTP_IF_ID, OTP_CTRL,wr_buf,1);

    ///////////////////////////////////////////
    // PROGRAM MR
    // Set SLOW, MODE_SEL
    wr_buf[0] = 0x01;
    DW1000_writeData(OTP_IF_ID,OTP_CTRL+1,wr_buf,1);
    // Load data

    wr_buf[0] = mr & 0x00ff;
    wr_buf[1] = (mr & 0xff00)>>8;
    DW1000_writeData(OTP_IF_ID, OTP_WDAT,wr_buf,2);

    // Set WRITE_MR
    wr_buf[0] = 0x08;
    DW1000_writeData(OTP_IF_ID, OTP_CTRL,wr_buf,1);

    // Wait?
    delay_ms(10);
    // Set Clear Mode sel
    wr_buf[0] = 0x00;
    DW1000_writeData(OTP_IF_ID,OTP_CTRL+1,wr_buf,1);

    // Read confirm mode writes.
    // Set man override, MRA_SEL
    wr_buf[0] = OTP_CTRL_OTPRDEN;
    DW1000_writeData(OTP_IF_ID, OTP_CTRL,wr_buf,1);
    wr_buf[0] = 0x02;
    DW1000_writeData(OTP_IF_ID,OTP_CTRL+1,wr_buf,1);
    // MRB_SEL
    wr_buf[0] = 0x04;
    DW1000_writeData(OTP_IF_ID,OTP_CTRL+1,wr_buf,1);
    delay_ms(100);

    // Clear mode sel
    wr_buf[0] = 0x00;
    DW1000_writeData(OTP_IF_ID,OTP_CTRL+1,wr_buf,1);
    // Clear MAN_OVERRIDE
    wr_buf[0] = 0x00;
    DW1000_writeData(OTP_IF_ID, OTP_CTRL,wr_buf,1);

    delay_ms(10);

    if (((mode&0x0f) == 0x1)||((mode&0x0f) == 0x2))
	{
        // Read status register
        DW1000_readData(OTP_IF_ID, OTP_STAT,rd_buf,1);
    }

    return DWT_SUCCESS;
}

/*! ------------------------------------------------------------------------------------------------------------------
 * @fn _dwt_otpprogword32()
 *
 * @brief function to program the OTP memory. Ensure that MR,MRa,MRb are reset to 0.
 * VNM Charge pump needs to be enabled (see _dwt_otpsetmrregs)
 * Note the address is only 11 bits long.
 *
 * input parameters
 * @param address - address to read at
 *
 * output parameters
 *
 * returns DWT_SUCCESS for success, or DWT_ERROR for error
 */
uint32_t _dwt_otpprogword32(uint32_t data, uint16_t address)
{
    uint8_t rd_buf[1];
    uint8_t wr_buf[4];
    uint8_t otp_done;

    // Read status register
    DW1000_readData(OTP_IF_ID, OTP_STAT, rd_buf, 1);

    if((rd_buf[0] & 0x02) != 0x02)
    {
//        printf("OTP PROG 32: ERROR VPP NOT OK, programming will fail. Are MR/MRA/MRB set?\n");
        return (uint32_t)DWT_ERROR;
    }

    // Write the data
    wr_buf[3] = (data>>24) & 0xff;
    wr_buf[2] = (data>>16) & 0xff;
    wr_buf[1] = (data>>8) & 0xff;
    wr_buf[0] = data & 0xff;
    DW1000_writeData(OTP_IF_ID, OTP_WDAT, wr_buf, 4);

    // Write the address [10:0]
    wr_buf[1] = (address>>8) & 0x07;
    wr_buf[0] = address & 0xff;
    DW1000_writeData(OTP_IF_ID, OTP_ADDR, wr_buf, 2);

    // Enable Sequenced programming
    wr_buf[0] = OTP_CTRL_OTPPROG;
    DW1000_writeData(OTP_IF_ID, OTP_CTRL, wr_buf, 1);
    wr_buf[0] = 0x00; // And clear
    DW1000_writeData(OTP_IF_ID, OTP_CTRL, wr_buf, 1);

    // WAIT for status to flag PRGM OK..
    otp_done = 0;
    while(otp_done == 0)
    {
        delay_ms(1);
        DW1000_readData(OTP_IF_ID, OTP_STAT, rd_buf, 1);

        if((rd_buf[0] & 0x01) == 0x01)
        {
            otp_done = 1;
        }
    }

    return DWT_SUCCESS;
}

/*! ------------------------------------------------------------------------------------------------------------------
 * @fn _dwt_aonconfigupload()
 *
 * @brief This function uploads always on (AON) configuration, as set in the AON_CFG0_OFFSET register.
 *
 * input parameters
 *
 * output parameters
 *
 * no return value
 */
void _dwt_aonconfigupload(void)
{
    uint8_t buf[1];

    buf[0] = 0x04;
    DW1000_writeData(AON_ID,AON_CTRL_OFFSET,buf,1);
    buf[0] = 0x00;
    DW1000_writeData(AON_ID,AON_CTRL_OFFSET,buf,1);
}

/*! ------------------------------------------------------------------------------------------------------------------
 * @fn _dwt_aonarrayupload()
 *
 * @brief This function uploads always on (AON) data array and configuration. Thus if this function is used, then _dwt_aonconfigupload
 * is not necessary. The DW1000 will go so SLEEP straight after this if the DWT_SLP_EN has been set.
 *
 * input parameters
 *
 * output parameters
 *
 * no return value
 */
void _dwt_aonarrayupload(void)
{
    uint8_t buf[1];

    buf[0] = 0x00;
    DW1000_writeData(AON_ID,AON_CTRL_OFFSET,buf,1);
    buf[0] = 0x02;
    DW1000_writeData(AON_ID,AON_CTRL_OFFSET,buf,1);
}

/*! ------------------------------------------------------------------------------------------------------------------
 * @fn dwt_entersleepaftertx(int enable)
 *
 * @brief sets the auto TX to sleep bit. This means that after a frame
 * transmission the device will enter deep sleep mode. The dwt_setdeepsleep() function
 * needs to be called before this to configure the on-wake settings
 *
 * NOTE: the IRQ line has to be low/inactive (i.e. no pending events)
 *
 * input parameters
 * @param enable - 1 to configure the device to enter deep sleep after TX, 0 - disables the configuration
 *
 * output parameters
 *
 * no return value
 */
void dwt_entersleepaftertx(int enable)
{
    uint32_t reg = DW1000_readData32(PMSC_ID, PMSC_CTRL1_OFFSET);
    // Set the auto TX -> sleep bit
    if(enable)
    {
        reg |= PMSC_CTRL1_ATXSLP;
    }
    else
    {
        reg &= ~(PMSC_CTRL1_ATXSLP);
    }
    DW1000_writeData32(PMSC_ID, PMSC_CTRL1_OFFSET, reg);
}

/*! ------------------------------------------------------------------------------------------------------------------
 * @fn _dwt_configlde()
 *
 * @brief configure LDE algorithm parameters
 *
 * input parameters
 * @param prf   -   this is the PRF index (0 or 1) 0 corresponds to 16 and 1 to 64 PRF
 *
 * output parameters
 *
 * no return value
 */
void _dwt_configlde(int prfIndex)
{
    uint8_t x = LDE_PARAM1;

    DW1000_writeData( LDE_IF_ID, LDE_CFG1_OFFSET, &x , 1); // 8-bit configuration register

    if(prfIndex)
    {
        DW1000_writeData16( LDE_IF_ID, LDE_CFG2_OFFSET, (uint16_t) LDE_PARAM3_64); // 16-bit LDE configuration tuning register
    }
    else
    {
        DW1000_writeData16( LDE_IF_ID, LDE_CFG2_OFFSET, (uint16_t) LDE_PARAM3_16);
    }
}

/*! ------------------------------------------------------------------------------------------------------------------
 * @fn dwt_setrxaftertxdelay()
 *
 * @brief This sets the receiver turn on delay time after a transmission of a frame
 *
 * input parameters
 * @param rxDelayTime - (20 bits) - the delay is in UWB microseconds
 *
 * output parameters
 *
 * no return value
 */
void dwt_setrxaftertxdelay(uint32_t rxDelayTime)
{
    uint32_t val = DW1000_readData32(ACK_RESP_T_ID, 0) ; // Read ACK_RESP_T_ID register

    val &= ~(ACK_RESP_T_W4R_TIM_MASK) ; // Clear the timer (19:0)

    val |= (rxDelayTime & ACK_RESP_T_W4R_TIM_MASK) ; // In UWB microseconds (e.g. turn the receiver on 20uus after TX)

    DW1000_writeData32(ACK_RESP_T_ID, 0, val) ;
}

/*! ------------------------------------------------------------------------------------------------------------------
 * @fn _dwt_disablesequencing()
 *
 * @brief This function disables the TX blocks sequencing, it disables PMSC control of RF blocks, system clock is also set to XTAL
 *
 * input parameters none
 *
 * output parameters none
 *
 * no return value
 */
void _dwt_disablesequencing(void) // Disable sequencing and go to state "INIT"
{
    DW1000_SetClock(FORCE_SYS_XTI); // Set system clock to XTI

    DW1000_writeData16(PMSC_ID, PMSC_CTRL1_OFFSET, PMSC_CTRL1_PKTSEQ_DISABLE); // Disable PMSC ctrl of RF and RX clk blocks
}

/*! ------------------------------------------------------------------------------------------------------------------
 * @fn dwt_setdelayedtrxtime()
 *
 * @brief This API function configures the delayed transmit time or the delayed RX on time
 *
 * input parameters
 * @param starttime - the TX/RX start time (the 32 bits should be the high 32 bits of the system time at which to send the message,
 * or at which to turn on the receiver)
 *
 * output parameters none
 *
 * no return value
 */
void dwt_setdelayedtrxtime(uint32_t starttime)
{
    DW1000_writeData32(DW1000_DX_TIME, 1, starttime) ;

} // end dwt_setdelayedtrxtime()

/*! ------------------------------------------------------------------------------------------------------------------
 * @fn dwt_starttx()
 *
 * @brief This call initiates the transmission, input parameter indicates which TX mode is used see below
 *
 * input parameters:
 * @param mode - if 0 immediate TX (no response expected)
 *               if 1 delayed TX (no response expected)
 *               if 2 immediate TX (response expected - so the receiver will be automatically turned on after TX is done)
 *               if 3 delayed TX (response expected - so the receiver will be automatically turned on after TX is done)
 *
 * output parameters
 *
 * returns DWT_SUCCESS for success, or DWT_ERROR for error (e.g. a delayed transmission will fail if the delayed time has passed)
 */
int dwt_starttx(uint8_t mode)
{
    int retval = DWT_SUCCESS ;
    uint8_t temp  = 0x00;
    uint16_t checkTxOK = 0 ;

    if(mode & DWT_RESPONSE_EXPECTED)
    {
        temp = 0x80 ; // Set wait4response bit
        DW1000_writeData(DW1000_SYS_CTRL,0,&temp,1) ;
        KDW.wait4resp = 1;
    }

    if (mode & DWT_START_TX_DELAYED)
    {
        //uint32_t status ;

        // Both SYS_CTRL_TXSTRT and SYS_CTRL_TXDLYS to correctly enable TX
        temp |= (uint8_t)(0x06) ;
        DW1000_writeData(DW1000_SYS_CTRL,0,&temp,1) ;
        checkTxOK = DW1000_readData16(SYS_STATUS_ID,3) ;
        //status = DW1000_readData32(SYS_STATUS_ID) ; // Read status register
        if ((checkTxOK & SYS_STATUS_TXERR) == 0) // Transmit Delayed Send set over Half a Period away or Power Up error (there is enough time to send but not to power up individual blocks).
        {
            //printf("tx delayed \n");
            retval = DWT_SUCCESS ; // All okay
        }
        else
        {
            // I am taking DSHP set to Indicate that the TXDLYS was set too late for the specified DX_TIME.
            // Remedial Action - (a) cancel delayed send
            temp = (uint8_t)0x40; // This assumes the bit is in the lowest byte
            DW1000_writeData(DW1000_SYS_CTRL,0,&temp,1) ;
            // Note event Delayed TX Time too Late
            // Could fall through to start a normal send (below) just sending late.....
            // ... instead return and assume return value of 1 will be used to detect and recover from the issue.

            // Clear the "auto TX to sleep" bit
            dwt_entersleepaftertx(0);
            KDW.wait4resp = 0;
            retval = DWT_ERROR ; // Failed !

        }
    }
    else
    {
        temp |= (uint8_t)0x02 ;
        DW1000_writeData(DW1000_SYS_CTRL,0,&temp,1) ;
    }

    return retval;

} // end dwt_starttx()

/*! ------------------------------------------------------------------------------------------------------------------
 * @fn dwt_forcetrxoff()
 *
 * @brief This is used to turn off the transceiver
 *
 * input parameters
 *
 * output parameters
 *
 * no return value
 */
void dwt_forcetrxoff(void)
{
//	decaIrqStatus_t stat ;
    uint8_t temp ;
	uint32_t mask;

    temp = (uint8_t)0x40 ; // This assumes the bit is in the lowest byte

	mask = DW1000_readData32(SYS_MASK_ID, 0) ; // Read set interrupt mask

	// Need to beware of interrupts occurring in the middle of following read modify write cycle
	// We can disable the radio, but before the status is cleared an interrupt can be set (e.g. the
	// event has just happened before the radio was disabled) thus we need to disable interrupt during this operation
//    stat = decamutexon() ;

	DW1000_writeData32(SYS_MASK_ID, 0, 0) ; // Clear interrupt mask - so we don't get any unwanted events

    DW1000_writeData(DW1000_SYS_CTRL,0,&temp,1) ; // Disable the radio

    // Forcing Transceiver off - so we do not want to see any new events that may have happened
    DW1000_writeData32(SYS_STATUS_ID, 0, (SYS_STATUS_ALL_TX | SYS_STATUS_ALL_RX_ERR | SYS_STATUS_ALL_RX_GOOD)) ;

    dwt_syncrxbufptrs();

	DW1000_writeData32(SYS_MASK_ID, 0, mask) ; // Set interrupt mask to what it was

	// Enable/restore interrupts again...
//	decamutexoff(stat);
    KDW.wait4resp = 0;

} // end deviceforcetrxoff()

/*! ------------------------------------------------------------------------------------------------------------------
 * @fn dwt_syncrxbufptrs()
 *
 * @brief this function synchronizes rx buffer pointers
 * need to make sure that the host/IC buffer pointers are aligned before starting RX
 *
 * input parameters:
 *
 * output parameters
 *
 * no return value
 */
void dwt_syncrxbufptrs(void)
{
  uint8_t  buff;
  // Need to make sure that the host/IC buffer pointers are aligned before starting RX
  DW1000_readData(SYS_STATUS_ID, 3, &buff, 1);

  if((buff & (SYS_STATUS_ICRBP >> 24)) !=         // IC side Receive Buffer Pointer
    ((buff & (SYS_STATUS_HSRBP>>24)) << 1) ) {    // Host Side Receive Buffer Pointer
    uint8_t hsrb = 0x01;
    DW1000_writeData(DW1000_SYS_CTRL, 3 , &hsrb, 1) ; // We need to swap RX buffer status reg (write one to toggle internally)
  }
}

/*! ------------------------------------------------------------------------------------------------------------------
 * @fn dwt_rxenable()
 *
 * @brief This call turns on the receiver, can be immediate or delayed.
 * The receiver will stay turned on, listening to any messages until
 * it either receives a good frame, an error (CRC, PHY header, Reed Solomon) or  it times out (SFD, Preamble or Frame).
 *
 * input parameters
 * @param delayed - TRUE the receiver is turned on after some delay (as programmed with dwt_setdelayedtime())
 *
 * returns DWT_SUCCESS for success, or DWT_ERROR for error (e.g. a delayed receive enable will be too far in the future if delayed time has passed (if delayed time is > 8s from now))
 */
int dwt_rxenable( int delayed )
{
  uint16_t temp;
  uint8_t temp1 = 0;

  dwt_syncrxbufptrs();

  temp = 0x0100;

  if(delayed) {
    temp |= (uint16_t)0x0200;
  }

  DW1000_writeData16(DW1000_SYS_CTRL, 0, temp);

  if(delayed) {   // Check for errors
    //uint32_t status1 = DW1000_readData32(SYS_STATUS_ID) ; // Read status register

    DW1000_readData(SYS_STATUS_ID,3,&temp1,1) ;

    if(temp1 & (SYS_STATUS_HPDWARN >> 24)) { // If delay has not passed do delayed else immediate RX on
      dwt_forcetrxoff(); // Turn the delayed receive off, and do immediate receive, return warning indication
      temp = (uint16_t)0x0100; // Clear the delay bit
      DW1000_writeData16(DW1000_SYS_CTRL,0,temp) ;
      return DWT_ERROR;
    }
  }

  return DWT_SUCCESS;
} // end dwt_rxenable()

/*! ------------------------------------------------------------------------------------------------------------------
 * @fn dwt_setrxtimeout()
 *
 * @brief This call enables RX timeout (SY_STAT_RFTO event)
 *
 * input parameters
 * @param time - how long the receiver remains on from the RX enable command
 *               The time parameter used here is in 1.0256 us (512/499.2MHz) units
 *               If set to 0 the timeout is disabled.
 *
 * output parameters
 *
 * no return value
 */
void dwt_setrxtimeout(uint16_t time)
{
    uint8_t temp ;

    DW1000_readData(SYS_CFG_ID,3,&temp,1) ; // Read register

    if(time > 0)
    {
        DW1000_writeData16(DW1000_RX_FWTO, 0x0, time) ;

        temp |= (uint8_t)(SYS_CFG_RXWTOE>>24);
        // OR in 32bit value (1 bit set), I know this is in high byte.
        KDW.sysCFGreg |= SYS_CFG_RXWTOE;

        DW1000_writeData(SYS_CFG_ID,3,&temp,1) ;
    }
    else
    {
        temp &= ~((uint8_t)(SYS_CFG_RXWTOE>>24));
        // AND in inverted 32bit value (1 bit clear), I know this is in high byte.
        KDW.sysCFGreg &= ~(SYS_CFG_RXWTOE);

        DW1000_writeData(SYS_CFG_ID,3,&temp,1) ;

        //DW1000_writeData16(RX_FWTO_ID,0,0) ; // Clearing the time is not needed
    }

} // end dwt_setrxtimeout()

/*! ------------------------------------------------------------------------------------------------------------------
 * @fn dwt_rxreset()
 *
 * @brief this function resets the receiver of the DW1000
 *
 * input parameters:
 *
 * output parameters
 *
 * no return value
 */
void dwt_rxreset(void)
{
	uint8_t resetrx = 0xe0;
	// Set RX reset
	DW1000_writeData(PMSC_ID, 0x3, &resetrx, 1);

	resetrx = 0xf0; // Clear RX reset
	DW1000_writeData(PMSC_ID, 0x3, &resetrx, 1);
}

/*! ------------------------------------------------------------------------------------------------------------------
 * @fn dwt_configcwmode()
 *
 * @brief this function sets the DW1000 to transmit cw signal at specific channel frequency
 *
 * input parameters:
 * @param chan - specifies the operating channel (e.g. 1, 2, 3, 4, 5, 6 or 7)
 *
 * output parameters
 *
 * returns DWT_SUCCESS for success, or DWT_ERROR for error
 */
int dwt_configcwmode(uint8_t chan)
{
    uint8_t write_buf[1];

    // Disable TX/RX RF block sequencing (needed for cw frame mode)
    //
    _dwt_disablesequencing();

    // Config RF pll (for a given channel)
    // Configure PLL2/RF PLL block CFG
    DW1000_writeData(FS_CTRL_ID, FS_PLLCFG_OFFSET, &pll2_config[chan_idx[chan]][0], 5);
    // PLL wont be enabled until a TX/RX enable is issued later on
    // Configure RF TX blocks (for specified channel and prf)
    // Config RF TX control
    DW1000_writeData32(RF_CONF_ID, RF_TXCTRL_OFFSET, tx_config[chan_idx[chan]]);


    //
    // Enable RF PLL
    //
    DW1000_writeData32(RF_CONF_ID, 0, RF_CONF_TXPLLPOWEN_MASK); // Enable LDO and RF PLL blocks
    DW1000_writeData32(RF_CONF_ID, 0, RF_CONF_TXALLEN_MASK); // Enable the rest of TX blocks

    //
    // Configure TX clocks
    //
    write_buf[0] = 0x22;
    DW1000_writeData(PMSC_ID,PMSC_CTRL0_OFFSET,write_buf,1);
    write_buf[0] = 0x07;
    DW1000_writeData(PMSC_ID,0x1,write_buf,1);

    // Disable fine grain TX seq
    DW1000_writeData16(PMSC_ID, PMSC_TXFINESEQ_OFFSET, PMSC_TXFINESEQ_DIS_MASK);


	write_buf[0] = TC_PGTEST_CW;

    // Configure CW mode
    DW1000_writeData(TX_CAL_ID, TC_PGTEST_OFFSET, write_buf, TC_PGTEST_LEN);


    return DWT_SUCCESS ;
}

/*! ------------------------------------------------------------------------------------------------------------------
 * @fn dwt_configcontinuousframemode()
 *
 * @brief this function sets the DW1000 to continuous tx frame mode for regulatory approvals testing.
 *
 * input parameters:
 * @param framerepetitionrate - This is a 32-bit value that is used to set the interval between transmissions.
*  The minimum value is 4. The units are approximately 8 ns. (or more precisely 512/(499.2e6*128) seconds)).
 *
 * output parameters
 *
 * no return value
 */
void dwt_configcontinuousframemode(uint32_t framerepetitionrate)
{
    uint8_t write_buf[4];

    //
    // Disable TX/RX RF block sequencing (needed for continuous frame mode)
    //
    _dwt_disablesequencing();

    //
    // Enable RF PLL and TX blocks
    //
    DW1000_writeData32(RF_CONF_ID, 0, RF_CONF_TXPLLPOWEN_MASK); // Enable LDO and RF PLL blocks
    DW1000_writeData32(RF_CONF_ID, 0, RF_CONF_TXALLEN_MASK); // Enable the rest of TX blocks

    //
    // Configure TX clocks
    //
    DW1000_SetClock(FORCE_SYS_PLL);
    DW1000_SetClock(FORCE_TX_PLL);

    // Set the frame repetition rate
    if(framerepetitionrate < 4)
    {
        framerepetitionrate = 4;
    }
    DW1000_writeData32(DW1000_DX_TIME, 0, framerepetitionrate) ;
    //
    // Configure continuous frame TX
    //
    write_buf[0] = (uint8_t)(DIAG_TMC_TX_PSTM) ;
    DW1000_writeData(DIG_DIAG_ID, DIAG_TMC_OFFSET,write_buf, 1); // Turn the tx power spectrum test mode - continuous sending of frames
}
