/*{ test_eth::main.c ********************************************
 *                        ToolChain_STM32
 * 
 * Project test_eth
 * Author: Fer
 *
 * This code is part of a project for the ToolChain_STM32 framework for 
 * microcontrollers with ARM CortexM3 CPU core.
 * ToolChain_STM32 is a free of charge set of open source scripts that download 
 * and install all tools being required to compile, flash and debug software for 
 * the before mentioned uCs. No additional closed source software is required.
 *
 * ToolChain_STM32 has been written 2010-2012 by Gregor Rebel
 *
 * 
 * Support for external tools
 *   + QtCreator
 *     The subfolder QtCreator/ contains an automatically updated creator-project.
 *     This project can be loaded into QtCreator. Using this IDE seems to be more comfortable
 *     for many developers.
 * 
 *   + Eclipse
 *     Support for Eclipse should come soon.
 *
 *
 * Basic framework template for
 *   - Olimex STM32-P107/ STM32-H107/ STM32-LCD/ .. eval boards
 *   - StdPeripheralsLibrary with CMSIS
 *   - GNU toolchain from CodeSourcery G++
 *   - FreeRTOS
 *   - FixPointLib 
 *   - TCP/IP-Stacks
 *   - Color LCD Panel support
 *   - local/ remote debugging (JTAG-Adapter may be connected to different computer)
 *
 * Coding-style
 *  + gnu99
 *    It's year 2012, get a decent compiler if your's does not comply with this.
 *    arm-none-eabi-gcc-4.4.1 does support gnu99.
 *     
 *  + Text Folding
 *    This source makes use of text folding to improve hierarchical readability 
 *    It has been written with jEdit + plugin Configurable Fold Handler.
 *    Activate folding in your favourite text-editor for { and } 
 *    (fold-start and -end) for best visual experience.
 *
 * Prerequisites:
 *   ToolChain_STM32 (http://hlb-labor.de/cortexm3)
 *   Latest version should be downloaded and installed automatically by issuing ./compile.sh
 *
 * Compile source + flash binary onto uC:  
 *   modify _/interface.cfg to suit to your JTAG programmer 
 *   ./compile.sh
 *
 * Debug:
 * - start openocd as gdb-server (may even run on another computer connected over network)
 *   ./_/gdb_server.sh
 * - start gdb text user-interface in another shell
 *   ./_/gdb.sh
 * 
 * Configuration of source-code
 *  - FreeRTOSConfig.h
 *    configure behaviour and features of FeeRTOS
 *
 *  - stm32f10x_conf.h
 *    modify according to your hardware (if differing from above) 
 *  
 *  - stm32f10x_it.c/ stm32f10x_it.h
 *    implement your own interrupthandlers in existing functions
 *    
 *  - main.c (this file)
 *    Ideally, the main.c file contains only some function calls of names like
 *    init_XXX() or start_XXX(). The intention of this file is to show an overview
 *    of your project and how to use your code. 
 *    Place real algorithms and implementations into extra .c and .h files and include 
 *    them from here. It should be easy to copy your code from this main.c into foregin sources.
 *    This makes reuse of your code much easier.
 *    
 *  - makefile
 *    check activate_project.sh for local makefile extensions
 * 
 *  - template.c_/ template.h_
 *    can be used as source-code templates for your own code
 * 
}*/
//{ Defines/ TypDefs *****************************************************

//} Defines
//{ Includes *************************************************************

#include "extensions.h"  // will include activated extensions

#include "stm32_eth.h"

#include "system_stm32f10x.h"
#include "stm32f10x_conf.h"
#include "ttc_gpio.h"

#ifdef TTC_LED1
ttc_Port_t Led1Port;
#ifdef TTC_LED2
ttc_Port_t Led2Port;
#endif
#endif


#define ETH_RXBUFNB        4
#define ETH_TXBUFNB        2

#define TxBufferSize1           64
#define RxBufferSize1           64

