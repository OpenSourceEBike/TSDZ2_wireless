/*
 * TSDZ2 EBike wireless firmware
 *
 * Copyright (C) Casainho, 2020
 *
 * Released under the GPL License, Version 3
 * 
 */

#pragma once

#include <stdio.h>
#include "state.h"

extern ui_vars_t *gp_ui_vars;
extern volatile uint8_t ui8_g_enter_bootloader;
extern volatile uint8_t ui8_g_ant_device_id;

void tsdz2_write_handler_periodic(uint8_t *p_data, uint16_t len);
void tsdz2_write_handler_configurations(uint8_t *p_data, uint16_t len);