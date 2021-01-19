# The Device Firmware Update (DFU) Process
The DFU process can be run by using one of the following Nordic tools. Each of these tools is used to send the DFU package to the target device to perform the update.

* The nrfutil command-line tool
* nRF Connect for desktop
* nRF Connect for mobile
Two devices are involved in the DFU process: the DFU controller which transfers the DFU package, and the DFU target which receives and applies the DFU package. (the TSDZ2 remote or controller)

---------
Step 1. Generate DFU .zip packet
--------
--------
A DFU .zip packet is required for the DFU master to send new image(s) to the TSDZ2 remote control or controller boards.(the target) The .zip file contains the image hex file(s) we want to update and the init packet, including the signature of the packet. 
Prepare the TSDZ2 application zip file. Build the TSDZ2 remote control and/or the TSDZ2 controller and find the .zip file inside the _build folder. It's named TSDZ2_combined_with_sd. 
The make file uses nrfutil to generate the packet zip file.
The syntax is as follows:
nrfutil pkg generate --hw-version 52 --application-version 1 --application nrf52832_xxaa.hex --sd-req 0x98 --key-file private.key app_dfu_package.zip
Explanation:
--hw-version: This should match the chip on the dongle. We use the nRF52840 SOC chip, hence "52". 
--application-version: By default the start number for the application version is 0.  
--sd-req: For the TSDZ2, we need Softdevice S340 v6.1.1. The number to use for this softdevice version is 0xB9. 
--application : Tells nrfutil that you going to update the application and an application image is provided.

----------
Step 2. Performing DFU (Bluetooth)
--------
---------
Now you have your DFU .zip file containing the application update and the bootloader and bootloader settings installed on the board, it's time to actually do wireless DFU.
1.  Start DFU mode
     This can be done by entering 0x99 as a configuration variable in either the remote control or wireless controller firmware using bluetooth control.
     You can also initiate DFU mode by holding the board button down for several seconds during a reset. (ie: when power cycling, or pressing the reset button on the board)
   
     Verify the bootloader starts advertising as "TSDZ2_DFU".You’ll need to make sure the package you created in step #7 is accessible on the mobile phone you’re using (if you’re using nRF Connect for Mobile). Now, connect to the DFU target:

2. Copy the DFU .zip file you just generated to the phone or to the folder on PC of your choice.
   
3. Use nRFConnect/nRFToolbox app either on the phone or on PC to connect and do an OTA DFU using the .zip file you copied (Press DFU button).After you’ve connected to the DFU target in nRF Connect on Mobile, swipe to the left twice to navigate to the DFU screen. Then click “Open Document Picker”:Choose the .zip file  and start the DFU process.
Click the “Start” button:
   
4. Once the DFU process is complete, you should see that your new application (that was part of the DFU package) is now running on the DFU Target 

Here is a video showing a wireless DFU for the TZDZ2 remote control:
[![video](https://img.youtube.com/vi/va3LJoiosoc/hqdefault.jpg)](https://youtu.be/va3LJoiosoc)

## [back](../README.md)
