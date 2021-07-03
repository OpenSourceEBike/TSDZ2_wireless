/*
 * Bafang LCD 850C firmware
 *
 * Copyright (C) Casainho, 2018.
 *
 * Released under the GPL License, Version 3
 */

#include "stdio.h"
#include <string.h>
#include "eeprom.h"
//#include "main_screen.h"
#include "state.h"
#include "nrf_sdh.h"
#include "nrf_sdh_ble.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "app_error.h"
#include "fds.h"

#define CONFIG_FILE     (0x8010)
#define CONFIG_REC_KEY  (0x7010)

fds_record_desc_t m_desc_config = {0};
fds_find_token_t  m_tok_config  = {0};

static configurations_t m_configurations;

// get rid of some copypasta with this little wrapper for copying arrays between structs
#define COPY_ARRAY(dest, src, field) memcpy((dest)->field, (src)->field, sizeof((dest)->field))

const configurations_t m_configurations_defaults = {
  .ui8_configurations_version = CONFIGURATIONS_VERSION,
  .ui8_assist_level = DEFAULT_VALUE_ASSIST_LEVEL,
  .ui16_wheel_perimeter = DEFAULT_VALUE_WHEEL_PERIMETER,
  .ui8_wheel_max_speed = DEFAULT_VALUE_WHEEL_MAX_SPEED,
  .ui8_units_type = DEFAULT_VALUE_UNITS_TYPE,
  .ui32_wh_x10_offset = DEFAULT_VALUE_WH_X10_OFFSET,
  .ui32_wh_x10_100_percent = DEFAULT_VALUE_HW_X10_100_PERCENT,
  .ui8_battery_max_current = DEFAULT_VALUE_BATTERY_MAX_CURRENT,
  .ui8_target_max_battery_power_div25 = DEFAULT_VALUE_TARGET_MAX_BATTERY_POWER,
  .ui8_motor_max_current = DEFAULT_VALUE_MOTOR_MAX_CURRENT,
  .ui8_motor_current_min_adc = DEFAULT_VALUE_CURRENT_MIN_ADC,
  .ui8_field_weakening = DEFAULT_FIELD_WEAKENING,
  .ui8_ramp_up_amps_per_second_x10 = DEFAULT_VALUE_RAMP_UP_AMPS_PER_SECOND_X10,
  .ui16_battery_low_voltage_cut_off_x10 = DEFAULT_VALUE_BATTERY_LOW_VOLTAGE_CUT_OFF_X10,
  .ui8_motor_type = DEFAULT_VALUE_MOTOR_TYPE,
  .ui8_motor_current_control_mode = DEFAULT_VALUE_MOTOR_CURRENT_CONTROL_MODE,
  .ui8_motor_assistance_startup_without_pedal_rotation = DEFAULT_VALUE_MOTOR_ASSISTANCE_WITHOUT_PEDAL_ROTATION,
  .ui16_assist_level_factor = {
  DEFAULT_VALUE_ASSIST_LEVEL_FACTOR_1,
  DEFAULT_VALUE_ASSIST_LEVEL_FACTOR_2,
  DEFAULT_VALUE_ASSIST_LEVEL_FACTOR_3,
  DEFAULT_VALUE_ASSIST_LEVEL_FACTOR_4,
  DEFAULT_VALUE_ASSIST_LEVEL_FACTOR_5,
  DEFAULT_VALUE_ASSIST_LEVEL_FACTOR_6,
  DEFAULT_VALUE_ASSIST_LEVEL_FACTOR_7,
  },
  .ui8_number_of_assist_levels = DEFAULT_VALUE_NUMBER_OF_ASSIST_LEVELS,
  .ui8_startup_motor_power_boost_feature_enabled = DEFAULT_VALUE_STARTUP_MOTOR_POWER_BOOST_FEATURE_ENABLED,
  .ui8_startup_motor_power_boost_always = DEFAULT_VALUE_STARTUP_MOTOR_POWER_BOOST_ALWAYS,
  .ui16_startup_motor_power_boost_factor = {
  DEFAULT_VALUE_ASSIST_LEVEL_FACTOR_1,
  DEFAULT_VALUE_ASSIST_LEVEL_FACTOR_2,
  DEFAULT_VALUE_ASSIST_LEVEL_FACTOR_3,
  DEFAULT_VALUE_ASSIST_LEVEL_FACTOR_4,
  DEFAULT_VALUE_ASSIST_LEVEL_FACTOR_5,
  DEFAULT_VALUE_ASSIST_LEVEL_FACTOR_6,
  DEFAULT_VALUE_ASSIST_LEVEL_FACTOR_7,
  },
  .ui8_startup_motor_power_boost_time =
  DEFAULT_VALUE_STARTUP_MOTOR_POWER_BOOST_TIME,
  .ui8_startup_motor_power_boost_fade_time =
  DEFAULT_VALUE_STARTUP_MOTOR_POWER_BOOST_FADE_TIME,
  .ui8_temperature_limit_feature_enabled =
  DEFAULT_VALUE_MOTOR_TEMPERATURE_FEATURE_ENABLE,
  .ui8_motor_temperature_min_value_to_limit =
  DEFAULT_VALUE_MOTOR_TEMPERATURE_MIN_VALUE_LIMIT,
  .ui8_motor_temperature_max_value_to_limit =
  DEFAULT_VALUE_MOTOR_TEMPERATURE_MAX_VALUE_LIMIT,
  .ui16_battery_voltage_reset_wh_counter_x10 =
  DEFAULT_VALUE_BATTERY_VOLTAGE_RESET_WH_COUNTER_X10,
  .ui8_system_power_off_time_minutes =
  DEFAULT_VALUE_LCD_POWER_OFF_TIME,
  .ui16_battery_pack_resistance_x1000 =
  DEFAULT_VALUE_BATTERY_PACK_RESISTANCE,
  .ui32_odometer_x10 =
  DEFAULT_VALUE_ODOMETER_X10,
  .ui8_walk_assist_feature_enabled =
  DEFAULT_VALUE_WALK_ASSIST_FEATURE_ENABLED,
  .ui8_walk_assist_level_factor = {
  DEFAULT_VALUE_WALK_ASSIST_LEVEL_FACTOR_1,
  DEFAULT_VALUE_WALK_ASSIST_LEVEL_FACTOR_2,
  DEFAULT_VALUE_WALK_ASSIST_LEVEL_FACTOR_3,
  DEFAULT_VALUE_WALK_ASSIST_LEVEL_FACTOR_4,
  DEFAULT_VALUE_WALK_ASSIST_LEVEL_FACTOR_5,
  DEFAULT_VALUE_WALK_ASSIST_LEVEL_FACTOR_6,
  DEFAULT_VALUE_WALK_ASSIST_LEVEL_FACTOR_7,
  },

  .ui16_torque_sensor_calibration_table_left[0][0] = DEFAULT_TORQUE_SENSOR_CALIBRATION_LEFT_WEIGHT_1,
  .ui16_torque_sensor_calibration_table_left[0][1] = DEFAULT_TORQUE_SENSOR_CALIBRATION_LEFT_ADC_1,
  .ui16_torque_sensor_calibration_table_left[1][0] = DEFAULT_TORQUE_SENSOR_CALIBRATION_LEFT_WEIGHT_2,
  .ui16_torque_sensor_calibration_table_left[1][1] = DEFAULT_TORQUE_SENSOR_CALIBRATION_LEFT_ADC_2,
  .ui16_torque_sensor_calibration_table_left[2][0] = DEFAULT_TORQUE_SENSOR_CALIBRATION_LEFT_WEIGHT_3,
  .ui16_torque_sensor_calibration_table_left[2][1] = DEFAULT_TORQUE_SENSOR_CALIBRATION_LEFT_ADC_3,
  .ui16_torque_sensor_calibration_table_left[3][0] = DEFAULT_TORQUE_SENSOR_CALIBRATION_LEFT_WEIGHT_4,
  .ui16_torque_sensor_calibration_table_left[3][1] = DEFAULT_TORQUE_SENSOR_CALIBRATION_LEFT_ADC_4,
  .ui16_torque_sensor_calibration_table_left[4][0] = DEFAULT_TORQUE_SENSOR_CALIBRATION_LEFT_WEIGHT_5,
  .ui16_torque_sensor_calibration_table_left[4][1] = DEFAULT_TORQUE_SENSOR_CALIBRATION_LEFT_ADC_5,
  .ui16_torque_sensor_calibration_table_left[5][0] = DEFAULT_TORQUE_SENSOR_CALIBRATION_LEFT_WEIGHT_6,
  .ui16_torque_sensor_calibration_table_left[5][1] = DEFAULT_TORQUE_SENSOR_CALIBRATION_LEFT_ADC_6,
  .ui16_torque_sensor_calibration_table_left[6][0] = DEFAULT_TORQUE_SENSOR_CALIBRATION_LEFT_WEIGHT_7,
  .ui16_torque_sensor_calibration_table_left[6][1] = DEFAULT_TORQUE_SENSOR_CALIBRATION_LEFT_ADC_7,
  .ui16_torque_sensor_calibration_table_left[7][0] = DEFAULT_TORQUE_SENSOR_CALIBRATION_LEFT_WEIGHT_8,
  .ui16_torque_sensor_calibration_table_left[7][1] = DEFAULT_TORQUE_SENSOR_CALIBRATION_LEFT_ADC_8,

  .ui16_torque_sensor_calibration_table_right[0][0] = DEFAULT_TORQUE_SENSOR_CALIBRATION_RIGHT_WEIGHT_1,
  .ui16_torque_sensor_calibration_table_right[0][1] = DEFAULT_TORQUE_SENSOR_CALIBRATION_RIGHT_ADC_1,
  .ui16_torque_sensor_calibration_table_right[1][0] = DEFAULT_TORQUE_SENSOR_CALIBRATION_RIGHT_WEIGHT_2,
  .ui16_torque_sensor_calibration_table_right[1][1] = DEFAULT_TORQUE_SENSOR_CALIBRATION_RIGHT_ADC_2,
  .ui16_torque_sensor_calibration_table_right[2][0] = DEFAULT_TORQUE_SENSOR_CALIBRATION_RIGHT_WEIGHT_3,
  .ui16_torque_sensor_calibration_table_right[2][1] = DEFAULT_TORQUE_SENSOR_CALIBRATION_RIGHT_ADC_3,
  .ui16_torque_sensor_calibration_table_right[3][0] = DEFAULT_TORQUE_SENSOR_CALIBRATION_RIGHT_WEIGHT_4,
  .ui16_torque_sensor_calibration_table_right[3][1] = DEFAULT_TORQUE_SENSOR_CALIBRATION_RIGHT_ADC_4,
  .ui16_torque_sensor_calibration_table_right[4][0] = DEFAULT_TORQUE_SENSOR_CALIBRATION_RIGHT_WEIGHT_5,
  .ui16_torque_sensor_calibration_table_right[4][1] = DEFAULT_TORQUE_SENSOR_CALIBRATION_RIGHT_ADC_5,
  .ui16_torque_sensor_calibration_table_right[5][0] = DEFAULT_TORQUE_SENSOR_CALIBRATION_RIGHT_WEIGHT_6,
  .ui16_torque_sensor_calibration_table_right[5][1] = DEFAULT_TORQUE_SENSOR_CALIBRATION_RIGHT_ADC_6,
  .ui16_torque_sensor_calibration_table_right[6][0] = DEFAULT_TORQUE_SENSOR_CALIBRATION_RIGHT_WEIGHT_7,
  .ui16_torque_sensor_calibration_table_right[6][1] = DEFAULT_TORQUE_SENSOR_CALIBRATION_RIGHT_ADC_7,
  .ui16_torque_sensor_calibration_table_right[7][0] = DEFAULT_TORQUE_SENSOR_CALIBRATION_RIGHT_WEIGHT_8,
  .ui16_torque_sensor_calibration_table_right[7][1] = DEFAULT_TORQUE_SENSOR_CALIBRATION_RIGHT_ADC_8,

  .ui8_street_mode_function_enabled = DEFAULT_STREET_MODE_FUNCTION_ENABLE,
  .ui8_street_mode_enabled_on_startup = DEFAULT_STREET_MODE_ENABLE_AT_STARTUP,
  .ui8_street_mode_enabled = DEFAULT_STREET_MODE_ENABLE,
  .ui8_street_mode_speed_limit = DEFAULT_STREET_MODE_SPEED_LIMIT,
  .ui8_street_mode_power_limit_div25 = DEFAULT_STREET_MODE_POWER_LIMIT,
  .ui8_street_mode_throttle_enabled = DEFAULT_STREET_MODE_THROTTLE_ENABLE,
  .ui8_street_mode_hotkey_enabled = DEFAULT_STREET_MODE_HOTKEY_ENABLE,
  .ui8_pedal_cadence_fast_stop = DEFAULT_PEDAL_CADENCE_FAST_STOP_ENABLE,
  .ui8_coast_brake_adc = DEFAULT_COAST_BRAKE_ADC,
  .ui8_throttle_virtual_step = DEFAULT_THROTTLE_VIRTUAL_STEP,
  .ui8_torque_sensor_filter = DEFAULT_TORQUE_SENSOR_FILTER,
  .ui8_torque_sensor_adc_threshold = DEFAULT_TORQUE_SENSOR_ADC_THRESHOLD,
  .ui8_coast_brake_enable = DEFAULT_COAST_BRAKE_ENABLE,

  .ui8_ant_device_id = 1,
  .ui8_enter_bootloader = 0,
  .ui8_garmin_datafields = 1,
};

