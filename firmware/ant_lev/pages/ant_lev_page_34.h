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
    uint32_t odometer;
    uint16_t fuel_consumption;
    uint16_t lev_speed;
} ant_lev_page_34_data_t;