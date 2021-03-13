// vim: syntax=c

using Toybox.System;
using Toybox.Ant;

class AntDevice extends Ant.GenericChannel {



  const ANT_CHANNEL = 1;
  const DEV_NUMBER = 2;





  const DEVICE_TYPE = 2;
  const PERIOD = 16384;
  const CHANNEL = 66;
  var device_cfg;

  const REOPEN_DELAY = 20;
  var open_delay = 1;
  var opened = false;
  var searching = false;
var deviceNum;
var data_valid = false;


    var fieldData = 0;
  var fieldDataID = 0;

  function initialize() {
    var chan_ass = new Ant.ChannelAssignment(
        Ant.CHANNEL_TYPE_RX_NOT_TX,
        Ant.NETWORK_PUBLIC);
    GenericChannel.initialize(method(:onMessage), chan_ass);

    device_cfg = new Ant.DeviceConfig({
        :deviceNumber => DEV_NUMBER,
        :deviceType => DEVICE_TYPE,
        :transmissionType => 1,
        :messagePeriod => PERIOD,
        :radioFrequency => CHANNEL,
        :searchTimeoutLowPriority => 4, // 10 seconds
        :searchThreshold => 0,
        });
  }

  function open() {
    GenericChannel.setDeviceConfig(device_cfg);
    GenericChannel.open();
    opened = true;
    searching = true;
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
  }

  function close() {
    GenericChannel.close();
  }

  function getData(id) {
    fieldDataID = id;
    return fieldData;
  }

    hidden function doChResponse(id, code) {
        if (id == Ant.MSG_ID_RF_EVENT && code == Ant.MSG_CODE_EVENT_RX_FAIL) {
//            debug("rx fail");
            fieldData = 9901;
        } else if (id == Ant.MSG_ID_RF_EVENT && code == Ant.MSG_CODE_EVENT_RX_SEARCH_TIMEOUT) {
//            debug("search timeout");
            opened = false;
            data_valid = false;
            fieldData = 9902;
        } else if (id == Ant.MSG_ID_RF_EVENT && code == Ant.MSG_CODE_EVENT_RX_FAIL_GO_TO_SEARCH) {
//            debug("rx fail go to search");
            searching = true;
            fieldData = 9903;
        } else if (id == Ant.MSG_ID_RF_EVENT && code == Ant.MSG_CODE_EVENT_CHANNEL_CLOSED) {
//            debug("channel closed");
            opened = false;
            data_valid = false;
            fieldData = 9904;
        } else {
//            debug("channel response, id: " + id.format("%02x") + " code: " + code.format("%02x"));
            fieldData = 9905;
        }
    }

  function onMessage(msg) {
    var payload = msg.getPayload();
    var msgId = msg.messageId;



        if (msgId == Ant.MSG_ID_CHANNEL_RESPONSE_EVENT) {

            // Channel Response
            doChResponse(payload[0], payload[1]);

        } else if (msgId == Ant.MSG_ID_BROADCAST_DATA) {

          deviceNum = msg.deviceNumber;

      //      if (payload[0] != ANT_CHANNEL) {
      //          // Page 0
      //          return;
      //      }

          switch (fieldDataID) {
            case 0: // battery voltage
              fieldData = (payload[1] | (payload[2] << 8)) / 10.0;
              break;

            case 5: // motor temperature
              fieldData = payload[3];
              break;
          }

            data_valid = true;
          searching = false;
        }
  }
}
