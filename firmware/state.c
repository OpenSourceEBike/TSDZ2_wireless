/*
 * Bafang LCD 850C firmware
 *
 * Copyright (C) Casainho, 2018.
 *
 * Released under the GPL License, Version 3
 */

#include <math.h>
#include <stdlib.h>
#include "stdio.h"
#include "uart.h"
#include "utils.h"
#include "state.h"

typedef enum {
  FRAME_TYPE_ALIVE = 0,
  FRAME_TYPE_STATUS = 1,
  FRAME_TYPE_PERIODIC = 2,
  FRAME_TYPE_CONFIGURATIONS = 3,
  FRAME_TYPE_FIRMWARE_VERSION = 4,
} frame_type_t;

static uint8_t ui8_m_usart1_received_first_package = 0;
uint8_t ui8_g_battery_soc;
volatile uint8_t ui8_g_motorVariablesStabilized = 0;

volatile uint8_t m_get_tsdz2_firmware_version; // true if we are simulating a motor (and therefore not talking on serial at all)
volatile motor_init_state_t g_motor_init_state = MOTOR_INIT_GET_MOTOR_ALIVE;
volatile motor_init_state_config_t g_motor_init_state_conf = MOTOR_INIT_CONFIG_SEND_CONFIG;
volatile motor_init_status_t ui8_g_motor_init_status = MOTOR_INIT_STATUS_RESET;

tsdz2_firmware_version_t g_tsdz2_firmware_version = { 0xff, 0, 0 };

rt_vars_t rt_vars;
ui_vars_t ui_vars;

// Set to true if we should automatically convert kph -> mph and km -> mi
bool screenConvertMiles = false;

// Set to true if we should automatically convert C -> F
bool screenConvertFarenheit = false;

// Set to true if we should automatically convert kg -> lb
bool screenConvertPounds = false;

extern void rt_processing_stop(void);
extern void rt_processing_start(void);

ui_vars_t* get_ui_vars(void) {
	return &ui_vars;
}

rt_vars_t* get_rt_vars(void) {
  return &rt_vars;
}

void set_conversions(void) {
  screenConvertMiles = ui_vars.ui8_units_type != 0; // Set initial value on unit conversions (FIXME, move this someplace better)
  screenConvertFarenheit = screenConvertMiles; // FIXME, should be based on a different eeprom config value
  screenConvertPounds = screenConvertMiles;
}

