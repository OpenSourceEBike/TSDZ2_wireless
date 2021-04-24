/*
 * TSDZ2 EBike wireless remote firmware
 *
 * Copyright (C) Casainho, 2020
 * Copyright (C) Rananna, 2020
 *
 * Released under the GPL License, Version 3
 */

#include <stdio.h>
#include <stdint.h>
#include "app_timer.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_sdh.h"
#include "ble.h"
#include "ble_err.h"
#include "ble_hci.h"
#include "ble_srv_common.h"
#include "ble_advdata.h"
#include "ble_conn_params.h"
#include "ble_conn_state.h"
#include "peer_manager.h"
#include "peer_manager_handler.h"
#include "ble_advdata.h"
#include "ble_advertising.h"
#include "nrf_sdh.h"
#include "nrf_sdh_ble.h"
#include "nrf_ble_gatt.h"
#include "nrf_ble_qwr.h"
#include "ble_services.h"
#include "nrf_sdh_ant.h"
#include "ant_key_manager.h"
#include "ant_lev.h"
#include "pins.h"
#include "app_util_platform.h"
#include "ant_interface.h"
#include "nrf_delay.h"
#include "fds.h"

#include "app_button.h"
#include "nrf_drv_clock.h"
#include "sdk_config.h"
#include "ant_state_indicator.h"
#include "bsp_btn_ant.h"
#include "antplus_controls.h"
#include "eeprom.h"
#include "nrf_sdh_soc.h"
#include "nrf_power.h"
#include "nrf_bootloader_info.h"
#include "nrf_gpio.h"
#include "bsp.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_drv_gpiote.h"
#include "common.h"
#include "ledalert.h"
#include "nordic_common.h"

#include "ant_search_config.h"
#include <math.h>
#define WAIT_TIME 1000 // wait 3 seconds before a reset
bool config_press = false;
//motor state control variables
uint8_t motor_init_state = 0;
uint8_t motor_error_state;
uint8_t motor_soc_state;
bool motor_display_soc = false;
bool display_assist = false;
uint8_t walk_mode = 0;
uint8_t light_mode = 0;
bool disp_config_flag = false;

bool searching_flag = false;

#define BUTTON_DETECTION_DELAY APP_TIMER_TICKS(10)           /**< Delay from a GPIOTE event until a button is reported as pushed (in number of timer ticks). */
#define BUTTON_PRESS_TIMEOUT APP_TIMER_TICKS(60 * 60 * 1000) // 1h to enter low power mode
#define BUTTON_LONG_PRESS_TIMEOUT APP_TIMER_TICKS(1000)      // 1 seconds for long press
#define BUTTON_CONFIG_PRESS_TIMEOUT APP_TIMER_TICKS(5000)    // 5 seconds TO ENTER CONFIG MODE

#define ANT_Search_TIMEOUT APP_TIMER_TICKS(600) // 600 ms for Ant Search check
#define DEVICE_NAME "TSDZ2_remote"              /**< Name of device. Will be included in the advertising data. */

#define APP_BLE_CONN_CFG_TAG 1 /**< A tag identifying the SoftDevice BLE configuration. */

#define APP_ADV_INTERVAL 64                                    /**< The advertising interval (in units of 0.625 ms; this value corresponds to 40 ms). */
#define APP_ADV_DURATION BLE_GAP_ADV_TIMEOUT_GENERAL_UNLIMITED /**< The advertising time-out (in units of seconds). When set to 0, we will never time out. */

#define MIN_CONN_INTERVAL MSEC_TO_UNITS(100, UNIT_1_25_MS) /**< Minimum acceptable connection interval (0.5 seconds). */
#define MAX_CONN_INTERVAL MSEC_TO_UNITS(200, UNIT_1_25_MS) /**< Maximum acceptable connection interval (1 second). */
#define SLAVE_LATENCY 0                                    /**< Slave latency. */
#define CONN_SUP_TIMEOUT MSEC_TO_UNITS(4000, UNIT_10_MS)   /**< Connection supervisory time-out (4 seconds). */

#define FIRST_CONN_PARAMS_UPDATE_DELAY APP_TIMER_TICKS(20000) /**< Time from initiating event (connect or start of notification) to first time sd_ble_gap_conn_param_update is called (15 seconds). */
#define NEXT_CONN_PARAMS_UPDATE_DELAY APP_TIMER_TICKS(5000)   /**< Time between each call to sd_ble_gap_conn_param_update after the first call (5 seconds). */
#define MAX_CONN_PARAMS_UPDATE_COUNT 3                        /**< Number of attempts before giving up the connection parameter negotiation. */

#define SEC_PARAM_BOND 1                               /**< Perform bonding. */
#define SEC_PARAM_MITM 0                               /**< Man In The Middle protection not required. */
#define SEC_PARAM_LESC 0                               /**< LE Secure Connections not enabled. */
#define SEC_PARAM_KEYPRESS 0                           /**< Keypress notifications not enabled. */
#define SEC_PARAM_IO_CAPABILITIES BLE_GAP_IO_CAPS_NONE /**< No I/O capabilities. */
#define SEC_PARAM_OOB 0                                /**< Out Of Band data not available. */
#define SEC_PARAM_MIN_KEY_SIZE 7                       /**< Minimum encryption key size. */
#define SEC_PARAM_MAX_KEY_SIZE 16                      /**< Maximum encryption key size. */

#define APP_FEATURE_NOT_SUPPORTED BLE_GATT_STATUS_ATTERR_APP_BEGIN + 2 /**< Reply when unsupported features are requested. */

#define CONTROLS_HW_REVISION 2
#define LEV_HW_REVISION 1

#define CONTROLS_MANUFACTURER_ID 255
#define LEV_MANUFACTURER_ID 254

#define CONTROLS_MODEL_NUMBER 2
#define LEV_MODEL_NUMBER 1

#define CONTROLS_SW_REVISION_MAJOR 2
#define LEV_SW_REVISION_MAJOR 1

#define CONTROLS_SW_REVISION_MINOR 2
#define LEV_SW_REVISION_MINOR 1

#define CONTROLS_SERIAL_NUMBER 3241
#define LEV_SERIAL_NUMBER 1234

#define CONTROLS_CHANNEL_NUM 1 //see: NRF_SDH_ANT_TOTAL_CHANNELS_ALLOCATED in sdk_config.sys
#define LEV_CHANNEL_NUM 0      //LEV on chan 0,CONTROL on chan 1

#define ANT_LEV_ANT_OBSERVER_PRIO 1
#define ANT_CONTROLS_ANT_OBSERVER_PRIO 1

#define CONTROLS_CHAN_ID_TRANS_TYPE 0 // slave wildcard match
#define LEV_CHAN_ID_TRANS_TYPE 0

#define CONTROLS_CHAN_ID_DEV_NUM 0 // wildcard match to any master
#define LEV_CHAN_ID_DEV_NUM 0

#define ANTPLUS_NETWORK_NUM 0

// @snippet [ANT LEV RX Instance]
void ant_lev_evt_handler(ant_lev_profile_t *p_profile, ant_lev_evt_t event);
void antplus_controls_evt_handler(antplus_controls_profile_t *p_profile, antplus_controls_evt_t event);

LEV_DISP_CHANNEL_CONFIG_DEF(m_ant_lev,
                            LEV_CHANNEL_NUM,
                            LEV_CHAN_ID_TRANS_TYPE,
                            LEV_CHAN_ID_DEV_NUM,
                            ANTPLUS_NETWORK_NUM,
                            LEV_MSG_PERIOD_4Hz);

CONTROLS_SENS_CHANNEL_CONFIG_DEF(m_antplus_controls,
                                 CONTROLS_CHANNEL_NUM,
                                 CONTROLS_CHAN_ID_TRANS_TYPE,
                                 CONTROLS_CHAN_ID_DEV_NUM,
                                 ANTPLUS_NETWORK_NUM,
                                 CONTROLS_MSG_PERIOD_4Hz);

