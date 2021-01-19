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
                       
typedef enum {
    PLUS__PIN  = 5,
    MINUS__PIN  = 6,
    ENTER__PIN = 7,
    STANDBY__PIN = 8,
    BOOTLOADER__PIN = 18,
    BUTTON_PIN_ELEMENTS = 5 // must be updated when added or removed an element
} button_pins_t;
#endif
// NRF52840 NORDIC Dongle
#if defined(BOARD_PCA10059)
                       
typedef enum {
    PLUS__PIN  = 13,
    MINUS__PIN  = 15,
    ENTER__PIN = 17,
    STANDBY__PIN = 20,
    BOOTLOADER__PIN= 19,
    BUTTON_PIN_ELEMENTS = 5 // must be updated when added or removed an element
} button_pins_t;
#endif

// NRF52840 Dongle (The Blue One)


void pins_init(void);
bool button_plus_is_set(void);
bool button_minus_is_set(void);
bool button_enter_is_set(void);
bool button_standby_is_set(void);
bool button_bootloader_is_set(void);

#endif /* PINS_H_ */
