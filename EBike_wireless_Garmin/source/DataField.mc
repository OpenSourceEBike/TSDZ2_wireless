// vim: syntax=c

using Toybox.WatchUi;
using Toybox.System;

class DataField extends WatchUi.SimpleDataField {
  hidden var ant_device;
  hidden var id_displayed = false;

// 0 - battery voltage
// 1 - batter current
// 2 - battery SOC
// 3 - battery used energy
// 4 - motor current
// 5 - motor temperature
// 6 - motor speed
// 7 - speed
// 8 - throttle
// 9 - cadence
// 10 - duty-cycle
// 11 - human power
// 12 - assist level

  var fieldData = 0;
  var fieldDataID = 5;

  function initialize(device) {
    SimpleDataField.initialize();

      switch (fieldDataID) {
        case 0:
          label = "battery voltage";
          break;

        case 5:
          label = "motor temperature";
          break;
      }

    ant_device = device;
  }

  function compute(info) {

    if (!ant_device.opened) {

      id_displayed = false;
      ant_device.maybe_open();
      return "Idle";

    } else if (ant_device.searching) {

      return "Searching... id: " + ant_device.deviceNum;

    } else if (!id_displayed || !ant_device.data_valid) {

      id_displayed = true;
      return "id: " + ant_device.deviceNum;

    }

    return ant_device.getData(fieldDataID);
  }
}
