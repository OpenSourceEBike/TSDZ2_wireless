/*
 * TSDZ2 EBike wireless firmware
 *
 * Copyright (C) Casainho, 2020
 *
 * Released under the GPL License, Version 3
 */

#include "sdk_common.h"

#include "nrf_assert.h"
#include "app_error.h"
#include "ant_interface.h"
#include "ant_lev_pages.h"
#include "ant_lev.h"
#include "app_error.h"
#include "pins.h"
#include "app_button.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_delay.h"
#include "led_softblink.h"

#define COMMON_DATA_INTERVAL 20 /**< Common data page is sent every 20th message. */

void shutdown(void);
typedef struct
{
    ant_lev_page_t page_number;
    uint8_t page_payload[7];
} ant_lev_message_layout_t;
/**@brief Function for initializing the ANT LEV profile instance.
 *
 * @param[in]  p_profile        Pointer to the profile instance.
 * @param[in]  p_channel_config Pointer to the ANT channel configuration structure.
 *
 * @retval     NRF_SUCCESS      If initialization was successful. Otherwise, an error code is returned.
 */
static ret_code_t ant_lev_init(ant_lev_profile_t *p_profile,
                               ant_channel_config_t const *p_channel_config)
{
    p_profile->channel_number = p_channel_config->channel_number;

    p_profile->page_1 = DEFAULT_ANT_LEV_PAGE1();
    p_profile->page_2 = DEFAULT_ANT_LEV_PAGE2();
    p_profile->page_3 = DEFAULT_ANT_LEV_PAGE3();
    p_profile->page_4 = DEFAULT_ANT_LEV_PAGE4();
    p_profile->page_5 = DEFAULT_ANT_LEV_PAGE5();
    p_profile->page_16 = DEFAULT_ANT_LEV_PAGE16();
    p_profile->page_34 = DEFAULT_ANT_LEV_PAGE34();
    p_profile->common = DEFAULT_ANT_LEV_COMMON_DATA();
    p_profile->page_80 = DEFAULT_ANT_COMMON_page80();
    p_profile->page_81 = DEFAULT_ANT_COMMON_page81();

    return ant_channel_init(p_channel_config);
}
ret_code_t ant_lev_disp_init(ant_lev_profile_t *p_profile,
                             ant_channel_config_t const *p_channel_config,
                             ant_lev_evt_handler_t evt_handler)
{
    ASSERT(p_profile != NULL);
    ASSERT(p_channel_config != NULL);
    ASSERT(evt_handler != NULL);

    p_profile->evt_handler = evt_handler;

    return ant_lev_init(p_profile, p_channel_config);
}

bool buttons_send_page16(ant_lev_profile_t *p_profile, button_pins_t button, bool m_button_long_press)
{
    ASSERT(p_profile != NULL);
    bool long_press = m_button_long_press;
    bool send_page = false;
    //open if previously closed
    sd_ant_channel_open(p_profile->channel_number);
    if (!long_press)
    {
        if (button == MINUS__PIN)
        {
            if (p_profile->page_16.travel_mode != 0)
                p_profile->page_16.travel_mode -= 8;
            //p_profile->page_16.travel_mode=p_profile->common.travel_mode_state;
            send_page = true;
        }
        else if (button == PLUS__PIN)
        {
            if (p_profile->page_16.travel_mode != 56)
                p_profile->page_16.travel_mode += 8;

            send_page = true;
        }
        else if (button == ENTER__PIN)
        {
            // p_profile->page_16.wheel_circumference -= 10; //1818 circum

            // send_page = true;
        }
        else if (button == STANDBY__PIN)
        {
            // p_profile->page_16.manufacturer_id += 1;

            //send_page = true;
        }
    }
    else //long press actions
    {
        /*
        if (button == MINUS__PIN)
        {
            if (p_profile->page_16.travel_mode != 0)
                p_profile->page_16.travel_mode -= 8;
            //p_profile->page_16.travel_mode=p_profile->common.travel_mode_state;
            send_page = true;
        }
        else if (button == PLUS__PIN)
        {
            if (p_profile->page_16.travel_mode != 56)
                p_profile->page_16.travel_mode += 8;

            send_page = true;
        }
        
        if (button == ENTER__PIN)
        {
           p_profile->page_16.wheel_circumference += 10; //1818 circum

            send_page = true;
        }
        else if (button == STANDBY__PIN)
        {
           p_profile->page_16.manufacturer_id -= 1;

            send_page = true;
        }
        */
    }
    // send_page = true;
    if (send_page)
    {
        send_page = false;

        static uint8_t p_message_payload[ANT_STANDARD_DATA_PAYLOAD_SIZE] = {
            ANT_LEV_PAGE_16,
            0x01,
            0x00,
            0x0F,
            0x00,
            0x00,
            0x00,
            0x00};

        ant_lev_message_layout_t *p_lev_message_payload =
            (ant_lev_message_layout_t *)p_message_payload;

        ant_lev_page_16_encode(p_lev_message_payload->page_payload,
                               &(p_profile->page_16));

        uint32_t err_code;
        err_code = sd_ant_acknowledge_message_tx(p_profile->channel_number, sizeof(p_message_payload), p_message_payload);
        if (err_code != 0)
        {
            // nrf_delay_ms(50);
        }
        (void)err_code; // ignore
        //nrf_delay_ms(50);
    }

    if (send_page)
        return true;
    else
        return false;
}