/* Keep track of the progress of a delete_all operation. */
static struct
{
  bool delete_next;   //!< Delete next record.
  bool pending;       //!< Waiting for an fds FDS_EVT_DEL_RECORD event, to delete the next record.
} m_delete_all;

/* A record containing dummy configuration data. */
static fds_record_t const m_fds_configurations =
{
  .file_id           = CONFIG_FILE,
  .key               = CONFIG_REC_KEY,
  .data.p_data       = &m_configurations,
  /* The length of a record is always expressed in 4-byte units (words). */
  .data.length_words = (sizeof(m_configurations) + 3) / sizeof(uint32_t),
};

/* Flag to check fds initialization. */
static bool volatile m_fds_initialized;

static void fds_evt_handler(fds_evt_t const * p_evt)
{
  switch (p_evt->id)
  {
    case FDS_EVT_INIT:
      if (p_evt->result == NRF_SUCCESS)
      {
        m_fds_initialized = true;
      }
      break;

    case FDS_EVT_WRITE:
    {
      if (p_evt->result == NRF_SUCCESS)
      {
        NRF_LOG_INFO("Record ID:\t0x%04x",  p_evt->write.record_id);
        NRF_LOG_INFO("File ID:\t0x%04x",    p_evt->write.file_id);
        NRF_LOG_INFO("Record key:\t0x%04x", p_evt->write.record_key);
      }
    } break;

    case FDS_EVT_DEL_RECORD:
    {
      if (p_evt->result == NRF_SUCCESS)
      {
        NRF_LOG_INFO("Record ID:\t0x%04x",  p_evt->del.record_id);
        NRF_LOG_INFO("File ID:\t0x%04x",    p_evt->del.file_id);
        NRF_LOG_INFO("Record key:\t0x%04x", p_evt->del.record_key);
      }
      m_delete_all.pending = false;
    } break;

    default:
      break;
  }
}

