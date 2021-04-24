using Toybox.WatchUi;

class datafieldView extends WatchUi.SimpleDataField {
  hidden var bleDevice;

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

  var fieldData = 5;

  function initialize(device) {
    SimpleDataField.initialize();

    switch (fieldData) {
      case 0:
        label = "battery voltage";
        break;

      case 5:
        label = "motor temperature";
        break;
    }

    bleDevice = device;
  }

  function compute(info) {
    if (bleDevice.scanning) {
      return "Scanning...";
    } else if (bleDevice.device == null) {
      return "Disconnected";
    }

    return bleDevice.getData(fieldData);
  }
}