#include "sdk_common.h"
#include "ble_services.h"
#include "ble_srv_common.h"

static void on_write_tsdz2(ble_tsdz2_t * p_tsdz2, ble_evt_t const * p_ble_evt)
{
  ble_gatts_evt_write_t const * p_evt_write = &p_ble_evt->evt.gatts_evt.params.write;

  if ((p_evt_write->handle == p_tsdz2->tsdz2_periodic_char_handles.value_handle)
    && (p_evt_write->len == BLE_TSDZ2_PERIODIC_LEN)
    && (p_tsdz2->tsdz2_write_handler_periodic != NULL))
  {
    p_tsdz2->tsdz2_write_handler_periodic(p_evt_write->data, p_evt_write->len);
  }

  if ((p_evt_write->handle == p_tsdz2->tsdz2_configurations_char_handles.value_handle)
    && (p_evt_write->len == BLE_TSDZ2_CONFIGURATIONS_LEN)
    && (p_tsdz2->tsdz2_write_handler_configurations != NULL))
  {
    p_tsdz2->tsdz2_write_handler_configurations(p_evt_write->data, p_evt_write->len);
  }
}

static void on_write_ant_id(ble_ant_id_t * p_ant_id, ble_evt_t const * p_ble_evt)
{
  ble_gatts_evt_write_t const * p_evt_write = &p_ble_evt->evt.gatts_evt.params.write;

  if ((p_evt_write->handle == p_ant_id->ant_id_char_handles.value_handle)
    && (p_evt_write->len == 1)
    && (p_ant_id->ant_id_write_handler != NULL))
  {
    p_ant_id->ant_id_write_handler(p_ble_evt->evt.gap_evt.conn_handle, p_ant_id, p_evt_write->data[0]);
  }
}

void ble_ant_id_on_ble_evt(ble_evt_t const * p_ble_evt, void * p_context)
{
  ble_ant_id_t * p_ant_id = (ble_ant_id_t *)p_context;

  switch (p_ble_evt->header.evt_id)
  {
    case BLE_GATTS_EVT_WRITE:
      on_write_ant_id(p_ant_id, p_ble_evt);
      break;

    default:
      // No implementation needed.
      break;
  }
}

void ble_tsdz2_on_ble_evt(ble_evt_t const * p_ble_evt, void * p_context)
{
  ble_tsdz2_t * p_tsdz2 = (ble_tsdz2_t *)p_context;

  switch (p_ble_evt->header.evt_id)
  {
    case BLE_GATTS_EVT_WRITE:
      on_write_tsdz2(p_tsdz2, p_ble_evt);
      break;

    default:
      // No implementation needed.
      break;
  }
}

uint32_t ble_service_ant_id_init(ble_ant_id_t * p_ant_id, const ble_ant_id_init_t * p_ant_id_init)
{
  uint32_t              err_code;
  ble_uuid_t            ble_uuid;
  ble_add_char_params_t add_char_params;

  p_ant_id->ant_id_write_handler = p_ant_id_init->ant_id_write_handler;

  // Add service.
  ble_uuid128_t base_uuid = {ANT_ID_UUID_BASE};
  err_code = sd_ble_uuid_vs_add(&base_uuid, &p_ant_id->uuid_type);
  VERIFY_SUCCESS(err_code);

  ble_uuid.type = p_ant_id->uuid_type;
  ble_uuid.uuid = ANT_ID_UUID_SERVICE;

  err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY, &ble_uuid, &p_ant_id->service_handle);
  VERIFY_SUCCESS(err_code);

  memset(&add_char_params, 0, sizeof(add_char_params));
  add_char_params.uuid             = ANT_ID_UUID_CHAR;
  add_char_params.uuid_type        = p_ant_id->uuid_type;
  add_char_params.init_len         = sizeof(uint8_t);
  add_char_params.max_len          = sizeof(uint8_t);
  add_char_params.char_props.read  = 1;
  add_char_params.char_props.write = 1;

  add_char_params.read_access  = SEC_OPEN;
  add_char_params.write_access = SEC_OPEN;

  return characteristic_add(p_ant_id->service_handle, &add_char_params, &p_ant_id->ant_id_char_handles);
}

