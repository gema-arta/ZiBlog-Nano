/*
 * File: network.c
 * Date: 01.10.2013
 * Denis Zheleznyakov aka ZiB @ http://ziblog.ru
 */

#include "main.h"

union packet_type
{
	uint8_t raw[8];
	struct
	{
		uint8_t marker;
		uint8_t recipient_address :4, sender_address :4;
		uint8_t access_point :7, direction :1;
		uint8_t data_0;
		uint8_t data_1;
		uint8_t data_2;
		uint8_t data_3;
		uint8_t crc;
	};
} packet;

enum direction
{
	READ = 1, WRITE = 0
};

void packet_calc_crc(void)
{
	uint8_t byte_number;
	uint8_t i;

	packet.crc = 0;

	for (byte_number = 0; byte_number < ARRAY_LENGHT(packet.raw) - 1; byte_number++)
	{
		packet.crc ^= packet.raw[byte_number];

		for (i = 0; i < 8; i++)
		{
			if ((packet.crc & 0x01) != 0x00)
			{
				packet.crc >>= 1;
				packet.crc ^= 0x8C;
			}
			else
			{
				packet.crc >>= 1;
			}
		}
	}
}

void network_uart_send_packet(void)
{
	uint8_t byte_number;

	// считаем контрольную сумму
	packet_calc_crc();

	// передаем пакет
	for (byte_number = 0; byte_number < ARRAY_LENGHT(packet.raw); byte_number++)
	{
		mcu_usart_fifo_transmit(packet.raw[byte_number]);
	}
}

void network_uart_process(void)
{
	static uint8_t rx_buffer[8];
	static uint8_t rx_buffer_index = 0;
	uint8_t byte_number;
	uint8_t rx_data;
	uint8_t i;
	uint8_t crc8;

	while (mcu_usart_fifo_receive(&rx_data))
	{
		// сохраняем принятый байт в буфер
		rx_buffer[rx_buffer_index++ & 0x07] = rx_data;

		// проверка приема начала пакета
		if (rx_buffer[rx_buffer_index & 0x07] == 0xAA)
		{
			// копируем пакет
			for (byte_number = 0; byte_number < ARRAY_LENGHT(packet.raw); byte_number++)
			{
				// параллельно копируем пакет
				packet.raw[byte_number] = rx_buffer[(rx_buffer_index + byte_number) & 0x07];
			}

			// считаем контрольную сумму
			packet_calc_crc();

			// проверка контрольной суммы пакета
			if (packet.crc == rx_buffer[(rx_buffer_index - 1) & 0x07])
			{
				// спец пакет конфигурации
				if ((packet.sender_address == 0) && (packet.recipient_address == 0))
				{
					if (packet.direction == READ)
					{
						// ретрансляция пакета
						packet.data_0++;
						network_uart_send_packet();

						// ответ уст-ва
						packet.sender_address = 15;
						packet.direction = 0;
						packet.access_point = packet.data_3;
						packet.data_1 = (uint8_t) (DEVICE_SETTINGS_ID);
						packet.data_2 = (uint8_t) (DEVICE_SETTINGS_ID >> 8);
						packet.data_3 = device.address;

						network_uart_send_packet();
					}
					else // установка адреса устройства
					{
						if (++packet.data_0 == packet.data_2)
						{
							if ((packet.data_3 > 0) && (packet.data_3 < 15))
							{
								device.address = packet.data_3;
							}
						}
						else
						{
							network_uart_send_packet();
						}
					}
				}
				else
				{
					// ретрансляция "чужих" пакетов
					if ((packet.recipient_address != device.address) && (packet.sender_address != device.address))
					{
						for (byte_number = 0; byte_number < ARRAY_LENGHT(packet.raw); byte_number++)
						{
							mcu_usart_fifo_transmit(packet.raw[byte_number]);
						}
					}

					// обработка широковещательных и "своих" пакетов
					if ((packet.recipient_address == 15) || (packet.recipient_address == device.address))
					{
						if (packet.direction == WRITE)
						{
							access_points_write(packet.access_point, &packet.raw[3]);
						}
						else
						{
							packet.recipient_address = packet.sender_address;
							packet.sender_address = device.address;
							packet.direction = WRITE;
							byte_number = packet.data_3;

							access_points_read(packet.access_point, &packet.raw[3]);

							packet.access_point = byte_number;
							network_uart_send_packet();
						}
					}
				}
			}
		}
	}
}
