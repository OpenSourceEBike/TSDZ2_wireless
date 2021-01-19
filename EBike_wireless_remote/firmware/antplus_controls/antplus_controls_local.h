/*
 * TSDZ2 EBike wireless firmware
 *
 * Copyright (C) Casainho, 2020
 *
 * Released under the GPL License, Version 3
 */

#ifndef ANTPLUS_CONTROLS_LOCAL_H__
#define ANTPLUS_CONTROLS_LOCAL_H__

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

  typedef struct
  {
    antplus_controls_page_t common_page_number;
    ant_request_controller_t req_controller;
  } antplus_controls_sens_cb_t;

#ifdef __cplusplus
}
#endif

#endif