CONTROLS_SENS_PROFILE_CONFIG_DEF(m_antplus_controls,
                                 antplus_controls_evt_handler);

static ant_lev_profile_t m_ant_lev;
static antplus_controls_profile_t m_antplus_controls;

NRF_SDH_ANT_OBSERVER(m_ant_observer, ANT_LEV_ANT_OBSERVER_PRIO, ant_lev_disp_evt_handler, &m_ant_lev);
NRF_SDH_ANT_OBSERVER(m_ant_observer_control, ANT_CONTROLS_ANT_OBSERVER_PRIO, antplus_controls_sens_evt_handler, &m_antplus_controls);
uint8_t configuration_flag = 0;     // flag to determine configutaion mode 1 config mode/ 0 normal mode
bool enable_configuration = false;  //used to turn on config mode
bool disable_configuration = false; //used to turn off config mode

uint8_t ebike = 1;  //ebike control as default                                                //ANT LEV ebike as a default
uint8_t garmin = 0; //no garmin computer as a default
uint8_t brake = 0;  //BRAKE FLAG
bool m_button_long_press = false;
bool shutdown_flag = false;
bool brightness_flag = false;
NRF_BLE_GATT_DEF(m_gatt);           /**< GATT module instance. */
NRF_BLE_QWR_DEF(m_qwr);             /**< Context for the Queued Write module.*/
BLE_ADVERTISING_DEF(m_advertising); /**< Advertising module instance. */
BLE_ANT_ID_DEF(m_ble_ant_id_service);
//test flash write completed
static uint16_t m_conn_handle = BLE_CONN_HANDLE_INVALID; /**< Handle of the current connection. */
//static uint8_t m_adv_handle = BLE_GAP_ADV_SET_HANDLE_NOT_SET;           /**< Advertising handle used to identify an advertising set. */
//static uint8_t m_enc_advdata[BLE_GAP_ADV_SET_DATA_SIZE_MAX];            /**< Buffer for storing an encoded advertising set. */
//static uint8_t m_enc_scan_response_data[BLE_GAP_ADV_SET_DATA_SIZE_MAX]; /**< Buffer for storing an encoded scan data. */

uint8_t old_ant_device_id = 0; // initially in pairing mode
uint8_t new_ant_device_id = 0; // used to check for change of ant id

/**@brief Struct that contains pointers to the encoded advertising data. */
/*
static ble_gap_adv_data_t m_adv_data =
    {
        .adv_data =
            {
                .p_data = m_enc_advdata,
                .len = BLE_GAP_ADV_SET_DATA_SIZE_MAX},
        .scan_rsp_data =
            {
                .p_data = m_enc_scan_response_data,
                .len = BLE_GAP_ADV_SET_DATA_SIZE_MAX}};
*/

APP_TIMER_DEF(ANT_Search_timer);
APP_TIMER_DEF(multiple_click_timer);
#define CLICK_INTERVAL APP_TIMER_TICKS(250) // count # of button clicks in 0.25 seconds
uint8_t button_clicks = 0;
bool enter_pin_flag = false;
bool standby_pin_flag = false;

static void multiple_click_timeout(void *p_context)
{
  UNUSED_PARAMETER(p_context);
  //process button clicks

  if (button_clicks >= 2) //more than one click
  {
    if ((standby_pin_flag) && (motor_init_state == 0))
      shutdown_flag = true; // set flag for low power

    if (enter_pin_flag)
      brightness_flag = true; // change brightness
  }
  //reset button clicks and flags
  enter_pin_flag = false;
  standby_pin_flag = false;
  button_clicks = 0;
}

void check_motor_init()
{
  static bool soc_disp = true;

  static bool motor_on;
  //static bool key_disp = false;

  //display SOC if standby key pressed and motor is on
  // if (motor_display_soc && motor_soc_state && key_disp && (motor_init_state == 1)) // display soc when STANDBY Key is pressed
  if (motor_display_soc && motor_init_state == 1) // display soc when STANDBY Key is pressed
  {
    disp_soc(motor_soc_state);
    motor_display_soc = false;
  }

  switch (motor_init_state)
  {
  case 0: //motor off
    if (motor_on)
    {
      {
        nrf_lp_delay_ms(500);
        led_sequence_play_next(LED_EVENT_MOTOR_OFF);
        motor_on = false;
        // disp_soc(motor_soc_state);
      }
    }
    //key_disp = false;

    break;
  case 1: //motor on

    if (soc_disp && motor_soc_state) //display if soc>0
    {
      ////indicate the motor SOC when motor turns on
      if (!searching_flag) //needed if you have a garmin bike computer
      {
        led_sequence_play_next(LED_EVENT_MOTOR_ON_COMPLETE);
      }
      motor_on = true;
      soc_disp = false;
      // key_disp = true;
    }
    break;
  case 2: //motor initializing
    nrf_lp_delay_ms(500);
    led_sequence_play(LED_EVENT_MOTOR_ON_WAIT);
    soc_disp = true; //show the soc when motor turns on

    break;
  case 3: //used to signal turning  motor on/off
    // do nothing

    break;
  }
  //check for motor errors

  switch (motor_error_state)
  {
  case 1: //MOTOR_INIT_ERROR_ALIVE:

    break;
  case 2: //MOTOR_INIT_ERROR_GET_FIRMWARE_VERSION
  case 3: //MOTOR_INIT_ERROR_FIRMWARE_VERSION
    break;
  case 4: //MOTOR_INIT_ERROR_SET_CONFIGURATIONS

    break;
  }
}
static ble_uuid_t m_adv_uuids[] = /**< Universally unique service identifiers. */
    {
        {BLE_UUID_HEALTH_THERMOMETER_SERVICE, BLE_UUID_TYPE_BLE},
        {BLE_UUID_BATTERY_SERVICE, BLE_UUID_TYPE_BLE},
        {BLE_UUID_DEVICE_INFORMATION_SERVICE, BLE_UUID_TYPE_BLE}};

/**@brief Clear bond information from persistent storage.
 */

//uint32_t err_code=0;
volatile uint32_t main_ticks;

static void delete_bonds(void)
{
  ret_code_t err_code;

  NRF_LOG_INFO("Erase bonds!");

  err_code = pm_peers_delete();
  APP_ERROR_CHECK(err_code);
}

/**@brief Function for starting advertising.
 */
static void advertising_start(bool erase_bonds)
{
  if (erase_bonds == true)
  {
    delete_bonds();
    // Advertising is started by PM_EVT_PEERS_DELETE_SUCCEEDED event.
  }
  else
  {
    uint32_t err_code = ble_advertising_start(&m_advertising, BLE_ADV_MODE_FAST);
    APP_ERROR_CHECK(err_code);
  }
}

/**@brief Function for handling BLE events.
 *
 * @param[in]   p_ble_evt   Bluetooth stack event.
 * @param[in]   p_context   Unused.
 */
