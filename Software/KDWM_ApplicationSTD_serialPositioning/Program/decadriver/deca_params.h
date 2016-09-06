
#ifndef _DECA_PARAMS_H_
#define _DECA_PARAMS_H_

#include "drivers\stm32f4_system.h"
#include "algorithms\mathUnit.h"

#define NUM_BR            3
#define NUM_PRF           2
#define NUM_PACS          4
#define NUM_BW            2   // 2 bandwidths are supported
#define NUM_SFD           2   // supported number of SFDs - standard = 0, non-standard = 1
#define NUM_CH            6   // supported channels are 1, 2, 3, 4, 5, 7
#define NUM_CH_SUPPORTED  8   // supported channels are '0', 1, 2, 3, 4, 5, '6', 7
#define PCODES            25  // supported preamble codes

typedef struct {
  uint32_t lo32;
  uint16_t target[NUM_PRF];
} agc_cfg_struct;

extern const agc_cfg_struct agc_config;

// SFD threshold settings for 110k, 850k, 6.8Mb standard and non-standard
extern const uint16_t sftsh[NUM_BR][NUM_SFD]; 

extern const uint16_t dtune1[NUM_PRF];

#define XMLPARAMS_VERSION	(1.17f)

extern const uint8_t  pll2_config[NUM_CH][5];
extern const uint8_t  rx_config[NUM_BW];
extern const uint32_t tx_config[NUM_CH];
extern const uint8_t  dwnsSFDlen[NUM_BR];				// length of SFD for each of the bitrates
extern const uint32_t digital_bb_config[NUM_PRF][NUM_PACS];
extern const uint8_t  chan_idx[NUM_CH_SUPPORTED];

#define PEAK_MULTPLIER  (0x60)	// 3 -> (0x3 * 32) & 0x00E0 
#define N_STD_FACTOR    (13)
#define LDE_PARAM1      (PEAK_MULTPLIER | N_STD_FACTOR)

#define LDE_PARAM3_16   (0x1607)
#define LDE_PARAM3_64   (0x0607)

extern const uint16_t lde_replicaCoeff[PCODES];





































/****************************************************************************//**
 * @brief Bit definitions for register SYS_CFG
**/
#define SYS_CFG_ID              0x04            /* System Configuration (31:0) */
#define SYS_CFG_LEN             (4)
/*mask and shift */
#define SYS_CFG_MASK            0xF047FFFFUL    /* access mask to SYS_CFG_ID */
#define SYS_CFG_FF_ALL_EN       0x000001FEUL    /* Frame filtering options all frames allowed */
/*offset 0 */
#define SYS_CFG_FFE             0x00000001UL    /* Frame Filtering Enable. This bit enables the frame filtering functionality */
#define SYS_CFG_FFBC            0x00000002UL    /* Frame Filtering Behave as a Co-ordinator */
#define SYS_CFG_FFAB            0x00000004UL    /* Frame Filtering Allow Beacon frame reception */
#define SYS_CFG_FFAD            0x00000008UL    /* Frame Filtering Allow Data frame reception */
#define SYS_CFG_FFAA            0x00000010UL    /* Frame Filtering Allow Acknowledgment frame reception */
#define SYS_CFG_FFAM            0x00000020UL    /* Frame Filtering Allow MAC command frame reception */
#define SYS_CFG_FFAR            0x00000040UL    /* Frame Filtering Allow Reserved frame types */
#define SYS_CFG_FFA4            0x00000080UL    /* Frame Filtering Allow frames with frame type field of 4, (binary 100) */
/*offset 8 */
#define SYS_CFG_FFA5            0x00000100UL    /* Frame Filtering Allow frames with frame type field of 5, (binary 101) */
#define SYS_CFG_HIRQ_POL        0x00000200UL    /* Host interrupt polarity */
#define SYS_CFG_SPI_EDGE        0x00000400UL    /* SPI data launch edge */
#define SYS_CFG_DIS_FCE         0x00000800UL    /* Disable frame check error handling */
#define SYS_CFG_DIS_DRXB        0x00001000UL    /* Disable Double RX Buffer */
#define SYS_CFG_DIS_PHE         0x00002000UL    /* Disable receiver abort on PHR error */
#define SYS_CFG_DIS_RSDE        0x00004000UL    /* Disable Receiver Abort on RSD error */
#define SYS_CFG_FCS_INIT2F      0x00008000UL    /* initial seed value for the FCS generation and checking function */
/*offset 16 */
#define SYS_CFG_PHR_MODE_00     0x00000000UL    /* Standard Frame mode */
#define SYS_CFG_PHR_MODE_11     0x00030000UL    /* Long Frames mode */
#define SYS_CFG_DIS_STXP        0x00040000UL    /* Disable Smart TX Power control */
#define SYS_CFG_RXM110K         0x00400000UL    /* Receiver Mode 110 kbps data rate */
/*offset 24 */
#define SYS_CFG_RXWTOE          0x10000000UL    /* Receive Wait Timeout Enable. */
#define SYS_CFG_RXAUTR          0x20000000UL    /* Receiver Auto-Re-enable. This bit is used to cause the receiver to re-enable automatically */
#define SYS_CFG_AUTOACK         0x40000000UL    /* Automatic Acknowledgement Enable */
#define SYS_CFG_AACKPEND        0x80000000UL    /* Automatic Acknowledgement Pending bit control */

/****************************************************************************//**
 * @brief Bit definitions for register  SYS_MASK
**/
#define SYS_MASK_ID             0x0E            /* System Event Mask Register */
#define SYS_MASK_LEN            (4)
/*masks */
#define SYS_MASK_MASK_32        0x3FF7FFFEUL    /* System Event Mask Register access mask (all unused fields should always be writen as zero) */
/*offset 0 */
#define SYS_MASK_MCPLOCK        0x00000002UL    /* Mask clock PLL lock event    */
#define SYS_MASK_MESYNCR        0x00000004UL    /* Mask clock PLL lock event    */
#define SYS_MASK_MAAT           0x00000008UL    /* Mask automatic acknowledge trigger event */
#define SYS_MASK_MTXFRB         0x00000010UL    /* Mask transmit frame begins event */
#define SYS_MASK_MTXPRS         0x00000020UL    /* Mask transmit preamble sent event    */
#define SYS_MASK_MTXPHS         0x00000040UL    /* Mask transmit PHY Header Sent event  */
#define SYS_MASK_MTXFRS         0x00000080UL    /* Mask transmit frame sent event   */
/*offset 8 */
#define SYS_MASK_MRXPRD         0x00000100UL    /* Mask receiver preamble detected event    */
#define SYS_MASK_MRXSFDD        0x00000200UL    /* Mask receiver SFD detected event */
#define SYS_MASK_MLDEDONE       0x00000400UL    /* Mask LDE processing done event   */
#define SYS_MASK_MRXPHD         0x00000800UL    /* Mask receiver PHY header detect event    */
#define SYS_MASK_MRXPHE         0x00001000UL    /* Mask receiver PHY header error event */
#define SYS_MASK_MRXDFR         0x00002000UL    /* Mask receiver data frame ready event */
#define SYS_MASK_MRXFCG         0x00004000UL    /* Mask receiver FCS good event */
#define SYS_MASK_MRXFCE         0x00008000UL    /* Mask receiver FCS error event    */
/*offset 16 */
#define SYS_MASK_MRXRFSL        0x00010000UL    /* Mask receiver Reed Solomon Frame Sync Loss event */
#define SYS_MASK_MRXRFTO        0x00020000UL    /* Mask Receive Frame Wait Timeout event    */
#define SYS_MASK_MLDEERR        0x00040000UL    /* Mask leading edge detection processing error event   */
#define SYS_MASK_MRXOVRR        0x00100000UL    /* Mask Receiver Overrun event  */
#define SYS_MASK_MRXPTO         0x00200000UL    /* Mask Preamble detection timeout event    */
#define SYS_MASK_MGPIOIRQ       0x00400000UL    /* Mask GPIO interrupt event    */
#define SYS_MASK_MSLP2INIT      0x00800000UL    /* Mask SLEEP to INIT event */
/*offset 24*/
#define SYS_MASK_MRFPLLLL       0x01000000UL    /* Mask RF PLL Loosing Lock warning event   */
#define SYS_MASK_MCPLLLL        0x02000000UL    /* Mask Clock PLL Loosing Lock warning event    */
#define SYS_MASK_MRXSFDTO       0x04000000UL    /* Mask Receive SFD timeout event   */
#define SYS_MASK_MHPDWARN       0x08000000UL    /* Mask Half Period Delay Warning event */
#define SYS_MASK_MTXBERR        0x10000000UL    /* Mask Transmit Buffer Error event */
#define SYS_MASK_MAFFREJ        0x20000000UL    /* Mask Automatic Frame Filtering rejection event   */

