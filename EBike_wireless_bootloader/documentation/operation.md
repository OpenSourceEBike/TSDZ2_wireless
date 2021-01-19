# Bootloader Operation
 Based on the 'Secure Bootloader' example provided in the Nordic nRF52 development kit. 
- DFU over BLE (bluetooth low energy) OTA 
- DFU using USB (Windows or Unix PC's)
- Bootloader is self upgradable via either OTA or USB Serial
- Bootloader will time out and load a valid application if no DFU is initiated 
- Bootloader will sense pressed button pins on start up.
  If button is pressed then an adjustable timer starts, when timed out enters DFU mode. 
  If button is released before timeout, the bootloader will start the installed application immediately.
- Optionally, the firmware upgrade can be downgradable (`NRF_DFU_BL_ALLOW_DOWNGRADE` Makefile option)

## DFU mode will be initiated by any one of the following:
  ### 1. Bootloader cannot find a valid application to load
  If there is no valid application in flash memory, the bootloader will immediately enter DFU mode.
  ### 2. Bluetooth access in DFU mode
  A BLE packet is used to switch to DFU mode without physical contact. The way it works is pretty simple, to switch, we write to the retention register GPREGRET a flag and then we do a soft reset. Since the retention register keeps its value after the reset, the bootloader will check this value when it booting up after the reset and then can enter DFU mode instead of starting the normal application. This is the same behavior as when we hold the board button and trigger a reset.
  ### 3. Button based DFU 
  If, for any reason, you want to force the firmware into DFU mode, this can be done using the physical button on the board. To enter DFU mode press the button on the board during board reset. A reset can be accomplished by either a power cycle or by pressing the reset button on the board. 




## [back](../README.md)