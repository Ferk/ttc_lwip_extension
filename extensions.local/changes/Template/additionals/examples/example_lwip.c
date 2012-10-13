/*{ example_lwip.c ************************************************

  example_lwip_start() spawns the thread task_echo_client(),
  or task_echo_server() if EXAMPLE_LWIP_SERVER is defined.

  example_lwip_init() calls tcpip_init() which will
  initialise lwip and spawn tcpip_thread(), the thread
  that managed all the networking requests (tcp and udp)

  netif_configuration() is called by tcpip_thread before
  dispatching any networking operation. It will initialise
  the ttc_eth_inet interface which in turn initialises the
  ethernet device.

  ****
  To test this example connecting the board to a PC,
  the program "netcat" can be used with the following arguments.
  (asuming the board is IP 192.168.0.99 and the service is in port 1200)
  
  TCP server:  nc -lp 1200
  UDP server:  nc -ulp 1200 
  TCP client:  nc 192.168.0.99 1200
  UDP client:  nc 192.168.0.99 1200 -u

  }*/

#include "example_lwip.h"

//{ Global variables *************************************************

// Network interface
struct netif netif;

#ifdef TTC_LED1
ttc_Port_t Led1Port;
#ifdef TTC_LED2
ttc_Port_t Led2Port;
#endif
#endif

const u8_t ETH_Index = 1; // Use the first available ethernet device

//} Global variables
//{ Function definitions *************************************************


/**
 * @brief  Sets up the network interface
 */
void netif_configuration(void)
{
    struct ip_addr ipaddr;
    struct ip_addr netmask;
    struct ip_addr gw;

#if LWIP_DHCP
    ipaddr.addr = 0;
    netmask.addr = 0;
    gw.addr = 0;
#else
    IP4_ADDR(&ipaddr, configIP_ADDR0,configIP_ADDR1,configIP_ADDR2,configIP_ADDR3);
    IP4_ADDR(&netmask, configNET_MASK0,configNET_MASK1,configNET_MASK2,configNET_MASK3);
    IP4_ADDR(&gw, configGW_ADDR0,configGW_ADDR1,configGW_ADDR2,configGW_ADDR3);
#endif

    // initialize interface using the first available ETH_Index
    ttc_eth_netif_state_t initstate = { ETH_Index };
    
    /* - netif_add(struct netif *netif, struct ip_addr *ipaddr,
       struct ip_addr *netmask, struct ip_addr *gw,
       void *state, err_t (* init)(struct netif *netif),
       err_t (* input)(struct pbuf *p, struct netif *netif))

       Adds your network interface to the netif_list. Allocate a struct
       netif and pass a pointer to this structure as the first argument.
       Give pointers to cleared ip_addr structures when using DHCP,
       or fill them with sane numbers otherwise. The state pointer may be NULL.

       The init function pointer must point to a initialization function for
       your ethernet netif interface. The following code illustrates it's use.*/
    netif_add( &netif,     // Pointer to the network interface struct to be initialized
	       &ipaddr,    // IP address of the device in the network
	       &netmask,   // IP Network mask
	       &gw,        // Gateway IP address
	       &initstate, // Struct with state initialization data
	       &ttc_eth_netif_init, // Initialization function 
	       &ethernet_input      // Input callback function
	       );
    
    /*  Registers the default network interface.*/
    netif_set_default(&netif);

#if LWIP_DHCP

    /*  Creates a new DHCP client for this interface on the first call.
        Note: you must call dhcp_fine_tmr() and dhcp_coarse_tmr() at
        the predefined regular intervals after starting the client.
        You can peek in the netif->dhcp struct for the actual DHCP status.*/
    dhcp_start(&netif);

    LWIP_DEBUGF(1,("DHCP Started\r\n"));
#endif

    /*  When the netif is fully configured this function must be called.*/
    netif_set_up(&netif);
    Assert( netif_is_up(&netif), ec_UNKNOWN ); // Check that it's actually up
}


#if EXAMPLE_LWIP_SERVER

