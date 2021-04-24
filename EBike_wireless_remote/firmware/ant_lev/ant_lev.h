/*
 * TSDZ2 EBike wireless firmware
 *
 * Copyright (C) Casainho, 2020
 *
 * Released under the GPL License, Version 3
 */

#ifndef ANT_LEV_H__
#define ANT_LEV_H__

#include <stdint.h>
#include <stdbool.h>
#include "ant_parameters.h"
#include "nrf_sdh_ant.h"
#include "ant_channel_config.h"
#include "ant_request_controller.h"
#include "ant_lev_pages.h"
#include "sdk_errors.h"
#include "pins.h"
extern void ANT_Search_Start(void);
extern void ANT_Search_Stop(void);
#define LEV_DEVICE_TYPE 0x14u      // ANT+ LEV datasheet: Device type reserved for ANT+ LEV.
#define LEV_ANTPLUS_RF_FREQ 0x39u  // ANT+ LEV datasheet: Frequency, decimal 57 / 0x39 (2457 MHz).
#define LEV_MSG_PERIOD_4Hz 0x2000u ///< Message period, decimal 8192 (4.00 Hz).
//#define LEV_MSG_PERIOD_4Hz    0x1F86u   ///< Message period, decimal 8070 (4.06 Hz).
#define LEV_MSG_PERIOD_2Hz 0x3F0Cu ///< Message period, decimal 16140 (2.03 Hz).
#define LEV_MSG_PERIOD_1Hz 0x7E18u ///< Message period, decimal 32280 (1.02 Hz).

#define LEV_EXT_ASSIGN 0x00                      //< ANT ext assign (see Ext. Assign Channel Parameters in ant_parameters.h: @ref ant_parameters).
#define LEV_DISP_CHANNEL_TYPE CHANNEL_TYPE_SLAVE // TX LEV channel type.

#define LEV_DISP_CHANNEL_CONFIG_DEF(NAME,                                        \
                                    CHANNEL_NUMBER,                              \
                                    TRANSMISSION_TYPE,                           \
                                    DEVICE_NUMBER,                               \
                                    NETWORK_NUMBER,                              \
                                    LEV_MSG_PERIOD_4Hz)                          \
     static  ant_channel_config_t CONCAT_2(NAME, _channel_lev_disp_config) = \
        {                                                                        \
            .channel_number = (CHANNEL_NUMBER),                                  \
            .channel_type = LEV_DISP_CHANNEL_TYPE,                               \
            .ext_assign = LEV_EXT_ASSIGN,                                        \
            .rf_freq = LEV_ANTPLUS_RF_FREQ,                                      \
            .transmission_type = (TRANSMISSION_TYPE),                            \
            .device_type = LEV_DEVICE_TYPE,                                      \
            .device_number = (DEVICE_NUMBER),                                    \
            .channel_period = LEV_MSG_PERIOD_4Hz,                                \
            .network_number = (NETWORK_NUMBER),                                  \
    }
    // was     static const ant_channel_config_t CONCAT_2(NAME, _channel_lev_disp_config) to allow old_ant_device_id to be set
#define LEV_DISP_CHANNEL_CONFIG(NAME) &CONCAT_2(NAME, _channel_lev_disp_config)

#define LEV_DISP_PROFILE_CONFIG_DEF(NAME,                                         \
                                    EVT_HANDLER)                                  \
    static ant_lev_disp_cb_t CONCAT_2(NAME, _lev_disp_cb);                        \
    static const ant_lev_disp_config_t CONCAT_2(NAME, _profile_lev_disp_config) = \
        {                                                                         \
            .p_cb = &CONCAT_2(NAME, _lev_sens_cb),                                \
            .evt_handler = (EVT_HANDLER),                                         \
    }
#define LEV_SENS_PROFILE_CONFIG(NAME) &CONCAT_2(NAME, _profile_lev_sens_config)

