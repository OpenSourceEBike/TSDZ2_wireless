# **Remote operation**
## Getting started with the Wireless remote 

**To get started , follow this simple four step process (or five steps if you have connected the e-brakes):**
(Please note that buttons will briefly flash **RED** if they are inactive)

1. Ensure that the second wireless board is powered up and connected to the TSDZ2 motor controller. Ensure that the remote has a battery connected and press any button to start the pairing process with the motor controller. When the remote is searching for an ANT connection, the **RED LED** will slowly flash. When a connection is made with the motor, the **GREEN LED** will short flash twice followed by one long flash to indicate connection.
2. To turn on the motor from the remote control, press and hold the **POWER** key on the remote control. A  **YELLOW** LED will start flashing, and will continue flashing until the motor is on, at which time a **GREEN** LED will short flash twice followed by one long flash to indicate that the motor is on.
3. press the **PLUS/MINUS** keys briefly to increase/decrease the motor assist level. The **GREEN** LED will briefly flash on every **PLUS** press of the button, while the **YELLOW** LED will briefly flash on every MINUS press of the button. There are 7 levels of assist. When you reach level 7 (maximum assist) or level 0 (motor off) the **RED** LED will rapidly flash.
4. (Optional step needed you have connected the e-brakes) Depress either the right or left brake lever. The **RED** LED will light and the motor assist will stop. Release the brake lever and the **RED** led will turn off and the motor assist will resume.
5. To turn off the motor, press and hold the **POWER** key on the remote control. The **RED** LED will short flash twice, followed by one long flash to indicate the motor is off. The LED will then flash one flash for every 10% of charge remaining in the battery.(ie:5 flashes=50% charge remaining). The flashes will be **RED** from 0%-40%, **YELLOW** from 40%-70%, and **GREEN** from 70% to 100% of charge.

### **That's basically all you need to get started with the wireless remote.**

Read the "**Button Operation (NORMAL MODE)**" section below for more details on the remote's operation when riding your e-bike.

If you would like to configure the remote, please read the "**Remote Operating Modes**" section next. Please note that the remote will control the motor "out of the box" and no configuration is necessary unless you would like to pair a specific bike computer, set specific ANT configurations, or wirelessly update the remote firmware . Most people can safely ignore **CONFIGURATION MODE** and still enjoy full functionality from their remote.

Have Fun!!

## Remote Operating Modes
The remote has two modes of operation; **CONFIGURATION MODE** and **NORMAL MODE**, for configuration and normal operation, respectively.

----

## NORMAL MODE

----

**General LED button signals:**

* When the motor is off, inactive buttons will briefly flash a **RED** LED when pressed.

* Long Pressing a button will briefly flash a **GREEN** LED to indicate that the button has been pressed
  
----
**Short Press Button Actions:**

* Short Press the [**PLUS**](**GREEN** LED) /[**MINUS**] (**YELLOW** LED) buttons to increase/decrease the assist level. If the assist level has reached either 0 or 7, pressing the [**PLUS**] or [**MINUS**] keys key will cause the led to briefly flash **RED** multiple times to indicate the limit has been reached. 
* Short Press the [**POWER**] button to display the motor battery state of charge. Battery state is indicated by flashing. The number of flashes will indicate battery charge state from from 1 flash (10% charge) to 10 flashes (100% charge). The flashes will be **RED** from 0%-40%, **YELLOW** from 40%-70%, and **GREEN** from 70% to 100% of charge.
(Motor On **ONLY**.)  
 * Short Press the [**ENTER**] button to **PAGE UP** on a connected garmin bike computer. (Works **ONLY** if garmin control is activated)

**Long Press Button Actions:**

* Long Press the [**ENTER**] button to **PAGE DOWN** on a connected garmin bike computer. (Works **ONLY** if garmin control is activated)
* A long press of the [**POWER**] key will turn the motor ON or OFF. A  **YELLOW** LED will start flashing, and will continue flashing until the motor is on, at which time a **GREEN** LED will short flash twice followed by one long flash to indicate that the motor is on. If the motor is turning off, The **RED** LED will short flash twice, followed by one long flash to indicate the motor is off. The LED will then flash one flash for every 10% of charge remaining in the battery.(ie:5 flashes=50% charge remaining). The flashes will be **RED** from 0%-40%, **YELLOW** from 40%-70%, and **GREEN** from 70% to 100% of charge.
* Long press the [**PLUS**] button to toggle the lights on/off 
* Long press and hold the [**MINUS**] button to start the motor walk assist. When walk mode is active the **GREEN**  LED** will flash slowly.Releasing the [**MINUS**] button stops walk assist.
* Long press the [**MINUS**] + [**PLUS**] buttons to put the remote control in 'deep sleep' low power mode. The **RED** and **YELLOW** LEDS will short flash followed by a long flash of the **GREEN** LED to indicated initiation of low power mode.
* A very long press (more than 5 seconds) of the [**ENTER**] button will enter **CONFIGURATION MODE**. The **BLUE** LED will flash rapidly 10 times to indicate thay configuration mode is active.
  
----

## CONFIGURATION MODE

----
In **CONFIGURATION MODE**, the remote has a 2 configuration options, namely specifying a specific ANT ID for use with the ebike and setting up ANT LEV control options. Both of these settings are optional as the remote will default to control the TSDZ2 wireless controller without the need to set anything.

* The ANT ID allows you to pair the remote to a specific TSDZ2 motor. The default ANT ID of 0 allows the remote to pair with any TSDZ2 motor within range. Any other number will cause the remote to ONLY pair with a motor with the same ANT ID. This is a useful option if you have multiple ebikes. The ANT ID can be changed by a bluetooth connection to the remote.
* The ANT LEV control options allow you to pair the remote to an ebike alone, (the default option) , a bike computer (like the Garmin Edge series) , or any combination of these devices.
to enter and exit **CONFIGURATION MODE** press and hold the [**ENTER**] button for more than 5 seconds. The **BLUE** LED will flash rapidly 10 times to indicate that configuration mode is active, followed by a visual display of ANT  options. The **RED** LED will flash 5 times to indicate TSDZ2 motor control is active, followed by the **GREEN** LED flashing 5 times to indicate that control of Garmin bike computers is active <br>

----

## Button Operation (CONFIGURATION MODE)

----

* Short press the ENTER key to display configuration options. The **RED** LED will flash 5 times to indicate TSDZ2 motor control is active, followed by the **GREEN** LED flashing 5 times to indicate that control of Garmin bike computers is active
* Long press the ENTER key to leave **CONFIGURATION**  mode. The **BLUE** LED will flash 5 times to indicate you are entering **NORMAL** mode
* Short press the **PLUS/MINUS** keys to enable/disable garmin control. (The **GREEN** LED will flash 5 times to indicate that garmin control is active)
* Long Press PLUS/MINUS keys will enable/disable TSDZ2 motor control(The **RED** LED will flash 5 times to indicate that motor control is active)
* A Long Press of the [**POWER**] button will initate Device Firmware Update (DFU) mode.  The **BLUE** LED will flash rapidly 10 times, and the board will enter DFU mode.Either the remote or bootloader firmware can be updated to a new version using a provided upgrade packet zip file in DFU mode. For more information click [here](dfu.md).



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