using Toybox.WatchUi;

class datafieldView extends WatchUi.SimpleDataField {
  hidden var bleDevice;


  var fieldData = 2;


  function initialize(device) {
    SimpleDataField.initialize();

    switch (fieldData) {
      case 1:
        label = "Assist";
        break;

      case 2:
        label = "Battery SOC";
        break;
        
      case 3:
        label = "Motor Power";
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
    
    if (bleDevice.getData(fieldData) == -1) {
    	return "ERROR 1";
	}

    return bleDevice.getData(fieldData);
  }
}