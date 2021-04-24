/*
 * TSDZ2 EBike wireless firmware
 *
 * Copyright (C) Casainho, 2020
 *
 * Released under the GPL License, Version 3
 */

#ifndef ANTPLUS_CONTROLS_PAGE_73_H__
#define ANTPLUS_CONTROLS_PAGE_73_H__

#include <stdint.h>

typedef struct
{
  uint32_t utf8_character;
} antplus_controls_page_73_data_t;

#define DEFAULT_ANTPLUS_CONTROLS_PAGE73() \
  (antplus_controls_page_73_data_t)       \
  {                                       \
    .utf8_character = 0,                  \
  }

void antplus_controls_page_73_encode(uint8_t *p_page_buffer,
                                     antplus_controls_page_73_data_t const *p_page_data);

#endif