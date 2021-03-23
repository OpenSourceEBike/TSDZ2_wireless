// vim: syntax=c

using Toybox.System;
using Toybox.Ant;

class AntDevice extends Ant.GenericChannel {
  const ANT_CHANNEL = fieldDataID;
  const DEVICE_TYPE = 0x7b;
  const PERIOD = 16384;
  const FREQUENCY = 48;
  var device_cfg;
    
  var searching = false;
  var deviceNum;
    var fieldData = "--";
    var chan_ass;
    
    // timers for self-checking
    hidden var _lastMessageTime = 0;
    var timer = 0;
    var timerOnTimer = 0;
    var timerReopen = -1;
    var validData = false;
    
    var deviceNumber;  	
    
  function initialize() {
   	switch (fieldDataID) {
 		case 1:
 		case 2:
 		case 3:
 		case 4:
 		case 5:
 		case 9:
 			deviceNumber = 65136;
 			break;
 			
 		case 6:
 		case 7:
 		case 10:
 		case 11:
 			deviceNumber = 65137;
 			break;
 			
 		case 8:
 		case 12:
 			deviceNumber = 65138;
 			break;
 			
 		default:
 			break;
 	}
  
  	Math.srand(System.getTimer());
  
    chan_ass = new Ant.ChannelAssignment(
        Ant.CHANNEL_TYPE_RX_ONLY,
        Ant.NETWORK_PUBLIC);

    device_cfg = new Ant.DeviceConfig({
        :deviceNumber => deviceNumber,
        :deviceType => 0,
        :transmissionType => 0,
        :messagePeriod => PERIOD,
        :radioFrequency => FREQUENCY,
        :searchTimeoutLowPriority => 4, // 10 seconds
        :searchThreshold => 10,
        });
        
    GenericChannel.initialize(method(:onMessage), chan_ass);
  
    reopen(); // will open but with a kind of random timming
  }

  function open() {
    GenericChannel.initialize(method(:onMessage), chan_ass);
    GenericChannel.setDeviceConfig(device_cfg);
    GenericChannel.open();
  }

  function close() {
    GenericChannel.release();
  }
  
      // Re-open connection. Use this when sensor is not responding
    // Usage: sensor.reopen();
    var reopening = false;
    function reopen() {
        timerOnTimer = -1;
        
        if (reopening == false) {
            timerReopen = random(fieldDataID);
            reopening = true;
            close();
        }
    }

    function reopen_finish() {
        timerReopen = -1;
        timerOnTimer = 7;
        open();
        reopening = false;
    }

    // execute a timer to check the activity of the sensor. Reset if there is a lapse.
    function onTimer() {
        timerOnTimer = -1;
        
        var secondsSinceLastUpdate = timer - _lastMessageTime;
        if (secondsSinceLastUpdate > 3) {
            reopen();
            _lastMessageTime = timer;
            validData = false;
        }

        timerOnTimer = 7;
    }

  function onMessage(msg) {
    var payload = msg.getPayload();
    var msgId = msg.messageId;

        if (Ant.MSG_ID_BROADCAST_DATA == msg.messageId) {
 
	        _lastMessageTime = timer;	       
	        validData = true;
	        
     	switch (fieldDataID) { 
            case 1:
            	fieldData = payload[0] & 0xC0;
            	break;
            	
        	case 2:
        		if ((payload[0] & 0xC0) == 0) {
        			fieldData = payload[1];
    			}
    			break;
    			
        	case 3:
        		if ((payload[0] & 0xC0) == 0) {
        			fieldData = payload[2] | (payload[3] << 8);
    			}
    			break;
    			
    		case 4:
        		if ((payload[0] & 0xC0) == 0) {
        			fieldData = payload[4] | (payload[5] << 8);
    			}
    			break;
    			
        	case 5:
        		if ((payload[0] & 0xC0) == 0) {
        			fieldData = payload[6];
    			}
    			break;
    			
        	case 6:
        		if ((payload[0] & 0xC0) == 0x40) {
      				fieldData = (payload[1] | (payload[2] << 8)) / 10.0;
    			}
    			break;
    			
        	case 7:
        		if ((payload[0] & 0xC0) == 0x40) {
      				fieldData = (payload[3] | (payload[4] << 8)) / 5.0;
    			}
    			break;
    			
        	case 8:
        		if ((payload[0] & 0xC0) == 0x80) {
      				fieldData = (payload[1] | (payload[2] << 8) | (payload[3] << 16)) / 10.0;
    			}
    			break;
    			
        	case 9:
        		if ((payload[0] & 0xC0) == 0) {
  				    fieldData = payload[7];
    			}
    			break;
    			
        	case 10:
        		if ((payload[0] & 0xC0) == 0x40) {
  				    fieldData = payload[5];
    			}
    			break;
    			
			default:
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
                else if (payload[1] == Ant.MSG_CODE_EVENT_RX_FAIL)
                {
                   
                } else {

                }
            }
        } else {

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

  function random(n) {    
     return Math.rand() % n + n; //Random number between n and 2*n
  }
}
