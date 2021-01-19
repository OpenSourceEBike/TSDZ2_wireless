/*
 * TSDZ2 EBike wireless firmware
 *
 * Copyright (C) Casainho, 2020
 *
 * Released under the GPL License, Version 3
 */

#ifndef ANT_LEV_PAGE_4_H__
#define ANT_LEV_PAGE_4_H__

#include <stdint.h>

typedef struct
{
    uint16_t charging_cycle_count;
    uint16_t fuel_consumption;
    uint8_t battery_voltage;
    uint16_t distance_on_current_charge;
} ant_lev_page_4_data_t;

#define DEFAULT_ANT_LEV_PAGE4()          \
    (ant_lev_page_4_data_t)              \
    {                                    \
        .charging_cycle_count = 0,       \
        .fuel_consumption = 0,           \
        .battery_voltage = 0xD0,         \
        .distance_on_current_charge = 0, \
    }

void ant_lev_page_4_encode(uint8_t *p_page_buffer,
                           ant_lev_page_4_data_t const *p_page_data);

void ant_lev_page_4_decode(const uint8_t *p_page_buffer,
                           ant_lev_page_4_data_t *p_page_data);

#endif