/**
 * 
 * The ToolChain Ethernet LwIP Network Interface
 *
 *
 * Fernando Carmona Varo 2012
 *
 */

#include "lwip/opt.h"

#include "lwip/def.h"
#include "lwip/mem.h"
#include "lwip/pbuf.h"
#include "lwip/sys.h"
#include <lwip/stats.h>
#include <lwip/snmp.h>
#include "netif/etharp.h"
#include "netif/ppp_oe.h"

#include <string.h>

#include "ttc_ethernet.h"
#include "ttc_eth_netif.h"

/* interface name */
#define IFNAME0 't'
#define IFNAME1 'c'

#ifndef TTC_ETH_SPEED
/* use ethernet speed */
#define TTC_ETH_SPEED 100000000 /* link speed in bits per second */
#endif

/**
 * Set MAC Address
 */
void ttc_eth_netif_set_mac_addr(struct netif *netif, unsigned char* macaddr)
{
  ttc_eth_netif_state_t *ttc_eth_netif = netif->state;
    
  /* set MAC hardware address length */
  netif->hwaddr_len = ETHARP_HWADDR_LEN;

  /* set MAC hardware address */
  netif->hwaddr[0] = macaddr[0];
  netif->hwaddr[1] = macaddr[1];
  netif->hwaddr[2] = macaddr[2];
  netif->hwaddr[3] = macaddr[3];
  netif->hwaddr[4] = macaddr[4];
  netif->hwaddr[5] = macaddr[5];

  ttc_eth_set_mac_addr(ttc_eth_netif->ETH_Index, netif->hwaddr);
}

/**
 * In this function, the hardware should be initialized.
 * Called from ttc_eth_netif_init().
 *
 * @param netif the already initialized lwip network interface structure
 *        for this ttc_eth_netif
 */
static void
low_level_init(struct netif *netif)
{
  ttc_eth_netif_state_t *ttc_eth_netif = netif->state;
  
  /* set MAC hardware address length */
  netif->hwaddr_len = ETHARP_HWADDR_LEN;

#if defined(configMAC_ADDR0)&&defined(configMAC_ADDR1)&&defined(configMAC_ADDR2)&&defined(configMAC_ADDR3)&&defined(configMAC_ADDR4)&&defined(configMAC_ADDR5)
  /* set MAC hardware address */
  netif->hwaddr[0] = configMAC_ADDR0;
  netif->hwaddr[1] = configMAC_ADDR1;
  netif->hwaddr[2] = configMAC_ADDR2;
  netif->hwaddr[3] = configMAC_ADDR3;
  netif->hwaddr[4] = configMAC_ADDR4;
  netif->hwaddr[5] = configMAC_ADDR5;

  ttc_eth_set_mac_addr(ttc_eth_netif->ETH_Index, netif->hwaddr);
#endif
  
  /* maximum transfer unit */
  netif->mtu = 1500;
  
  /* device capabilities */
  /* don't set NETIF_FLAG_ETHARP if this device is not an ethernet one */
  netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_LINK_UP;
 
  /* Initialise ttc_eth ethernet. */
  ttc_eth_generic_t eth_config;
  ttc_eth_architecture_t eth_arch;
  ttc_eth_errorcode_e err;
  err= ttc_eth_get_defaults( ttc_eth_netif->ETH_Index, &eth_config );
  Assert( err == tee_OK, ec_UNKNOWN);
  err= ttc_eth_init( ttc_eth_netif->ETH_Index, &eth_config, &eth_arch );
  Assert( err == tee_OK, ec_UNKNOWN);
}

/**
 * This function should do the actual transmission of the packet. The packet is
 * contained in the pbuf that is passed to the function. This pbuf
 * might be chained.
 *
 * @param netif the lwip network interface structure for this ttc_eth_netif
 * @param p the MAC packet to send (e.g. IP packet including MAC addresses and type)
 * @return ERR_OK if the packet could be sent
 *         an err_t value if the packet couldn't be sent
 *
 * @note Returning ERR_MEM here if a DMA queue of your MAC is full can lead to
 *       strange results. You might consider waiting for space in the DMA queue
 *       to become availale since the stack doesn't retry to send a packet
 *       dropped because of memory failure (except for the TCP timers).
 */

static err_t
low_level_output(struct netif *netif, struct pbuf *p)
{
  ttc_eth_netif_state_t *ttc_eth_netif = netif->state;
  struct pbuf *q;
  ttc_eth_errorcode_e err;
  u8_t *buffer;
  int l = 0;
  
#if ETH_PAD_SIZE
  pbuf_header(p, -ETH_PAD_SIZE); /* drop the padding word */
#endif

  // get a new output buffer
  err = ttc_eth_get_output_buffer(ttc_eth_netif->ETH_Index, &buffer);
  if(( err != tee_OK ) || (p->tot_len > MAX_ETH_PAYLOAD)) {
    LINK_STATS_INC( link.lenerr );
    LINK_STATS_INC( link.drop );
    snmp_inc_ifoutdiscards( netif );
    return ERR_BUF;
  }
  
  // fill the buffer
  for(q = p; q != NULL; q = q->next) {
    memcpy(buffer+l, q->payload, q->len);
    l += q->len;
  }

  // send it
  err = ttc_eth_send_output_buffer(ttc_eth_netif->ETH_Index, l);
  if(err != tee_OK) {
    LINK_STATS_INC( link.memerr );
    LINK_STATS_INC( link.drop );
    snmp_inc_ifoutdiscards( netif );
    return ERR_BUF;
  }
  
#if ETH_PAD_SIZE
  pbuf_header(p, ETH_PAD_SIZE); /* reclaim the padding word */
#endif

  LINK_STATS_INC(link.xmit);
  snmp_add_ifoutoctets( netif, p->tot_len - ETH_PAD_SIZE );
  
  return ERR_OK;
}

