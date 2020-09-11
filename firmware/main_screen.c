/*
 * Bafang LCD 850C firmware
 *
 * Copyright (C) Casainho, 2018.
 *
 * Released under the GPL License, Version 3
 */

#include <math.h>
#include <string.h>
#include "stdio.h"
#include "utils.h"
#include "screen.h"
#include "rtc.h"
#include "fonts.h"
#include "uart.h"
#include "main_screen.h"
#include "eeprom.h"
#include "buttons.h"
// #include "lcd.h"
#include "adc.h"
#include "ugui.h"
#include "configscreen.h"
#include "state.h"
#include "timer.h"
#ifdef SW102
#include "peer_manager.h"
#endif

// // only used on SW102, to count timeout to override the wheel speed value with assist level value
// static uint16_t m_assist_level_change_timeout = 0;

// uint8_t ui8_m_wheel_speed_integer;
// uint8_t ui8_m_wheel_speed_decimal;

// static uint8_t ui8_walk_assist_timeout = 0;

// uint16_t ui16_m_battery_current_filtered_x10;
// uint16_t ui16_m_motor_current_filtered_x10;
// uint16_t ui16_m_battery_power_filtered;
// uint16_t ui16_m_pedal_power_filtered;

// uint8_t g_showNextScreenIndex = 0;
// uint8_t g_showNextScreenPreviousIndex = 0;
// uint16_t ui16_m_alternate_field_value;
// uint8_t ui8_m_alternate_field_state = 0;
// uint8_t ui8_m_alternate_field_timeout_cnt = 0;
// uint8_t ui8_m_vthrottle_can_increment_decrement = 0;

// void lcd_main_screen(void);
// void warnings(void);
// void walk_assist_state(void);
// void power(void);
// void time(void);
// void wheel_speed(void);
// void battery_soc(void);
// void up_time(void);
// void trip_time(void);
// void updateTripTime(uint32_t tripTime, Field *field);
// void wheel_speed(void);
// void showNextScreen();
// static bool renderWarning(FieldLayout *layout);
// void DisplayResetToDefaults(void);
// void TripMemoriesReset(void);
// void DisplayResetBluetoothPeers(void);
// void onSetConfigurationBatteryTotalWh(uint32_t v);
// void batteryTotalWh(void);
// void batteryCurrent(void);
// void batteryResistance(void);
// void motorCurrent(void);
// void batteryPower(void);
// void pedalPower(void);
// void thresholds(void);

// /// set to true if this boot was caused because we had a watchdog failure, used to show user the problem in the fault line
// bool wd_failure_detected;

// #define MAX_TIMESTR_LEN 8 // including nul terminator
// #define MAX_BATTERY_POWER_USAGE_STR_LEN 6 // Wh/km or Wh/mi , including nul terminator
// #define MAX_ALTERNATE_USAGE_STR_LEN 10 // "max power", "throttle"  including nul terminator

// //
// // Fields - these might be shared my multiple screens
// //
// Field socField = FIELD_DRAWTEXT_RW();
// Field timeField = FIELD_DRAWTEXT_RW();
// Field assistLevelField = FIELD_READONLY_UINT("assist", &ui_vars.ui8_assist_level, "", false);
// Field wheelSpeedIntegerField = FIELD_READONLY_UINT("speed", &ui8_m_wheel_speed_integer, "kph", false);
// Field wheelSpeedDecimalField = FIELD_READONLY_UINT("", &ui8_m_wheel_speed_decimal, "kph", false);
// Field wheelSpeedField = FIELD_READONLY_UINT("speed", &ui_vars.ui16_wheel_speed_x10, "kph", true, .div_digits = 1);

// // Note: this field is special, the string it is pointing to must be in RAM so we can change it later
// Field upTimeField = FIELD_READONLY_STRING(_S("up time", "up time"), (char [MAX_TIMESTR_LEN]){ 0 });

// Field tripADistanceField = FIELD_READONLY_UINT(_S("A trip dist", "A trip dis"), &ui_vars.ui32_trip_a_distance_x100, "km", false, .div_digits = 2);
// // Note: this field is special, the string it is pointing to must be in RAM so we can change it later
// Field tripATimeField = FIELD_READONLY_STRING(_S("A trip time", "A trip tim"), (char [MAX_TIMESTR_LEN]){ 0 });
// Field tripAAvgSpeedField = FIELD_READONLY_UINT(_S("A avg speed", "A avgspeed"), &ui_vars.ui16_trip_a_avg_speed_x10, "kph", true, .div_digits = 1);
// Field tripAMaxSpeedField = FIELD_READONLY_UINT(_S("A max speed", "A maxspeed"), &ui_vars.ui16_trip_a_max_speed_x10, "kph", true, .div_digits = 1);

// Field tripBDistanceField = FIELD_READONLY_UINT(_S("B trip dist", "B trip dis"), &ui_vars.ui32_trip_b_distance_x100, "km", false, .div_digits = 2);
// // Note: this field is special, the string it is pointing to must be in RAM so we can change it later
// Field tripBTimeField = FIELD_READONLY_STRING(_S("B trip time", "B trip tim"), (char [MAX_TIMESTR_LEN]){ 0 });
// Field tripBAvgSpeedField = FIELD_READONLY_UINT(_S("B avg speed", "B avgspeed"), &ui_vars.ui16_trip_b_avg_speed_x10, "kph", true, .div_digits = 1);
// Field tripBMaxSpeedField = FIELD_READONLY_UINT(_S("B max speed", "B maxspeed"), &ui_vars.ui16_trip_b_max_speed_x10, "kph", true, .div_digits = 1);

// Field odoField = FIELD_READONLY_UINT("odometer", &ui_vars.ui32_odometer_x10, "km", false, .div_digits = 1);
// Field cadenceField = FIELD_READONLY_UINT("cadence", &ui_vars.ui8_pedal_cadence_filtered, "rpm", true, .div_digits = 0);
// Field humanPowerField = FIELD_READONLY_UINT(_S("human power", "human powr"), &ui16_m_pedal_power_filtered, "W", true, .div_digits = 0);
// Field batteryPowerField = FIELD_READONLY_UINT(_S("motor power", "motor powr"), &ui16_m_battery_power_filtered, "W", true, .div_digits = 0);
// Field fieldAlternate = FIELD_READONLY_UINT((char [MAX_ALTERNATE_USAGE_STR_LEN]){ 0 }, &ui16_m_alternate_field_value, "", 0, 2500, .div_digits = 0,);
// Field batteryVoltageField = FIELD_READONLY_UINT(_S("batt voltage", "bat volts"), &ui_vars.ui16_battery_voltage_filtered_x10, "", true, .div_digits = 1);
// Field batteryCurrentField = FIELD_READONLY_UINT(_S("batt current", "bat curren"), &ui16_m_battery_current_filtered_x10, "", true, .div_digits = 1);
// Field motorCurrentField = FIELD_READONLY_UINT(_S("motor current", "mot curren"), &ui16_m_motor_current_filtered_x10, "", true, .div_digits = 1);
// Field batterySOCField = FIELD_READONLY_UINT(_S("battery SOC", "bat SOC"), &ui8_g_battery_soc, "%", true, .div_digits = 0);
// Field motorTempField = FIELD_READONLY_UINT(_S("motor temp", "mot temp"), &ui_vars.ui8_motor_temperature, "C", true, .div_digits = 0);
// Field motorErpsField = FIELD_READONLY_UINT(_S("motor speed", "mot speed"), &ui_vars.ui16_motor_speed_erps, "", true, .div_digits = 0);
// Field pwmDutyField = FIELD_READONLY_UINT(_S("motor pwm", "mot pwm"), &ui_vars.ui8_duty_cycle, "", true, .div_digits = 0);
// Field motorFOCField = FIELD_READONLY_UINT(_S("motor foc", "mot foc"), &ui_vars.ui8_foc_angle, "", true, .div_digits = 0);
// Field batteryPowerUsageField = FIELD_READONLY_UINT((char [MAX_BATTERY_POWER_USAGE_STR_LEN]){ 0 }, &ui_vars.battery_energy_km_value_x100, "kph", true, .div_digits = 2);


// Field warnField = FIELD_CUSTOM(renderWarning);

