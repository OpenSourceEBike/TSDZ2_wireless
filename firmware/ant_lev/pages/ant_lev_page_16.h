/*
 * TSDZ2 wireless firmware
 *
 * Copyright (C) Casainho, 2020
 *
 * Released under the GPL License, Version 3
 */

#ifndef ANT_LEV_PAGE_16_H__
#define ANT_LEV_PAGE_16_H__

#include <stdint.h>

typedef struct
{
    uint16_t wheel_circumference;
    uint8_t travel_mode;
    uint16_t display_command;
    uint16_t manufacturer_id;
} ant_lev_page_16_data_t;

#endif