void rt_send_tx_package(frame_type_t type) {
  uint8_t crc_len = 3; // minimun is 3
	uint8_t *ui8_usart1_tx_buffer = uart_get_tx_buffer();

	/************************************************************************************************/
	// send tx package
	// start up byte
	ui8_usart1_tx_buffer[0] = 0x59;
  ui8_usart1_tx_buffer[1] = crc_len;
  // type of the frame
	ui8_usart1_tx_buffer[2] = (uint8_t) type;

	switch (type) {
	  case FRAME_TYPE_PERIODIC:
      if (rt_vars.ui8_walk_assist) {
        ui8_usart1_tx_buffer[3] = (uint8_t) rt_vars.ui8_walk_assist_level_factor[((rt_vars.ui8_assist_level) - 1)];
        ui8_usart1_tx_buffer[4] = 0;
      } else if (rt_vars.ui8_assist_level) {
        uint16_t ui16_temp = rt_vars.ui16_assist_level_factor[((rt_vars.ui8_assist_level) - 1)];
        ui8_usart1_tx_buffer[3] = (uint8_t) (ui16_temp & 0xff);
        ui8_usart1_tx_buffer[4] = (uint8_t) (ui16_temp >> 8);
      } else {
        // if rt_vars.ui8_assist_level = 0, send 0!! always disable motor when assist level is 0
        ui8_usart1_tx_buffer[3] = 0;
        ui8_usart1_tx_buffer[4] = 0;
      }

      ui8_usart1_tx_buffer[5] = (rt_vars.ui8_lights & 1) | ((rt_vars.ui8_walk_assist & 1) << 1);

      // battery power limit
      if (rt_vars.ui8_street_mode_enabled)
      {
        ui8_usart1_tx_buffer[6] = rt_vars.ui8_street_mode_power_limit_div25;
      }
      else
      {
        ui8_usart1_tx_buffer[6] = rt_vars.ui8_target_max_battery_power_div25;
      }

      // startup motor power boost
      uint16_t ui16_temp = (uint8_t) rt_vars.ui16_startup_motor_power_boost_factor[((rt_vars.ui8_assist_level) - 1)];
      ui8_usart1_tx_buffer[7] = (uint8_t) (ui16_temp & 0xff);
      ui8_usart1_tx_buffer[8] = (uint8_t) (ui16_temp >> 8);

      // wheel max speed
      if (rt_vars.ui8_street_mode_enabled)
      {
        ui8_usart1_tx_buffer[9] = rt_vars.ui8_street_mode_speed_limit;
      }
      else
      {
        ui8_usart1_tx_buffer[9] = rt_vars.ui8_wheel_max_speed;
      }

      // motor temperature limit function or throttle
      if (rt_vars.ui8_street_mode_enabled &&
          rt_vars.ui8_street_mode_throttle_enabled)
      {
        ui8_usart1_tx_buffer[10] = rt_vars.ui8_temperature_limit_feature_enabled & 1;
      }
      else
      {
        ui8_usart1_tx_buffer[10] = rt_vars.ui8_temperature_limit_feature_enabled & 3;
      }

      // virtual throttle
      ui8_usart1_tx_buffer[11] = (uint8_t) ((((uint16_t) rt_vars.ui8_throttle_virtual) * 255) / 100);

      crc_len = 13;
      ui8_usart1_tx_buffer[1] = crc_len;
	    break;

    // set configurations
	  case FRAME_TYPE_CONFIGURATIONS:
      // battery low voltage cut-off
      ui8_usart1_tx_buffer[3] = (uint8_t) (rt_vars.ui16_battery_low_voltage_cut_off_x10 & 0xff);
      ui8_usart1_tx_buffer[4] = (uint8_t) (rt_vars.ui16_battery_low_voltage_cut_off_x10 >> 8);

      // wheel perimeter
      ui8_usart1_tx_buffer[5] = (uint8_t) (rt_vars.ui16_wheel_perimeter & 0xff);
      ui8_usart1_tx_buffer[6] = (uint8_t) (rt_vars.ui16_wheel_perimeter >> 8);

      // battery max current
      ui8_usart1_tx_buffer[7] = rt_vars.ui8_battery_max_current;

      ui8_usart1_tx_buffer[8] = rt_vars.ui8_startup_motor_power_boost_feature_enabled |
          (rt_vars.ui8_startup_motor_power_boost_always << 1) |
          (rt_vars.ui8_startup_motor_power_boost_limit_power << 2) |
          (rt_vars.ui8_torque_sensor_calibration_feature_enabled << 3) |
          (rt_vars.ui8_torque_sensor_calibration_pedal_ground << 4) |
          (rt_vars.ui8_motor_assistance_startup_without_pedal_rotation << 5) |
          ((rt_vars.ui8_motor_type & 1) << 6);

      // motor max current
      ui8_usart1_tx_buffer[9] = rt_vars.ui8_motor_max_current;
      // startup motor power boost time
      ui8_usart1_tx_buffer[10] = rt_vars.ui8_startup_motor_power_boost_time;
      // startup motor power boost fade time
      ui8_usart1_tx_buffer[11] = rt_vars.ui8_startup_motor_power_boost_fade_time;

      // motor over temperature min and max values to limit
      ui8_usart1_tx_buffer[12] = rt_vars.ui8_motor_temperature_min_value_to_limit;
      ui8_usart1_tx_buffer[13] = rt_vars.ui8_motor_temperature_max_value_to_limit;

      ui8_usart1_tx_buffer[14] = rt_vars.ui8_ramp_up_amps_per_second_x10;

      // TODO
      // target speed for cruise
      ui8_usart1_tx_buffer[15] = 0;

      // torque sensor calibration tables
      uint8_t j = 16;
      for (uint8_t i = 0; i < 8; i++) {
        ui8_usart1_tx_buffer[j++] = (uint8_t) rt_vars.ui16_torque_sensor_calibration_table_left[i][0];
        ui8_usart1_tx_buffer[j++] = (uint8_t) (rt_vars.ui16_torque_sensor_calibration_table_left[i][0] >> 8);
        ui8_usart1_tx_buffer[j++] = (uint8_t) rt_vars.ui16_torque_sensor_calibration_table_left[i][1];
        ui8_usart1_tx_buffer[j++] = (uint8_t) (rt_vars.ui16_torque_sensor_calibration_table_left[i][1] >> 8);
      }

      for (uint8_t i = 0; i < 8; i++) {
        ui8_usart1_tx_buffer[j++] = (uint8_t) rt_vars.ui16_torque_sensor_calibration_table_right[i][0];
        ui8_usart1_tx_buffer[j++] = (uint8_t) (rt_vars.ui16_torque_sensor_calibration_table_right[i][0] >> 8);
        ui8_usart1_tx_buffer[j++] = (uint8_t) rt_vars.ui16_torque_sensor_calibration_table_right[i][1];
        ui8_usart1_tx_buffer[j++] = (uint8_t) (rt_vars.ui16_torque_sensor_calibration_table_right[i][1] >> 8);
      }

      // battery current min ADC
      ui8_usart1_tx_buffer[79] = rt_vars.ui8_motor_current_min_adc;
      ui8_usart1_tx_buffer[80] = (rt_vars.ui8_pedal_cadence_fast_stop |
          (rt_vars.ui8_field_weakening << 1) |
          (rt_vars.ui8_coast_brake_enable << 2) |
          (rt_vars.ui8_motor_current_control_mode << 3));
      ui8_usart1_tx_buffer[81] = rt_vars.ui8_coast_brake_adc;
      ui8_usart1_tx_buffer[82] = rt_vars.ui8_adc_lights_current_offset;
      ui8_usart1_tx_buffer[83] = rt_vars.ui8_torque_sensor_filter;
      ui8_usart1_tx_buffer[84] = rt_vars.ui8_torque_sensor_adc_threshold;

      crc_len = 86;
      ui8_usart1_tx_buffer[1] = crc_len;
	    break;

	    case FRAME_TYPE_STATUS:
	    case FRAME_TYPE_FIRMWARE_VERSION:
	      // nothing to add to the package
	      break;

	    default:
	      break;
	}

	// prepare crc of the package
	uint16_t ui16_crc_tx = 0xffff;
	for (uint8_t ui8_i = 0; ui8_i < crc_len; ui8_i++) {
		crc16(ui8_usart1_tx_buffer[ui8_i], &ui16_crc_tx);
	}
	ui8_usart1_tx_buffer[crc_len] =
			(uint8_t) (ui16_crc_tx & 0xff);
	ui8_usart1_tx_buffer[crc_len + 1] =
			(uint8_t) (ui16_crc_tx >> 8) & 0xff;

	// send the full package to UART
  uart_send_tx_buffer(ui8_usart1_tx_buffer, ui8_usart1_tx_buffer[1] + 2);
}