// /**
//  * NOTE: The indexes into this array are stored in EEPROM, to prevent user confusion add new options only at the end.
//  * If you remove old values, either warn users or bump up eeprom version to force eeprom contents to be discarded.
//  */
// Field *customizables[] = {
//     &upTimeField, // 0
//     &odoField, // 1
//     &tripADistanceField, // 2
//     &tripATimeField, // 3
//     &tripAAvgSpeedField, // 4
//     &tripAMaxSpeedField, // 5
//     &tripBDistanceField, // 6
//     &tripBTimeField, // 7
//     &tripBMaxSpeedField, // 8
//     &tripBAvgSpeedField, // 9
//     &wheelSpeedField, // 10
//     &cadenceField, // 11
// 		&humanPowerField, // 12
// 		&batteryPowerField, // 13
//     &batteryVoltageField, // 14
//     &batteryCurrentField, // 15
//     &motorCurrentField, // 16
//     &batterySOCField, // 17
// 		&motorTempField, // 18
//     &motorErpsField, // 19
// 		&pwmDutyField, // 20
// 		&motorFOCField, // 21
// 		&batteryPowerUsageField, // 22
// 		NULL
// };

// // We currently don't have any graphs in the SW102, so leave them here until then
// // kevinh: I think the following could be probably shared with the defs above (no need to copy and compute twice).  Also high chance of introducing bugs
// // only in one place.
// // Though I'm not sure why you need l2 vs l3 vars in this case.
// Field wheelSpeedFieldGraph = FIELD_READONLY_UINT("speed", &rt_vars.ui16_wheel_speed_x10, "km", false, .div_digits = 1);
// Field tripDistanceFieldGraph = FIELD_READONLY_UINT("trip distance", &rt_vars.ui32_trip_a_distance_x1000, "km", false, .div_digits = 1);
// Field odoFieldGraph = FIELD_READONLY_UINT("odometer", &rt_vars.ui32_odometer_x10, "km", false, .div_digits = 1);
// Field cadenceFieldGraph = FIELD_READONLY_UINT("cadence", &rt_vars.ui8_pedal_cadence_filtered, "", false);
// Field humanPowerFieldGraph = FIELD_READONLY_UINT("human power", &rt_vars.ui16_pedal_power_filtered, "", false);
// Field batteryPowerFieldGraph = FIELD_READONLY_UINT("motor power", &rt_vars.ui16_battery_power_filtered, "", false);
// Field batteryVoltageFieldGraph = FIELD_READONLY_UINT("battery voltage", &rt_vars.ui16_battery_voltage_filtered_x10, "", false, .div_digits = 1);
// Field batteryCurrentFieldGraph = FIELD_READONLY_UINT("battery current", &ui16_m_battery_current_filtered_x10, "", false, .div_digits = 1);
// Field motorCurrentFieldGraph = FIELD_READONLY_UINT("motor current", &ui16_m_motor_current_filtered_x10, "", false, .div_digits = 1);
// Field batterySOCFieldGraph = FIELD_READONLY_UINT("battery SOC", &ui8_g_battery_soc, "", false);
// Field motorTempFieldGraph = FIELD_READONLY_UINT("motor temperature", &rt_vars.ui8_motor_temperature, "C", false);
// Field motorErpsFieldGraph = FIELD_READONLY_UINT("motor speed", &rt_vars.ui16_motor_speed_erps, "", false);
// Field pwmDutyFieldGraph = FIELD_READONLY_UINT("pwm duty-cycle", &rt_vars.ui8_duty_cycle, "", false);
// Field motorFOCFieldGraph = FIELD_READONLY_UINT("motor foc", &rt_vars.ui8_foc_angle, "", false);

// // Note: this field label is special, the string it is pointing to must be in RAM so we can change it later
// Field batteryPowerUsageFieldGraph = FIELD_READONLY_UINT((char [MAX_BATTERY_POWER_USAGE_STR_LEN]){ 0 }, &rt_vars.battery_energy_h_km.ui32_value_x10, "kph", false, .div_digits = 1);

// #ifndef SW102 // we don't have any graphs yet on SW102, possibly move this into mainscreen_850.c
// Field wheelSpeedGraph = FIELD_GRAPH(&wheelSpeedFieldGraph, .min_threshold = -1, .graph_vars = &g_graphVars[VarsWheelSpeed]);
// Field tripDistanceGraph = FIELD_GRAPH(&tripDistanceFieldGraph, .min_threshold = -1, .graph_vars = &g_graphVars[VarsTripDistance]);
// Field cadenceGraph = FIELD_GRAPH(&cadenceFieldGraph, .min_threshold = -1, .graph_vars = &g_graphVars[VarsCadence]);
// Field humanPowerGraph = FIELD_GRAPH(&humanPowerFieldGraph, .min_threshold = -1, .graph_vars = &g_graphVars[VarsHumanPower]);
// Field batteryPowerGraph = FIELD_GRAPH(&batteryPowerFieldGraph, .min_threshold = -1, .graph_vars = &g_graphVars[VarsBatteryPower]);
// Field batteryPowerUsageGraph = FIELD_GRAPH(&batteryPowerUsageFieldGraph, .min_threshold = -1, .graph_vars = &g_graphVars[VarsBatteryPowerUsage]);
// Field batteryVoltageGraph = FIELD_GRAPH(&batteryVoltageFieldGraph, .min_threshold = -1, .graph_vars = &g_graphVars[VarsBatteryVoltage]);
// Field batteryCurrentGraph = FIELD_GRAPH(&batteryCurrentFieldGraph, .filter = FilterSquare, .min_threshold = -1, .graph_vars = &g_graphVars[VarsBatteryCurrent]);
// Field motorCurrentGraph = FIELD_GRAPH(&motorCurrentFieldGraph, .min_threshold = -1, .graph_vars = &g_graphVars[VarsMotorCurrent]);
// Field batterySOCGraph = FIELD_GRAPH(&batterySOCFieldGraph, .min_threshold = -1, .graph_vars = &g_graphVars[VarsBatterySOC]);
// Field motorTempGraph = FIELD_GRAPH(&motorTempFieldGraph, .min_threshold = -1, .graph_vars = &g_graphVars[VarsMotorTemp]);
// Field motorErpsGraph = FIELD_GRAPH(&motorErpsFieldGraph, .min_threshold = -1, .graph_vars = &g_graphVars[VarsMotorERPS]);
// Field pwmDutyGraph = FIELD_GRAPH(&pwmDutyFieldGraph, .min_threshold = -1, .graph_vars = &g_graphVars[VarsMotorPWM]);
// Field motorFOCGraph = FIELD_GRAPH(&motorFOCFieldGraph, .min_threshold = -1, .graph_vars = &g_graphVars[VarsMotorFOC]);
// #endif

// // Note: the number of graphs in this collection must equal GRAPH_VARIANT_SIZE (for now)
// #ifndef SW102
// Field graph1 = FIELD_CUSTOMIZABLE(&ui_vars.graphs_field_selectors[0],
//   &wheelSpeedGraph,
//   &tripDistanceGraph,
//   &cadenceGraph,
//   &humanPowerGraph,
//   &batteryPowerGraph,
//   &batteryPowerUsageGraph,
//   &batteryVoltageGraph,
//   &batteryCurrentGraph,
//   &motorCurrentGraph,
//   &batterySOCGraph,
//   &motorTempGraph,
//   &motorErpsGraph,
//   &pwmDutyGraph,
//   &motorFOCGraph);

// Field graph2 = FIELD_CUSTOMIZABLE(&ui_vars.graphs_field_selectors[1],
//   &wheelSpeedGraph,
//   &tripDistanceGraph,
//   &cadenceGraph,
//   &humanPowerGraph,
//   &batteryPowerGraph,
//   &batteryPowerUsageGraph,
//   &batteryVoltageGraph,
//   &batteryCurrentGraph,
//   &motorCurrentGraph,
//   &batterySOCGraph,
//   &motorTempGraph,
//   &motorErpsGraph,
//   &pwmDutyGraph,
//   &motorFOCGraph);

// Field graph3 = FIELD_CUSTOMIZABLE(&ui_vars.graphs_field_selectors[2],
//   &wheelSpeedGraph,
//   &tripDistanceGraph,
//   &cadenceGraph,
//   &humanPowerGraph,
//   &batteryPowerGraph,
//   &batteryPowerUsageGraph,
//   &batteryVoltageGraph,
//   &batteryCurrentGraph,
//   &motorCurrentGraph,
//   &batterySOCGraph,
//   &motorTempGraph,
//   &motorErpsGraph,
//   &pwmDutyGraph,
//   &motorFOCGraph);

// Field *graphs[3] = { &graph1, &graph2, &graph3 }; // 3 graphs, each one for each main screen
// #endif

// Field *activeGraphs = NULL; // set only once graph data is safe to read

