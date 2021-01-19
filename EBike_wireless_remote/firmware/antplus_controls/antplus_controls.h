/*
 * TSDZ2 EBike wireless firmware
 *
 * Copyright (C) Casainho, 2020
 *
 * Released under the GPL License, Version 3
 */

#ifndef ANTPLUS_CONTROLS_H__
#define ANTPLUS_CONTROLS_H__

#include <stdint.h>
#include <stdbool.h>
#include "ant_parameters.h"
#include "nrf_sdh_ant.h"
#include "ant_channel_config.h"
#include "ant_request_controller.h"
#include "antplus_controls_pages.h"
#include "sdk_errors.h"
#include "pins.h"
extern void ANT_Search_Start(void);
extern void ANT_Search_Stop(void);
#define CONTROLS_DEVICE_TYPE 0x10u     // ANT+ controls datasheet: 16 (0x10) – indicates search for an ANT+ controllable device.
#define CONTROLS_ANTPLUS_RF_FREQ 0x39u // ANT+ controls datasheet: Frequency, decimal 57 / 0x39 (2457 MHz).

#define CONTROLS_MSG_PERIOD_4Hz 0x2000u // Message period, decimal 8192 (4.00 Hz).
//#define CONTROLS_MSG_PERIOD_4Hz    0x1F86u   ///< Message period, decimal 8070 (4.06 Hz).
#define CONTROLS_MSG_PERIOD_2Hz 0x3F0Cu ///< Message period, decimal 16140 (2.03 Hz).
#define CONTROLS_MSG_PERIOD_1Hz 0x7E18u ///< Message period, decimal 32280 (1.02 Hz).
#define CONTROLS_EXT_ASSIGN 0x00                      //< ANT ext assign (see Ext. Assign Channel Parameters in ant_parameters.h: @ref ant_parameters).
#define CONTROLS_SENS_CHANNEL_TYPE CHANNEL_TYPE_SLAVE // TX controls channel type.

#define CONTROLS_SENS_CHANNEL_CONFIG_DEF(NAME,                                        \
                                    CHANNEL_NUMBER,                              \
                                    TRANSMISSION_TYPE,                           \
                                    DEVICE_NUMBER,                                  \
                                    NETWORK_NUMBER,                                 \
                                    CONTROLS_MSG_PERIOD_4Hz)                        \
   static const ant_channel_config_t CONCAT_2(NAME, _channel_controls_sens_config) = \
        {                                                                             \
            .channel_number = (CHANNEL_NUMBER),                                       \
            .channel_type = CONTROLS_SENS_CHANNEL_TYPE,                               \
            .ext_assign = CONTROLS_EXT_ASSIGN,                                        \
            .rf_freq = CONTROLS_ANTPLUS_RF_FREQ,                                      \
            .transmission_type = (TRANSMISSION_TYPE),                                 \
            .device_type = CONTROLS_DEVICE_TYPE,                                      \
            .device_number = (DEVICE_NUMBER),                                         \
            .channel_period = CONTROLS_MSG_PERIOD_4Hz,                                \
            .network_number = (NETWORK_NUMBER),                                       \
    }
#define CONTROLS_SENS_CHANNEL_CONFIG(NAME) &CONCAT_2(NAME, _channel_controls_sens_config)

#define CONTROLS_SENS_PROFILE_CONFIG_DEF(NAME,                                                  \
                                         EVT_HANDLER)                                           \
    static antplus_controls_sens_cb_t CONCAT_2(NAME, _controls_sens_cb);                        \
    static const antplus_controls_sens_config_t CONCAT_2(NAME, _profile_controls_sens_config) = \
        {                                                                                       \
            .p_cb = &CONCAT_2(NAME, _controls_sens_cb),                                         \
            .evt_handler = (EVT_HANDLER),                                                       \
    }
   
#define CONTROLS_SENS_PROFILE_CONFIG(NAME) &CONCAT_2(NAME, _profile_controls_sens_config)

typedef enum
{
    ANTPLUS_CONTROLS_PAGE_73 = 73,
    ANTPLUS_CONTROLS_PAGE_82 = 82,
    ANTPLUS_CONTROLS_PAGE_80 = ANT_COMMON_PAGE_80,
    ANTPLUS_CONTROLS_PAGE_81 = ANT_COMMON_PAGE_81,
} antplus_controls_page_t;

typedef enum
{
    ANTPLUS_CONTROLS_PAGE_73_UPDATED = ANTPLUS_CONTROLS_PAGE_73,
    ANTPLUS_CONTROLS_PAGE_82_UPDATED = ANTPLUS_CONTROLS_PAGE_82,
    ANTPLUS_CONTROLS_PAGE_80_UPDATED = ANTPLUS_CONTROLS_PAGE_80,
    ANTPLUS_CONTROLS_PAGE_81_UPDATED = ANTPLUS_CONTROLS_PAGE_81,
    ANTPLUS_CONTROLS_PAGE_REQUEST_SUCCESS,
    ANTPLUS_CONTROLS_PAGE_REQUEST_FAILED,
} antplus_controls_evt_t;

// Forward declaration of the antplus_controls_profile_t type.
typedef struct antplus_controls_profile_s antplus_controls_profile_t;

/**@brief SDM event handler type. */
typedef void (*antplus_controls_evt_handler_t)(antplus_controls_profile_t *, antplus_controls_evt_t);

#include "antplus_controls_local.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**@brief LEV Sensor configuration structure. */
    typedef struct
    {
        antplus_controls_sens_cb_t *p_cb;           ///< Pointer to the data buffer for internal use.
        antplus_controls_evt_handler_t evt_handler; ///< Event handler to be called for handling events in the SDM profile.
    } antplus_controls_sens_config_t;

    struct antplus_controls_profile_s
    {
        uint8_t channel_number; ///< Channel number assigned to the profile.
        union
        {
            antplus_controls_sens_cb_t *p_sens_cb;
        } _cb;                                      ///< Pointer to internal control block.
        antplus_controls_evt_handler_t evt_handler; ///< Event handler to be called for handling events in the LEV profile.
        antplus_controls_page_73_data_t page_73;    ///< Page 73.
        antplus_controls_page_82_data_t page_82;    ///< Page 82.
        antplus_common_page80_data_t page_80;       ///< Page 80.
        antplus_common_page81_data_t page_81;       ///< Page 81.
    };

    ret_code_t antplus_controls_sens_init(antplus_controls_profile_t *p_profile,
                                          ant_channel_config_t const *p_channel_config,
                                          antplus_controls_sens_config_t const *p_sens_config);

    ret_code_t antplus_controls_sens_open(antplus_controls_profile_t *p_profile);

    void antplus_controls_sens_evt_handler(ant_evt_t *p_ant_evt, void *p_context);

    void buttons_send_pag73(antplus_controls_profile_t *p_profile, button_pins_t button,uint8_t pagectrl);

#ifdef __cplusplus
}
#endif

#endif