static void disp_message_decode(ant_lev_profile_t *p_profile, uint8_t *p_message_payload)
{
    const ant_lev_message_layout_t *p_lev_message_payload =
        (ant_lev_message_layout_t *)p_message_payload;

    // NRF_LOG_INFO("HRM RX Page Number: %u", p_lev_message_payload->page_number);

    //ant_lev_page_0_decode(p_lev_message_payload->page_payload, &(p_profile->page_0)); // Page 0 is present in each message

    switch (p_lev_message_payload->page_number)
    {
    case ANT_LEV_PAGE_16:
        ant_lev_page_16_decode(p_lev_message_payload->page_payload, &(p_profile->page_16));
        break;

    case ANT_LEV_PAGE_1:
        ant_lev_page_1_decode(p_lev_message_payload->page_payload, &(p_profile->page_1), &(p_profile->common));
        break;

    case ANT_LEV_PAGE_2:
        ant_lev_page_2_decode(p_lev_message_payload->page_payload, &(p_profile->page_2), &(p_profile->common));
        break;

    case ANT_LEV_PAGE_3:
        ant_lev_page_3_decode(p_lev_message_payload->page_payload, &(p_profile->page_3), &(p_profile->common));
        break;

    case ANT_LEV_PAGE_4:
        ant_lev_page_4_decode(p_lev_message_payload->page_payload, &(p_profile->page_4));
        break;

    case ANT_LEV_PAGE_5:
        ant_lev_page_5_decode(p_lev_message_payload->page_payload, &(p_profile->page_5));
        break;

    case ANT_LEV_PAGE_34:
        ant_lev_page_34_decode(p_lev_message_payload->page_payload, &(p_profile->page_34), &(p_profile->common));
        break;

    default:
        return;
    }

    p_profile->evt_handler(p_profile, (ant_lev_evt_t)p_lev_message_payload->page_number);
}

void ant_lev_disp_evt_handler(ant_evt_t *p_ant_evt, void *p_context)
{
    ASSERT(p_context != NULL);
    ASSERT(p_ant_evt != NULL);
    ant_lev_profile_t *p_profile = (ant_lev_profile_t *)p_context;
    static int8_t restart_count = 0;

    if (p_ant_evt->channel == p_profile->channel_number)
    {
        uint32_t err_code;
        uint8_t p_message_payload[ANT_STANDARD_DATA_PAYLOAD_SIZE];
        ant_lev_disp_cb_t *p_lev_cb = p_profile->_cb.p_sens_cb;
        //  ant_request_controller_disp_evt_handler(&(p_lev_cb->req_controller), p_ant_evt);

        switch (p_ant_evt->event)
        {
        case EVENT_TX:
        case EVENT_TRANSFER_TX_FAILED:
        case EVENT_TRANSFER_TX_COMPLETED:

            disp_message_decode(p_profile, p_message_payload);
            if (ant_request_controller_ack_needed(&(p_lev_cb->req_controller)))
            {
                err_code = sd_ant_acknowledge_message_tx(p_profile->channel_number,
                                                         sizeof(p_message_payload),
                                                         p_message_payload);
                APP_ERROR_CHECK(err_code);
            }
            /*           else
            {
               err_code = sd_ant_broadcast_message_tx(p_profile->channel_number,
                                                       sizeof(p_message_payload),
                                                       p_message_payload); 
             APP_ERROR_CHECK(err_code);                                          
                                                       
                                                       
                                                      
            } */
            break;

        case EVENT_RX:
            if (p_ant_evt->message.ANT_MESSAGE_ucMesgID == MESG_BROADCAST_DATA_ID || p_ant_evt->message.ANT_MESSAGE_ucMesgID == MESG_ACKNOWLEDGED_DATA_ID || p_ant_evt->message.ANT_MESSAGE_ucMesgID == MESG_BURST_DATA_ID)
            {
                disp_message_decode(p_profile, p_ant_evt->message.ANT_MESSAGE_aucPayload);
            }
            break;
        case EVENT_RX_SEARCH_TIMEOUT:
            ANT_Search_Stop();

            break;
        case EVENT_CHANNEL_CLOSED:
            //communication has been lost
            //try restarting up to 12 times if connection cannot be made (3 min total search time)
            restart_count += 1;
            if (restart_count < 12)
            {
                sd_ant_channel_open(p_profile->channel_number);
                ANT_Search_Start();
            }
            break;
        case EVENT_RX_FAIL_GO_TO_SEARCH:
            ANT_Search_Start();
            break;
        default:
            break;
        }
    }
}

ret_code_t ant_lev_disp_open(ant_lev_profile_t *p_profile)
{
    ASSERT(p_profile != NULL);

    // NRF_LOG_INFO("ANT LEV channel %u open", p_profile->channel_number);
    return sd_ant_channel_open(p_profile->channel_number);
}
