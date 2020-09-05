#ifndef __FAST_UART_PLATFORM_H_
#define __FAST_UART_PLATFORM_H_

/**
 * @brief Platform dependent feature:
 * Pass pointer to this structure as part of device_d -> platform_data
 */
struct fast_uart_plat {
	/** Clock speed */
	unsigned int clock;
};

#endif				/* __FAST_UART_PLATFORM_H_ */
