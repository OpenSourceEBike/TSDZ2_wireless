/*
 * TSDZ2 EBike wireless firmware
 *
 * Copyright (C) Casainho, 2020
 *
 * Released under the GPL License, Version 3
 */

#include <stdint.h>
#include <stdbool.h>
#include "ant_lev_page_16.h"

typedef struct
{
    uint8_t wheel_circumference_lsb;
    uint8_t wheel_circumference_msn;
    uint8_t travel_mode;
    uint8_t display_command_lsb;
    uint8_t display_command_msb;
    uint8_t manufacturer_id_lsb;
    uint8_t manufacturer_id_msb;
} ant_lev_page_16_data_layout_t;

void ant_lev_page_16_decode(const uint8_t *p_page_buffer,
                            ant_lev_page_16_data_t *p_page_data)
{
    ant_lev_page_16_data_layout_t const *p_incoming_data = (ant_lev_page_16_data_layout_t *)p_page_buffer;

    p_page_data->wheel_circumference = ((uint16_t)p_incoming_data->wheel_circumference_lsb) |
                                       (((uint16_t)(p_incoming_data->wheel_circumference_msn & 0x0f)) << 8);

    p_page_data->travel_mode = p_incoming_data->travel_mode;

    p_page_data->manufacturer_id = ((uint16_t)p_incoming_data->manufacturer_id_lsb) |
                                   (((uint16_t)(p_incoming_data->manufacturer_id_msb & 0x0f)) << 8);

    p_page_data->turn_signal_right = (p_incoming_data->wheel_circumference_lsb & 0x01) ? true : false;
    p_page_data->turn_signal_left = (p_incoming_data->manufacturer_id_lsb & 0x02) ? true : false;
    p_page_data->light_beam = (p_incoming_data->manufacturer_id_lsb & 0x04) ? true : false;
    p_page_data->light = (p_incoming_data->manufacturer_id_lsb & 0x08) ? true : false;
    p_page_data->current_front_gear = (p_incoming_data->manufacturer_id_lsb & 0x30) >> 4;
    p_page_data->current_rear_gear = ((p_incoming_data->manufacturer_id_lsb & 0xC0) >> 6) |
                                     ((p_incoming_data->manufacturer_id_msb & 0x02) >> 8);
}
void ant_lev_page_16_encode(uint8_t *p_page_buffer,
                            ant_lev_page_16_data_t const *p_page_data)
{
    ant_lev_page_16_data_layout_t *p_outcoming_data = (ant_lev_page_16_data_layout_t *)p_page_buffer;
    p_outcoming_data->wheel_circumference_lsb = ((uint8_t)p_page_data->wheel_circumference) & 0xFF;
    p_outcoming_data->wheel_circumference_msn = ((uint8_t)(p_page_data->wheel_circumference >> 8)) & 0x0F;
    p_outcoming_data->travel_mode = p_page_data->travel_mode;
    p_outcoming_data->manufacturer_id_lsb = ((uint8_t)p_page_data->manufacturer_id) & 0xFF;
    p_outcoming_data->manufacturer_id_msb = ((uint8_t)(p_page_data->manufacturer_id >> 8)) & 0x0F;
}
