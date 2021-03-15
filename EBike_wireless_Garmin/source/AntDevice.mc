// vim: syntax=c

using Toybox.System;
using Toybox.Ant;

class AntDevice extends Ant.GenericChannel {
  const ANT_CHANNEL = fieldDataID;
  const DEV_NUMBER = 35 + fieldDataID;
  const DEVICE_TYPE = 2;
  const PERIOD = 8192;
  const CHANNEL = 71;
  var device_cfg;

  const REOPEN_DELAY = 3;
  var open_delay = 1;
  var opened = false;
  var searching = false;
  var deviceNum;
    var fieldData = "STARTING";
    var chan_ass;
    
    // timers for self-checking
    hidden var _lastMessageTime = 0;
    var timer = 0;
    var timerOnTimer = 10;
    var timerReopen = 0;
    var validData = false;

  function initialize() {
    open();
  }

  function open() {
    chan_ass = new Ant.ChannelAssignment(
        Ant.CHANNEL_TYPE_RX_ONLY,
        Ant.NETWORK_PUBLIC);

    device_cfg = new Ant.DeviceConfig({
        :deviceNumber => DEV_NUMBER,
        :deviceType => DEVICE_TYPE,
        :transmissionType => 1,
        :messagePeriod => PERIOD,
        :radioFrequency => CHANNEL,
        :searchTimeoutLowPriority => 12, // 30 seconds
        :searchThreshold => 0, // Pair to all transmitting sensors
        });

    GenericChannel.initialize(method(:onMessage), chan_ass);
    GenericChannel.setDeviceConfig(device_cfg);
    GenericChannel.open();
  }

  function maybe_open() {
    if (opened) {
      return;
    }

    if (open_delay > 0) {
      open_delay--;
      return;
    }

    open_delay = REOPEN_DELAY;
    open();
  fieldData = "SEARCHING";
  }

  function close() {
	  GenericChannel.close();
	  GenericChannel.release();
  }
  
      // Re-open connection. Use this when sensor is not responding
    // Usage: sensor.reopen();
    var reopening = false;
    function reopen() {
        if (!reopening) {
//            close();
            timerOnTimer = 0;
            timerReopen = 5;
            reopening = true;
        } else {
        }
    }

    function reopen_finish () {
//        open();
        reopening = false;
        timerOnTimer = 10;
        timerReopen = 0;
    }

    // execute a timer to check the activity of the sensor. Reset if there is a lapse.
    function onTimer() {
        var secondsSinceLastUpdate = timer - _lastMessageTime;
        if (secondsSinceLastUpdate > 30) {
            reopen();
            _lastMessageTime = timer;
            validData = false;
        }

        timerOnTimer = 10;
    }
  

    hidden function doChResponse(id, code) {
        if (id == Ant.MSG_ID_RF_EVENT && code == Ant.MSG_CODE_EVENT_RX_SEARCH_TIMEOUT) {
            // Channel closed, re-open
            reopen();
            validData = false;
        } else if (id == Ant.MSG_ID_RF_EVENT && code == Ant.MSG_CODE_EVENT_RX_FAIL_GO_TO_SEARCH) {
	        // Channel dropped, re-open
	        reopen();
	        validData = false;
        } else if (id == Ant.MSG_ID_RF_EVENT && code == Ant.MSG_CODE_EVENT_CHANNEL_CLOSED) {
            // Channel closed, re-open
            reopen();
            validData = false;
        }
    }

  function onMessage(msg) {
    var payload = msg.getPayload();
    var msgId = msg.messageId;

        if (msgId == Ant.MSG_ID_CHANNEL_RESPONSE_EVENT) {

            // Channel Response
            doChResponse(payload[0], payload[1]);

        } else if (msgId == Ant.MSG_ID_BROADCAST_DATA) {
        
        validData = true;

      switch (fieldDataID) {
        case 1:
        case 4:
          fieldData = (payload[0] | (payload[1] << 8)) / 10.0;
          break;

        case 2:
        case 5:
          fieldData = payload[0] / 5;
          break;

        case 3:
        case 7:
        case 9:
        case 10:
        case 12:
          fieldData = payload[0];
          break;

        case 6:
        case 8:
        case 11:
          fieldData = (payload[0] | (payload[1] << 8));
          break;

        case 13:
          fieldData = (payload[0] | (payload[1] << 8) | (payload[2] << 16) | (payload[3] << 24)) / 10;
          break;
      }

    searching = false;
    }
  }
  
function getData() {
    return fieldData;
  }

  function timerTick() {
    timer++;

    if (timerOnTimer > 0) {
        timerOnTimer--;
    } else if (timerOnTimer == 0) {
        timerOnTimer = -1;
        onTimer();
    }

    if (timerReopen > 0) {
        timerReopen--;
    } else if (timerReopen == 0) {
        timerReopen = -1;
        reopen();
    }
  }
}
