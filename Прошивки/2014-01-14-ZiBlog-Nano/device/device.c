/*
 * File: device.c
 * Date: 01.10.2013
 * Denis Zheleznyakov aka ZiB @ http://ziblog.ru
 */

#include "main.h"

struct device_type device;

void device_init(void)
{
	device.address = 1;
}

void device_process(void)
{
	// обработка пакетов сети USART
	network_uart_process();
}
