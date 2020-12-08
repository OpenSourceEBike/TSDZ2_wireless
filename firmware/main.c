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
#include "ble_project.h"

extern uint8_t ui8_g_battery_soc;
ui_vars_t *gp_ui_vars;

volatile uint8_t ui8_g_enter_bootloader = 0;
volatile uint8_t ui8_g_ant_device_id = 0;
volatile uint8_t ui8_m_flash_configurations = 0;

static uint16_t m_ble_conn_handle;

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

void ant_lev_evt_handler_pre(ant_lev_profile_t *p_profile, ant_lev_evt_t event)
{
  nrf_pwr_mgmt_feed();

  switch (event)
  {
  case ANT_LEV_PAGE_1_UPDATED:
    // assist level
    p_profile->common.travel_mode_state |= (gp_ui_vars->ui8_assist_level << 3) & 0x38;

    // lights
    p_profile->common.system_state |= (gp_ui_vars->ui8_lights << 3) & 0x08;
    break;

  case ANT_LEV_PAGE_2_UPDATED:
    break;

  case ANT_LEV_PAGE_3_UPDATED:
    // assist level
    p_profile->common.travel_mode_state |= (gp_ui_vars->ui8_assist_level << 3) & 0x38;

    // lights
    p_profile->common.system_state |= (gp_ui_vars->ui8_lights << 3) & 0x08;
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
    // assist level
    p_profile->common.travel_mode_state = p_profile->page_16.travel_mode;
    gp_ui_vars->ui8_assist_level = p_profile->page_16.travel_mode >> 3;

    // lights
    p_profile->common.system_state |= ((((uint8_t)p_profile->page_16.light) << 3) & 0x08);
    gp_ui_vars->ui8_lights = ((uint8_t)p_profile->page_16.light);
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

void tsdz2_write_handler_periodic(uint8_t *p_data, uint16_t len)
{
  ui_vars.ui8_assist_level = p_data[0];
}

void tsdz2_write_handler_configurations(uint8_t *p_data, uint16_t len)
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
  tx_data[5] = ui_vars.ui8_braking;
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
  

  if (m_ble_conn_handle != BLE_CONN_HANDLE_INVALID)
  {
    ret_code_t err_code;
    err_code = ble_tsdz2_periodic_on_change(m_ble_conn_handle, &m_ble_tsdz2_service, tx_data);
    if (err_code == NRF_SUCCESS)
    {
    }
  }
}

void ble_update_configurations_data(void)
{
  uint8_t tx_data[BLE_TSDZ2_CONFIGURATIONS_LEN];

  if (m_ble_conn_handle != BLE_CONN_HANDLE_INVALID)
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
    err_code = ble_tsdz2_configurations_on_change(m_ble_conn_handle, &m_ble_tsdz2_service, tx_data);
    if (err_code == NRF_SUCCESS)
    {
    }
  }
}

int main(void)
{
  gp_ui_vars = get_ui_vars();
  // Initialize the async SVCI interface to bootloader before any interrupts are enabled.
  pins_init();
  motor_power_enable(true);
  lfclk_config(); // needed by the APP_TIMER
  init_app_timers();
  eeprom_init();

  //below is what I had to do to get NVIC_SystemReset() not to hangup.
  //the s340 sd if what is preventing the restart into the bootloader, so it is important to reset before bluetooth starts.
  //basically, the user changes the ANT_ID to 0x99 , the firmware reboots as normal and we catch the change here before bluetooth starts
  //it now works in debug mode!
  if (gp_ui_vars->ui8_enter_bootloader) //check to see if reboot into the bootloader is needed
  {
    gp_ui_vars->ui8_enter_bootloader = 0;
    nrf_power_gpregret_set(BOOTLOADER_DFU_START); //set the dfu register
    nrf_delay_ms(1000);                           //wait for write to complete
    eeprom_write_variables();
    nrf_delay_ms(3000); //wait for write to complete
    NVIC_SystemReset(); //reboot into bootloader
  }
  
  m_ble_conn_handle = ble_init();
  ant_setup();
  uart_init();

  // setup this global variable ui8_g_ant_device_id
  ui8_g_ant_device_id = gp_ui_vars->ui8_ant_device_id;

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
    }

    // every 1 second
    if (main_ticks % (1000 / MSEC_PER_TICK) == 0)
    {
      //see if DFU reboot is needed

      // see if there was a change to the ANT ID
      if (ui8_g_ant_device_id != gp_ui_vars->ui8_ant_device_id)
      {
        gp_ui_vars->ui8_ant_device_id = ui8_g_ant_device_id;
        eeprom_write_variables_and_reset();
      }

      // see if there a request to enter in bootloader
      if (ui8_g_enter_bootloader)
      {
        ui8_g_enter_bootloader = 0;
        gp_ui_vars->ui8_enter_bootloader = 1;
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
