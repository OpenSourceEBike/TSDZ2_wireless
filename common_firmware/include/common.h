/*
 * TSDZ2 EBike wireless firmware
 *
 * Copyright (C) Casainho, 2021
 *
 * Released under the GPL License, Version 3
 */

#pragma once


void disp_soc(int soc);
void nrf_lp_delay_ms_init(void);
void nrf_lp_delay_ms(int ms_delay);