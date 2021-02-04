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
#include "boards.h"

#define MOTOR_POWER_ENABLE__PIN             NRF_GPIO_PIN_MAP(1,0)
#define BRAKE__PIN                          9

// UART pins
#define RX_PIN_NUMBER 24
#define TX_PIN_NUMBER 22
#define RTS_PIN_NUMBER 0xFFFFFFFF  // UART_PIN_DISCONNECTED
#define CTS_PIN_NUMBER 0xFFFFFFFF  // UART_PIN_DISCONNECTED

void pins_init(void);
void motor_power_enable(bool state);

typedef enum {
    PLUS__PIN  = 13,
    MINUS__PIN  = 15,
    ENTER__PIN = 17,
    STANDBY__PIN = 20,
    BUTTON_PIN_ELEMENTS = 4 // must be updated when added or removed an element
} button_pins_t;

typedef enum {
    LED_R__PIN  = 1,
    LED_G__PIN  = 2,
    LED_B__PIN  = 3,
    LED_PWR__PIN = 0,
} LED_pins_t;

bool button_plus_is_set(void);
bool button_minus_is_set(void);
bool button_enter_is_set(void);
bool button_standby_is_set(void);
bool button_bootloader_is_set(void);

#endif /* PINS_H_ */
