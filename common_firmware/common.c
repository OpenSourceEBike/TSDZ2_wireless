/*
 * TSDZ2 EBike wireless firmware
 *
 * Copyright (C) Casainho, 2021
 *
 * Released under the GPL License, Version 3
 * 
 */

#include "nrf_delay.h"
#include "bsp.h"
#include "pins.h"
#include "ledalert.h"
#include "app_timer.h"

APP_TIMER_DEF(lp_delay_timer);
static volatile bool lp_delay_flag = false; /**< Flag to indicate lp delay is running. */

/**
 * @brief Handler for low-power delay timer
 * @details this event is triggered when calling the delay-timer and has no other purpose than delaying the program
 */
static void lp_delay_timer_timeout(void *p_context)
{
  lp_delay_flag = false;
}

void nrf_lp_delay_ms_init(void)
{
  ret_code_t err_code;
  err_code = app_timer_create(&lp_delay_timer, APP_TIMER_MODE_SINGLE_SHOT, lp_delay_timer_timeout);
  APP_ERROR_CHECK(err_code);
}

/*
Power efficient delay timer.
calls sd_app_evt_wait() until timeout
*/
void nrf_lp_delay_ms(int ms_delay)
{

  uint32_t err_code;
  err_code = app_timer_start(lp_delay_timer, APP_TIMER_TICKS(ms_delay), NULL);
  APP_ERROR_CHECK(err_code);
  lp_delay_flag = true;
  while (lp_delay_flag)
  {
    err_code = sd_app_evt_wait();
    APP_ERROR_CHECK(err_code);
  }
 }



void disp_soc(int soc)
{
  switch (soc)
  {
  case (0):
    led_alert(LED_EVENT_BATTERY_SOC_0_PERCENT);
    break;

  case (1):
    led_alert(LED_EVENT_BATTERY_SOC_10_PERCENT);
    break;
  case (2):
    led_alert(LED_EVENT_BATTERY_SOC_20_PERCENT);
    break;
  case (3):
    led_alert(LED_EVENT_BATTERY_SOC_30_PERCENT);
    break;
  case (4):
    led_alert(LED_EVENT_BATTERY_SOC_40_PERCENT);
    break;
  case (5):
    led_alert(LED_EVENT_BATTERY_SOC_50_PERCENT);
    break;
  case (6):
    led_alert(LED_EVENT_BATTERY_SOC_60_PERCENT);
    break;
  case (7):
    led_alert(LED_EVENT_BATTERY_SOC_70_PERCENT);
    break;
  case (8):
    led_alert(LED_EVENT_BATTERY_SOC_80_PERCENT);
    break;
  case (9):
    led_alert(LED_EVENT_BATTERY_SOC_90_PERCENT);
    break;
  case 10:
    led_alert(LED_EVENT_BATTERY_SOC_100_PERCENT);
    break;
  }
}
