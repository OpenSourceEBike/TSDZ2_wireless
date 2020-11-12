#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "ble.h"
#include "ble_srv_common.h"
#include "nrf_sdh_ble.h"

#ifdef __cplusplus
extern "C" {
#endif

#define BLE_TSDZ2_PERIODIC_LEN 10
#define BLE_TSDZ2_CONFIGURATIONS_LEN 161

#define BLE_ANT_ID_BLE_OBSERVER_PRIO 2
#define BLE_TSDZ2_BLE_OBSERVER_PRIO 2

#define BLE_ANT_ID_DEF(_name)                                                                          \
static ble_ant_id_t _name;                                                                             \
NRF_SDH_BLE_OBSERVER(_name ## _obs,                                                                 \
                     BLE_ANT_ID_BLE_OBSERVER_PRIO,                                                     \
                     ble_ant_id_on_ble_evt, &_name)

#define BLE_TSDZ2_DEF(_name)                                                                          \
static ble_tsdz2_t _name;                                                                             \
NRF_SDH_BLE_OBSERVER(_name ## _obs,                                                                 \
                     BLE_TSDZ2_BLE_OBSERVER_PRIO,                                                     \
                     ble_tsdz2_on_ble_evt, &_name)                                     

#define TSDZ2_UUID_BASE                 {0x5f, 0xB4, 0xCB, 0x4C, 0x6E, 0x7F, 0xAF, 0xBF, \
                                        0x2F, 0x46, 0xDD, 0xCF, 0xB0, 0xEA, 0xC2, 0xDA}
#define TSDZ2_UUID_SERVICE              0x1400
#define TSDZ2_PERIODIC_UUID_CHAR        0x1401
#define TSDZ2_CONFIGURATIONS_UUID_CHAR  0x1402

#define ANT_ID_UUID_BASE                {0xf9, 0xAD, 0xE9, 0x68, 0x49, 0x08, 0x40, 0x5C, \
                                        0x9A, 0x0B, 0xD2, 0x4D, 0x31, 0x46, 0xf7, 0x97}
#define ANT_ID_UUID_SERVICE             0x1500
#define ANT_ID_UUID_CHAR                0x1501

typedef struct ble_ant_id_s ble_ant_id_t;
typedef struct ble_tsdz2_s ble_tsdz2_t;

typedef void (*ble_ant_id_write_handler_t) (uint16_t conn_handle, ble_ant_id_t * p_ant_id, uint8_t value);
typedef void (*ble_tsdz2_write_handler_t) (const uint8_t *p_data, uint16_t len);

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

typedef struct
{
  ble_tsdz2_write_handler_t tsdz2_write_handler_periodic;
  ble_tsdz2_write_handler_t tsdz2_write_handler_configurations;
} ble_tsdz2_init_t;

struct ble_tsdz2_s
{
  uint16_t                    service_handle;      
  ble_gatts_char_handles_t    tsdz2_periodic_char_handles;
  ble_gatts_char_handles_t    tsdz2_configurations_char_handles;
  uint8_t                     uuid_type;
  ble_tsdz2_write_handler_t   tsdz2_write_handler_periodic;
  ble_tsdz2_write_handler_t   tsdz2_write_handler_configurations;
};

uint32_t ble_service_ant_id_init(ble_ant_id_t * p_ant_id, const ble_ant_id_init_t * p_ant_id_init);
uint32_t ble_service_tsdz2_init(ble_tsdz2_t * p_tsdz2, const ble_tsdz2_init_t * p_tsdz2_init);
void ble_ant_id_on_ble_evt(ble_evt_t const * p_ble_evt, void * p_context);
void ble_tsdz2_on_ble_evt(ble_evt_t const * p_ble_evt, void * p_context);
uint32_t ble_ant_id_on_change(uint16_t conn_handle, ble_ant_id_t * p_ant_id_t, uint8_t value);
uint32_t ble_tsdz2_periodic_on_change(uint16_t conn_handle, ble_tsdz2_t * p_ble_tsdz2_t, uint8_t *p_value);
uint32_t ble_tsdz2_configurations_on_change(uint16_t conn_handle, ble_tsdz2_t * p_ble_tsdz2_t, uint8_t *p_value);

#ifdef __cplusplus
}
#endif