// // Note: field_selectors[0] is used on the 850C for the graphs selector
// Field custom1 = FIELD_CUSTOMIZABLE_PTR(&ui_vars.field_selectors[0], customizables),
//   custom2 = FIELD_CUSTOMIZABLE_PTR(&ui_vars.field_selectors[1], customizables),
//   custom3 = FIELD_CUSTOMIZABLE_PTR(&ui_vars.field_selectors[2], customizables),
//   custom4 = FIELD_CUSTOMIZABLE_PTR(&ui_vars.field_selectors[3], customizables),
//   custom5 = FIELD_CUSTOMIZABLE_PTR(&ui_vars.field_selectors[4], customizables),
// #ifdef SW102
//   custom6 = FIELD_CUSTOMIZABLE_PTR(&ui_vars.field_selectors[5], customizables);
// #else
//   custom6 = FIELD_CUSTOMIZABLE_PTR(&ui_vars.field_selectors[5], customizables),
//   custom7 = FIELD_CUSTOMIZABLE_PTR(&ui_vars.field_selectors[6], customizables),
//   custom8 = FIELD_CUSTOMIZABLE_PTR(&ui_vars.field_selectors[7], customizables),
//   custom9 = FIELD_CUSTOMIZABLE_PTR(&ui_vars.field_selectors[8], customizables),
//   custom10 = FIELD_CUSTOMIZABLE_PTR(&ui_vars.field_selectors[9], customizables),
//   custom11 = FIELD_CUSTOMIZABLE_PTR(&ui_vars.field_selectors[10], customizables),
//   custom12 = FIELD_CUSTOMIZABLE_PTR(&ui_vars.field_selectors[11], customizables);
// #endif


// Field bootHeading = FIELD_DRAWTEXT_RO(_S("OpenSource EBike", "OS-EBike")),
//    bootURL_1 = FIELD_DRAWTEXT_RO(_S("www.github.com/", "Keep pedal")),
//    bootURL_2 = FIELD_DRAWTEXT_RO(_S("OpenSource-EBike-Firmware", "free")),

// #ifdef DISPLAY_850C
//    bootFirmwareVersion = FIELD_DRAWTEXT_RO("850C firmware version:"),
// #elif DISPLAY_860C
//    bootFirmwareVersion = FIELD_DRAWTEXT_RO("860C firmware version:"),
// #endif

//    bootVersion = FIELD_DRAWTEXT_RO(VERSION_STRING),
//    bootStatus1 = FIELD_DRAWTEXT_RO(_S("Keep pedals free and wait", "free pedal")),
//    bootStatus2 = FIELD_DRAWTEXT_RW(.msg = "");

// static void bootScreenOnPreUpdate() {
//   switch (g_motor_init_state) {
//     case MOTOR_INIT_WAIT_GOT_CONFIGURATIONS_OK:
//     case MOTOR_INIT_READY:
//     case MOTOR_INIT_SIMULATING:
//       if (buttons_get_onoff_state() == 0) {
//         buttons_clear_all_events();
//         showNextScreen();
//       } else {
//         if ((g_motor_init_state == MOTOR_INIT_WAIT_GOT_CONFIGURATIONS_OK) ||
//             (g_motor_init_state == MOTOR_INIT_READY)) {
//           fieldPrintf(&bootStatus2, _S("TSDZ2 firmware: %u.%u.%u", "%u.%u.%u"),
//           g_tsdz2_firmware_version.major,
//           g_tsdz2_firmware_version.minor,
//           g_tsdz2_firmware_version.patch);
//         }
//       }

//     // any error state will block here and avoid leave the boot screen
//     default:
//       break;
//   }
// }

// void bootScreenOnExit(void) {
//   // SW102: now that we are goind to main screen, start by showing the assist level for 5 seconds
//   m_assist_level_change_timeout = 50;
// }

// Screen bootScreen = {
//   .onPreUpdate = bootScreenOnPreUpdate,
//   .onExit = bootScreenOnExit,

//   .fields = {
// #ifdef SW102
//     {
//       .x = 0, .y = YbyEighths(0) + 2, .height = -1,
//       .field = &bootHeading,
//       .font = &REGULAR_TEXT_FONT,
//     },
//     {
//       .x = 0, .y = -24, .height = -1,
//       .field = &bootURL_1,
//       .font = &SMALL_TEXT_FONT,
//     },

//     {
//       .x = 0, .y = -6, .height = -1,
//       .field = &bootURL_2,
//       .font = &SMALL_TEXT_FONT,
//     },
// #else
//     {
//       .x = 0, .y = YbyEighths(1), .height = -1,
//       .field = &bootHeading,
//       .font = &REGULAR_TEXT_FONT,
//     },
//     {
//       .x = 0, .y = -20, .height = -1,
//       .field = &bootURL_1,
//       .font = &SMALL_TEXT_FONT,
//     },

//     {
//       .x = 0, .y = -6, .height = -1,
//       .field = &bootURL_2,
//       .font = &SMALL_TEXT_FONT,
//     },
// #endif
// #ifndef SW102
//     {
//       .x = 0, .y = YbyEighths(4), .height = -1,
//       .field = &bootStatus1,
//       .font = &SMALL_TEXT_FONT,
//     },
//     {
//       .x = 0, .y = YbyEighths(6), .height = -1,
//       .field = &bootFirmwareVersion,
//       .font = &SMALL_TEXT_FONT,
//     },
// #endif
// #ifdef SW102
//     {
//       .x = 0, .y = -24, .height = -1,
//       .field = &bootVersion,
//       .font = &SMALL_TEXT_FONT,
//     },
// #else
//     {
//       .x = 0, .y = -8, .height = -1,
//       .field = &bootVersion,
//       .font = &SMALL_TEXT_FONT,
//     },
// #endif
//     {
//       .x = 0, .y = YbyEighths(7), .height = -1,
//       .field = &bootStatus2,
//       .font = &SMALL_TEXT_FONT,
//     },
//     {
//       .field = NULL
//     }
//   }
// };

// // Allow common operations (like walk assist and headlights) button presses to work on any page
// bool anyscreen_onpress(buttons_events_t events) {
//   if ((events & DOWN_LONG_CLICK) && ui_vars.ui8_walk_assist_feature_enabled) {
//     ui_vars.ui8_walk_assist = 1;
//     return true;
//   }

//   // long up to turn on headlights
//   if (events & UP_LONG_CLICK) {
//     ui_vars.ui8_lights = !ui_vars.ui8_lights;
//     set_lcd_backlight();

//     return true;
//   }

//   return false;
// }

// static bool onPressAlternateField(buttons_events_t events) {
//   bool handled = false;

//   // start increment throttle only with UP_LONG_CLICK
//   if ((ui8_m_alternate_field_state == 7) &&
//       (events & UP_LONG_CLICK) &&
//       (ui8_m_vthrottle_can_increment_decrement == 0)) {
//     ui8_m_vthrottle_can_increment_decrement = 1;
//     events |= UP_CLICK; // let's increment, consider UP CLICK
//     ui8_m_alternate_field_timeout_cnt = 50; // 50 * 20ms = 1 second
//   }

//   if (ui8_m_alternate_field_timeout_cnt == 0) {
//     ui_vars.ui8_throttle_virtual = 0;
//     ui8_m_vthrottle_can_increment_decrement = 0;
//   }

//   switch (ui8_m_alternate_field_state) {
//     case 0:
//       if (events & SCREENCLICK_ALTERNATE_FIELD_START) {
//         ui8_m_alternate_field_state = 1;
//         handled = true;
//       }
//       break;

//     // max power
//     case 3:
//       if (
//         (
//           ui_vars.ui8_street_mode_function_enabled
//           && ui_vars.ui8_street_mode_enabled
//           && ui_vars.ui8_street_mode_throttle_enabled
//           || !ui_vars.ui8_street_mode_function_enabled
//           || !ui_vars.ui8_street_mode_enabled
//         )
//         && events & SCREENCLICK_ALTERNATE_FIELD_START
//       ) {
//         ui8_m_alternate_field_state = 6;
//         handled = true;
//         break;
//       }

//       if (events & SCREENCLICK_ALTERNATE_FIELD_STOP) {
//         ui8_m_alternate_field_state = 4;
//         handled = true;
//         break;
//       }

//       if (events & UP_CLICK) {
//         handled = true;

//         if (ui_vars.ui8_target_max_battery_power_div25 < 10) {
//           ui_vars.ui8_target_max_battery_power_div25++;
//         } else {
//           ui_vars.ui8_target_max_battery_power_div25 += 2;
//         }

