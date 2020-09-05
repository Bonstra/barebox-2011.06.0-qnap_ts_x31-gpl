
#ifndef __C2000_ETH_PDATA_H__
#define __C2000_ETH_PDATA_H__

struct c2000_eth_platform_data {
	int gemac_port;
	u8 mac_addr[6];
};

#endif
