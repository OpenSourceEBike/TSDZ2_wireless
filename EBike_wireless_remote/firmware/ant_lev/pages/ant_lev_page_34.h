/*
 * TSDZ2 EBike wireless firmware
 *
 * Copyright (C) Casainho, 2020
 *
 * Released under the GPL License, Version 3
 */

#ifndef ANT_LEV_PAGE_34_H__
#define ANT_LEV_PAGE_34_H__

#include <stdint.h>
#include "ant_lev_common_data.h"

typedef struct
{
    uint16_t fuel_consumption;
} ant_lev_page_34_data_t;

#define DEFAULT_ANT_LEV_PAGE34() \
    (ant_lev_page_34_data_t)     \
    {                            \
        .fuel_consumption = 0,   \
    }

void ant_lev_page_34_encode(uint8_t *p_page_buffer,
                            ant_lev_page_34_data_t const *p_page_data,
                            ant_lev_common_data_t const *p_common_data);
void ant_lev_page_34_decode(const uint8_t *p_page_buffer,
                            ant_lev_page_34_data_t *p_page_data,
                            ant_lev_common_data_t *p_common_data);

#endif