/****************************************************************************//**
 * @brief Bit definitions for register SYS_STATUS
**/
#define SYS_STATUS_ID           0x0F            /* System event Status Register */
#define SYS_STATUS_LEN          (5)             /* Note 40 bit register */
/*masks */
#define SYS_STATUS_MASK_32      0xFFF7FFFFUL    /* System event Status Register access mask (all unused fields should always be writen as zero) */
/*offset 0 */
#define SYS_STATUS_IRQS         0x00000001UL    /* Interrupt Request Status READ ONLY */
#define SYS_STATUS_CPLOCK       0x00000002UL    /* Clock PLL Lock */
#define SYS_STATUS_ESYNCR       0x00000004UL    /* External Sync Clock Reset */
#define SYS_STATUS_AAT          0x00000008UL    /* Automatic Acknowledge Trigger */
#define SYS_STATUS_TXFRB        0x00000010UL    /* Transmit Frame Begins */
#define SYS_STATUS_TXPRS        0x00000020UL    /* Transmit Preamble Sent */
#define SYS_STATUS_TXPHS        0x00000040UL    /* Transmit PHY Header Sent */
#define SYS_STATUS_TXFRS        0x00000080UL    /* Transmit Frame Sent: This is set when the transmitter has completed the sending of a frame */
/*offset 8 */
#define SYS_STATUS_RXPRD        0x00000100UL    /* Receiver Preamble Detected status */
#define SYS_STATUS_RXSFDD       0x00000200UL    /* Receiver Start Frame Delimiter Detected. */
#define SYS_STATUS_LDEDONE      0x00000400UL    /* LDE processing done */
#define SYS_STATUS_RXPHD        0x00000800UL    /* Receiver PHY Header Detect */
#define SYS_STATUS_RXPHE        0x00001000UL    /* Receiver PHY Header Error */
#define SYS_STATUS_RXDFR        0x00002000UL    /* Receiver Data Frame Ready */
#define SYS_STATUS_RXFCG        0x00004000UL    /* Receiver FCS Good */
#define SYS_STATUS_RXFCE        0x00008000UL    /* Receiver FCS Error */
/*offset 16 */
#define SYS_STATUS_RXRFSL       0x00010000UL    /* Receiver Reed Solomon Frame Sync Loss */
#define SYS_STATUS_RXRFTO       0x00020000UL    /* Receive Frame Wait Timeout */
#define SYS_STATUS_LDEERR       0x00040000UL    /* Leading edge detection processing error */
#define SYS_STATUS_reserved     0x00080000UL    /* bit19 reserved */
#define SYS_STATUS_RXOVRR       0x00100000UL    /* Receiver Overrun */
#define SYS_STATUS_RXPTO        0x00200000UL    /* Preamble detection timeout */
#define SYS_STATUS_GPIOIRQ      0x00400000UL    /* GPIO interrupt */
#define SYS_STATUS_SLP2INIT     0x00800000UL    /* SLEEP to INIT */
/*offset 24 */
#define SYS_STATUS_RFPLL_LL     0x01000000UL    /* RF PLL Losing Lock */
#define SYS_STATUS_CLKPLL_LL    0x02000000UL    /* Clock PLL Losing Lock */
#define SYS_STATUS_RXSFDTO      0x04000000UL    /* Receive SFD timeout */
#define SYS_STATUS_HPDWARN      0x08000000UL    /* Half Period Delay Warning */
#define SYS_STATUS_TXBERR       0x10000000UL    /* Transmit Buffer Error */
#define SYS_STATUS_AFFREJ       0x20000000UL    /* Automatic Frame Filtering rejection */
#define SYS_STATUS_HSRBP        0x40000000UL    /* Host Side Receive Buffer Pointer */
#define SYS_STATUS_ICRBP        0x80000000UL    /* IC side Receive Buffer Pointer READ ONLY */
/*offset 32 */
#define SYS_STATUS_RXRSCS       0x0100000000ULL /* Receiver Reed-Solomon Correction Status */
#define SYS_STATUS_RXPREJ       0x0200000000ULL /* Receiver Preamble Rejection */
#define SYS_STATUS_TXPUTE       0x0400000000ULL /* Transmit power up time error */

#define SYS_STATUS_TXERR        (0x0408)		/* These bits are the 16 high bits of status register TXPUTE and HPDWARN flags */

/* All RX events after a correct packet reception mask. */
#define SYS_STATUS_ALL_RX_GOOD (SYS_STATUS_RXDFR | SYS_STATUS_RXFCG | SYS_STATUS_RXPRD | \
                                SYS_STATUS_RXSFDD | SYS_STATUS_RXPHD | SYS_STATUS_LDEDONE)

/* All double buffer events mask. */
#define SYS_STATUS_ALL_DBLBUFF (SYS_STATUS_RXDFR | SYS_STATUS_RXFCG)

/* All RX errors mask. */
#define SYS_STATUS_ALL_RX_ERR  (SYS_STATUS_RXPHE | SYS_STATUS_RXFCE | SYS_STATUS_RXRFSL | SYS_STATUS_RXSFDTO \
                                | SYS_STATUS_RXRFTO | SYS_STATUS_RXPTO | SYS_STATUS_AFFREJ | SYS_STATUS_LDEERR)

/* All TX events mask. */
#define SYS_STATUS_ALL_TX      (SYS_STATUS_AAT | SYS_STATUS_TXFRB | SYS_STATUS_TXPRS | \
                                SYS_STATUS_TXPHS | SYS_STATUS_TXFRS )


/****************************************************************************//**
 * @brief Bit definitions for register RX_FINFO
**/
#define RX_FINFO_ID             0x10            /* RX Frame Information (in double buffer set) */
#define RX_FINFO_LEN            (4)
/*mask and shift */
#define RX_FINFO_MASK_32        0xFFFFFBFFUL    /* System event Status Register access mask (all unused fields should always be writen as zero) */
#define RX_FINFO_RXFLEN_MASK    0x0000007FUL    /* Receive Frame Length (0 to 127) */
#define RX_FINFO_RXFLE_MASK     0x00000380UL    /* Receive Frame Length Extension (0 to 7)<<7 */
#define RX_FINFO_RXFL_MASK_1023 0x000003FFUL    /* Receive Frame Length Extension (0 to 1023) */

