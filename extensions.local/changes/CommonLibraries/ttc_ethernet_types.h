#ifndef TTC_ETH_TYPES_H
#define TTC_ETH_TYPES_H

/*{ ttc_eth_types.h ***********************************************
 *
 * Written by Gregor Rebel 2010-2012
 *
 * Basic defines, enums and structures.
 *
 * Include this file from 
 * architecture independent header files (ttc_XXX.h) and 
 * architecture depend      header files (e.g. stm32_XXX.h)
 * 
}*/
#include "ttc_basic.h"


//{ Defines/ TypeDefs ****************************************************

// TTC_ETHn has to be defined as constant by makefile.100_board_*
#ifdef TTC_ETH5
  #ifndef TTC_ETH4
    #error TTC_ETH5 is defined, but not TTC_ETH4 - all lower TTC_ETHn must be defined!
  #endif
  #ifndef TTC_ETH3
    #error TTC_ETH5 is defined, but not TTC_ETH3 - all lower TTC_ETHn must be defined!
  #endif
  #ifndef TTC_ETH2
    #error TTC_ETH5 is defined, but not TTC_ETH2 - all lower TTC_ETHn must be defined!
  #endif
  #ifndef TTC_ETH1
    #error TTC_ETH5 is defined, but not TTC_ETH1 - all lower TTC_ETHn must be defined!
  #endif

  #define TTC_AMOUNT_ETHS 5
#else
  #ifdef TTC_ETH4
    #define TTC_AMOUNT_ETHS 4

    #ifndef TTC_ETH3
      #error TTC_ETH5 is defined, but not TTC_ETH3 - all lower TTC_ETHn must be defined!
    #endif
    #ifndef TTC_ETH2
      #error TTC_ETH5 is defined, but not TTC_ETH2 - all lower TTC_ETHn must be defined!
    #endif
    #ifndef TTC_ETH1
      #error TTC_ETH5 is defined, but not TTC_ETH1 - all lower TTC_ETHn must be defined!
    #endif
  #else
    #ifdef TTC_ETH3
    
      #ifndef TTC_ETH2
        #error TTC_ETH5 is defined, but not TTC_ETH2 - all lower TTC_ETHn must be defined!
      #endif
      #ifndef TTC_ETH1
        #error TTC_ETH5 is defined, but not TTC_ETH1 - all lower TTC_ETHn must be defined!
      #endif
    
      #define TTC_AMOUNT_ETHS 3
    #else
      #ifdef TTC_ETH2

        #ifndef TTC_ETH1
          #error TTC_ETH5 is defined, but not TTC_ETH1 - all lower TTC_ETHn must be defined!
        #endif

        #define TTC_AMOUNT_ETHS 2
      #else
        #ifdef TTC_ETH1
          #define TTC_AMOUNT_ETHS 1
        #else
          #define TTC_AMOUNT_ETHS 0
        #endif
      #endif
    #endif
  #endif
#endif

//} Defines
//{ Structures/ Enums 1 **************************************************


typedef enum {     // ttc_eth_errorcode_e
  tee_OK,                                  // =0: no error
  tee_TimeOut,                             // timeout occured in called function
  tee_ReceptionError,                      // packet reception failed
  tee_NotImplemented,                      // function has no implementation for current architecture
  tee_DeviceNotFound,                      // adressed I2C device not available in current uC
  tee_InvalidArgument,                     // general argument error
  tee_DeviceNotReady,                      // choosen device has not been initialized properly
  tee_DescriptorLocked,                    // Descriptor is owned by DMA

  tee_UnknownError
} ttc_eth_errorcode_e;

typedef enum { // ttc_eth_framefilter_e
  tef_None,
  tef_HashTableFilter,
  tef_PerfectFilter,
  tef_PerfectHashTableFilter 
} ttc_eth_framefilter_e;

typedef enum { // ttc_eth_addrfilter_e
  tea_None,
  tea_NormalFilter,
  tea_InverseFilter
} ttc_eth_addrfilter_e;

typedef enum { // ttc_eth_dmaarbitration_e
  ted_RoundRobin_RxTx_1to1,
  ted_RoundRobin_RxTx_2to1,
  ted_RoundRobin_RxTx_3to1,
  ted_RoundRobin_RxTx_4to1,
  ted_RxPriorTx
} ttc_eth_dmaarbitration_e;

typedef enum { // ttc_eth_passcontrol_e
  tep_BlockAll,   // MAC filters all control frames from reaching the application
  tep_ForwardAll, // MAC forwards all control frames to application even if they fail the Address Filter
  tep_ForwardPassedAddrFilter // MAC forwards control frames that pass the Address Filter.
} ttc_eth_passcontrol_e;


