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

// UART pins
#define RX_PIN_NUMBER 24
#define TX_PIN_NUMBER 22
#define RTS_PIN_NUMBER 0xFFFFFFFF  // UART_PIN_DISCONNECTED
#define CTS_PIN_NUMBER 0xFFFFFFFF  // UART_PIN_DISCONNECTED

void pins_init(void);
void motor_power_enable(bool state);

#endif /* PINS_H_ */
