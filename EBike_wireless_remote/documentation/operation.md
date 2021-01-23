# Remote operation
LED Information Signals
------
1. ANT Searching/Connection
   When the remote is searching for either an ANT+ LEV or an ANT+ Controls connection, the RED LED will slowly flash. When a connection is made, the RED LED will quickly flash and then go out.
2. The RED LED will briefly flash to indicate  that a long press has been made.
3. The RED LED will turn on for 2 seconds to indicated that the remote is entering DFU mode.
4. The RED LED will turn on when the brake lever is pressed, and turn off when the brake lever is released.
5. The Blue LED will flash slowly to indicate that bluetooth mode is active.
6. A long press of the ENTER key can be used to determine the configuration options. [See Configuration Options for LED Behavior in this mode](configuration.md)
7. A long press of the STANDBY key will turn the motor ON or OFF. When the motor is initializing, the LED will flash off-white. When the motor is on, the off-white led will rapidly flash, followed 2 seconds later by a display of the motor battery state. If the motor is turning off, the battery state will also be displayed. 
Battery state is indicated by flashing the GREEN LED. The number of flashes will indicate percent battery charge from 10% (1 flash) to 100% (10 flashes). For example, 5 flashes would indicate a 50% charge.
7. A short press of the [STANDBY] key will also display the % battery state of charge if the motor is on. (see 6 above for a description of the LED display in this mode.
8. Motor error states are indicated by the Green power LED on the other side of the Nordic board. There are three error states indicated:

    1 - Motor initialization errors are indicated by a slowly flashing LED. 

    2 - Firmware mismatch errors are indicated by a fast flashing LED. 

    3 - Motor configuration errors are indicated by a full on Green LED.


Short Press buttons
----
* Short Press the [STANDBY] button to display the motor batter state of charge. 
* Short Press the [ENTER] button to switch pages (pageup) on an ANT+ CONTROLS device (ie: garmin bike computer) 
  (You can also use the long press of [PLUS] and [MINUS] to pageup/pagedown - see below)
* Short press the [PLUS] button to increase the motor assist level (ANT+ LEV control)
* Short press the [MINUS] button to decrease the motor assist level (ANT+ LEV control)
  
Long Press Buttons
-----
* Long Press the STANDBY button to turn on/off the motor. See LED Operation Signalling above for a description of operation.
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