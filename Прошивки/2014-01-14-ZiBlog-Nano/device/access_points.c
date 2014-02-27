/*
 * File: access_points.c
 * Date: 01.10.2013
 * Denis Zheleznyakov aka ZiB @ http://ziblog.ru
 */

#include "main.h"

@inline void access_point_zero_read(uint8_t * value)
{
	switch (value[0])
	{
		case (0): // команда идентификации
		{
			value[1] = (uint8_t) (DEVICE_SETTINGS_ID);
			value[2] = (uint8_t) (DEVICE_SETTINGS_ID >> 8);
			value[3] = device.address;

			break;
		}
		case (6): // время работы после сброса
		{
			value[0] = (uint8_t) (device.time_life);
			value[1] = (uint8_t) (device.time_life >> 8);
			value[2] = (uint8_t) (device.time_life >> 16);
			value[3] = (uint8_t) (device.time_life >> 24);
			break;
		}
		case (7): // загрузка МК
		{
			value[3] = (uint8_t) (device.mcu_load);
			device.mcu_load = 0;

			break;
		}
		default:
		{
			value[0] = 0;
			value[1] = 0;
			value[2] = 0;
			value[3] = 0;
		}
	}
}

@inline void access_point_zero_write(uint8_t * value)
{
	switch (value[0])
	{
		case (0): // установка адреса
		{
			device.address = value[3];

			break;
		}
	}
}

void access_points_read(uint8_t number, uint8_t * value)
{
	// точка доступа "Ноль"
	if (number == 0)
	{
		access_point_zero_read(value);
	}
	else
	{
		value[0] = 0;
		value[1] = 0;
		value[2] = 0;
		value[3] = 0;
	}
}

void access_points_write(uint8_t number, uint8_t * value)
{
	// точка доступа "Ноль"
	if (number == 0)
	{
		access_point_zero_write(value);
	}
	else
	{

	}
}
