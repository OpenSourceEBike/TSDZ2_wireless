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




// 24de8869-f64e-4e70-a72b-799f5ef05345

#define ANT_ID_UUID_BASE        {0x45, 0x53, 0xF0, 0x5E, 0x9F, 0x79, 0x2B, 0xA7, \
                                0x70, 0x4E, 0x4E, 0xF6, 0x69, 0x88, 0xDE, 0x24}
#define ANT_ID_UUID_SERVICE     0x1500
#define ANT_ID_UUID_LED_CHAR    0x1501

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
