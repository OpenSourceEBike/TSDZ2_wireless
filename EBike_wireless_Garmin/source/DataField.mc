using Toybox.WatchUi;
using Toybox.System;


var fieldDataID = 1;


class DataField extends WatchUi.SimpleDataField {
  hidden var ant_device;

  function initialize(device) {
    SimpleDataField.initialize();

      switch (fieldDataID) {
        case 1:
          label = "Assist";
          break;

        case 2:
          label = "Battery SOC";
          break;

        case 3:
          label = "Power";
          break;

        case 4:
          label = "Human power";
          break;

        case 5:
          label = "Cadence";
          break;

        case 6:
          label = "Bat voltage";
          break;

        case 7:
          label = "Bat current";
          break;

        case 8:
          label = "Bat used power";
          break;

        case 9:
          label = "Mot temperature";
          break;

        case 10:
          label = "Mot PWM";
          break;

        case 11:
          label = "Mot speed";
          break;

        case 12:
          label = "Odometer";
          break;
      }

    ant_device = device;
  }

function compute(info) {
    var value = "--";

    ant_device.timerTick();

    if (ant_device.validData == false) {
        return value;
    }

    switch (fieldDataID) {
      case 6:
      case 7:
      case 8:
      case 10:
      case 13:
    	value = ant_device.getData().format("%.1f");
        break;

      case 1:
      case 2:
      case 3:
      case 4:
      case 5:
      case 9:
      case 11:
      case 12:
        	value = ant_device.getData();
        break;
    }

    return value;
  }
}
