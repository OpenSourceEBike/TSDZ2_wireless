/*
 * TSDZ2 EBike wireless firmware
 *
 * Copyright (C) Casainho, 2020
 *
 * Released under the GPL License, Version 3
 * 
 */

#include <stdio.h>
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_drv_clock.h"
#include "nrf.h"
#include "hardfault.h"
#include "app_error.h"
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
#include "uart.h"
#include "nrf_drv_uart.h"
#include "app_util_platform.h"
#include "app_uart.h"
#include "eeprom.h"
#include "state.h"
#include "ant_interface.h"
#include "nrf_delay.h"
#include "fds.h"
#include "nrf_power.h"
#include <stdbool.h>
#include "boards.h"
#include "nrf_bootloader_info.h"
#include "buttons.h"
#include "screen.h"
#include "led_softblink.h"
#include "low_power_pwm.h"

// Copied from rananna's wireless remote code for led control
uint8_t led_duty_cycle = 120;
//mask_number used for pwm debugging
int8_t mask_number = 0;
#define P_LED BSP_LED_0_MASK //green (pwr)
#define R_LED BSP_LED_1_MASK //red
#define G_LED BSP_LED_2_MASK //green
#define B_LED BSP_LED_3_MASK //blue
//pwm blinking led routine is led_pwm_on
//only one instance may be active at any time
//softblink is the instance flag 1 means led busy, 0 or 2 means led ready
uint8_t soft_blink = 0;
APP_TIMER_DEF(led_timer);

extern uint8_t ui8_g_battery_soc;
ui_vars_t *mp_ui_vars;
bool brake_flag = false;

volatile uint8_t ui8_m_enter_bootloader = 0;
volatile uint8_t ui8_m_ant_device_id = 0;
volatile uint8_t ui8_m_flash_configurations = 0;

static uint16_t m_assist_level_change_timeout = 0;

// uint8_t ui8_m_wheel_speed_integer;
// uint8_t ui8_m_wheel_speed_decimal;

static uint8_t ui8_walk_assist_timeout = 0;

// uint16_t ui16_m_battery_current_filtered_x10;
// uint16_t ui16_m_motor_current_filtered_x10;
// uint16_t ui16_m_battery_power_filtered;
// uint16_t ui16_m_pedal_power_filtered;

// uint8_t g_showNextScreenIndex = 0;
// uint8_t g_showNextScreenPreviousIndex = 0;
uint16_t ui16_m_alternate_field_value;
uint8_t ui8_m_alternate_field_state = 0;
uint8_t ui8_m_alternate_field_timeout_cnt = 0;
uint8_t ui8_m_vthrottle_can_increment_decrement = 0;

typedef enum
{
  TSDZ2_POWER_STATE_OFF_START,
  TSDZ2_POWER_STATE_OFF_WAIT,
  TSDZ2_POWER_STATE_OFF,
  TSDZ2_POWER_STATE_ON_START,
  TSDZ2_POWER_STATE_ON,
} TSDZ2_power_state_t;

TSDZ2_power_state_t m_TSDZ2_power_state = TSDZ2_POWER_STATE_OFF_START;

#define MSEC_PER_TICK 10
APP_TIMER_DEF(main_timer);
#define MAIN_INTERVAL APP_TIMER_TICKS(MSEC_PER_TICK)
volatile uint32_t main_ticks;
uint32_t ui32_seconds_since_startup = 0;

bool m_rt_processing_stop = false;

void rt_processing_stop(void)
{
  m_rt_processing_stop = true;
}

void rt_processing_start(void)
{
  m_rt_processing_stop = false;
}

#define LEV_HW_REVISION 1
#define LEV_MANUFACTURER_ID (UINT16_MAX - 1)
#define LEV_MODEL_NUMBER 1
#define LEV_SW_REVISION_MAJOR 1
#define LEV_SW_REVISION_MINOR 1
#define LEV_SERIAL_NUMBER 1234567
#define LEV_CHANNEL_NUM 0    // ?? seems can be any value from 0 to 8
#define CHAN_ID_TRANS_TYPE 5 // LEV table 4.2
#define CHAN_ID_DEV_NUM 1    // [1 - 65535], LEV table 4.2
#define ANTPLUS_NETWORK_NUM 0
#define ANT_LEV_ANT_OBSERVER_PRIO 1

void ant_lev_evt_handler_pre(ant_lev_profile_t *p_profile, ant_lev_evt_t event);
void ant_lev_evt_handler_post(ant_lev_profile_t *p_profile, ant_lev_evt_t event);

LEV_SENS_CHANNEL_CONFIG_DEF(m_ant_lev,
                            LEV_CHANNEL_NUM,
                            CHAN_ID_TRANS_TYPE,
                            CHAN_ID_DEV_NUM,
                            ANTPLUS_NETWORK_NUM);
LEV_SENS_PROFILE_CONFIG_DEF(m_ant_lev,
                            ant_lev_evt_handler_pre,
                            ant_lev_evt_handler_post);

static ant_lev_profile_t m_ant_lev;

NRF_SDH_ANT_OBSERVER(m_ant_observer, ANT_LEV_ANT_OBSERVER_PRIO, ant_lev_sens_evt_handler, &m_ant_lev);

#define DEVICE_NAME "TSDZ2_wireless" /**< Name of device. Will be included in the advertising data. */

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

NRF_BLE_GATT_DEF(m_gatt);           /**< GATT module instance. */
NRF_BLE_QWR_DEF(m_qwr);             /**< Context for the Queued Write module.*/
BLE_ADVERTISING_DEF(m_advertising); /**< Advertising module instance. */
BLE_ANT_ID_DEF(m_ble_ant_id_service);
BLE_TSDZ2_DEF(m_ble_tsdz2_service);

void enter_dfu(void);

static uint16_t m_conn_handle = BLE_CONN_HANDLE_INVALID; /**< Handle of the current connection. */

/**< Universally unique service identifiers. */
static ble_uuid_t m_adv_uuids[] =
    {
        {ANT_ID_UUID_SERVICE, BLE_UUID_TYPE_VENDOR_BEGIN},
};

/**@brief Clear bond information from persistent storage.
 */
/*
uint32_t ui32_seconds_since_startup = 0;
//uint32_t err_code=0;
volatile uint32_t main_ticks;
uint8_t enable_bluetooth = 0;
*/
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

