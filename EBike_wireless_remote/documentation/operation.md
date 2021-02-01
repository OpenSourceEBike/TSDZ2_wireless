# Remote operation

The remote has two operating modes, **NORMAL** **MODE** which is the usual mode of operation, and **CONFIGURATION** **MODE**, which is used to set various remote operating options described below. Pressing the **ENTER** key for a long press of more than 5 seconds will toggle between **NORMAL MODE** and **CONFIGURATION MODE**.
In either mode, if the key is inactive it will briefly flash the **RED LED**.

## Button Operation (NORMAL MODE)

----

* Short Press the [**POWER**] button to display the motor battery state of charge. (works **ONLY** if the motor is ON.)
  
* Long Press the [**POWER**] button to turn on/off the motor. See **LED Signalling** below for a description of operation.
* Short Press the [**ENTER**] button to **PAGE UP** on a connected garmin bike computer. 
* Long Press the [**ENTER**] button to **PAGE DOWN** on a garmin bike computer. (Works **ONLY** if garmin control is activated)
* Short press the [**PLUS**] button to increase the motor assist level (Wireless TSDZ2 and supported **ANT+ LEV** enabled e-bikes)
* Short press the [**MINUS**] button to decrease the motor assist level (Wireless TSDZ2 and supported **ANT+ LEV** enabled e-bikes)
* Long press the [**MINUS**] + [**PLUS**] buttons to put the remote control in 'deep sleep' low power mode.
* A very long press (more than 5 seconds) of the [**ENTER**] button will enter**CONFIGURATION MODE**
  
----

## Button Operation (CONFIGURATION MODE)

* a short press of the [**ENTER**] key will cycle through the configuration LED display. A **RED LED** indicates support for ANT LEV ebikes, while a BLUE LED indicated support for Garmin bike computers. [See Configuration Options](configuration.md)
* A Long press the [**ENTER**] button will exit **CONFIGURATION MODE** and return to **NORMAL MODE** . **CONFIGURATION MODE** will also automatically turn off if no keys are pressed for 5 minutes. 
* Long Press the [**POWER**] button to initate Device Firmware Update (DFU) mode.  Either the remote or bootloader firmware can be updated to a new version using a provided upgrade packet zip file in DFU mode. For more information click [here](dfu.md).
* Short press the [**PLUS**] button to enable support for Garmin bike computers.(disabled by default)
* Short press the [**MINUS**] button to disable support for Garmin bike computers.
* Long press the [**PLUS**] button to enable support for either a Wireless TSDZ2 or an ANT LEV enabled ebike. (enabled by default)
* Long press the [**MINUS**] button to disable support for either a Wireless TSDZ2 or an ANT LEV enabled ebike.

  
## LED Signalling (Normal mode)

----

1. ANT Searching/Connection
   When the remote is searching for either an ANT+ LEV or an ANT+ Controls connection, the **RED LED** will slowly flash. When a connection is made, the **RED LED** will quickly flash and then go out.
2. The **GREEN LED** will flash once briefly when the [**PLUS**] or [**MINUS**] keys are pressed to indicate the assist level is changing. if the assist level has reached either 0 or 7, pressing the [**PLUS**] or [**MINUS**] keys key will cause the led to briefly flash **RED** multiple times to indicate the limit has been reached. 
3. If the motor is off, pressing the [**PLUS**] or [**MINUS**] keys will cause a **RED** LED to flash. The assist level will NOT change with the motor off. 
4. The **RED LED** will briefly flash to indicate a long press has been made
5. A very long press (> 5 seconds) of the [**ENTER**] key will flash the BLUE LED for 2 seconds when entering the configuration mode.
6. A long press of the [**POWER**] key will turn the motor ON or OFF. When the motor is initializing, the leds will flash [**OFF-WHITE**]. When the motor is on, the **OFF-WHITE LED** will rapidly flash, followed 2 seconds later by a display of the motor battery state. If the motor is turning off, the battery state will also be displayed. Battery state is indicated by flashing the **GREEN LED**. The number of flashes will indicate battery charge state from from 1 flash (10% charge) to 10 flashes (100% charge).

## LED Signalling (Configuration mode)

1. The **BLUE LED** will flash for 2 seconds to indicated configuration mode is active.
2. The **OFF-WHITE LED** will turn on for 2 seconds to indicated that the remote is entering DFU mode.
3. A short press of the ENTER key will display the current remote configuration[See Configuration Options](configuration.md)


See controlling a Garmin 1030 bike computer for assist levels and page control using a simulated ANT+ LEV Ebike in this video:

[![video](https://img.youtube.com/vi/s7URIMVzcwc/hqdefault.jpg)](https://www.youtube.com/watch?v=s7URIMVzcwc)

See changing the ANT+ LEV Device Number (to connect to only one specific eBike) using the Nordic nRF Connect app in this video:

[![video](https://img.youtube.com/vi/_ALauuDxZuQ/hqdefault.jpg)](https://youtu.be/_ALauuDxZuQ) 

nRFConnect is available on the play store [here](https://play.google.com/store/apps/details?id=no.nordicsemi.android.mcp&hl=en_CA&gl=US)

## [back](../README.md)