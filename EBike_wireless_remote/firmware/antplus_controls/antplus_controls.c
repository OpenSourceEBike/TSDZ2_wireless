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
#include "antplus_controls_pages.h"
#include "antplus_controls.h"
#include "app_error.h"
#include "pins.h"
#include "app_button.h"
#include "nrf_pwr_mgmt.h"

extern void shutdown(void);

typedef struct
{
  antplus_controls_page_t page_number;
  uint8_t page_payload[7];
} antplus_controls_message_layout_t;

static ret_code_t antplus_controls_init(antplus_controls_profile_t *p_profile,
                                        ant_channel_config_t const *p_channel_config)
{
  p_profile->channel_number = p_channel_config->channel_number;

  p_profile->page_73 = DEFAULT_ANTPLUS_CONTROLS_PAGE73();
  p_profile->page_82 = DEFAULT_ANTPLUS_CONTROLS_PAGE82();
  p_profile->page_80 = DEFAULT_ANT_COMMON_page80();
  p_profile->page_81 = DEFAULT_ANT_COMMON_page81();

  return ant_channel_init(p_channel_config);
}

ret_code_t antplus_controls_sens_init(antplus_controls_profile_t *p_profile,
                                      ant_channel_config_t const *p_channel_config,
                                      antplus_controls_sens_config_t const *p_sens_config)
{
  ASSERT(p_profile != NULL);
  ASSERT(p_channel_config != NULL);
  ASSERT(p_sens_config != NULL);
  ASSERT(p_sens_config->p_cb != NULL);
  ASSERT(p_sens_config->evt_handler != NULL);

  p_profile->evt_handler = p_sens_config->evt_handler;
  p_profile->_cb.p_sens_cb = p_sens_config->p_cb;
  ant_request_controller_init(&(p_profile->_cb.p_sens_cb->req_controller));

  p_profile->_cb.p_sens_cb->common_page_number = ANTPLUS_CONTROLS_PAGE_80;

  return antplus_controls_init(p_profile, p_channel_config);
}
/*
static bool message_encode(antplus_controls_profile_t * p_profile, uint8_t * p_message_payload)
 {
  antplus_controls_message_layout_t * p_controls_message_payload =
    (antplus_controls_message_layout_t *)p_message_payload;
       antplus_controls_page_t page_number = 0;

   ant_request_controller_pending_get(&(p_profile->_cb.p_sens_cb->req_controller), (uint8_t *)&page_number);
   switch (page_number)
   {
     case ANTPLUS_CONTROLS_PAGE_82:
       antplus_controls_page_82_encode(p_controls_message_payload->page_payload, &(p_profile->page_82));
       break;

     case ANTPLUS_CONTROLS_PAGE_80:
       antplus_common_page_80_encode(p_controls_message_payload->page_payload, &(p_profile->page_80));
       break;

     case ANTPLUS_CONTROLS_PAGE_81:
       antplus_common_page_81_encode(p_controls_message_payload->page_payload, &(p_profile->page_81));
       break;

     default:
       break;
   }

   p_profile->evt_handler(p_profile, (antplus_controls_evt_t)p_controls_message_payload->page_number);

   if (page_number != 0)
     return true;
   else
     return false;
 }
*/
void buttons_send_pag73(antplus_controls_profile_t *p_profile, button_pins_t button, uint8_t pagectrl)
{
  ASSERT(p_profile != NULL);

  bool send_page = false;

  if (button == ENTER__PIN)
  {
    p_profile->page_73.utf8_character = pagectrl;
    send_page = true;
  }

  if (send_page)
  {
    send_page = false;

    static uint8_t p_message_payload[ANT_STANDARD_DATA_PAYLOAD_SIZE] = {
        ANTPLUS_CONTROLS_PAGE_73,
        0x01,
        0x00,
        0x0F,
        0x00,
        0x00,
        0x00,
        0x00};

    antplus_controls_message_layout_t *p_controls_message_payload =
        (antplus_controls_message_layout_t *)p_message_payload;

    antplus_controls_page_73_encode(p_controls_message_payload->page_payload,
                                    &(p_profile->page_73));

    uint32_t err_code;
    err_code = sd_ant_acknowledge_message_tx(p_profile->channel_number,
                                             sizeof(p_message_payload),
                                             p_message_payload);
    send_page = true;
    (void)err_code; // ignore
                    //the following code is needed to start a new ANT Rx search if the garmin is disconnected during use and restarted
    err_code = antplus_controls_sens_open(p_profile);
    //  APP_ERROR_CHECK(err_code);
  }
}

void antplus_controls_sens_evt_handler(ant_evt_t *p_ant_evt, void *p_context)
{
  ASSERT(p_context != NULL);
  ASSERT(p_ant_evt != NULL);
  static int8_t restart_count = 0;

  antplus_controls_profile_t *p_profile = (antplus_controls_profile_t *)p_context;

  if (p_ant_evt->channel == p_profile->channel_number)
  {

    switch (p_ant_evt->event)
    {

    case EVENT_TX:
    case EVENT_TRANSFER_TX_FAILED:
    case EVENT_TRANSFER_TX_COMPLETED:

      // nothing to do
      break;

    case EVENT_RX_SEARCH_TIMEOUT:
      ANT_Search_Stop();
      break;

    case EVENT_RX:
      /*
      if (p_ant_evt->message.ANT_MESSAGE_ucMesgID == MESG_BROADCAST_DATA_ID || p_ant_evt->message.ANT_MESSAGE_ucMesgID == MESG_ACKNOWLEDGED_DATA_ID || p_ant_evt->message.ANT_MESSAGE_ucMesgID == MESG_BURST_DATA_ID)
      {
         
         uint8_t p_message_payload[ANT_STANDARD_DATA_PAYLOAD_SIZE];
         antplus_controls_sens_cb_t *p_CONTROLS_cb = p_profile->_cb.p_sens_cb;

         bool buttons_page_sent = buttons_clock_pag73(p_profile);
      

       //only look to send for requested pages if no button pages sent
       //  if (buttons_page_sent == false) {
         ant_request_controller_sens_evt_handler(&(p_CONTROLS_cb->req_controller), p_ant_evt);

         bool page_encoded = message_encode(p_profile, p_message_payload);
         if (page_encoded)
         {
           if (ant_request_controller_ack_needed(&(p_CONTROLS_cb->req_controller)))
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
         }
         }
      }
      */
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

ret_code_t antplus_controls_sens_open(antplus_controls_profile_t *p_profile)
{
  ASSERT(p_profile != NULL);

  return sd_ant_channel_open(p_profile->channel_number);
}
