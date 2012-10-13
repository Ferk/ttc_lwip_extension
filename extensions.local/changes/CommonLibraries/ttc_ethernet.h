#ifndef TTC_ETH_H
#define TTC_ETH_H

/*{ ttc_eth.h **********************************************************
 
                      The ToolChain
                      
   Device independent support for Ethernet
   
   Currently implemented architectures: stm32

   
   written by Fernando Carmona 2012
   
   Reference:
   
   RM008 - STM32 Reference Manual p. 757
   -> http://www.st.com/internet/com/TECHNICAL_RESOURCES/TECHNICAL_LITERATURE/REFERENCE_MANUAL/CD00171190.pdf

   
   How it works
   
   Each makefile.100_board* can define one or more ETHs being routed to connectors.
   This is done via lines of this type:
     COMPILE_OPTS += -D<OPTION>=<VALUE>
   This sets compile option <OPTION> to value <VALUE>
   
   ETHn are enumerated starting at index 1 (e.g. ETH1, ETH2, ..).
   Each ETHn can be configured via this constants:
   TTC_ETH1     <ETH>                sets internal ETH device to use
   TTC_ETH1_TX  <GPIO_BANK>,<PIN_NO>   sets port bit to use for TX pin
   TTC_ETH1_RX  <GPIO_BANK>,<PIN_NO>   sets port bit to use for RX pin
   TTC_ETH1_RTS <GPIO_BANK>,<PIN_NO>   sets port bit to use for RTS pin
   TTC_ETH1_CTS <GPIO_BANK>,<PIN_NO>   sets port bit to use for CTS pin
   
}*/
//{ Defines/ TypeDefs ****************************************************
//} Defines
//{ Includes *************************************************************

#include "ttc_basic.h"
#include "ttc_ethernet_types.h"

#ifdef TARGET_ARCHITECTURE_STM32
#include "stm32_ethernet.h"

#else
  #error Ethernet interface not yet supported for this architecture
#endif

//} Includes
//{ Structures/ Enums ****************************************************

// architecture specific configuration data of single ETH 
#ifdef TARGET_ARCHITECTURE_STM32
typedef stm32_eth_architecture_t ttc_eth_architecture_t;
#endif

//} Structures/ Enums
//{ Global Variables *****************************************************

//} Global Variables
//{ Function prototypes **************************************************


/* initializes single ETH
 * @param ETH_Index     device index of ETH to init (0..ttc_eth_get_max_index()-1)
 * @param ETH_Generic   filled out struct ttc_eth_generic_t
 * @param ETH_Arch      empty structure which will be filled with architecture dependend data
 * @return                == 0: ETH has been initialized successfully; != 0: error-code
 */
ttc_eth_errorcode_e ttc_eth_init(u8_t ETH_Index, ttc_eth_generic_t* ETH_Generic, ttc_eth_architecture_t* ETH_Arch);
 

/* fills out given ETH_Generic with default values for indexed ETH
 * @param ETH_Index     device index of ETH to init (1..ttc_eth_get_max_index())
 * @param ETH_Generic   pointer to struct ttc_eth_generic_t
 * @return  == 0:         *ETH_Generic has been initialized successfully; != 0: error-code
 */
ttc_eth_errorcode_e ttc_eth_get_defaults(u8_t ETH_Index, ttc_eth_generic_t* ETH_Generic);

/*
 * Return the address of the buffer pointed by the current output descriptor.
 * This is the address of the original descriptor buffer, it's intended for it to be filled with data.
 * After the Data is copied into the buffer a 'ttc_eth_send_output_buffer' call should be made to send 
 * the data and release the buffer for further output.
 * @param Buffer      Address of the pointer to be pointed towards the output buffer
 * @return == 0:      buffer address successfully returned
 */
ttc_eth_errorcode_e ttc_eth_get_output_buffer(u8_t ETH_Index, u8_t** Buffer);

/*
 * Sends the data from the current output buffer as a frame through the ethernet.
 * @param FrameLength Size of the data to sent from the buffer
 * @return == 0:      Data sucessfully sent
 */
ttc_eth_errorcode_e ttc_eth_send_output_buffer(u8_t ETH_Index, u16 FrameLength);

 
/*
 * Gets size of received frame (size cero if no frame received)
 * @param FrameLength Variable to be filled with the size
 * @return == 0:      Valid size returned
 */
ttc_eth_errorcode_e ttc_eth_get_received_size(u8_t ETH_Index, u32_t* Size);

/*
 * Gets the received frame (its assigned buffer and length).
 * Note that the buffer should not be freed, since this is the address of the original descriptor buffer.
 * After the Buffer is processed a 'ttc_eth_resume_input()' call should be made to release the buffer for further input. 
 * @param Buffer      Variable to be filled with a pointer to the buffer with the data
 * @param FrameLength Variable to be filled with the size
 * @return == 0:      Data sucessfully retrieved
 */
ttc_eth_errorcode_e ttc_eth_get_input_buffer(u8_t ETH_Index, u8_t** Buffer, u32_t* FrameLength);

/*
 * Resumes reception, reset flags and gives control back to DMA.
 * This is required after a get_input_buffer to keep handling received frames.
 * @return == 0:      Reception resumed successfully
 */
ttc_eth_errorcode_e ttc_eth_resume_input(u8_t ETH_Index);

/*
 * Discards the received frame from the current descriptor
 * @return == 0:      Reception resumed successfully
 */
ttc_eth_errorcode_e ttc_eth_drop_input_buffer();

/*
 * Set the physical address (MAC) of the device
 * @param macaddr     Variable containing the MAC address to set
 * @return == 0:      Data sucessfully sent
 */
ttc_eth_errorcode_e ttc_eth_set_mac_addr(u8_t ETH_Index, u8_t* macaddr);

/*
 * Get the physical address (MAC) of the device
 * @param macaddr     Variable to be filled with the current MAC address
 * @return == 0:      Data sucessfully sent
 */ 
ttc_eth_errorcode_e ttc_eth_get_mac_addr(u8_t ETH_Index, u8_t* macaddr);

//} Function prototypes

#endif //TTC_ETH_H
