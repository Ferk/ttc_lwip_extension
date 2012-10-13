/*{ stm32_eth::.c -------------------------------------------------------------

  The ToolChain

  Eth interface for ARM CortexM3 microcontrollers.
  Currently tested to comply to STM32F107.

  written by Fernando Carmona Varo 2012

  Note: See ttc_eth.h for description of architecture independent ETH implementation.

  }*/

#include "stm32_ethernet.h"


#define  ETH_DMARxDesc_FrameLengthShift 16

// Number of receive and transfer buffers
#define ETH_RXBUFNB  4
#define ETH_TXBUFNB  4

// Ethernet Buffers
u8_t Rx_Buff[ETH_RXBUFNB][ETH_MAX_PACKET_SIZE];
u8_t Tx_Buff[ETH_TXBUFNB][ETH_MAX_PACKET_SIZE];
    
// Default is RMII active for STM32, set TTC_ETH_RMII to "0" explicitly to use MII instead
#ifndef TTC_ETH_RMII
#define TTC_ETH_RMII 1
#endif /* TTC_ETH_RMII */

ETH_DMADESCTypeDef  DMARxDscrTab[ETH_RXBUFNB], DMATxDscrTab[ETH_TXBUFNB]; /* Ethernet Rx & Tx DMA Descriptors */
ETH_DMADESCTypeDef  *DMATxDesc = DMATxDscrTab;


u8_t* stm32_ETHs[TTC_AMOUNT_ETHS];
u8_t seth_Initialized=0;

//{ Function definitions --------------------------------------------------------