ETH_DMADESCTypeDef  DMARxDscrTab[ETH_RXBUFNB], DMATxDscrTab[ETH_TXBUFNB]; /* Ethernet Rx & Tx DMA Descriptors */
uint8_t Rx_Buff[ETH_RXBUFNB][ETH_MAX_PACKET_SIZE], Tx_Buff[ETH_TXBUFNB][ETH_MAX_PACKET_SIZE]; /* Ethernet buffers */

ETH_DMADESCTypeDef  *DMATxDesc = DMATxDscrTab;
//extern ETH_DMADESCTypeDef  *DMATxDescToSet;
//extern ETH_DMADESCTypeDef  *DMARxDescToGet;


//} Includes
//{ Global Variables *****************************************************

//} Global Variables
//{ Function prototypes **************************************************

int main(void);

// task which runs main eventloop
void taskMain(void *pvParameters);

//} Function prototypes
//{ Function definitions *************************************************

        int main(void) {
    startExtensions(taskMain, 512, NULL);  // will call taskMain() as first task
}


/**
  * @brief  Configures the different GPIO ports.
  * @param  None
  * @retval None
  */
void my_eth_GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
  
    /* ETHERNET pins configuration */
    /* AF Output Push Pull:
          - ETH_MII_MDIO / ETH_RMII_MDIO: PA2
          - ETH_MII_MDC / ETH_RMII_MDC: PC1
          - ETH_MII_TXD2: PC2
          - ETH_MII_TX_EN / ETH_RMII_TX_EN: PB11
          - ETH_MII_TXD0 / ETH_RMII_TXD0: PB12
          - ETH_MII_TXD1 / ETH_RMII_TXD1: PB13
          - ETH_MII_PPS_OUT / ETH_RMII_PPS_OUT: PB5
          - ETH_MII_TXD3: PB8 */

    /* Configure PA2 as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Configure PC1, PC2 and PC3 as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    /* Configure PB5, PB8, PB11, PB12 and PB13 as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_11 |
            GPIO_Pin_12 | GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /**************************************************************/
    /*               For Remapped Ethernet pins                   */
    /*************************************************************/
    /* Input (Reset Value):
          - ETH_MII_CRS CRS: PA0
          - ETH_MII_RX_CLK / ETH_RMII_REF_CLK: PA1
          - ETH_MII_COL: PA3
          - ETH_MII_RX_DV / ETH_RMII_CRS_DV: PD8
          - ETH_MII_TX_CLK: PC3
          - ETH_MII_RXD0 / ETH_RMII_RXD0: PD9
          - ETH_MII_RXD1 / ETH_RMII_RXD1: PD10
          - ETH_MII_RXD2: PD11
          - ETH_MII_RXD3: PD12
          - ETH_MII_RX_ER: PB10 */

    /* ETHERNET pins remapp in STM3210C-EVAL board: RX_DV and RxD[3:0] */
    GPIO_PinRemapConfig(GPIO_Remap_ETH, DISABLE);

    /* Configure PA0, PA1 and PA3 as input */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_7 ;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Configure PB10 as input */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* Configure PC3 as input */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    /* Configure PD8, PD9, PD10, PD11 and PD12 as input */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOC, &GPIO_InitStructure); /**/

    /* MCO pin configuration------------------------------------------------- */
    /* Configure MCO (PA8) as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void my_eth_NVIC_Configuration(void){

    NVIC_InitTypeDef   NVIC_InitStructure;

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

}

void my_eth_Ethernet_Init(void){

    ETH_InitTypeDef ETH_InitStructure;

    
#ifdef MII_MODE /* Mode MII with STM3210C-EVAL  */
  GPIO_ETH_MediaInterfaceConfig(GPIO_ETH_MediaInterface_MII);
  /* Get HSE clock = 25MHz on PA8 pin (MCO) */
  RCC_MCOConfig(RCC_MCO_HSE);
