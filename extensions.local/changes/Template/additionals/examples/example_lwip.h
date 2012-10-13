#ifndef EXAMPLE_LWIP_H
#define EXAMPLE_LWIP_H

/*{ example_lwip.h ************************************************
 
                       The ToolChain
                       
 Example code demonstrating how to use 
 the LWIP network stack
 
 written by Fernando Carmona Varo 2012
 
}*/
//{ Includes *************************************************************

#include "lwip/memp.h"
#include "lwip/tcpip.h"
#include "lwip/tcp.h"
#include "lwip/tcp_impl.h"
#include "lwip/udp.h"
#include "netif/etharp.h"
#include "lwip/dhcp.h"
#include "lwip/sockets.h"
#include "lwip/debug.h"

#include "ttc_eth_netif.h"
#include "ttc_ethernet.h"
#include "ttc_gpio.h"
#include "ttc_task.h"

#ifdef EXTENSION_500_ttc_usart
#include "ttc_usart.h"
#endif

#include <string.h>
#include <stdio.h>

#include "FreeRTOSConfig.h"


//} Includes
//{ Defines/ TypeDefs ****************************************************

// Set to 1 to use TCP connections
#define EXAMPLE_LWIP_USE_TCP 0
// Set to 1 to act as a server instead of a client
#define EXAMPLE_LWIP_SERVER  0

#if    EXAMPLE_LWIP_USE_TCP && !LWIP_TCP
#error LWIP_TCP must be set to 1 for example_lwip to use UDP
#elif !EXAMPLE_LWIP_USE_TCP && !LWIP_UDP
#error LWIP_UDP must be set to 1 for example_lwip to use UDP
#endif

// Max size for the message buffer
#define BUFFER_SIZE 20

// Port number where to stablish the connection
#define SOCK_PORT  1200

#if ! EXAMPLE_LWIP_SERVER
// IP of the server to connect (when working as client)
#define REMOTE_SERVER_IP "192.168.0.8"
#endif

//} Defines
//{ Structures/ Enums ****************************************************

//} Structures/ Enums
//{ Structures/ Enums ****************************************************

//} Structures/ Enums
//{ Global Variables *****************************************************

//} Global Variables
//{ Function prototypes **************************************************

/* Initialises and starts LwIP example
 */
void example_lwip_start();

/* Initialises the LwIP stack
 */
void example_lwip_init();

/* Manages the reception of a package
   To be called by the ethernet interruption handler (ETH_IRQHandler)
 */
void example_lwip_package_handler();

#ifdef EXAMPLE_LWIP_SERVER

/* Echo client
 */
void task_echo_server(void *pvParameters );

#else /* EXAMPLE_LWIP_SERVER */

/* Echo client
 */
void task_echo_client(void *pvParameters );

#endif /* EXAMPLE_LWIP_SERVER */


//} Function prototypes

#endif //EXAMPLE_LEDS
