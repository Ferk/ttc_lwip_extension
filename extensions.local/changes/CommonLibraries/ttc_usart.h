#ifndef TTC_USART_H
#define TTC_USART_H

/*{ ttc_usart.h **********************************************************
 
                      The ToolChain
                      
   Device independent support for 
   Universal Synchronous Asynchronous Receiver and Transmitters (USARTs)
   
   Currently implemented architectures: stm32

   
   written by Gregor Rebel 2012
   
   Reference:
   
   RM008 - STM32 Reference Manual p. 757
   -> http://www.st.com/internet/com/TECHNICAL_RESOURCES/TECHNICAL_LITERATURE/REFERENCE_MANUAL/CD00171190.pdf

   
   How it works
   
   Each makefile.100_board* can define one or more USARTs being routed to connectors.
   This is done via lines of this type:
     COMPILE_OPTS += -D<OPTION>=<VALUE>
   This sets compile option <OPTION> to value <VALUE>
   
   USARTn are enumerated starting at index 1 (e.g. USART1, USART2, ..).
   Each USARTn can be configured via this constants:
   TTC_USART1     <USART>                sets internal USART device to use
   TTC_USART1_TX  <GPIO_BANK>,<PIN_NO>   sets port bit to use for TX pin
   TTC_USART1_RX  <GPIO_BANK>,<PIN_NO>   sets port bit to use for RX pin
   TTC_USART1_RTS <GPIO_BANK>,<PIN_NO>   sets port bit to use for RTS pin
   TTC_USART1_CTS <GPIO_BANK>,<PIN_NO>   sets port bit to use for CTS pin
   
   # Example excerpt from makefile.100_board_olimex_p107
   COMPILE_OPTS += -DTTC_USART1=USART3       # USART connected to RS232 connector 9-pin female
   COMPILE_OPTS += -DTTC_USART1_TX=GPIOD,8   # RS232 connector 9-pin female TX-pin
   COMPILE_OPTS += -DTTC_USART1_RX=GPIOD,9   # RS232 connector 9-pin female RX-pin
   COMPILE_OPTS += -DTTC_USART1_RTS=GPIOD,12 # RS232 connector 9-pin female RTS-pin
   COMPILE_OPTS += -DTTC_USART1_CTS=GPIOD,11 # RS232 connector 9-pin female CTS-pin
    
   COMPILE_OPTS += -DTTC_USART2=USART2       # USART connected to 10-pin male header connector
   COMPILE_OPTS += -DTTC_USART2_TX=GPIOD,4   # RS232 on 10-pin male header connector TX-Pin
   COMPILE_OPTS += -DTTC_USART2_RX=GPIOD,5   # RS232 on 10-pin male header connector RX-Pin

}*/
//{ Defines/ TypeDefs ****************************************************


//} Defines
//{ Includes *************************************************************

#include "ttc_basic.h"
#include "ttc_usart_types.h"

#ifdef TARGET_ARCHITECTURE_STM32
  #include "stm32_usart.h"
#endif

//} Includes
//{ Structures/ Enums ****************************************************

// architecture specific configuration data of single USART 
#ifdef TARGET_ARCHITECTURE_STM32
    typedef stm32_usart_architecture_t ttc_usart_architecture_t;
#endif

//} Structures/ Enums
//{ Global Variables *****************************************************



//} Global Variables
//{ Function prototypes **************************************************

/* returns amount of USART devices available on current uC
 * @return amount of available USART devices
 */
u8_t ttc_usart_get_max_index();

/* fills out given USART_Generic with default values for indexed USART
 * @param USART_Index     device index of USART to init (1..ttc_usart_get_max_index())
 * @param USART_Generic   pointer to struct ttc_usart_generic_t
 * @return  == 0:         *USART_Generic has been initialized successfully; != 0: error-code
 */
ttc_usart_errorcode_e ttc_usart_get_defaults(u8_t USART_Index, ttc_usart_generic_t* USART_Generic);

/* fills out given USART_Generic with maximum valid values for indexed USART
 * @param USART_Index     device index of USART to init (1..ttc_usart_get_max_index())
 * @param USART_Generic   pointer to struct ttc_usart_generic_t
 * @return  == 0:         *USART_Generic has been initialized successfully; != 0: error-code
 */
