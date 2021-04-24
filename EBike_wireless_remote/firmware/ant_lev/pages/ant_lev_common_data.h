/*
 * TSDZ2 EBike wireless firmware
 *
 * Copyright (C) Casainho, 2020
 *
 * Released under the GPL License, Version 3
 */

#ifndef ANT_LEV_COMMON_DATA_H__
#define ANT_LEV_COMMON_DATA_H__

#include <stdint.h>

typedef struct
{
    uint8_t travel_mode_state;
    uint8_t system_state;
    uint8_t gear_state;
    uint16_t lev_speed;
    uint32_t odometer;
} ant_lev_common_data_t;

#define DEFAULT_ANT_LEV_COMMON_DATA() \
    (ant_lev_common_data_t)           \
    {                                 \
        .travel_mode_state = 0,       \
        .system_state = 0x8,          \
        .gear_state = 0,              \
        .lev_speed = 0xFA,            \
        .odometer = 0,                \
    }

#endif