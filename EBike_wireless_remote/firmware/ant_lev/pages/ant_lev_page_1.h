/*
 * TSDZ2 EBike wireless firmware
 *
 * Copyright (C) Casainho, 2020
 *
 * Released under the GPL License, Version 3
 */

#ifndef ANT_LEV_PAGE_1_H__
#define ANT_LEV_PAGE_1_H__

#include <stdint.h>
#include "ant_lev_common_data.h"

typedef struct
{
    uint8_t temperature_state;
    uint8_t error_message;
} ant_lev_page_1_data_t;

#define DEFAULT_ANT_LEV_PAGE1()  \
    (ant_lev_page_1_data_t)      \
    {                            \
        .temperature_state = 70, \
        .error_message = 0,      \
    }

void ant_lev_page_1_encode(uint8_t *p_page_buffer,
                           ant_lev_page_1_data_t const *p_page_data,
                           ant_lev_common_data_t const *p_common_data);

void ant_lev_page_1_decode(const uint8_t *p_page_buffer,
                           ant_lev_page_1_data_t *p_page_data,
                           ant_lev_common_data_t *p_common_data);
//void ant_lev_page_16_decode(const uint8_t * p_page_buffer,
//                         ant_lev_page_16_data_t * p_page_data);
#endif
