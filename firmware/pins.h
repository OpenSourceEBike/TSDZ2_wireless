/*
 * TSDZ2 EBike wireless firmware
 *
 * Copyright (C) Casainho, 2020
 *
 * Released under the GPL License, Version 3
 */

#ifndef PINS_H_
#define PINS_H_

#include <stdio.h>
#include <stdbool.h>

// NRF52840 MDK Dongle
#if defined(BOARD_CUSTOM) && defined(NRF52840_MDK_USB_DONGLE)
#define MOTOR_POWER_ENABLE__PIN             19
#define MOTOR_TX__PIN                       7 
#define MOTOR_RX__PIN                       6
#endif

// NRF52840 Dongle (The Blue One)
#ifdef BOARD_PCA10059
#define MOTOR_POWER_ENABLE__PIN             NRF_GPIO_PIN_MAP(1,0)
#define MOTOR_TX__PIN                       NRF_GPIO_PIN_MAP(0,22)
#define MOTOR_RX__PIN                       NRF_GPIO_PIN_MAP(0,24)
#endif

void pins_init(void);
void motor_power_enable(bool state);

#endif /* PINS_H_ */