//         // limit to 100 * 25 = 2500 Watts
//         if(ui_vars.ui8_target_max_battery_power_div25 > 100) {
//           ui_vars.ui8_target_max_battery_power_div25 = 100;
//         }

//         break;
//       }

//       if (events & DOWN_CLICK) {
//         handled = true;

//         if (ui_vars.ui8_target_max_battery_power_div25 <= 10 &&
//             ui_vars.ui8_target_max_battery_power_div25 > 1) {
//           ui_vars.ui8_target_max_battery_power_div25--;
//         } else if (ui_vars.ui8_target_max_battery_power_div25 > 10) {
//           ui_vars.ui8_target_max_battery_power_div25 -= 2;
//         }

//         break;
//       }
//     break;

//     // virtual throttle
//     case 7:
//       if (events & SCREENCLICK_ALTERNATE_FIELD_START) {
//         ui8_m_alternate_field_state = 1;
//         handled = true;
//         break;
//       }

//       if (events & SCREENCLICK_ALTERNATE_FIELD_STOP) {
//         ui_vars.ui8_throttle_virtual = 0;
//         ui8_m_alternate_field_timeout_cnt = 0;
//         ui8_m_vthrottle_can_increment_decrement = 0;
//         ui8_m_alternate_field_state = 4;
//         handled = true;
//         break;
//       }

//       if (events & UP_CLICK) {
//         handled = true;

//         if (ui8_m_vthrottle_can_increment_decrement &&
//             ui_vars.ui8_assist_level) {
//           if ((ui_vars.ui8_throttle_virtual + ui_vars.ui8_throttle_virtual_step) <= 100) {
//             ui_vars.ui8_throttle_virtual += ui_vars.ui8_throttle_virtual_step;
//           } else {
//             ui_vars.ui8_throttle_virtual = 100;
//           }

//           ui8_m_alternate_field_timeout_cnt = 50;
//         }

//         break;
//       }

//       if (events & DOWN_CLICK) {
//         handled = true;

//         if (ui8_m_vthrottle_can_increment_decrement &&
//             ui_vars.ui8_assist_level) {
//           if (ui_vars.ui8_throttle_virtual >= ui_vars.ui8_throttle_virtual_step) {
//             ui_vars.ui8_throttle_virtual -= ui_vars.ui8_throttle_virtual_step;
//           } else {
//             ui_vars.ui8_throttle_virtual = 0;
//           }

//           ui8_m_alternate_field_timeout_cnt = 50;
//         }

//         break;
//       }
//     break;
//   }

//   if (ui8_m_alternate_field_state == 7) {
//     // user will keep UP DOWN LONG clicks on this state, so, clean them to not pass to next code
//     if ((events & UP_LONG_CLICK) ||
//         (events & DOWN_LONG_CLICK))
//       handled = true;
//   }

//   return handled;
// }

// static bool onPressStreetMode(buttons_events_t events) {
//   bool handled = false;

//   if (events & SCREENCLICK_STREET_MODE)
//   {
//     if (ui_vars.ui8_street_mode_function_enabled && ui_vars.ui8_street_mode_hotkey_enabled)
//     {
//       if (ui_vars.ui8_street_mode_enabled)
//         ui_vars.ui8_street_mode_enabled = 0;
//       else
//         ui_vars.ui8_street_mode_enabled = 1;

//       mainScreenOnDirtyClean();
//     }

//     handled = true;
//   }

//   return handled;
// }

// bool mainScreenOnPress(buttons_events_t events) {
//   bool handled = false;

//   handled = onPressAlternateField(events);

//   if (handled == false)
//     handled = anyscreen_onpress(events);

//   if (handled == false)
//     handled = onPressStreetMode(events);

//   if (handled == false &&
//       ui8_m_alternate_field_state == 0) {
//     if (events & UP_CLICK) {
//       ui_vars.ui8_assist_level++;

//       if (ui_vars.ui8_assist_level > ui_vars.ui8_number_of_assist_levels) {
//         ui_vars.ui8_assist_level = ui_vars.ui8_number_of_assist_levels;
//       }

//       m_assist_level_change_timeout = 20; // 2 seconds
//       handled = true;
//     }

//     if (
//       events & DOWN_CLICK
//       && !ui_vars.ui8_walk_assist // do not lower assist level if walk assist is active
//     ) {
//       if (ui_vars.ui8_assist_level > 0)
//         ui_vars.ui8_assist_level--;

//       m_assist_level_change_timeout = 20; // 2 seconds
//       handled = true;
//     }
//   }

// 	return handled;
// }

void set_conversions() {
  screenConvertMiles = ui_vars.ui8_units_type != 0; // Set initial value on unit conversions (FIXME, move this someplace better)
  screenConvertFarenheit = screenConvertMiles; // FIXME, should be based on a different eeprom config value
  screenConvertPounds = screenConvertMiles;
}

// void lcd_main_screen(void) {
// 	time();
// 	walk_assist_state();
// //  offroad_mode();
// 	battery_soc();
// 	battery_display();
// 	warnings();
//   up_time();
// 	trip_time();
// 	wheel_speed();
// }

// void wheel_speed(void)
// {
//   uint16_t ui16_wheel_speed = ui_vars.ui16_wheel_speed_x10;

//   // reset otherwise at startup this value goes crazy
//   if (ui8_g_motorVariablesStabilized == 0)
//     ui16_wheel_speed = 0;

//   ui8_m_wheel_speed_integer = (uint8_t) (ui16_wheel_speed / 10);
//   ui8_m_wheel_speed_decimal = (uint8_t) (ui16_wheel_speed % 10);

// #ifdef SW102
//   // if we are inside the timeout, override the wheel speed value so assist level is shown there
//   if (m_assist_level_change_timeout > 0) {
//     m_assist_level_change_timeout--;
//     ui8_m_wheel_speed_integer = ui_vars.ui8_assist_level;
//   }
// #endif
// }

// void alternatField(void) {
//   static const char str_max_power[] = "max power";
//   static const char str_throttle[] = "throttle";

//   switch (ui8_m_alternate_field_state) {
//     case 1:
// #ifndef SW102
//       assistLevelField.rw->visibility = FieldTransitionNotVisible;
// #else
//       wheelSpeedIntegerField.rw->visibility = FieldTransitionNotVisible;
// #endif
//       ui8_m_alternate_field_state = 2;

// #ifndef SW102
//       UG_SetBackcolor(C_BLACK);
//       UG_SetForecolor(MAIN_SCREEN_FIELD_LABELS_COLOR);
//       UG_FontSelect(&FONT_10X16);
//       UG_PutString(15, 46, "      ");
// #endif
//       break;

//     case 2:
//       updateReadOnlyLabelStr(&fieldAlternate, str_max_power);
//       fieldAlternate.rw->visibility = FieldTransitionVisible;
//       mainScreenOnDirtyClean();
//       ui8_m_alternate_field_state = 3;
//       break;

//     case 3:
//       // keep updating the variable to show on display
//       ui16_m_alternate_field_value = ((uint16_t) ui_vars.ui8_target_max_battery_power_div25) * 25;
//       break;

//     case 4:
//       fieldAlternate.rw->visibility = FieldTransitionNotVisible;
//       ui8_m_alternate_field_state = 5;
//       break;

//     case 5:
// #ifndef SW102
//       assistLevelField.rw->visibility = FieldTransitionVisible;
// #else
//       wheelSpeedIntegerField.rw->visibility = FieldTransitionVisible;
// #endif
//       mainScreenOnDirtyClean();
//       ui8_m_alternate_field_state = 0;
//       break;

//     case 6:
//       updateReadOnlyLabelStr(&fieldAlternate, str_throttle);
//       mainScreenOnDirtyClean();
//       ui8_m_alternate_field_state = 7;
//       break;

//     case 7:
//       // keep updating the variable to show on display
//       ui16_m_alternate_field_value = (uint16_t) ui_vars.ui8_throttle_virtual;
//       break;
//   }
// }

// void streetMode(void) {
//   if (ui_vars.ui8_street_mode_function_enabled)
//   {
//     ui_vars.ui8_street_mode_power_limit_div25 = (ui_vars.ui16_street_mode_power_limit / 25);
//   }
// }

// void screen_clock(void) {
//   static int counter_time_ms = 0;
//   int time_ms = 0;

//   // No point to processing less than every 100ms, as the data comming from the motor is only updated every 100ms, not less
//   time_ms = get_time_base_counter_1ms();
//   if((time_ms - counter_time_ms) >= 100) // not least than evey 100ms
//   {
//     counter_time_ms = time_ms;

