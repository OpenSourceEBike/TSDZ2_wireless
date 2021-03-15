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

    try
    {
        GenericChannel.open();
        opened = true;
        searching = true;
    }

    catch (ex instanceof AnExceptionClass) {
     opened = false;
    }
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
    if (opened) {
      GenericChannel.close();
      GenericChannel.release();
    }

    opened = false;
  }

  function getData(id) {
    fieldDataID = id;
    return fieldData;
  }

    hidden function doChResponse(id, code) {
        if (id == Ant.MSG_ID_RF_EVENT && code == Ant.MSG_CODE_EVENT_RX_FAIL) {
        } else if (id == Ant.MSG_ID_RF_EVENT && code == Ant.MSG_CODE_EVENT_RX_SEARCH_TIMEOUT) {
          opened = false;
          searching = false;
          open_delay = REOPEN_DELAY;
          fieldData = "EV_RX_FAIL searching";
        } else if (id == Ant.MSG_ID_RF_EVENT && code == Ant.MSG_CODE_EVENT_RX_FAIL_GO_TO_SEARCH) {
            opened = false;
            searching = false;
            open_delay = REOPEN_DELAY;
            fieldData = "EV_RX_FAIL GO SEARCH";
        } else if (id == Ant.MSG_ID_RF_EVENT && code == Ant.MSG_CODE_EVENT_CHANNEL_CLOSED) {
          opened = false;
          searching = false;
          open_delay = REOPEN_DELAY;
          fieldData = "EV_CH_CLOSED searching";
        } else {

        }
    }

  function onMessage(msg) {
    var payload = msg.getPayload();
    var msgId = msg.messageId;

        if (msgId == Ant.MSG_ID_CHANNEL_RESPONSE_EVENT) {

            // Channel Response
            doChResponse(payload[0], payload[1]);

        } else if (msgId == Ant.MSG_ID_BROADCAST_DATA) {

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
}
