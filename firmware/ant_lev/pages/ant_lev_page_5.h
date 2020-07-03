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
    uint8_t travel_modes_supported;
    uint16_t wheel_circumference;
} ant_lev_page_5_data_t;