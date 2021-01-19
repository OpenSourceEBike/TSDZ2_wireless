/*
 * TSDZ2 EBike wireless firmware
 *
 * Copyright (C) Casainho, 2020
 *
 * Released under the GPL License, Version 3
 */

#ifndef ANT_LEV_PAGE_5_H__
#define ANT_LEV_PAGE_5_H__

#include <stdint.h>

typedef struct
{
    uint8_t travel_modes_supported;
    uint16_t wheel_circumference;
} ant_lev_page_5_data_t;

#define DEFAULT_ANT_LEV_PAGE5()         \
    (ant_lev_page_5_data_t)             \
    {                                   \
        .travel_modes_supported = 0x38, \
        .wheel_circumference = 2050,    \
    }

void ant_lev_page_5_encode(uint8_t *p_page_buffer,
                           ant_lev_page_5_data_t const *p_page_data);
void ant_lev_page_5_decode(const uint8_t *p_page_buffer,
                           ant_lev_page_5_data_t *p_page_data);

#endif