/**
 * Called from the main thread every 100ms
 *
 */
void copy_rt_ui_vars(void) {
	ui_vars.ui16_adc_battery_voltage = rt_vars.ui16_adc_battery_voltage;
	ui_vars.ui8_battery_current_x5 = rt_vars.ui8_battery_current_x5;
	ui_vars.ui16_battery_power_loss = rt_vars.ui16_battery_power_loss;
	ui_vars.ui8_motor_current_x5 = rt_vars.ui8_motor_current_x5;
	ui_vars.ui8_throttle = rt_vars.ui8_throttle;
	ui_vars.ui16_adc_pedal_torque_sensor = rt_vars.ui16_adc_pedal_torque_sensor;
	ui_vars.ui8_pedal_weight_with_offset = rt_vars.ui8_pedal_weight_with_offset;
	ui_vars.ui8_pedal_weight = rt_vars.ui8_pedal_weight;
	ui_vars.ui8_duty_cycle = rt_vars.ui8_duty_cycle;
	ui_vars.ui8_error_states = rt_vars.ui8_error_states;
	ui_vars.ui16_wheel_speed_x10 = rt_vars.ui16_wheel_speed_x10;
	ui_vars.ui8_pedal_cadence = rt_vars.ui8_pedal_cadence;
	ui_vars.ui8_pedal_cadence_filtered = rt_vars.ui8_pedal_cadence_filtered;
	ui_vars.ui16_motor_speed_erps = rt_vars.ui16_motor_speed_erps;
	ui_vars.ui8_motor_hall_sensors = rt_vars.ui8_motor_hall_sensors;
	ui_vars.ui8_pas_pedal_right = rt_vars.ui8_pas_pedal_right;
	ui_vars.ui8_motor_temperature = rt_vars.ui8_motor_temperature;
	ui_vars.ui32_wheel_speed_sensor_tick_counter =
			rt_vars.ui32_wheel_speed_sensor_tick_counter;
	ui_vars.ui16_battery_voltage_filtered_x10 =
			rt_vars.ui16_battery_voltage_filtered_x10;
	ui_vars.ui16_battery_current_filtered_x5 =
			rt_vars.ui16_battery_current_filtered_x5;
  ui_vars.ui16_motor_current_filtered_x5 =
      rt_vars.ui16_motor_current_filtered_x5;
	ui_vars.ui16_full_battery_power_filtered_x50 =
			rt_vars.ui16_full_battery_power_filtered_x50;
	ui_vars.ui16_battery_power = rt_vars.ui16_battery_power_filtered;
	ui_vars.ui16_pedal_power = rt_vars.ui16_pedal_power_filtered;
	ui_vars.ui16_battery_voltage_soc_x10 = rt_vars.ui16_battery_voltage_soc_x10;
	ui_vars.ui32_wh_sum_x5 = rt_vars.ui32_wh_sum_x5;
	ui_vars.ui32_wh_sum_counter = rt_vars.ui32_wh_sum_counter;
	ui_vars.ui32_wh_x10 = rt_vars.ui32_wh_x10;
	ui_vars.ui8_braking = rt_vars.ui8_braking;
	ui_vars.ui8_foc_angle = (((uint16_t) rt_vars.ui8_foc_angle) * 14) / 10; // each units is equal to 1.4 degrees ((360 degrees / 256) = 1.4)

  ui_vars.ui32_trip_a_last_update_time = rt_vars.ui32_trip_a_last_update_time;
  ui_vars.ui32_trip_b_last_update_time = rt_vars.ui32_trip_b_last_update_time;

	ui_vars.ui32_trip_a_distance_x1000 = rt_vars.ui32_trip_a_distance_x1000;
  ui_vars.ui32_trip_a_distance_x100 = rt_vars.ui32_trip_a_distance_x1000 / 10;  
  ui_vars.ui32_trip_a_time = rt_vars.ui32_trip_a_time;
  ui_vars.ui16_trip_a_avg_speed_x10 = rt_vars.ui16_trip_a_avg_speed_x10;
  ui_vars.ui16_trip_a_max_speed_x10 = rt_vars.ui16_trip_a_max_speed_x10;

  ui_vars.ui32_trip_b_distance_x1000 = rt_vars.ui32_trip_b_distance_x1000;
  ui_vars.ui32_trip_b_distance_x100 = rt_vars.ui32_trip_b_distance_x1000 / 10;
  ui_vars.ui32_trip_b_time = rt_vars.ui32_trip_b_time;
  ui_vars.ui16_trip_b_avg_speed_x10 = rt_vars.ui16_trip_b_avg_speed_x10;
  ui_vars.ui16_trip_b_max_speed_x10 = rt_vars.ui16_trip_b_max_speed_x10;

	ui_vars.ui32_odometer_x10 = rt_vars.ui32_odometer_x10;
	ui_vars.battery_energy_km_value_x100 = rt_vars.battery_energy_h_km.ui32_value_x100;
  ui_vars.ui16_adc_battery_current = rt_vars.ui16_adc_battery_current;

  rt_vars.ui32_wh_x10_100_percent = ui_vars.ui32_wh_x10_100_percent;
	rt_vars.ui32_wh_x10_offset = ui_vars.ui32_wh_x10_offset;
	rt_vars.ui16_battery_pack_resistance_x1000 = ui_vars.ui16_battery_pack_resistance_x1000;
	rt_vars.ui8_assist_level = ui_vars.ui8_assist_level;
	for (uint8_t i = 0; i < ASSIST_LEVEL_NUMBER; i++) {
	  rt_vars.ui16_assist_level_factor[i] = ui_vars.ui16_assist_level_factor[i];
	}
  for (uint8_t i = 0; i < ASSIST_LEVEL_NUMBER; i++) {
    rt_vars.ui8_walk_assist_level_factor[i] = ui_vars.ui8_walk_assist_level_factor[i];
  }
	rt_vars.ui8_lights = ui_vars.ui8_lights;
	rt_vars.ui8_walk_assist = ui_vars.ui8_walk_assist;
	rt_vars.ui8_offroad_mode = ui_vars.ui8_offroad_mode;
	rt_vars.ui8_battery_max_current = ui_vars.ui8_battery_max_current;
	rt_vars.ui8_motor_max_current = ui_vars.ui8_motor_max_current;
	rt_vars.ui8_motor_current_min_adc = ui_vars.ui8_motor_current_min_adc;
	rt_vars.ui8_field_weakening = ui_vars.ui8_field_weakening;
	rt_vars.ui8_ramp_up_amps_per_second_x10 =
			ui_vars.ui8_ramp_up_amps_per_second_x10;
	rt_vars.ui8_target_max_battery_power_div25 = ui_vars.ui8_target_max_battery_power_div25;
	rt_vars.ui16_battery_low_voltage_cut_off_x10 =
			ui_vars.ui16_battery_low_voltage_cut_off_x10;
	rt_vars.ui16_wheel_perimeter = ui_vars.ui16_wheel_perimeter;
	rt_vars.ui8_wheel_max_speed = ui_vars.wheel_max_speed_x10 / 10;
	rt_vars.ui8_motor_type = ui_vars.ui8_motor_type;
	rt_vars.ui8_motor_current_control_mode = ui_vars.ui8_motor_current_control_mode;
	rt_vars.ui8_motor_assistance_startup_without_pedal_rotation =
			ui_vars.ui8_motor_assistance_startup_without_pedal_rotation;
	rt_vars.ui8_temperature_limit_feature_enabled =
			ui_vars.ui8_temperature_limit_feature_enabled;
	rt_vars.ui8_startup_motor_power_boost_always =
			ui_vars.ui8_startup_motor_power_boost_always;
	rt_vars.ui8_startup_motor_power_boost_limit_power =
			ui_vars.ui8_startup_motor_power_boost_limit_power;
	rt_vars.ui8_startup_motor_power_boost_time =
			ui_vars.ui8_startup_motor_power_boost_time;
  for (uint8_t i = 0; i < 9; i++) {
    rt_vars.ui16_startup_motor_power_boost_factor[i] = ui_vars.ui16_startup_motor_power_boost_factor[i];
  }
	rt_vars.ui8_startup_motor_power_boost_fade_time =
			ui_vars.ui8_startup_motor_power_boost_fade_time;
	rt_vars.ui8_startup_motor_power_boost_feature_enabled =
			ui_vars.ui8_startup_motor_power_boost_feature_enabled;
	rt_vars.ui8_motor_temperature_min_value_to_limit =
			ui_vars.ui8_motor_temperature_min_value_to_limit;
	rt_vars.ui8_motor_temperature_max_value_to_limit =
			ui_vars.ui8_motor_temperature_max_value_to_limit;
	rt_vars.ui8_offroad_feature_enabled = ui_vars.ui8_offroad_feature_enabled;
	rt_vars.ui8_offroad_enabled_on_startup =
			ui_vars.ui8_offroad_enabled_on_startup;
	rt_vars.ui8_offroad_speed_limit = ui_vars.ui8_offroad_speed_limit;
	rt_vars.ui8_offroad_power_limit_enabled =
			ui_vars.ui8_offroad_power_limit_enabled;
	rt_vars.ui8_offroad_power_limit_div25 =
			ui_vars.ui8_offroad_power_limit_div25;
  rt_vars.ui8_torque_sensor_calibration_pedal_ground =
      ui_vars.ui8_torque_sensor_calibration_pedal_ground;

  rt_vars.ui8_torque_sensor_calibration_feature_enabled = ui_vars.ui8_torque_sensor_calibration_feature_enabled;
  rt_vars.ui8_torque_sensor_calibration_pedal_ground = ui_vars.ui8_torque_sensor_calibration_pedal_ground;

  rt_vars.ui8_street_mode_enabled = ui_vars.ui8_street_mode_enabled;
  rt_vars.ui8_street_mode_speed_limit = ui_vars.ui8_street_mode_speed_limit;
  rt_vars.ui8_street_mode_power_limit_div25 = ui_vars.ui8_street_mode_power_limit_div25;
  rt_vars.ui8_street_mode_throttle_enabled = ui_vars.ui8_street_mode_throttle_enabled;

  rt_vars.ui8_pedal_cadence_fast_stop = ui_vars.ui8_pedal_cadence_fast_stop;
  rt_vars.ui8_coast_brake_adc = ui_vars.ui8_coast_brake_adc;
  rt_vars.ui8_adc_lights_current_offset = ui_vars.ui8_adc_lights_current_offset;
  rt_vars.ui8_throttle_virtual = ui_vars.ui8_throttle_virtual;
  rt_vars.ui8_torque_sensor_filter = ui_vars.ui8_torque_sensor_filter;
  rt_vars.ui8_torque_sensor_adc_threshold = ui_vars.ui8_torque_sensor_adc_threshold;
  rt_vars.ui8_coast_brake_enable = ui_vars.ui8_coast_brake_enable;
}