void stm32_eth_reset_all() {
    memset(stm32_ETHs, 0, sizeof(ETH_TypeDef*) * TTC_AMOUNT_ETHS);
    seth_Initialized=1;
}
ttc_eth_errorcode_e stm32_eth_get_defaults(u8_t ETH_Index, ttc_eth_generic_t* ETH_Generic) {
    if (!seth_Initialized) stm32_eth_reset_all();
#ifndef TTC_NO_ARGUMENT_CHECKS //{
    // assumption: *ETH_Generic has been zeroed

    Assert(ETH_Index > 0,    ec_InvalidArgument);
    Assert(ETH_Generic != NULL, ec_NULL);
    if (ETH_Index > TTC_AMOUNT_ETHS) return tee_DeviceNotFound;
#endif //}

    ETH_Generic->Flags.All=0;
    /* ETH_InitStruct members default value */
    /*---------------   MAC   ---------------*/
    ETH_Generic->Flags.AutoNegotiation          = 0;
    ETH_Generic->Flags.Watchdog                 = 1;
    ETH_Generic->Flags.Jabber                   = 1;
    ETH_Generic->Flags.CarrierSense             = 1;
    ETH_Generic->Flags.ReceiveOwn               = 1;
    ETH_Generic->Flags.LoopbackMode             = 0;
    ETH_Generic->Flags.HalfDuplexMode           = 0;//1;
    ETH_Generic->Flags.ChecksumOffload          = 0;
    ETH_Generic->Flags.RetryTransmission        = 0;//1;
    ETH_Generic->Flags.AutomaticPadCRCStrip     = 0;
    ETH_Generic->Flags.DeferralCheck            = 0;
    ETH_Generic->Flags.ReceiveAll               = 1;//0;
    ETH_Generic->Flags.BroadcastFramesReception = 0;
    ETH_Generic->Flags.PromiscuousMode          = 0;
    ETH_Generic->Flags.ZeroQuantaPause          = 0;
    ETH_Generic->Flags.UnicastPauseFrameDetect  = 0;
    ETH_Generic->Flags.ReceiveFlowControl       = 0;
    ETH_Generic->Flags.TransmitFlowControl      = 0;
    ETH_Generic->Flags.VLANTagComparison        = 1;
    /*-------------   DMA   --------------*/
    ETH_Generic->Flags.DropTCPIPChecksumErrorFrame = 1;//0
    ETH_Generic->Flags.ReceiveStoreForward         = 1;
    ETH_Generic->Flags.FlushReceivedFrame          = 0;
    ETH_Generic->Flags.TransmitStoreForward        = 1;
    ETH_Generic->Flags.ForwardErrorFrames          = 0;
    ETH_Generic->Flags.ForwardUndersizedGoodFrames = 0;
    ETH_Generic->Flags.SecondFrameOperate          = 1;//0
    ETH_Generic->Flags.AddressAlignedBeats         = 1;
    ETH_Generic->Flags.FixedBurst                  = 1;//0

    /* MAC */
    ETH_Generic->InterFrameGap                  = 96;
    ETH_Generic->Speed                          = 100;//10;
    ETH_Generic->BackOffLimit                   = 10;
    ETH_Generic->SourceAddrFilter               = tea_None;
    ETH_Generic->PassControlFrames              = tep_BlockAll;
    ETH_Generic->DestinationAddrFilter          = tea_NormalFilter;
    ETH_Generic->MulticastFramesFilter          = tef_None;//tef_PerfectFilter;
    ETH_Generic->UnicastFramesFilter            = tef_PerfectFilter;
    ETH_Generic->HashTableHigh                  = 0x0;
    ETH_Generic->HashTableLow                   = 0x0;
    ETH_Generic->PauseTime                      = 0x0;
    ETH_Generic->PauseLowThreshold              = 4;
    ETH_Generic->VLANTagIdentifier              = 0x0;
    /* DMA */
    ETH_Generic->TransmitThresholdControl          = 64;
    ETH_Generic->ReceiveThresholdControl           = 64;
    ETH_Generic->RxDMABurstLength                  = 32;
    ETH_Generic->TxDMABurstLength                  = 32;
    ETH_Generic->DescriptorSkipLength              = 0x0;
    ETH_Generic->DMAArbitration                    = ted_RoundRobin_RxTx_2to1;

    ETH_Generic->Layout                            = 0;

    return tee_OK;
}
ttc_eth_errorcode_e stm32_eth_init(u8_t ETH_Index, ttc_eth_generic_t* ETH_Generic, stm32_eth_architecture_t* ETH_Arch) {
    if (!seth_Initialized) stm32_eth_reset_all();
#ifndef TTC_NO_ARGUMENT_CHECKS //{
    // assumption: *ETH_Generic has been zeroed

    Assert(ETH_Index > 0,    ec_InvalidArgument);
    Assert(ETH_Generic != NULL, ec_NULL);
    if (ETH_Index > TTC_AMOUNT_ETHS) return tee_DeviceNotFound;
#endif //}

    if (1) { // determine pin layout
	switch (ETH_Index) {
	case 1:
	    stm32_loadPort(&ETH_Arch->PortRMII_MDIO,    TTC_ETH1_RMII_MDIO    );
	    stm32_loadPort(&ETH_Arch->PortRMII_MDC,     TTC_ETH1_RMII_MDC     );
	    stm32_loadPort(&ETH_Arch->PortRMII_TX_EN,   TTC_ETH1_RMII_TX_EN   );
	    stm32_loadPort(&ETH_Arch->PortRMII_TXD0,    TTC_ETH1_RMII_TXD0    );
	    stm32_loadPort(&ETH_Arch->PortRMII_TXD1,    TTC_ETH1_RMII_TXD1    );
	    stm32_loadPort(&ETH_Arch->PortRMII_PPS_OUT, TTC_ETH1_RMII_PPS_OUT );
	    stm32_loadPort(&ETH_Arch->PortMII_MDIO,     TTC_ETH1_MII_MDIO     );
	    stm32_loadPort(&ETH_Arch->PortMII_MDC,      TTC_ETH1_MII_MDC      );
	    stm32_loadPort(&ETH_Arch->PortMII_TXD2,     TTC_ETH1_MII_TXD2     );
	    stm32_loadPort(&ETH_Arch->PortMII_TX_EN,    TTC_ETH1_MII_TX_EN    );
	    stm32_loadPort(&ETH_Arch->PortMII_TXD0,     TTC_ETH1_MII_TXD0     );
	    stm32_loadPort(&ETH_Arch->PortMII_TXD1,     TTC_ETH1_MII_TXD1     );
	    stm32_loadPort(&ETH_Arch->PortMII_PPS_OUT,  TTC_ETH1_MII_PPS_OUT  );
	    stm32_loadPort(&ETH_Arch->PortMII_TXD3,     TTC_ETH1_MII_TXD3     );
	    stm32_loadPort(&ETH_Arch->PortRMII_REF_CLK, TTC_ETH1_RMII_REF_CLK );
	    stm32_loadPort(&ETH_Arch->PortRMII_CRS_DV,  TTC_ETH1_RMII_CRS_DV  );
	    stm32_loadPort(&ETH_Arch->PortRMII_RXD0,    TTC_ETH1_RMII_RXD0    );
	    stm32_loadPort(&ETH_Arch->PortRMII_RXD1,    TTC_ETH1_RMII_RXD1    );
	    stm32_loadPort(&ETH_Arch->PortMII_CRS_WKUP, TTC_ETH1_MII_CRS_WKUP );
	    stm32_loadPort(&ETH_Arch->PortMII_RX_CLK,   TTC_ETH1_MII_RX_CLK   );
	    stm32_loadPort(&ETH_Arch->PortMII_COL,      TTC_ETH1_MII_COL      );
	    stm32_loadPort(&ETH_Arch->PortMII_RX_DV,    TTC_ETH1_MII_RX_DV    );
	    stm32_loadPort(&ETH_Arch->PortMII_TX_CLK,   TTC_ETH1_MII_TX_CLK   );
	    stm32_loadPort(&ETH_Arch->PortMII_RXD0,     TTC_ETH1_MII_RXD0     );
	    stm32_loadPort(&ETH_Arch->PortMII_RXD1,     TTC_ETH1_MII_RXD1     );
	    stm32_loadPort(&ETH_Arch->PortMII_RXD2,     TTC_ETH1_MII_RXD2     );
	    stm32_loadPort(&ETH_Arch->PortMII_RXD3,     TTC_ETH1_MII_RXD3     );
	    stm32_loadPort(&ETH_Arch->PortMII_RX_ER,    TTC_ETH1_MII_RX_ER    );
	}
    }
    if (1) { // activate clocks
        /* Enable ETHERNET clock  */
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_ETH_MAC | RCC_AHBPeriph_ETH_MAC_Tx |
                              RCC_AHBPeriph_ETH_MAC_Rx, ENABLE);
        /* Enable GPIO clocks */
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE | RCC_APB2Periph_AFIO, ENABLE);
    }
      
    if (1) { // configure GPIOs
	ttc_Port_t Port;
        /* Output */
#if TTC_ETH_RMII
        stm32_initPort2(&ETH_Arch->PortRMII_MDIO,    GPIO_Mode_AF_PP, GPIO_Speed_50MHz);
        stm32_initPort2(&ETH_Arch->PortRMII_MDC,     GPIO_Mode_AF_PP, GPIO_Speed_50MHz);
        stm32_initPort2(&ETH_Arch->PortRMII_TX_EN,   GPIO_Mode_AF_PP, GPIO_Speed_50MHz);
        stm32_initPort2(&ETH_Arch->PortRMII_TXD0,    GPIO_Mode_AF_PP, GPIO_Speed_50MHz);
        stm32_initPort2(&ETH_Arch->PortRMII_PPS_OUT, GPIO_Mode_AF_PP, GPIO_Speed_50MHz);
        stm32_initPort2(&ETH_Arch->PortRMII_TXD1,    GPIO_Mode_AF_PP, GPIO_Speed_50MHz);
#else
        stm32_initPort2(&ETH_Arch->PortMII_MDIO,     GPIO_Mode_AF_PP, GPIO_Speed_50MHz);
        stm32_initPort2(&ETH_Arch->PortMII_MDC,      GPIO_Mode_AF_PP, GPIO_Speed_50MHz);
        stm32_initPort2(&ETH_Arch->PortMII_TXD2,     GPIO_Mode_AF_PP, GPIO_Speed_50MHz);
        stm32_initPort2(&ETH_Arch->PortMII_TX_EN,    GPIO_Mode_AF_PP, GPIO_Speed_50MHz);
        stm32_initPort2(&ETH_Arch->PortMII_TXD0,     GPIO_Mode_AF_PP, GPIO_Speed_50MHz);
        stm32_initPort2(&ETH_Arch->PortMII_TXD1,     GPIO_Mode_AF_PP, GPIO_Speed_50MHz);
        stm32_initPort2(&ETH_Arch->PortMII_PPS_OUT,  GPIO_Mode_AF_PP, GPIO_Speed_50MHz);
        stm32_initPort2(&ETH_Arch->PortMII_TXD3,     GPIO_Mode_AF_PP, GPIO_Speed_50MHz);
#endif

        /* Input  */
	GPIO_PinRemapConfig(GPIO_Remap_ETH, DISABLE);///D
#if TTC_ETH_RMII
        stm32_initPort2(&ETH_Arch->PortRMII_REF_CLK, GPIO_Mode_IN_FLOATING, GPIO_Speed_50MHz);
        stm32_initPort2(&ETH_Arch->PortRMII_CRS_DV,  GPIO_Mode_IN_FLOATING, GPIO_Speed_50MHz);
        stm32_initPort2(&ETH_Arch->PortRMII_RXD0,    GPIO_Mode_IN_FLOATING, GPIO_Speed_50MHz);
        stm32_initPort2(&ETH_Arch->PortRMII_RXD1,    GPIO_Mode_IN_FLOATING, GPIO_Speed_50MHz);
#else
        stm32_initPort2(&ETH_Arch->PortMII_CRS_WKUP, GPIO_Mode_IN_FLOATING, GPIO_Speed_50MHz);
        stm32_initPort2(&ETH_Arch->PortMII_RX_CLK,   GPIO_Mode_IN_FLOATING, GPIO_Speed_50MHz);
        stm32_initPort2(&ETH_Arch->PortMII_COL,      GPIO_Mode_IN_FLOATING, GPIO_Speed_50MHz);
        stm32_initPort2(&ETH_Arch->PortMII_RX_DV,    GPIO_Mode_IN_FLOATING, GPIO_Speed_50MHz);
        stm32_initPort2(&ETH_Arch->PortMII_TX_CLK,   GPIO_Mode_IN_FLOATING, GPIO_Speed_50MHz);
        stm32_initPort2(&ETH_Arch->PortMII_RXD0,     GPIO_Mode_IN_FLOATING, GPIO_Speed_50MHz);
        stm32_initPort2(&ETH_Arch->PortMII_RXD1,     GPIO_Mode_IN_FLOATING, GPIO_Speed_50MHz);
        stm32_initPort2(&ETH_Arch->PortMII_RXD2,     GPIO_Mode_IN_FLOATING, GPIO_Speed_50MHz);
        stm32_initPort2(&ETH_Arch->PortMII_RXD3,     GPIO_Mode_IN_FLOATING, GPIO_Speed_50MHz);
        stm32_initPort2(&ETH_Arch->PortMII_RX_ER,    GPIO_Mode_IN_FLOATING, GPIO_Speed_50MHz);
#endif
        /* Configure MCO pin (PA8) as push-pull too */
        ttc_initPort2(&Port, GPIOA,8, tgm_alternate_function_push_pull, GPIO_Speed_50MHz);
    }
    if (1) { // apply remap layout

        if (ETH_Generic->Layout)
            /* ETHERNET pins remapp in STM3210C-EVAL board: RX_DV and RxD[3:0] */
            GPIO_PinRemapConfig(GPIO_Remap_ETH, ENABLE);
        else
            {;}///GPIO_PinRemapConfig(GPIO_Remap_ETH, DISABLE);

    }
    if (1) { // init ETH


#if TTC_ETH_RMII
	GPIO_ETH_MediaInterfaceConfig(GPIO_ETH_MediaInterface_RMII); // Set as RMII
        /* Get HSE clock = 25MHz on PA8 pin(MCO) */
        /* set PLL3 clock output to 50MHz (25MHz /5 *10 =50MHz) */
        RCC_PLL3Config(RCC_PLL3Mul_10);
        /* Enable PLL3 */
        RCC_PLL3Cmd(ENABLE);
        /* Wait till PLL3 is ready */
        while (RCC_GetFlagStatus(RCC_FLAG_PLL3RDY) == RESET);

        /* Get clock PLL3 clock on PA8 pin */
        RCC_MCOConfig(RCC_MCO_PLL3CLK);
#else
	GPIO_ETH_MediaInterfaceConfig(GPIO_ETH_MediaInterface_MII); // Set as MII
        /* Get HSE clock = 25MHz on PA8 pin (MCO) */
        RCC_MCOConfig(RCC_MCO_HSE);
#endif
	
	/* Reset ETHERNET on AHB Bus */
	ETH_DeInit();
        /* Software reset */
        ETH_SoftwareReset();
        while(ETH_GetSoftwareResetStatus()==SET); // Wait for reset

        unsigned int PhyAddr;
        for(PhyAddr = 1; PhyAddr <= 32; PhyAddr++) {
            if((0x0006 == ETH_ReadPHYRegister(PhyAddr,2))
               && (0x1c50 == (ETH_ReadPHYRegister(PhyAddr,3)&0xFFF0))) break;
        }

        if( PhyAddr > 32 )
            return tee_DeviceNotFound;

        ETH_InitTypeDef ETH_MyInit;
	/*------------------------   MAC   -----------------------------------*/
	if (ETH_Generic->Flags.AutoNegotiation) ETH_MyInit.ETH_AutoNegotiation = ETH_AutoNegotiation_Enable;
	else ETH_MyInit.ETH_AutoNegotiation = ETH_AutoNegotiation_Disable;
        if (ETH_Generic->Flags.Watchdog) ETH_MyInit.ETH_Watchdog = ETH_Watchdog_Enable;
	else ETH_MyInit.ETH_Watchdog = ETH_Watchdog_Disable;
        if (ETH_Generic->Flags.Jabber) ETH_MyInit.ETH_Jabber = ETH_Jabber_Enable;
	else ETH_MyInit.ETH_Jabber = ETH_Jabber_Disable;
        if (ETH_Generic->Flags.CarrierSense) ETH_MyInit.ETH_CarrierSense = ETH_CarrierSense_Enable;
	else ETH_MyInit.ETH_CarrierSense = ETH_CarrierSense_Disable;
        if (ETH_Generic->Flags.ReceiveOwn) ETH_MyInit.ETH_ReceiveOwn = ETH_ReceiveOwn_Enable;
	else ETH_MyInit.ETH_ReceiveOwn = ETH_ReceiveOwn_Disable;
        if (ETH_Generic->Flags.LoopbackMode) ETH_MyInit.ETH_LoopbackMode = ETH_LoopbackMode_Enable;
	else ETH_MyInit.ETH_LoopbackMode = ETH_LoopbackMode_Disable;
        if (ETH_Generic->Flags.ChecksumOffload) ETH_MyInit.ETH_ChecksumOffload = ETH_ChecksumOffload_Enable;
	else ETH_MyInit.ETH_ChecksumOffload = ETH_ChecksumOffload_Disable;
        if (ETH_Generic->Flags.RetryTransmission) ETH_MyInit.ETH_RetryTransmission = ETH_RetryTransmission_Enable;
	else ETH_MyInit.ETH_RetryTransmission = ETH_RetryTransmission_Disable;
        if (ETH_Generic->Flags.AutomaticPadCRCStrip) ETH_MyInit.ETH_AutomaticPadCRCStrip = ETH_AutomaticPadCRCStrip_Enable;
	else ETH_MyInit.ETH_AutomaticPadCRCStrip = ETH_AutomaticPadCRCStrip_Disable;
        if (ETH_Generic->Flags.DeferralCheck) ETH_MyInit.ETH_DeferralCheck = ETH_DeferralCheck_Enable;
	else ETH_MyInit.ETH_DeferralCheck = ETH_DeferralCheck_Disable;
        if (ETH_Generic->Flags.ReceiveAll) ETH_MyInit.ETH_ReceiveAll = ETH_ReceiveAll_Enable;
	else ETH_MyInit.ETH_ReceiveAll = ETH_ReceiveAll_Disable;
        if (ETH_Generic->Flags.BroadcastFramesReception) ETH_MyInit.ETH_BroadcastFramesReception = ETH_BroadcastFramesReception_Enable;
	else ETH_MyInit.ETH_BroadcastFramesReception = ETH_BroadcastFramesReception_Disable;
        if (ETH_Generic->Flags.PromiscuousMode) ETH_MyInit.ETH_PromiscuousMode = ETH_PromiscuousMode_Enable;
	else ETH_MyInit.ETH_PromiscuousMode = ETH_PromiscuousMode_Disable;
        if (ETH_Generic->Flags.ZeroQuantaPause) ETH_MyInit.ETH_ZeroQuantaPause = ETH_ZeroQuantaPause_Enable;
	else ETH_MyInit.ETH_ZeroQuantaPause = ETH_ZeroQuantaPause_Disable;
        if (ETH_Generic->Flags.UnicastPauseFrameDetect) ETH_MyInit.ETH_UnicastPauseFrameDetect = ETH_UnicastPauseFrameDetect_Enable;
	else ETH_MyInit.ETH_UnicastPauseFrameDetect = ETH_UnicastPauseFrameDetect_Disable;
        if (ETH_Generic->Flags.ReceiveFlowControl) ETH_MyInit.ETH_ReceiveFlowControl = ETH_ReceiveFlowControl_Enable;
	else ETH_MyInit.ETH_ReceiveFlowControl = ETH_ReceiveFlowControl_Disable;
        if (ETH_Generic->Flags.TransmitFlowControl) ETH_MyInit.ETH_TransmitFlowControl = ETH_TransmitFlowControl_Enable;
	else ETH_MyInit.ETH_TransmitFlowControl = ETH_TransmitFlowControl_Disable;
        /* options to be included */

	switch(ETH_Generic->PassControlFrames) {
	case tep_BlockAll:                ETH_MyInit.ETH_PassControlFrames = ETH_PassControlFrames_BlockAll; break;
	case tep_ForwardAll:              ETH_MyInit.ETH_PassControlFrames = ETH_PassControlFrames_ForwardAll; break;
	case tep_ForwardPassedAddrFilter: ETH_MyInit.ETH_PassControlFrames = ETH_PassControlFrames_ForwardPassedAddrFilter;break;
	default: Assert(0,ec_UNKNOWN); break; // Invalid value 
	}
	switch(ETH_Generic->UnicastFramesFilter) {
	case tef_HashTableFilter:        ETH_MyInit.ETH_UnicastFramesFilter = ETH_UnicastFramesFilter_HashTable; break;
	case tef_PerfectFilter:          ETH_MyInit.ETH_UnicastFramesFilter = ETH_UnicastFramesFilter_Perfect; break;
	case tef_PerfectHashTableFilter: ETH_MyInit.ETH_UnicastFramesFilter = ETH_UnicastFramesFilter_PerfectHashTable; break;
	default: Assert(0,ec_UNKNOWN); break; // Invalid value 
	}

	switch(ETH_Generic->MulticastFramesFilter) {
	case tef_HashTableFilter: ETH_MyInit.ETH_MulticastFramesFilter = ETH_MulticastFramesFilter_HashTable; break;
	case tef_PerfectFilter: ETH_MyInit.ETH_MulticastFramesFilter = ETH_MulticastFramesFilter_Perfect; break;
	case tef_PerfectHashTableFilter: ETH_MyInit.ETH_MulticastFramesFilter = ETH_MulticastFramesFilter_PerfectHashTable; break;
	default: ETH_MyInit.ETH_MulticastFramesFilter = ETH_MulticastFramesFilter_None; break;
	}


	switch(ETH_Generic->SourceAddrFilter) {
	case tea_NormalFilter: ETH_MyInit.ETH_SourceAddrFilter = ETH_SourceAddrFilter_Normal_Enable; break;
	case tea_InverseFilter: ETH_MyInit.ETH_SourceAddrFilter = ETH_SourceAddrFilter_Inverse_Enable; break;
	default: ETH_MyInit.ETH_SourceAddrFilter = ETH_SourceAddrFilter_Disable; break;
	}
	
	switch(ETH_Generic->DestinationAddrFilter) {
	case tea_NormalFilter: ETH_MyInit.ETH_DestinationAddrFilter = ETH_DestinationAddrFilter_Normal; break;
	case tea_InverseFilter: ETH_MyInit.ETH_DestinationAddrFilter = ETH_DestinationAddrFilter_Inverse; break;
	default: Assert(0,ec_UNKNOWN); break; // Invalid value 
	}
	
        if(ETH_Generic->Flags.HalfDuplexMode)
            ETH_MyInit.ETH_Mode = ETH_Mode_HalfDuplex;
        else
            ETH_MyInit.ETH_Mode = ETH_Mode_FullDuplex;

	if(ETH_Generic->Flags.VLANTagComparison)
	    ETH_MyInit.ETH_VLANTagComparison = ETH_VLANTagComparison_16Bit;
	else
	    ETH_MyInit.ETH_VLANTagComparison = ETH_VLANTagComparison_12Bit;
	
        switch(ETH_Generic->Speed) {
        case 10:  ETH_MyInit.ETH_Speed = ETH_Speed_10M; break;
        case 100: ETH_MyInit.ETH_Speed = ETH_Speed_100M; break;
	default: Assert(0,ec_UNKNOWN); break; // Invalid value 
        }

	switch(ETH_Generic->BackOffLimit) {
        case 10: ETH_MyInit.ETH_BackOffLimit = ETH_BackOffLimit_10; break;
	case 8:  ETH_MyInit.ETH_BackOffLimit = ETH_BackOffLimit_8; break;
	case 4:  ETH_MyInit.ETH_BackOffLimit = ETH_BackOffLimit_4; break;
	case 1:  ETH_MyInit.ETH_BackOffLimit = ETH_BackOffLimit_1; break;
	default: Assert(0,ec_UNKNOWN); break; // Invalid value 
        }

	switch(ETH_Generic->InterFrameGap) {
	case 96: ETH_MyInit.ETH_InterFrameGap = ETH_InterFrameGap_96Bit; break;
	case 88: ETH_MyInit.ETH_InterFrameGap = ETH_InterFrameGap_88Bit; break;
	case 80: ETH_MyInit.ETH_InterFrameGap = ETH_InterFrameGap_80Bit; break;
	case 72: ETH_MyInit.ETH_InterFrameGap = ETH_InterFrameGap_72Bit; break;
	case 64: ETH_MyInit.ETH_InterFrameGap = ETH_InterFrameGap_64Bit; break;
	case 56: ETH_MyInit.ETH_InterFrameGap = ETH_InterFrameGap_56Bit; break;
	case 48: ETH_MyInit.ETH_InterFrameGap = ETH_InterFrameGap_48Bit; break;
	case 40: ETH_MyInit.ETH_InterFrameGap = ETH_InterFrameGap_40Bit; break;
	default: Assert(0,ec_UNKNOWN); break; // Invalid value 
	}

	switch(ETH_Generic->PauseLowThreshold) {
	case 4: ETH_MyInit.ETH_PauseLowThreshold = ETH_PauseLowThreshold_Minus4; break;
	case 28: ETH_MyInit.ETH_PauseLowThreshold = ETH_PauseLowThreshold_Minus28; break;
	case 144: ETH_MyInit.ETH_PauseLowThreshold = ETH_PauseLowThreshold_Minus144; break;
	case 256: ETH_MyInit.ETH_PauseLowThreshold = ETH_PauseLowThreshold_Minus256; break;
	default: Assert(0,ec_UNKNOWN); break; // Invalid value 
	}
	
        ETH_MyInit.ETH_HashTableHigh     = ETH_Generic->HashTableHigh;
        ETH_MyInit.ETH_HashTableLow      = ETH_Generic->HashTableLow;
        ETH_MyInit.ETH_PauseTime         = ETH_Generic->PauseTime;
        /*------------------------   DMA   -----------------------------------*/
        if (ETH_Generic->Flags.DropTCPIPChecksumErrorFrame) ETH_MyInit.ETH_DropTCPIPChecksumErrorFrame = ETH_DropTCPIPChecksumErrorFrame_Enable;
	else ETH_MyInit.ETH_DropTCPIPChecksumErrorFrame = ETH_DropTCPIPChecksumErrorFrame_Disable;
        if (ETH_Generic->Flags.ReceiveStoreForward) ETH_MyInit.ETH_ReceiveStoreForward = ETH_ReceiveStoreForward_Enable;
	else ETH_MyInit.ETH_ReceiveStoreForward = ETH_ReceiveStoreForward_Disable;
        if (ETH_Generic->Flags.FlushReceivedFrame) ETH_MyInit.ETH_FlushReceivedFrame = ETH_FlushReceivedFrame_Enable;
	else ETH_MyInit.ETH_FlushReceivedFrame = ETH_FlushReceivedFrame_Disable;
        if (ETH_Generic->Flags.TransmitStoreForward) ETH_MyInit.ETH_TransmitStoreForward = ETH_TransmitStoreForward_Enable;
	else ETH_MyInit.ETH_TransmitStoreForward = ETH_TransmitStoreForward_Disable;
        if (ETH_Generic->Flags.ForwardErrorFrames) ETH_MyInit.ETH_ForwardErrorFrames = ETH_ForwardErrorFrames_Enable;
	else ETH_MyInit.ETH_ForwardErrorFrames = ETH_ForwardErrorFrames_Disable;
        if (ETH_Generic->Flags.ForwardUndersizedGoodFrames) ETH_MyInit.ETH_ForwardUndersizedGoodFrames = ETH_ForwardUndersizedGoodFrames_Enable;
	else ETH_MyInit.ETH_ForwardUndersizedGoodFrames = ETH_ForwardUndersizedGoodFrames_Disable;
        if (ETH_Generic->Flags.SecondFrameOperate) ETH_MyInit.ETH_SecondFrameOperate = ETH_SecondFrameOperate_Enable;
	else ETH_MyInit.ETH_SecondFrameOperate = ETH_SecondFrameOperate_Disable;
        if (ETH_Generic->Flags.AddressAlignedBeats) ETH_MyInit.ETH_AddressAlignedBeats = ETH_AddressAlignedBeats_Enable;
	else ETH_MyInit.ETH_AddressAlignedBeats = ETH_AddressAlignedBeats_Disable;
        if (ETH_Generic->Flags.FixedBurst) ETH_MyInit.ETH_FixedBurst = ETH_FixedBurst_Enable;
	else ETH_MyInit.ETH_FixedBurst = ETH_FixedBurst_Disable;

        ETH_MyInit.ETH_DescriptorSkipLength = ETH_Generic->DescriptorSkipLength;
	ETH_MyInit.ETH_VLANTagIdentifier = ETH_Generic->VLANTagIdentifier;

	switch (ETH_Generic->DMAArbitration) {
	case ted_RoundRobin_RxTx_1to1: ETH_MyInit.ETH_DMAArbitration = ETH_DMAArbitration_RoundRobin_RxTx_1_1; break;
	case ted_RoundRobin_RxTx_2to1: ETH_MyInit.ETH_DMAArbitration = ETH_DMAArbitration_RoundRobin_RxTx_2_1; break;
	case ted_RoundRobin_RxTx_3to1: ETH_MyInit.ETH_DMAArbitration = ETH_DMAArbitration_RoundRobin_RxTx_3_1; break;
	case ted_RoundRobin_RxTx_4to1: ETH_MyInit.ETH_DMAArbitration = ETH_DMAArbitration_RoundRobin_RxTx_4_1; break;
	case ted_RxPriorTx:            ETH_MyInit.ETH_DMAArbitration = ETH_DMAArbitration_RxPriorTx; break;
	default: Assert(0,ec_UNKNOWN); break; // Invalid value 
	}
	
        switch (ETH_Generic->RxDMABurstLength) {
	case 1:   ETH_MyInit.ETH_RxDMABurstLength = ETH_RxDMABurstLength_1Beat; break;
	case 2:   ETH_MyInit.ETH_RxDMABurstLength = ETH_RxDMABurstLength_2Beat; break;
	case 4:   ETH_MyInit.ETH_RxDMABurstLength = ETH_RxDMABurstLength_4Beat; break;
	case 8:   ETH_MyInit.ETH_RxDMABurstLength = ETH_RxDMABurstLength_8Beat; break;
	case 16:  ETH_MyInit.ETH_RxDMABurstLength = ETH_RxDMABurstLength_16Beat; break;
	case 32:  ETH_MyInit.ETH_RxDMABurstLength = ETH_RxDMABurstLength_32Beat; break;
	case 64:  ETH_MyInit.ETH_RxDMABurstLength = ETH_RxDMABurstLength_4xPBL_64Beat; break;
	case 128: ETH_MyInit.ETH_RxDMABurstLength = ETH_RxDMABurstLength_4xPBL_128Beat; break;
	default: Assert(0,ec_UNKNOWN); break; // Invalid value 
	}

	switch (ETH_Generic->TxDMABurstLength) {
	case 1:   ETH_MyInit.ETH_TxDMABurstLength = ETH_TxDMABurstLength_1Beat; break;
	case 2:   ETH_MyInit.ETH_TxDMABurstLength = ETH_TxDMABurstLength_2Beat; break;
	case 4:   ETH_MyInit.ETH_TxDMABurstLength = ETH_TxDMABurstLength_4Beat; break;
	case 8:   ETH_MyInit.ETH_TxDMABurstLength = ETH_TxDMABurstLength_8Beat; break;
	case 16:  ETH_MyInit.ETH_TxDMABurstLength = ETH_TxDMABurstLength_16Beat; break;
	case 32:  ETH_MyInit.ETH_TxDMABurstLength = ETH_TxDMABurstLength_32Beat; break;
	case 64:  ETH_MyInit.ETH_TxDMABurstLength = ETH_TxDMABurstLength_4xPBL_64Beat; break;
	case 128: ETH_MyInit.ETH_TxDMABurstLength = ETH_TxDMABurstLength_4xPBL_128Beat; break;
	default: Assert(0,ec_UNKNOWN); break; // Invalid value 
	}
	
        switch (ETH_Generic->TransmitThresholdControl) {
        case 64:  ETH_MyInit.ETH_TransmitThresholdControl = ETH_TransmitThresholdControl_64Bytes; break;
        case 128: ETH_MyInit.ETH_TransmitThresholdControl = ETH_TransmitThresholdControl_128Bytes; break;
        case 192: ETH_MyInit.ETH_TransmitThresholdControl = ETH_TransmitThresholdControl_192Bytes; break;
        case 256: ETH_MyInit.ETH_TransmitThresholdControl = ETH_TransmitThresholdControl_256Bytes; break;
        case 40:  ETH_MyInit.ETH_TransmitThresholdControl = ETH_TransmitThresholdControl_40Bytes; break;
        case 32:  ETH_MyInit.ETH_TransmitThresholdControl = ETH_TransmitThresholdControl_32Bytes; break;
        case 24:  ETH_MyInit.ETH_TransmitThresholdControl = ETH_TransmitThresholdControl_24Bytes; break;
        case 16:  ETH_MyInit.ETH_TransmitThresholdControl = ETH_TransmitThresholdControl_16Bytes; break;
	default: Assert(0,ec_UNKNOWN); break; // Invalid value 
        }

        switch (ETH_Generic->ReceiveThresholdControl) {
        case 64:  ETH_MyInit.ETH_ReceiveThresholdControl = ETH_ReceiveThresholdControl_64Bytes; break;
        case 32:  ETH_MyInit.ETH_ReceiveThresholdControl = ETH_ReceiveThresholdControl_32Bytes; break;
        case 96:  ETH_MyInit.ETH_ReceiveThresholdControl = ETH_ReceiveThresholdControl_96Bytes; break;
        case 128: ETH_MyInit.ETH_ReceiveThresholdControl = ETH_ReceiveThresholdControl_128Bytes; break;
	default: Assert(0,ec_UNKNOWN); break; // Invalid value 
        }
        /**/

        if( ETH_Init(&ETH_MyInit, PhyAddr) == 0 )
            return tee_DeviceNotReady;
    }
    if (1) { // setup interrupts

        /* Enable and set ETHn Interrupt to the lowest priority */
        NVIC_InitTypeDef NVIC_InitStructure;

	/* 2 bit for pre-emption priority, 2 bits for subpriority */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	/* Enable the Ethernet global Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = ETH_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/* Enable the Ethernet wakeup Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = ETH_WKUP_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Enable the Ethernet Rx Interrupt */
	ETH_DMAITConfig(ETH_DMA_IT_NIS | ETH_DMA_IT_R, ENABLE);
	
	//Enable MAC Interrupts
	ETH_MACITConfig(ETH_MAC_IT_MMC, ENABLE);
	//ETH_MAC_IT_MMC
	
	//Enable Wakeup Interrupts
	ETH_GlobalUnicastWakeUpCmd(ENABLE);
	ETH_WakeUpFrameDetectionCmd(ENABLE);
	
	/* Enable DMA interrupt */
	for(int i=0; i<ETH_RXBUFNB; i++)
	    ETH_DMARxDescReceiveITConfig(&DMARxDscrTab[i], ENABLE);
	
#ifdef CHECKSUM_BY_HARDWARE
	/* Enable the checksum insertion for the Tx frames */
	for(int i=0; i<ETH_TXBUFNB; i++)
	    ETH_DMATxDescChecksumInsertionConfig(&DMATxDscrTab[i], ETH_DMATxDesc_ChecksumTCPUDPICMPFull);
#endif
    }
    
    /* Initialize Tx Descriptors list: Chain Mode */
    ETH_DMATxDescChainInit(DMATxDscrTab, &Tx_Buff[0][0], ETH_TXBUFNB);
    /* Initialize Rx Descriptors list: Chain Mode  */
    ETH_DMARxDescChainInit(DMARxDscrTab, &Rx_Buff[0][0], ETH_RXBUFNB);
    
    /* Enable MAC and DMA transmission and reception */
    ETH_Start();
    
    ///ETH_Cmd(ETH_Arch->Base, ENABLE);
    ///stm32_ETHs[ETH_Index]=(ETH_t*) ETH_Arch->Base;
    return tee_OK;
}