#define RX_FINFO_RXNSPL_MASK    0x00001800UL    /* Receive Non-Standard Preamble Length */
#define RX_FINFO_RXPSR_MASK     0x000C0000UL    /* RX Preamble Repetition. 00 = 16 symbols, 01 = 64 symbols, 10 = 1024 symbols, 11 = 4096 symbols */

#define RX_FINFO_RXPEL_MASK     0x000C1800UL    /* Receive Preamble Length = RXPSR+RXNSPL */
#define RX_FINFO_RXPEL_64       0x00040000UL    /* Receive Preamble length = 64 */
#define RX_FINFO_RXPEL_128      0x00040800UL    /* Receive Preamble length = 128 */
#define RX_FINFO_RXPEL_256      0x00041000UL    /* Receive Preamble length = 256 */
#define RX_FINFO_RXPEL_512      0x00041800UL    /* Receive Preamble length = 512 */
#define RX_FINFO_RXPEL_1024     0x00080000UL    /* Receive Preamble length = 1024 */
#define RX_FINFO_RXPEL_1536     0x00080800UL    /* Receive Preamble length = 1536 */
#define RX_FINFO_RXPEL_2048     0x00081000UL    /* Receive Preamble length = 2048 */
#define RX_FINFO_RXPEL_4096     0x000C0000UL    /* Receive Preamble length = 4096 */

#define RX_FINFO_RXBR_MASK      0x00006000UL    /* Receive Bit Rate report. This field reports the received bit rate */
#define RX_FINFO_RXBR_110k      0x00000000UL    /* Received bit rate = 110 kbps */
#define RX_FINFO_RXBR_850k      0x00002000UL    /* Received bit rate = 850 kbps */
#define RX_FINFO_RXBR_6M        0x00004000UL    /* Received bit rate = 6.8 Mbps */
#define RX_FINFO_RXBR_SHIFT     (13)

#define RX_FINFO_RNG            0x00008000UL    /* Receiver Ranging. Ranging bit in the received PHY header identifying the frame as a ranging packet. */
#define RX_FINFO_RNG_SHIFT      (15)

#define RX_FINFO_RXPRF_MASK     0x00030000UL    /* RX Pulse Repetition Rate report */
#define RX_FINFO_RXPRF_16M      0x00010000UL    /* PRF being employed in the receiver = 16M */
#define RX_FINFO_RXPRF_64M      0x00020000UL    /* PRF being employed in the receiver = 64M */
#define RX_FINFO_RXPRF_SHIFT    (16)

#define RX_FINFO_RXPACC_MASK    0xFFF00000UL    /* Preamble Accumulation Count */
#define RX_FINFO_RXPACC_SHIFT   (20)

/****************************************************************************//**
 * @brief Bit definitions for register ACK_RESP_T
**/
/* Acknowledge (31:24 preamble symbol delay before auto ACK is sent) and respose (19:0 - unit 1us) timer */
#define ACK_RESP_T_ID           0x1A            /* Acknowledgement Time and Response Time */
#define ACK_RESP_T_LEN          (4)
/*mask and shift */
#define ACK_RESP_T_MASK         0xFF0FFFFFUL    /* Acknowledgement Time and Response access mask */
#define ACK_RESP_T_W4R_TIM_MASK 0x000FFFFFUL    /* Wait-for-Response turn-around Time 20 bit field */
#define W4R_TIM_MASK            ACK_RESP_T_W4R_TIM_MASK
#define ACK_RESP_T_ACK_TIM_MASK 0xFF000000UL    /* Auto-Acknowledgement turn-around Time */
#define ACK_TIM_MASK            ACK_RESP_T_ACK_TIM_MASK

/****************************************************************************//**
 * @brief Bit definitions for register 0x1B 0x1C
**/
#define REG_1B_ID_RESERVED      0x1B
#define REG_1C_ID_RESERVED      0x1C

/****************************************************************************//**
 * @brief Bit definitions for register RX_SNIFF
 *        Sniff Mode Configuration or Pulsed Preamble Reception Configuration
**/
#define RX_SNIFF_ID             0x1D            /* Sniff Mode Configuration */
#define RX_SNIFF_LEN            (4)
/*mask and shift */
#define RX_SNIFF_MASK           0x0000FF0FUL    /*  */
#define RX_SNIFF_SNIFF_ONT_MASK 0x0000000FUL    /* SNIFF Mode ON time. Specified in units of PAC */
#define SNIFF_ONT_MASK          RX_SNIFF_SNIFF_ONT_MASK
#define RX_SNIFF_SNIFF_OFFT_MASK 0x0000FF00UL   /* SNIFF Mode OFF time specified in units of approximately 1mkS, or 128 system clock cycles.*/
#define SNIFF_OFFT_MASK         RX_SNIFF_SNIFF_OFFT_MASK

/****************************************************************************//**
 * @brief Bit definitions for register TX_POWER
**/
#define TX_POWER_ID             0x1E            /* TX Power Control */
#define TX_POWER_LEN            (4)
/*mask and shift definition for Smart Transmit Power Control*/
#define TX_POWER_BOOSTNORM_MASK 0x00000000UL    /* This is the normal power setting used for frames that do not fall */
#define BOOSTNORM_MASK          TX_POWER_BOOSTNORM_MASK
#define TX_POWER_BOOSTNORM_SHIFT (0)
#define TX_POWER_BOOSTP500_MASK 0x00000000UL    /* This value sets the power applied during transmission at the 6.8 Mbps data rate frames that are less than 0.5 ms duration */
#define BOOSTP500_MASK          TX_POWER_BOOSTP500_MASK
#define TX_POWER_BOOSTP500_SHIFT (8)
#define TX_POWER_BOOSTP250_MASK 0x00000000UL    /* This value sets the power applied during transmission at the 6.8 Mbps data rate frames that are less than 0.25 ms duration */
#define BOOSTP250_MASK          TX_POWER_BOOSTP250_MASK
#define TX_POWER_BOOSTP250_SHIFT (16)
#define TX_POWER_BOOSTP125_MASK 0x00000000UL    /* This value sets the power applied during transmission at the 6.8 Mbps data rate frames that are less than 0.125 ms */
#define BOOSTP125_MASK          TX_POWER_BOOSTP125_MASK
#define TX_POWER_BOOSTP125_SHIFT (24)
/*mask and shift definition for Manual Transmit Power Control (DIS_STXP=1 in SYS_CFG)*/
#define TX_POWER_MAN_DEFAULT    0x0E080222UL
#define TX_POWER_TXPOWPHR_MASK  0x0000FF00UL    /* This power setting is applied during the transmission of the PHY header (PHR) portion of the frame. */
#define TX_POWER_TXPOWSD_MASK   0x00FF0000UL    /* This power setting is applied during the transmission of the synchronisation header (SHR) and data portions of the frame. */


/****************************************************************************//**
 * @brief Bit definitions for register CHAN_CTRL
**/
#define CHAN_CTRL_ID            0x1F            /* Channel Control */
#define CHAN_CTRL_LEN           (4)
/*mask and shift */
#define CHAN_CTRL_MASK          0xFFFF00FFUL    /* Channel Control Register access mask */
#define CHAN_CTRL_TX_CHAN_MASK  0x0000000FUL    /* Supported channels are 1, 2, 3, 4, 5, and 7.*/
#define CHAN_CTRL_TX_CHAN_SHIFT (0)             /* Bits 0..3        TX channel number 0-15 selection */

#define CHAN_CTRL_RX_CHAN_MASK  0x000000F0UL
#define CHAN_CTRL_RX_CHAN_SHIFT (4)             /* Bits 4..7        RX channel number 0-15 selection */

