# Security Encryption
Note that the TSDZ2 bootloader source code is distributed with a public and private keypair for application signing. (private.key & public.key). This will permit anyone who has access to the private key to be able to flash and overwrite any application you have while in OTA DFU mode. If you want to protect your eBike firmware fron unauthorized access, build your own binary with a new generated keypair.
### Generating new keys
You will need to generate a pair of Public and Private Keys to encrypt the signature and sign the DFU image using ECDSA_P256_SHA256.
Nordic provides the nRFutil tool to generate these keys. nRFutil.exe can be downloaded from github [here](https://github.com/NordicSemiconductor/pc-nrfutil), the user guide is [here](https://www.nordicsemi.com/-/media/DocLib/Other/User_Guides/nrfutilv16pdf.pdf).
Or acquired from python using pip3 install nrfutil. To check for update, call pip3 install nrfutil --upgrade (or use python -m pip install nrfutil)
 
A1. Generate your own private key. Type this command line:
nrfutil.exe keys generate private.key
private.key file will be generated. You will keep this key secret
A2. Generate your public key based on your private key.
nrfutil keys display --key pk --format code private.key --out_file public_key.c
After you have the public_key.c file we can build the bootloader using the make file.

## Boot-up Sequence
Based on the settings stored in the bootloader settings page, the bootloader determines: whether the application exists, and the location of it. The secure bootloader performs a signature verification of the application before booting into it.

Here are the boot-up steps that occur from reset to starting the application:

First, the MBR is booted.
The MBR looks up the location of the bootloader.
If a bootloader is found, it is run by the MBR.
A secure bootloader:
 (1) uses cryptographic operations to verify the signature of the firmware (Authenticity) and 
 (2) that is it not corrupted (Data Integrity). This is performed in two scenarios: at bootup, and when a new firmware image is received.
If a bootloader is not found, the MBR boots the image that follows it (the MBR) at address 0x1000 (the SoftDevice).
The SoftDevice then boots the application.

There are four different boot validation modes that can be configured:

1. Signature validation (ECDSA) – most secure, and data integrity check.

2. Hash validation (SHA-256) – less security, and data integrity check.
  
3. CRC validation (CRC32) – no security, only data integrity check.
  
4.  No validation – no security, no integrity check.

These modes are configured as part of the firmware update package. If a signature mode is specified, then the signature will exist in the package. For hash and CRC validation, the cryptographic digest is created on-chip and written to flash when the update is applied.

Important Note: the boot validation is independent of the firmware update validation process. This means that the update package is signed regardless of the secure boot mode contained in it. This ensures that the system is protected from unauthorized firmware updates even with no boot validation.

## Secure Boot Validation
There is a boot validation performed at the booting phase of the bootloader involving for the application, softdevice and the bootloader.
In short, a normal secure booting sequence starts with the MBR booting first, it then starts the bootloader. The bootloader does two tasks, it protects it self using BPROT/ACL protection and then validate the softdevice and the application image. You can choose the boot option when generating the DFU package. Use --sd-boot-validation for softdevice validation and --app-boot-validation when generating the DFU .zip package with nrfutil, you have 4 options to choose from:
- NO_VALIDATION
- VALIDATE_GENERATED_CRC
- VALIDATE_GENERATED_SHA256
- VALIDATE_ECDSA_P256_SHA256. 
This also applies for bootloader setting. You can generate the bootloader setting with the same option for boot validating the application and/or the softdevice, instead of just CRC validation for application as before. 



## [back](../README.md)
