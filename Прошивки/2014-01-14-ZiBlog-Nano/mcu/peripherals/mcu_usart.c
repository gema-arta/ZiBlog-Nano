/*
 *  File:   mcu_usart.c
 *  Date:   26.05.2011
 *  Denis Zheleznjakov @ ZiBlog.ru
 */

#include "main.h"

struct
{
	volatile uint8_t rx_buffer[USART_RX_BUFFER_SIZE];
	volatile uint8_t rx_buffer_head;
	volatile uint8_t rx_buffer_tail;
	volatile uint8_t tx_buffer[USART_TX_BUFFER_SIZE];
	volatile uint8_t tx_buffer_head;
	volatile uint8_t tx_buffer_tail;
} usart_fifo;

void mcu_usart_init(uint32_t baud_rate)
{
	usart_fifo.rx_buffer_head = 0;
	usart_fifo.rx_buffer_tail = 0;
	usart_fifo.tx_buffer_head = 0;
	usart_fifo.tx_buffer_tail = 0;

	CLK_PeripheralClockConfig(CLK_Peripheral_USART1, ENABLE);

	USART_DeInit(USART1);

	USART_Init(USART1, baud_rate, USART_WordLength_8b, USART_StopBits_2, USART_Parity_No, (USART_Mode_TypeDef) (USART_Mode_Tx | USART_Mode_Rx));

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

	USART_Cmd(USART1, ENABLE);
}

_Bool mcu_usart_fifo_receive(uint8_t * data)
{
	uint8_t rx_head;
	uint8_t rx_tail;

	rx_head = usart_fifo.rx_buffer_head;

	if (rx_head == usart_fifo.rx_buffer_tail)
		return FALSE;

	rx_tail = (uint8_t) ((usart_fifo.rx_buffer_tail + 1) & (uint8_t) (USART_RX_BUFFER_SIZE - 1));

	usart_fifo.rx_buffer_tail = rx_tail;

	*data = usart_fifo.rx_buffer[rx_tail];

	return TRUE;
}

void mcu_usart_fifo_transmit(uint8_t data)
{
	uint8_t tx_head;

	tx_head = (uint8_t) ((usart_fifo.tx_buffer_head + 1) & (uint8_t) (USART_TX_BUFFER_SIZE - 1));

	while (tx_head == usart_fifo.tx_buffer_tail)
	{
	}

	usart_fifo.tx_buffer[tx_head] = data;
	usart_fifo.tx_buffer_head = tx_head;

	USART1->CR2 |= USART_CR2_TIEN;
	//USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
}

void mcu_usart_fifo_tx_handler(void)
{
	uint8_t status;
	uint8_t tx_tail;
	uint8_t tx_head;

	status = USART1->SR;

	if (status & USART_SR_TC)
	{
		USART1->SR &= (uint8_t) (~USART_SR_TC);
	}

	if (status & USART_SR_TXE)
	{
		tx_head = usart_fifo.tx_buffer_head;
		tx_tail = usart_fifo.tx_buffer_tail;

		if (tx_head != tx_tail++)
		{
			tx_tail &= USART_TX_BUFFER_SIZE - 1;

			usart_fifo.tx_buffer_tail = tx_tail;
			USART1->DR = usart_fifo.tx_buffer[tx_tail];
		}
		else
		{
			USART1->CR2 &= (uint8_t) (~USART_CR2_TIEN);
		}
	}
}

void mcu_usart_fifo_rx_handler(void)
{
	uint8_t rx_data;
	uint8_t rx_head;

	(void) USART1->SR;
	rx_data = USART1->DR;

	rx_head = (uint8_t) ((usart_fifo.rx_buffer_head + 1) & (uint8_t) (USART_RX_BUFFER_SIZE - 1));

	if (rx_head != usart_fifo.rx_buffer_tail)
	{
		usart_fifo.rx_buffer_head = rx_head;
		usart_fifo.rx_buffer[rx_head] = rx_data;
	}
}
