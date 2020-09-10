#pragma once

#include "screen.h"

extern Screen mainScreen1, mainScreen2, bootScreen;
extern Screen *screens[];
extern Field
  socField,
  timeField,
  assistLevelField,
  wheelSpeedIntegerField,
  wheelSpeedDecimalField,
  tripTimeField,
  tripDistanceField,
  odoField,
  wheelSpeedField,
  cadenceField,
  humanPowerField,
  batteryPowerField,
  batteryPowerUsageField,
  fieldAlternate,
  batteryVoltageField,
  batteryCurrentField,
  motorCurrentField,
  batterySOCField,
  motorTempField,
  motorErpsField,
  pwmDutyField,
  motorFOCField,
  motorTempGraph,
  bootStatus2,
#ifdef SW102
  custom1, custom2,
  custom3, custom4,
  custom5, custom6,
  warnField; // just close previous definition
#endif
#ifndef SW102 // we don't have any graphs yet on SW102, possibly move this into mainscreen_850.c
  graph1, graph2, graph3,
  *graphs[3],
  custom1, custom2, custom3, custom4,
  custom5, custom6, custom7, custom8,
  custom9, custom10, custom11, custom12,
  warnField,

  wheelSpeedGraph,
  tripDistanceGraph,
  odoGraph,
  cadenceGraph,
  humanPowerGraph,
  batteryPowerGraph,
  batteryPowerUsageGraph,
  batteryPowerUsageFieldGraph,
  batteryVoltageGraph,
  batteryCurrentGraph,
  motorCurrentGraph,
  batterySOCGraph,
  motorTempGraph,
  motorErpsGraph,
  pwmDutyGraph,
  motorFOCGraph;
  void mainScreenOnDirtyClean();
#endif

extern uint8_t g_showNextScreenIndex;
extern uint8_t g_showNextScreenPreviousIndex;

extern Field batteryField; // These fields are custom for board type
void battery_display(); // 850C and sw102 provide alternative versions due to different implementations
void set_conversions();
bool anyscreen_onpress(buttons_events_t events);
void clock_time(void);
void onSetConfigurationClockHours(uint32_t v);
void onSetConfigurationClockMinutes(uint32_t v);
void onSetConfigurationDisplayLcdBacklightOnBrightness(uint32_t v);
void onSetConfigurationDisplayLcdBacklightOffBrightness(uint32_t v);
void onSetConfigurationBatteryTotalWh(uint32_t v);
void onSetConfigurationWheelOdometer(uint32_t v);
void onSetConfigurationBatterySOCUsedWh(uint32_t v);
void mainScreenOnDirtyClean(void);
void secondMainScreenOnDirtyClean(void);
void mainScreenonPostUpdate(void);
bool mainScreenOnPress(buttons_events_t events);
void showNextScreen();
void setWarning(ColorOp color, const char *str);
void screen_clock(void);

/// set to true if this boot was caused because we had a watchdog failure, used to show user the problem in the fault line
extern bool wd_failure_detected;

extern uint8_t ui8_g_configuration_clock_hours;
extern uint8_t ui8_g_configuration_clock_minutes;

