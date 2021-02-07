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

void disp_soc(int soc)
{
  nrf_delay_ms(500);
  for (int i = 0; i < soc; i++)
  {
    bsp_board_led_on(LED_B__PIN);
    nrf_delay_ms(300);
    bsp_board_led_off(LED_B__PIN);
    nrf_delay_ms(300);
  }
}