#define CHAN_CTRL_RXFPRF_MASK   0x000C0000UL    /* Bits 18..19      Specify (Force) RX Pulse Repetition Rate: 00 = 4 MHz, 01 = 16 MHz, 10 = 64MHz. */
#define CHAN_CTRL_RXFPRF_SHIFT  (18)
/* Specific RXFPRF configuration */
#define CHAN_CTRL_RXFPRF_4      0x00000000UL    /* Specify (Force) RX Pulse Repetition Rate: 00 = 4 MHz, 01 = 16 MHz, 10 = 64MHz. */
#define CHAN_CTRL_RXFPRF_16     0x00040000UL    /* Specify (Force) RX Pulse Repetition Rate: 00 = 4 MHz, 01 = 16 MHz, 10 = 64MHz. */
#define CHAN_CTRL_RXFPRF_64     0x00080000UL    /* Specify (Force) RX Pulse Repetition Rate: 00 = 4 MHz, 01 = 16 MHz, 10 = 64MHz. */
#define CHAN_CTRL_TX_PCOD_MASK  0x07C00000UL    /* Bits 22..26      TX Preamble Code selection, 1 to 24. */
#define CHAN_CTRL_TX_PCOD_SHIFT (22)
#define CHAN_CTRL_RX_PCOD_MASK  0xF8000000UL    /* Bits 27..31      RX Preamble Code selection, 1 to 24. */
#define CHAN_CTRL_RX_PCOD_SHIFT (27)
/*offset 16 */
#define CHAN_CTRL_DWSFD         0x00020000UL    /* Bit 17 This bit enables a non-standard DecaWave proprietary SFD sequence. */
#define CHAN_CTRL_DWSFD_SHIFT   (17)
#define CHAN_CTRL_TNSSFD        0x00100000UL    /* Bit 20 Non-standard SFD in the transmitter */
#define CHAN_CTRL_TNSSFD_SHIFT  (20)
#define CHAN_CTRL_RNSSFD        0x00200000UL    /* Bit 21 Non-standard SFD in the receiver */
#define CHAN_CTRL_RNSSFD_SHIFT  (21)

/****************************************************************************//**
 * @brief Bit definitions for register AGC_CTRL
 * Please take care to write to this register as doing so may cause the DW1000 to malfunction
**/
#define AGC_CTRL_ID             0x23            /* Automatic Gain Control configuration */
#define AGC_CTRL_LEN            (32)
#define AGC_CFG_STS_ID          AGC_CTRL_ID
/* offset from AGC_CTRL_ID in bytes */
#define AGC_CTRL1_OFFSET        (0x02)
#define AGC_CTRL1_LEN           (2)
#define AGC_CTRL1_MASK          0x0001          /* access mask to AGC configuration and control register */
#define AGC_CTRL1_DIS_AM        0x0001          /* Disable AGC Measurement. The DIS_AM bit is set by default. */
/* offset from AGC_CTRL_ID in bytes */
/* Please take care not to write other values to this register as doing so may cause the DW1000 to malfunction */
#define AGC_TUNE1_OFFSET        (0x04)
#define AGC_TUNE1_LEN           (2)
#define AGC_TUNE1_MASK          0xFFFF          /* It is a 16-bit tuning register for the AGC. */
#define AGC_TUNE1_16M           0x8870
#define AGC_TUNE1_64M           0x889B
/* offset from AGC_CTRL_ID in bytes */
/* Please take care not to write other values to this register as doing so may cause the DW1000 to malfunction */
#define AGC_TUNE2_OFFSET        (0x0C)
#define AGC_TUNE2_LEN           (4)
#define AGC_TUNE2_MASK          0xFFFFFFFFUL
#define AGC_TUNE2_VAL           0X2502A907UL
/* offset from AGC_CTRL_ID in bytes */
/* Please take care not to write other values to this register as doing so may cause the DW1000 to malfunction */
#define AGC_TUNE3_OFFSET        (0x12)
#define AGC_TUNE3_LEN           (2)
#define AGC_TUNE3_MASK          0xFFFF
#define AGC_TUNE3_VAL           0X0055
/* offset from AGC_CTRL_ID in bytes */
#define AGC_STAT1_OFFSET        (0x1E)
#define AGC_STAT1_LEN           (3)
#define AGC_STAT1_MASK          0x0FFFFF
#define AGC_STAT1_EDG1_MASK     0x0007C0        /* This 5-bit gain value relates to input noise power measurement. */
#define AGC_STAT1_EDG2_MASK     0x0FF800        /* This 9-bit value relates to the input noise power measurement. */

/****************************************************************************//**
 * @brief Bit definitions for register EXT_SYNC
**/
#define EXT_SYNC_ID             0x24            /* External synchronisation control */
#define EXT_SYNC_LEN            (12)
/* offset from EXT_SYNC_ID in bytes */
#define EC_CTRL_OFFSET          (0x00)
#define EC_CTRL_LEN             (4)
#define EC_CTRL_MASK            0x00000FFBUL    /* sub-register 0x00 is the External clock synchronisation counter configuration register */
#define EC_CTRL_OSTSM           0x00000001UL    /* External transmit synchronisation mode enable */
#define EC_CTRL_OSRSM           0x00000002UL    /* External receive synchronisation mode enable */
#define EC_CTRL_PLLLCK          0x04            /* PLL lock detect enable */
#define EC_CTRL_OSTRM           0x00000800UL    /* External timebase reset mode enable */
#define EC_CTRL_WAIT_MASK       0x000007F8UL    /* Wait counter used for external transmit synchronisation and external timebase reset */
/* offset from EXT_SYNC_ID in bytes */
#define EC_RXTC_OFFSET          (0x04)
#define EC_RXTC_LEN             (4)
#define EC_RXTC_MASK            0xFFFFFFFFUL    /* External clock synchronisation counter captured on RMARKER */
/* offset from EXT_SYNC_ID in bytes */
#define EC_GOLP                 (0x08)
#define EC_GOLP_LEN             (4)
#define EC_GOLP_MASK            0x0000003FUL    /* sub-register 0x08 is the External clock offset to first path 1 GHz counter, EC_GOLP */
#define EC_GOLP_OFFSET_EXT_MASK 0x0000003FUL    /* This register contains the 1 GHz count from the arrival of the RMARKER and the next edge of the external clock. */

/****************************************************************************//**
 * @brief Bit definitions for register  DRX_CONF
 * Digital Receiver configuration block
**/
#define DRX_CONF_ID             0x27            /* Digital Receiver configuration */
#define DRX_CONF_LEN            (44)
/* offset from DRX_CONF_ID in bytes */
#define DRX_TUNE0b_OFFSET       (0x02)  /* sub-register 0x02 is a 16-bit tuning register. */
#define DRX_TUNE0b_LEN          (2)
#define DRX_TUNE0b_MASK         0xFFFF  /* 7.2.40.2 Sub-Register 0x27:02 ֠DRX_TUNE0b */

/* offset from DRX_CONF_ID in bytes */
#define DRX_TUNE1a_OFFSET       0x04    /* 7.2.40.3 Sub-Register 0x27:04 ֠DRX_TUNE1a */
#define DRX_TUNE1a_LEN          (2)
#define DRX_TUNE1a_MASK         0xFFFF

/* offset from DRX_CONF_ID in bytes */
#define DRX_TUNE1b_OFFSET       0x06    /* 7.2.40.4 Sub-Register 0x27:06 ֠DRX_TUNE1b */
#define DRX_TUNE1b_LEN          (2)
#define DRX_TUNE1b_MASK         0xFFFF

