/*
 * TSDZ2 EBike wireless firmware
 *
 * Copyright (C) Casainho, 2020
 *
 * Released under the GPL License, Version 3
 */

#include <stdint.h>
#include "ant_lev_common_data.h"

typedef struct
{
    uint8_t temperature_state;
    uint8_t travel_mode_state;
    uint8_t system_state;
    uint8_t gear_state;
    uint8_t error_message;
    uint8_t lev_speed_lsb;
    uint8_t lev_speed_msn;
} ant_lev_common_data_layout_t;

