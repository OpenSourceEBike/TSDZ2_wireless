/*
 * TSDZ2 wireless firmware
 *
 * Copyright (C) Casainho, 2020
 *
 * Released under the GPL License, Version 3
 */

#include <stdint.h>
#include "ant_lev_page_3.h"

typedef struct
{
    uint8_t battery_soc;
    uint8_t travel_mode_state;
    uint8_t system_state;
    uint8_t gear_state;
    uint8_t assist;
    uint8_t lev_speed_lsb;
    uint8_t lev_speed_msn;
} ant_lev_page_3_data_layout_t;

void ant_lev_page_3_encode(uint8_t                    * p_page_buffer,
                           ant_lev_page_3_data_t const * p_page_data)
{
    ant_lev_page_3_data_layout_t * p_outcoming_data = (ant_lev_page_3_data_layout_t *)p_page_buffer;

    p_outcoming_data->battery_soc = p_page_data->battery_soc;
    p_outcoming_data->travel_mode_state = p_page_data->travel_mode_state;
    p_outcoming_data->system_state = p_page_data->system_state;
    p_outcoming_data->gear_state = p_page_data->gear_state;
    p_outcoming_data->assist = p_page_data->assist;
    p_outcoming_data->lev_speed_lsb = ((uint8_t) p_page_data->lev_speed) & 0xff;
    p_outcoming_data->lev_speed_msn = ((uint8_t) (p_page_data->lev_speed >> 8)) & 0x0f;
}
