/*
 * TSDZ2 EBike wireless firmware
 *
 * Copyright (C) Casainho, 2020
 *
 * Released under the GPL License, Version 3
 */

#ifndef ANTPLUS_CONTROLS_PAGE_82_H__
#define ANTPLUS_CONTROLS_PAGE_82_H__

#include <stdint.h>

typedef struct
{
  uint16_t battery_voltage_x100;
} antplus_controls_page_82_data_t;

#define DEFAULT_ANTPLUS_CONTROLS_PAGE82() \
  (antplus_controls_page_82_data_t)       \
  {                                       \
    .battery_voltage_x100 = 0,            \
  }

#define ANTPLUS_CONTROLS_PAGE82(bat_vol_x100) \
  (antplus_controls_page_82_data_t)           \
  {                                           \
    .battery_voltage_x100 = (bat_vol_x100),   \
  }

void antplus_controls_page_82_encode(uint8_t *p_page_buffer,
                                     antplus_controls_page_82_data_t const *p_page_data);

#endif