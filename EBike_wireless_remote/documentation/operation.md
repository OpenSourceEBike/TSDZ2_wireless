
# **Remote operation**
## Getting started with your new remote 

**To get started with the remote, follow this 4 step process:**

1. Ensure that the second wireless board is powered up and connected to the TSDZ2 motor controller.
2. To turn on the motor, press and hold the **POWER** key on the remote control.
A **RED** LED will briefly flash, followed by a flashing **RED+BLUE** LED. This LED will continue flashing until a connected is established with the wireless controller.
3. press the **PLUS/MINUS** keys briefly to increase/decrease the motor assist level
4. To turn off the motor, press and hold the **POWER** key on the remote control.
A **RED** LED will briefly flash, followed by a flashing **GREEN** LED. This LED will indicate the current state of charge of the battery by one flash for every 10% of charge.

That's basically all you need to use the remote for basic operation. 
Read the "**Button Operation**" section below for more details on the remote's operation.
If you would like to configure the remote, please read the "**Remote Operating Modes**" section next. 
Enjoy!!

## Remote Operating Modes

The remote has a 2 configuration options, namely specifying a specific ANT ID for the ebike and setting ANT LEV control options 

1. The ANT ID allows you to pair the remote to a specific TSDZ2 motor. The default ANT ID of 0 will allow the remote to pair with any TSDZ2 motor within range. This is a useful option if you have multiple ebikes. The ANT ID can be changed by a bluetooth connection to the remote.
2. The ANT LEV control options allow you to pair the remote to an ebike and/or a garmin bike computer simultaneously.
The remote has two operating modes, **NORMAL** **MODE** which is the usual mode of operation, and **CONFIGURATION** **MODE**, which is used to set the remote operating options described above. Pressing the **ENTER** key for a long press of more than 5 seconds will toggle between **NORMAL MODE** and **CONFIGURATION MODE**.<br>

In either mode, if a key is pressed while inactive the **RED LED** will briefly flash.

----

## Button Operation (NORMAL MODE)

----

* Short Press the [**POWER**] button to display the motor battery state of charge. Battery state is indicated by flashing the **GREEN LED**. The number of flashes will indicate battery charge state from from 1 flash (10% charge) to 10 flashes (100% charge). (Motor On **ONLY**.)  
* Long Press the [**POWER**] button to turn on/off the motor. See **LED Signalling** below for a description of operation.
* Short Press the [**ENTER**] button to **PAGE UP** on a connected garmin bike computer. (Works **ONLY** if garmin control is activated)
* Long Press the [**ENTER**] button to **PAGE DOWN** on a connected garmin bike computer. (Works **ONLY** if garmin control is activated)
* Short press the [**PLUS**] button to increase the motor assist level (Wireless TSDZ2 and supported **ANT+ LEV** enabled e-bikes)
* Long press the [**PLUS**] button to toggle the lights on/off (Light support is a planned feature, not yet implemented)
* Short press the [**MINUS**] button to decrease the motor assist level (Wireless TSDZ2 and supported **ANT+ LEV** enabled e-bikes)
* Long press and hold the [**MINUS**] button to start the motor walk assist. Releasing the [**MINUS**] button stops walk assist (Wlak assist is a planned feature, not yet implemented)
* Long press the [**MINUS**] + [**PLUS**] buttons to put the remote control in 'deep sleep' low power mode.
* A very long press (more than 5 seconds) of the [**ENTER**] button will enter **CONFIGURATION MODE**
  
----

## Button Operation (CONFIGURATION MODE)

----

* A short press of the [**ENTER**] key will cycle through the configuration LED display. A **RED LED** indicates support for ANT LEV ebikes, while a BLUE LED indicated support for Garmin bike computers. [See Configuration Options](configuration.md)
* A Long press the [**ENTER**] button will exit **CONFIGURATION MODE** and return to **NORMAL MODE** . **CONFIGURATION MODE** will also automatically turn off if no keys are pressed for 5 minutes. 
* A Long Press the [**POWER**] button will initate Device Firmware Update (DFU) mode.  Either the remote or bootloader firmware can be updated to a new version using a provided upgrade packet zip file in DFU mode. For more information click [here](dfu.md).
* A Short press of the [**PLUS**] button will enable support for Garmin bike computers.(disabled by default)
* A Short press of the [**MINUS**] button will disable support for Garmin bike computers.
* A Long press of the [**PLUS**] button will enable support for either a Wireless TSDZ2 or an ANT LEV enabled ebike. (enabled by default)
* A Long press of the [**MINUS**] button will disable support for either a Wireless TSDZ2 or an ANT LEV enabled ebike.

----

## LED Signalling (NORMAL MODE)

----

1. ANT Searching/Connection
   When the remote is searching for either an ANT+ LEV or an ANT+ Controls connection, the **RED LED** will slowly flash. When a connection is made, the **RED LED** will quickly flash and then go out.
2. The **GREEN LED** will flash once briefly when the [**PLUS**] or [**MINUS**] keys are pressed to indicate the assist level is changing. if the assist level has reached either 0 or 7, pressing the [**PLUS**] or [**MINUS**] keys key will cause the led to briefly flash **RED** multiple times to indicate the limit has been reached. 
3. If the motor is off, pressing the [**PLUS**] or [**MINUS**] keys will cause a **RED** LED to flash. The assist level will NOT change with the motor off. 
4. The **RED LED** will briefly flash to indicate a long press has been made
5. A very long press (> 5 seconds) of the [**ENTER**] key will flash the BLUE LED for 2 seconds when entering the configuration mode.
6. A long press of the [**POWER**] key will turn the motor ON or OFF. When the motor is initializing, the leds will flash [**OFF-WHITE**]. When the motor is on, the **OFF-WHITE LED** will rapidly flash. If the motor is turning off, the battery state will be displayed. Battery state is indicated by flashing the **GREEN LED**. The number of flashes will indicate battery charge state from from 1 flash (10% charge) to 10 flashes (100% charge).
7. When walk mode is active the **BLUE LED** will flash slowly.

----

## LED Signalling (CONFIGURATION MODE)

----

1. The **BLUE LED** will flash for 2 seconds to indicated configuration mode is active.
2. The **OFF-WHITE LED** will turn on for 2 seconds to indicated that the remote is entering DFU mode.
3. A short press of the ENTER key will display the current remote configuration[See Configuration Options](configuration.md)

----

See controlling a Garmin 1030 bike computer for assist levels and page control using a simulated ANT+ LEV Ebike in this video:

[![video](https://img.youtube.com/vi/s7URIMVzcwc/hqdefault.jpg)](https://www.youtube.com/watch?v=s7URIMVzcwc)

----

See changing the ANT+ LEV Device Number (to connect to only one specific eBike) using the Nordic nRF Connect app in this video:

[![video](https://img.youtube.com/vi/_ALauuDxZuQ/hqdefault.jpg)](https://youtu.be/_ALauuDxZuQ) 

----

nRFConnect is available on the play store [here](https://play.google.com/store/apps/details?id=no.nordicsemi.android.mcp&hl=en_CA&gl=US)

----

## [back](../README.md)