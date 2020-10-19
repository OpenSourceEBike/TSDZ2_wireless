#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "state.h"

#define ASSIST_LEVEL_NUMBER 20

typedef enum {
  MOTOR_INIT_GET_MOTOR_ALIVE,
  MOTOR_INIT_WAIT_MOTOR_ALIVE,
  MOTOR_INIT_GET_MOTOR_FIRMWARE_VERSION,
  MOTOR_INIT_WAIT_MOTOR_FIRMWARE_VERSION,
  MOTOR_INIT_GOT_MOTOR_FIRMWARE_VERSION,
  MOTOR_INIT_ERROR_GET_FIRMWARE_VERSION,
  MOTOR_INIT_RECEIVED_MOTOR_FIRMWARE_VERSION,
  MOTOR_INIT_ERROR_FIRMWARE_VERSION,
  MOTOR_INIT_SET_CONFIGURATIONS,
  MOTOR_INIT_WAIT_CONFIGURATIONS_OK,
  MOTOR_INIT_WAIT_GOT_CONFIGURATIONS_OK,
  MOTOR_INIT_ERROR_SET_CONFIGURATIONS,
  MOTOR_INIT_ERROR,
  MOTOR_INIT_READY,
} motor_init_state_t;

typedef enum {
  MOTOR_INIT_CONFIG_SEND_CONFIG,
  MOTOR_INIT_CONFIG_GET_STATUS,
  MOTOR_INIT_CONFIG_CHECK_STATUS,
} motor_init_state_config_t;

typedef enum {
  MOTOR_INIT_STATUS_RESET = 0,
  MOTOR_INIT_STATUS_GOT_CONFIG = 1,
  MOTOR_INIT_STATUS_INIT_OK = 2,
} motor_init_status_t;

extern volatile motor_init_state_t g_motor_init_state;

typedef struct battery_energy_h_km_struct {
  uint32_t ui32_sum_x50;
  uint32_t ui32_value_x100;
  uint32_t ui32_value_x10;
} battery_energy_h_km_t;

typedef struct rt_vars_struct {
	uint16_t ui16_adc_battery_voltage;
	uint8_t ui8_battery_current_x5;
  uint16_t ui16_battery_power_loss;
	uint8_t ui8_motor_current_x5;
	uint8_t ui8_adc_throttle;
	uint8_t ui8_throttle;
	uint16_t ui16_adc_pedal_torque_sensor;
	uint8_t ui8_pedal_weight_with_offset;
	uint8_t ui8_pedal_weight;
	uint16_t ui16_pedal_power_x10;
	uint8_t ui8_duty_cycle;
	uint8_t ui8_error_states;
	uint16_t ui16_wheel_speed_x10;
	uint8_t ui8_pedal_cadence;
	uint16_t ui16_motor_speed_erps;
	uint8_t ui8_foc_angle;
	uint8_t ui8_motor_hall_sensors;
	uint8_t ui8_pas_pedal_right;
	uint8_t ui8_motor_temperature;
	uint32_t ui32_wheel_speed_sensor_tick_counter;
	uint16_t ui16_battery_voltage_filtered_x10;
	uint16_t ui16_battery_current_filtered_x5;
	uint16_t ui16_motor_current_filtered_x5;
	uint16_t ui16_full_battery_power_filtered_x50;
	uint16_t ui16_battery_power_filtered;
	uint16_t ui16_pedal_power_filtered;
	uint8_t ui8_pedal_cadence_filtered;
	uint16_t ui16_battery_voltage_soc_x10;
	uint32_t ui32_wh_sum_x5;
	uint32_t ui32_wh_sum_counter;
	uint32_t ui32_wh_x10;
	uint32_t ui32_wheel_speed_sensor_tick_counter_offset;

	uint8_t ui8_assist_level;
	uint8_t ui8_number_of_assist_levels;
	uint16_t ui16_wheel_perimeter;
	uint8_t ui8_wheel_max_speed;
	uint8_t ui8_units_type;
	uint32_t ui32_wh_x10_offset;
	uint32_t ui32_wh_x10_100_percent;
	uint8_t ui8_target_max_battery_power_div25;
	uint8_t ui8_battery_max_current;
	uint8_t ui8_motor_max_current;
  uint8_t ui8_motor_current_min_adc;
  uint8_t ui8_field_weakening;
	uint8_t ui8_ramp_up_amps_per_second_x10;
	uint16_t ui16_battery_low_voltage_cut_off_x10;
	uint16_t ui16_battery_voltage_reset_wh_counter_x10;
	uint16_t ui16_battery_pack_resistance_x1000;
	uint8_t ui8_motor_type;
	uint8_t ui8_motor_current_control_mode;
	uint8_t ui8_motor_assistance_startup_without_pedal_rotation;
	uint16_t ui16_assist_level_factor[ASSIST_LEVEL_NUMBER];
	uint8_t ui8_walk_assist_feature_enabled;
	uint8_t ui8_walk_assist_level_factor[ASSIST_LEVEL_NUMBER];
	uint8_t ui8_startup_motor_power_boost_feature_enabled;
	uint8_t ui8_startup_motor_power_boost_always;
	uint8_t ui8_startup_motor_power_boost_limit_power;
	uint8_t ui8_startup_motor_power_boost_time;
	uint8_t ui8_startup_motor_power_boost_fade_time;
	uint16_t ui16_startup_motor_power_boost_factor[ASSIST_LEVEL_NUMBER];
	uint8_t ui8_temperature_limit_feature_enabled;
	uint8_t ui8_motor_temperature_min_value_to_limit;
	uint8_t ui8_motor_temperature_max_value_to_limit;
	uint32_t ui32_odometer_x10;

#ifndef SW102
	uint8_t  ui8_trip_a_auto_reset;
	uint16_t ui16_trip_a_auto_reset_hours;
	uint32_t ui32_trip_a_last_update_time;
#endif
	uint32_t ui32_trip_a_distance_x1000;
	uint32_t ui32_trip_a_time;
	uint16_t ui16_trip_a_avg_speed_x10;
	uint16_t ui16_trip_a_max_speed_x10;

#ifndef SW102
	uint8_t  ui8_trip_b_auto_reset;
	uint16_t ui16_trip_b_auto_reset_hours;
	uint32_t ui32_trip_b_last_update_time;
#endif
	uint32_t ui32_trip_b_distance_x1000;
	uint32_t ui32_trip_b_time;
  	uint16_t ui16_trip_b_avg_speed_x10;
  	uint16_t ui16_trip_b_max_speed_x10;

	uint8_t ui8_lights;
	uint8_t ui8_braking;
	uint8_t ui8_walk_assist;
	uint8_t ui8_offroad_mode;

  uint8_t ui8_torque_sensor_calibration_feature_enabled;
  uint8_t ui8_torque_sensor_calibration_pedal_ground;
  uint16_t ui16_torque_sensor_calibration_table_left[8][2];
  uint16_t ui16_torque_sensor_calibration_table_right[8][2];

  uint8_t ui8_street_mode_enabled;
  uint8_t ui8_street_mode_speed_limit;
  uint8_t ui8_street_mode_power_limit_div25;
  uint8_t ui8_street_mode_throttle_enabled;

  uint8_t ui8_pedal_cadence_fast_stop;
  uint8_t ui8_coast_brake_adc;
  uint8_t ui8_adc_lights_current_offset;
  uint16_t ui16_adc_battery_current;
  uint8_t ui8_throttle_virtual;
  uint8_t ui8_torque_sensor_filter;
  uint8_t ui8_torque_sensor_adc_threshold;
  uint8_t ui8_coast_brake_enable;

  battery_energy_h_km_t battery_energy_h_km;
} rt_vars_t;

