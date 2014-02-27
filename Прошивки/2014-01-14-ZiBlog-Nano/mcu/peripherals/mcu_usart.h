/*
 *  File:   mcu_usart.h
 *  Date:   26.05.2011
 *  Denis Zheleznjakov @ ZiBlog.ru
 */

#ifndef MCU_USART_H_
#define MCU_USART_H_

#define USART_TX_BUFFER_SIZE    64 // кратен двум
#define USART_RX_BUFFER_SIZE    64 // кратен двум

void mcu_usart_init(uint32_t baud_rate);
void mcu_usart_fifo_tx_handler(void);
void mcu_usart_fifo_rx_handler(void);
void mcu_usart_fifo_transmit(uint8_t data);
_Bool mcu_usart_fifo_receive(uint8_t * data);

#endif /* MCU_USART_H_ */
