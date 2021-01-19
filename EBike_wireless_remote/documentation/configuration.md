# Remote Configuration Options
The device configuration of the remote can be set via a bluetooth characteristic.

Once set, the configuration options are permanently stored in flash memory.
To change the remote configuration, long press the PLUS button to turn on bluetooth.
As an example of making a configuration change, see changing the ANT+ LEV Device Number (to connect to only one specific eBike) using the Nordic nRF Connect app in this video:

[![video](https://img.youtube.com/vi/_ALauuDxZuQ/hqdefault.jpg)](https://youtu.be/_ALauuDxZuQ) 

## The currently implemented configuration options are:
1. Setting the ANT+ Device number for the ANT LEV ebike. This ensures the remote will only pair to a specific ebike. valid inputs are hexadecimal 0x01 - 0x59 (decimal 01-89)
Please note that setting the device number to 0x00 is a 'wild card' setting that will cause the remote to pair with ANY ebike device number.
-----------------
2. __Turn on/off ANT+ LEV control.__ 
 (input hex 0x90 to turn on, hex 0x91 to turn off) 
This allows the remote to control only a garmin bike computer or any other device that implement the ANT+ Controls profile
----------
3. __Turn on/off ANT+ CONTROLS__  
 (input hex 0x92 to turn on, hex 0x93 to turn off)  
 This allows the remote to turn on/off the ANT+ CONTROLS capability depending on the use case. ie: the user may not have a garmin bike computer that needs to be controlled.
 --------
 4. __Turn on/off brake control__  depending on the use case
 (input hex 0x94 to turn on, hex 0x95 to turn off)  
 ---------
1. __Boot the remote control into Device Firmware Update (DFU) mode__ by inputting 0x99 to allow the remote firrmware to be updated wirelessly using bluetooth.
(input hex 0x99 to boot into DFU)  
---------
A video showing a DFU process can be seen here:
[![video](https://img.youtube.com/vi/va3LJoiosoc/hqdefault.jpg)](https://youtu.be/va3LJoiosoc) 
-------
## Display the current configuration
The currently selected configuration options can be displayed using the on board led by long pressing the ENTER button on the remote.
The LED will continue a visual cycle spanning 2 seconds when the button is held down as follows:

- __RED LED__ (0n for 0.5 seconds) - ANT+ LEV active
- __GREEN LED__ (0n for 0.5 seconds) - ANT+ CONTROLS active
- __BLUE LED__ (0n for 0.5 seconds) - brake control active
- __ALL LEDs OFF__ for 0.5 seconds
- (__repeat__ while button is pressed)

## [back](../README.md)