/* Selector positions for customizable fields
 * 0 is the graph,
 * 1-4  are the boxes above the graph, mainscreen1 on 850C
 * 5-8  are the boxes above the graph, mainscreen2 on 850C
 * 9-12 are the boxes above the graph, mainscreen2 on 850C
 */
#ifdef SW102
#define NUM_CUSTOMIZABLE_FIELDS 6
#else
#define NUM_CUSTOMIZABLE_FIELDS 12
#endif

typedef struct ui_vars_struct {
	uint16_t ui16_adc_battery_voltage;
	uint8_t ui8_battery_current_x5;
  uint16_t ui16_battery_power_loss;
	uint8_t ui8_motor_current_x5;
	uint8_t ui8_adc_throttle;
	uint8_t ui8_throttle;
	uint16_t ui16_adc_pedal_torque_sensor;
	uint8_t ui8_pedal_weight_with_offset;
	uint8_t ui8_pedal_weight;
	uint8_t ui8_duty_cycle;
	uint8_t ui8_error_states;
	uint16_t ui16_wheel_speed_x10;
	uint8_t ui8_pedal_cadence;
	uint16_t ui16_motor_speed_erps;
	uint8_t ui8_foc_angle;
	uint8_t ui8_motor_hall_sensors;
	uint8_t ui8_pas_pedal_right;
	uint8_t ui8_motor_temperature;
	uint32_t ui32_wheel_speed_sensor_tick_counter;
	uint32_t ui32_wheel_speed_sensor_tick_counter_offset;
	uint16_t ui16_battery_voltage_filtered_x10;
	uint16_t ui16_battery_current_filtered_x5;
	uint16_t ui16_motor_current_filtered_x5;
	uint16_t ui16_full_battery_power_filtered_x50;
	uint16_t ui16_battery_power;
	uint16_t ui16_pedal_torque_filtered;
	uint16_t ui16_pedal_power;
	uint8_t ui8_pedal_cadence_filtered;
	uint16_t ui16_battery_voltage_soc_x10;
	uint32_t ui32_wh_sum_x5;
	uint32_t ui32_wh_sum_counter;
	uint32_t ui32_wh_x10;

	uint8_t ui8_assist_level;
	uint8_t ui8_number_of_assist_levels;
	uint16_t ui16_wheel_perimeter;
	uint16_t wheel_max_speed_x10;
	uint8_t ui8_units_type;
	uint32_t ui32_wh_x10_offset;
	uint32_t ui32_wh_x10_100_percent;
	uint8_t ui8_battery_soc_enable;
	uint8_t ui8_target_max_battery_power_div25;
	uint8_t ui8_battery_max_current;
	uint8_t ui8_motor_max_current;
	uint8_t ui8_motor_current_min_adc;
	uint8_t ui8_field_weakening;
	uint8_t ui8_ramp_up_amps_per_second_x10;
	uint16_t ui16_battery_low_voltage_cut_off_x10;
	uint16_t ui16_battery_voltage_reset_wh_counter_x10;
	uint16_t ui16_battery_pack_resistance_x1000;
	uint16_t ui16_battery_pack_resistance_estimated_x1000;
	uint8_t ui8_motor_type;
	uint8_t ui8_motor_current_control_mode;
	uint8_t ui8_motor_assistance_startup_without_pedal_rotation;
	uint16_t ui16_assist_level_factor[ASSIST_LEVEL_NUMBER];
	uint8_t ui8_walk_assist_feature_enabled;
	uint8_t ui8_walk_assist_level_factor[ASSIST_LEVEL_NUMBER];
	uint8_t ui8_startup_motor_power_boost_feature_enabled;
	uint8_t ui8_startup_motor_power_boost_always;
	uint8_t ui8_startup_motor_power_boost_limit_power;
	uint8_t ui8_startup_motor_power_boost_time;
	uint8_t ui8_startup_motor_power_boost_fade_time;
	uint16_t ui16_startup_motor_power_boost_factor[ASSIST_LEVEL_NUMBER];
	uint8_t ui8_temperature_limit_feature_enabled;
	uint8_t ui8_motor_temperature_min_value_to_limit;
	uint8_t ui8_motor_temperature_max_value_to_limit;
	uint8_t ui8_system_power_off_time_minutes;
	uint32_t ui32_odometer_x10;

#ifndef SW102
	uint8_t  ui8_trip_a_auto_reset;
	uint16_t ui16_trip_a_auto_reset_hours;
	uint32_t ui32_trip_a_last_update_time;
#endif
	uint32_t ui32_trip_a_distance_x1000;
	uint32_t ui32_trip_a_distance_x100;
	uint32_t ui32_trip_a_time;
	uint16_t ui16_trip_a_avg_speed_x10;
	uint16_t ui16_trip_a_max_speed_x10;

#ifndef SW102
	uint8_t  ui8_trip_b_auto_reset;
	uint16_t ui16_trip_b_auto_reset_hours;
	uint32_t ui32_trip_b_last_update_time;
#endif
	uint32_t ui32_trip_b_distance_x1000;
	uint32_t ui32_trip_b_distance_x100;
	uint32_t ui32_trip_b_time;
  	uint16_t ui16_trip_b_avg_speed_x10;
  	uint16_t ui16_trip_b_max_speed_x10;

	uint32_t battery_energy_km_value_x100;

	uint8_t ui8_lights;
	uint8_t ui8_braking;
	uint8_t ui8_walk_assist;
	uint8_t ui8_offroad_mode;

	uint8_t ui8_torque_sensor_calibration_feature_enabled;
	uint8_t ui8_torque_sensor_calibration_pedal_ground;
	uint16_t ui16_torque_sensor_calibration_table_left[8][2];
	uint16_t ui16_torque_sensor_calibration_table_right[8][2];

	uint8_t ui8_street_mode_function_enabled;
	uint8_t ui8_street_mode_enabled;
	uint8_t ui8_street_mode_enabled_on_startup;
	uint8_t ui8_street_mode_speed_limit;
	uint8_t ui8_street_mode_power_limit_div25;
	uint16_t ui16_street_mode_power_limit;
	uint8_t ui8_street_mode_throttle_enabled;
	uint8_t ui8_street_mode_hotkey_enabled;

  uint8_t ui8_pedal_cadence_fast_stop;
  uint8_t ui8_coast_brake_adc;
  uint16_t ui16_adc_battery_current;
  uint8_t ui8_throttle_virtual;
  uint8_t ui8_throttle_virtual_step;
  uint8_t ui8_torque_sensor_filter;
  uint8_t ui8_torque_sensor_adc_threshold;
  uint8_t ui8_coast_brake_enable;

  uint8_t ui8_ant_device_id;
} ui_vars_t;

ui_vars_t* get_ui_vars(void);
rt_vars_t* get_rt_vars(void);

extern rt_vars_t rt_vars; // FIXME - this shouldn't be exposed outside of state.c - but currently mid merge
extern ui_vars_t ui_vars;

// extern volatile uint8_t ui8_g_motorVariablesStabilized;

typedef struct {
  uint8_t major;
  uint8_t minor;
  uint8_t patch;
} tsdz2_firmware_version_t;


// /**
//  * Called from the main thread every 100ms
//  *
//  */
void copy_rt_ui_vars(void);
void set_conversions(void);
void prepare_torque_sensor_calibration_table(void);
void rt_processing(void);

extern tsdz2_firmware_version_t g_tsdz2_firmware_version;
extern volatile motor_init_status_t ui8_g_motor_init_status;
