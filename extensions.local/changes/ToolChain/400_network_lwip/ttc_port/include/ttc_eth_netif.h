#ifndef __TTC_ETH_NETIF_H__
#define __TTC_ETH_NETIF_H__

#include "lwip/err.h"
#include "lwip/netif.h"


#ifndef MAX_ETH_PAYLOAD
#define MAX_ETH_PAYLOAD 1500 /* Maximal ETH frame size (header+extra+payload+crc)*/
#endif MAX_ETH_PAYLOAD

/**
 * Helper struct to hold private data used to operate the ethernet interface.
 */
typedef struct {
  u8_t ETH_Index;
} ttc_eth_netif_state_t;

err_t ttc_eth_netif_init(struct netif *netif);
void ttc_eth_netif_input(struct netif *netif);
void ttc_eth_netif_set_mac_addr(struct netif *netif, unsigned char* macaddr);

#endif
