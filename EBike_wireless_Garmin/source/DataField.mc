using Toybox.WatchUi;
using Toybox.System;

// 1 - battery voltage
// 2 - battery current
// 3 - battery SOC
// 4 - battery used energy
// 5 - motor current
// 6 - motor power
// 7 - motor PWM
// 8 - motor speed
// 9 - motor temperature
// 10 - cadence
// 11 - human power
// 12 - assist level
// 13 - odometer

const fieldDataID = 1;

class DataField extends WatchUi.SimpleDataField {
  hidden var ant_device;
  hidden var id_displayed = false;
  var fieldData = 1;

  function initialize(device) {
    SimpleDataField.initialize();

      switch (fieldDataID) {
        case 1:
          label = "battery voltage";
          break;

        case 2:
          label = "battery current";
          break;

        case 3:
          label = "battery SOC";
          break;

        case 4:
          label = "battery used energy";
          break;

        case 5:
          label = "motor current";
          break;

        case 6:
          label = "motor power";
          break;

        case 7:
          label = "motor PWM";
          break;

        case 8:
          label = "motor speed";
          break;

        case 9:
          label = "motor temperature";
          break;

        case 10:
          label = "cadence";
          break;

        case 11:
          label = "human power";
          break;

        case 12:
          label = "assist level";
          break;

        case 13:
          label = "odometer";
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

      return "Searching...";

    } else if (!id_displayed || !ant_device.data_valid) {

      id_displayed = true;
      return "id: " + ant_device.deviceNum;

    }

    var return_value;
    switch (fieldDataID) {
      case 0:
        return_value = ant_device.getData(fieldDataID).format("%.1f");
        break;

      case 5:
          return_value = ant_device.getData(fieldDataID);
        break;
    }

    return return_value;
  }
}
