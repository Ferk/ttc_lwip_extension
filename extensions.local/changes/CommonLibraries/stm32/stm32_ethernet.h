#ifndef STM32_ETH_H
#define STM32_ETH_H

/*{ stm32_eth.h **********************************************************
 
                      The ToolChain
                      
   Ethernet interface for ARM CortexM3 microcontrollers.
   Currently tested to comply to STM32F107.
   
   written by Fernando Carmona Varo 2012

 
}*/
//{ Defines/ TypeDefs ****************************************************

//} Defines
//{ Includes *************************************************************

#include "ttc_ethernet_types.h"
#include "ttc_task.h"

 // Basic set of helper functions
#include "stm32_gpio.h"
#include "stm32_registers.h"
#include <stdlib.h>

#define USE_STDPERIPH_DRIVER
#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "stm32_eth.h"
#include "misc.h"

/*{ optional
// ADC
#include "stm32f10x_adc.h"
#include "stm32f10x_dma.h"

// Timers
#include "stm32f10x_tim.h"

#include "stm32f10x_it.h"
#include "system_stm32f10x.h"

#include "bits.h"
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"
}*/

//} Includes
//{ Structures/ Enums ****************************************************

typedef struct {  // architecture specific configuration data of single ETH 
  // Port Pins
  stm32_Port_t PortRMII_MDIO;
  stm32_Port_t PortRMII_MDC;
  stm32_Port_t PortRMII_TX_EN;
  stm32_Port_t PortRMII_TXD0;
  stm32_Port_t PortRMII_TXD1;
  stm32_Port_t PortRMII_PPS_OUT;
  stm32_Port_t PortMII_MDIO;
  stm32_Port_t PortMII_MDC;
  stm32_Port_t PortMII_TXD2;
  stm32_Port_t PortMII_TX_EN;
  stm32_Port_t PortMII_TXD0;
  stm32_Port_t PortMII_TXD1;
  stm32_Port_t PortMII_PPS_OUT;
  stm32_Port_t PortMII_TXD3;
  stm32_Port_t PortRMII_REF_CLK;
  stm32_Port_t PortRMII_CRS_DV;
  stm32_Port_t PortRMII_RXD0;
  stm32_Port_t PortRMII_RXD1;
  stm32_Port_t PortMII_CRS_WKUP;
  stm32_Port_t PortMII_RX_CLK;
  stm32_Port_t PortMII_COL;
  stm32_Port_t PortMII_RX_DV;
  stm32_Port_t PortMII_TX_CLK;
  stm32_Port_t PortMII_RXD0;
  stm32_Port_t PortMII_RXD1;
  stm32_Port_t PortMII_RXD2;
  stm32_Port_t PortMII_RXD3;
  stm32_Port_t PortMII_RX_ER;
} __attribute__((__packed__)) stm32_eth_architecture_t;

//} Structures/ Enums
//{ Function prototypes **************************************************

/* resets library. Automatically called.
 */
void stm32_eth_reset_all();

/* fills out given ETH_ with default values for indexed ETH
 * @param ETH_Index     device index of ETH to init (1..ttc_eth_get_max_index())
 * @param ETH_Generic   pointer to struct ttc_eth_generic_t
 * @return  == 0:         *ETH_Generic has been initialized successfully; != 0: error-code
 */
ttc_eth_errorcode_e stm32_eth_get_defaults(u8_t ETH_Index, ttc_eth_generic_t* ETH_Generic);

/* initializes single ETH
 * @param ETH_Index     device index of ETH to init (1..ttc_eth_get_max_index())
 * @param ETH_Generic   filled out struct ttc_eth_generic_t
 * @return  == 0:         ETH has been initialized successfully; != 0: error-code
 */
ttc_eth_errorcode_e stm32_eth_init(u8_t ETH_Index, ttc_eth_generic_t* ETH_Generic, stm32_eth_architecture_t* ETH_Arch);

/*
 * Return the address of the buffer pointed by the current output descriptor.
 * This is the address of the original descriptor buffer, it's intended for it to be filled with data.
 * After the Data is copied into the buffer a 'ttc_eth_send_output_buffer' call should be made to send 
 * the data and release the buffer for further output.
 * @param Buffer      Address of the pointer to be pointed towards the output buffer
 * @return == 0:      buffer address successfully returned
 */
ttc_eth_errorcode_e stm32_eth_get_output_buffer(u8_t** Buffer);

/*
 * Sends the data from the current output buffer as a frame through the ethernet.
 * @param FrameLength Size of the data to sent from the buffer
 * @return == 0:      Data sucessfully sent
 */
ttc_eth_errorcode_e stm32_eth_send_output_buffer(u16 FrameLength);

/*
 * Gets size of received frame (size cero if no frame received)
 * @param FrameLength Variable to be filled with the size
 * @return == 0:      Valid size returned
 */
ttc_eth_errorcode_e stm32_eth_get_received_size(u32_t* Size);

/*
 * Gets the received frame (its assigned buffer and length).
 * Note that the buffer should not be freed, since this is the address of the original descriptor buffer.
 * After the Buffer is processed a 'ttc_eth_resume_input()' call should be made to release the buffer for further input. 
 * @param Buffer      Variable to be filled with a pointer to the buffer with the data
 * @param FrameLength Variable to be filled with the size
 * @return == 0:      Data sucessfully retrieved
 */
ttc_eth_errorcode_e stm32_eth_get_input_buffer(u8_t** Buffer, u32_t* FrameLength);

/*
 * Resumes reception, reset flags and gives control back to DMA.
 * This is required after a get_input_buffer to keep handling received frames.
 * @return == 0:      Reception resumed successfully
 */
ttc_eth_errorcode_e stm32_eth_resume_input();

/*
 * Discards the received frame from the current descriptor
 * @return == 0:      Reception resumed successfully
 */
ttc_eth_errorcode_e stm32_eth_drop_input_buffer();


/*
 * Set the physical address (MAC) of the device
 * @param macaddr     Variable containing the MAC address to set
 * @return == 0:      Data sucessfully sent
 */
ttc_eth_errorcode_e stm32_eth_set_mac_addr(u8_t* macaddr);

/*
 * Get the physical address (MAC) of the device
 * @param macaddr     Variable to be filled with the current MAC address
 * @return == 0:      Data sucessfully sent
 */
ttc_eth_errorcode_e stm32_eth_get_mac_addr(u8_t* macaddr);

  
//} Function prototypes

#endif //STM32_ETH_H
