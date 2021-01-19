# Building the Bootloader
## The Garmin S340 softdevice
This bootloader (as well as the TSDZ2 remote control and controller firmwares) require the Garmin S340 softdevice to gain access to the bluetooth stack. Because of this, the S340 softdevice hex file needs to be flashed along with the bootloader hex file. The MAKE file will automatically combine these two hex files and flash the resulting combined file to the board. 
## Release Binary Hex file
If you're just looking to flash the combined bootloader and softdevice, simply flash this hex file using OPENOCD from the launch json menu in vscode for the respective giyhub repository source code for the TSDZ2 remote and controller. 
> The firmware HEX file to flash is in the github release.

## Bootloader build options
Hex for the combined bootloaded and S340 softdevice flashes are output to the `_build` directory after compilation and linking success.
`NRF_DFU_BL_ALLOW_DOWNGRADE`  will allow the firmware to accept a downgraded version if desired
 `NRF_DFU_BL_ACCEPT_SAME_VERSION`).will allow the firmware to acccept the same version
 `BOARD_USE_SF_CLOCK` is defined to utilize the onboard Soft Device LFCLK.
### TSDZ2 remote control and Controller builds
for both firmwares, the respective MAKE files will automatically put the combined hex file and the signed firmware upgrade zip files in the _upgrade folder after the build process is completed.

## [back](../README.md)
