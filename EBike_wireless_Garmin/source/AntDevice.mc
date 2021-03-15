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

  var searching = false;
  var deviceNum;
    var fieldData = "STARTING";
    var chan_ass;
    
    // timers for self-checking
    hidden var _lastMessageTime = 0;
    var timer = 0;
    var timerOnTimer = 10;
    var timerReopen = -1;
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

  function close() {
	  GenericChannel.close();
	  GenericChannel.release();
  }
  
      // Re-open connection. Use this when sensor is not responding
    // Usage: sensor.reopen();
    var reopening = false;
    function reopen() {
        if (reopening == false) {
            close();
            timerOnTimer = -1;
            timerReopen = 5;
            reopening = true;
        }
    }

    function reopen_finish() {
        open();
        reopening = false;
        timerOnTimer = 10;
        timerReopen = -1;
    }

    // execute a timer to check the activity of the sensor. Reset if there is a lapse.
    function onTimer() {
        var secondsSinceLastUpdate = timer - _lastMessageTime;
        if (secondsSinceLastUpdate > 3) {
            reopen();
            _lastMessageTime = timer;
            validData = false;
        }

        timerOnTimer = 10;
    }

  function onMessage(msg) {
    var payload = msg.getPayload();
    var msgId = msg.messageId;
    
    _lastMessageTime = timer;

        if (Ant.MSG_ID_BROADCAST_DATA == msg.messageId)
        {
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

        }
        else if( Ant.MSG_ID_CHANNEL_RESPONSE_EVENT == msg.messageId )
        {
            if( Ant.MSG_ID_RF_EVENT == payload[0] )
            {
                if (payload[1] == Ant.MSG_CODE_EVENT_CHANNEL_CLOSED)
                {
                    // Channel closed, re-open
                    reopen();
                    validData = false;
                }
                else if (payload[1] == Ant.MSG_CODE_EVENT_RX_FAIL_GO_TO_SEARCH)
                {
                    // Channel dropped, re-open
                    reopen();
                    validData = false;
                }
                else if (payload[1] == Ant.MSG_CODE_EVENT_RX_SEARCH_TIMEOUT)
                {
                    // Search timed out, re-open
                    reopen();
                    validData = false;
                }
            }
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
        reopen_finish();
    }
  }
}