/**
 * Return the address of the buffer pointed by the current output descriptor.
 * You can store in this address the data to be sent so that calling
 * stm_eth_send_output_buffer will send it.
 * Doing it like this avoids copying the data from another address.
 */
inline ttc_eth_errorcode_e stm32_eth_get_output_buffer(u8_t** Buffer)
{
    // from stm32_eth.c
    extern ETH_DMADESCTypeDef  *DMATxDescToSet;

    // Return Buffer address
    *Buffer= (u8_t*) DMATxDescToSet->Buffer1Addr;
    return tee_OK;
}


inline ttc_eth_errorcode_e stm32_eth_send_output_buffer(u16 FrameLength)
{
    // from stm32_eth.c
    extern ETH_DMADESCTypeDef  *DMATxDescToSet;

    /* Check if the descriptor is owned by the ETHERNET DMA (when set) or CPU (when reset) */
    if((DMATxDescToSet->Status & ETH_DMATxDesc_OWN) != (u32)RESET) {
        /* Return ERROR: OWN bit set */
        return tee_DescriptorLocked;
    }

    /* Setting the Frame Length: bits[12:0] */
    DMATxDescToSet->ControlBufferSize = (FrameLength & ETH_DMATxDesc_TBS1);

    /* Setting the last segment and first segment bits (in this case a frame is transmitted in one descriptor) */
    DMATxDescToSet->Status |= ETH_DMATxDesc_LS | ETH_DMATxDesc_FS;

    /* Set Own bit of the Tx descriptor Status: gives the buffer back to ETHERNET DMA */
    DMATxDescToSet->Status |= ETH_DMATxDesc_OWN;

    /* When Tx Buffer unavailable flag is set: clear it and resume transmission */
    if ((ETH->DMASR & ETH_DMASR_TBUS) != (u32)RESET) {
        /* Clear TBUS ETHERNET DMA flag */
        ETH->DMASR = ETH_DMASR_TBUS;
        /* Resume DMA transmission*/
        ETH->DMATPDR = 0;
    }

    /* Update the ETHERNET DMA global Tx descriptor with next Tx decriptor */  
    /* Chained Mode */
    if((DMATxDescToSet->Status & ETH_DMATxDesc_TCH) != (uint32_t)RESET) {     
      /* Selects the next DMA Tx descriptor list for next buffer to send */ 
      DMATxDescToSet = (ETH_DMADESCTypeDef*) (DMATxDescToSet->Buffer2NextDescAddr);    
    }
    else { /* Ring Mode */
      if((DMATxDescToSet->Status & ETH_DMATxDesc_TER) != (uint32_t)RESET) {
	/* Selects the first DMA Tx descriptor for next buffer to send: last Tx descriptor was used */
	DMATxDescToSet = (ETH_DMADESCTypeDef*) (ETH->DMATDLAR);      
      }
      else {  
	/* Selects the next DMA Tx descriptor list for next buffer to send */
	DMATxDescToSet = (ETH_DMADESCTypeDef*) ((uint32_t)DMATxDescToSet + 0x10 + ((ETH->DMABMR & ETH_DMABMR_DSL) >> 2));      
      }
    }

    return tee_OK;
}



