using Toybox.Application;
using Toybox.BluetoothLowEnergy as Ble;

class datafieldApp extends Application.AppBase {
  hidden var bleDevice;

  function initialize() {
    AppBase.initialize();
  }

  // onStart() is called on application start up
  function onStart(state) {
    bleDevice = new BleDevice();
    Ble.setDelegate(bleDevice);
    bleDevice.open();
  }

  // onStop() is called when your application is exiting
  function onStop(state) {
    bleDevice.close();
  }

  // Return the initial view of your application here
  function getInitialView() {
    return [ new datafieldView(bleDevice) ];
  }
}