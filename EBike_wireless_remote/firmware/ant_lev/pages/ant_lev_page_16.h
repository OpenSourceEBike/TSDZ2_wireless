/*
 * TSDZ2 EBike wireless firmware
 *
 * Copyright (C) Casainho, 2020
 *
 * Released under the GPL License, Version 3
 */

#ifndef ANT_LEV_PAGE_16_H__
#define ANT_LEV_PAGE_16_H__

#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    uint16_t wheel_circumference;
    uint8_t travel_mode;
    uint16_t manufacturer_id;
    uint8_t current_rear_gear;
    uint8_t current_front_gear;
    bool light;
    bool light_beam;
    bool turn_signal_left;
    bool turn_signal_right;
} ant_lev_page_16_data_t;

#define DEFAULT_ANT_LEV_PAGE16()     \
    (ant_lev_page_16_data_t)         \
    {                                \
        .wheel_circumference = 1050, \
        .travel_mode = 0,           \
        .manufacturer_id = 108,      \
        .current_rear_gear = 0,      \
        .current_front_gear = 0,     \
        .light = false,              \
        .light_beam = false,         \
        .turn_signal_left = false,   \
        .turn_signal_right = false,  \
    }

void ant_lev_page_16_decode(const uint8_t *p_page_buffer,
                            ant_lev_page_16_data_t *p_page_data);

void ant_lev_page_16_encode(uint8_t *p_page_buffer,
                            ant_lev_page_16_data_t const *p_page_data);

#endif
