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

void disp_soc(int soc)
{
  uint32_t ui32_time_now;
  static uint32_t ui32_last_run_time;
 
  //check for double clicks
  ui32_time_now = get_time_base_counter_1ms();
  //process if button click are >5 seconds apart
  if ((ui32_time_now - ui32_last_run_time) >= 5000) 
  {
    ui32_last_run_time = ui32_time_now; //prevent multiple double clicks
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
}