/* offset from DRX_CONF_ID in bytes */
#define DRX_TUNE2_OFFSET        0x08    /* 7.2.40.5 Sub-Register 0x27:08 ֠DRX_TUNE2 */
#define DRX_TUNE2_LEN           (4)
#define DRX_TUNE2_MASK          0xFFFFFFFFUL

/* offset from DRX_CONF_ID in bytes */
/* WARNING: Please do NOT set DRX_SFDTOC to zero (disabling SFD detection timeout)
 * since this risks IC malfunction due to prolonged receiver activity in the event of false preamble detection.
 */
#define DRX_SFDTOC_OFFSET       0x20    /* 7.2.40.7 Sub-Register 0x27:20 ֠DRX_SFDTOC */
#define DRX_SFDTOC_LEN          (2)
#define DRX_SFDTOC_MASK         0xFFFF

/* offset from DRX_CONF_ID in bytes */
#define DRX_PRETOC_OFFSET       0x24    /* 7.2.40.9 Sub-Register 0x27:24 ֠DRX_PRETOC */
#define DRX_PRETOC_LEN          (2)
#define DRX_PRETOC_MASK         0xFFFF

/* offset from DRX_CONF_ID in bytes */
#define DRX_DRX_TUNE4HOFFSET    0x26    /* 7.2.40.10 Sub-Register 0x27:26 ֠DRX_TUNE4H */
#define DRX_DRX_TUNE4H_LEN      (2)
#define DRX_DRX_TUNE4H_MASK     0xFFFF



/****************************************************************************//**
 * @brief Bit definitions for register  RF_CONF
 * Analog RF Configuration block
 * Refer to section 7.2.41 Register file: 0x28 ֠Analog RF configuration block
**/
#define RF_CONF_ID              0x28            /* Analog RF Configuration */
#define RF_CONF_LEN             (58)
#define RF_CONF_TXEN_MASK		0x00400000UL   /* TX enable */
#define RF_CONF_RXEN_MASK		0x00200000UL   /* RX enable */
#define RF_CONF_TXPOW_MASK		0x001F0000UL   /* turn on power all LDOs */
#define RF_CONF_PLLEN_MASK		0x0000E000UL   /* enable PLLs */
#define RF_CONF_TXBLOCKSEN_MASK	0x00001F00UL   /* enable TX blocks */
#define RF_CONF_TXPLLPOWEN_MASK (RF_CONF_PLLEN_MASK | RF_CONF_TXPOW_MASK)
#define RF_CONF_TXALLEN_MASK	(RF_CONF_TXEN_MASK | RF_CONF_TXPOW_MASK | RF_CONF_PLLEN_MASK | RF_CONF_TXBLOCKSEN_MASK)
/* offset from TX_CAL_ID in bytes */
#define RF_RXCTRLH_OFFSET       0x0B    /*  */
/* offset from TX_CAL_ID in bytes */
#define RF_TXCTRL_OFFSET        0x0C    /* Analog TX Control Register */
#define RF_TXCTRL_LEN           (4)
#define RF_TXCTRL_TXMTUNE_MASK  0x000001E0UL    /* Transmit mixer tuning register */
#define RF_TXCTRL_TXTXMQ_MASK   0x00000E00UL    /* Transmit mixer Q-factor tuning register */
#define RF_TXCTRL_CH1           0x00005C40UL    /* 32-bit value to program to Sub-Register 0x28:0C ֠RF_TXCTRL */
#define RF_TXCTRL_CH2           0x00045CA0UL    /* 32-bit value to program to Sub-Register 0x28:0C ֠RF_TXCTRL */
#define RF_TXCTRL_CH3           0x00086CC0UL    /* 32-bit value to program to Sub-Register 0x28:0C ֠RF_TXCTRL */
#define RF_TXCTRL_CH4           0x00045C80UL    /* 32-bit value to program to Sub-Register 0x28:0C ֠RF_TXCTRL */
#define RF_TXCTRL_CH5           0x001E3FE0UL    /* 32-bit value to program to Sub-Register 0x28:0C ֠RF_TXCTRL */
#define RF_TXCTRL_CH7           0x001E7DE0UL    /* 32-bit value to program to Sub-Register 0x28:0C ֠RF_TXCTRL */

/* offset from TX_CAL_ID in bytes */
#define RF_STATUS_OFFSET        0x2C    /*  */

/****************************************************************************//**
 * @brief Bit definitions for register TX_CAL
 * Refer to section 7.2.43 Register file: 0x2A ֠Transmitter Calibration block
**/
#define TX_CAL_ID               0x2A            /* Transmitter calibration block */
#define TX_CAL_LEN              (52)
/* offset from TX_CAL_ID in bytes */
#define TC_SARL_SAR_C		        (0)         /* SAR control */
//#define TC_SARL_OFFSET            0x03            /* Transmitter Calibration ֠Latest SAR readings. RO */
//#define TC_SARL_LEN               (2)
/*cause bug in register block TX_CAL, we need to read 1 byte in a time*/
#define TC_SARL_SAR_LVBAT_OFFSET    (3)         /* Latest SAR reading for Voltage level */
#define TC_SARL_SAR_LTEMP_OFFSET    (4)         /* Latest SAR reading for Temperature level */
/* offset from TX_CAL_ID in bytes */
//#define TC_SARW_OFFSET            0x06            /* Transmitter Calibration ֠SAR readings at last Wake-Up */
//#define TC_SARW_LEN           (2)
#define TC_SARW_SAR_WTEMP_OFFSET    0x06            /* SAR reading of Temperature level taken at last wakeup event */
#define TC_SARW_SAR_WVBAT_OFFSET    0x07            /* SAR reading of Voltage level taken at last wakeup event */
/* offset from TX_CAL_ID in bytes */
#define TC_PGDELAY_OFFSET       0x0B            /* Transmitter Calibration ֠Pulse Generator Delay */
#define TC_PGDELAY_LEN          (1)
#define TC_PGDELAY_CH1          0xC9            /* Recommended value for channel 1 */
#define TC_PGDELAY_CH2          0xC2            /* Recommended value for channel 2 */
#define TC_PGDELAY_CH3          0xC5            /* Recommended value for channel 3 */
#define TC_PGDELAY_CH4          0x95            /* Recommended value for channel 4 */
#define TC_PGDELAY_CH5          0xC0            /* Recommended value for channel 5 */
#define TC_PGDELAY_CH7          0x93            /* Recommended value for channel 7 */
/* offset from TX_CAL_ID in bytes */
#define TC_PGTEST_OFFSET        0x0C            /* Transmitter Calibration ֠Pulse Generator Test */
#define TC_PGTEST_LEN           (1)
#define TC_PGTEST_NORMAL        0x00            /* Normal operation */
#define TC_PGTEST_CW            0x13            /* Continuous Wave (CW) Test Mode */

