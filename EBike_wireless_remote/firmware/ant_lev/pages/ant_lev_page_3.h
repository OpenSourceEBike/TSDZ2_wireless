/*
 * TSDZ2 EBike wireless firmware
 *
 * Copyright (C) Casainho, 2020
 *
 * Released under the GPL License, Version 3
 */

#ifndef ANT_LEV_PAGE_3_H__
#define ANT_LEV_PAGE_3_H__

#include <stdint.h>
#include "ant_lev_common_data.h"

typedef struct
{
    uint8_t battery_soc;
    uint8_t assist;
} ant_lev_page_3_data_t;

#define DEFAULT_ANT_LEV_PAGE3() \
    (ant_lev_page_3_data_t)     \
    {                           \
        .battery_soc = 80,      \
        .assist = 0x01,         \
    }

void ant_lev_page_3_encode(uint8_t *p_page_buffer,
                           ant_lev_page_3_data_t const *p_page_data,
                           ant_lev_common_data_t const *p_common_data);

void ant_lev_page_3_decode(const uint8_t *p_page_buffer,
                           ant_lev_page_3_data_t *p_page_data,
                           ant_lev_common_data_t *p_common_data);

#endif