//     // exchange data from realtime layer to UI layer
//     // do this in atomic way, disabling the real time layer (should be no problem as
//     // copy_rt_to_ui_vars() should be fast and take a small piece of the 100ms periodic realtime layer processing
//     rt_processing_stop();
//     copy_rt_to_ui_vars();
//     rt_processing_start();

//     lcd_main_screen();
// #ifndef SW102
//     clock_time();
// #endif
//     DisplayResetToDefaults();
//     TripMemoriesReset();
//     DisplayResetBluetoothPeers();
//     batteryTotalWh();
//     batteryCurrent();
//     batteryResistance();
//     motorCurrent();
//     batteryPower();
//     pedalPower();
//     alternatField();
//     streetMode();
// #ifndef SW102
//     thresholds();
// #endif
//     screenUpdate();
//   }
// }

// void thresholds(void) {
// #ifndef SW102

//   odoField.rw->editable.number.auto_thresholds = FIELD_THRESHOLD_DISABLED;
//   odoFieldGraph.rw->editable.number.auto_thresholds = FIELD_THRESHOLD_DISABLED;
//   tripADistanceField.rw->editable.number.auto_thresholds = FIELD_THRESHOLD_DISABLED;
//   tripDistanceFieldGraph.rw->editable.number.auto_thresholds = FIELD_THRESHOLD_DISABLED;
//   batteryPowerUsageField.rw->editable.number.auto_thresholds = FIELD_THRESHOLD_DISABLED;
//   batteryPowerUsageFieldGraph.rw->editable.number.auto_thresholds = FIELD_THRESHOLD_DISABLED;

//   if (*wheelSpeedField.rw->editable.number.auto_thresholds == FIELD_THRESHOLD_AUTO) {
//     wheelSpeedField.rw->editable.number.error_threshold =
//         wheelSpeedFieldGraph.rw->editable.number.error_threshold = ui_vars.wheel_max_speed_x10;
//     wheelSpeedField.rw->editable.number.warn_threshold =
//         wheelSpeedFieldGraph.rw->editable.number.warn_threshold = ui_vars.wheel_max_speed_x10 - (ui_vars.wheel_max_speed_x10 / 5); // -20%
//   } else if (*wheelSpeedField.rw->editable.number.auto_thresholds == FIELD_THRESHOLD_MANUAL) {
//     wheelSpeedField.rw->editable.number.error_threshold =
//         wheelSpeedFieldGraph.rw->editable.number.error_threshold = *wheelSpeedField.rw->editable.number.config_error_threshold;
//     wheelSpeedField.rw->editable.number.warn_threshold =
//         wheelSpeedFieldGraph.rw->editable.number.warn_threshold = *wheelSpeedField.rw->editable.number.config_warn_threshold;
//   }

//   if (g_graphVars[VarsWheelSpeed].auto_max_min == GRAPH_AUTO_MAX_MIN_SEMI_AUTO) {
//     g_graphVars[VarsWheelSpeed].max = ui_vars.wheel_max_speed_x10;
//     // forcing 0 to min, this way the max will adjust automatically if is higher
//     g_graphVars[VarsWheelSpeed].min = 0;
//   }

//   if (*cadenceField.rw->editable.number.auto_thresholds == FIELD_THRESHOLD_AUTO) {
//     if (ui_vars.ui8_torque_sensor_calibration_feature_enabled) {
//       cadenceField.rw->editable.number.error_threshold =
//         cadenceFieldGraph.rw->editable.number.error_threshold = 120; // max value for motor assistance
//       cadenceField.rw->editable.number.warn_threshold =
//         cadenceFieldGraph.rw->editable.number.warn_threshold = 100;
//     } else {
//       cadenceField.rw->editable.number.error_threshold =
//         cadenceFieldGraph.rw->editable.number.error_threshold = 92;
//       cadenceField.rw->editable.number.warn_threshold =
//         cadenceFieldGraph.rw->editable.number.warn_threshold = 83; // -10%
//     }
//   } else if (*cadenceField.rw->editable.number.auto_thresholds == FIELD_THRESHOLD_MANUAL) {
//     cadenceField.rw->editable.number.error_threshold =
//         cadenceFieldGraph.rw->editable.number.error_threshold = *cadenceField.rw->editable.number.config_error_threshold;
//     cadenceField.rw->editable.number.warn_threshold =
//         cadenceFieldGraph.rw->editable.number.warn_threshold = *cadenceField.rw->editable.number.config_warn_threshold;
//   }

//   if (*humanPowerField.rw->editable.number.auto_thresholds == FIELD_THRESHOLD_MANUAL) {
//     humanPowerField.rw->editable.number.error_threshold =
//         humanPowerFieldGraph.rw->editable.number.error_threshold = *humanPowerField.rw->editable.number.config_error_threshold;
//     humanPowerField.rw->editable.number.warn_threshold =
//         humanPowerFieldGraph.rw->editable.number.warn_threshold = *humanPowerField.rw->editable.number.config_warn_threshold;
//   }

//   if (*batteryPowerField.rw->editable.number.auto_thresholds == FIELD_THRESHOLD_MANUAL) {
//     batteryPowerField.rw->editable.number.error_threshold =
//         batteryPowerFieldGraph.rw->editable.number.error_threshold = *batteryPowerField.rw->editable.number.config_error_threshold;
//     batteryPowerField.rw->editable.number.warn_threshold =
//         batteryPowerFieldGraph.rw->editable.number.warn_threshold = *batteryPowerField.rw->editable.number.config_warn_threshold;
//   }

//   if (*batteryVoltageField.rw->editable.number.auto_thresholds == FIELD_THRESHOLD_AUTO) {
//     int32_t temp = (int32_t) ui_vars.ui16_battery_low_voltage_cut_off_x10;
//     batteryVoltageField.rw->editable.number.error_threshold =
//         batteryVoltageFieldGraph.rw->editable.number.error_threshold = temp;
//     temp *= 10;
//     batteryVoltageField.rw->editable.number.warn_threshold =
//         batteryVoltageFieldGraph.rw->editable.number.warn_threshold = (temp + (temp / 20)) / 10; // -5%
//   } else if (*batteryVoltageField.rw->editable.number.auto_thresholds == FIELD_THRESHOLD_MANUAL) {
//     batteryVoltageField.rw->editable.number.error_threshold =
//         batteryVoltageFieldGraph.rw->editable.number.error_threshold = *batteryVoltageField.rw->editable.number.config_error_threshold;
//     batteryVoltageField.rw->editable.number.warn_threshold =
//         batteryVoltageFieldGraph.rw->editable.number.warn_threshold = *batteryVoltageField.rw->editable.number.config_warn_threshold;
//   }

//   if (g_graphVars[VarsBatteryVoltage].auto_max_min == GRAPH_AUTO_MAX_MIN_SEMI_AUTO) {
//     g_graphVars[VarsBatteryVoltage].min = ui_vars.ui16_battery_low_voltage_cut_off_x10;
//     // forcing the same value as the min, this way the max will adjust automatically if is higher
//     g_graphVars[VarsBatteryVoltage].max = g_graphVars[VarsBatteryVoltage].min;
//   }

//   if (*batteryCurrentField.rw->editable.number.auto_thresholds == FIELD_THRESHOLD_AUTO) {
//     int32_t temp = (int32_t) ui_vars.ui8_battery_max_current * 10;
//     batteryCurrentField.rw->editable.number.error_threshold =
//         batteryCurrentFieldGraph.rw->editable.number.error_threshold = temp;
//     temp *= 10; // current_x10 * 10
//     batteryCurrentField.rw->editable.number.warn_threshold =
//         batteryCurrentFieldGraph.rw->editable.number.warn_threshold = (temp - (temp / 10)) / 10; // -10%
//   } else if (*batteryCurrentField.rw->editable.number.auto_thresholds == FIELD_THRESHOLD_MANUAL) {
//     batteryCurrentField.rw->editable.number.error_threshold =
//         batteryCurrentFieldGraph.rw->editable.number.error_threshold = *batteryCurrentField.rw->editable.number.config_error_threshold;
//     batteryCurrentField.rw->editable.number.warn_threshold =
//         batteryCurrentFieldGraph.rw->editable.number.warn_threshold = *batteryCurrentField.rw->editable.number.config_warn_threshold;
//   }

//   if (g_graphVars[VarsBatteryCurrent].auto_max_min == GRAPH_AUTO_MAX_MIN_SEMI_AUTO) {
//     g_graphVars[VarsBatteryCurrent].max = ((uint32_t) ui_vars.ui8_battery_max_current) * 10;
//     g_graphVars[VarsBatteryCurrent].min = 0;
//   }

