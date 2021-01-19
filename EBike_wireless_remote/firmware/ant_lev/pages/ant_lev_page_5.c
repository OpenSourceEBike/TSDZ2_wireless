/*
 * TSDZ2 EBike wireless firmware
 *
 * Copyright (C) Casainho, 2020
 *
 * Released under the GPL License, Version 3
 */

#include <stdint.h>
#include "ant_lev_page_5.h"

typedef struct
{
    uint8_t reserved_1;
    uint8_t travel_modes_supported;
    uint8_t wheel_circumference_lsb;
    uint8_t wheel_circumference_msn;
    uint8_t reserved_2;
    uint8_t reserved_3;
    uint8_t reserved_4;
} ant_lev_page_5_data_layout_t;

void ant_lev_page_5_encode(uint8_t *p_page_buffer,
                           ant_lev_page_5_data_t const *p_page_data)
{
    ant_lev_page_5_data_layout_t *p_outcoming_data = (ant_lev_page_5_data_layout_t *)p_page_buffer;

    p_outcoming_data->travel_modes_supported = p_page_data->travel_modes_supported;
    p_outcoming_data->wheel_circumference_lsb = ((uint8_t)p_page_data->wheel_circumference) & 0xff;
    p_outcoming_data->wheel_circumference_msn = ((uint8_t)(p_page_data->wheel_circumference >> 8)) & 0x0f;
}
void ant_lev_page_5_decode(const uint8_t *p_page_buffer,
                           ant_lev_page_5_data_t *p_page_data)

{
    ant_lev_page_5_data_layout_t *p_incoming_data = (ant_lev_page_5_data_layout_t *)p_page_buffer;

    p_page_data->travel_modes_supported = p_incoming_data->travel_modes_supported;
    p_page_data->wheel_circumference = ((uint16_t)p_incoming_data->wheel_circumference_msn & 0x0f << 8) | (uint16_t)(p_incoming_data->wheel_circumference_lsb);
    // p_page_data->wheel_circumference = ((uint16_t) p_incoming_data->wheel_circumference_lsb) |  (((uint16_t) (p_incoming_data->wheel_circumference_msn & 0x0f)) << 8);
}