uint32_t ble_service_tsdz2_init(ble_tsdz2_t * p_tsdz2, const ble_tsdz2_init_t * p_tsdz2_init)
{
  uint32_t              err_code;
  ble_uuid_t            ble_uuid;
  ble_add_char_params_t add_char_params;

  p_tsdz2->tsdz2_write_handler_periodic = p_tsdz2_init->tsdz2_write_handler_periodic;
  p_tsdz2->tsdz2_write_handler_configurations = p_tsdz2_init->tsdz2_write_handler_configurations;

  // Add service.
  ble_uuid128_t base_uuid = {TSDZ2_UUID_BASE};
  err_code = sd_ble_uuid_vs_add(&base_uuid, &p_tsdz2->uuid_type);
  VERIFY_SUCCESS(err_code);

  ble_uuid.type = p_tsdz2->uuid_type;
  ble_uuid.uuid = TSDZ2_UUID_SERVICE;

  err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY, &ble_uuid, &p_tsdz2->service_handle);
  VERIFY_SUCCESS(err_code);

  memset(&add_char_params, 0, sizeof(add_char_params));
  add_char_params.uuid             = TSDZ2_PERIODIC_UUID_CHAR;
  add_char_params.uuid_type        = p_tsdz2->uuid_type;
  add_char_params.is_var_len       = true;
  add_char_params.init_len         = sizeof(uint8_t);
  add_char_params.max_len          = BLE_TSDZ2_PERIODIC_LEN;
  add_char_params.char_props.read  = 1;
  add_char_params.char_props.write = 1;
  add_char_params.char_props.notify = 1;

  add_char_params.read_access       = SEC_OPEN;
  add_char_params.write_access      = SEC_OPEN;
  add_char_params.cccd_write_access = SEC_OPEN;

  err_code = characteristic_add(p_tsdz2->service_handle, &add_char_params, &p_tsdz2->tsdz2_periodic_char_handles);
  VERIFY_SUCCESS(err_code);

  memset(&add_char_params, 0, sizeof(add_char_params));
  add_char_params.uuid             = TSDZ2_CONFIGURATIONS_UUID_CHAR;
  add_char_params.uuid_type        = p_tsdz2->uuid_type;
  add_char_params.init_len         = BLE_TSDZ2_CONFIGURATIONS_LEN;
  add_char_params.max_len          = BLE_TSDZ2_CONFIGURATIONS_LEN;
  add_char_params.char_props.read  = 1;
  add_char_params.char_props.write = 1;

  add_char_params.read_access       = SEC_OPEN;
  add_char_params.write_access      = SEC_OPEN;

  err_code = characteristic_add(p_tsdz2->service_handle, &add_char_params, &p_tsdz2->tsdz2_configurations_char_handles);
  VERIFY_SUCCESS(err_code);

  return err_code;
}

uint32_t ble_ant_id_on_change(uint16_t conn_handle, ble_ant_id_t * p_ant_id_t, uint8_t value)
{
  ble_gatts_hvx_params_t params;
  uint16_t len = 1;

  memset(&params, 0, sizeof(params));
  params.type   = BLE_GATT_HVX_NOTIFICATION;
  params.handle = p_ant_id_t->ant_id_char_handles.value_handle;
  params.p_data = &value;
  params.p_len  = &len;

  return sd_ble_gatts_hvx(conn_handle, &params);
}

uint32_t ble_tsdz2_periodic_on_change(uint16_t conn_handle, ble_tsdz2_t * p_ble_tsdz2_t, uint8_t *p_value)
{
  ret_code_t err_code;
  ble_gatts_hvx_params_t params;
  uint16_t len = BLE_TSDZ2_PERIODIC_LEN;

  memset(&params, 0, sizeof(params));
  params.type   = BLE_GATT_HVX_NOTIFICATION;
  params.handle = p_ble_tsdz2_t->tsdz2_periodic_char_handles.value_handle;
  params.p_data = p_value;
  params.p_len  = &len;

  err_code = sd_ble_gatts_hvx(conn_handle, &params);

  return err_code;
}

uint32_t ble_tsdz2_configurations_on_change(uint16_t conn_handle, ble_tsdz2_t * p_ble_tsdz2_t, uint8_t *p_value)
{
  ret_code_t         err_code = NRF_SUCCESS;
  ble_gatts_value_t  gatts_value;

  // Initialize value struct.
  memset(&gatts_value, 0, sizeof(gatts_value));

  gatts_value.len     = BLE_TSDZ2_CONFIGURATIONS_LEN;
  gatts_value.offset  = 0;
  gatts_value.p_value = p_value;

  // Update database.
  err_code = sd_ble_gatts_value_set(conn_handle,
                                    p_ble_tsdz2_t->tsdz2_configurations_char_handles.value_handle,
                                    &gatts_value);

  return err_code;
}