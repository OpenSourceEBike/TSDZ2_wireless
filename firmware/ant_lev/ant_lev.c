/*
 * TSDZ2 wireless firmware
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

#define COMMON_DATA_INTERVAL 20          /**< Common data page is sent every 20th message. */

typedef struct
{
    ant_lev_page_t  page_number;
    uint8_t         page_payload[7];
} ant_lev_message_layout_t;

static ret_code_t ant_lev_init(ant_lev_profile_t          * p_profile,
                               ant_channel_config_t const * p_channel_config)
{
    p_profile->channel_number = p_channel_config->channel_number;

    p_profile->page_1  = DEFAULT_ANT_LEV_PAGE1();
    p_profile->page_2  = DEFAULT_ANT_LEV_PAGE2();
    p_profile->page_3  = DEFAULT_ANT_LEV_PAGE3();
    p_profile->page_4  = DEFAULT_ANT_LEV_PAGE4();
    p_profile->page_5  = DEFAULT_ANT_LEV_PAGE5();
    p_profile->page_34  = DEFAULT_ANT_LEV_PAGE34();
    p_profile->common  = DEFAULT_ANT_LEV_COMMON_DATA();
    p_profile->page_80 = DEFAULT_ANT_COMMON_page80();
    p_profile->page_81 = DEFAULT_ANT_COMMON_page81();

    return ant_channel_init(p_channel_config);
}

ret_code_t ant_lev_sens_init(ant_lev_profile_t           * p_profile,
                             ant_channel_config_t const  * p_channel_config,
                             ant_lev_sens_config_t const * p_sens_config)
{
    ASSERT(p_profile != NULL);
    ASSERT(p_channel_config != NULL);
    ASSERT(p_sens_config != NULL);
    ASSERT(p_sens_config->p_cb != NULL);
    ASSERT(p_sens_config->evt_handler != NULL);

    p_profile->evt_handler    = p_sens_config->evt_handler;
    p_profile->_cb.p_sens_cb = p_sens_config->p_cb;
    ant_request_controller_init(&(p_profile->_cb.p_sens_cb->req_controller));

    p_profile->_cb.p_sens_cb->block_cnt    = 0;
    p_profile->_cb.p_sens_cb->pag_2_34    = 0;
    p_profile->_cb.p_sens_cb->pag_4_5    = 0;
    p_profile->_cb.p_sens_cb->message_counter = 0;
    p_profile->_cb.p_sens_cb->common_page_number = ANT_LEV_PAGE_80;

    return ant_lev_init(p_profile, p_channel_config);
}

static ant_lev_page_t next_page_number_get(ant_lev_profile_t * p_profile)
{
    ant_lev_sens_cb_t * p_lev_cb = p_profile->_cb.p_sens_cb;
    ant_lev_page_t page_number;

    if (ant_request_controller_pending_get(&(p_lev_cb->req_controller), (uint8_t *)&page_number))
    {
        // No implementation needed
    }
    else if (p_lev_cb->message_counter == (COMMON_DATA_INTERVAL)) // send either 80 or 81 common data page
    {
        page_number  = p_lev_cb->common_page_number;
        p_lev_cb->message_counter++;
    }
    else if (p_lev_cb->message_counter == (COMMON_DATA_INTERVAL + 1))  // send again the previous common page
    {
        page_number  = p_lev_cb->common_page_number;
        // next time will send the other common page: either 80 or 81
        p_lev_cb->common_page_number = (p_lev_cb->common_page_number == ANT_LEV_PAGE_80)
                                     ? ANT_LEV_PAGE_81 : ANT_LEV_PAGE_80;
        p_lev_cb->message_counter = 0;
    }
    else
    {
        switch (p_lev_cb->block_cnt) 
        {
            case 0:
                page_number = ANT_LEV_PAGE_1;
                break;

            case 1:
                if (p_lev_cb->pag_2_34)
                    page_number = ANT_LEV_PAGE_2;
                else
                    page_number = ANT_LEV_PAGE_34;

                p_lev_cb->pag_2_34 = !p_lev_cb->pag_2_34;
                break;

            case 2:
                page_number = ANT_LEV_PAGE_3;
                break;

            case 3:
                if (p_lev_cb->pag_4_5)
                    page_number = ANT_LEV_PAGE_4;
                else
                    page_number = ANT_LEV_PAGE_5;

                p_lev_cb->pag_4_5 = !p_lev_cb->pag_4_5;
                p_lev_cb->block_cnt = 0;
                break;

            default:
                page_number = ANT_LEV_PAGE_1;
                p_lev_cb->block_cnt = 0;
                break;
        }

        p_lev_cb->block_cnt++;
        p_lev_cb->message_counter++;
    }

    return page_number;
}