typedef enum
{
    ANT_LEV_PAGE_1 = 1,
    ANT_LEV_PAGE_2 = 2,
    ANT_LEV_PAGE_3 = 3,
    ANT_LEV_PAGE_4 = 4,
    ANT_LEV_PAGE_5 = 5,
    ANT_LEV_PAGE_16 = 16,
    ANT_LEV_PAGE_34 = 34,
    ANT_LEV_PAGE_70 = ANT_COMMON_PAGE_70,
    ANT_LEV_PAGE_80 = ANT_COMMON_PAGE_80,
    ANT_LEV_PAGE_81 = ANT_COMMON_PAGE_81,
} ant_lev_page_t;

typedef enum
{
    ANT_LEV_PAGE_1_UPDATED = ANT_LEV_PAGE_1,
    ANT_LEV_PAGE_2_UPDATED = ANT_LEV_PAGE_2,
    ANT_LEV_PAGE_3_UPDATED = ANT_LEV_PAGE_3,
    ANT_LEV_PAGE_4_UPDATED = ANT_LEV_PAGE_4,
    ANT_LEV_PAGE_5_UPDATED = ANT_LEV_PAGE_5,
    ANT_LEV_PAGE_16_UPDATED = ANT_LEV_PAGE_16,
    ANT_LEV_PAGE_34_UPDATED = ANT_LEV_PAGE_34,
    ANT_LEV_PAGE_80_UPDATED = ANT_LEV_PAGE_80,
    ANT_LEV_PAGE_81_UPDATED = ANT_LEV_PAGE_81,
    ANT_LEV_PAGE_REQUEST_SUCCESS,
    ANT_LEV_PAGE_REQUEST_FAILED,
} ant_lev_evt_t;

// Forward declaration of the ant_lev_profile_t type.
typedef struct ant_lev_profile_s ant_lev_profile_t;

/**@brief SDM event handler type. */
typedef void (*ant_lev_evt_handler_t)(ant_lev_profile_t *, ant_lev_evt_t);

#include "ant_lev_local.h"

#ifdef __cplusplus
extern "C"
{
#endif
    bool buttons_send_page16(ant_lev_profile_t *p_profile, button_pins_t button, bool long_press);
    /**@brief LEV Sensor configuration structure. */
    typedef struct
    {
        ant_lev_disp_cb_t *p_cb;           ///< Pointer to the data buffer for internal use.
        ant_lev_evt_handler_t evt_handler; ///< Event handler to be called for handling events in the SDM profile.
    } ant_lev_disp_config_t;

    struct ant_lev_profile_s
    {
        uint8_t channel_number; ///< Channel number assigned to the profile.
        union
        {
            ant_lev_disp_cb_t *p_sens_cb;
        } _cb;                             ///< Pointer to internal control block.
        ant_lev_evt_handler_t evt_handler; ///< Event handler to be called for handling events in the LEV profile.
        ant_lev_page_1_data_t page_1;      ///< Page 1.
        ant_lev_page_2_data_t page_2;      ///< Page 2.
        ant_lev_page_3_data_t page_3;      ///< Page 3.
        ant_lev_page_4_data_t page_4;      ///< Page 4.
        ant_lev_page_5_data_t page_5;      ///< Page 5.
        ant_lev_page_16_data_t page_16;    ///< Page 16.
        ant_lev_page_34_data_t page_34;    ///< Page 34.
        antplus_common_page80_data_t page_80;  ///< Page 80.
        antplus_common_page81_data_t page_81;  ///< Page 81.
        ant_lev_common_data_t common;      ///< SDM common data.
    };

    ret_code_t ant_lev_disp_init(ant_lev_profile_t *p_profile,
                                 ant_channel_config_t const *p_channel_config,
                                 ant_lev_evt_handler_t evt_handler);
    ret_code_t ant_lev_disp_open(ant_lev_profile_t *p_profile);

    ret_code_t ant_lev_page_request(ant_lev_profile_t *p_profile, antplus_common_page70_data_t *p_page_70);

    void ant_lev_disp_evt_handler(ant_evt_t *p_ant_evt, void *p_context);

#ifdef __cplusplus
}
#endif

#endif // ANT_LEV_H__
