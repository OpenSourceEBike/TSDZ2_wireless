/*
 * TSDZ2 EBike wireless firmware
 *
 * Copyright (C) Casainho, 2020
 *
 * Released under the GPL License, Version 3
 */

#include <stdint.h>
#include "antplus_controls_page_73.h"

typedef struct
{
  uint8_t serial_number_lsb;
  uint8_t serial_number_msb;
  uint8_t unknown_1;
  uint8_t unknown_2;
  uint8_t sequence;
  uint8_t utf8_character_lsb;
  uint8_t utf8_character_msb;
} antplus_controls_page_73_data_layout_t;

void antplus_controls_page_73_encode(uint8_t *p_page_buffer,
                                     antplus_controls_page_73_data_t const *p_page_data)
{
  antplus_controls_page_73_data_layout_t *p_outcoming_data = (antplus_controls_page_73_data_layout_t *)p_page_buffer;

  p_outcoming_data->sequence++;
  p_outcoming_data->utf8_character_lsb = ((uint8_t)p_page_data->utf8_character) & 0xff;
  p_outcoming_data->utf8_character_msb = ((uint8_t)(p_page_data->utf8_character >> 1)) & 0xff;
}
