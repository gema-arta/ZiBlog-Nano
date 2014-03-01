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

@inline void timers_init(void)
{
	CLK->PCKENR1 |= CLK_PCKENR1_TIM4;
	CLK->PCKENR2 |= CLK_PCKENR2_TIM1;

	// таймер 1 - ШИМ 100 Гц
	TIM1_TimeBaseInit(0, TIM1_PSCReloadMode_Update, 625, 0); // 16МГц / 1 /625 = 25,6кГц
	TIM1_ClearFlag(TIM1_FLAG_Update);
	TIM1_ITConfig(TIM1_IT_Update, ENABLE);
	TIM1_Cmd(ENABLE);

	// таймер 4 - формирование 0.5 мс интервалов
	TIM4_TimeBaseInit(TIM4_Prescaler_32, 250 - 1); // 16МГц / 32 / 250 = 2 кГц
	TIM4_ClearFlag(TIM4_FLAG_Update);
	TIM4_ITConfig(TIM4_IT_Update, ENABLE);
	TIM4_Cmd(ENABLE);
}

void main(void)
{
	uint8_t data;

	disableInterrupts();

	gpio_init();

	clock_init();

	device_init();

	timers_init();

	mcu_usart_init(38400);

	enableInterrupts();

	while (1)
	{
		// синхронизация времени - 10 мс
		if (time_synchronization_10ms())
		{
			// тестовый сигнал
			PIN_ON(PIN_IO_D2);

			// счетчик нагрузки
			device.mcu_load_counter = 0;

			// обработка пакетов сети USART
			network_uart_process();

			// загрузка МК
			if (device.mcu_load_counter > device.mcu_load)
			{
				device.mcu_load = device.mcu_load_counter;
			}

			// тестовый сигнал
			PIN_OFF(PIN_IO_D2);
		}
	}
}
