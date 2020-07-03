/*
 * TSDZ2 wireless firmware
 *
 * Copyright (C) Casainho, 2020
 *
 * Released under the GPL License, Version 3
 */

#include <stdint.h>
#include "ant_lev_page_34.h"

typedef struct
{
    uint8_t odometer_lsb;
    uint8_t odometer;
    uint8_t odometer_msb;
    uint8_t fuel_consumption_lsb;
    uint8_t fuel_consumption_msn;
    uint8_t lev_speed_lsb;
    uint8_t lev_speed_msn;
} ant_lev_page_34_data_layout_t;

void ant_lev_page_34_encode(uint8_t                    * p_page_buffer,
                           ant_lev_page_34_data_t const * p_page_data)
{
    ant_lev_page_34_data_layout_t * p_outcoming_data = (ant_lev_page_34_data_layout_t *)p_page_buffer;

    p_outcoming_data->odometer_lsb = ((uint8_t) p_page_data->odometer) & 0xff;
    p_outcoming_data->odometer = ((uint8_t) (p_page_data->odometer >> 8)) & 0xff;
    p_outcoming_data->odometer_msb = ((uint8_t) (p_page_data->odometer >> 16)) & 0xff;
    p_outcoming_data->fuel_consumption_lsb = ((uint8_t) p_page_data->fuel_consumption) & 0xff;
    p_outcoming_data->fuel_consumption_msn = ((uint8_t) (p_page_data->fuel_consumption >> 8)) & 0x0f;
    p_outcoming_data->lev_speed_lsb = ((uint8_t) p_page_data->lev_speed) & 0xff;
    p_outcoming_data->lev_speed_msn = ((uint8_t) (p_page_data->lev_speed >> 8)) & 0x0f;
}
