/*
 * LCD3 firmware
 *
 * Copyright (C) Casainho, 2018.
 *
 * Released under the GPL License, Version 3
 */

#ifndef _BUTTON_H_
#define _BUTTON_H_

typedef enum {
	ONOFF_CLICK = 1,
	ONOFF_CLICK_LONG_CLICK = 2,
	ONOFF_LONG_CLICK = 4,
	ONOFFUP_LONG_CLICK = 8,
	ONOFFDOWN_LONG_CLICK = 16,
	UP_CLICK = 32,
	UP_LONG_CLICK = 64,
  UPDOWN_LONG_CLICK = 128,
	DOWN_CLICK = 256,
	DOWN_LONG_CLICK = 512,
	M_CLICK = 1024, // Some devices (SW102) have this extra button
	M_LONG_CLICK = 2048,
	ONOFFUPDOWN_LONG_CLICK = 4096,
} buttons_events_t;

uint32_t buttons_get_m_state(void);
uint32_t buttons_get_up_state(void);
uint32_t buttons_get_up_click_event(void);
uint32_t buttons_get_up_click_long_click_event(void);
uint32_t buttons_get_up_long_click_event(void);
void buttons_clear_up_click_event(void);
void buttons_clear_up_click_long_click_event(void);
void buttons_clear_up_long_click_event(void);
uint32_t buttons_get_down_state(void);
uint32_t buttons_get_down_click_event(void);
uint32_t buttons_get_down_click_long_click_event(void);
uint32_t buttons_get_down_long_click_event(void);
void buttons_clear_down_click_event(void);
void buttons_clear_down_click_long_click_event(void);
void buttons_clear_down_long_click_event(void);
uint32_t buttons_get_onoff_state(void);
uint32_t buttons_get_onoff_click_event(void);
uint32_t buttons_get_onoff_click_long_click_event(void);
uint32_t buttons_get_onoff_long_click_event(void);
void buttons_clear_onoff_click_event(void);
void buttons_clear_onoff_click_long_click_event(void);
void buttons_clear_onoff_long_click_event(void);
uint32_t buttons_get_up_down_click_event(void);
void buttons_clear_up_down_click_event(void);
void buttons_clock(void);
buttons_events_t buttons_get_events(void);
void buttons_clear_all_events(void);
void buttons_set_events(buttons_events_t events);
void buttons_init(void);

extern buttons_events_t buttons_events;

#endif /* _BUTTON_H_ */