#else
  GPIO_ETH_MediaInterfaceConfig(GPIO_ETH_MediaInterface_RMII);
  
  /* Set PLL3 clock output to 50MHz (25MHz /5 *10 =50MHz) */
  RCC_PLL3Config(RCC_PLL3Mul_10);
  /* Enable PLL3 */
  RCC_PLL3Cmd(ENABLE);
  /* Wait till PLL3 is ready */
  while (RCC_GetFlagStatus(RCC_FLAG_PLL3RDY) == RESET);

  /* Get PLL3 clock on PA8 pin (MCO) */
  RCC_MCOConfig(RCC_MCO_PLL3CLK);
#endif

    /* Reset ETHERNET on AHB Bus */
    ETH_DeInit();

    /* Software reset */
    ETH_SoftwareReset();

    /* Wait for software reset */
    while(ETH_GetSoftwareResetStatus()==SET);

    /* ETHERNET Configuration ------------------------------------------------------*/
    /* Call ETH_StructInit if you don't like to configure all ETH_InitStructure parameter */
    ETH_StructInit(&ETH_InitStructure);

    /* Fill ETH_InitStructure parametrs */
    /*------------------------   MAC   -----------------------------------*/
    ETH_InitStructure.ETH_AutoNegotiation = ETH_AutoNegotiation_Disable  ;
    //ETH_InitStructure.ETH_Speed = ETH_Speed_100M;
    ETH_InitStructure.ETH_LoopbackMode = ETH_LoopbackMode_Disable;
    //ETH_InitStructure.ETH_Mode = ETH_Mode_FullDuplex;
    ETH_InitStructure.ETH_RetryTransmission = ETH_RetryTransmission_Disable;
    ETH_InitStructure.ETH_AutomaticPadCRCStrip = ETH_AutomaticPadCRCStrip_Disable;
    ETH_InitStructure.ETH_ReceiveAll = ETH_ReceiveAll_Enable;
    ETH_InitStructure.ETH_BroadcastFramesReception = ETH_BroadcastFramesReception_Disable;
    ETH_InitStructure.ETH_PromiscuousMode = ETH_PromiscuousMode_Disable;
    ETH_InitStructure.ETH_MulticastFramesFilter = ETH_MulticastFramesFilter_None;//ETH_MulticastFramesFilter_Perfect;
    ETH_InitStructure.ETH_UnicastFramesFilter = ETH_UnicastFramesFilter_Perfect;
    ETH_InitStructure.ETH_Mode = ETH_Mode_FullDuplex;
    ETH_InitStructure.ETH_Speed = ETH_Speed_100M;


  /*------------------------   DMA   -----------------------------------*/  
  
  /* When we use the Checksum offload feature, we need to enable the Store and Forward mode: 
  the store and forward guarantee that a whole frame is stored in the FIFO, so the MAC can insert/verify the checksum, 
  if the checksum is OK the DMA can handle the frame otherwise the frame is dropped */
  ETH_InitStructure.ETH_DropTCPIPChecksumErrorFrame = ETH_DropTCPIPChecksumErrorFrame_Enable; 
  ETH_InitStructure.ETH_ReceiveStoreForward = ETH_ReceiveStoreForward_Enable;         
  ETH_InitStructure.ETH_TransmitStoreForward = ETH_TransmitStoreForward_Enable;     
 
  ETH_InitStructure.ETH_ForwardErrorFrames = ETH_ForwardErrorFrames_Disable;       
  ETH_InitStructure.ETH_ForwardUndersizedGoodFrames = ETH_ForwardUndersizedGoodFrames_Disable;   
  ETH_InitStructure.ETH_SecondFrameOperate = ETH_SecondFrameOperate_Enable;                                                          
  ETH_InitStructure.ETH_AddressAlignedBeats = ETH_AddressAlignedBeats_Enable;      
  ETH_InitStructure.ETH_FixedBurst = ETH_FixedBurst_Enable;                
  ETH_InitStructure.ETH_RxDMABurstLength = ETH_RxDMABurstLength_32Beat;          
  ETH_InitStructure.ETH_TxDMABurstLength = ETH_TxDMABurstLength_32Beat;                                                                 
  ETH_InitStructure.ETH_DMAArbitration = ETH_DMAArbitration_RoundRobin_RxTx_2_1;

    
    unsigned int PhyAddr;
    for(PhyAddr = 1; 32 >= PhyAddr; PhyAddr++)
      if((0x0006 == ETH_ReadPHYRegister(PhyAddr,2)) && (0x1c50 == (ETH_ReadPHYRegister(PhyAddr,3)&0xFFF0))) break;
    
    if(32 < PhyAddr)
    {

#ifdef useUARTDebug
        UART_write_string(USART3,"Ethernet Phy Not Found\r\n");
#endif

        //  while(1);
    }else{
        /* Configure Ethernet */
        if(0 == ETH_Init(&ETH_InitStructure, PhyAddr))
        {

#ifdef useUARTDebug
            UART_write_string(USART3,"Ethernet Initialization Failed\r\n");
#endif
            Assert(FALSE,ec_UNKNOWN);
        }
}
    
    /* Enable the Ethernet Rx Interrupt */
    ETH_DMAITConfig(ETH_DMA_IT_NIS | ETH_DMA_IT_R, ENABLE);

}