/****************************************************************************//**
 * @brief Bit definitions for register
 * Refer to section 7.2.44 Register file: 0x2B ֠Frequency synthesiser control block
**/
#define FS_CTRL_ID              0x2B            /* Frequency synthesiser control block */
#define FS_CTRL_LEN             (21)
/* offset from FS_CTRL_ID in bytes */
#define FS_RES1_OFFSET          0x00            /* reserved area. Please take care not to write to this area as doing so may cause the DW1000 to malfunction. */
#define FS_RES1_LEN             (7)
/* offset from FS_CTRL_ID in bytes */
#define FS_PLLCFG_OFFSET        0x07            /* Frequency synthesiser ֠PLL configuration */
#define FS_PLLCFG_LEN           (5)
#define FS_PLLCFG_CH1           0x09000407UL    /* Operating Channel 1 */
#define FS_PLLCFG_CH2           0x08400508UL    /* Operating Channel 2 (same as 4) */
#define FS_PLLCFG_CH3           0x08401009UL    /* Operating Channel 3 */
#define FS_PLLCFG_CH4           0x08400508UL    /* Operating Channel 4 (same as 2) */
#define FS_PLLCFG_CH5           0x0800041DUL    /* Operating Channel 5 (same as 7) */
#define FS_PLLCFG_CH7           0x0800041DUL    /* Operating Channel 7 (same as 5) */
/* offset from FS_CTRL_ID in bytes */
#define FS_PLLTUNE_OFFSET       0x0B            /* Frequency synthesiser ֠PLL Tuning */
#define FS_PLLTUNE_LEN          (1)
#define FS_PLLTUNE_CH1          0x1E    /* Operating Channel 1 */
#define FS_PLLTUNE_CH2          0x26    /* Operating Channel 2 (same as 4) */
#define FS_PLLTUNE_CH3          0x5E    /* Operating Channel 3 */
#define FS_PLLTUNE_CH4          0x26    /* Operating Channel 4 (same as 2) */
#define FS_PLLTUNE_CH5          0xA6    /* Operating Channel 5 (same as 7) */
#define FS_PLLTUNE_CH7          0xA6    /* Operating Channel 7 (same as 5) */
/* offset from FS_CTRL_ID in bytes */
#define FS_RES2_OFFSET          0x0C    /* reserved area. Please take care not to write to this area as doing so may cause the DW1000 to malfunction. */
#define FS_RES2_LEN             (2)
/* offset from FS_CTRL_ID in bytes */
#define FS_XTALT_OFFSET         0x0E    /* Frequency synthesiser ֠Crystal trim */
#define FS_XTALT_LEN            (1)
#define FS_XTALT_MASK           0x1F    /* Crystal Trim. Crystals may be trimmed using this register setting to tune out errors, see 8.1 ֠IC Calibration ֠Crystal Oscillator Trim. */
#define FS_XTALT_MIDRANGE       0x10
/* offset from FS_CTRL_ID in bytes */
#define FS_RES3_OFFSET          0x0F    /* reserved area. Please take care not to write to this area as doing so may cause the DW1000 to malfunction. */
#define FS_RES3_LEN             (6)

/****************************************************************************//**
 * @brief Bit definitions for register
**/
#define AON_ID                  0x2C            /* Always-On register set */
#define AON_LEN                 (12)
/* offset from AON_ID in bytes */
#define AON_WCFG_OFFSET         0x00    /* used to control what the DW1000 IC does as it wakes up from low-power SLEEP or DEEPSLEEPstates. */
#define AON_WCFG_LEN            (2)
#define AON_WCFG_MASK           0x09CB  /* access mask to AON_WCFG register*/
#define AON_WCFG_ONW_RADC       0x0001  /* On Wake-up Run the (temperature and voltage) Analog-to-Digital Convertors */
#define AON_WCFG_ONW_RX         0x0002  /* On Wake-up turn on the Receiver */
#define AON_WCFG_ONW_LEUI       0x0008  /* On Wake-up load the EUI from OTP memory into Register file: 0x01 ֠Extended Unique Identifier. */
#define AON_WCFG_ONW_LDC        0x0040  /* On Wake-up load configurations from the AON memory into the host interface register set */
#define AON_WCFG_ONW_L64P       0x0080  /* On Wake-up load the Length64 receiver operating parameter set */
#define AON_WCFG_PRES_SLEEP     0x0100  /* Preserve Sleep. This bit determines what the DW1000 does with respect to the ARXSLP and ATXSLP sleep controls */
#define AON_WCFG_ONW_LLDE       0x0800  /* On Wake-up load the LDE microcode. */
#define AON_WCFG_ONW_LLDO       0x1000  /* On Wake-up load the LDO tune value. */
/* offset from AON_ID in bytes */
#define AON_CTRL_OFFSET         0x02    /* The bits in this register in general cause direct activity within the AON block with respect to the stored AON memory */
#define AON_CTRL_LEN            (1)
#define AON_CTRL_MASK           0x8F    /* access mask to AON_CTRL register */
#define AON_CTRL_RESTORE        0x01    /* When this bit is set the DW1000 will copy the user configurations from the AON memory to the host interface register set. */
#define AON_CTRL_SAVE           0x02    /* When this bit is set the DW1000 will copy the user configurations from the host interface register set into the AON memory */
#define AON_CTRL_UPL_CFG        0x04    /* Upload the AON block configurations to the AON  */
#define AON_CTRL_DCA_READ       0x08    /* Direct AON memory access read */
#define AON_CTRL_DCA_ENAB       0x80    /* Direct AON memory access enable bit */
/* offset from AON_ID in bytes */
#define AON_RDAT_OFFSET         0x03    /* AON Direct Access Read Data Result */
#define AON_RDAT_LEN            (1)
/* offset from AON_ID in bytes */
#define AON_ADDR_OFFSET         0x04    /* AON Direct Access Address */
#define AON_ADDR_LEN            (1)

/* offset from AON_ID in bytes */
#define AON_CFG0_OFFSET         0x06    /* 32-bit configuration register for the always on block. */
#define AON_CFG0_LEN            (4)
#define AON_CFG0_SLEEP_EN           0x00000001UL    /* This is the sleep enable configuration bit */
#define AON_CFG0_WAKE_PIN           0x00000002UL    /* Wake using WAKEUP pin */
#define AON_CFG0_WAKE_SPI           0x00000004UL    /* Wake using SPI access SPICSn */
#define AON_CFG0_WAKE_CNT           0x00000008UL    /* Wake when sleep counter elapses */
#define AON_CFG0_LPDIV_EN           0x00000010UL    /* Low power divider enable configuration */
#define AON_CFG0_LPCLKDIVA_MASK     0x0000FFE0UL    /* divider count for dividing the raw DW1000 XTAL oscillator frequency to set an LP clock frequency */
#define AON_CFG0_LPCLKDIVA_SHIFT    (5)
#define AON_CFG0_SLEEP_TIM          0xFFFF0000UL    /* Sleep time. This field configures the sleep time count elapse value */
#define AON_CFG0_SLEEP_SHIFT        (16)
/* offset from AON_ID in bytes */
#define AON_CFG1_OFFSET         0x0A
#define AON_CFG1_LEN            (2)
#define AON_CFG1_MASK           0x0007  /* aceess mask to AON_CFG1 */
#define AON_CFG1_SLEEP_CEN      0x0001  /* This bit enables the sleep counter */
#define AON_CFG1_SMXX           0x0002  /* This bit needs to be set to 0 for correct operation in the SLEEP state within the DW1000 */
#define AON_CFG1_LPOSC_CAL      0x0004  /* This bit enables the calibration function that measures the period of the ICӳ internal low powered oscillator */