static void ble_evt_handler(ble_evt_t const *p_ble_evt, void *p_context)
{
  ret_code_t err_code;

  switch (p_ble_evt->header.evt_id)
  {
  case BLE_GAP_EVT_CONNECTED:
    NRF_LOG_INFO("Connected");
    m_conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
    err_code = nrf_ble_qwr_conn_handle_assign(&m_qwr, m_conn_handle);
    APP_ERROR_CHECK(err_code);
    break;

  case BLE_GAP_EVT_DISCONNECTED:
    NRF_LOG_INFO("Disconnected");
    m_conn_handle = BLE_CONN_HANDLE_INVALID;
    advertising_start(false);
    break;

  case BLE_GAP_EVT_SEC_PARAMS_REQUEST:
    break;

  case BLE_GAP_EVT_PHY_UPDATE_REQUEST:
  {
    NRF_LOG_DEBUG("PHY update request.");
    ble_gap_phys_t const phys =
        {
            .rx_phys = BLE_GAP_PHY_AUTO,
            .tx_phys = BLE_GAP_PHY_AUTO,
        };
    err_code = sd_ble_gap_phy_update(p_ble_evt->evt.gap_evt.conn_handle, &phys);
    APP_ERROR_CHECK(err_code);
  }
  break;

  case BLE_GATTS_EVT_SYS_ATTR_MISSING:
    // No system attributes have been stored.
    err_code = sd_ble_gatts_sys_attr_set(m_conn_handle, NULL, 0, 0);
    APP_ERROR_CHECK(err_code);
    break;

  case BLE_GATTC_EVT_TIMEOUT:
    // Disconnect on GATT Client timeout event.
    NRF_LOG_DEBUG("GATT Client Timeout.");
    err_code = sd_ble_gap_disconnect(p_ble_evt->evt.gattc_evt.conn_handle,
                                     BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
    APP_ERROR_CHECK(err_code);
    break;

  case BLE_GATTS_EVT_TIMEOUT:
    // Disconnect on GATT Server timeout event.
    NRF_LOG_DEBUG("GATT Server Timeout.");
    err_code = sd_ble_gap_disconnect(p_ble_evt->evt.gatts_evt.conn_handle,
                                     BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
    APP_ERROR_CHECK(err_code);
    break;

  case BLE_GATTS_EVT_RW_AUTHORIZE_REQUEST:
  {
    ble_gatts_evt_rw_authorize_request_t req;
    ble_gatts_rw_authorize_reply_params_t auth_reply;

    req = p_ble_evt->evt.gatts_evt.params.authorize_request;

    if (req.type != BLE_GATTS_AUTHORIZE_TYPE_INVALID)
    {
      if ((req.request.write.op == BLE_GATTS_OP_PREP_WRITE_REQ) ||
          (req.request.write.op == BLE_GATTS_OP_EXEC_WRITE_REQ_NOW) ||
          (req.request.write.op == BLE_GATTS_OP_EXEC_WRITE_REQ_CANCEL))
      {
        if (req.type == BLE_GATTS_AUTHORIZE_TYPE_WRITE)
        {
          auth_reply.type = BLE_GATTS_AUTHORIZE_TYPE_WRITE;
        }
        else
        {
          auth_reply.type = BLE_GATTS_AUTHORIZE_TYPE_READ;
        }
        auth_reply.params.write.gatt_status = APP_FEATURE_NOT_SUPPORTED;
        sd_ble_gatts_rw_authorize_reply(p_ble_evt->evt.gatts_evt.conn_handle, &auth_reply);
      }
    }
  }
  break; // BLE_GATTS_EVT_RW_AUTHORIZE_REQUEST

  default:
    // No implementation needed.
    break;
  }
}

//APP_TIMER_DEF(m_lev_send);
APP_TIMER_DEF(m_timer_button_press_timeout);
APP_TIMER_DEF(m_timer_button_long_press_timeout);
APP_TIMER_DEF(m_timer_button_config_press_timeout);

//APP_TIMER_DEF(m_antplus_controls_send);

button_pins_t m_buttons_wait_to_send = 0;
bool m_timer_buttons_send_running = false;

//set default  old ant ID for reset;

APP_TIMER_DEF(bluetooth_timer);

#define BLUETOOTH_TIMEOUT APP_TIMER_TICKS(1000 * 60 * 5) //turn off bluetooth after 5 min

void shutdown(void);

uint16_t cnt_1;

void antplus_controls_evt_handler(antplus_controls_profile_t *p_profile, antplus_controls_evt_t event)
{
  nrf_pwr_mgmt_feed();

  switch (event)
  {
  case ANTPLUS_CONTROLS_PAGE_73_UPDATED:
    break;

  default:
    break;
  }
}

void ant_lev_evt_handler(ant_lev_profile_t *p_profile, ant_lev_evt_t event)
{
  nrf_pwr_mgmt_feed();
  // get motor state variables
  motor_init_state = p_profile->common.gear_state & 0x03;
  motor_error_state = (p_profile->common.gear_state) >> 2;

  switch (event)
  {
  case ANT_LEV_PAGE_1_UPDATED:
      //  p_profile->page_16.travel_mode = p_profile->common.travel_mode_state;
      ;
    break;

  case ANT_LEV_PAGE_2_UPDATED:
    break;

  case ANT_LEV_PAGE_3_UPDATED:
    motor_soc_state = round(p_profile->page_3.battery_soc / 10); //0 to 10
    break;

  case ANT_LEV_PAGE_4_UPDATED:
    break;

  case ANT_LEV_PAGE_5_UPDATED:
    break;

  case ANT_LEV_PAGE_34_UPDATED:

    break;

  case ANT_LEV_PAGE_16_UPDATED:

    break;

  case ANT_LEV_PAGE_80_UPDATED:

    break;

  case ANT_LEV_PAGE_81_UPDATED:

    break;

  case ANT_LEV_PAGE_REQUEST_SUCCESS:

    break;

  case ANT_LEV_PAGE_REQUEST_FAILED:
    break;

  default:
    break;
  }
}

void wait_and_reset(void)
{
  nrf_lp_delay_ms(WAIT_TIME);
  sd_nvic_SystemReset(); // reset and start again
}
void ANT_Search_Stop(void) //ant search has timed out without finding a device
{
  ret_code_t err_code;
  err_code = app_timer_stop(ANT_Search_timer);
  APP_ERROR_CHECK(err_code);
  searching_flag = false;
}
void ANT_Search_Start(void)
{
  ret_code_t err_code;
  err_code = app_timer_start(ANT_Search_timer, ANT_Search_TIMEOUT, NULL);
  searching_flag = true;
  APP_ERROR_CHECK(err_code);
}
static void ANT_Search_timeout(void *p_context) //check every 400 ms
{
  UNUSED_PARAMETER(p_context);
  // first see if ANT pairing is completed
  uint8_t LEV_Status;
  uint8_t CTRL_Status;
  ret_code_t err_code;
  static uint8_t ui8_cnt_ant_search_timeout = 0;

  // keep a counter for the number of ANT search timeouts
  ui8_cnt_ant_search_timeout++;
  if (ui8_cnt_ant_search_timeout > 50) // 600ms * 50 = 30 seconds
  {
    // if is not connected, shutdown
    shutdown_flag = true;
    return;
  }
  if (searching_flag)
    led_sequence_play(LED_EVENT_ANT_DISCONNECT);
  err_code = sd_ant_channel_status_get(LEV_CHANNEL_NUM, &LEV_Status);
  APP_ERROR_CHECK(err_code);
  err_code = sd_ant_channel_status_get(CONTROLS_CHANNEL_NUM, &CTRL_Status);
  APP_ERROR_CHECK(err_code);

  if (ebike && garmin) //both ant profiles are active
  {
    if (LEV_Status != (uint8_t)STATUS_SEARCHING_CHANNEL && CTRL_Status != (uint8_t)STATUS_SEARCHING_CHANNEL) // both device are paired
    {
      APP_ERROR_CHECK(err_code);
      ui8_cnt_ant_search_timeout = 0;
      err_code = app_timer_stop(ANT_Search_timer);
      APP_ERROR_CHECK(err_code);

      led_sequence_play_next(LED_EVENT_ANT_CONNECT);

      searching_flag = false;
    }

    return;
  }

  if (ebike)
  {
    if (LEV_Status != (uint8_t)STATUS_SEARCHING_CHANNEL)
    {

      ui8_cnt_ant_search_timeout = 0;
      led_sequence_play_next(LED_EVENT_ANT_CONNECT);
      err_code = app_timer_stop(ANT_Search_timer);

      searching_flag = false;
    }
    return;
  }
  if (garmin)
  {
    if (CTRL_Status != (uint8_t)STATUS_SEARCHING_CHANNEL)
    {
      ui8_cnt_ant_search_timeout = 0;

      led_sequence_play_next(LED_EVENT_ANT_CONNECT);
      err_code = app_timer_stop(ANT_Search_timer);

      searching_flag = false;
    }
    return;
  }
}

static void bluetooth_timer_timeout(void *p_context)
{
  UNUSED_PARAMETER(p_context);
  disable_configuration = true;
}
static void timer_button_config_press_timeout_handler(void *p_context)
{
  UNUSED_PARAMETER(p_context);

  config_press = true;
  // enter configuration mode
  if (configuration_flag)
  {
    disable_configuration = true;
    enable_configuration = false;
  }
  else
  {
    enable_configuration = true;
    disable_configuration = false;
  }
}
static void timer_button_press_timeout_handler(void *p_context)
{
  UNUSED_PARAMETER(p_context);

  // enter ultra low power mode

  shutdown();
}

static void timer_button_long_press_timeout_handler(void *p_context)
{
  UNUSED_PARAMETER(p_context);

  ret_code_t err_code;
  //stop the long press timer
  err_code = app_timer_stop(m_timer_button_long_press_timeout); //stop the long press timer
  APP_ERROR_CHECK(err_code);
  if ((nrf_gpio_pin_read(ENTER__PIN) != 0) && (nrf_gpio_pin_read(MINUS__PIN) != 0) && (nrf_gpio_pin_read(STANDBY__PIN) != 0)) //if none of these are pressed
  {
    led_sequence_play(LED_EVENT_SHORT_GREEN);
  }
  if (configuration_flag)
  {

    //CONFIG long press button options

    if (nrf_gpio_pin_read(STANDBY__PIN) == 0)
    {

      new_ant_device_id = 0x99;
    }
    if (nrf_gpio_pin_read(PLUS__PIN) == 0)
    {
      new_ant_device_id = 0x90;
    }
    if (nrf_gpio_pin_read(MINUS__PIN) == 0)
    {
      new_ant_device_id = 0x91;
    }
  }
  else
  {
    //pageup/pagedown
    if ((nrf_gpio_pin_read(ENTER__PIN) == 0) && garmin && !brightness_flag && !enter_pin_flag)
    {

      buttons_send_pag73(&m_antplus_controls, ENTER__PIN, 0);

      led_sequence_play(LED_EVENT_GARMIN_PAGEUP);
    }

    if ((nrf_gpio_pin_read(MINUS__PIN) == 0) && (motor_init_state == 1))
    {
      // start walk mode
      walk_mode = 55; //set walk mode flag to allow button release to work
                      //start blinking blue led
                      //slow flash
      m_button_long_press = false;
      buttons_send_page16(&m_ant_lev, walk_mode, m_button_long_press);
      //check for walk mode
      led_sequence_play_next_until(LED_EVENT_WALK_ASSIST_ACTIVE);
    }
    else
    {
      if (nrf_gpio_pin_read(STANDBY__PIN) != 0)
        led_sequence_play_next(LED_EVENT_BUTTON_INACTIVE);
    }
    if (nrf_gpio_pin_read(PLUS__PIN) == 0)
    {
      // toggle lights on/off
      light_mode = 54;
      m_button_long_press = false;
      //54 if flag to send light mode command
      buttons_send_page16(&m_ant_lev, light_mode, m_button_long_press);
    }

    if (nrf_gpio_pin_read(STANDBY__PIN) == 0) //if motor on/off requested

    {
      //turn motor power on/off
      m_button_long_press = true;
      buttons_send_page16(&m_ant_lev, STANDBY__PIN, m_button_long_press);
    }
  }
  m_button_long_press = true; //needed for app_release long press actions
}

static void button_event_handler(uint8_t pin_no, uint8_t button_action)
{
  button_pins_t button_pin = (button_pins_t)pin_no;
  ret_code_t err_code;
  if (configuration_flag) //configuration mode
  {
    //configuration buttons
    switch (button_action)
    {
    case APP_BUTTON_PUSH:
      m_button_long_press = false;                                                                    //button pushed
      err_code = app_timer_start(m_timer_button_long_press_timeout, BUTTON_LONG_PRESS_TIMEOUT, NULL); //start the long press timer
      APP_ERROR_CHECK(err_code);
      if (button_pin == STANDBY__PIN)
      {
        if (motor_init_state == 0) //motor is off
        {
          led_sequence_play_next(LED_EVENT_BUTTON_INACTIVE);
        }
      }
      if (button_pin == ENTER__PIN)
      {

        //start the config timer
        err_code = app_timer_start(m_timer_button_config_press_timeout, BUTTON_CONFIG_PRESS_TIMEOUT, NULL); //start the long press timer
        APP_ERROR_CHECK(err_code);
      }
      break;
    case APP_BUTTON_RELEASE:                                        //process the button actions
      err_code = app_timer_stop(m_timer_button_long_press_timeout); //stop the long press timer
      APP_ERROR_CHECK(err_code);
      if (button_pin == STANDBY__PIN)
      {
        //not assigned
        //led_sequence_play_next(LED_EVENT_SHORT_RED);
      }
      if (button_pin == PLUS__PIN)
      {

        new_ant_device_id = 0x92;
      }
      if (button_pin == MINUS__PIN)
      {

        new_ant_device_id = 0x93;
      }
      if ((button_pin == ENTER__PIN) && (!config_press))
      {
        err_code = app_timer_stop(m_timer_button_config_press_timeout); //stop the config  timer
        APP_ERROR_CHECK(err_code);
        disp_config_flag = true;
      }
      break;
    }
  }
  else
  {
    switch (button_action)
    {
    case APP_BUTTON_RELEASE: //process the button actions
      if (walk_mode)
      {
        //cancel walk_mode
        led_sequence_cancel_play_until();
        m_button_long_press = true;
        buttons_send_page16(&m_ant_lev, walk_mode, m_button_long_press);
        walk_mode = 0;
      }
      if (button_pin == ENTER__PIN)
      {
        err_code = app_timer_stop(m_timer_button_config_press_timeout); //stop the config  timer
        APP_ERROR_CHECK(err_code);
      }
      if ((button_pin == MINUS__PIN) && !light_mode)

      {

        if ((ebike) && (!m_button_long_press))
        {
          if (motor_init_state == 1)
          {
            buttons_send_page16(&m_ant_lev, button_pin, m_button_long_press);
            if (m_ant_lev.page_16.travel_mode == 0) //at limits
            {
              led_sequence_play_next(LED_EVENT_ASSIST_LIMITS_REACHED);
            }
            else
            {
              led_sequence_play_next(LED_EVENT_ASSIST_LEVEL_DECREASE);
            }
          }
          else
          {
            led_sequence_play_next(LED_EVENT_BUTTON_INACTIVE);
          }
        }
      }
      else if ((button_pin == BRAKE__PIN) && (motor_init_state == 1))
      {

        led_sequence_play_now(LED_EVENT_BRAKE_OFF);

        m_button_long_press = true;
        buttons_send_page16(&m_ant_lev, BRAKE__PIN, m_button_long_press); //critical command - send twice
      }
      else if ((button_pin == STANDBY__PIN) && (!m_button_long_press))
      { //display the battery SOC
        if (motor_init_state == 1)
        {
          motor_display_soc = true; //display charge state
        }
        else
        {
          led_sequence_play(LED_EVENT_BUTTON_INACTIVE); //inactive
          motor_display_soc = false;              //flag needed due to interrupt priority
        }
      }
      else if (button_pin == PLUS__PIN)
      {
        if ((ebike) && (!m_button_long_press))
        {
          if (motor_init_state == 1)
          {
            buttons_send_page16(&m_ant_lev, button_pin, m_button_long_press);
            if (m_ant_lev.page_16.travel_mode == 56) //at limits
            {
              led_sequence_play_next(LED_EVENT_ASSIST_LIMITS_REACHED);
            }
            else
            {
              led_sequence_play_next(LED_EVENT_ASSIST_LEVEL_INCREASE);
            }
          }
          else
          {
            led_sequence_play_next(LED_EVENT_BUTTON_INACTIVE);
          }
        }
      }
      else if ((button_pin == ENTER__PIN) && (!m_button_long_press) && (!brightness_flag))
      //pageup on bike computer
      {
        if (garmin)
        {
          buttons_send_pag73(&m_antplus_controls, button_pin, 1);
          led_sequence_play(LED_EVENT_GARMIN_PAGEUP);
        }
        else
        {
          //garmin not activated, flash red led
          led_sequence_play(LED_EVENT_BUTTON_INACTIVE);
        }
      }

      m_button_long_press = false; //reset the long press timer

      //reset the button timers
      err_code = app_timer_stop(m_timer_button_press_timeout); //1hr timeout for low power
      APP_ERROR_CHECK(err_code);
      err_code = app_timer_start(m_timer_button_press_timeout, BUTTON_PRESS_TIMEOUT, NULL);
      APP_ERROR_CHECK(err_code);
      err_code = app_timer_stop(m_timer_button_long_press_timeout); //stop the long press timer
      APP_ERROR_CHECK(err_code);
      light_mode = 0;
      break;

    case APP_BUTTON_PUSH:
      m_button_long_press = false;
      //stART THE MULTIPLE CLICK TIMER on first push
      if (button_clicks == 0)
      {
        err_code = app_timer_start(multiple_click_timer, CLICK_INTERVAL, NULL);
        APP_ERROR_CHECK(err_code);
      }
      //increment the button clicks and set the button flags
      button_clicks++; //increment the number of clicks
      if (button_pin == ENTER__PIN)
        enter_pin_flag = true;
      if (button_pin == STANDBY__PIN)
        standby_pin_flag = true;

      if (button_pin == ENTER__PIN)
      {
        //start the config timer
        err_code = app_timer_start(m_timer_button_config_press_timeout, BUTTON_CONFIG_PRESS_TIMEOUT, NULL); //start the long press timer
        APP_ERROR_CHECK(err_code);
      }                                                             //button pushed
      err_code = app_timer_stop(m_timer_button_long_press_timeout); //stop the long press timer
      APP_ERROR_CHECK(err_code);
      //send the brake signal on a button push for BRAKE__PIN
      if ((button_pin == BRAKE__PIN) && (motor_init_state == 1)) //motor is on
      {
        //set the brake flag in the rear gearing to signal that the brake has been pressed
        buttons_send_page16(&m_ant_lev, BRAKE__PIN, m_button_long_press);
        led_sequence_play_next(LED_EVENT_BRAKE_ON);
      }
      else
      {
        err_code = app_timer_start(m_timer_button_long_press_timeout, BUTTON_LONG_PRESS_TIMEOUT, NULL); //start the long press timer
        APP_ERROR_CHECK(err_code);
      }
      break;
    }
  }
}

void buttons_init(void)
{
  ret_code_t err_code;

  // the array must be static because a pointer to it will be saved in the button handler module
  static app_button_cfg_t buttons[5] =
      {
          {(uint8_t)PLUS__PIN, APP_BUTTON_ACTIVE_LOW, GPIO_PIN_CNF_PULL_Pullup, button_event_handler},
          {(uint8_t)MINUS__PIN, APP_BUTTON_ACTIVE_LOW, GPIO_PIN_CNF_PULL_Pullup, button_event_handler},
          {(uint8_t)ENTER__PIN, APP_BUTTON_ACTIVE_LOW, GPIO_PIN_CNF_PULL_Pullup, button_event_handler},
          {(uint8_t)STANDBY__PIN, APP_BUTTON_ACTIVE_LOW, GPIO_PIN_CNF_PULL_Pullup, button_event_handler},
          {(uint8_t)BRAKE__PIN, APP_BUTTON_ACTIVE_LOW, GPIO_PIN_CNF_PULL_Pullup, button_event_handler}};

  err_code = app_button_init(buttons, ARRAY_SIZE(buttons), BUTTON_DETECTION_DELAY);
  // this will enable wakeup from ultra low power mode (any button press)
  nrf_gpio_cfg_sense_input(PLUS__PIN, GPIO_PIN_CNF_PULL_Pullup, GPIO_PIN_CNF_SENSE_Low);
  nrf_gpio_cfg_sense_input(MINUS__PIN, GPIO_PIN_CNF_PULL_Pullup, GPIO_PIN_CNF_SENSE_Low);
  nrf_gpio_cfg_sense_input(ENTER__PIN, GPIO_PIN_CNF_PULL_Pullup, GPIO_PIN_CNF_SENSE_Low);
  nrf_gpio_cfg_sense_input(STANDBY__PIN, GPIO_PIN_CNF_PULL_Pullup, GPIO_PIN_CNF_SENSE_Low);
  nrf_gpio_cfg_sense_input(BRAKE__PIN, GPIO_PIN_CNF_PULL_Pullup, GPIO_PIN_CNF_SENSE_Low);
  nrf_drv_gpiote_in_event_disable(BUTTON_1);

  if (err_code == NRF_SUCCESS)
  {
    err_code = app_button_enable();
  }
  APP_ERROR_CHECK(err_code);

  err_code = app_timer_create(&m_timer_button_press_timeout,
                              APP_TIMER_MODE_SINGLE_SHOT,
                              timer_button_press_timeout_handler);

  APP_ERROR_CHECK(err_code);
  err_code = app_timer_create(&m_timer_button_long_press_timeout,
                              APP_TIMER_MODE_SINGLE_SHOT,
                              timer_button_long_press_timeout_handler);

  APP_ERROR_CHECK(err_code);
  err_code = app_timer_create(&m_timer_button_config_press_timeout,
                              APP_TIMER_MODE_SINGLE_SHOT,
                              timer_button_config_press_timeout_handler);

  APP_ERROR_CHECK(err_code);

  err_code = app_timer_create(&multiple_click_timer, APP_TIMER_MODE_SINGLE_SHOT, multiple_click_timeout);
  APP_ERROR_CHECK(err_code);

  err_code = app_timer_start(m_timer_button_press_timeout, BUTTON_PRESS_TIMEOUT, NULL);
  APP_ERROR_CHECK(err_code);

  err_code = app_timer_stop(m_timer_button_long_press_timeout); //stop the long press timer
  APP_ERROR_CHECK(err_code);
}
void shutdown(void)
{
  led_sequence_play_next(LED_SEQUENCE_SHORT_GREEN);
  led_sequence_play(LED_SEQUENCE_SHORT_BLUE);
  led_sequence_play(LED_SEQUENCE_SHORT_YELLOW);
  nrf_lp_delay_ms(2000);
  nrf_gpio_pin_clear(19); //reset
  nrf_lp_delay_ms(10);
  nrf_gpio_pin_clear(BUTTON_1); //button1
  nrf_lp_delay_ms(10);
  app_timer_stop(ANT_Search_timer);
  nrf_lp_delay_ms(10);
  app_timer_stop(m_timer_button_press_timeout);
  nrf_lp_delay_ms(10);
  app_timer_stop(m_timer_button_long_press_timeout);
  nrf_lp_delay_ms(10);
  app_timer_stop(bluetooth_timer);
  nrf_lp_delay_ms(10);
  sd_clock_hfclk_release();
  nrf_lp_delay_ms(10);
  // Disable TWI ready for sleep
  NRF_TWI0->ENABLE = TWI_ENABLE_ENABLE_Disabled << TWI_ENABLE_ENABLE_Pos;
  nrf_lp_delay_ms(10);
  NRF_SPI0->ENABLE = 0;
  nrf_lp_delay_ms(10);
  NRF_UART0->ENABLE = 0;
  nrf_lp_delay_ms(10);

  // shut down the dcdc
  sd_power_dcdc_mode_set(NRF_POWER_DCDC_DISABLE);
  sd_power_pof_enable(0);
  nrf_lp_delay_ms(100);

  nrf_gpio_cfg_default(LED1_G);
  nrf_gpio_cfg_default(LED2_R);
  nrf_gpio_cfg_default(LED2_G);
  nrf_gpio_cfg_default(LED2_B);

  nrf_gpio_cfg_default(BUTTON_1);
  nrf_gpio_cfg_default(19);
  sd_clock_hfclk_release();
  nrf_lp_delay_ms(10);

  nrf_lp_delay_ms(WAIT_TIME);
  nrf_pwr_mgmt_shutdown(NRF_PWR_MGMT_SHUTDOWN_GOTO_SYSOFF);
}

static void profile_setup(void)
{
  ret_code_t err_code;
  ant_search_config_t lev_search_config = DEFAULT_ANT_SEARCH_CONFIG(LEV_CHANNEL_NUM);
  ant_search_config_t controls_search_config = DEFAULT_ANT_SEARCH_CONFIG(CONTROLS_CHANNEL_NUM);
  lev_search_config.low_priority_timeout = 2;       //5 seconds
  controls_search_config.low_priority_timeout = 2;  //5 seconds
  lev_search_config.high_priority_timeout = 5;      //4*2.5 =10 seconds
  controls_search_config.high_priority_timeout = 5; //4*2.5 =10 seconds

  //start the ANT Search LED if profiles are active
  if (ebike || garmin)
    ANT_Search_Start();

  // fill battery status data page.
  m_antplus_controls.page_82 = ANTPLUS_CONTROLS_PAGE82(295); // battery 2.95 volts, fully charged

  // fill manufacturer's common data page.

  m_antplus_controls.page_80 = ANT_COMMON_page80(CONTROLS_HW_REVISION,
                                                 CONTROLS_MANUFACTURER_ID,
                                                 CONTROLS_MODEL_NUMBER);
  // fill product's common data page.
  m_antplus_controls.page_81 = ANT_COMMON_page81(CONTROLS_SW_REVISION_MAJOR,
                                                 CONTROLS_SW_REVISION_MINOR,
                                                 CONTROLS_SERIAL_NUMBER);

  m_ant_lev.page_80 = ANT_COMMON_page80(LEV_HW_REVISION,
                                        LEV_MANUFACTURER_ID,
                                        LEV_MODEL_NUMBER);
  // fill product's common data page.
  m_ant_lev.page_81 = ANT_COMMON_page81(LEV_SW_REVISION_MAJOR,
                                        LEV_SW_REVISION_MINOR,
                                        LEV_SERIAL_NUMBER);

  //@snippet [ANT LEV RX Profile Setup]

  //retrieve the old ant id from a power cycle

  m_ant_lev_channel_lev_disp_config.device_number = old_ant_device_id;

  if (ebike)
  {
    err_code = ant_lev_disp_init(&m_ant_lev, LEV_DISP_CHANNEL_CONFIG(m_ant_lev), ant_lev_evt_handler);
    APP_ERROR_CHECK(err_code);
    err_code = ant_lev_disp_open(&m_ant_lev);
    // Configure ant search timeout
    err_code = ant_search_init(&lev_search_config);
    APP_ERROR_CHECK(err_code);
  }

  if (garmin)
  {
    err_code = antplus_controls_sens_init(&m_antplus_controls, CONTROLS_SENS_CHANNEL_CONFIG(m_antplus_controls), CONTROLS_SENS_PROFILE_CONFIG(m_antplus_controls));
    APP_ERROR_CHECK(err_code);
    err_code = antplus_controls_sens_open(&m_antplus_controls);
    APP_ERROR_CHECK(err_code);
    err_code = ant_search_init(&controls_search_config);
    APP_ERROR_CHECK(err_code);
  }
}

static void softdevice_setup(void)
{
  ret_code_t err_code = nrf_sdh_enable_request();
  APP_ERROR_CHECK(err_code);

  ASSERT(nrf_sdh_is_enabled());

  err_code = nrf_sdh_ant_enable();
  APP_ERROR_CHECK(err_code);

  err_code = ant_plus_key_set(ANTPLUS_NETWORK_NUM);
  APP_ERROR_CHECK(err_code);
}
/*
static void lfclk_config(void)
{
  ret_code_t err_code = nrf_drv_clock_init();
  APP_ERROR_CHECK(err_code);

  nrf_drv_clock_lfclk_request(NULL);
}
*/

/**@brief Function for initializing the BLE stack.
 *
 * @details Initializes the SoftDevice and the BLE event interrupt.
 */
static void ble_stack_init(void)
{
  ret_code_t err_code;
  /*
  err_code = nrf_sdh_enable_request();
  APP_ERROR_CHECK(err_code);
*/
  // Configure the BLE stack using the default settings.
  // Fetch the start address of the application RAM.
  uint32_t ram_start = 0;
  err_code = nrf_sdh_ble_default_cfg_set(APP_BLE_CONN_CFG_TAG, &ram_start);
  APP_ERROR_CHECK(err_code);
  ram_start += 32;
  //ram_start += 10028;
  // Enable BLE stack.
  err_code = nrf_sdh_ble_enable(&ram_start);
  APP_ERROR_CHECK(err_code);

  // Register a handler for BLE events.
  NRF_SDH_BLE_OBSERVER(m_ble_observer, APP_BLE_OBSERVER_PRIO, ble_evt_handler, NULL);
}

/**@brief Function for the GAP initialization.
 *
 * @details This function sets up all the necessary GAP (Generic Access Profile) parameters of the
 *          device including the device name, appearance, and the preferred connection parameters.
 */
static void gap_params_init(void)
{
  ret_code_t err_code;
  ble_gap_conn_params_t gap_conn_params;
  ble_gap_conn_sec_mode_t sec_mode;

  BLE_GAP_CONN_SEC_MODE_SET_OPEN(&sec_mode);

  err_code = sd_ble_gap_device_name_set(&sec_mode,
                                        (const uint8_t *)DEVICE_NAME,
                                        strlen(DEVICE_NAME));
  APP_ERROR_CHECK(err_code);

  memset(&gap_conn_params, 0, sizeof(gap_conn_params));

  gap_conn_params.min_conn_interval = MIN_CONN_INTERVAL;
  gap_conn_params.max_conn_interval = MAX_CONN_INTERVAL;
  gap_conn_params.slave_latency = SLAVE_LATENCY;
  gap_conn_params.conn_sup_timeout = CONN_SUP_TIMEOUT;

  err_code = sd_ble_gap_ppcp_set(&gap_conn_params);
  APP_ERROR_CHECK(err_code);
}

/**@brief Function for initializing the GATT module.
 */
static void gatt_init(void)
{
  ret_code_t err_code = nrf_ble_gatt_init(&m_gatt, NULL);
  APP_ERROR_CHECK(err_code);
}

/**@brief Function for handling Queued Write Module errors.
 *
 * @details A pointer to this function will be passed to each service which may need to inform the
 *          application about an error.
 *
 * @param[in]   nrf_error   Error code containing information about what went wrong.
 */
static void nrf_qwr_error_handler(uint32_t nrf_error)
{
  APP_ERROR_HANDLER(nrf_error);
}

static void ant_id_write_handler(uint16_t conn_handle, ble_ant_id_t *p_ant_id, uint8_t value)
{
  new_ant_device_id = value;
}

/**@brief Function for initializing services that will be used by the application.
 */
static void services_init(void)
{
  ret_code_t err_code;
  ble_ant_id_init_t init = {0};
  nrf_ble_qwr_init_t qwr_init = {0};

  // Initialize Queued Write Module.
  qwr_init.error_handler = nrf_qwr_error_handler;

  err_code = nrf_ble_qwr_init(&m_qwr, &qwr_init);
  APP_ERROR_CHECK(err_code);

  init.ant_id_write_handler = ant_id_write_handler;

  err_code = ble_service_ant_id_init(&m_ble_ant_id_service, &init);
  APP_ERROR_CHECK(err_code);

  ble_ant_id_on_change(m_conn_handle, &m_ble_ant_id_service, old_ant_device_id);
}

/**@brief Function for handling advertising events.
 *
 * @details This function will be called for advertising events which are passed to the application.
 *
 * @param[in] ble_adv_evt  Advertising event.
 */
static void on_adv_evt(ble_adv_evt_t ble_adv_evt)
{
  switch (ble_adv_evt)
  {
  case BLE_ADV_EVT_FAST:
    NRF_LOG_INFO("Fast advertising.");
    break;

  case BLE_ADV_EVT_IDLE:
    // sleep_mode_enter();
    break;

  default:
    break;
  }
}

/**@brief Function for initializing the Advertising functionality.
 *
 * @details Encodes the required advertising data and passes it to the stack.
 *          Also builds a structure to be passed to the stack when starting advertising.
 */
static void advertising_init(void)
{
  ret_code_t err_code;
  ble_advertising_init_t init;

  memset(&init, 0, sizeof(init));

  init.advdata.name_type = BLE_ADVDATA_FULL_NAME;
  init.advdata.include_appearance = true;
  init.advdata.flags = BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE;
  init.advdata.uuids_complete.uuid_cnt = sizeof(m_adv_uuids) / sizeof(m_adv_uuids[0]);
  init.advdata.uuids_complete.p_uuids = m_adv_uuids;

  init.config.ble_adv_fast_enabled = true;
  init.config.ble_adv_fast_interval = APP_ADV_INTERVAL;
  init.config.ble_adv_fast_timeout = APP_ADV_DURATION;

  init.evt_handler = on_adv_evt;

  err_code = ble_advertising_init(&m_advertising, &init);
  APP_ERROR_CHECK(err_code);

  ble_advertising_conn_cfg_tag_set(&m_advertising, APP_BLE_CONN_CFG_TAG);
}

/**@brief Function for handling the Connection Parameters Module.
 *
 * @details This function will be called for all events in the Connection Parameters Module that
 *          are passed to the application.
 *
 * @note All this function does is to disconnect. This could have been done by simply
 *       setting the disconnect_on_fail config parameter, but instead we use the event
 *       handler mechanism to demonstrate its use.
 *
 * @param[in] p_evt  Event received from the Connection Parameters Module.
 */
static void on_conn_params_evt(ble_conn_params_evt_t *p_evt)
{
  ret_code_t err_code;

  if (p_evt->evt_type == BLE_CONN_PARAMS_EVT_FAILED)
  {
    err_code = sd_ble_gap_disconnect(m_conn_handle, BLE_HCI_CONN_INTERVAL_UNACCEPTABLE);
    APP_ERROR_CHECK(err_code);
  }
}

/**@brief Function for handling a Connection Parameters error.
 *
 * @param[in] nrf_error  Error code containing information about what went wrong.
 */
static void conn_params_error_handler(uint32_t nrf_error)
{
  APP_ERROR_HANDLER(nrf_error);
}

/**@brief Function for initializing the Connection Parameters module.
 */
static void conn_params_init(void)
{
  ret_code_t err_code;
  ble_conn_params_init_t cp_init;

  memset(&cp_init, 0, sizeof(cp_init));

  cp_init.p_conn_params = NULL;
  cp_init.first_conn_params_update_delay = FIRST_CONN_PARAMS_UPDATE_DELAY;
  cp_init.next_conn_params_update_delay = NEXT_CONN_PARAMS_UPDATE_DELAY;
  cp_init.max_conn_params_update_count = MAX_CONN_PARAMS_UPDATE_COUNT;
  cp_init.start_on_notify_cccd_handle = BLE_GATT_HANDLE_INVALID;
  cp_init.disconnect_on_fail = false;
  cp_init.evt_handler = on_conn_params_evt;
  cp_init.error_handler = conn_params_error_handler;

  err_code = ble_conn_params_init(&cp_init);
  APP_ERROR_CHECK(err_code);
}

/**@brief Function for handling Peer Manager events.
 *
 * @param[in] p_evt  Peer Manager event.
 */
static void pm_evt_handler(pm_evt_t const *p_evt)
{
  ret_code_t err_code;

  pm_handler_on_pm_evt(p_evt);
  pm_handler_flash_clean(p_evt);

  switch (p_evt->evt_id)
  {
  case PM_EVT_PEERS_DELETE_SUCCEEDED:
    advertising_start(false);
    break;

  case PM_EVT_CONN_SEC_START:
    break;

  case PM_EVT_CONN_SEC_SUCCEEDED:
    // Update the rank of the peer.
    ble_conn_state_role(p_evt->conn_handle);
    break;

  case PM_EVT_CONN_SEC_FAILED:
    // In some cases, when securing fails, it can be restarted directly. Sometimes it can be
    // restarted, but only after changing some Security Parameters. Sometimes, it cannot be
    // restarted until the link is disconnected and reconnected. Sometimes it is impossible
    // to secure the link, or the peer device does not support it. How to handle this error
    // is highly application-dependent.
    m_conn_handle = BLE_CONN_HANDLE_INVALID;
    err_code = sd_ble_gap_disconnect(m_conn_handle, BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
    // APP_ERROR_CHECK(err_code);
    break;

  case PM_EVT_CONN_SEC_CONFIG_REQ:
  {
    // A connected peer (central) is trying to pair, but the Peer Manager already has a bond
    // for that peer. Setting allow_repairing to false rejects the pairing request.
    // If this event is ignored (pm_conn_sec_config_reply is not called in the event
    // handler), the Peer Manager assumes allow_repairing to be false.
    pm_conn_sec_config_t conn_sec_config = {.allow_repairing = false};
    pm_conn_sec_config_reply(p_evt->conn_handle, &conn_sec_config);
  }
  break;

  case PM_EVT_STORAGE_FULL:
    // Run garbage collection on the flash.
    err_code = fds_gc();
    if (err_code == FDS_ERR_BUSY || err_code == FDS_ERR_NO_SPACE_IN_QUEUES)
    {
      // Retry.
    }
    else
    {
      APP_ERROR_CHECK(err_code);
    }
    break;

  default:
    break;
  }
}

/**@brief Function for the Peer Manager initialization.
 */
static void peer_manager_init(void)
{
  ble_gap_sec_params_t sec_param;
  ret_code_t err_code;

  err_code = pm_init();
  APP_ERROR_CHECK(err_code);

  memset(&sec_param, 0, sizeof(ble_gap_sec_params_t));

  // Security parameters to be used for all security procedures.
  sec_param.bond = SEC_PARAM_BOND;
  sec_param.mitm = SEC_PARAM_MITM;
  sec_param.lesc = SEC_PARAM_LESC;
  sec_param.keypress = SEC_PARAM_KEYPRESS;
  sec_param.io_caps = SEC_PARAM_IO_CAPABILITIES;
  sec_param.oob = SEC_PARAM_OOB;
  sec_param.min_key_size = SEC_PARAM_MIN_KEY_SIZE;
  sec_param.max_key_size = SEC_PARAM_MAX_KEY_SIZE;
  sec_param.kdist_own.enc = 1;
  sec_param.kdist_own.id = 1;
  sec_param.kdist_peer.enc = 1;
  sec_param.kdist_peer.id = 1;

  err_code = pm_sec_params_set(&sec_param);
  APP_ERROR_CHECK(err_code);

  err_code = pm_register(pm_evt_handler);
  APP_ERROR_CHECK(err_code);
}

void ble_init(void)
{
  ble_stack_init();
  gap_params_init();
  gatt_init();
  services_init();
  advertising_init();
  conn_params_init();
  peer_manager_init();
  advertising_start(true);
}

void check_interrupt_flags(void)
{
  check_motor_init(); //check for errors and motor status

  if (disp_config_flag) //display configuration
  {

    if (ebike)
    {
      led_sequence_play(LED_EVENT_CONFIG_LEV_ACTIVE);
    }

    if (garmin)
    {
      led_sequence_play(LED_EVENT_CONFIG_CTRL_ACTIVE);
    }
    disp_config_flag = false;
  }

  //need flags to handle interrupt events for flash write
  //this is required due to interrupt priority
  //see: https://devzone.nordicsemi.com/f/nordic-q-a/57067/calling-fds_record_update-in-isr

  //first see if there was a change to the ANT ID, if so, store in flash and turn the bluetooth off on restart
  if (new_ant_device_id != old_ant_device_id)
  {
    //first check for configuration changes
    // 0x90 - turn ANT LEV control on
    // 0x91 - turn ANT LEV control off
    // 0x92 - turn ANT CONTROL on
    // 0x93 - turn ANT CONTROL off
    // 0x94 - turn brake control on
    // 0x95 - turn brake control off
    // 0x99 - initiate DFU
    switch (new_ant_device_id)
    {
    case 0x90: //ANT LEV on
      ebike = 1;
      break;

    case 0x91: //ANT LEV off
      ebike = 0;
      break;

    case 0x92: //ANT CONTROL on
      garmin = 1;
      break;

    case 0x93: //ANT CONTROL off
      garmin = 0;
      break;
    case 0x94: //?? control on - not used for now

      break;

    case 0x95: //?? control off - not used for now

      break;
    case 0x99: // start bootloader
        //turn off config mode on reboot
        //INDICATE ENTERING BOOTLOADER MODE
      led_sequence_play(LED_EVENT_ENTER_DFU);
      nrf_lp_delay_ms(3000);
      led_sequence_play(LED_EVENT_ENTER_DFU);
      nrf_lp_delay_ms(3000);
      eeprom_write_variables(old_ant_device_id, 0, ebike, garmin, brake); // disable BLUETOOTH on restart}
      nrf_lp_delay_ms(WAIT_TIME);
      nrf_power_gpregret_set(BOOTLOADER_DFU_START);
      wait_and_reset();
      break;

    default: //ant ID change requested
      old_ant_device_id = new_ant_device_id;
    }

    // save changes and keep in  configuration mode
    eeprom_write_variables(old_ant_device_id, 1, ebike, garmin, brake);
    nrf_lp_delay_ms(WAIT_TIME);
    wait_and_reset();
  }
  //check to see if brightness change is requested'
  if (brightness_flag) //do three levels of brightness, 1 4 and 7
  {
    static uint8_t brightness = 1;
    brightness += 3;
    if (brightness > 7)
      brightness = 1;
    led_set_global_brightness(brightness);
    led_sequence_play_next(LED_EVENT_BRIGHTNESS_CHECK);
    brightness_flag = false;
  }
  // check to see if low power mode is requested
  if (shutdown_flag)
    shutdown();

  // now check for bluetooth flag on plus button press
  if (enable_configuration)
  {
    eeprom_write_variables(old_ant_device_id, 1, ebike, garmin, brake); // Enable BLUETOOTH on restart}
    nrf_lp_delay_ms(WAIT_TIME);
    wait_and_reset();
  }

  // finally check bluetooth timeout flag and minus button press
  if (disable_configuration)
  {
    eeprom_write_variables(old_ant_device_id, 0, ebike, garmin, brake); // Disable BLUETOOTH on restart}
    nrf_lp_delay_ms(WAIT_TIME);
    wait_and_reset();
  }
}
static void init_app_timers(void)
{
  ret_code_t err_code;
  err_code = app_timer_init();
  APP_ERROR_CHECK(err_code);

  err_code = app_timer_create(&bluetooth_timer, APP_TIMER_MODE_SINGLE_SHOT, bluetooth_timer_timeout);
  APP_ERROR_CHECK(err_code);

  err_code = app_timer_stop(bluetooth_timer);
  APP_ERROR_CHECK(err_code);

  err_code = app_timer_create(&ANT_Search_timer, APP_TIMER_MODE_REPEATED, ANT_Search_timeout);
  APP_ERROR_CHECK(err_code);
  nrf_lp_delay_ms_init();
}

static void leds_init(void)
{
  ret_code_t ret_val;
  if (LEDS_NUMBER > 0)
  {

    bsp_board_init(BSP_INIT_LEDS);
    ret_val = bsp_init(BSP_INIT_LEDS, NULL);
    APP_ERROR_CHECK(ret_val);
//turn off the pwr led FOR THE NORDIC DONGLE
#if defined(BOARD_PCA10059)
    bsp_board_led_off(LED_PWR__PIN);
    //note NRF_PWR_MGMT_CONFIG_DEBUG_PIN_ENABLED to flash this LED when cpu active
#endif
  }
}

void ram_retention_setup(void)
{
#define NRF52_ONRAM1_OFFRAM1 POWER_RAM_POWER_S0POWER_On << POWER_RAM_POWER_S0POWER_Pos | POWER_RAM_POWER_S1POWER_On << POWER_RAM_POWER_S1POWER_Pos | POWER_RAM_POWER_S0RETENTION_On << POWER_RAM_POWER_S0RETENTION_Pos | POWER_RAM_POWER_S1RETENTION_On << POWER_RAM_POWER_S1RETENTION_Pos;

#define NRF52_ONRAM1_OFFRAM0 POWER_RAM_POWER_S0POWER_On << POWER_RAM_POWER_S0POWER_Pos | POWER_RAM_POWER_S1POWER_On << POWER_RAM_POWER_S1POWER_Pos | POWER_RAM_POWER_S0RETENTION_Off << POWER_RAM_POWER_S0RETENTION_Pos | POWER_RAM_POWER_S1RETENTION_Off << POWER_RAM_POWER_S1RETENTION_Pos;

#define NRF52_ONRAM0_OFFRAM0 POWER_RAM_POWER_S0POWER_Off << POWER_RAM_POWER_S0POWER_Pos | POWER_RAM_POWER_S1POWER_Off << POWER_RAM_POWER_S1POWER_Pos;

  // Configure nRF52 RAM retention parameters. Set for System Off 0kB RAM retention
  NRF_POWER->RAM[0].POWER = NRF52_ONRAM1_OFFRAM0; //no ram in power off mode
  NRF_POWER->RAM[1].POWER = NRF52_ONRAM1_OFFRAM0;
  NRF_POWER->RAM[2].POWER = NRF52_ONRAM1_OFFRAM0;
  NRF_POWER->RAM[3].POWER = NRF52_ONRAM1_OFFRAM0;
  NRF_POWER->RAM[4].POWER = NRF52_ONRAM1_OFFRAM0;
  NRF_POWER->RAM[5].POWER = NRF52_ONRAM1_OFFRAM0;
  NRF_POWER->RAM[6].POWER = NRF52_ONRAM1_OFFRAM0;
  NRF_POWER->RAM[7].POWER = NRF52_ONRAM1_OFFRAM0;
}
void power_mgt_init(void)
{
  ret_code_t err_code;
  err_code = nrf_pwr_mgmt_init();
  APP_ERROR_CHECK(err_code);
  //set up the pwr configuration
  sd_power_mode_set(NRF_POWER_MODE_LOWPWR);
}

int main(void)
{

  ret_code_t err_code;
  //lfclk_config()
  ram_retention_setup();
  sd_power_dcdc_mode_set(NRF_POWER_DCDC_ENABLE);
  softdevice_setup();

  leds_init();

  init_app_timers();
  buttons_init();
  led_init(); //pwm initialization
  led_sequence_play(LED_EVENT_WIRELESS_BOARD_POWER_ON);
  //read the flash memory and setup the ANT ID and Bluetooth flag
  eeprom_init(&old_ant_device_id, &configuration_flag, &ebike, &garmin, &brake);
  new_ant_device_id = old_ant_device_id; //no change at this time.
  if (configuration_flag)
  {

    led_sequence_play_next(LED_EVENT_CONFIGURATION_MODE);

    //start the bluetooth 5 min timer -turn off configurartion mode automatically in 5 minutes
    err_code = app_timer_start(bluetooth_timer, BLUETOOTH_TIMEOUT, NULL);
    APP_ERROR_CHECK(err_code);
    ble_init();
  }
  else
    profile_setup();
  power_mgt_init();

  //idle loop
  while (true)
  {
    nrf_pwr_mgmt_run();
    check_interrupt_flags();
  }
}