/**@brief   Wait for fds to initialize. */
static void wait_for_fds_ready(void)
{
  while (!m_fds_initialized)
  {
    // power_manage();
  }
}

void eeprom_init() {
  ret_code_t err_code;

  /* Register first to receive an event when initialization is complete. */
  (void) fds_register(fds_evt_handler);

  err_code = fds_init();
  APP_ERROR_CHECK(err_code);
  wait_for_fds_ready();

  err_code = fds_record_find(CONFIG_FILE, CONFIG_REC_KEY, &m_desc_config, &m_tok_config);
  // see if a record is found for config file
  if (err_code == NRF_SUCCESS)
  {
    /* A config file is in flash. Let's update it. */
    fds_flash_record_t config = {0};

    /* Open the record and read its contents. */
    err_code = fds_record_open(&m_desc_config, &config);
    APP_ERROR_CHECK(err_code);

    /* Copy the configuration from flash into m_dummy_cfg. */
    memcpy(&m_configurations, config.p_data, sizeof(m_configurations));
    
    /* Close the record when done reading. */
    err_code = fds_record_close(&m_desc_config);
    APP_ERROR_CHECK(err_code);

    // if eeprom is blank or configurations version is incorrect, RESET to default values
    if (m_configurations.ui8_configurations_version != CONFIGURATIONS_VERSION)
    {
      memcpy(&m_configurations, &m_configurations_defaults,
          sizeof(m_configurations_defaults));
    }
  }
  else // no record is found for config file, set to default values
  {
    memcpy(&m_configurations, &m_configurations_defaults,
        sizeof(m_configurations_defaults));

    err_code = fds_record_write(&m_desc_config, &m_fds_configurations);
    APP_ERROR_CHECK(err_code);
  }
  
  eeprom_init_variables();
  set_conversions();

  // prepare torque_sensor_calibration_table as it will be used at begin to init the motor
  prepare_torque_sensor_calibration_table();
}