typedef struct { // architecture independent configuration data of single ETH
  union  {
    u32_t All;
    struct {
      /* MAC */ 
      unsigned AutoNegotiation   : 1; // =1: enable AutoNegotiation with the external PHY
      unsigned Watchdog          : 1; // =1: enable Watchdog timer
      unsigned Jabber            : 1; // =1: enable Jabber timer
      unsigned CarrierSense      : 1; // =1: enable Carrier Sense
      unsigned ReceiveOwn        : 1; // =1: enable reception if TX_EN signal asserted in Half-Duplex mode 
      unsigned LoopbackMode         : 1; // =1: Enable internal MAC MII Loopback mode
      unsigned HalfDuplexMode       : 1; // =1 Select MAC Half-Duplex mode (=0: Full-Duplex mode)
      unsigned ChecksumOffload      : 1; // =1: calculates complement sum of all received Ethernet payloads 
      unsigned RetryTransmission    : 1; // =1: MAC attempt will retry transmission, based on the settings of BL, when a colision occurs (Half-Duplex mode)
      unsigned AutomaticPadCRCStrip : 1; // =1: enable Automatic MAC Pad/CRC Stripping
      unsigned DeferralCheck        : 1; // =1: enable deferral check function (Half-Duplex mode)
      unsigned ReceiveAll           : 1; // =1: enable all frames reception by the MAC (No fitering)
      unsigned BroadcastFramesReception : 1; // =1: enable reception of Broadcast Frames
      unsigned PromiscuousMode          : 1; // =1: enable Promiscuous Mode
      unsigned ZeroQuantaPause          : 1; // =1: automatic generation of Zero-Quanta Pause Control frames
      unsigned UnicastPauseFrameDetect  : 1; // =1: MAC detects the Pause frames (with MAC Address0 unicast address and unique multicast address)
      unsigned ReceiveFlowControl       : 1; // =1: MAC decodes the received Pause frame and disable its transmitter for a specified (Pause Time) time
      unsigned TransmitFlowControl      : 1; // =1: MAC transmits Pause frames (Full-Duplex mode) or the MAC back-pressure operation (Half-Duplex mode)
      unsigned VLANTagComparison        : 1; // =1: Use the full 16-bit VLAN tag for comparison and filtering, instead of just the 12-bit VLAN identifier
      /* DMA */ 
      unsigned DropTCPIPChecksumErrorFrame : 1; // =1: drop TCP/IP Checksum Error Frames
      unsigned ReceiveStoreForward         : 1; // =1: enable Receive store and forward
      unsigned FlushReceivedFrame          : 1; // =1: enable flushing of received frames
      unsigned TransmitStoreForward        : 1; // =1: enable Transmit store and forward
      unsigned ForwardErrorFrames          : 1; // =1: forward to DMA of all frames except runt error frames
      unsigned ForwardUndersizedGoodFrames : 1; // =1: Rx FIFO will forward Undersized frames (frames with no Error and length less than 64 bytes) including pad-bytes and CRC)
      unsigned SecondFrameOperate  : 1; // 1=: DMA processes a second frame of Transmit data even before status for first frame is obtained
      unsigned AddressAlignedBeats : 1; // 1=: enable Address Aligned Beats
      unsigned FixedBurst          : 1; // 1=: enable AHB Master interface fixed burst transfers
      /**/
      unsigned Reserved1  : 5; // pad to 32 bits
    };
  } Flags;

  ttc_eth_addrfilter_e SourceAddrFilter; // Selects Normal/Inverse/disable Source Address Filter comparison
  ttc_eth_addrfilter_e DestinationAddrFilter; // Selects EnableNormal/EnableInverse destination filter for both unicast and multicast frames
  ttc_eth_framefilter_e MulticastFramesFilter; // Selects the Multicast Frames filter
  ttc_eth_framefilter_e UnicastFramesFilter;   // Selects the Unicast Frames filter
  ttc_eth_passcontrol_e PassControlFrames; // Selects None/All/FilterPass of all control frames (including unicast and multicast PAUSE frames)
  ttc_eth_dmaarbitration_e DMAArbitration; // Selects DMA Tx/Rx arbitration
  u8_t ReceiveThresholdControl; // Selects the threshold level of the Receive FIFO
  u16_t TransmitThresholdControl; // Selects the Transmit Threshold Control
  u8_t DescriptorSkipLength; // Specifies the number of word to skip between two unchained descriptors (Ring mode)
  u8_t TxDMABurstLength; // Indicate the maximum number of beats to be transferred in one Tx DMA transaction
  u8_t RxDMABurstLength; // Indicate the maximum number of beats to be transferred in one Rx DMA transaction
  u32_t PauseTime; // This field holds the value to be used in the Pause Time field in the transmit control frame
  u32_t HashTableLow; // This field contains the lower 32 bits of Hash table. 
  u32_t HashTableHigh; // This field contains the higher 32 bits of Hash table. 
  u32_t VLANTagIdentifier; // VLAN tag identifier for receive frames
  u16_t PauseLowThreshold; // This field configures the threshold of the PAUSE to be checked for automatic retransmission of PAUSE Frame
  u8_t BackOffLimit; // Selects the BackOff limit value
  u8_t Speed; // Indicates the Ethernet speed: 10/100 Mbps
  u8_t InterFrameGap; // Selects minimum IFG between frames during transmission 

  u8_t Layout;     // select pin layout to use (some uC allow pin remapping)

} __attribute__((__packed__)) ttc_eth_generic_t;


//}

#endif // TTC_ETH_TYPES_H