//   if (*motorCurrentField.rw->editable.number.auto_thresholds == FIELD_THRESHOLD_AUTO) {
//     int32_t temp = (int32_t) ui_vars.ui8_motor_max_current * 10;
//     motorCurrentField.rw->editable.number.error_threshold =
//         motorCurrentFieldGraph.rw->editable.number.error_threshold = temp;
//     temp *= 10; // current_x10 * 10
//     motorCurrentField.rw->editable.number.warn_threshold =
//         motorCurrentFieldGraph.rw->editable.number.warn_threshold = (temp - (temp / 10)) / 10; // -10%
//   } else if (*motorCurrentField.rw->editable.number.auto_thresholds == FIELD_THRESHOLD_MANUAL) {
//     motorCurrentField.rw->editable.number.error_threshold =
//         motorCurrentFieldGraph.rw->editable.number.error_threshold = *motorCurrentField.rw->editable.number.config_error_threshold;
//     motorCurrentField.rw->editable.number.warn_threshold =
//         motorCurrentFieldGraph.rw->editable.number.warn_threshold = *motorCurrentField.rw->editable.number.config_warn_threshold;
//   }

//   if (g_graphVars[VarsMotorCurrent].auto_max_min == GRAPH_AUTO_MAX_MIN_SEMI_AUTO) {
//     g_graphVars[VarsMotorCurrent].max = ((uint32_t) ui_vars.ui8_motor_max_current) * 10;
//     g_graphVars[VarsMotorCurrent].min = 0;
//   }

//   if (*batterySOCField.rw->editable.number.auto_thresholds == FIELD_THRESHOLD_AUTO) {
//     batterySOCField.rw->editable.number.error_threshold =
//         batterySOCFieldGraph.rw->editable.number.error_threshold = 10;
//     batterySOCField.rw->editable.number.warn_threshold =
//         batterySOCFieldGraph.rw->editable.number.warn_threshold = 25;
//   } else if (*batterySOCField.rw->editable.number.auto_thresholds == FIELD_THRESHOLD_MANUAL) {
//     batterySOCField.rw->editable.number.error_threshold =
//         batterySOCFieldGraph.rw->editable.number.error_threshold = *batterySOCField.rw->editable.number.config_error_threshold;
//     batterySOCField.rw->editable.number.warn_threshold =
//         batterySOCFieldGraph.rw->editable.number.warn_threshold = *batterySOCField.rw->editable.number.config_warn_threshold;
//   }

//   if (*motorTempField.rw->editable.number.auto_thresholds == FIELD_THRESHOLD_AUTO) {
//     motorTempField.rw->editable.number.error_threshold =
//         motorTempFieldGraph.rw->editable.number.error_threshold = (int32_t) ui_vars.ui8_motor_temperature_max_value_to_limit;
//     motorTempField.rw->editable.number.warn_threshold =
//         motorTempFieldGraph.rw->editable.number.warn_threshold = (int32_t) ui_vars.ui8_motor_temperature_min_value_to_limit;
//   } else if (*motorTempField.rw->editable.number.auto_thresholds == FIELD_THRESHOLD_MANUAL) {
//     motorTempField.rw->editable.number.error_threshold =
//         motorTempFieldGraph.rw->editable.number.error_threshold = *motorTempField.rw->editable.number.config_error_threshold;
//     motorTempField.rw->editable.number.warn_threshold =
//         motorTempFieldGraph.rw->editable.number.error_threshold = *motorTempField.rw->editable.number.config_warn_threshold;
//   }

//   if (g_graphVars[VarsMotorTemp].auto_max_min == GRAPH_AUTO_MAX_MIN_SEMI_AUTO) {
//     g_graphVars[VarsMotorTemp].max = ui_vars.ui8_motor_temperature_max_value_to_limit;
//     g_graphVars[VarsMotorTemp].min = 0;
//   }

//   if (*motorErpsField.rw->editable.number.auto_thresholds == FIELD_THRESHOLD_AUTO) {
//     motorErpsField.rw->editable.number.error_threshold =
//         motorErpsFieldGraph.rw->editable.number.error_threshold = 525;
//     motorErpsField.rw->editable.number.warn_threshold =
//         motorErpsFieldGraph.rw->editable.number.warn_threshold = 473; // -10%
//   } else if (*motorErpsField.rw->editable.number.auto_thresholds == FIELD_THRESHOLD_MANUAL) {
//     motorErpsField.rw->editable.number.error_threshold =
//         motorErpsFieldGraph.rw->editable.number.error_threshold = *motorErpsField.rw->editable.number.config_error_threshold;
//     motorErpsField.rw->editable.number.warn_threshold =
//         motorErpsFieldGraph.rw->editable.number.warn_threshold = *motorErpsField.rw->editable.number.config_warn_threshold;
//   }

//   if (*pwmDutyField.rw->editable.number.auto_thresholds == FIELD_THRESHOLD_AUTO) {
//     if (ui_vars.ui8_torque_sensor_calibration_feature_enabled) {
//       pwmDutyField.rw->editable.number.error_threshold =
//           pwmDutyFieldGraph.rw->editable.number.error_threshold = 111;
//       pwmDutyField.rw->editable.number.warn_threshold =
//           pwmDutyFieldGraph.rw->editable.number.warn_threshold = 99;
//     } else {
//       pwmDutyField.rw->editable.number.error_threshold =
//           pwmDutyFieldGraph.rw->editable.number.error_threshold = 100;
//       pwmDutyField.rw->editable.number.warn_threshold =
//           pwmDutyFieldGraph.rw->editable.number.warn_threshold = 90;
//     }
//   } else if (*pwmDutyField.rw->editable.number.auto_thresholds == FIELD_THRESHOLD_MANUAL) {
//     pwmDutyField.rw->editable.number.error_threshold =
//         pwmDutyFieldGraph.rw->editable.number.error_threshold = *pwmDutyField.rw->editable.number.config_error_threshold;
//     pwmDutyField.rw->editable.number.warn_threshold =
//         pwmDutyFieldGraph.rw->editable.number.warn_threshold = *pwmDutyField.rw->editable.number.config_warn_threshold;
//   }

//   if (g_graphVars[VarsMotorPWM].auto_max_min == GRAPH_AUTO_MAX_MIN_SEMI_AUTO) {
//     if (ui_vars.ui8_torque_sensor_calibration_feature_enabled) {
//       g_graphVars[VarsMotorPWM].max = 111;
//       g_graphVars[VarsMotorPWM].min = 0;
//     } else {
//       g_graphVars[VarsMotorPWM].max = 100;
//       g_graphVars[VarsMotorPWM].min = 0;
//     }
//   }

//   if (*motorFOCField.rw->editable.number.auto_thresholds == FIELD_THRESHOLD_AUTO) {
//     motorFOCField.rw->editable.number.error_threshold =
//         motorFOCFieldGraph.rw->editable.number.error_threshold = 8;
//     motorFOCField.rw->editable.number.warn_threshold =
//         motorFOCFieldGraph.rw->editable.number.warn_threshold = 6; // -20%
//   } else if (*motorFOCField.rw->editable.number.auto_thresholds == FIELD_THRESHOLD_MANUAL) {
//     motorFOCField.rw->editable.number.error_threshold =
//         motorFOCFieldGraph.rw->editable.number.error_threshold = *motorFOCField.rw->editable.number.config_error_threshold;
//     motorFOCField.rw->editable.number.warn_threshold =
//         motorFOCFieldGraph.rw->editable.number.warn_threshold = *motorFOCField.rw->editable.number.config_warn_threshold;
//   }
// #endif
// }

// void up_time(void) {
// 	rtc_time_t *p_time = rtc_get_time_since_startup();
// 	static int oldmin = -1; // used to prevent unneeded updates
// 	char timestr[MAX_TIMESTR_LEN]; // 12:13

// 	if (p_time->ui8_minutes != oldmin) {
// 		oldmin = p_time->ui8_minutes;
// 		sprintf(timestr, "%d:%02d", p_time->ui8_hours, p_time->ui8_minutes);
// 		updateReadOnlyStr(&upTimeField, timestr);
// 	}
// }

// void trip_time(void){
//   updateTripTime(ui_vars.ui32_trip_a_time, &tripATimeField);
//   updateTripTime(ui_vars.ui32_trip_b_time, &tripBTimeField);
// }