void ant_lev_evt_handler_pre(ant_lev_profile_t *p_profile, ant_lev_evt_t event)
{
  nrf_pwr_mgmt_feed();
  //set the assist level
  p_profile->common.travel_mode_state = (mp_ui_vars->ui8_assist_level << 3);
  p_profile->page_16.travel_mode = p_profile->common.travel_mode_state;
  //common gear state is used for motor state control
  // use front gear setting to transfer motor state information to the remote:
  // the two bits are used as follows:
  // 00 - motor off (MOTOR_INIT_OFF (0))
  // 01  - motor ready (MOTOR_INIT_READY (1))
  // 10  - motor not ready (MOTOR_INIT_GET_MOTOR_ALIVE-2, MOTOR_INIT_WAIT_MOTOR_ALIVE-3)
  // 11  - signal to turn motor on/off (3)
  // higher bits are used to indicated motor start up errors (MOTOR_INIT_ERROR_ALIVE -4, MOTOR_INIT_ERROR_GET_FIRMWARE_VERSION-8,
  //       MOTOR_INIT_ERROR_FIRMWARE_VERSION-10,MOTOR_INIT_ERROR_SET_CONFIGURATIONS-14)
  //set up the common gear state byte
  if (g_motor_init_state < 4)         // no errors
    p_profile->common.gear_state = 0; // clear any error states

  switch (g_motor_init_state)
  {
  case MOTOR_INIT_OFF:

    //first two bits are front gear
    p_profile->common.gear_state = (p_profile->common.gear_state & 0x7C) + 0;
    break;

  case MOTOR_INIT_READY:
    p_profile->common.gear_state = (p_profile->common.gear_state & 0x7C) + 1;
    break;
  case MOTOR_INIT_GET_MOTOR_ALIVE:
  case MOTOR_INIT_WAIT_MOTOR_ALIVE:
    p_profile->common.gear_state = (p_profile->common.gear_state & 0x7C) + 2;

    break;
    // note gear state value 3 is used to turn on/off the motor; above 3 are error states
  case MOTOR_INIT_ERROR_ALIVE:
    p_profile->common.gear_state = (4 | (p_profile->common.gear_state & 0x03));
    //p_profile->page_16.current_rear_gear = 1;
    break;
  case MOTOR_INIT_ERROR_GET_FIRMWARE_VERSION:
    p_profile->common.gear_state = (8 | (p_profile->common.gear_state & 0x03));
    // p_profile->page_16.current_rear_gear = 2;
    break;
  case MOTOR_INIT_ERROR_FIRMWARE_VERSION:
    p_profile->common.gear_state = (12 | (p_profile->common.gear_state & 0x03));
    p_profile->page_16.current_rear_gear = 3;
    break;
  case MOTOR_INIT_ERROR_SET_CONFIGURATIONS:
    p_profile->common.gear_state = (16 | (p_profile->common.gear_state & 0x03));
    // p_profile->page_16.current_rear_gear = 4;
    break;

  default:
    break;
  }
  //set variables for ANT transmission

  // battery voltage
  p_profile->page_4.battery_voltage = ui_vars.ui16_battery_voltage_filtered_x10;

  // assist level
  p_profile->common.travel_mode_state |= (mp_ui_vars->ui8_assist_level << 3) & 0x38;

  // lights
  p_profile->common.system_state |= (mp_ui_vars->ui8_lights << 3) & 0x08;

  // lev speed
  p_profile->common.lev_speed = ui_vars.ui16_wheel_speed_x10 / 10;

  //state of charge
  p_profile->page_3.battery_soc = ui8_g_battery_soc;

  switch (event)
  {
  case ANT_LEV_PAGE_1_UPDATED:

    break;
  case ANT_LEV_PAGE_2_UPDATED:

    break;

  case ANT_LEV_PAGE_3_UPDATED:

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

void ant_lev_evt_handler_post(ant_lev_profile_t *p_profile, ant_lev_evt_t event)
{
  nrf_pwr_mgmt_feed();

  switch (event)
  {
  case ANT_LEV_PAGE_1_UPDATED:
    break;

  case ANT_LEV_PAGE_2_UPDATED:
    break;

  case ANT_LEV_PAGE_3_UPDATED:
    break;

  case ANT_LEV_PAGE_4_UPDATED:
    break;

  case ANT_LEV_PAGE_5_UPDATED:
    break;

  case ANT_LEV_PAGE_34_UPDATED:
    break;

  case ANT_LEV_PAGE_16_UPDATED:

    if (p_profile->page_16.light)
    {
      //light mode activated
      
    }
    else
    {
      // light mode  deactivated

    }

    if (p_profile->page_16.current_rear_gear == 14)
    {
      // walk mode is activated
      
    }
    if (p_profile->page_16.current_rear_gear == 15)
    {
      // enable brakes: be as fast as possible
      nrf_gpio_port_out_clear(NRF_P0, 1UL << BRAKE__PIN);
    }
    if (p_profile->page_16.current_rear_gear == 0)
    {
      //this state sshould clear both brakes and walk mode
      // disable walk mode
      // disable brakes: be as fast as possible
      nrf_gpio_port_out_set(NRF_P0, 1UL << BRAKE__PIN);
    }
    if (p_profile->page_16.current_front_gear == 3)
    {

      if (m_TSDZ2_power_state == TSDZ2_POWER_STATE_OFF)
      {
        // turn on TSDZ2 motor controller
        m_TSDZ2_power_state = TSDZ2_POWER_STATE_ON_START;
      }

      else if (m_TSDZ2_power_state == TSDZ2_POWER_STATE_ON)
      {
        //  turn off TSDZ2 motor controller
        m_TSDZ2_power_state = TSDZ2_POWER_STATE_OFF_START;
      }
    }

    // assist level
    p_profile->common.travel_mode_state = p_profile->page_16.travel_mode;
    mp_ui_vars->ui8_assist_level = p_profile->page_16.travel_mode >> 3;

    // lights
    p_profile->common.system_state |= ((((uint8_t)p_profile->page_16.light) << 3) & 0x08);
    mp_ui_vars->ui8_lights = ((uint8_t)p_profile->page_16.light);
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

static void ant_setup(void)
{
  ret_code_t err_code;

  err_code = nrf_sdh_ant_enable();
  APP_ERROR_CHECK(err_code);

  err_code = ant_plus_key_set(ANTPLUS_NETWORK_NUM);
  APP_ERROR_CHECK(err_code);

  ui_vars_t *p_ui_vars = get_ui_vars();
  m_ant_lev_channel_lev_sens_config.device_number = p_ui_vars->ui8_ant_device_id;

  err_code = ant_lev_sens_init(&m_ant_lev,
                               &m_ant_lev_channel_lev_sens_config,
                               LEV_SENS_PROFILE_CONFIG(m_ant_lev));
  APP_ERROR_CHECK(err_code);

  // fill manufacturer's common data page.
  m_ant_lev.page_80 = ANT_COMMON_page80(LEV_HW_REVISION,
                                        LEV_MANUFACTURER_ID,
                                        LEV_MODEL_NUMBER);
  // fill product's common data page.
  m_ant_lev.page_81 = ANT_COMMON_page81(LEV_SW_REVISION_MAJOR,
                                        LEV_SW_REVISION_MINOR,
                                        LEV_SERIAL_NUMBER);

  err_code = ant_lev_sens_open(&m_ant_lev);
  APP_ERROR_CHECK(err_code);
}

static void main_timer_timeout(void *p_context)
{
  UNUSED_PARAMETER(p_context);

  main_ticks++;

  if (main_ticks % (1000 / MSEC_PER_TICK) == 0)
    ui32_seconds_since_startup++;

  if ((main_ticks % (50 / MSEC_PER_TICK) == 0) && // every 50ms
      m_rt_processing_stop == false)
    rt_processing();
}

/// msecs since boot (note: will roll over every 50 days)
uint32_t get_time_base_counter_1ms()
{
  return main_ticks * MSEC_PER_TICK;
}

uint32_t get_seconds()
{
  return ui32_seconds_since_startup;
}

static void lfclk_config(void)
{
  uint32_t err_code = nrf_drv_clock_init();
  APP_ERROR_CHECK(err_code);

  nrf_drv_clock_lfclk_request(NULL);
}

static void init_app_timers(void)
{
  ret_code_t err_code;

  err_code = app_timer_init();
  APP_ERROR_CHECK(err_code);

  err_code = app_timer_create(&main_timer, APP_TIMER_MODE_REPEATED, main_timer_timeout);
  APP_ERROR_CHECK(err_code);

  err_code = app_timer_start(main_timer, MAIN_INTERVAL, NULL);
  APP_ERROR_CHECK(err_code);
}

/**@brief Function for initializing the BLE stack.
 *
 * @details Initializes the SoftDevice and the BLE event interrupt.
 */
static void ble_stack_init(void)
{
  ret_code_t err_code;

  err_code = nrf_sdh_enable_request();
  APP_ERROR_CHECK(err_code);

  // Configure the BLE stack using the default settings.
  // Fetch the start address of the application RAM.
  uint32_t ram_start = 0;
  err_code = nrf_sdh_ble_default_cfg_set(APP_BLE_CONN_CFG_TAG, &ram_start);
  APP_ERROR_CHECK(err_code);

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

  //err_code = nrf_ble_gatt_att_mtu_periph_set(&m_gatt, NRF_SDH_BLE_GATT_MAX_MTU_SIZE);
  //APP_ERROR_CHECK(err_code);
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
  if (value == 0x99)
  {
    ui8_m_enter_bootloader = 1;
  }
  else
  {
    ui8_m_ant_device_id = value;
  }
}

static void tsdz2_write_handler_periodic(uint8_t *p_data, uint16_t len)
{
  if (p_data[0] != 255)
    ui_vars.ui8_assist_level = p_data[0];

  if (p_data[1] != 255)
  {
    switch (p_data[1])
    {
    case 1:
      // turn on TSDZ2 motor controller
      if (m_TSDZ2_power_state == TSDZ2_POWER_STATE_OFF)
        m_TSDZ2_power_state = TSDZ2_POWER_STATE_ON_START;
      break;

    case 2:
      // turn off TSDZ2 motor controller
      if (m_TSDZ2_power_state == TSDZ2_POWER_STATE_ON)
        m_TSDZ2_power_state = TSDZ2_POWER_STATE_OFF_START;
      break;
    }
  }
}

static void tsdz2_write_handler_configurations(uint8_t *p_data, uint16_t len)
{
  static uint8_t data[BLE_TSDZ2_CONFIGURATIONS_LEN];

  memcpy(&data, p_data, BLE_TSDZ2_CONFIGURATIONS_LEN);

  // assume received configurations only if version is correct
  if (data[0] == ui_vars.ui8_configurations_version)
  {
    ui_vars.ui8_assist_level = data[1];
    ui_vars.ui16_wheel_perimeter = ((uint16_t)data[2]) +
                                   (((uint16_t)data[3]) << 8);
    ui_vars.ui8_wheel_max_speed = data[4];
    ui_vars.ui8_units_type = data[5];
    ui_vars.ui32_wh_x10 = ((uint16_t)data[6]) +
                          (((uint16_t)data[7]) << 8) +
                          (((uint16_t)data[8]) << 16) +
                          (((uint16_t)data[9]) << 24);
    ui_vars.ui32_wh_x10_100_percent = ((uint16_t)data[10]) +
                                      (((uint16_t)data[11]) << 8) +
                                      (((uint16_t)data[12]) << 16) +
                                      (((uint16_t)data[13]) << 24);

    ui_vars.ui8_battery_soc_enable = data[14];
    ui_vars.ui8_target_max_battery_power_div25 = data[15];
    ui_vars.ui8_battery_max_current = data[16];
    ui_vars.ui8_motor_max_current = data[17];
    ui_vars.ui8_motor_current_min_adc = data[18];
    ui_vars.ui8_field_weakening = data[19];
    ui_vars.ui8_ramp_up_amps_per_second_x10 = data[20];
    ui_vars.ui16_battery_low_voltage_cut_off_x10 = ((uint16_t)data[21]) +
                                                   (((uint16_t)data[22]) << 8);
    ui_vars.ui8_motor_type = data[23];
    ui_vars.ui8_motor_current_control_mode = data[24];
    ui_vars.ui8_motor_assistance_startup_without_pedal_rotation = data[25];

    ui_vars.ui16_assist_level_factor[0] = ((uint16_t)data[26]) +
                                          (((uint16_t)data[27]) << 8);
    ui_vars.ui16_assist_level_factor[1] = ((uint16_t)data[28]) +
                                          (((uint16_t)data[29]) << 8);
    ui_vars.ui16_assist_level_factor[2] = ((uint16_t)data[30]) +
                                          (((uint16_t)data[31]) << 8);
    ui_vars.ui16_assist_level_factor[3] = ((uint16_t)data[32]) +
                                          (((uint16_t)data[33]) << 8);
    ui_vars.ui16_assist_level_factor[4] = ((uint16_t)data[34]) +
                                          (((uint16_t)data[35]) << 8);
    ui_vars.ui16_assist_level_factor[5] = ((uint16_t)data[36]) +
                                          (((uint16_t)data[37]) << 8);
    ui_vars.ui16_assist_level_factor[6] = ((uint16_t)data[38]) +
                                          (((uint16_t)data[39]) << 8);

    ui_vars.ui8_number_of_assist_levels = data[40];
    ui_vars.ui8_startup_motor_power_boost_feature_enabled = data[41];
    ui_vars.ui8_startup_motor_power_boost_always = data[42];
    ui_vars.ui8_startup_motor_power_boost_limit_power = data[43];

    ui_vars.ui16_startup_motor_power_boost_factor[0] = ((uint16_t)data[44]) +
                                                       (((uint16_t)data[45]) << 8);
    ui_vars.ui16_startup_motor_power_boost_factor[1] = ((uint16_t)data[46]) +
                                                       (((uint16_t)data[47]) << 8);
    ui_vars.ui16_startup_motor_power_boost_factor[2] = ((uint16_t)data[48]) +
                                                       (((uint16_t)data[49]) << 8);
    ui_vars.ui16_startup_motor_power_boost_factor[3] = ((uint16_t)data[50]) +
                                                       (((uint16_t)data[51]) << 8);
    ui_vars.ui16_startup_motor_power_boost_factor[4] = ((uint16_t)data[52]) +
                                                       (((uint16_t)data[53]) << 8);
    ui_vars.ui16_startup_motor_power_boost_factor[5] = ((uint16_t)data[54]) +
                                                       (((uint16_t)data[55]) << 8);
    ui_vars.ui16_startup_motor_power_boost_factor[6] = ((uint16_t)data[159]) +
                                                       (((uint16_t)data[160]) << 8);

    ui_vars.ui8_startup_motor_power_boost_time = data[56];
    ui_vars.ui8_startup_motor_power_boost_fade_time = data[57];
    ui_vars.ui8_temperature_limit_feature_enabled = data[58];
    ui_vars.ui8_motor_temperature_min_value_to_limit = data[59];
    ui_vars.ui8_motor_temperature_max_value_to_limit = data[60];

    ui_vars.ui8_coast_brake_enable = data[61];
    ui_vars.ui8_coast_brake_adc = data[62];

    ui_vars.ui16_battery_voltage_reset_wh_counter_x10 = ((uint16_t)data[63]) +
                                                        (((uint16_t)data[64]) << 8);

    ui_vars.ui8_system_power_off_time_minutes = data[65];

    ui_vars.ui16_battery_pack_resistance_x1000 = ((uint16_t)data[66]) +
                                                 (((uint16_t)data[67]) << 8);

    ui_vars.ui32_odometer_x10 = ((uint16_t)data[68]) +
                                (((uint16_t)data[69]) << 8) +
                                (((uint16_t)data[70]) << 16) +
                                (((uint16_t)data[71]) << 24);

    ui_vars.ui8_walk_assist_feature_enabled = data[72];

    ui_vars.ui8_walk_assist_level_factor[0] = data[73];
    ui_vars.ui8_walk_assist_level_factor[1] = data[74];
    ui_vars.ui8_walk_assist_level_factor[2] = data[75];
    ui_vars.ui8_walk_assist_level_factor[3] = data[76];
    ui_vars.ui8_walk_assist_level_factor[4] = data[77];
    ui_vars.ui8_walk_assist_level_factor[5] = data[78];
    ui_vars.ui8_walk_assist_level_factor[6] = data[79];

    ui_vars.ui8_torque_sensor_calibration_feature_enabled = data[80];
    ui_vars.ui8_torque_sensor_calibration_pedal_ground = data[81];
    ui_vars.ui8_torque_sensor_filter = data[82];
    ui_vars.ui8_torque_sensor_adc_threshold = data[83];

    ui_vars.ui16_torque_sensor_calibration_table_left[0][0] = ((uint16_t)data[84]) +
                                                              (((uint16_t)data[85]) << 8);
    ui_vars.ui16_torque_sensor_calibration_table_left[0][1] = ((uint16_t)data[86]) +
                                                              (((uint16_t)data[87]) << 8);

    ui_vars.ui16_torque_sensor_calibration_table_left[1][0] = ((uint16_t)data[88]) +
                                                              (((uint16_t)data[89]) << 8);
    ui_vars.ui16_torque_sensor_calibration_table_left[1][1] = ((uint16_t)data[90]) +
                                                              (((uint16_t)data[91]) << 8);

    ui_vars.ui16_torque_sensor_calibration_table_left[2][0] = ((uint16_t)data[92]) +
                                                              (((uint16_t)data[93]) << 8);
    ui_vars.ui16_torque_sensor_calibration_table_left[2][1] = ((uint16_t)data[94]) +
                                                              (((uint16_t)data[95]) << 8);

    ui_vars.ui16_torque_sensor_calibration_table_left[3][0] = ((uint16_t)data[96]) +
                                                              (((uint16_t)data[97]) << 8);
    ui_vars.ui16_torque_sensor_calibration_table_left[3][1] = ((uint16_t)data[98]) +
                                                              (((uint16_t)data[99]) << 8);

    ui_vars.ui16_torque_sensor_calibration_table_left[4][0] = ((uint16_t)data[100]) +
                                                              (((uint16_t)data[101]) << 8);
    ui_vars.ui16_torque_sensor_calibration_table_left[4][1] = ((uint16_t)data[102]) +
                                                              (((uint16_t)data[103]) << 8);

    ui_vars.ui16_torque_sensor_calibration_table_left[5][0] = ((uint16_t)data[104]) +
                                                              (((uint16_t)data[105]) << 8);
    ui_vars.ui16_torque_sensor_calibration_table_left[5][1] = ((uint16_t)data[106]) +
                                                              (((uint16_t)data[107]) << 8);

    ui_vars.ui16_torque_sensor_calibration_table_left[6][0] = ((uint16_t)data[108]) +
                                                              (((uint16_t)data[109]) << 8);
    ui_vars.ui16_torque_sensor_calibration_table_left[6][1] = ((uint16_t)data[110]) +
                                                              (((uint16_t)data[111]) << 8);

    ui_vars.ui16_torque_sensor_calibration_table_left[7][0] = ((uint16_t)data[112]) +
                                                              (((uint16_t)data[113]) << 8);
    ui_vars.ui16_torque_sensor_calibration_table_left[7][1] = ((uint16_t)data[114]) +
                                                              (((uint16_t)data[115]) << 8);

    ui_vars.ui16_torque_sensor_calibration_table_right[0][0] = ((uint16_t)data[116]) +
                                                               (((uint16_t)data[117]) << 8);
    ui_vars.ui16_torque_sensor_calibration_table_right[0][1] = ((uint16_t)data[118]) +
                                                               (((uint16_t)data[119]) << 8);

    ui_vars.ui16_torque_sensor_calibration_table_right[1][0] = ((uint16_t)data[120]) +
                                                               (((uint16_t)data[121]) << 8);
    ui_vars.ui16_torque_sensor_calibration_table_right[1][1] = ((uint16_t)data[122]) +
                                                               (((uint16_t)data[123]) << 8);

    ui_vars.ui16_torque_sensor_calibration_table_right[2][0] = ((uint16_t)data[124]) +
                                                               (((uint16_t)data[125]) << 8);
    ui_vars.ui16_torque_sensor_calibration_table_right[2][1] = ((uint16_t)data[126]) +
                                                               (((uint16_t)data[127]) << 8);

    ui_vars.ui16_torque_sensor_calibration_table_right[3][0] = ((uint16_t)data[128]) +
                                                               (((uint16_t)data[129]) << 8);
    ui_vars.ui16_torque_sensor_calibration_table_right[3][1] = ((uint16_t)data[130]) +
                                                               (((uint16_t)data[131]) << 8);

    ui_vars.ui16_torque_sensor_calibration_table_right[4][0] = ((uint16_t)data[132]) +
                                                               (((uint16_t)data[133]) << 8);
    ui_vars.ui16_torque_sensor_calibration_table_right[4][1] = ((uint16_t)data[134]) +
                                                               (((uint16_t)data[135]) << 8);

    ui_vars.ui16_torque_sensor_calibration_table_right[5][0] = ((uint16_t)data[136]) +
                                                               (((uint16_t)data[137]) << 8);
    ui_vars.ui16_torque_sensor_calibration_table_right[5][1] = ((uint16_t)data[138]) +
                                                               (((uint16_t)data[139]) << 8);

    ui_vars.ui16_torque_sensor_calibration_table_right[6][0] = ((uint16_t)data[140]) +
                                                               (((uint16_t)data[141]) << 8);
    ui_vars.ui16_torque_sensor_calibration_table_right[6][1] = ((uint16_t)data[142]) +
                                                               (((uint16_t)data[143]) << 8);

    ui_vars.ui16_torque_sensor_calibration_table_right[7][0] = ((uint16_t)data[144]) +
                                                               (((uint16_t)data[145]) << 8);
    ui_vars.ui16_torque_sensor_calibration_table_right[7][1] = ((uint16_t)data[146]) +
                                                               (((uint16_t)data[147]) << 8);

    ui_vars.ui8_street_mode_function_enabled = data[148];
    ui_vars.ui8_street_mode_enabled = data[149];
    ui_vars.ui8_street_mode_enabled_on_startup = data[150];
    ui_vars.ui8_street_mode_speed_limit = data[151];
    ui_vars.ui8_street_mode_power_limit_div25 = data[152];
    ui_vars.ui8_street_mode_throttle_enabled = data[153];
    ui_vars.ui8_street_mode_hotkey_enabled = data[154];
    ui_vars.ui8_pedal_cadence_fast_stop = data[155];
    ui_vars.ui8_throttle_virtual_step = data[156];
    ui_vars.ui8_street_mode_enabled = data[157];

    ui_vars.ui8_ant_device_id = data[158];

    ui8_m_flash_configurations = 1;
  }
}

/**@brief Function for initializing services that will be used by the application.
 */
static void services_init(void)
{
  ret_code_t err_code;
  nrf_ble_qwr_init_t qwr_init = {0};
  ble_ant_id_init_t init_ant_id = {0};
  ble_tsdz2_init_t init_tsdz2 = {0};

  // Initialize Queued Write Module.
  qwr_init.error_handler = nrf_qwr_error_handler;

  err_code = nrf_ble_qwr_init(&m_qwr, &qwr_init);
  APP_ERROR_CHECK(err_code);

  // ANT ID
  init_ant_id.ant_id_write_handler = ant_id_write_handler;

  err_code = ble_service_ant_id_init(&m_ble_ant_id_service, &init_ant_id);
  APP_ERROR_CHECK(err_code);

  // set actual ANT ID on the BLE service characteristic
  ble_ant_id_on_change(m_conn_handle, &m_ble_ant_id_service, mp_ui_vars->ui8_ant_device_id);

  // TSDZ2
  memset(&init_tsdz2, 0, sizeof(init_tsdz2));

  init_tsdz2.tsdz2_write_handler_periodic = tsdz2_write_handler_periodic;
  init_tsdz2.tsdz2_write_handler_configurations = tsdz2_write_handler_configurations;

  err_code = ble_service_tsdz2_init(&m_ble_tsdz2_service, &init_tsdz2);
  APP_ERROR_CHECK(err_code);
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

void eeprom_write_variables_and_reset(void)
{
  // NOTE that flash of EEPROM does not work on an interrupt like on the ant_id_write_handler(), hence it is done here on main()
  eeprom_write_variables();

  // wait some time to make sure eeprom is written
  nrf_delay_ms(1000);

  // finally reset so the new ANT ID will take effect
  NVIC_SystemReset();
}

void ble_send_periodic_data(void)
{
  // send periodic to mobile app
  uint8_t tx_data[BLE_TSDZ2_PERIODIC_LEN] = {0};
  tx_data[0] = (uint8_t)(ui_vars.ui16_battery_voltage_filtered_x10 & 0xff);
  tx_data[1] = (uint8_t)(ui_vars.ui16_battery_voltage_filtered_x10 >> 8);
  tx_data[2] = ui_vars.ui8_battery_current_x5;
  tx_data[3] = (uint8_t)(ui_vars.ui16_wheel_speed_x10 & 0xff);
  tx_data[4] = (uint8_t)(ui_vars.ui16_wheel_speed_x10 >> 8);
  tx_data[5] = (uint8_t)((ui_vars.ui8_braking & 1) | ((ui_vars.ui8_lights & 1) << 1));
  tx_data[6] = ui_vars.ui8_motor_hall_sensors;
  tx_data[7] = ui_vars.ui8_pas_pedal_right;
  tx_data[8] = ui_vars.ui8_adc_throttle;
  tx_data[9] = ui_vars.ui8_motor_temperature;
  tx_data[10] = ui_vars.ui8_throttle;
  tx_data[11] = (uint8_t)(ui_vars.ui16_adc_pedal_torque_sensor & 0xff);
  tx_data[12] = (uint8_t)(ui_vars.ui16_adc_pedal_torque_sensor >> 8);
  tx_data[13] = ui_vars.ui8_pedal_weight_with_offset;
  tx_data[14] = ui_vars.ui8_pedal_weight;
  tx_data[15] = ui_vars.ui8_pedal_cadence_filtered;
  tx_data[16] = ui_vars.ui8_duty_cycle;
  tx_data[17] = (uint8_t)(ui_vars.ui16_motor_speed_erps & 0xff);
  tx_data[18] = (uint8_t)(ui_vars.ui16_motor_speed_erps >> 8);
  tx_data[19] = ui_vars.ui8_foc_angle;
  tx_data[20] = ui_vars.ui8_error_states;
  tx_data[21] = ui_vars.ui8_motor_current_x5;
  tx_data[22] = (uint8_t)(ui_vars.ui16_adc_battery_current & 0xff);
  tx_data[23] = (uint8_t)(ui_vars.ui16_adc_battery_current >> 8);
  tx_data[24] = ui_vars.ui8_assist_level;
  tx_data[25] = (uint8_t)(ui_vars.ui16_pedal_power & 0xff);
  tx_data[26] = (uint8_t)(ui_vars.ui16_pedal_power >> 8);
  tx_data[27] = ui8_g_battery_soc;
  tx_data[28] = (uint8_t)(ui_vars.ui32_odometer_x10 & 0xff);
  tx_data[29] = (uint8_t)(ui_vars.ui32_odometer_x10 >> 8);
  tx_data[30] = (uint8_t)(ui_vars.ui32_odometer_x10 >> 16);
  tx_data[31] = (uint8_t)(ui_vars.ui32_odometer_x10 >> 24);
  tx_data[28] = (uint8_t)(ui_vars.ui32_wh_x10 & 0xff);
  tx_data[29] = (uint8_t)(ui_vars.ui32_wh_x10 >> 8);
  tx_data[30] = (uint8_t)(ui_vars.ui32_wh_x10 >> 16);
  tx_data[31] = (uint8_t)(ui_vars.ui32_wh_x10 >> 24);
  tx_data[32] = (uint8_t)g_motor_init_state;

  if (m_conn_handle != BLE_CONN_HANDLE_INVALID)
  {
    ret_code_t err_code;
    err_code = ble_tsdz2_periodic_on_change(m_conn_handle, &m_ble_tsdz2_service, tx_data);
    if (err_code == NRF_SUCCESS)
    {
    }
  }
}

void ble_update_configurations_data(void)
{
  uint8_t tx_data[BLE_TSDZ2_CONFIGURATIONS_LEN];

  if (m_conn_handle != BLE_CONN_HANDLE_INVALID)
  {
    tx_data[0] = CONFIGURATIONS_VERSION;
    tx_data[1] = ui_vars.ui8_assist_level;
    tx_data[2] = (uint8_t)(ui_vars.ui16_wheel_perimeter & 0xff);
    tx_data[3] = (uint8_t)(ui_vars.ui16_wheel_perimeter >> 8);
    tx_data[4] = ui_vars.ui8_wheel_max_speed;
    tx_data[5] = ui_vars.ui8_units_type;
    tx_data[6] = (uint8_t)(ui_vars.ui32_wh_x10 & 0xff);
    tx_data[7] = (uint8_t)((ui_vars.ui32_wh_x10 >> 8) & 0xff);
    tx_data[8] = (uint8_t)((ui_vars.ui32_wh_x10 >> 16) & 0xff);
    tx_data[9] = (uint8_t)((ui_vars.ui32_wh_x10 >> 24) & 0xff);
    tx_data[10] = (uint8_t)(ui_vars.ui32_wh_x10_100_percent & 0xff);
    tx_data[11] = (uint8_t)((ui_vars.ui32_wh_x10_100_percent >> 8) & 0xff);
    tx_data[12] = (uint8_t)((ui_vars.ui32_wh_x10_100_percent >> 16) & 0xff);
    tx_data[13] = (uint8_t)((ui_vars.ui32_wh_x10_100_percent >> 24) & 0xff);
    tx_data[14] = ui_vars.ui8_battery_soc_enable;
    tx_data[15] = ui_vars.ui8_target_max_battery_power_div25;
    tx_data[16] = ui_vars.ui8_battery_max_current;
    tx_data[17] = ui_vars.ui8_motor_max_current;
    tx_data[18] = ui_vars.ui8_motor_current_min_adc;
    tx_data[19] = ui_vars.ui8_field_weakening;
    tx_data[20] = ui_vars.ui8_ramp_up_amps_per_second_x10;
    tx_data[21] = (uint8_t)(ui_vars.ui16_battery_low_voltage_cut_off_x10 & 0xff);
    tx_data[22] = (uint8_t)(ui_vars.ui16_battery_low_voltage_cut_off_x10 >> 8);
    tx_data[23] = ui_vars.ui8_motor_type;
    tx_data[24] = ui_vars.ui8_motor_current_control_mode;
    tx_data[25] = ui_vars.ui8_motor_assistance_startup_without_pedal_rotation;

    tx_data[26] = (uint8_t)(ui_vars.ui16_assist_level_factor[0] & 0xff);
    tx_data[27] = (uint8_t)(ui_vars.ui16_assist_level_factor[0] >> 8);
    tx_data[28] = (uint8_t)(ui_vars.ui16_assist_level_factor[1] & 0xff);
    tx_data[29] = (uint8_t)(ui_vars.ui16_assist_level_factor[1] >> 8);
    tx_data[30] = (uint8_t)(ui_vars.ui16_assist_level_factor[2] & 0xff);
    tx_data[31] = (uint8_t)(ui_vars.ui16_assist_level_factor[2] >> 8);
    tx_data[32] = (uint8_t)(ui_vars.ui16_assist_level_factor[3] & 0xff);
    tx_data[33] = (uint8_t)(ui_vars.ui16_assist_level_factor[3] >> 8);
    tx_data[34] = (uint8_t)(ui_vars.ui16_assist_level_factor[4] & 0xff);
    tx_data[35] = (uint8_t)(ui_vars.ui16_assist_level_factor[4] >> 8);
    tx_data[36] = (uint8_t)(ui_vars.ui16_assist_level_factor[5] & 0xff);
    tx_data[37] = (uint8_t)(ui_vars.ui16_assist_level_factor[5] >> 8);
    tx_data[38] = (uint8_t)(ui_vars.ui16_assist_level_factor[6] & 0xff);
    tx_data[39] = (uint8_t)(ui_vars.ui16_assist_level_factor[6] >> 8);

    tx_data[40] = ui_vars.ui8_number_of_assist_levels;
    tx_data[41] = ui_vars.ui8_startup_motor_power_boost_feature_enabled;
    tx_data[42] = ui_vars.ui8_startup_motor_power_boost_always;
    tx_data[43] = ui_vars.ui8_startup_motor_power_boost_limit_power;

    tx_data[44] = (uint8_t)(ui_vars.ui16_startup_motor_power_boost_factor[0] & 0xff);
    tx_data[45] = (uint8_t)(ui_vars.ui16_startup_motor_power_boost_factor[0] >> 8);
    tx_data[46] = (uint8_t)(ui_vars.ui16_startup_motor_power_boost_factor[1] & 0xff);
    tx_data[47] = (uint8_t)(ui_vars.ui16_startup_motor_power_boost_factor[1] >> 8);
    tx_data[48] = (uint8_t)(ui_vars.ui16_startup_motor_power_boost_factor[2] & 0xff);
    tx_data[49] = (uint8_t)(ui_vars.ui16_startup_motor_power_boost_factor[2] >> 8);
    tx_data[50] = (uint8_t)(ui_vars.ui16_startup_motor_power_boost_factor[3] & 0xff);
    tx_data[51] = (uint8_t)(ui_vars.ui16_startup_motor_power_boost_factor[3] >> 8);
    tx_data[52] = (uint8_t)(ui_vars.ui16_startup_motor_power_boost_factor[4] & 0xff);
    tx_data[53] = (uint8_t)(ui_vars.ui16_startup_motor_power_boost_factor[4] >> 8);
    tx_data[54] = (uint8_t)(ui_vars.ui16_startup_motor_power_boost_factor[5] & 0xff);
    tx_data[55] = (uint8_t)(ui_vars.ui16_startup_motor_power_boost_factor[5] >> 8);
    tx_data[159] = (uint8_t)(ui_vars.ui16_startup_motor_power_boost_factor[6] & 0xff);
    tx_data[160] = (uint8_t)(ui_vars.ui16_startup_motor_power_boost_factor[6] >> 8);

    tx_data[56] = ui_vars.ui8_startup_motor_power_boost_time;
    tx_data[57] = ui_vars.ui8_startup_motor_power_boost_fade_time;
    tx_data[58] = ui_vars.ui8_temperature_limit_feature_enabled;
    tx_data[59] = ui_vars.ui8_motor_temperature_min_value_to_limit;
    tx_data[60] = ui_vars.ui8_motor_temperature_max_value_to_limit;
    tx_data[61] = ui_vars.ui8_coast_brake_enable;
    tx_data[62] = ui_vars.ui8_coast_brake_adc;

    tx_data[63] = (uint8_t)(ui_vars.ui16_battery_voltage_reset_wh_counter_x10 & 0xff);
    tx_data[64] = (uint8_t)(ui_vars.ui16_battery_voltage_reset_wh_counter_x10 >> 8);

    tx_data[65] = ui_vars.ui8_system_power_off_time_minutes;

    tx_data[66] = (uint8_t)(ui_vars.ui16_battery_pack_resistance_x1000 & 0xff);
    tx_data[67] = (uint8_t)(ui_vars.ui16_battery_pack_resistance_x1000 >> 8);

    tx_data[68] = (uint8_t)(ui_vars.ui32_odometer_x10 & 0xff);
    tx_data[69] = (uint8_t)((ui_vars.ui32_odometer_x10 >> 8) & 0xff);
    tx_data[70] = (uint8_t)((ui_vars.ui32_odometer_x10 >> 16) & 0xff);
    tx_data[71] = (uint8_t)((ui_vars.ui32_odometer_x10 >> 24) & 0xff);

    tx_data[72] = ui_vars.ui8_walk_assist_feature_enabled;

    tx_data[73] = ui_vars.ui8_walk_assist_level_factor[0];
    tx_data[74] = ui_vars.ui8_walk_assist_level_factor[1];
    tx_data[75] = ui_vars.ui8_walk_assist_level_factor[2];
    tx_data[76] = ui_vars.ui8_walk_assist_level_factor[3];
    tx_data[77] = ui_vars.ui8_walk_assist_level_factor[4];
    tx_data[78] = ui_vars.ui8_walk_assist_level_factor[5];
    tx_data[79] = ui_vars.ui8_walk_assist_level_factor[6];

    tx_data[80] = ui_vars.ui8_torque_sensor_calibration_feature_enabled;
    tx_data[81] = ui_vars.ui8_torque_sensor_calibration_pedal_ground;
    tx_data[82] = ui_vars.ui8_torque_sensor_filter;
    tx_data[83] = ui_vars.ui8_torque_sensor_adc_threshold;

    tx_data[84] = (uint8_t)(ui_vars.ui16_torque_sensor_calibration_table_left[0][0] & 0xff);
    tx_data[85] = (uint8_t)(ui_vars.ui16_torque_sensor_calibration_table_left[0][0] >> 8);
    tx_data[86] = (uint8_t)(ui_vars.ui16_torque_sensor_calibration_table_left[0][1] & 0xff);
    tx_data[87] = (uint8_t)(ui_vars.ui16_torque_sensor_calibration_table_left[0][1] >> 8);

    tx_data[88] = (uint8_t)(ui_vars.ui16_torque_sensor_calibration_table_left[1][0] & 0xff);
    tx_data[89] = (uint8_t)(ui_vars.ui16_torque_sensor_calibration_table_left[1][0] >> 8);
    tx_data[90] = (uint8_t)(ui_vars.ui16_torque_sensor_calibration_table_left[1][1] & 0xff);
    tx_data[91] = (uint8_t)(ui_vars.ui16_torque_sensor_calibration_table_left[1][1] >> 8);

    tx_data[92] = (uint8_t)(ui_vars.ui16_torque_sensor_calibration_table_left[2][0] & 0xff);
    tx_data[93] = (uint8_t)(ui_vars.ui16_torque_sensor_calibration_table_left[2][0] >> 8);
    tx_data[94] = (uint8_t)(ui_vars.ui16_torque_sensor_calibration_table_left[2][1] & 0xff);
    tx_data[95] = (uint8_t)(ui_vars.ui16_torque_sensor_calibration_table_left[2][1] >> 8);

    tx_data[96] = (uint8_t)(ui_vars.ui16_torque_sensor_calibration_table_left[3][0] & 0xff);
    tx_data[97] = (uint8_t)(ui_vars.ui16_torque_sensor_calibration_table_left[3][0] >> 8);
    tx_data[98] = (uint8_t)(ui_vars.ui16_torque_sensor_calibration_table_left[3][1] & 0xff);
    tx_data[99] = (uint8_t)(ui_vars.ui16_torque_sensor_calibration_table_left[3][1] >> 8);

    tx_data[100] = (uint8_t)(ui_vars.ui16_torque_sensor_calibration_table_left[4][0] & 0xff);
    tx_data[101] = (uint8_t)(ui_vars.ui16_torque_sensor_calibration_table_left[4][0] >> 8);
    tx_data[102] = (uint8_t)(ui_vars.ui16_torque_sensor_calibration_table_left[4][1] & 0xff);
    tx_data[103] = (uint8_t)(ui_vars.ui16_torque_sensor_calibration_table_left[4][1] >> 8);

    tx_data[104] = (uint8_t)(ui_vars.ui16_torque_sensor_calibration_table_left[5][0] & 0xff);
    tx_data[105] = (uint8_t)(ui_vars.ui16_torque_sensor_calibration_table_left[5][0] >> 8);
    tx_data[106] = (uint8_t)(ui_vars.ui16_torque_sensor_calibration_table_left[5][1] & 0xff);
    tx_data[107] = (uint8_t)(ui_vars.ui16_torque_sensor_calibration_table_left[5][1] >> 8);

    tx_data[108] = (uint8_t)(ui_vars.ui16_torque_sensor_calibration_table_left[6][0] & 0xff);
    tx_data[109] = (uint8_t)(ui_vars.ui16_torque_sensor_calibration_table_left[6][0] >> 8);
    tx_data[110] = (uint8_t)(ui_vars.ui16_torque_sensor_calibration_table_left[6][1] & 0xff);
    tx_data[111] = (uint8_t)(ui_vars.ui16_torque_sensor_calibration_table_left[6][1] >> 8);

    tx_data[112] = (uint8_t)(ui_vars.ui16_torque_sensor_calibration_table_left[7][0] & 0xff);
    tx_data[113] = (uint8_t)(ui_vars.ui16_torque_sensor_calibration_table_left[7][0] >> 8);
    tx_data[114] = (uint8_t)(ui_vars.ui16_torque_sensor_calibration_table_left[7][1] & 0xff);
    tx_data[115] = (uint8_t)(ui_vars.ui16_torque_sensor_calibration_table_left[7][1] >> 8);

    tx_data[116] = (uint8_t)(ui_vars.ui16_torque_sensor_calibration_table_right[0][0] & 0xff);
    tx_data[117] = (uint8_t)(ui_vars.ui16_torque_sensor_calibration_table_right[0][0] >> 8);
    tx_data[118] = (uint8_t)(ui_vars.ui16_torque_sensor_calibration_table_right[0][1] & 0xff);
    tx_data[119] = (uint8_t)(ui_vars.ui16_torque_sensor_calibration_table_right[0][1] >> 8);

    tx_data[120] = (uint8_t)(ui_vars.ui16_torque_sensor_calibration_table_right[1][0] & 0xff);
    tx_data[121] = (uint8_t)(ui_vars.ui16_torque_sensor_calibration_table_right[1][0] >> 8);
    tx_data[122] = (uint8_t)(ui_vars.ui16_torque_sensor_calibration_table_right[1][1] & 0xff);
    tx_data[123] = (uint8_t)(ui_vars.ui16_torque_sensor_calibration_table_right[1][1] >> 8);

    tx_data[124] = (uint8_t)(ui_vars.ui16_torque_sensor_calibration_table_right[2][0] & 0xff);
    tx_data[125] = (uint8_t)(ui_vars.ui16_torque_sensor_calibration_table_right[2][0] >> 8);
    tx_data[126] = (uint8_t)(ui_vars.ui16_torque_sensor_calibration_table_right[2][1] & 0xff);
    tx_data[127] = (uint8_t)(ui_vars.ui16_torque_sensor_calibration_table_right[2][1] >> 8);

    tx_data[128] = (uint8_t)(ui_vars.ui16_torque_sensor_calibration_table_right[3][0] & 0xff);
    tx_data[129] = (uint8_t)(ui_vars.ui16_torque_sensor_calibration_table_right[3][0] >> 8);
    tx_data[130] = (uint8_t)(ui_vars.ui16_torque_sensor_calibration_table_right[3][1] & 0xff);
    tx_data[131] = (uint8_t)(ui_vars.ui16_torque_sensor_calibration_table_right[3][1] >> 8);

    tx_data[132] = (uint8_t)(ui_vars.ui16_torque_sensor_calibration_table_right[4][0] & 0xff);
    tx_data[133] = (uint8_t)(ui_vars.ui16_torque_sensor_calibration_table_right[4][0] >> 8);
    tx_data[134] = (uint8_t)(ui_vars.ui16_torque_sensor_calibration_table_right[4][1] & 0xff);
    tx_data[135] = (uint8_t)(ui_vars.ui16_torque_sensor_calibration_table_right[4][1] >> 8);

    tx_data[136] = (uint8_t)(ui_vars.ui16_torque_sensor_calibration_table_right[5][0] & 0xff);
    tx_data[137] = (uint8_t)(ui_vars.ui16_torque_sensor_calibration_table_right[5][0] >> 8);
    tx_data[138] = (uint8_t)(ui_vars.ui16_torque_sensor_calibration_table_right[5][1] & 0xff);
    tx_data[139] = (uint8_t)(ui_vars.ui16_torque_sensor_calibration_table_right[5][1] >> 8);

    tx_data[140] = (uint8_t)(ui_vars.ui16_torque_sensor_calibration_table_right[6][0] & 0xff);
    tx_data[141] = (uint8_t)(ui_vars.ui16_torque_sensor_calibration_table_right[6][0] >> 8);
    tx_data[142] = (uint8_t)(ui_vars.ui16_torque_sensor_calibration_table_right[6][1] & 0xff);
    tx_data[143] = (uint8_t)(ui_vars.ui16_torque_sensor_calibration_table_right[6][1] >> 8);

    tx_data[144] = (uint8_t)(ui_vars.ui16_torque_sensor_calibration_table_right[7][0] & 0xff);
    tx_data[145] = (uint8_t)(ui_vars.ui16_torque_sensor_calibration_table_right[7][0] >> 8);
    tx_data[146] = (uint8_t)(ui_vars.ui16_torque_sensor_calibration_table_right[7][1] & 0xff);
    tx_data[147] = (uint8_t)(ui_vars.ui16_torque_sensor_calibration_table_right[7][1] >> 8);

    tx_data[148] = ui_vars.ui8_street_mode_function_enabled;
    tx_data[149] = ui_vars.ui8_street_mode_enabled;
    tx_data[150] = ui_vars.ui8_street_mode_enabled_on_startup;
    tx_data[151] = ui_vars.ui8_street_mode_speed_limit;
    tx_data[152] = ui_vars.ui8_street_mode_power_limit_div25;
    tx_data[153] = ui_vars.ui8_street_mode_throttle_enabled;
    tx_data[154] = ui_vars.ui8_street_mode_hotkey_enabled;
    tx_data[155] = ui_vars.ui8_pedal_cadence_fast_stop;
    tx_data[156] = ui_vars.ui8_throttle_virtual_step;
    tx_data[157] = ui_vars.ui8_street_mode_enabled;
    tx_data[158] = ui_vars.ui8_ant_device_id;

    ret_code_t err_code;
    err_code = ble_tsdz2_configurations_on_change(m_conn_handle, &m_ble_tsdz2_service, tx_data);
    if (err_code == NRF_SUCCESS)
    {
    }
  }
}

void TSDZ2_power_manage(void)
{
  static uint8_t counter;

  switch (m_TSDZ2_power_state)
  {
  case TSDZ2_POWER_STATE_OFF_START:
    motor_power_enable(false);
    counter = 10;
    m_TSDZ2_power_state = TSDZ2_POWER_STATE_OFF_WAIT;
    break;

  case TSDZ2_POWER_STATE_OFF_WAIT:
    counter--;
    if (counter == 0)
    {
      // reset state variables
      uart_reset_rx_buffer();
      g_motor_init_state = MOTOR_INIT_OFF;
      g_motor_init_state_conf = MOTOR_INIT_CONFIG_SEND_CONFIG;
      ui8_g_motor_init_status = MOTOR_INIT_STATUS_RESET;

      m_TSDZ2_power_state = TSDZ2_POWER_STATE_OFF;
    }
    break;

  case TSDZ2_POWER_STATE_OFF:
    // do nothing
    break;

  case TSDZ2_POWER_STATE_ON_START:
    motor_power_enable(true);
    g_motor_init_state = MOTOR_INIT_GET_MOTOR_ALIVE;
    m_TSDZ2_power_state = TSDZ2_POWER_STATE_ON;
    break;

  case TSDZ2_POWER_STATE_ON:
    // do nothing
    break;
  }
}

// Allow common operations (like walk assist and headlights) button presses to work on any page
bool anyscreen_onpress(buttons_events_t events) {
  if ((events & DOWN_LONG_CLICK) && ui_vars.ui8_walk_assist_feature_enabled) {
    ui_vars.ui8_walk_assist = 1;
    return true;
  }

  // long up to turn on headlights
  if (events & UP_LONG_CLICK) {
    ui_vars.ui8_lights = !ui_vars.ui8_lights;
    //set_lcd_backlight();

    return true;
  }

  return false;
}

static bool onPressAlternateField(buttons_events_t events) {
  bool handled = false;

  // start increment throttle only with UP_LONG_CLICK
  if ((ui8_m_alternate_field_state == 7) &&
      (events & UP_LONG_CLICK) &&
      (ui8_m_vthrottle_can_increment_decrement == 0)) {
    ui8_m_vthrottle_can_increment_decrement = 1;
    events |= UP_CLICK; // let's increment, consider UP CLICK
    ui8_m_alternate_field_timeout_cnt = 50; // 50 * 20ms = 1 second
  }

  if (ui8_m_alternate_field_timeout_cnt == 0) {
    ui_vars.ui8_throttle_virtual = 0;
    ui8_m_vthrottle_can_increment_decrement = 0;
  }

  switch (ui8_m_alternate_field_state) {
    case 0:
      if (events & SCREENCLICK_ALTERNATE_FIELD_START) {
        ui8_m_alternate_field_state = 1;
        handled = true;
      }
      break;

    // max power
    case 3:
      if (
        (
          ui_vars.ui8_street_mode_function_enabled
          && ui_vars.ui8_street_mode_enabled
          && ui_vars.ui8_street_mode_throttle_enabled
          || !ui_vars.ui8_street_mode_function_enabled
          || !ui_vars.ui8_street_mode_enabled
        )
        && events & SCREENCLICK_ALTERNATE_FIELD_START
      ) {
        ui8_m_alternate_field_state = 6;
        handled = true;
        break;
      }

      if (events & SCREENCLICK_ALTERNATE_FIELD_STOP) {
        ui8_m_alternate_field_state = 4;
        handled = true;
        break;
      }

      if (events & UP_CLICK) {
        handled = true;

        if (ui_vars.ui8_target_max_battery_power_div25 < 10) {
          ui_vars.ui8_target_max_battery_power_div25++;
        } else {
          ui_vars.ui8_target_max_battery_power_div25 += 2;
        }

        // limit to 100 * 25 = 2500 Watts
        if(ui_vars.ui8_target_max_battery_power_div25 > 100) {
          ui_vars.ui8_target_max_battery_power_div25 = 100;
        }

        break;
      }

      if (events & DOWN_CLICK) {
        handled = true;

        if (ui_vars.ui8_target_max_battery_power_div25 <= 10 &&
            ui_vars.ui8_target_max_battery_power_div25 > 1) {
          ui_vars.ui8_target_max_battery_power_div25--;
        } else if (ui_vars.ui8_target_max_battery_power_div25 > 10) {
          ui_vars.ui8_target_max_battery_power_div25 -= 2;
        }

        break;
      }
    break;

    // virtual throttle
    // From what I can see - for VT to work- you need street mode enabled, you need throttle enabled for street mode.
    // up+onoff long press then gets you into alternate state 3
    // up+onoff long press 2nd time then gets you into alternate state 7
    // onoff long press seems to get you out of alternate state (back to 0) and back to normal assist
    // Having said all that - i cannot get the motor to spin when in either 3 or 7 state - 
    // not sure if this lack of something implemented here - or that I'm not activating VT properly.
    case 7:
      if (events & SCREENCLICK_ALTERNATE_FIELD_START) {
        ui8_m_alternate_field_state = 1;
        handled = true;
        break;
      }

      if (events & SCREENCLICK_ALTERNATE_FIELD_STOP) {
        ui_vars.ui8_throttle_virtual = 0;
        ui8_m_alternate_field_timeout_cnt = 0;
        ui8_m_vthrottle_can_increment_decrement = 0;
        ui8_m_alternate_field_state = 4;
        handled = true;
        break;
      }

      if (events & UP_CLICK) {
        handled = true;

        if (ui8_m_vthrottle_can_increment_decrement &&
            ui_vars.ui8_assist_level) {
          if ((ui_vars.ui8_throttle_virtual + ui_vars.ui8_throttle_virtual_step) <= 100) {
            ui_vars.ui8_throttle_virtual += ui_vars.ui8_throttle_virtual_step;
          } else {
            ui_vars.ui8_throttle_virtual = 100;
          }

          ui8_m_alternate_field_timeout_cnt = 50;
        }

        break;
      }

      if (events & DOWN_CLICK) {
        handled = true;

        if (ui8_m_vthrottle_can_increment_decrement &&
            ui_vars.ui8_assist_level) {
          if (ui_vars.ui8_throttle_virtual >= ui_vars.ui8_throttle_virtual_step) {
            ui_vars.ui8_throttle_virtual -= ui_vars.ui8_throttle_virtual_step;
          } else {
            ui_vars.ui8_throttle_virtual = 0;
          }

          ui8_m_alternate_field_timeout_cnt = 50;
        }

        break;
      }
    break;
  }

  if (ui8_m_alternate_field_state == 7) {
    // user will keep UP DOWN LONG clicks on this state, so, clean them to not pass to next code
    if ((events & UP_LONG_CLICK) ||
        (events & DOWN_LONG_CLICK))
      handled = true;
  }

  return handled;
}

static bool onPressStreetMode(buttons_events_t events) {
  bool handled = false;

  if (events & SCREENCLICK_STREET_MODE)
  {
    if (ui_vars.ui8_street_mode_function_enabled && ui_vars.ui8_street_mode_hotkey_enabled)
    {
      if (ui_vars.ui8_street_mode_enabled)
        ui_vars.ui8_street_mode_enabled = 0;
      else
        ui_vars.ui8_street_mode_enabled = 1;

      //mainScreenOnDirtyClean();
    }

    handled = true;
  }

  return handled;
}

bool mainScreenOnPress(buttons_events_t events) {
  bool handled = false;

  handled = onPressAlternateField(events);

  if (handled == false)
    handled = anyscreen_onpress(events);

  if (handled == false)
    handled = onPressStreetMode(events);

  if (handled == false &&
      ui8_m_alternate_field_state == 0) {
    if (events & UP_CLICK) {
      ui_vars.ui8_assist_level++;

      if (ui_vars.ui8_assist_level > ui_vars.ui8_number_of_assist_levels) {
        ui_vars.ui8_assist_level = ui_vars.ui8_number_of_assist_levels;
      }

      m_assist_level_change_timeout = 20; // 2 seconds
      handled = true;
    }

    if (
      events & DOWN_CLICK
      && !ui_vars.ui8_walk_assist // do not lower assist level if walk assist is active
    ) {
      if (ui_vars.ui8_assist_level > 0)
        ui_vars.ui8_assist_level--;

      m_assist_level_change_timeout = 20; // 2 seconds
      handled = true;
    }
  }

	return handled;
}

void alternatField(void) {
  //static const char str_max_power[] = "max power";
  //static const char str_throttle[] = "throttle";

  switch (ui8_m_alternate_field_state) {
    case 1:
// #ifndef SW102
//       assistLevelField.rw->visibility = FieldTransitionNotVisible;
// #else
//       wheelSpeedIntegerField.rw->visibility = FieldTransitionNotVisible;
// #endif
      ui8_m_alternate_field_state = 2;

// #ifndef SW102
//       UG_SetBackcolor(C_BLACK);
//       UG_SetForecolor(MAIN_SCREEN_FIELD_LABELS_COLOR);
//       UG_FontSelect(&FONT_10X16);
//       UG_PutString(15, 46, "      ");
// #endif
      break;

    case 2:
      //updateReadOnlyLabelStr(&fieldAlternate, str_max_power);
      //fieldAlternate.rw->visibility = FieldTransitionVisible;
      //mainScreenOnDirtyClean();
      ui8_m_alternate_field_state = 3;
      break;

    case 3:
      // keep updating the variable to show on display
      ui16_m_alternate_field_value = ((uint16_t) ui_vars.ui8_target_max_battery_power_div25) * 25;
      break;

    case 4:
      //fieldAlternate.rw->visibility = FieldTransitionNotVisible;
      ui8_m_alternate_field_state = 5;
      break;

    case 5:
// #ifndef SW102
//       assistLevelField.rw->visibility = FieldTransitionVisible;
// #else
//       wheelSpeedIntegerField.rw->visibility = FieldTransitionVisible;
// #endif
      //mainScreenOnDirtyClean();
      ui8_m_alternate_field_state = 0;
      break;

    case 6:
      //updateReadOnlyLabelStr(&fieldAlternate, str_throttle);
      //mainScreenOnDirtyClean();
      ui8_m_alternate_field_state = 7;
      break;

    case 7:
      // keep updating the variable to show on display
      ui16_m_alternate_field_value = (uint16_t) ui_vars.ui8_throttle_virtual;
      break;
  }
}

void streetMode(void) {
  if (ui_vars.ui8_street_mode_function_enabled)
  {
    ui_vars.ui8_street_mode_power_limit_div25 = (ui_vars.ui16_street_mode_power_limit / 25);
  }
}

void walk_assist_state(void) {
// kevinh - note on the sw102 we show WALK in the box normally used for BRAKE display - the display code is handled there now
  if (ui_vars.ui8_walk_assist_feature_enabled) {
    // if down button is still pressed
    if (ui_vars.ui8_walk_assist && buttons_get_down_state()) {
      ui8_walk_assist_timeout = 2; // 0.2 seconds
    } else if (buttons_get_down_state() == 0 && --ui8_walk_assist_timeout == 0) {
      ui_vars.ui8_walk_assist = 0;
    }
  } else {
    ui_vars.ui8_walk_assist = 0;
  }
}

static bool appwide_onpress(buttons_events_t events)
{
  // power off only after we release first time the onoff button
  if (events & ONOFF_LONG_CLICK)
  {
    // Toggle power state...
          if (m_TSDZ2_power_state == TSDZ2_POWER_STATE_OFF)
      {
        // turn on TSDZ2 motor controller
        m_TSDZ2_power_state = TSDZ2_POWER_STATE_ON_START;
      }

      else if (m_TSDZ2_power_state == TSDZ2_POWER_STATE_ON)
      {
        //  turn off TSDZ2 motor controller
        m_TSDZ2_power_state = TSDZ2_POWER_STATE_OFF_START;
      }
    return true;
  }

/*   if ((events & SCREENCLICK_NEXT_SCREEN) &&
      ((g_motor_init_state == MOTOR_INIT_READY) ||
      (g_motor_init_state == MOTOR_INIT_SIMULATING))) {
    showNextScreen();
    return true;
  }

  if (events & SCREENCLICK_ENTER_CONFIGURATIONS) {
    screenShow(&configScreen);
    return true;
  } */

	return false;
}


/// Called every 20ms to check for wired button events and dispatch to our handlers
static void handle_buttons() {

  static uint8_t firstTime = 1;

  // keep tracking of first time release of onoff button
  if(firstTime && buttons_get_onoff_state() == 0) {
    firstTime = 0;
    buttons_clear_onoff_click_event();
    buttons_clear_onoff_long_click_event();
    buttons_clear_onoff_click_long_click_event();
  }

   if (ui8_m_alternate_field_state == 7) { // if virtual throttle mode
    if (buttons_get_up_state() == 0 && // UP and DOWN buttons not pressed
            buttons_get_down_state() == 0) {
      if (ui8_m_alternate_field_timeout_cnt) {
        ui8_m_alternate_field_timeout_cnt--;
      } else {
        ui8_m_vthrottle_can_increment_decrement = 0;
        ui_vars.ui8_throttle_virtual = 0;
      }
    } else {
      ui8_m_alternate_field_timeout_cnt = 50;
    }
  }  

  if (buttons_events && firstTime == 0)
  {
    bool handled = false;

		//if (!handled)
		//	handled |= screenOnPress(buttons_events);
		if (!handled)
			handled |= mainScreenOnPress(buttons_events);
		 //Note: this must be after the screen/menu handlers have had their shot
		 if (!handled)
		 	handled |= appwide_onpress(buttons_events);

		if (handled)
			buttons_clear_all_events();
	}

	buttons_clock(); // Note: this is done _after_ button events is checked to provide a 20ms debounce
}

void led_pwm_on(uint32_t mask, uint8_t duty_cycle_max, uint8_t duty_cycle_min, uint8_t duty_cycle_step, uint32_t led_on_ms)
{
  //mask can be ORed to turn on R &B colors
  //ie: R_LED || B_LED
  //not G_LED as it is on a diffderent gpio port
  ret_code_t err_code;
  NRF_GPIO_Type *port;
  uint32_t ON_TICKS = 0;
  ON_TICKS = APP_TIMER_TICKS(led_on_ms);

  if (soft_blink == 0) //ok to start another pwm instance
  {

    //fix for port number problem with green led
    port = NRF_P0;
    if (mask == G_LED)
      port = NRF_P1;

#define LED_PWM_PARAMS(mask)                             \
  {                                                      \
    .active_high = false,                                \
    .duty_cycle_max = duty_cycle_max,                    \
    .duty_cycle_min = duty_cycle_min,                    \
    .duty_cycle_step = duty_cycle_step,                  \
    .off_time_ticks = 3000,                              \
    .on_time_ticks = 0,                                  \
    .leds_pin_bm = LED_SB_INIT_PARAMS_LEDS_PIN_BM(mask), \
    .p_leds_port = port                                  \
  }
    const led_sb_init_params_t led_pwm_init_param = LED_PWM_PARAMS(mask);

    err_code = led_softblink_init(&led_pwm_init_param);
    APP_ERROR_CHECK(err_code);

    if (led_on_ms)
    {
      err_code = app_timer_start(led_timer, ON_TICKS, NULL);
      APP_ERROR_CHECK(err_code);
    }
    err_code = led_softblink_start(mask);
    APP_ERROR_CHECK(err_code);
    soft_blink = 1; //set the blocking flag
  }
}

void disp_soc(void)
{
  nrf_delay_ms(1000);
  for (int i = 0; i < (ui8_g_battery_soc/10); i++)
  {
    led_pwm_on(G_LED, 100, 0, 5, 0);
    nrf_delay_ms(200);
    soft_blink = led_softblink_uninit(); // turn off the soft_blink led
    nrf_delay_ms(300);
  }
}

int main(void)
{
  mp_ui_vars = get_ui_vars();
  // Initialize the async SVCI interface to bootloader before any interrupts are enabled.
  pins_init();
  lfclk_config(); // needed by the APP_TIMER
  init_app_timers();
  eeprom_init();

  //below is what I had to do to get NVIC_SystemReset() not to hangup.
  //the s340 sd if what is preventing the restart into the bootloader, so it is important to reset before bluetooth starts.
  //basically, the user changes the ANT_ID to 0x99 , the firmware reboots as normal and we catch the change here before bluetooth starts
  //it now works in debug mode!
  if (mp_ui_vars->ui8_enter_bootloader) //check to see if reboot into the bootloader is needed
  {
    mp_ui_vars->ui8_enter_bootloader = 0;
    nrf_power_gpregret_set(BOOTLOADER_DFU_START); //set the dfu register
    nrf_delay_ms(1000);                           //wait for write to complete
    eeprom_write_variables();
    nrf_delay_ms(3000); //wait for write to complete
    NVIC_SystemReset(); //reboot into bootloader
  }

  ble_init();
  ant_setup();
  uart_init();

  // setup this member variable ui8_m_ant_device_id
  ui8_m_ant_device_id = mp_ui_vars->ui8_ant_device_id;

  while (1)
  {
    // every 50 ms
    if (main_ticks % (50 / MSEC_PER_TICK) == 0)
    {
      // exchange data from realtime layer to UI layer
      // do this in atomic way, disabling the real time layer (should be no problem as
      // copy_rt_to_ui_vars() should be fast and take a small piece of the 50ms periodic realtime layer processing
      rt_processing_stop();
      copy_rt_ui_vars();
      rt_processing_start();

      ble_send_periodic_data();
      ble_update_configurations_data();
      TSDZ2_power_manage();

      walk_assist_state();
      handle_buttons();
      //alternatField(); // Removed until we can resolve what to do with the alternate state display requirements
      streetMode();
    }

    // every 1 second
    if (main_ticks % (1000 / MSEC_PER_TICK) == 0)
    {
      //see if DFU reboot is needed

      // see if there was a change to the ANT ID
      if (ui8_m_ant_device_id != mp_ui_vars->ui8_ant_device_id)
      {
        mp_ui_vars->ui8_ant_device_id = ui8_m_ant_device_id;
        eeprom_write_variables_and_reset();
      }

      // see if there a request to enter in bootloader
      if (ui8_m_enter_bootloader)
      {
        ui8_m_enter_bootloader = 0;
        mp_ui_vars->ui8_enter_bootloader = 1;
        eeprom_write_variables_and_reset();
      }

      // see if there was a change to the ANT ID
      if (ui8_m_flash_configurations)
      {
        ui8_m_flash_configurations = 0;
        eeprom_write_variables();
      }
    }
  }
}