void communications(void) {
  frame_type_t ui8_frame;
  uint8_t process_frame = 0;
  uint16_t ui16_temp;

  const uint8_t *p_rx_buffer = uart_get_rx_buffer_rdy();

  // process rx package if UART had a packet
  if (p_rx_buffer) {
    // now process rx data
    ui8_frame = (frame_type_t) p_rx_buffer[2];

    switch (g_motor_init_state) {
      case MOTOR_INIT_WAIT_MOTOR_ALIVE:
        if (ui8_frame == FRAME_TYPE_ALIVE)
          g_motor_init_state = MOTOR_INIT_GET_MOTOR_FIRMWARE_VERSION;
        break;

      case MOTOR_INIT_WAIT_MOTOR_FIRMWARE_VERSION:
        if (ui8_frame == FRAME_TYPE_FIRMWARE_VERSION)
          process_frame = 1;
        break;

      case MOTOR_INIT_WAIT_CONFIGURATIONS_OK:
      case MOTOR_INIT_WAIT_GOT_CONFIGURATIONS_OK:
        if (ui8_frame == FRAME_TYPE_STATUS)
          process_frame = 1;
        break;

      case MOTOR_INIT_READY:
          process_frame = 1;
        break;
    }

    if (process_frame) {
      switch (ui8_frame) {
        case FRAME_TYPE_STATUS:
          ui8_g_motor_init_status = p_rx_buffer[3];
          break;

        case FRAME_TYPE_PERIODIC:
          rt_vars.ui16_adc_battery_voltage = p_rx_buffer[3] | (((uint16_t) (p_rx_buffer[4] & 0x30)) << 4);
          rt_vars.ui8_battery_current_x5 = p_rx_buffer[5];
          ui16_temp = ((uint16_t) p_rx_buffer[6]) | (((uint16_t) p_rx_buffer[7] << 8));
          rt_vars.ui16_wheel_speed_x10 = ui16_temp & 0x7ff; // 0x7ff = 204.7km/h as the other bits are used for other things

          uint8_t ui8_temp = p_rx_buffer[8];
          rt_vars.ui8_braking = ui8_temp & 1;
          rt_vars.ui8_motor_hall_sensors = (ui8_temp >> 1) & 7;
          rt_vars.ui8_pas_pedal_right = (ui8_temp >> 4) & 1;
          rt_vars.ui8_adc_throttle = p_rx_buffer[9];

          if (rt_vars.ui8_temperature_limit_feature_enabled) {
            rt_vars.ui8_motor_temperature = p_rx_buffer[10];
          } else {
            rt_vars.ui8_throttle = p_rx_buffer[10];
          }

          rt_vars.ui16_adc_pedal_torque_sensor = ((uint16_t) p_rx_buffer[11]) | (((uint16_t) (p_rx_buffer[7] & 0xC0)) << 2);
          rt_vars.ui8_pedal_weight_with_offset = p_rx_buffer[12];
          rt_vars.ui8_pedal_weight = p_rx_buffer[13];

          rt_vars.ui8_pedal_cadence = p_rx_buffer[14];

          rt_vars.ui8_duty_cycle = p_rx_buffer[15];

          rt_vars.ui16_motor_speed_erps = ((uint16_t) p_rx_buffer[16]) | ((uint16_t) p_rx_buffer[17] << 8);
          rt_vars.ui8_foc_angle = p_rx_buffer[18];
          rt_vars.ui8_error_states = p_rx_buffer[19];
          rt_vars.ui8_motor_current_x5 = p_rx_buffer[20];

          uint32_t ui32_wheel_speed_sensor_tick_temp;
          ui32_wheel_speed_sensor_tick_temp = ((uint32_t) p_rx_buffer[21]) |
              (((uint32_t) p_rx_buffer[22]) << 8) | (((uint32_t) p_rx_buffer[23]) << 16);
          rt_vars.ui32_wheel_speed_sensor_tick_counter = ui32_wheel_speed_sensor_tick_temp;

          rt_vars.ui16_pedal_power_x10 = ((uint16_t) p_rx_buffer[24]) | ((uint16_t) p_rx_buffer[25] << 8);

          ui16_temp = (uint16_t) p_rx_buffer[26];
          rt_vars.ui16_adc_battery_current = ui16_temp | ((uint16_t) ((p_rx_buffer[7] & 0x18) << 5));
          break;

        case FRAME_TYPE_FIRMWARE_VERSION:
          rt_vars.ui8_error_states = p_rx_buffer[3];
          g_tsdz2_firmware_version.major = p_rx_buffer[4];
          g_tsdz2_firmware_version.minor = p_rx_buffer[5];
          g_tsdz2_firmware_version.patch = p_rx_buffer[6];
          g_motor_init_state = MOTOR_INIT_GOT_MOTOR_FIRMWARE_VERSION;
          break;
      }
    }

    // let's wait for 10 packages, seems that first ADC battery voltages have incorrect values
    ui8_m_usart1_received_first_package++;
    if (ui8_m_usart1_received_first_package > 10)
      ui8_m_usart1_received_first_package = 10;

    uart_get_rx_buffer_rdy_clear();
  }

  if (g_motor_init_state == MOTOR_INIT_READY)
    rt_send_tx_package(FRAME_TYPE_PERIODIC);
}