void taskMain(void *pvParameters) {

    // initialize your task
    pvParameters = pvParameters; // avoids warning: unused parameter 'pvParameters'

#ifdef TTC_LED1
    ttc_initPort(&Led1Port, TTC_LED1, tgm_output_push_pull);
    ttc_portClr(&Led1Port);
#ifdef TTC_LED2
    ttc_initPort(&Led2Port, TTC_LED2, tgm_output_push_pull);
    ttc_portClr(&Led2Port);
#endif /* TTC_LED2 */
#endif /* TTC_LED1 */

    SystemInit();
    
    /*****************/

    /*** Enable ETHERNET clock  ***/
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_ETH_MAC | RCC_AHBPeriph_ETH_MAC_Tx |
                          RCC_AHBPeriph_ETH_MAC_Rx, ENABLE);
    

  /* Enable GPIOs and ADC1 clocks */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE | RCC_APB2Periph_AFIO, ENABLE);

    
    /****/

    my_eth_NVIC_Configuration();
    my_eth_GPIO_Configuration();
    my_eth_Ethernet_Init();
    
    /****/
    

    //Enable MAC Interrupts
    //ETH_MACITConfig(ETH_MAC_IT_MMC, ENABLE);
    //ETH_MAC_IT_MMC

    /* //Enable Wakeup Interrupts */
    /* ETH_GlobalUnicastWakeUpCmd(ENABLE); */
    /* ETH_WakeUpFrameDetectionCmd(ENABLE); */

    
    /***** ETHERNETIF.C *****/
    //Initialize Tx Descriptors list: Chain Mode
    ETH_DMATxDescChainInit(DMATxDscrTab, &Tx_Buff[0][0], ETH_TXBUFNB);
    // Initialize Rx Descriptors list: Chain Mode
    ETH_DMARxDescChainInit(DMARxDscrTab, &Rx_Buff[0][0], ETH_RXBUFNB);

    /* Enable Ethernet Rx interrrupt */
    { int i;
      for(i=0; i<ETH_RXBUFNB; i++)
	{
	  ETH_DMARxDescReceiveITConfig(&DMARxDscrTab[i], ENABLE);
	}
    }
        
    // Enable MAC and DMA transmission and reception
    ETH_Start();
    

    /****************/

    while (1) { 
      // code your main event loop here
#ifdef TTC_LED2
        ttc_portSet(&Led2Port);
#endif /* TTC_LED2 */
	mSleep(500);
#ifdef TTC_LED2
        ttc_portClr(&Led2Port);
#endif /* TTC_LED2 */
	mSleep(500);
    }
}

//} Function definitions