/****************************************************************************//**
 * @brief Bit definitions for register OTP_IF
 * Refer to section 7.2.46 Register file: 0x2D ֠OTP Memory Interface
**/
#define OTP_IF_ID               0x2D            /* One Time Programmable Memory Interface */
#define OTP_IF_LEN              (18)
/* offset from OTP_IF_ID in bytes */
#define OTP_WDAT                0x00            /* 32-bit register. The data value to be programmed into an OTP location  */
#define OTP_WDAT_LEN            (4)
/* offset from OTP_IF_ID in bytes */
#define OTP_ADDR                0x04            /* 16-bit register used to select the address within the OTP memory block */
#define OTP_ADDR_LEN            (2)
#define OTP_ADDR_MASK           0x07FF          /* This 11-bit field specifies the address within OTP memory that will be accessed read or written. */
/* offset from OTP_IF_ID in bytes */
#define OTP_CTRL                0x06            /* used to control the operation of the OTP memory */
#define OTP_CTRL_LEN            (2)
#define OTP_CTRL_MASK           0x8002
#define OTP_CTRL_OTPRDEN		0x0001			/* This bit forces the OTP into manual read mode */
#define OTP_CTRL_OTPREAD        0x0002          /* This bit commands a read operation from the address specified in the OTP_ADDR register */
#define OTP_CTRL_LDELOAD        0x8000          /* This bit forces a load of LDE microcode */
#define OTP_CTRL_OTPPROG        0x0040          /* Setting this bit will cause the contents of OTP_WDAT to be written to OTP_ADDR. */
/* offset from OTP_IF_ID in bytes */
#define OTP_STAT                0x08
#define OTP_STAT_LEN            (2)
#define OTP_STAT_MASK           0x0003
#define OTP_STAT_OTPPRGD        0x0001          /* OTP Programming Done */
//#define OTP_STAT_OTPVLTOK     0x0002          /* OTP Programming Voltage OK */   !!!!!!!!!!!!
/* offset from OTP_IF_ID in bytes */
#define OTP_RDAT                0x0A            /* 32-bit register. The data value read from an OTP location will appear here */
#define OTP_RDAT_LEN            (4)
/* offset from OTP_IF_ID in bytes */
#define OTP_SRDAT               0x0E            /* 32-bit register. The data value stored in the OTP SR (0x400) location will appear here after power up */
#define OTP_SRDAT_LEN           (4)
/* offset from OTP_IF_ID in bytes */
#define OTP_SF                  0x12            /*8-bit special function register used to select and load special receiver operational parameter */
#define OTP_SF_LEN              (1)
#define OTP_SF_MASK             0x63
#define OTP_SF_OPS_KICK         0x01            /* This bit when set initiates a load of the operating parameter set selected by the OPS_SEL */
#define OTP_SF_LDO_KICK         0x02            /* This bit when set initiates a load of the LDO tune code */
#define OTP_SF_OPS_SEL_L64      0x00            /* Operating parameter set selection: Length64 */
#define OTP_SF_OPS_SEL_TIGHT    0x40            /* Operating parameter set selection: Tight */

/****************************************************************************//**
 * @brief Bit definitions for register LDE_IF
 * Refer to section 7.2.47 Register file: 0x2E ֠Leading Edge Detection Interface
 * PLEASE NOTE: Other areas within the address space of Register file: 0x2E ֠Leading Edge Detection Interface
 * are reserved. To ensure proper operation of the LDE algorithm (i.e. to avoid loss of performance or a malfunction),
 * care must be taken not to write to any byte locations other than those defined in the sub-sections below.
**/
#define LDE_IF_ID               0x2E            /* Leading edge detection control block */
#define LDE_IF_LEN              (0)
/* offset from LDE_IF_ID in bytes */
#define LDE_THRESH_OFFSET       0x0000  /* 16-bit status register reporting the threshold that was used to find the first path */
#define LDE_THRESH_LEN          (2)
/* offset from LDE_IF_ID in bytes */
#define LDE_CFG1_OFFSET         0x0806  /*8-bit configuration register*/
#define LDE_CFG1_LEN            (1)
#define LDE_CFG1_NSTDEV_MASK    0x1F    /* Number of Standard Deviations mask. */
#define LDE_CFG1_PMULT_MASK     0xE0    /* Peak Multiplier mask. */
/* offset from LDE_IF_ID in bytes */
#define LDE_PPINDX_OFFSET       0x1000  /* reporting the position within the accumulator that the LDE algorithm has determined to contain the maximum */
#define LDE_PPINDX_LEN          (2)
/* offset from LDE_IF_ID in bytes */
#define LDE_PPAMPL_OFFSET       0x1002  /* reporting the magnitude of the peak signal seen in the accumulator data memory */
#define LDE_PPAMPL_LEN          (2)
/* offset from LDE_IF_ID in bytes */
#define LDE_RXANTD_OFFSET       0x1804  /* 16-bit configuration register for setting the receive antenna delay */
#define LDE_RXANTD_LEN          (2)
/* offset from LDE_IF_ID in bytes */
#define LDE_CFG2_OFFSET         0x1806  /* 16-bit LDE configuration tuning register */
#define LDE_CFG2_LEN            (2)
/* offset from LDE_IF_ID in bytes */
#define LDE_REPC_OFFSET         0x2804  /* 16-bit configuration register for setting the replica avoidance coefficient */
#define LDE_REPC_LEN            (2)

/****************************************************************************//**
 * @brief Bit definitions for register DIG_DIAG
 * Digital Diagnostics interface.
 * It contains a number of sub-registers that give diagnostics information.
**/
#define DIG_DIAG_ID             0x2F        /* Digital Diagnostics Interface */
#define DIG_DIAG_LEN            (41)

/* offset from DIG_DIAG_ID in bytes */
#define EVC_CTRL_OFFSET         0x00        /* Event Counter Control */
#define EVC_CTRL_LEN            (4)
#define EVC_CTRL_MASK           0x00000003UL/* access mask to Register for bits should always be set to zero to avoid any malfunction of the device. */
#define EVC_EN                  0x00000001UL/* Event Counters Enable bit */
#define EVC_CLR                 0x00000002UL

/* offset from DIG_DIAG_ID in bytes */
#define EVC_PHE_OFFSET          0x04        /* PHR Error Event Counter */
#define EVC_PHE_LEN             (2)
#define EVC_PHE_MASK            0x0FFF
/* offset from DIG_DIAG_ID in bytes */
#define EVC_RSE_OFFSET          0x06        /* Reed Solomon decoder (Frame Sync Loss) Error Event Counter */
#define EVC_RSE_LEN             (2)
#define EVC_RSE_MASK            0x0FFF

/* offset from DIG_DIAG_ID in bytes */
#define EVC_FCG_OFFSET          0x08        /* The EVC_FCG field is a 12-bit counter of the frames received with good CRC/FCS sequence. */
#define EVC_FCG_LEN             (2)
#define EVC_FCG_MASK            0x0FFF
/* offset from DIG_DIAG_ID in bytes */
#define EVC_FCE_OFFSET          0x0A        /* The EVC_FCE field is a 12-bit counter of the frames received with bad CRC/FCS sequence. */
#define EVC_FCE_LEN             (2)
#define EVC_FCE_MASK            0x0FFF

/* offset from DIG_DIAG_ID in bytes */
#define EVC_FFR_OFFSET          0x0C        /* The EVC_FFR field is a 12-bit counter of the frames rejected by the receive frame filtering function. */
#define EVC_FFR_LEN             (2)
#define EVC_FFR_MASK            0x0FFF
/* offset from DIG_DIAG_ID in bytes */
#define EVC_OVR_OFFSET          0x0E        /* The EVC_OVR field is a 12-bit counter of receive overrun events */
#define EVC_OVR_LEN             (2)
#define EVC_OVR_MASK            0x0FFF

/* offset from DIG_DIAG_ID in bytes */
#define EVC_STO_OFFSET          0x10        /* The EVC_STO field is a 12-bit counter of SFD Timeout Error events */
#define EVC_OVR_LEN             (2)
#define EVC_OVR_MASK            0x0FFF
/* offset from DIG_DIAG_ID in bytes */
#define EVC_PTO_OFFSET          0x12        /* The EVC_PTO field is a 12-bit counter of Preamble detection Timeout events */
#define EVC_PTO_LEN             (2)
#define EVC_PTO_MASK            0x0FFF