/**
 * Gets size of received packet
 */
inline ttc_eth_errorcode_e stm32_eth_get_received_size(u32_t* Size)
{
    // from stm32_eth.c
    extern ETH_DMADESCTypeDef* DMARxDescToGet;

    if(((DMARxDescToGet->Status & ETH_DMARxDesc_OWN) == (u32_t)RESET) &&
       ((DMARxDescToGet->Status & ETH_DMARxDesc_ES) == (u32_t)RESET) &&
       ((DMARxDescToGet->Status & ETH_DMARxDesc_LS) != (u32_t)RESET) &&
       ((DMARxDescToGet->Status & ETH_DMARxDesc_FS) != (u32_t)RESET)) {
        /* Get the size of the packet: including 4 bytes of the CRC */
        *Size = ETH_GetDMARxDescFrameLength(DMARxDescToGet);
    }
    else
        return tee_DescriptorLocked;

    return tee_OK;
}


inline ttc_eth_errorcode_e stm32_eth_get_input_buffer(u8_t** Buffer, u32_t* FrameLength)
{
    // from stm32_eth.c
    extern ETH_DMADESCTypeDef* DMARxDescToGet;
    
    /* Check if the descriptor is owned by the ETHERNET DMA (when set) or CPU (when reset) */
    if((DMARxDescToGet->Status & ETH_DMARxDesc_OWN) != (u32)RESET) {
        if ((ETH->DMASR & ETH_DMASR_RBUS) != (u32)RESET) {
            /* Clear RBUS ETHERNET DMA flag */
            ETH->DMASR = ETH_DMASR_RBUS;
            /* Resume DMA reception */
            ETH->DMARPDR = 0;
        }

        /* Return error: OWN bit set */
        return tee_DescriptorLocked;
    }

    if(((DMARxDescToGet->Status & ETH_DMARxDesc_ES) == (u32)RESET) &&
       ((DMARxDescToGet->Status & ETH_DMARxDesc_LS) != (u32)RESET) &&
       ((DMARxDescToGet->Status & ETH_DMARxDesc_FS) != (u32)RESET)) {
        /* Get the Frame Length of the received packet: substruct 4 bytes of the CRC */
        *FrameLength = ((DMARxDescToGet->Status & ETH_DMARxDesc_FL) >> ETH_DMARxDesc_FrameLengthShift ) - 4;

        /* Get the addrees of the actual buffer */
        *Buffer = (u8_t*) DMARxDescToGet->Buffer1Addr;
    }
    else
        return tee_DescriptorLocked;

    return tee_OK;
}


