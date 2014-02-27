/*
 *	File:	main.c
 *	Date:	27.02.2014
 *	Denis Zheleznyakov aka ZiB @ http://ziblog.ru
 */

#include "main.h"

@inline void gpio_init(void)
{
	// аналоговые линии
	PIN_CONFIGURATION(PIN_IO_A0);
	PIN_CONFIGURATION(PIN_IO_A1);
	PIN_CONFIGURATION(PIN_IO_A2);
	PIN_CONFIGURATION(PIN_IO_A3);
	PIN_CONFIGURATION(PIN_IO_A4);
	PIN_CONFIGURATION(PIN_IO_A5);
	PIN_CONFIGURATION(PIN_IO_A6);
	PIN_CONFIGURATION(PIN_IO_A7);

	// цифровые линии
	PIN_CONFIGURATION(PIN_IO_D0);
	PIN_CONFIGURATION(PIN_IO_D1);
	PIN_CONFIGURATION(PIN_IO_D2);
	PIN_CONFIGURATION(PIN_IO_D3);
	PIN_CONFIGURATION(PIN_IO_D4);
	PIN_CONFIGURATION(PIN_IO_D5);
	PIN_CONFIGURATION(PIN_IO_D6);
	PIN_CONFIGURATION(PIN_IO_D7);
	PIN_CONFIGURATION(PIN_IO_D8);
	PIN_CONFIGURATION(PIN_IO_D9);
	PIN_CONFIGURATION(PIN_IO_D10);
	PIN_CONFIGURATION(PIN_IO_D11);
	PIN_CONFIGURATION(PIN_IO_D12);
	PIN_CONFIGURATION(PIN_IO_D13);

	// I2C интерфейс
	PIN_CONFIGURATION(PIN_I2C_SDA);
	PIN_CONFIGURATION(PIN_I2C_SDL);

	// USART интерфейс
	PIN_CONFIGURATION(PIN_USART_RXD);
	PIN_CONFIGURATION(PIN_USART_TXD);
}

@inline void clock_init(void)
{
	// "переключаемся" на внешний кварцевый резонатор
	CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);
	CLK_SYSCLKSourceSwitchCmd(ENABLE);
	CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSE);
	while (CLK_GetSYSCLKSource() != CLK_SYSCLKSource_HSE)
	{
	}

	// выключаем HSI
	CLK_HSICmd(DISABLE);
}

void main(void)
{
	uint8_t data;

	disableInterrupts();

	gpio_init();

	clock_init();

	device_init();

	mcu_usart_init(38400);

	enableInterrupts();

	while (1)
	{
		device_process();
	}
}