static void sens_message_encode(ant_lev_profile_t * p_profile, uint8_t * p_message_payload)
{
    ant_lev_message_layout_t * p_sdm_message_payload =
        (ant_lev_message_layout_t *)p_message_payload;

    p_sdm_message_payload->page_number = next_page_number_get(p_profile);

    switch (p_sdm_message_payload->page_number)
    {
        case ANT_LEV_PAGE_1:
            ant_lev_page_1_encode(p_sdm_message_payload->page_payload, &(p_profile->page_1),
                                  &(p_profile->common));
            break;

        case ANT_LEV_PAGE_2:
            ant_lev_page_2_encode(p_sdm_message_payload->page_payload, &(p_profile->page_2),
                        &(p_profile->common));
            break;

        case ANT_LEV_PAGE_3:
            ant_lev_page_3_encode(p_sdm_message_payload->page_payload, &(p_profile->page_3),
                        &(p_profile->common));
            break;

        case ANT_LEV_PAGE_4:
            ant_lev_page_4_encode(p_sdm_message_payload->page_payload, &(p_profile->page_4));
            break;

        case ANT_LEV_PAGE_5:
            ant_lev_page_5_encode(p_sdm_message_payload->page_payload, &(p_profile->page_5));
            break;

        case ANT_LEV_PAGE_34:
            ant_lev_page_34_encode(p_sdm_message_payload->page_payload, &(p_profile->page_34),
                        &(p_profile->common));
            break;            

        case ANT_LEV_PAGE_80:
            ant_common_page_80_encode(p_sdm_message_payload->page_payload, &(p_profile->page_80));
            break;

        case ANT_LEV_PAGE_81:
            ant_common_page_81_encode(p_sdm_message_payload->page_payload, &(p_profile->page_81));
            break;

        default:
            return;
    }

    p_profile->evt_handler(p_profile, (ant_lev_evt_t)p_sdm_message_payload->page_number);
}

void ant_lev_sens_evt_handler(ant_evt_t * p_ant_evt, void * p_context)
{
    ASSERT(p_context   != NULL);
    ASSERT(p_ant_evt != NULL);
    ant_lev_profile_t * p_profile = (ant_lev_profile_t *)p_context;

    if (p_ant_evt->channel == p_profile->channel_number)
    {
        uint32_t err_code;
        uint8_t p_message_payload[ANT_STANDARD_DATA_PAYLOAD_SIZE];
        ant_lev_sens_cb_t * p_lev_cb = p_profile->_cb.p_sens_cb;
        ant_request_controller_sens_evt_handler(&(p_lev_cb->req_controller), p_ant_evt);

        switch (p_ant_evt->event)
        {
            case EVENT_TX:
            case EVENT_TRANSFER_TX_FAILED:
            case EVENT_TRANSFER_TX_COMPLETED:
                sens_message_encode(p_profile, p_message_payload);
                if (ant_request_controller_ack_needed(&(p_lev_cb->req_controller)))
                {
                    err_code = sd_ant_acknowledge_message_tx(p_profile->channel_number,
                                                             sizeof(p_message_payload),
                                                             p_message_payload);
                }
                else
                {
                    err_code = sd_ant_broadcast_message_tx(p_profile->channel_number,
                                                           sizeof(p_message_payload),
                                                           p_message_payload);
                }
                APP_ERROR_CHECK(err_code);
                break;
            default:
                break;
        }
    }
}

ret_code_t ant_lev_sens_open(ant_lev_profile_t * p_profile)
{
    ASSERT(p_profile != NULL);

    // Fill tx buffer for the first frame
    uint32_t err_code;
    uint8_t  p_message_payload[ANT_STANDARD_DATA_PAYLOAD_SIZE];

    sens_message_encode(p_profile, p_message_payload);
    err_code =
        sd_ant_broadcast_message_tx(p_profile->channel_number,
                                    sizeof(p_message_payload),
                                    p_message_payload);
    APP_ERROR_CHECK(err_code);

    return sd_ant_channel_open(p_profile->channel_number);
}