inline ttc_eth_errorcode_e stm32_eth_resume_input()
{
  extern ETH_DMADESCTypeDef* DMARxDescToGet;
  /* Set Own bit of the Rx descriptor Status: gives the buffer back to ETHERNET DMA */
  DMARxDescToGet->Status = ETH_DMARxDesc_OWN; 
 
  /* When Rx Buffer unavailable flag is set: clear it and resume reception */
  if ((ETH->DMASR & ETH_DMASR_RBUS) != (uint32_t)RESET)  
  {
    /* Clear RBUS ETHERNET DMA flag */
    ETH->DMASR = ETH_DMASR_RBUS;
    /* Resume DMA reception */
    ETH->DMARPDR = 0;
  }
  
  /* Update the ETHERNET DMA global Rx descriptor with next Rx decriptor */      
  /* Chained Mode */
  if((DMARxDescToGet->ControlBufferSize & ETH_DMARxDesc_RCH) != (uint32_t)RESET)
  {     
    /* Selects the next DMA Rx descriptor list for next buffer to read */ 
    DMARxDescToGet = (ETH_DMADESCTypeDef*) (DMARxDescToGet->Buffer2NextDescAddr);    
  }
  else /* Ring Mode */
  {   
    if((DMARxDescToGet->ControlBufferSize & ETH_DMARxDesc_RER) != (uint32_t)RESET)
    {
      /* Selects the first DMA Rx descriptor for next buffer to read: last Rx descriptor was used */
      DMARxDescToGet = (ETH_DMADESCTypeDef*) (ETH->DMARDLAR);      
    }
    else
    { 
      /* Selects the next DMA Rx descriptor list for next buffer to read */
      DMARxDescToGet = (ETH_DMADESCTypeDef*) ((uint32_t)DMARxDescToGet + 0x10 + ((ETH->DMABMR & ETH_DMABMR_DSL) >> 2));      
    }
  }

  ETH_DMAClearITPendingBit(ETH_DMA_IT_R);
  ETH_DMAClearITPendingBit(ETH_DMA_IT_NIS);

  return tee_OK;
}

inline ttc_eth_errorcode_e stm32_eth_drop_input_buffer()
{
  ETH_DropRxPkt();
  return tee_OK;
}

inline ttc_eth_errorcode_e stm32_eth_set_mac_addr(u8_t* macaddr)
{
    ETH_MACAddressConfig(ETH_MAC_Address0, macaddr);
    return tee_OK;
}

inline ttc_eth_errorcode_e stm32_eth_get_mac_addr(u8_t* macaddr)
{
    ETH_GetMACAddress(ETH_MAC_Address0, macaddr);
    return tee_OK;
}


//} Function definitions
//{ private functions (ideally) -------------------------------------------------

//} private functions
