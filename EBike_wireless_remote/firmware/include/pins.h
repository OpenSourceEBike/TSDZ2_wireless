/*
 * TSDZ2 EBike wireless firmware
 *
 * Copyright (C) Casainho, 2020
 *
 * Released under the GPL License, Version 3
 */

#ifndef PINS_H_
#define PINS_H_
#include "boards.h"
#include <stdio.h>
#include <stdbool.h>

#if defined(BOARD_CUSTOM) && defined(NRF52840_MDK_USB_DONGLE)
// NRF52840 MakerDiary Dongle

typedef enum {
    PLUS__PIN  = 5,
    MINUS__PIN  = 6,
    ENTER__PIN = 7,
    STANDBY__PIN = 8,
    BUTTON_PIN_ELEMENTS = 4 // must be updated when added or removed an element
} button_pins_t;

typedef enum {
    LED_G__PIN  = 0,
    LED_R__PIN  = 1,
    LED_B__PIN  = 2,
} LED_pins_t;

#elif defined(BOARD_PCA10059)

// NRF52840 NORDIC Dongle

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

#endif

void pins_init(void);
bool button_plus_is_set(void);
bool button_minus_is_set(void);
bool button_enter_is_set(void);
bool button_standby_is_set(void);
bool button_bootloader_is_set(void);

#endif /* PINS_H_ */
