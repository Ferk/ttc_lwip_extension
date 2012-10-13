/*{ ttc_eth::.c ************************************************
 
                      The ToolChain
                      
   Support for Ethernet
   
   written by Fernando Carmona Varo 2012
   
 
}*/
#include "ttc_ethernet.h"
#include "ttc_gpio.h"


/* initializes single ETH
 * @param ETH_Index     Device index of ETH to init (0..ttc_eth_get_max_index()-1)
 * @param ETH_Generic   Filled out struct ttc_eth_generic_t
 * @param ETH_Arch      Empty structure which will be filled with architecture dependend data
 * @return                == 0: ETH has been initialized successfully; != 0: error-code
 */
inline ttc_eth_errorcode_e ttc_eth_init(u8_t ETH_Index, ttc_eth_generic_t* ETH_Generic, ttc_eth_architecture_t* ETH_Arch)
{
#ifdef TARGET_ARCHITECTURE_STM32
    return stm32_eth_init(ETH_Index,ETH_Generic,ETH_Arch);
#endif
    return tee_NotImplemented;
}
 

/* fills out given ETH_Generic with default values for indexed ETH
 * @param ETH_Index     Device index of ETH to use
 * @param ETH_Generic   Pointer to struct ttc_eth_generic_t
 * @return  == 0:         *ETH_Generic has been initialized successfully; != 0: error-code
 */
inline ttc_eth_errorcode_e ttc_eth_get_defaults(u8_t ETH_Index, ttc_eth_generic_t* ETH_Generic)
{    
#ifdef TARGET_ARCHITECTURE_STM32    
    return stm32_eth_get_defaults( ETH_Index, ETH_Generic );
#endif
    return tee_NotImplemented;   
}

/*
 * Return the address of the buffer pointed by the current output descriptor.
 * This is the address of the original descriptor buffer, it's intended for it to be filled with data.
 * After the Data is copied into the buffer a 'ttc_eth_send_output_buffer' call should be made to send the data and release the buffer for further output.
 * @param ETH_Index   Device index of ETH to use
 * @param Buffer      Address of the pointer to be pointed towards the output buffer
 * @return == 0:      buffer address successfully returned
 */
inline ttc_eth_errorcode_e ttc_eth_get_output_buffer(u8_t ETH_Index, u8_t** Buffer)
{
#ifdef TARGET_ARCHITECTURE_STM32
    ETH_Index=ETH_Index; //unused (only 1 ETH in stm32_eth)
    return stm32_eth_get_output_buffer(Buffer);
#endif
    return tee_NotImplemented;
}

/*
 * Sends the data from the current output buffer as a frame through the ethernet.
 * @param ETH_Index   Device index of ETH to use
 * @param FrameLength Size of the data to sent from the buffer
 * @return == 0:      Data sucessfully sent
 */
inline ttc_eth_errorcode_e ttc_eth_send_output_buffer(u8_t ETH_Index, u16 FrameLength)
{
#ifdef TARGET_ARCHITECTURE_STM32
    ETH_Index=ETH_Index; //unused (only 1 ETH in stm32_eth)
    return stm32_eth_send_output_buffer(FrameLength);
#endif
    return tee_NotImplemented;
}

 
/*
 * Gets size of received frame (size cero if no frame received)
 * @param ETH_Index   Device index of ETH to use
 * @param FrameLength Variable to be filled with the size
 * @return == 0:      Valid size returned
 */
inline ttc_eth_errorcode_e ttc_eth_get_received_size(u8_t ETH_Index, u32_t* Size)
{
#ifdef TARGET_ARCHITECTURE_STM32
    ETH_Index=ETH_Index; //unused (only 1 ETH in stm32_eth)
    return stm32_eth_get_received_size(Size);
#endif
    return tee_NotImplemented;
}

/*
 * Gets the received frame (its assigned buffer and length).
 * Note that the buffer should not be freed, since this is the address of the original descriptor buffer.
 * After the Buffer is processed a 'ttc_eth_resume_input()' call should be made to release the buffer for further input. 
 * @param ETH_Index   Device index of ETH to use
 * @param Buffer      Variable to be filled with a pointer to the buffer with the data
 * @param FrameLength Variable to be filled with the size
 * @return == 0:      Data sucessfully retrieved
 */
inline ttc_eth_errorcode_e ttc_eth_get_input_buffer(u8_t ETH_Index, u8_t** Buffer, u32_t* FrameLength)
{
#ifdef TARGET_ARCHITECTURE_STM32
    ETH_Index=ETH_Index; //unused (only 1 ETH in stm32_eth)
    return stm32_eth_get_input_buffer(Buffer,FrameLength);
#endif
    return tee_NotImplemented;
}

/*
 * Resumes reception, reset flags and gives control back to DMA.
 * This is required after a get_input_buffer to keep handling received frames.
 * @param ETH_Index   Device index of ETH to use
 * @return == 0:      Reception resumed successfully
 */
inline ttc_eth_errorcode_e ttc_eth_resume_input(u8_t ETH_Index)
{
#ifdef TARGET_ARCHITECTURE_STM32
    ETH_Index=ETH_Index; //unused (only 1 ETH in stm32_eth)
    return stm32_eth_resume_input();
#endif
    return tee_NotImplemented;
}

/*
 * Discards the received frame from the current descriptor
 * @return == 0:      Reception resumed successfully
 */
ttc_eth_errorcode_e ttc_eth_drop_input_buffer()
{
#ifdef TARGET_ARCHITECTURE_STM32
    //ETH_Index=ETH_Index; //unused (only 1 ETH in stm32_eth)
    return stm32_eth_drop_input_buffer();
#endif
    return tee_NotImplemented;
}

/*
 * Set the physical address (MAC) of the device
 * @param ETH_Index   Device index of ETH to use
 * @param macaddr     Variable containing the MAC address to set
 * @return == 0:      Data sucessfully sent
 */
inline ttc_eth_errorcode_e ttc_eth_set_mac_addr(u8_t ETH_Index, u8_t* macaddr)
{
#ifdef TARGET_ARCHITECTURE_STM32
    ETH_Index=ETH_Index; //unused (only 1 ETH in stm32_eth)
    return stm32_eth_set_mac_addr(macaddr);
#endif
    return tee_NotImplemented;
}

/*
 * Get the physical address (MAC) of the device
 * @param ETH_Index   Device index of ETH to use
 * @param macaddr     Variable to be filled with the current MAC address
 * @return == 0:      Data sucessfully sent
 */ 
inline ttc_eth_errorcode_e ttc_eth_get_mac_addr(u8_t ETH_Index, u8_t* macaddr)
{
#ifdef TARGET_ARCHITECTURE_STM32
    ETH_Index=ETH_Index; //unused (only 1 ETH in stm32_eth)
    return stm32_eth_get_mac_addr(macaddr);
#endif
    return tee_NotImplemented;
}