static void motor_init(void) {
  static uint16_t ui16_motor_init_command_error_cnt = 0;
  static uint8_t ui8_motor_init_status_cnt = 0;

  if ((g_motor_init_state != MOTOR_INIT_ERROR) &&
      (g_motor_init_state != MOTOR_INIT_ERROR_GET_FIRMWARE_VERSION) &&
      (g_motor_init_state != MOTOR_INIT_ERROR_FIRMWARE_VERSION) &&
      (g_motor_init_state != MOTOR_INIT_READY))
  {
    switch (g_motor_init_state) {
      case MOTOR_INIT_GET_MOTOR_ALIVE:
        ui16_motor_init_command_error_cnt = 1000;
        g_motor_init_state = MOTOR_INIT_WAIT_MOTOR_ALIVE;
        // not break here to follow for next case

      case MOTOR_INIT_WAIT_MOTOR_ALIVE:
        // check timeout
        ui16_motor_init_command_error_cnt--;
        if (ui16_motor_init_command_error_cnt == 0) {
          g_motor_init_state = MOTOR_INIT_GET_MOTOR_ALIVE;
        }
        break;

      case MOTOR_INIT_GET_MOTOR_FIRMWARE_VERSION:
        ui16_motor_init_command_error_cnt = 1000; // 10 seconds
        g_motor_init_state = MOTOR_INIT_WAIT_MOTOR_FIRMWARE_VERSION;
        // not break here to follow for next case

      case MOTOR_INIT_WAIT_MOTOR_FIRMWARE_VERSION:
        rt_send_tx_package(FRAME_TYPE_FIRMWARE_VERSION);
        // check timeout
        ui16_motor_init_command_error_cnt--;
        if (ui16_motor_init_command_error_cnt == 0) {
          g_motor_init_state = MOTOR_INIT_ERROR_GET_FIRMWARE_VERSION;
        }
        break;

      case MOTOR_INIT_GOT_MOTOR_FIRMWARE_VERSION:
        if (g_tsdz2_firmware_version.major == atoi(TSDZ2_FIRMWARE_MAJOR) &&
            g_tsdz2_firmware_version.minor == atoi(TSDZ2_FIRMWARE_MINOR)) {

            g_motor_init_state = MOTOR_INIT_SET_CONFIGURATIONS;
            // not break here to follow for next case
        } else {
          g_motor_init_state = MOTOR_INIT_ERROR_FIRMWARE_VERSION;
          break;
        }

      case MOTOR_INIT_SET_CONFIGURATIONS:
        ui16_motor_init_command_error_cnt = 1000;
        g_motor_init_state_conf = MOTOR_INIT_CONFIG_SEND_CONFIG;
        g_motor_init_state = MOTOR_INIT_WAIT_CONFIGURATIONS_OK;
        // not break here to follow for next case

      case MOTOR_INIT_WAIT_CONFIGURATIONS_OK:
      case MOTOR_INIT_WAIT_GOT_CONFIGURATIONS_OK:
        // check timeout
        ui16_motor_init_command_error_cnt--;
        if (ui16_motor_init_command_error_cnt == 0) {
          g_motor_init_state = MOTOR_INIT_ERROR_SET_CONFIGURATIONS;
          break;
        }

        switch (g_motor_init_state_conf) {
          case MOTOR_INIT_CONFIG_SEND_CONFIG:
            rt_send_tx_package(FRAME_TYPE_CONFIGURATIONS);
            ui8_motor_init_status_cnt = 10;
            g_motor_init_state_conf = MOTOR_INIT_CONFIG_GET_STATUS;
            break;

          case MOTOR_INIT_CONFIG_GET_STATUS:
            rt_send_tx_package(FRAME_TYPE_STATUS);
            g_motor_init_state_conf = MOTOR_INIT_CONFIG_CHECK_STATUS;
            break;

          case MOTOR_INIT_CONFIG_CHECK_STATUS:
            if (ui8_g_motor_init_status == MOTOR_INIT_STATUS_RESET) {
              ui8_motor_init_status_cnt--;
              if (ui8_motor_init_status_cnt == 0) {
                g_motor_init_state_conf = MOTOR_INIT_CONFIG_SEND_CONFIG;
                break;
              }
            }

            if (ui8_g_motor_init_status == MOTOR_INIT_STATUS_RESET) {
              g_motor_init_state_conf = MOTOR_INIT_CONFIG_GET_STATUS;

            } else if (ui8_g_motor_init_status == MOTOR_INIT_STATUS_GOT_CONFIG) {

              g_motor_init_state = MOTOR_INIT_WAIT_GOT_CONFIGURATIONS_OK;
              g_motor_init_state_conf = MOTOR_INIT_CONFIG_GET_STATUS;

            } else if (ui8_g_motor_init_status == MOTOR_INIT_STATUS_INIT_OK) {

              g_motor_init_state = MOTOR_INIT_READY; // finally

              // reset state vars
              g_motor_init_state_conf = MOTOR_INIT_CONFIG_SEND_CONFIG;
              ui8_g_motor_init_status = MOTOR_INIT_STATUS_RESET;
            }
            break;
        }
    }
  }
}

