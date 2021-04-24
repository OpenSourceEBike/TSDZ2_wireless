/*
 * TSDZ2 EBike wireless firmware
 *
 * Copyright (C) Casainho, 2020
 *
 * Released under the GPL License, Version 3
 */

#include <stdint.h>
#include "antplus_controls_page_82.h"

typedef struct
{
  uint8_t reserved_1;
  uint8_t reserved_2;
  uint8_t cumulative_operation_time_lsb;
  uint8_t cumulative_operation_time_1;
  uint8_t cumulative_operation_time_msb;
  uint8_t fractional_battery_voltage;
  uint8_t descriptive_bit_field;
} antplus_controls_page_82_data_layout_t;

void antplus_controls_page_82_encode(uint8_t *p_page_buffer,
                                     antplus_controls_page_82_data_t const *p_page_data)
{
#define BATTERY_VOLTAGE_X10_50 290 // 2.9 volts
#define BATTERY_VOLTAGE_X10_10 285
#define BATTERY_VOLTAGE_X10_0 273

  antplus_controls_page_82_data_layout_t *p_outcoming_data = (antplus_controls_page_82_data_layout_t *)p_page_buffer;

  uint16_t bat_volt_x10 = p_page_data->battery_voltage_x100;

  p_outcoming_data->reserved_1 = 0xff;
  p_outcoming_data->reserved_2 = 0xff;

  // fake for now - will it work?
  p_outcoming_data->cumulative_operation_time_lsb = 1;
  p_outcoming_data->cumulative_operation_time_1 = 2;
  p_outcoming_data->cumulative_operation_time_msb = 3;

  p_outcoming_data->fractional_battery_voltage = ((bat_volt_x10 % 100) * 255) / 100;
  p_outcoming_data->descriptive_bit_field = (bat_volt_x10 / 100) & 0x0f;

  if (bat_volt_x10 > BATTERY_VOLTAGE_X10_50)
  {
    p_outcoming_data->descriptive_bit_field |= (3 << 4); // Battery Status = Ok
  }
  else if (bat_volt_x10 > BATTERY_VOLTAGE_X10_10)
  {
    p_outcoming_data->descriptive_bit_field |= (4 << 4); // Battery Status = Low
  }
  else
  {
    p_outcoming_data->descriptive_bit_field |= (5 << 4); // Battery Status = Critical
  }
}
