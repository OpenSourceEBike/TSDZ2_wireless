#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "ble.h"
#include "ble_srv_common.h"
#include "nrf_sdh_ble.h"

#ifdef __cplusplus
extern "C" {
#endif

#define BLE_ANT_ID_BLE_OBSERVER_PRIO 2

#define BLE_ANT_ID_DEF(_name)                                                                          \
static ble_ant_id_t _name;                                                                             \
NRF_SDH_BLE_OBSERVER(_name ## _obs,                                                                 \
                     BLE_ANT_ID_BLE_OBSERVER_PRIO,                                                     \
                     ble_ant_id_on_ble_evt, &_name)

// dac2eab0-cfdd-462f-bfaf-7f6e4ccbb45f
#define ANT_ID_UUID_BASE        {0x5f, 0xB4, 0xCB, 0x4C, 0x6E, 0x7F, 0xAF, 0xBF, \
                                0x2F, 0x46, 0xDD, 0xCF, 0xB0, 0xEA, 0xC2, 0xDA}
#define ANT_ID_UUID_SERVICE     0x1400
#define ANT_ID_UUID_LED_CHAR    0x1401

typedef struct ble_ant_id_s ble_ant_id_t;

typedef void (*ble_ant_id_write_handler_t) (uint16_t conn_handle, ble_ant_id_t * p_ant_id, uint8_t value);

typedef struct
{
  ble_ant_id_write_handler_t ant_id_write_handler;
} ble_ant_id_init_t;

struct ble_ant_id_s
{
    uint16_t                    service_handle;      
    ble_gatts_char_handles_t    ant_id_char_handles;
    uint8_t                     uuid_type;
    ble_ant_id_write_handler_t  ant_id_write_handler;
};

uint32_t ble_service_ant_id_init(ble_ant_id_t * p_ant_id, const ble_ant_id_init_t * p_ant_id_init);
void ble_ant_id_on_ble_evt(ble_evt_t const * p_ble_evt, void * p_context);
uint32_t ble_ant_id_on_change(uint16_t conn_handle, ble_ant_id_t * p_ant_id_t, uint8_t value);

#ifdef __cplusplus
}
#endif
