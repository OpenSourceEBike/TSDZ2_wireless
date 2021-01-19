/*
 * TSDZ2 EBike wireless firmware
 *
 * Copyright (C) Casainho, 2020
 *
 * Released under the GPL License, Version 3
 */

#ifndef ANT_LEV_LOCAL_H__
#define ANT_LEV_LOCAL_H__

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct
    {
        uint8_t block_cnt;
        bool pag_2_34;
        bool pag_4_5;
        ant_lev_page_t common_page_number;
        uint8_t message_counter;
        ant_request_controller_t req_controller;
    } ant_lev_disp_cb_t;

#ifdef __cplusplus
}
#endif

#endif // ANT_LEV_LOCAL_H__