/**
 * Should allocate a pbuf and transfer the bytes of the incoming
 * packet from the interface into the pbuf.
 *
 * @param netif the lwip network interface structure for this ttc_eth_netif
 * @return a pbuf filled with the received packet (including MAC header)
 *         NULL on memory error
 */
static struct pbuf *
low_level_input(struct netif *netif)
{
  ttc_eth_netif_state_t *ttc_eth_netif = netif->state;
  struct pbuf *p=NULL, *q;
  u32_t len;
  u8* buffer=NULL;
  int l=0;
  ttc_eth_errorcode_e err;

  // Receive a frame and obtain its size
  err= ttc_eth_get_input_buffer(ttc_eth_netif->ETH_Index, &buffer, &len);
  if( err != tee_OK ) return NULL;
  
#if ETH_PAD_SIZE
  len += ETH_PAD_SIZE; /* allow room for Ethernet padding */
#endif

  /* Allocate a pbuf chain from the pool, with enough size to store the frame. */
  p = pbuf_alloc(PBUF_RAW, len, PBUF_POOL);
  
  if (p != NULL) {

#if ETH_PAD_SIZE
    pbuf_header(p, -ETH_PAD_SIZE); /* drop the padding word */
#endif

    /*
     * Read enough bytes to fill this pbuf in the chain. The
     * available data in the pbuf is given by the q->len
     * variable.
     */
    for(q = p; q != NULL; q = q->next) {
	memcpy((u8_t*)q->payload, (u8_t*)&buffer[l], q->len);
	l += q->len;
    }
   
#if ETH_PAD_SIZE
    pbuf_header(p, ETH_PAD_SIZE); /* reclaim the padding word */
#endif
    
    LINK_STATS_INC(link.recv);
  } else {
    LINK_STATS_INC(link.memerr);
    LINK_STATS_INC(link.drop);
  }

  // Regardless whether the packet was read or dropped, resume input
  ttc_eth_resume_input(ttc_eth_netif->ETH_Index);
  
  return p;  
}

/**
 * This function should be called when a packet is ready to be read
 * from the interface. It uses the function low_level_input() that
 * should handle the actual reception of bytes from the network
 * interface. Then the type of the received packet is determined and
 * the appropriate input function is called.
 *
 * @param netif the lwip network interface structure for this ttc_eth_netif
 */
void
ttc_eth_netif_input(struct netif *netif)
{
  //ttc_eth_netif_state_t *ttc_eth_netif;
  struct eth_hdr *ethhdr;
  struct pbuf *p;

  //ttc_eth_netif = netif->state;

  /* move received packet into a new pbuf */
  p = low_level_input(netif);
  /* no packet could be read, silently ignore this */
  if (p == NULL) return;
  /* points to packet payload, which starts with an Ethernet header */
  ethhdr = p->payload;

  switch (htons(ethhdr->type)) {
  /* IP or ARP packet? */
  case ETHTYPE_IP:
  case ETHTYPE_ARP:
#if PPPOE_SUPPORT
  /* PPPoE packet? */
  case ETHTYPE_PPPOEDISC:
  case ETHTYPE_PPPOE:
#endif /* PPPOE_SUPPORT */
    /* full packet send to tcpip_thread to process */
    if (netif->input(p, netif)!=ERR_OK)
     { LWIP_DEBUGF(NETIF_DEBUG, ("ttc_eth_netif_input: IP input error\n"));
       pbuf_free(p);
       p = NULL;
     }
    break;

  default:
    pbuf_free(p);
    p = NULL;
    break;
  }
}

/**
 * Should be called at the beginning of the program to set up the
 * network interface. It calls the function low_level_init() to do the
 * actual setup of the hardware.
 *
 * This function should be passed as a parameter to netif_add().
 *
 * @param netif the lwip network interface structure for this ttc_eth_netif
 * @return ERR_OK if the loopif is initialized
 *         ERR_MEM if private data couldn't be allocated
 *         any other err_t on error
 */
err_t
ttc_eth_netif_init(struct netif *netif)
{
  ttc_eth_netif_state_t *ttc_eth_netif;

  LWIP_ASSERT("netif != NULL", (netif != NULL));

  ttc_eth_netif = mem_malloc(sizeof(ttc_eth_netif_state_t));
  
  if (ttc_eth_netif == NULL) {
    LWIP_DEBUGF(NETIF_DEBUG, ("ttc_eth_netif_init: out of memory\n"));
    return ERR_MEM;
  }
  
  if( netif->state != NULL )
      *ttc_eth_netif = *((ttc_eth_netif_state_t*) netif->state);

#if LWIP_NETIF_HOSTNAME
  /* Initialize interface hostname */
  netif->hostname = "ttc_lwip";
#endif /* LWIP_NETIF_HOSTNAME */

  /*
   * Initialize the snmp variables and counters inside the struct netif.
   */
  NETIF_INIT_SNMP(netif, snmp_ifType_ethernet_csmacd, TTC_ETH_SPEED);

  netif->state = ttc_eth_netif;
  netif->name[0] = IFNAME0;
  netif->name[1] = IFNAME1;
  /* We directly use etharp_output() here to save a function call.
   * You can instead declare your own function and call etharp_output()
   * from it if you have to do some checks before sending (e.g. if link
   * is available...) */
  netif->output = etharp_output;
  netif->linkoutput = low_level_output;
  
  /* initialize the hardware */
  low_level_init(netif);

  return ERR_OK;
}