// Note: this called from ISR context every 50ms
void rt_processing(void)
{
  communications();

  // called here because this state machine for motor_init should run every 100ms
  // montor init processing must be done when exiting the configurations menu
  // once motor is initialized, this should take almost no processing time
  motor_init();
}

void prepare_torque_sensor_calibration_table(void) {
  static bool first_time = true;

  // we need to make this atomic
  rt_processing_stop();

  // at the very first time, copy the ADC values from one table to the other
  if (first_time) {
    first_time = false;

    for (uint8_t i = 0; i < 8; i++) {
      rt_vars.ui16_torque_sensor_calibration_table_left[i][0] = ui_vars.ui16_torque_sensor_calibration_table_left[i][1];
      rt_vars.ui16_torque_sensor_calibration_table_right[i][0] = ui_vars.ui16_torque_sensor_calibration_table_right[i][1];
    }
  }

  // get the delta values of ADC steps per kg
  for (uint8_t i = 1; i < 8; i++) {
    // get the deltas x100
    rt_vars.ui16_torque_sensor_calibration_table_left[i][1] =
        ((ui_vars.ui16_torque_sensor_calibration_table_left[i][0] - ui_vars.ui16_torque_sensor_calibration_table_left[i - 1][0]) * 100) /
        (ui_vars.ui16_torque_sensor_calibration_table_left[i][1] - ui_vars.ui16_torque_sensor_calibration_table_left[i - 1][1]);

    rt_vars.ui16_torque_sensor_calibration_table_right[i][1] =
        ((ui_vars.ui16_torque_sensor_calibration_table_right[i][0] - ui_vars.ui16_torque_sensor_calibration_table_right[i - 1][0]) * 100) /
        (ui_vars.ui16_torque_sensor_calibration_table_right[i][1] - ui_vars.ui16_torque_sensor_calibration_table_right[i - 1][1]);
  }
  // very first table value need to the calculated here
  rt_vars.ui16_torque_sensor_calibration_table_left[0][1] = rt_vars.ui16_torque_sensor_calibration_table_left[1][1]; // the first delta is equal the the second one
  rt_vars.ui16_torque_sensor_calibration_table_right[0][1] = rt_vars.ui16_torque_sensor_calibration_table_right[1][1]; // the first delta is equal the the second one


  rt_processing_start();
}