void eeprom_init_variables(void) {
	ui_vars_t *ui_vars = get_ui_vars();

  ui_vars->ui8_configurations_version = m_configurations.ui8_configurations_version;
	ui_vars->ui8_assist_level = m_configurations.ui8_assist_level;
	ui_vars->ui16_wheel_perimeter = m_configurations.ui16_wheel_perimeter;
	ui_vars->ui8_wheel_max_speed = m_configurations.ui8_wheel_max_speed;
	ui_vars->ui8_units_type = m_configurations.ui8_units_type;
	ui_vars->ui32_wh_x10_offset = m_configurations.ui32_wh_x10_offset;
	ui_vars->ui32_wh_x10_100_percent =
			m_configurations.ui32_wh_x10_100_percent;
	ui_vars->ui8_battery_soc_enable =
			m_configurations.ui8_battery_soc_enable;
  ui_vars->ui8_target_max_battery_power_div25 =
      m_configurations.ui8_target_max_battery_power_div25;
	ui_vars->ui8_battery_max_current =
			m_configurations.ui8_battery_max_current;
  ui_vars->ui8_motor_max_current =
      m_configurations.ui8_motor_max_current;
  ui_vars->ui8_motor_current_min_adc =
      m_configurations.ui8_motor_current_min_adc;
  ui_vars->ui8_field_weakening =
      m_configurations.ui8_field_weakening;
	ui_vars->ui8_ramp_up_amps_per_second_x10 =
			m_configurations.ui8_ramp_up_amps_per_second_x10;
	ui_vars->ui16_battery_low_voltage_cut_off_x10 =
			m_configurations.ui16_battery_low_voltage_cut_off_x10;
	ui_vars->ui8_motor_type = m_configurations.ui8_motor_type;
	ui_vars->ui8_motor_current_control_mode = m_configurations.ui8_motor_current_control_mode;
	ui_vars->ui8_motor_assistance_startup_without_pedal_rotation =
			m_configurations.ui8_motor_assistance_startup_without_pedal_rotation;
	ui_vars->ui8_temperature_limit_feature_enabled =
			m_configurations.ui8_temperature_limit_feature_enabled;
	COPY_ARRAY(ui_vars, &m_configurations, ui16_assist_level_factor);
	ui_vars->ui8_number_of_assist_levels =
			m_configurations.ui8_number_of_assist_levels;
	ui_vars->ui8_startup_motor_power_boost_feature_enabled =
			m_configurations.ui8_startup_motor_power_boost_feature_enabled;
	ui_vars->ui8_startup_motor_power_boost_limit_power =
			m_configurations.ui8_startup_motor_power_boost_limit_power;
	ui_vars->ui8_startup_motor_power_boost_always =
			m_configurations.ui8_startup_motor_power_boost_always;
	COPY_ARRAY(ui_vars, &m_configurations,
			ui16_startup_motor_power_boost_factor);
	ui_vars->ui8_startup_motor_power_boost_time =
			m_configurations.ui8_startup_motor_power_boost_time;
	ui_vars->ui8_startup_motor_power_boost_fade_time =
			m_configurations.ui8_startup_motor_power_boost_fade_time;
	ui_vars->ui8_motor_temperature_min_value_to_limit =
			m_configurations.ui8_motor_temperature_min_value_to_limit;
	ui_vars->ui8_motor_temperature_max_value_to_limit =
			m_configurations.ui8_motor_temperature_max_value_to_limit;
	ui_vars->ui16_battery_voltage_reset_wh_counter_x10 =
			m_configurations.ui16_battery_voltage_reset_wh_counter_x10;
	ui_vars->ui8_system_power_off_time_minutes =
			m_configurations.ui8_system_power_off_time_minutes;
	ui_vars->ui16_battery_pack_resistance_x1000 =
			m_configurations.ui16_battery_pack_resistance_x1000;
	ui_vars->ui32_odometer_x10 = m_configurations.ui32_odometer_x10;
	ui_vars->ui8_walk_assist_feature_enabled =
			m_configurations.ui8_walk_assist_feature_enabled;
	COPY_ARRAY(ui_vars, &m_configurations, ui8_walk_assist_level_factor);
  ui_vars->ui8_torque_sensor_calibration_pedal_ground = m_configurations.ui8_torque_sensor_calibration_pedal_ground;

  ui_vars->ui8_torque_sensor_calibration_feature_enabled = m_configurations.ui8_torque_sensor_calibration_feature_enabled;
  ui_vars->ui8_torque_sensor_calibration_pedal_ground = m_configurations.ui8_torque_sensor_calibration_pedal_ground;
  for (uint8_t i = 0; i < 8; i++) {
    ui_vars->ui16_torque_sensor_calibration_table_left[i][0] = m_configurations.ui16_torque_sensor_calibration_table_left[i][0];
    ui_vars->ui16_torque_sensor_calibration_table_left[i][1] = m_configurations.ui16_torque_sensor_calibration_table_left[i][1];
    ui_vars->ui16_torque_sensor_calibration_table_right[i][0] = m_configurations.ui16_torque_sensor_calibration_table_right[i][0];
    ui_vars->ui16_torque_sensor_calibration_table_right[i][1] = m_configurations.ui16_torque_sensor_calibration_table_right[i][1];
  }

  ui_vars->ui8_street_mode_function_enabled =
      m_configurations.ui8_street_mode_function_enabled;
  ui_vars->ui8_street_mode_enabled =
      m_configurations.ui8_street_mode_enabled;
  ui_vars->ui8_street_mode_enabled_on_startup =
      m_configurations.ui8_street_mode_enabled_on_startup;
  ui_vars->ui8_street_mode_speed_limit =
      m_configurations.ui8_street_mode_speed_limit;
  ui_vars->ui8_street_mode_power_limit_div25 =
      m_configurations.ui8_street_mode_power_limit_div25;
  ui_vars->ui8_street_mode_throttle_enabled =
      m_configurations.ui8_street_mode_throttle_enabled;
  ui_vars->ui8_street_mode_hotkey_enabled =
      m_configurations.ui8_street_mode_hotkey_enabled;

  ui_vars->ui8_pedal_cadence_fast_stop =
      m_configurations.ui8_pedal_cadence_fast_stop;
  ui_vars->ui8_coast_brake_adc =
      m_configurations.ui8_coast_brake_adc;
  ui_vars->ui8_throttle_virtual_step =
      m_configurations.ui8_throttle_virtual_step;
  ui_vars->ui8_torque_sensor_filter =
      m_configurations.ui8_torque_sensor_filter;
  ui_vars->ui8_torque_sensor_adc_threshold =
      m_configurations.ui8_torque_sensor_adc_threshold;
  ui_vars->ui8_coast_brake_enable =
      m_configurations.ui8_coast_brake_enable;

  ui_vars->ui8_ant_device_id =
    m_configurations.ui8_ant_device_id;

  ui_vars->ui8_enter_bootloader =
    m_configurations.ui8_enter_bootloader;

  ui_vars->ui8_garmin_datafields = m_configurations.ui8_garmin_datafields;    
}

