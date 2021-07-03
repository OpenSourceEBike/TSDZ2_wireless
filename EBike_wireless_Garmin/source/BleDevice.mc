using Toybox.System;
using Toybox.BluetoothLowEnergy as Ble;

const TSDZ_SERVICE = Ble.stringToUuid("dac21400-cfdd-462f-bfaf-7f6e4ccbb45f");
const DEVICE_NAME = "TSDZ";



const TSDZ_CHARACTERISTICS = Ble.stringToUuid("dac21403-cfdd-462f-bfaf-7f6e4ccbb45f");



const TSDZ_SHORT_DESC = Ble.cccdUuid();

class BleDevice extends Ble.BleDelegate {
  var scanning = false;
  var device = null;

  var fieldData = -1;
  var fieldDataID = 0;

  function getData(id) {
    fieldDataID = id;
    return fieldData;
  }

  function initialize() {
    BleDelegate.initialize();
  }

  function onCharacteristicChanged(ch, value) {
    if (ch.getUuid().equals(TSDZ_CHARACTERISTICS)) {

      switch (value[0]) {
        case 1: // assist level
          fieldData =  value[1];
          break;

        case 2: // battery SOC
          fieldData = value[2];
          break;
          
     	 case 3: // battery SOC
          fieldData = value[3] | (value[4] << 8);
          break;
      }
    }
  }

  function TSDZ_setTSDZShortNotifications_DESC(value) {
    var service;
    var ch;
    var desc;

    if (device == null) {
      return;
    }

    service = device.getService(TSDZ_SERVICE);
    ch = service.getCharacteristic(TSDZ_CHARACTERISTICS);
    desc = ch.getDescriptor(TSDZ_SHORT_DESC);
    desc.requestWrite([value & 0x01, 0x00]b);
  }

  function onProfileRegister(uuid, status) {
  }

  function registerProfiles() {
    var profile = {
      :uuid => TSDZ_SERVICE,
      :characteristics => [{
        :uuid => TSDZ_CHARACTERISTICS,
        :descriptors => [TSDZ_SHORT_DESC],
      }]
    };

    BluetoothLowEnergy.registerProfile(profile);
  }

  function onScanStateChange(scanState, status) {
    if (scanState == Ble.SCAN_STATE_SCANNING) {
      scanning = true;
    } else {
      scanning = false;
    }
  }

  function onConnectedStateChanged(device, state) {
    if (state == Ble.CONNECTION_STATE_CONNECTED) {
      self.device = device;
      TSDZ_setTSDZShortNotifications_DESC(1);
    } else {
      self.device = null;
    }
  }

  private function connect(result) {
    Ble.setScanState(Ble.SCAN_STATE_OFF);
    Ble.pairDevice(result);
  }

  function onScanResults(scanResults) {
    var appearance, name, rssi;
    var mfg, uuids, service;
    for (var result = scanResults.next(); result != null; result = scanResults.next()) {
      name = result.getDeviceName();

      if (name != null && name.equals(DEVICE_NAME)) {
        connect(result);
        return;
      }
    }
  }

  function open() {
    registerProfiles();
    Ble.setScanState(Ble.SCAN_STATE_SCANNING);
  }

  function close() {
    if (scanning) {
      Ble.setScanState(Ble.SCAN_STATE_OFF);
    }

    if (device) {
      Ble.unpairDevice(device);
    }
  }
}