// void updateTripTime(uint32_t tripTime, Field *field) {
//   char timestr[MAX_TIMESTR_LEN]; // 12:13
//   uint32_t ui32_temp = tripTime % 86399; // 86399 = seconds in 1 day minus 1s

//   // Calculate trip time
//   uint8_t hours = ui32_temp / 3600;
//   uint8_t minutes = (ui32_temp % 3600) / 60;
//   uint8_t seconds = (ui32_temp % 3600) % 60;

//   if(hours > 0)  
//     sprintf(timestr, "%d:%02d", hours, minutes);
//   else
//     sprintf(timestr, "%d:%02d", minutes, seconds);

//   //sprintf(timestr, "%d:%02d:%02d", hours, minutes, seconds);

//   if(strcmp(field->editable.target, timestr) != 0)
//     updateReadOnlyStr(field, timestr);
// }

// static ColorOp warnColor = ColorNormal;
// static char warningStr[MAX_FIELD_LEN];

// // We use a custom callback so we can reuse the standard drawtext code, but with a dynamically changing color
// static bool renderWarning(FieldLayout *layout) {
// 	layout->color = warnColor;
// 	return renderDrawTextCommon(layout, warningStr);
// }

// void setWarning(ColorOp color, const char *str) {
// 	warnColor = color;
// 	warnField.rw->blink = (color == ColorError);
// 	warnField.rw->dirty = (strcmp(str, warningStr) != 0);
// 	if(warnField.rw->dirty)
// 		strncpy(warningStr, str, sizeof(warningStr));
// }

// static const char *motorErrors[] = { _S("None", "None"), _S("Motor init", "Motor init"), "Motor Blocked", "Torque Fault", "Brake Fault", "Throttle Fault", "Speed Fault", "Low Volt", "Comms"};

// void warnings(void) {
//   uint32_t motor_temp_limit = ui_vars.ui8_temperature_limit_feature_enabled & 1;
//   uint8_t ui8_motorErrorsIndex;

//   switch (g_motor_init_state) {
//     case MOTOR_INIT_ERROR_SET_CONFIGURATIONS:
//       setWarning(ColorError, _S("Error set config", "e: config"));
//       return;

//     case MOTOR_INIT_WAIT_CONFIGURATIONS_OK:
//     case MOTOR_INIT_WAIT_GOT_CONFIGURATIONS_OK:
//       setWarning(ColorWarning, _S("Motor init", "Motor init"));
//       return;
//   }

// 	// High priorty faults in red
//   if (ui_vars.ui8_error_states) {
//     if (ui_vars.ui8_error_states & 1)
//       ui8_motorErrorsIndex = 1;
//     else if (ui_vars.ui8_error_states & 2)
// //      ui8_motorErrorsIndex = 2; // ignore this error for now
//       return;
//     else if (ui_vars.ui8_error_states & 4)
//       ui8_motorErrorsIndex = 3;
//     else if (ui_vars.ui8_error_states & 8)
//       ui8_motorErrorsIndex = 4;
//     else if (ui_vars.ui8_error_states & 16)
//       ui8_motorErrorsIndex = 5;
//     else if (ui_vars.ui8_error_states & 32)
//       ui8_motorErrorsIndex = 6;
//     else if (ui_vars.ui8_error_states & 64)
//       ui8_motorErrorsIndex = 7;
//     else if (ui_vars.ui8_error_states & 128)
//       ui8_motorErrorsIndex = 8;

//     char str[24];
//     snprintf(str, sizeof(str), "%s%d%s%s", "e: ", ui8_motorErrorsIndex, " ", motorErrors[ui8_motorErrorsIndex]);
// 		setWarning(ColorError, str);
// 		return;
// 	}

// 	if (motor_temp_limit &&
// 	    ui_vars.ui8_motor_temperature >= ui_vars.ui8_motor_temperature_max_value_to_limit) {
// 		setWarning(ColorError, _S("Temp Shutdown", "Temp Shut"));
// 		return;
// 	}

// 	// If we had a watchdog failure, show it forever - so user will report a bug
// 	if (wd_failure_detected) {
//     setWarning(ColorError, "Report Bug!");
//     return;
// 	}

// 	// warn faults in yellow
//   if (motor_temp_limit &&
//       ui_vars.ui8_motor_temperature >= ui_vars.ui8_motor_temperature_min_value_to_limit) {
// 		setWarning(ColorWarning, _S("Temp Warning", "Temp Warn"));
// 		return;
// 	}

// 	// All of the following possible 'faults' are low priority

// 	if (ui_vars.ui8_braking) {
// 		setWarning(ColorNormal, "BRAKE");
// 		return;
// 	}

// 	if(ui_vars.ui8_walk_assist) {
// 		setWarning(ColorNormal, "WALK");
// 		return;
// 	}

// 	if (ui_vars.ui8_lights) {
// 		setWarning(ColorNormal, "LIGHT");
// 		return;
// 	}

// 	setWarning(ColorNormal, "");
// }

// void battery_soc(void) {
//   switch (ui_vars.ui8_battery_soc_enable) {
//     default:
//     case 0:
//       // clear the area
//       fieldPrintf(&socField, "");
//       break;

//     case 1:
//       fieldPrintf(&socField, "%3d%%", ui8_g_battery_soc);
//       break;

//     case 2:
//       fieldPrintf(&socField, "%u.%1uV",
//           ui_vars.ui16_battery_voltage_soc_x10 / 10,
//           ui_vars.ui16_battery_voltage_soc_x10 % 10);
//       break;
//   }
// }


// void time(void) {
// #ifndef SW102
//   rtc_time_t *p_rtc_time = rtc_get_time();

//   switch (ui_vars.ui8_time_field_enable) {
//     default:
//     case 0:
//       // clear the area
//       fieldPrintf(&timeField, "");
//       break;

//     case 1:
//       // force to be [0 - 12]
//       if (ui_vars.ui8_units_type) { // FIXME, should be based on a different eeprom config value, just because someone is using mph doesn't mean they want 12 hr time
//         if (p_rtc_time->ui8_hours > 12) {
//           p_rtc_time->ui8_hours -= 12;
//         }
//       }

//       fieldPrintf(&timeField, "%d:%02d", p_rtc_time->ui8_hours,
//           p_rtc_time->ui8_minutes);
//       break;

//     case 2:
//       fieldPrintf(&timeField, "%3d%%", ui8_g_battery_soc);
//       break;

//     case 3:
//       fieldPrintf(&timeField, "%u.%1uV",
//           ui_vars.ui16_battery_voltage_soc_x10 / 10,
//           ui_vars.ui16_battery_voltage_soc_x10 % 10);
//       break;
//   }
// #endif
// }

// void walk_assist_state(void) {
// // kevinh - note on the sw102 we show WALK in the box normally used for BRAKE display - the display code is handled there now
//   if (ui_vars.ui8_walk_assist_feature_enabled) {
//     // if down button is still pressed
//     if (ui_vars.ui8_walk_assist && buttons_get_down_state()) {
//       ui8_walk_assist_timeout = 2; // 0.2 seconds
//     } else if (buttons_get_down_state() == 0 && --ui8_walk_assist_timeout == 0) {
//       ui_vars.ui8_walk_assist = 0;
//     }
//   } else {
//     ui_vars.ui8_walk_assist = 0;
//   }
// }

// // Screens in a loop, shown when the user short presses the power button
// extern Screen *screens[];

// void showNextScreen() {
//   g_showNextScreenPreviousIndex = g_showNextScreenIndex;

//   // increase to index of next screen
//   if (screens[++g_showNextScreenIndex] == NULL) {
//     g_showNextScreenIndex = 0;
//   }

// 	screenShow(screens[g_showNextScreenIndex]);
// }

// static bool appwide_onpress(buttons_events_t events)
// {
//   // power off only after we release first time the onoff button
//   if (events & ONOFF_LONG_CLICK)
//   {
//     lcd_power_off(1);
//     return true;
//   }

//   if ((events & SCREENCLICK_NEXT_SCREEN) &&
//       ((g_motor_init_state == MOTOR_INIT_READY) ||
//       (g_motor_init_state == MOTOR_INIT_SIMULATING))) {
//     showNextScreen();
//     return true;
//   }

//   if (events & SCREENCLICK_ENTER_CONFIGURATIONS) {
//     screenShow(&configScreen);
//     return true;
//   }

// 	return false;
// }

// /// Called every 20ms to check for button events and dispatch to our handlers
// static void handle_buttons() {

//   static uint8_t firstTime = 1;