void task_echo_server(void *pvParameters )
{
    pvParameters= pvParameters; // unused argument

    int sd; // socket descriptor
    struct sockaddr_in addr;
    char buffer[BUFFER_SIZE];
    int len; // received message length

#if EXAMPLE_LWIP_USE_TCP
    int cd; // client connection socket
    // create a TCP socket
    sd = socket(AF_INET, SOCK_STREAM, 0);

#else

    struct sockaddr_in caddr; // client address
    socklen_t caddrlen;       // client address length

    // create a UDP socket
    sd = socket(AF_INET, SOCK_DGRAM, 0);

#endif /* EXAMPLE_LWIP_USE_TCP */

    LWIP_ASSERT("cannot open socket", sd >= 0);

    // fill in address
    memset(&addr, 0, sizeof(addr));
    addr.sin_len = sizeof(addr);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(SOCK_PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    /* bind socket to a local port */
    LWIP_ASSERT("cannot bind socket",
                bind(sd, (struct sockaddr*)&addr, sizeof(addr)) == 0);


#if EXAMPLE_LWIP_USE_TCP

    // Listen on the given port
    Assert( listen(sd, 5) ==0, ec_UNKNOWN);
    LWIP_DEBUGF(1,("listening for TCP connections on port %d\n\r",SOCK_PORT));

#else

    LWIP_DEBUGF(1,("waiting for UDP packets on port %d\n\r",SOCK_PORT));

#endif /* EXAMPLE_LWIP_USE_TCP */

    /** Start of server loop **/
    while(1) {

#ifdef TTC_LED2
        ttc_portSet(&Led2Port);
#endif /* TTC_LED2 */

        /*** Receive a message ***/
#if EXAMPLE_LWIP_USE_TCP
        // Wait new connection, asign it to a descriptor
        if( (cd = accept( sd, NULL, NULL) ) < 0 )
            continue; // skip on error

        len = recv( cd,             // Socket to read
                    (void*) buffer, // Buffer where store the message
                    sizeof(buffer), // Size of the buffer
                    MSG_WAITALL     // flags (MSG_WAITALL: blocking)
                    );
#else
        // receive a pakage retrieving also sender's info
        len = recvfrom( sd,                         // Socket to read
                        (void*) buffer,             // Buffer where store the message
                        sizeof(buffer),             // Size of the buffer
                        MSG_WAITALL,                // flags (MSG_WAITALL: blocking)
                        (struct sockaddr *) &caddr, // store sender address
                        &caddrlen                   // length of sender address
                        );
#endif /* EXAMPLE_LWIP_USE_TCP */

#ifdef TTC_LED2
        ttc_portClr(&Led2Port);
#endif /* TTC_LED2 */

        if( len < 0 ) {
            LWIP_DEBUGF(1,("error on package reception"));
            continue;
        }

        /*** Process the data.... ***/

        LWIP_DEBUGF(1,("received: %s\n\r",buffer));
        // append some chars if there's space
        if( ( len+2 < BUFFER_SIZE) && buffer != NULL) {
            buffer[len-1]= '-';
            buffer[len]= 'R';
	    buffer[len+1]= '\n';
            buffer[len+2]= 0;
            len += 2;
        }

        /*** Send data back ***/

#if EXAMPLE_LWIP_USE_TCP
        len = send( cd,     // socket to write into
                    buffer, // buffer to send
                    len,    // length of the buffer
                    0       // special flags (0: no flags)
                    );
#else
        len = sendto( sd,                         // socket to write into
                      buffer,                     // buffer to send
                      len,                        // length of the buffer
                      0,                          // flags (0: no flags)
                      (struct sockaddr *) &caddr, // address to send it to
                      caddrlen                    // length of the address
                      );
#endif /* EXAMPLE_LWIP_USE_TCP */

        if( len == -1)
            LWIP_DEBUGF(1,("error sending: %s\n\r",buffer));
        else
            LWIP_DEBUGF(1,("sent: %s\n\r",buffer));

#if EXAMPLE_LWIP_USE_TCP
        // end the connection after the exchange
        close(cd);
#endif /* EXAMPLE_LWIP_USE_TCP */
    }
}


#else /* EXAMPLE_LWIP_SERVER */


void task_echo_client(void *pvParameters )
{
    pvParameters= pvParameters; // unused argument

    int sd;  // socket descriptor
    struct sockaddr_in saddr; // server address
    char buffer[BUFFER_SIZE] = "0 Hello!\n\0"; // data buffer for transmission
    int len = 9; // length of the data for transmission
    int slen;    // length of the data sent (for confirmation)

#if EXAMPLE_LWIP_USE_TCP
    // create a TCP socket
    sd = socket(AF_INET, SOCK_STREAM, 0);
#else
    // create a UDP socket
    sd = socket(AF_INET, SOCK_DGRAM, 0);
#endif  /* EXAMPLE_LWIP_USE_TCP */

    memset((char *) &saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(SOCK_PORT);
    Assert( inet_aton(REMOTE_SERVER_IP, &saddr.sin_addr) !=0, ec_UNKNOWN );

#if EXAMPLE_LWIP_USE_TCP
    // negotiate TCP connection with the server
    // it will keep trying until the connection is stablished
    while( connect( sd, (struct sockaddr*) &saddr, sizeof(saddr)) == 0 ) 
	mSleep(100);
    // Note that in this example it won't reconnect if the first successful connection is lost
#endif /* EXAMPLE_LWIP_USE_TCP */

    /** Client main loop **/
    while(1) {

#ifdef TTC_LED2
        ttc_portSet(&Led2Port);
#endif

#if EXAMPLE_LWIP_USE_TCP
        slen = send( sd,    // socket to write into
                     buffer, // buffer to send
                     len,    // length of the data
                     0       // special flags (0: no flags)
                     );
#else
        slen = sendto( sd,                         // socket to write into
                       buffer,                     // buffer to send
                       len,                        // length of the data
                       0,                          // flags (0: no flags)
                       (struct sockaddr *) &saddr, // address to send it to
                       sizeof(saddr)               // length of the address
                       );
#endif /* EXAMPLE_LWIP_USE_TCP */

        if( slen < 0 )
            LWIP_DEBUGF(1,("error sending: %s\n\r",buffer));
        else
            LWIP_DEBUGF(1,("sent: %s\n\r",buffer));

#ifdef TTC_LED2
        ttc_portClr(&Led2Port);
#endif
        mSleep(3000);
	buffer[0]++; // increase first character of the buffer
    }
}

#endif /* EXAMPLE_LWIP_SERVER */


// Interruption handler
void example_lwip_package_handler(void)
{
    u32_t size;

#ifdef TTC_LED1
    ttc_portSet(&Led1Port);
#endif
    ttc_eth_get_received_size(ETH_Index,&size);
    /* Handles all the received frames */
    if( size != 0) {
        /* Read a received packet from the Ethernet buffers and send it to the lwIP interface input function for handling */
        ttc_eth_netif_input(&netif);
    }

    /* Clear the Eth DMA Rx IT pending bits */
    ETH_DMAClearITPendingBit(ETH_DMA_IT_R);
    ETH_DMAClearITPendingBit(ETH_DMA_IT_NIS);
    
#ifdef TTC_LED1
    ttc_portClr(&Led1Port);
#endif
}

void example_lwip_init()
{
#if defined(EXTENSION_500_ttc_usart) && defined(USART_INDEX_STDOUT)
    // Initialise USART for debugging
    ttc_usart_generic_t       USART_Generic;  // architecture independent USART configuration
    ttc_usart_architecture_t  USART_Arch;     // architecture specific USART configuration
    Assert( ttc_usart_get_defaults(USART_INDEX_STDOUT, &USART_Generic)      == tue_OK, ec_UNKNOWN);
    Assert( ttc_usart_init(USART_INDEX_STDOUT, &USART_Generic, &USART_Arch) == tue_OK, ec_UNKNOWN);

    ttc_usart_send_string(USART_INDEX_STDOUT,"USART STDOUT interface ready\n",6);
#endif

    // enable clocks to all used ports (other clocks are enabled by stm32_initPort() automatically)
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,  ENABLE);
#if defined (STM32F10X_LD_VL) || defined (STM32F10X_MD_VL)
    RCC_ADCCLKConfig(RCC_PCLK2_Div2);  // ADCCLK = PCLK2/2
#else
    RCC_ADCCLKConfig(RCC_PCLK2_Div4);  // ADCCLK = PCLK2/4
#endif
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,   ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

#ifdef TTC_LED1
    ttc_initPort(&Led1Port, TTC_LED1, tgm_output_push_pull);
    ttc_portClr(&Led1Port);
#ifdef TTC_LED2
    ttc_initPort(&Led2Port, TTC_LED2, tgm_output_push_pull);
    ttc_portClr(&Led2Port);
#endif /* TTC_LED2 */
#endif /* TTC_LED1 */

    // start the networking thread, passing to it the function to set up the network interface
    tcpip_init( (tcpip_init_done_fn) netif_configuration,NULL);
}


void example_lwip_start()
{
    example_lwip_init();

#if EXAMPLE_LWIP_SERVER
    ttc_task_create(task_echo_server,                  // function to start as thread
                    (const signed char*) "LwIP",       // thread name (just for debugging)
                    (size_t) 512,                      // stack size
                    (void *) NULL,                     // passed as argument to the task
                    (unsigned portBASE_TYPE) 1,        // task priority (higher values mean more process time)
                    (void*) NULL                       // can return a handle to created task
                    );
#else /* EXAMPLE_LWIP_SERVER */
    ttc_task_create(task_echo_client,                  // function to start as thread
                    (const signed char*) "LwIP",       // thread name (just for debugging)
                    (size_t) 512,                      // stack size
                    (void *) NULL,                     // passed as argument to the task
                    (unsigned portBASE_TYPE) 1,        // task priority (higher values mean more process time)
                    (void*) NULL                       // can return a handle to created task
                    );
#endif /* EXAMPLE_LWIP_SERVER */

}

//} Function definitions
