# Configure Code Studio IDE for firmware development, flash and debug

For the firmware development, flash and debug, you can use the OpenSource and free IDE [Visual Studio Code](https://code.visualstudio.com/). For sure that others will work like the Eclipse.

This guide assumes you are using Linux Ubuntu (if you are on Windows or such, the install process should be similar).

## Software to install

- Add your user to the dialout group: __sudo usermod -a -G dialout USER_NAME__ and reboot
- install [Visual Studio Code](https://code.visualstudio.com/)
  - install C/C++ extension (to edit C/C++ source files)
  - install Cortex-Debug extension (to be able to debug ARM microcontroller)
  - install Task Manager extension (to call makefile and other commands)
- install OpenOCD (to connect to STLinkV2): __sudo apt-get install openocd__
- install GDB GNU debugger (for debug and connect to OpenOCD): __sudo apt-get install gdb-multiarch__
- install ARM C/C++ GCC compiler: __sudo apt-get install gcc-arm-none-eabi__
- install ARM binutils: __sudo apt-get install binutils-arm-none-eabi__
- install ARM newlib: __sudo apt-get install libnewlib-arm-none-eabi__
- install srec_cat tool: __sudo apt-get install srecord__

## Open project folder with Visual Studio Code

Click on menu File -> Open folder... and select the firmware directory. You should see the firmware similar to this:

![](flash_debug_1.png)

## Build the code

Because you installed the Task Manager extension, you should see Task manager icon on the left bar - click on it. The build, clean and Launch OpenOCD tasks should be available.

Start by clicking on the clean to clean the code and then click on the build to build the code and you should see something similar to this - note the output on the terminal:

![](flash_debug_2.png)

The clean and build tasks call the __make clean__ and __make__ on the terminal. The tasks configurations are on the file .vscode/tasks.json.

## Flash the firmware and debug

1. Connect the STLinkV2 to the board. If you have a nRF52840 MDK Dongle see the pinout in the schematic folder. If you have the blue nRF52840 Dongle it has a different pinout, see [nRF52840 Dongle Pinout.png](./nRF52840%20Dongle%20Pinout.png), note that you do not need to connect the RST pin.

Note that you will also need to install the STLinkV2 udev rules file that are on the firmware/tools/ folder, so the STLinkV2 can be accessed by the OpenOCD:
```
sudo cp 60-st_link_v2.rules /etc/udev/rules.d
sudo udevadm control --reload-rules
```

2. Click on the task Launch OpenOCD

You will need to click only once. The OpenOCD should connect to the microcontroller and keep active - see the example output of OpenOCD on the terminal, when a correct connection was done with the microcontroller:

![](flash_debug_3.png)

The task Launch OpenOCD configuration is on the file .vscode/tasks.json.

3. Click NRF52 Flash and Debug

Click on the debug icon on the left panel and then click on the small green arrow for the NRF52 Flash and Debug - will flash the firmware on the microcontroller and then start the debug of the firmware.. Wait a few seconds and you should see something like this:

![](flash_debug_4.png)

The NRF52 Flash and Debug configuration is on the file .vscode/launch.json.

## Troubleshooting

### Flashing appears to work fine but I cannot debug
If you are flashing code via your STLinkV2 but unable to debug as when you pause the debugger it stops in unknown code you cannot step through (you see the error `ERROR: Unexpected GDB output from command "-exec-next". Cannot find bounds of current function`) then the issue is that your bootloader is doing CRC validation which fails and then refusing to load your code. You have two options to fix this:

1. Change the makefile to generate the bootloader settings with correct CRC in e.g. (you will need to install nrfutil `pip install nrfutil`)
```make
TSDZ2_with_SD:
  nrfutil settings generate --family NRF52840 --bootloader-version 0 --bl-settings-version 1 --application $(OUTPUT_DIRECTORY)/TSDZ2_wireless.hex --application-version 4294967295 $(OUTPUT_DIRECTORY)/settings.hex
  $(SREC_PATH)srec_cat $(OUTPUT_DIRECTORY)/settings.hex -Intel $(SOFT_DEVICE) -Intel $(OUTPUT_DIRECTORY)/TSDZ2_wireless.hex -Intel -Output $(OUTPUT_DIRECTORY)/TSDZ2_wireless_with_SD.hex
```
2. Or flash a more permission bootloader which doesn't do CRC checks using the dfu/open_bootloader/pca100059_usb_debug example from the SDK.
##[back](../README.md)