//   // keep tracking of first time release of onoff button
//   if(firstTime && buttons_get_onoff_state() == 0) {
//     firstTime = 0;
//     buttons_clear_onoff_click_event();
//     buttons_clear_onoff_long_click_event();
//     buttons_clear_onoff_click_long_click_event();
//   }

//   if (ui8_m_alternate_field_state == 7) { // if virtual throttle mode
//     if (buttons_get_up_state() == 0 && // UP and DOWN buttons not pressed
//             buttons_get_down_state() == 0) {
//       if (ui8_m_alternate_field_timeout_cnt) {
//         ui8_m_alternate_field_timeout_cnt--;
//       } else {
//         ui8_m_vthrottle_can_increment_decrement = 0;
//         ui_vars.ui8_throttle_virtual = 0;
//       }
//     } else {
//       ui8_m_alternate_field_timeout_cnt = 50;
//     }
//   }

//   if (buttons_events && firstTime == 0)
//   {
//     bool handled = false;

// 		if (!handled)
// 			handled |= screenOnPress(buttons_events);

// 		// Note: this must be after the screen/menu handlers have had their shot
// 		if (!handled)
// 			handled |= appwide_onpress(buttons_events);

// 		if (handled)
// 			buttons_clear_all_events();
// 	}

// 	buttons_clock(); // Note: this is done _after_ button events is checked to provide a 20ms debounce
// }

// /// Call every 20ms from the main thread.
// void main_idle() {
//   static int counter_time_ms = 0;
//   int time_ms = 0;

//   // no point to processing less than every 100ms, as the data comming from the motor is only updated every 100ms, not less
//   time_ms = get_time_base_counter_1ms();
//   if((time_ms - counter_time_ms) >= 100) // not least than evey 100ms
//   {
//     counter_time_ms = time_ms;
//     automatic_power_off_management();
//   }

// 	handle_buttons();
// 	screen_clock(); // This is _after_ handle_buttons so if a button was pressed this tick, we immediately update the GUI
// }

// void batteryTotalWh(void) {

//   ui32_g_configuration_wh_100_percent = ui_vars.ui32_wh_x10_100_percent / 10;
// }

// void onSetConfigurationBatteryTotalWh(uint32_t v) {

//   ui_vars.ui32_wh_x10_100_percent = v * 10;
// }

// void DisplayResetToDefaults(void) {

//   if (ui8_g_configuration_display_reset_to_defaults) {
//     ui8_g_configuration_display_reset_to_defaults = 0;
//     eeprom_init_defaults();
//   }
// }

// void TripMemoriesReset(void) {
//   if (ui8_g_configuration_trip_a_reset) {
//     ui8_g_configuration_trip_a_reset = 0;

//     uint32_t current_time = RTC_GetCounter();

//     rt_vars.ui32_trip_a_last_update_time = current_time;
//     rt_vars.ui32_trip_a_distance_x1000 = 0;
//     rt_vars.ui32_trip_a_time = 0;
//     rt_vars.ui16_trip_a_avg_speed_x10 = 0;
//     rt_vars.ui16_trip_a_max_speed_x10 = 0;
//   }

//   if (ui8_g_configuration_trip_b_reset) {
//     ui8_g_configuration_trip_b_reset = 0;

//     uint32_t current_time = RTC_GetCounter();

//     rt_vars.ui32_trip_b_last_update_time = current_time;
//     rt_vars.ui32_trip_b_distance_x1000 = 0;
//     rt_vars.ui32_trip_b_time = 0;
//     rt_vars.ui16_trip_b_avg_speed_x10 = 0;
//     rt_vars.ui16_trip_b_max_speed_x10 = 0;
//   }
// }

// void DisplayResetBluetoothPeers(void) {
// #ifdef SW102
//   if (ui8_g_configuration_display_reset_bluetooth_peers) {
//     ui8_g_configuration_display_reset_bluetooth_peers = 0;
//     // TODO: fist disable any connection
//     // Warning: Use this (pm_peers_delete) function only when not connected or connectable. If a peer is or becomes connected
//     // or a PM_PEER_DATA_FUNCTIONS function is used during this procedure (until the success or failure event happens),
//     // the behavior is undefined.
//     pm_peers_delete();
//   }
// #endif
// }

// void batteryCurrent(void) {

//   ui16_m_battery_current_filtered_x10 = ui_vars.ui16_battery_current_filtered_x5 * 2;
// }

// void batteryResistance(void) {

//   typedef enum {
//     WAIT_MOTOR_STOP = 0,
//     STARTUP = 1,
//     DELAY = 2,
//     CALC_RESISTANCE = 3,
//   } state_t;

//   static state_t state = WAIT_MOTOR_STOP;
//   static uint8_t ui8_counter;
//   static uint16_t ui16_batt_voltage_init_x10;
//   uint16_t ui16_batt_voltage_final_x10;
//   uint16_t ui16_batt_voltage_delta_x10;
//   uint16_t ui16_batt_current_final_x5;

//   switch (state) {
//     case WAIT_MOTOR_STOP:
//       // wait for motor stop to measure battery initial voltage
//       if (ui_vars.ui16_motor_current_filtered_x5 == 0) {
//         ui16_batt_voltage_init_x10 = ui_vars.ui16_battery_voltage_filtered_x10;
//         ui8_counter = 0;
//         state = STARTUP;
//       }
//       break;

//     case STARTUP:
//       // wait for motor running and at high battery current
//       if ((ui_vars.ui16_motor_speed_erps > 10) &&
//           (ui_vars.ui16_battery_current_filtered_x5 > (2 * 5))) {
//         ui8_counter = 0;
//         state = DELAY;
//       } else {

//         if (++ui8_counter > 50) // wait 5 seconds on this state
//           state = WAIT_MOTOR_STOP;
//       }
//       break;

//     case DELAY:
//       if (ui_vars.ui16_battery_current_filtered_x5 > (2 * 5)) {

//         if (++ui8_counter > 40) // sample battery final voltage after 4 seconds
//           state = CALC_RESISTANCE;

//       } else {
//         state = WAIT_MOTOR_STOP;
//       }
//       break;

//     case CALC_RESISTANCE:
//       ui16_batt_voltage_final_x10 = ui_vars.ui16_battery_voltage_filtered_x10;
//       ui16_batt_current_final_x5 = ui_vars.ui16_battery_current_filtered_x5;

//       if (ui16_batt_voltage_init_x10 > ui16_batt_voltage_final_x10) {
//         ui16_batt_voltage_delta_x10 = ui16_batt_voltage_init_x10 - ui16_batt_voltage_final_x10;
//       } else {
//         ui16_batt_voltage_delta_x10 = 0;
//       }

//       // R = U / I
//       ui_vars.ui16_battery_pack_resistance_estimated_x1000 =
//           (ui16_batt_voltage_delta_x10 * 500) / ui16_batt_current_final_x5 ;

//       state = WAIT_MOTOR_STOP;
//       break;
//   }

// }

// void motorCurrent(void) {

//   ui16_m_motor_current_filtered_x10 = ui_vars.ui16_motor_current_filtered_x5 * 2;
// }

// void onSetConfigurationWheelOdometer(uint32_t v) {

//   // let's update the main variable used for calculations of odometer
//   rt_vars.ui32_odometer_x10 = v;
// }

// void batteryPower(void) {

//   ui16_m_battery_power_filtered = ui_vars.ui16_battery_power;

//   // loose resolution under 200W
//   if (ui16_m_battery_power_filtered < 200) {
//     ui16_m_battery_power_filtered /= 10;
//     ui16_m_battery_power_filtered *= 10;
//   }
//   // loose resolution under 400W
//   else if (ui16_m_battery_power_filtered < 500) {
//     ui16_m_battery_power_filtered /= 20;
//     ui16_m_battery_power_filtered *= 20;
//   }
// }

// void pedalPower(void) {

//   ui16_m_pedal_power_filtered = ui_vars.ui16_pedal_power;

//   if (ui16_m_pedal_power_filtered > 500) {
//     ui16_m_pedal_power_filtered /= 20;
//     ui16_m_pedal_power_filtered *= 20;
//   } else if (ui16_m_pedal_power_filtered > 200) {
//     ui16_m_pedal_power_filtered /= 10;
//     ui16_m_pedal_power_filtered *= 10;
//   } else if (ui16_m_pedal_power_filtered > 10) {
//     ui16_m_pedal_power_filtered /= 5;
//     ui16_m_pedal_power_filtered *= 5;
//   }
// }

// void onSetConfigurationBatterySOCUsedWh(uint32_t v) {
//   reset_wh();
//   ui_vars.ui32_wh_x10_offset = v;
// }
