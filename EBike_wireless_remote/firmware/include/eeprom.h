/*
 * Bafang LCD 850C firmware
 *
 * Copyright (C) Casainho, 2018.
 *
 * Released under the GPL License, Version 3
 */

#ifndef _EEPROM_H_
#define _EEPROM_H_

#pragma pack(4)
typedef struct configurations
{
    uint32_t ui8_bluetooth_flag;
    uint32_t ui8_ant_device_id;
    uint32_t ui8_ant_lev_flag;
    uint32_t ui8_ant_controls_flag;
    uint32_t ui8_brake_flag;
} configurations_t;

void eeprom_init(uint8_t *ant_num, uint8_t *bluetooth,uint8_t *ebike,uint8_t *garmin,uint8_t *brake);
void eeprom_write_variables(uint8_t ant_num, uint8_t bluetooth,uint8_t ebike,uint8_t garmin,uint8_t brake);

#endif /* _EEPROM_H_ */
