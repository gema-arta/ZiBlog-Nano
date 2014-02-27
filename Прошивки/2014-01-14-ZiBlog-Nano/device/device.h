/*
 * File: device.h
 * Date: 01.10.2013
 * Denis Zheleznyakov aka ZiB @ http://ziblog.ru
 */

#ifndef DEVICE_H_
#define DEVICE_H_

enum device_settings
{
	DEVICE_SETTINGS_ID = (uint16_t)0x2501, // идентификатор устройства
	DEVICE_SETTINGS
};

struct device_type
{
	uint8_t address;
};

extern struct device_type device;

void device_init(void);
void device_process(void);

#endif /* DEVICE_H_ */
