/*
 * File: macros.h
 * Date: 28.01.2011
 * Denis Zheleznyakov aka ZiB @ http://ziblog.ru
 */

#ifndef MACROS_H_
#define MACROS_H_

#define BIT(NUMBER)            		(1UL << (NUMBER))

#define ARRAY_LENGHT(Value)			(sizeof(Value) / sizeof(Value[0]))

#define BYTES(value)    			((uint8_t *) & (value))

void synchronization_10ms_refresh(void);
_Bool time_synchronization_10ms(void);
void delay_ms_refresh(void);
void delay_ms(volatile uint16_t delay);

#endif /* MACROS_H_ */
