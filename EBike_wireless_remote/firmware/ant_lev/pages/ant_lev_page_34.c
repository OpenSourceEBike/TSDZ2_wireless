/*
 * TSDZ2 EBike wireless firmware
 *
 * Copyright (C) Casainho, 2020
 *
 * Released under the GPL License, Version 3
 */

#include <stdint.h>
#include "ant_lev_page_34.h"
#include "ant_lev_common_data.h"

typedef struct
{
  uint8_t odometer_lsb;
  uint8_t odometer;
  uint8_t odometer_msb;
  uint8_t fuel_consumption_lsb;
  uint8_t fuel_consumption_msn;
  uint8_t lev_speed_lsb;
  uint8_t lev_speed_msn;
} ant_lev_page_34_data_layout_t;

void ant_lev_page_34_encode(uint8_t *p_page_buffer,
                            ant_lev_page_34_data_t const *p_page_data,
                            ant_lev_common_data_t const *p_common_data)
{
  ant_lev_page_34_data_layout_t *p_outcoming_data = (ant_lev_page_34_data_layout_t *)p_page_buffer;

  p_outcoming_data->odometer_lsb = ((uint8_t)p_common_data->odometer) & 0xff;
  p_outcoming_data->odometer = ((uint8_t)(p_common_data->odometer >> 8)) & 0x0f;
  p_outcoming_data->odometer_msb = ((uint8_t)(p_common_data->odometer >> 16)) & 0x0f;

  p_outcoming_data->fuel_consumption_lsb = ((uint8_t)p_page_data->fuel_consumption) & 0xff;
  p_outcoming_data->fuel_consumption_msn = ((uint8_t)(p_page_data->fuel_consumption >> 8)) & 0x0f;

  p_outcoming_data->lev_speed_lsb = ((uint8_t)p_common_data->lev_speed) & 0xff;
  p_outcoming_data->lev_speed_msn = ((uint8_t)(p_common_data->lev_speed >> 8)) & 0x0f;
}
void ant_lev_page_34_decode(const uint8_t *p_page_buffer,
                            ant_lev_page_34_data_t *p_page_data,
                            ant_lev_common_data_t *p_common_data)

{
  ant_lev_page_34_data_layout_t *p_incoming_data = (ant_lev_page_34_data_layout_t *)p_page_buffer;
  p_common_data->odometer = ((uint16_t)p_incoming_data->odometer_msb & 0x0f << 8) | (uint16_t)(p_incoming_data->odometer_lsb);
  p_page_data->fuel_consumption = ((uint16_t)p_incoming_data->fuel_consumption_msn & 0x0f << 8) | (uint16_t)(p_incoming_data->fuel_consumption_lsb);
  p_common_data->lev_speed = ((uint16_t)p_incoming_data->lev_speed_msn & 0x0f << 8) | (uint16_t)(p_incoming_data->lev_speed_lsb);
  // p_page_data->wheel_circumference = ((uint16_t) p_incoming_data->wheel_circumference_lsb) |  (((uint16_t) (p_incoming_data->wheel_circumference_msn & 0x0f)) << 8);
}
