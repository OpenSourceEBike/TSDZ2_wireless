using Toybox.Application;

class Main extends Application.AppBase {
  hidden var antDevice;

  function initialize() {
    AppBase.initialize();
  }

  function onStart(state) {
    antDevice = new AntDevice();
  }

  function getInitialView() {
    return [new DataField(antDevice)];
  }

  function onStop(state) {
    antDevice.close();
  }
}
