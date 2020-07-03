/*
 * TSDZ2 wireless firmware
 *
 * Copyright (C) Casainho, 2020
 *
 * Released under the GPL License, Version 3
 */

#include <stdint.h>

typedef struct
{
    uint16_t charging_cycle_count;
    uint16_t fuel_consumption;
    uint8_t battery_voltage;
    uint16_t distance_on_current_charge;
} ant_lev_page_4_data_t;