void eeprom_write_variables(void) {
	ui_vars_t *ui_vars = get_ui_vars();
	m_configurations.ui8_assist_level = ui_vars->ui8_assist_level;
	m_configurations.ui16_wheel_perimeter = ui_vars->ui16_wheel_perimeter;
	m_configurations.ui8_wheel_max_speed = ui_vars->ui8_wheel_max_speed;
	m_configurations.ui8_units_type = ui_vars->ui8_units_type;
	m_configurations.ui32_wh_x10_offset = ui_vars->ui32_wh_x10_offset;
	m_configurations.ui32_wh_x10_100_percent =
			ui_vars->ui32_wh_x10_100_percent;
	m_configurations.ui8_battery_soc_enable =
			ui_vars->ui8_battery_soc_enable;
  m_configurations.ui8_target_max_battery_power_div25 =
      ui_vars->ui8_target_max_battery_power_div25;
  m_configurations.ui8_battery_max_current =
      ui_vars->ui8_battery_max_current;
  m_configurations.ui8_motor_max_current =
      ui_vars->ui8_motor_max_current;
  m_configurations.ui8_motor_current_min_adc =
      ui_vars->ui8_motor_current_min_adc;
  m_configurations.ui8_field_weakening =
      ui_vars->ui8_field_weakening;
	m_configurations.ui8_ramp_up_amps_per_second_x10 =
			ui_vars->ui8_ramp_up_amps_per_second_x10;
	m_configurations.ui16_battery_low_voltage_cut_off_x10 =
			ui_vars->ui16_battery_low_voltage_cut_off_x10;
	m_configurations.ui8_motor_type = ui_vars->ui8_motor_type;
	m_configurations.ui8_motor_current_control_mode = ui_vars->ui8_motor_current_control_mode;
	m_configurations.ui8_motor_assistance_startup_without_pedal_rotation =
			ui_vars->ui8_motor_assistance_startup_without_pedal_rotation;
	m_configurations.ui8_temperature_limit_feature_enabled =
			ui_vars->ui8_temperature_limit_feature_enabled;
	COPY_ARRAY(&m_configurations, ui_vars, ui16_assist_level_factor);
	m_configurations.ui8_number_of_assist_levels =
			ui_vars->ui8_number_of_assist_levels;
	m_configurations.ui8_startup_motor_power_boost_feature_enabled =
			ui_vars->ui8_startup_motor_power_boost_feature_enabled;
	m_configurations.ui8_startup_motor_power_boost_always =
			ui_vars->ui8_startup_motor_power_boost_always;
	m_configurations.ui8_startup_motor_power_boost_limit_power =
			ui_vars->ui8_startup_motor_power_boost_limit_power;
	COPY_ARRAY(&m_configurations, ui_vars,
			ui16_startup_motor_power_boost_factor);
	m_configurations.ui8_startup_motor_power_boost_time =
			ui_vars->ui8_startup_motor_power_boost_time;
	m_configurations.ui8_startup_motor_power_boost_fade_time =
			ui_vars->ui8_startup_motor_power_boost_fade_time;
	m_configurations.ui8_motor_temperature_min_value_to_limit =
			ui_vars->ui8_motor_temperature_min_value_to_limit;
	m_configurations.ui8_motor_temperature_max_value_to_limit =
			ui_vars->ui8_motor_temperature_max_value_to_limit;
	m_configurations.ui16_battery_voltage_reset_wh_counter_x10 =
			ui_vars->ui16_battery_voltage_reset_wh_counter_x10;
	m_configurations.ui8_system_power_off_time_minutes =
			ui_vars->ui8_system_power_off_time_minutes;
	m_configurations.ui32_odometer_x10 = ui_vars->ui32_odometer_x10;
	m_configurations.ui8_walk_assist_feature_enabled =
			ui_vars->ui8_walk_assist_feature_enabled;
	COPY_ARRAY(&m_configurations, ui_vars, ui8_walk_assist_level_factor);

  m_configurations.ui8_torque_sensor_calibration_feature_enabled = ui_vars->ui8_torque_sensor_calibration_feature_enabled;
  m_configurations.ui8_torque_sensor_calibration_pedal_ground = ui_vars->ui8_torque_sensor_calibration_pedal_ground;
  for (uint8_t i = 0; i < 8; i++) {
    m_configurations.ui16_torque_sensor_calibration_table_left[i][0] = ui_vars->ui16_torque_sensor_calibration_table_left[i][0];
    m_configurations.ui16_torque_sensor_calibration_table_left[i][1] = ui_vars->ui16_torque_sensor_calibration_table_left[i][1];
    m_configurations.ui16_torque_sensor_calibration_table_right[i][0] = ui_vars->ui16_torque_sensor_calibration_table_right[i][0];
    m_configurations.ui16_torque_sensor_calibration_table_right[i][1] = ui_vars->ui16_torque_sensor_calibration_table_right[i][1];
  }

  m_configurations.ui8_street_mode_function_enabled =
      ui_vars->ui8_street_mode_function_enabled;
  m_configurations.ui8_street_mode_enabled =
      ui_vars->ui8_street_mode_enabled;
  m_configurations.ui8_street_mode_enabled_on_startup =
      ui_vars->ui8_street_mode_enabled_on_startup;
  m_configurations.ui8_street_mode_speed_limit =
      ui_vars->ui8_street_mode_speed_limit;
  m_configurations.ui8_street_mode_power_limit_div25 =
      ui_vars->ui8_street_mode_power_limit_div25;
  m_configurations.ui8_street_mode_throttle_enabled =
      ui_vars->ui8_street_mode_throttle_enabled;
  m_configurations.ui8_street_mode_hotkey_enabled =
      ui_vars->ui8_street_mode_hotkey_enabled;

  m_configurations.ui8_pedal_cadence_fast_stop =
      ui_vars->ui8_pedal_cadence_fast_stop;
  m_configurations.ui8_coast_brake_adc =
      ui_vars->ui8_coast_brake_adc;
  m_configurations.ui8_throttle_virtual_step =
      ui_vars->ui8_throttle_virtual_step;
  m_configurations.ui8_torque_sensor_filter =
      ui_vars->ui8_torque_sensor_filter;
  m_configurations.ui8_torque_sensor_adc_threshold =
      ui_vars->ui8_torque_sensor_adc_threshold;
  m_configurations.ui8_coast_brake_enable =
      ui_vars->ui8_coast_brake_enable;

  m_configurations.ui8_ant_device_id =
    ui_vars->ui8_ant_device_id;

  m_configurations.ui8_enter_bootloader =
    ui_vars->ui8_enter_bootloader;

  m_configurations.ui8_garmin_datafields = ui_vars->ui8_garmin_datafields;

  /* Write the updated record to flash. */
  ret_code_t err_code = fds_record_update(&m_desc_config, &m_fds_configurations);
  APP_ERROR_CHECK(err_code);
  err_code=fds_gc();
  APP_ERROR_CHECK(err_code);

  /* see: https://devzone.nordicsemi.com/f/nordic-q-a/25923/fds-gc-seems-not-to-work
  
Running GC after fds_record_update() will delete the 'old' copy of the record, if it succeeds.

*/
}
