# Remote operation
## LED Signalling
------
1. ANT Searching/Connection
   When the remote is searching for either an ANT+ LEV or an ANT+ Controls connection, the **RED LED** will slowly flash. When a connection is made, the **RED LED** will quickly flash and then go out.
2.  The **GREEN LED** will flash once briefly when the [**PLUS**] or [**MINUS**] keys are pressed to indicate the assist level is changing. if the assist level has reached either 0 or 7, pressing the [**PLUS**] or [**MINUS**] keys key will cause the led to briefly flash multiple times to indicate the limit has been reached. If the motor is on, the color of this LED will be **RED**.
3. The **RED LED** will briefly flash to indicate a long press has been made
4. The **RED LED** will turn on for 2 seconds to indicated that the remote is entering DFU mode.
5. The **BLUE LED** will turn on for 2 seconds to indicated bluetooth mode is active.
6. A long press of the [**ENTER**] key can be used to determine the configuration options. [See Configuration Options](configuration.md)
7. A long press of the [**POWER**] key will turn the motor ON or OFF. When the motor is initializing, the leds will flash [**OFF-WHITE**]. When the motor is on, the **OFF-WHITE LED** will rapidly flash, followed 2 seconds later by a display of the motor battery state. If the motor is turning off, the battery state will also be displayed. Battery state is indicated by flashing the **GREEN LED**. The number of flashes will indicate battery charge state from from 1 flash (10% charge) to 10 flashes (100% charge).
8. Motor error states are indicated by the **GREEN LED** on the other side of the Nordic board. motor initialization errors are indicated by a slowly flashing LED, firmware mismatch errors are indicated by a fast flashing LED, and configuration errors are indicated by a steady on LED.


## Short Press buttons
----
* Short Press the [**POWER**] button to display the motor battery state of charge. 
* Short Press the [**ENTER**] button to display the current assist level by flashing the GREEN LED. ie: 3 flashes indicated that the motor is in assist level 3 
* Short press the [**PLUS**] button to increase the motor assist level (ANT+ LEV control)
* Short press the [**MINUS**] button to decrease the motor assist level (ANT+ LEV control)
  
## Long Press Buttons
-----
* Long Press the [**POWER**] button to turn on/off the motor. See LED Operation Signalling above for a description of operation.
* Long Press the [**PLUS**] button to pageup on a garmin bike computer
* Long Press the [**MINUS**] button to pagedown on a garmin bike computer
* Long press the [**ENTER**] button to cycle through the configuration LED display.     [See Configuration Options](configuration.md)
* Long Press both the [**ENTER**] + [**POWER**] buttons at the same time for 10 seconds or longer to initate Device Firmware Update (DFU) mode.  Either the remote or bootloader firmware can be updated to a new version using a provided upgrade packet zip file in DFU mode. For more information click [here](dfu.md).
* Long press the [**PLUS**] + [**POWER**] buttons to start bluetooth to allow the [Configuration Options](configuration.md)  to be set. 
* Long press the [**MINUS**] + [**POWER**] buttons to stop bluetooth to save power. 
* Long press the [**MINUS**] + [**PLUS**] buttons to put the remote control in 'deep sleep' low power mode

Bluetooth will also automatically turn off after:
    * 5 minutes if you left bluetooth running
    * After you change any [Configuration Options](configuration.md) 
      (See Configuration Options below)
 
See controlling a Garmin 1030 bike computer for assist levels and page control using a simulated ANT+ LEV Ebike in this video:

[![video](https://img.youtube.com/vi/s7URIMVzcwc/hqdefault.jpg)](https://www.youtube.com/watch?v=s7URIMVzcwc)

See changing the ANT+ LEV Device Number (to connect to only one specific eBike) using the Nordic nRF Connect app in this video:

[![video](https://img.youtube.com/vi/_ALauuDxZuQ/hqdefault.jpg)](https://youtu.be/_ALauuDxZuQ) 

nRFConnect is available on the play store [here](https://play.google.com/store/apps/details?id=no.nordicsemi.android.mcp&hl=en_CA&gl=US)

## [back](../README.md)