/*
 * Bafang LCD 850C firmware
 *
 * Copyright (C) Casainho, 2018.
 *
 * Released under the GPL License, Version 3
 */

#ifndef _RTC_H_
#define _RTC_H_

#include "stdio.h"

typedef struct _rtc_time {
	uint8_t ui8_hours;
	uint8_t ui8_minutes;
} rtc_time_t;

void rtc_init(void);
void rtc_set_time(rtc_time_t *rtc_time);
rtc_time_t* rtc_get_time(void);
rtc_time_t* rtc_get_time_since_startup(void);

extern uint32_t ui32_seconds_since_startup;

#endif /* _RTC_H_ */
