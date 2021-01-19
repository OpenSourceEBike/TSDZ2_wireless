# Remote operation
LED Operation Signals
------
1. ANT Searching/Connection
   When the remote is searching for either an ANT+ LEV or an ANT+ Controls connection, the RED LED will slowly flash. When a connection is made, the RED LED will quickly flash and then go out.
2. The RED LED will briefly flash to indicate a    long press has been made
3. The RED LED will turn on for 2 seconds to indicated that the remote is entering DFU mode.
4. The Blue LED will turn on for 2 seconds to indicated bluetooth mode is active.
5. The configuration of the remote is signalled by a long press of the ENTER key. [See Configuration Options](configuration.md)


Short Press buttons
----
* Short Press the [ENTER] button to switch pages (pageup) on an ANT+ CONTROLS device (ie: garmin bike computer) 
  (You can also use the long press of PLUS and MINUS to pageup/pagedown - see below)
* Short press the [PLUS] button to increase the motor assist level (ANT+ LEV control)
* Short press the [MINUS] button to decrease the motor assist level (ANT+ LEV control)
  
Long Press Buttons
-----

* Long Press the [PLUS] button to pageup on a garmin bike computer
* Long Press the [MINUS] button to pagedown on a garmin bike computer
* Long press the [ENTER] button to cycle through the configuration LED display.     [See Configuration Options](configuration.md)
* Long Press both the [ENTER] + [STANDBY] buttons at the same time to initate Device Firmware Update (DFU) mode.  Either the remote or bootloader firmware can be updated to a new version using a provided upgrade packet zip file in DFU mode. For more information click [here](dfu.md).
* Long press the [PLUS] + [STANDBY] buttons to start bluetooth to allow the [Configuration Options](configuration.md)  to be set. 
* Long press the [MINUS] + [STANDBY] buttons to stop bluetooth to save power. 
* Long press the [MINUS] + [PLUS] buttons to put the remote control in 'deep sleep' low power mode

Bluetooth will also automatically turn off after:
    * 5 minutes if you left bluetooth running
    * After you change any [Configuration Options](configuration.md) 
      (See Configuration Options below)
  * Planned feature: Long press the [POWER] button to turn off the TSDZ2 motor

See controlling a Garmin 1030 bike computer for assist levels and page control using a simulated ANT+ LEV Ebike in this video:

[![video](https://img.youtube.com/vi/s7URIMVzcwc/hqdefault.jpg)](https://www.youtube.com/watch?v=s7URIMVzcwc)

See changing the ANT+ LEV Device Number (to connect to only one specific eBike) using the Nordic nRF Connect app in this video:

[![video](https://img.youtube.com/vi/_ALauuDxZuQ/hqdefault.jpg)](https://youtu.be/_ALauuDxZuQ) 

nRFConnect is available on the play store here:
(https://play.google.com/store/apps/details?id=no.nordicsemi.android.mcp&hl=en_CA&gl=US)

## [back](../README.md)