ttc_usart_errorcode_e ttc_usart_get_features(u8_t USART_Index, ttc_usart_generic_t* USART_Generic);

/* initializes single USART
 * @param USART_Index     device index of USART to init (0..ttc_usart_get_max_index()-1)
 * @param USART_Generic   filled out struct ttc_usart_generic_t
 * @param USART_Arch      empty structure which will be filled with architecture dependend data
 * @return                == 0: USART has been initialized successfully; != 0: error-code
 */
ttc_usart_errorcode_e ttc_usart_init(u8_t USART_Index, ttc_usart_generic_t* USART_Generic, ttc_usart_architecture_t* USART_Arch);

/* Send out given amount of raw bytes from buffer.
 * Will not interpret bytes in buffer.
 *
 * Note: USART must be initialized before!
 * Note: This function is partially thread safe (use it for each USART from one thread only)!
 * Note: This function blocks until all bytes have been sent!
 * 
 * @param USART_Index    device index of USART to init (0..ttc_usart_get_max_index()-1)
 * @param Buffer         memory location from which to read data
 * @param Amount         amount of bytes to send from Buffer[]
 * @return               == 0: Buffer has been sent successfully; != 0: error-code
 */
ttc_usart_errorcode_e ttc_usart_send_raw(u8_t USART_Index, const char* Buffer, u16_t Amount);

/* Send out given bytes from buffer until first zero byte or MaxLength reached.
 *
 * Note: USART must be initialized before!
 * Note: This function is partially thread safe (use it for each USART from one thread only)!
 * Note: This function blocks until all bytes have been sent!
 * 
 * @param USART_Index    device index of USART to init (0..ttc_usart_get_max_index()-1)
 * @param Buffer         memory location from which to read data
 * @param MaxLength      no more than this amount of bytes will be send
 * @return               == 0: Buffer has been sent successfully; != 0: error-code
 */
ttc_usart_errorcode_e ttc_usart_send_string(u8_t USART_Index, const char* Buffer, u16_t MaxLength);

/* Send out given data word (8 or 9 bits)
 *
 * Note: USART must be initialized before!
 * Note: This function is partially thread safe (use it for each USART from one thread only)!
 * Note: This function blocks until all bytes have been sent!
 *
 * @param USART_Index    device index of USART to init (0..ttc_usart_get_max_index()-1)
 * @param Byte           8 or 9 bit value to send
 * @return               == 0: Buffer has been sent successfully; != 0: error-code
 */
ttc_usart_errorcode_e ttc_usart_send_word(u8_t USART_Index, const u16_t Word);

/* Reads single data word from input buffer (8 or 9 bits)
 *
 * Note: USART must be initialized before!
 * Note: This function is partially thread safe (use it for each USART from one thread only)!
 * Note: This function blocks until a valid word has been received or timeout occurs
 *
 * @param USART_Index    device index of USART to init (0..ttc_usart_get_max_index()-1)
 * @param Word           pointer to 16 bit buffer where to store Word
 * @param TimeOut        >0: max microseconds to wait; ==0: no timeout
 * @return               == 0: Word has been read successfully; != 0: error-code
 */
ttc_usart_errorcode_e ttc_usart_read_word(u8_t USART_Index, u16_t* Word, u32_t TimeOut);

/* Reads single data byte from input buffer.
 *
 * Note: USART must be initialized before!
 * Note: This function is partially thread safe (use it for each USART from one thread only)!
 * Note: This function blocks until a valid byte has been received or timeout occurs
 *
 * @param USART_Index    device index of USART to init (0..ttc_usart_get_max_index()-1)
 * @param Word           pointer to 16 bit buffer where to store Word
 * @param TimeOut        >0: max microseconds to wait; ==0: no timeout
 * @return               == 0: Word has been read successfully; != 0: error-code
 */
ttc_usart_errorcode_e ttc_usart_read_byte(u8_t USART_Index, char* Byte, u32_t TimeOut);

//} Function prototypes

#endif //TTC_USART_H
