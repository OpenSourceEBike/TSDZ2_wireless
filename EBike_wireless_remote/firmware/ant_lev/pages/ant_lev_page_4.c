/*
 * TSDZ2 EBike wireless firmware
 *
 * Copyright (C) Casainho, 2020
 *
 * Released under the GPL License, Version 3
 */

#include <stdint.h>
#include "ant_lev_page_4.h"

typedef struct
{
    uint8_t reserved;
    uint8_t charging_cycle_count_lsb;
    uint8_t charging_cycle_count__fuel_consumption;
    uint8_t fuel_consumption_lsb;
    uint8_t battery_voltage;
    uint8_t distance_on_current_charge_lsb;
    uint8_t distance_on_current_charge_msb;
} ant_lev_page_4_data_layout_t;

void ant_lev_page_4_encode(uint8_t *p_page_buffer,
                           ant_lev_page_4_data_t const *p_page_data)
{
    ant_lev_page_4_data_layout_t *p_outcoming_data = (ant_lev_page_4_data_layout_t *)p_page_buffer;

    p_outcoming_data->charging_cycle_count_lsb = ((uint8_t)p_page_data->charging_cycle_count) & 0xff;
    p_outcoming_data->charging_cycle_count__fuel_consumption = ((uint8_t)(p_page_data->charging_cycle_count >> 8)) & 0x0f;
    p_outcoming_data->fuel_consumption_lsb = ((uint8_t)p_page_data->fuel_consumption) & 0xff;
    p_outcoming_data->charging_cycle_count__fuel_consumption |= ((uint8_t)(p_page_data->fuel_consumption >> 12)) & 0x0f;
    p_outcoming_data->battery_voltage = p_page_data->battery_voltage;
    p_outcoming_data->distance_on_current_charge_lsb = ((uint8_t)p_page_data->distance_on_current_charge) & 0xff;
    p_outcoming_data->distance_on_current_charge_msb = ((uint8_t)(p_page_data->distance_on_current_charge >> 8)) & 0x0f;
}
void ant_lev_page_4_decode(const uint8_t *p_page_buffer,
                           ant_lev_page_4_data_t *p_page_data)

{
    ant_lev_page_4_data_layout_t *p_incoming_data = (ant_lev_page_4_data_layout_t *)p_page_buffer;

    p_page_data->distance_on_current_charge = ((uint16_t)p_incoming_data->distance_on_current_charge_msb & 0x0f << 8) | (uint16_t)(p_incoming_data->distance_on_current_charge_lsb);
    p_page_data->battery_voltage = p_incoming_data->battery_voltage;
    p_page_data->fuel_consumption = ((uint16_t)p_incoming_data->charging_cycle_count__fuel_consumption & 0x0f << 12) | (uint16_t)(p_incoming_data->fuel_consumption_lsb);
}