/* offset from DIG_DIAG_ID in bytes */
#define EVC_FWTO_OFFSET         0x14        /* The EVC_FWTO field is a 12-bit counter of receive frame wait timeout events */
#define EVC_FWTO_LEN            (2)
#define EVC_FWTO_MASK           0x0FFF
/* offset from DIG_DIAG_ID in bytes */
#define EVC_TXFS_OFFSET         0x16        /* The EVC_TXFS field is a 12-bit counter of transmit frames sent. This is incremented every time a frame is sent */
#define EVC_TXFS_LEN            (2)
#define EVC_TXFS_MASK           0x0FFF

/* offset from DIG_DIAG_ID in bytes */
#define EVC_HPW_OFFSET          0x18        /* The EVC_HPW field is a 12-bit counter of Ԉalf Period WarningsԮ */
#define EVC_HPW_LEN             (2)
#define EVC_HPW_MASK            0x0FFF
/* offset from DIG_DIAG_ID in bytes */
#define EVC_TPW_OFFSET          0x1A        /* The EVC_TPW field is a 12-bit counter of Ԕransmitter Power-Up WarningsԮ */
#define EVC_TPW_LEN             (2)
#define EVC_TPW_MASK            0x0FFF

/* offset from DIG_DIAG_ID in bytes */
#define EVC_RES1_OFFSET         0x1C        /* Please take care not to write to this register as doing so may cause the DW1000 to malfunction. */

/* offset from DIG_DIAG_ID in bytes */
#define DIAG_TMC_OFFSET         0x24
#define DIAG_TMC_LEN            (2)
#define DIAG_TMC_MASK           0x0010
#define DIAG_TMC_TX_PSTM        0x0010      /* This test mode is provided to help support regulatory approvals spectral testing. When the TX_PSTM bit is set it enables a repeating transmission of the data from the TX_BUFFER */


/****************************************************************************//**
 * @brief Bit definitions for register PMSC
**/
#define PMSC_ID                 0x36            /* Power Management System Control Block */
#define PMSC_LEN                (48)
/* offset from PMSC_ID in bytes */
#define PMSC_CTRL0_OFFSET       0x00
#define PMSC_CTRL0_LEN          (4)
#define PMSC_CTRL0_MASK         0xF08F847FUL    /* access mask to register PMSC_CTRL0 */
#define PMSC_CTRL0_SYSCLKS_AUTO 0x00000000UL    /* The system clock will run off the 19.2 MHz XTI clock until the PLL is calibrated and locked, then it will switch over the 125 MHz PLL clock */
#define PMSC_CTRL0_SYSCLKS_19M  0x00000001UL    /* Force system clock to be the 19.2 MHz XTI clock. */
#define PMSC_CTRL0_SYSCLKS_125M 0x00000002UL    /* Force system clock to the 125 MHz PLL clock. */
#define PMSC_CTRL0_RXCLKS_AUTO  0x00000000UL    /* The RX clock will be disabled until it is required for an RX operation */
#define PMSC_CTRL0_RXCLKS_19M   0x00000004UL    /* Force RX clock enable and sourced clock from the 19.2 MHz XTI clock */
#define PMSC_CTRL0_RXCLKS_125M  0x00000008UL    /* Force RX clock enable and sourced from the 125 MHz PLL clock */
#define PMSC_CTRL0_RXCLKS_OFF   0x0000000CUL    /* Force RX clock off. */
#define PMSC_CTRL0_TXCLKS_AUTO  0x00000000UL    /* The TX clock will be disabled until it is required for a TX operation */
#define PMSC_CTRL0_TXCLKS_19M   0x00000010UL    /* Force TX clock enable and sourced clock from the 19.2 MHz XTI clock */
#define PMSC_CTRL0_TXCLKS_125M  0x00000020UL    /* Force TX clock enable and sourced from the 125 MHz PLL clock */
#define PMSC_CTRL0_TXCLKS_OFF   0x00000030UL    /* Force TX clock off */
#define PMSC_CTRL0_FACE         0x00000040UL    /* Force Accumulator Clock Enable */
/* offset from PMSC_ID in bytes */
#define PMSC_CTRL1_OFFSET       0x04
#define PMSC_CTRL1_LEN          (4)
#define PMSC_CTRL1_MASK         0xFC02F802UL    /* access mask to register PMSC_CTRL1 */
#define PMSC_CTRL1_ARX2INIT     0x00000002UL    /* Automatic transition from receive mode into the INIT state */
#define PMSC_CTRL1_ATXSLP       0x00000800UL    /* If this bit is set then the DW1000 will automatically transition into SLEEP or DEEPSLEEP mode after transmission of a frame */
#define PMSC_CTRL1_ARXSLP       0x00001000UL    /* this bit is set then the DW1000 will automatically transition into SLEEP mode after a receive attempt */
#define PMSC_CTRL1_SNOZE        0x00002000UL    /* Snooze Enable */
#define PMSC_CTRL1_SNOZR        0x00004000UL    /* The SNOZR bit is set to allow the snooze timer to repeat twice */
#define PMSC_CTRL1_PLLSYN       0x00008000UL    /* This enables a special 1 GHz clock used for some external SYNC modes */
#define PMSC_CTRL1_LDERUNE      0x00020000UL    /* This bit enables the running of the LDE algorithm */
#define PMSC_CTRL1_KHZCLKDIV_MASK   0xFC000000UL    /* Kilohertz clock divisor */
#define PMSC_CTRL1_PKTSEQ_DISABLE   0x00		/* writing this to PMSC CONTROL 1 register (bits 10-3) disables PMSC control of analog RF subsystems */
#define PMSC_CTRL1_PKTSEQ_ENABLE    0xE7		/* writing this to PMSC CONTROL 1 register (bits 10-3) enables PMSC control of analog RF subsystems */
/* offset from PMSC_ID in bytes */
#define PMSC_RES1_OFFSET        0x08
/* offset from PMSC_ID in bytes */
#define PMSC_SNOZT_OFFSET       0x0C            /* PMSC Snooze Time Register */
#define PMSC_SNOZT_LEN          (1)
/* offset from PMSC_ID in bytes */
#define PMSC_RES2_OFFSET        0x10
/* offset from PMSC_ID in bytes */
#define PMSC_RES3_OFFSET        0x24
/* offset from PMSC_ID in bytes */
#define PMSC_TXFINESEQ_OFFSET        0x26		/* Writing PMSC_TXFINESEQ_DIS_MASK disables fine grain sequencing in the transmitter*/
#define PMSC_TXFINESEQ_DIS_MASK      (0x0)	
#define PMSC_TXFINESEQ_EN_MASK      (0B74)      /* Writing PMSC_TXFINESEQ_EN_MASK enables fine grain sequencing in the transmitter*/
/* offset from PMSC_ID in bytes */
#define PMSC_LEDC_OFFSET        0x28
#define PMSC_LEDC_LEN           (4)
#define PMSC_LEDC_MASK          0x000001FFUL    /* 32-bit LED control register. */
#define PMSC_LEDC_BLINK_TIM_MASK    0x000000FFUL    /* This field determines how long the LEDs remain lit after an event that causes them to be set on. default 0x20 give 0x20 * 14mS = 400mS */
#define PMSC_LEDC_BLNKEN        0x00000100UL    /* Blink Enable. When this bit is set to 1 the LED blink feature is enabled. */

float64_t dwt_getrangebias( uint8_t chan, float64_t range, uint8_t prf );

#endif
