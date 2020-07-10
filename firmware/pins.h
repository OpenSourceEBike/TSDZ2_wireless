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

#define MOTOR_POWER_ENABLE__PIN             19

void pins_init(void);
void motor_power_enable(bool state);

#endif /* PINS_H_ */
