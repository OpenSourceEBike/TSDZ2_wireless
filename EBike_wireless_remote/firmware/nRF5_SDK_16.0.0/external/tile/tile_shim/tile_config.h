/**
 * NOTICE
 * 
 * Copyright 2018 Tile Inc.  All Rights Reserved.
 * All code or other information included in the accompanying files ("Tile Source Material")
 * is PROPRIETARY information of Tile Inc. ("Tile") and access and use of the Tile Source Material
 * is subject to these terms. The Tile Source Material may only be used for demonstration purposes,
 * and may not be otherwise distributed or made available to others, including for commercial purposes.
 * Without limiting the foregoing , you understand and agree that no production use
 * of the Tile Source Material is allowed without a Tile ID properly obtained under a separate
 * agreement with Tile.
 * You also understand and agree that Tile may terminate the limited rights granted under these terms
 * at any time in its discretion.
 * All Tile Source Material is provided AS-IS without warranty of any kind.
 * Tile does not warrant that the Tile Source Material will be error-free or fit for your purposes.
 * Tile will not be liable for any damages resulting from your use of or inability to use
 * the Tile Source Material.
 *
 * Support: firmware_support@tile.com
 *
 */

/** @file tile_config.h
 ** @brief Configuration for Tile functionality
 */

#ifndef TILE_CONFIG_H_
#define TILE_CONFIG_H_


/* You need valid Tile Node Credentials [interim_tile_id, interim_tile_key] in order to access Tile Network.
 * Without valid Tile Node credentials, your prototype will not interoperate with the Tile Network or Tile Mobile Application.
 * Follow this link to get Tile Node credentials for your prototype: https://www.thetileapp.com/en-us/platform-business-solutions
 * Then fill interim_tile_id and interim_tile_key with the numbers provided to you by Tile and comment out the warning.
 */
#define INTERIM_TILE_ID   {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
#define INTERIM_AUTH_KEY  {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
#if !defined(INTERIM_TILE_ID) && !defined(INTERIM_AUTH_KEY)
    #warning Did you get your Tile Node credentials from Tile?
#endif // !defined(INTERIM_TILE_ID) && !defined(INTERIM_AUTH_KEY)

/*  Model Number Format shall follow the following pattern: "XXXX YY.YY" with the following constraints:
 *    - "XXXX" uses 4 ASCII letters ('A' to 'Z') to describe the Vendor ID.
 *    - The Vendor ID is assigned by Tile. Contact firmware_support@tile.com if you don't have one (required before Mass Production).
 *    - A space character after "XXXX".
 *    - "YY.YY" uses 4 ASCII numbers ('0' to '9') and describes the Model ID.
 *  Example: "TEST 00.00".
 */
#define TILE_MODEL_NUMBER "TEST 02.00"

/*  HW Version character pattern is "YY.YY" and uses 4 ASCII numbers ('0' to '9').
 *  Example: "01.00".
 *  You are free to use any number that is convenient for your product, providing it satisfies the character pattern.
 */
#define TILE_HARDWARE_VERSION "00.01"


#define TILE_DEFAULT_MODE TILE_MODE_SHIPPING

#define TILE_ENABLE_PLAYER 1

#endif
