/*
 * File: macros.c
 * Date: 01.03.2014
 * Denis Zheleznyakov aka ZiB @ http://ziblog.ru
 */

#include "main.h"

static _Bool is_ready;

void synchronization_10ms_refresh(void)
{
	static uint8_t counter_0_5_ms = 0;

	if (is_ready == FALSE)
	{
		if (++counter_0_5_ms >= 20)
		{
			counter_0_5_ms = 0;
			is_ready = TRUE;
		}
	}
}

_Bool time_synchronization_10ms(void)
{
	if (is_ready == TRUE)
	{
		is_ready = FALSE;
		return TRUE;
	}

	return FALSE;
}
