/*
 * TSDZ2 EBike wireless firmware
 *
 * Copyright (C) Casainho, 2020
 *
 * Released under the GPL License, Version 3
 */

#include <stdint.h>
#include "ant_lev_page_3.h"
#include "ant_lev_common_data.h"

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

void ant_lev_page_3_encode(uint8_t *p_page_buffer,
                           ant_lev_page_3_data_t const *p_page_data,
                           ant_lev_common_data_t const *p_common_data)
{
    ant_lev_page_3_data_layout_t *p_outcoming_data = (ant_lev_page_3_data_layout_t *)p_page_buffer;

    p_outcoming_data->battery_soc = p_page_data->battery_soc;
    p_outcoming_data->assist = p_page_data->assist;

    p_outcoming_data->travel_mode_state = p_common_data->travel_mode_state;
    p_outcoming_data->system_state = p_common_data->system_state;
    p_outcoming_data->gear_state = p_common_data->gear_state;
    p_outcoming_data->lev_speed_lsb = ((uint8_t)p_common_data->lev_speed) & 0xff;
    p_outcoming_data->lev_speed_msn = ((uint8_t)(p_common_data->lev_speed >> 8)) & 0x0f;
}
void ant_lev_page_3_decode(const uint8_t *p_page_buffer,
                           ant_lev_page_3_data_t *p_page_data,
                           ant_lev_common_data_t *p_common_data)

{
    ant_lev_page_3_data_layout_t *p_incoming_data = (ant_lev_page_3_data_layout_t *)p_page_buffer;

    p_page_data->battery_soc = p_incoming_data->battery_soc;
    p_page_data->assist = p_incoming_data->assist;
    p_common_data->travel_mode_state = p_incoming_data->system_state;
    p_common_data->gear_state = p_incoming_data->gear_state;
    p_common_data->lev_speed = ((uint16_t)p_incoming_data->lev_speed_msn & 0x0f << 8) | (uint16_t)(p_incoming_data->lev_speed_lsb);
}
