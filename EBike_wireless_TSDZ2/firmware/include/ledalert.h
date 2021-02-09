/*
 * LED Sequence player
 *
 * Copyright (C) 4var1/beemac, 2020.
 *
 * Released under the GPL License, Version 3
 */
#pragma once

#define LED_CLOCK_MS                            50

// LED Sequence command defines\macros

#define LED_RED                                 1
#define LED_GREEN                               2
#define LED_BLUE                                4
#define LED_MAGENTA                             LED_BLUE | LED_RED
#define LED_CYAN                                LED_BLUE | LED_GREEN
#define LED_YELLOW                              LED_RED | LED_GREEN
#define LED_WHITE                               LED_BLUE | LED_RED | LED_GREEN
#define LED_OFF                                 0
#define LED_END_SEQUENCE                        253
#define LED_REPEAT_LASTX                        254
#define WAIT_MS(a) ((a) / (LED_CLOCK_MS))
#define CMDS_RPT(a,b) ((a) + (16 * (b)))         //Repeat last x commands - next value last x-1 commands to repeat (0-15) plus 16*number of times - so max is repeat last 16 commands 16 times

// Define sequences here
// Sequence commands are 2 bytes each - first is the colour or other instruction, 2nd is either time to show for LED - or parameter for command.
// End sequences with two LED_END_SEQUENCE
// can loop previous commands with LED_REPEAT_LASTX, CMDS_RPT(2,10) where the two values are the number of previous commands to repeat,how many times.
// Don't nest loops in led sequences, there is no stack!

#define LED_NUM_SEQUENCES                       25 //Update when new sequences are added
#define LED_MAX_COMMANDS_IN_SEQUENCE            16

static const uint8_t ui8_led_sequences [LED_NUM_SEQUENCES][LED_MAX_COMMANDS_IN_SEQUENCE * 2] = {

    {LED_GREEN,WAIT_MS(200),LED_OFF,WAIT_MS(0),LED_END_SEQUENCE,LED_END_SEQUENCE},                                                         //LED_SEQUENCE_SHORT_GREEN

    {LED_RED,WAIT_MS(200),LED_OFF,WAIT_MS(0),LED_END_SEQUENCE,LED_END_SEQUENCE},                                                           //LED_SEQUENCE_SHORT_RED

    {LED_BLUE,WAIT_MS(200),LED_OFF,WAIT_MS(0),LED_END_SEQUENCE,LED_END_SEQUENCE},                                                          //LED_SEQUENCE_SHORT_BLUE

    {LED_YELLOW,WAIT_MS(200),LED_OFF,WAIT_MS(0),LED_END_SEQUENCE,LED_END_SEQUENCE},                                                        //LED_SEQUENCE_SHORT_YELLOW

    {LED_BLUE,WAIT_MS(50),LED_OFF,WAIT_MS(0),LED_END_SEQUENCE,LED_END_SEQUENCE},                                                           //LED_SEQUENCE_BLUEFLASH_1

    {LED_BLUE,WAIT_MS(50),LED_OFF,WAIT_MS(50),LED_BLUE,WAIT_MS(50),LED_OFF,WAIT_MS(0),LED_END_SEQUENCE,LED_END_SEQUENCE},                  //LED_SEQUENCE_BLUEFLASH_2

    {LED_BLUE,WAIT_MS(50),LED_OFF,WAIT_MS(50),LED_REPEAT_LASTX,CMDS_RPT(2,1),LED_BLUE,WAIT_MS(50),LED_OFF,WAIT_MS(0),
                                                                            LED_END_SEQUENCE,LED_END_SEQUENCE},                            //LED_SEQUENCE_BLUEFLASH_3

    {LED_BLUE,WAIT_MS(50),LED_OFF,WAIT_MS(50),LED_REPEAT_LASTX,CMDS_RPT(2,2),LED_BLUE,WAIT_MS(50),LED_OFF,WAIT_MS(0),
                                                                            LED_END_SEQUENCE,LED_END_SEQUENCE},                            //LED_SEQUENCE_BLUEFLASH_4

    {LED_BLUE,WAIT_MS(50),LED_OFF,WAIT_MS(50),LED_REPEAT_LASTX,CMDS_RPT(2,3),LED_BLUE,WAIT_MS(50),LED_OFF,WAIT_MS(0),
                                                                            LED_END_SEQUENCE,LED_END_SEQUENCE},                            //LED_SEQUENCE_BLUEFLASH_5

    {LED_BLUE,WAIT_MS(50),LED_OFF,WAIT_MS(50),LED_REPEAT_LASTX,CMDS_RPT(2,4),LED_BLUE,WAIT_MS(50),LED_OFF,WAIT_MS(0),
                                                                            LED_END_SEQUENCE,LED_END_SEQUENCE},                            //LED_SEQUENCE_BLUEFLASH_6

    {LED_BLUE,WAIT_MS(50),LED_OFF,WAIT_MS(50),LED_REPEAT_LASTX,CMDS_RPT(2,5),LED_BLUE,WAIT_MS(50),LED_OFF,WAIT_MS(0),
                                                                            LED_END_SEQUENCE,LED_END_SEQUENCE},                            //LED_SEQUENCE_BLUEFLASH_7

    {LED_BLUE,WAIT_MS(50),LED_OFF,WAIT_MS(50),LED_REPEAT_LASTX,CMDS_RPT(2,6),LED_BLUE,WAIT_MS(50),LED_OFF,WAIT_MS(0),
                                                                            LED_END_SEQUENCE,LED_END_SEQUENCE},                            //LED_SEQUENCE_BLUEFLASH_8

    {LED_BLUE,WAIT_MS(50),LED_OFF,WAIT_MS(50),LED_REPEAT_LASTX,CMDS_RPT(2,7),LED_BLUE,WAIT_MS(50),LED_OFF,WAIT_MS(0),
                                                                            LED_END_SEQUENCE,LED_END_SEQUENCE},                            //LED_SEQUENCE_BLUEFLASH_9

    {LED_BLUE,WAIT_MS(50),LED_OFF,WAIT_MS(50),LED_REPEAT_LASTX,CMDS_RPT(2,8),LED_BLUE,WAIT_MS(50),LED_OFF,WAIT_MS(0),
                                                                            LED_END_SEQUENCE,LED_END_SEQUENCE},                            //LED_SEQUENCE_BLUEFLASH_10

    {LED_YELLOW,WAIT_MS(200),LED_OFF,WAIT_MS(200),LED_REPEAT_LASTX,CMDS_RPT(2,9),LED_YELLOW,WAIT_MS(200),LED_OFF,WAIT_MS(0),
                                                                                LED_END_SEQUENCE,LED_END_SEQUENCE},                       //LED_SEQUENCE_YELLOW_SLOWFLASH_10;

    {LED_GREEN,WAIT_MS(200),LED_OFF,WAIT_MS(200),LED_REPEAT_LASTX,CMDS_RPT(2,4),LED_GREEN,WAIT_MS(200),LED_OFF,WAIT_MS(0),
                                                                                LED_END_SEQUENCE,LED_END_SEQUENCE},                       //LED_SEQUENCE_GREEN_SLOWFLASH_5;

    {LED_GREEN,WAIT_MS(200),LED_OFF,WAIT_MS(200),LED_REPEAT_LASTX,CMDS_RPT(2,2),LED_GREEN,WAIT_MS(750),LED_OFF,WAIT_MS(0),
                                                                                                    LED_END_SEQUENCE,LED_END_SEQUENCE},   //LED_SEQUENCE_GREEN_SLOWFLASH_2_LONGGREEN;

    {LED_RED,WAIT_MS(200),LED_OFF,WAIT_MS(200),LED_REPEAT_LASTX,CMDS_RPT(2,5),LED_END_SEQUENCE,LED_END_SEQUENCE},                         //LED_SEQUENCE_RED_SLOWFLASH_5;

    {LED_RED,WAIT_MS(200),LED_OFF,WAIT_MS(200),LED_REPEAT_LASTX,CMDS_RPT(2,2),LED_RED,WAIT_MS(750),LED_OFF,WAIT_MS(0),
                                                                                                    LED_END_SEQUENCE,LED_END_SEQUENCE},   //LED_SEQUENCE_RED_SLOWFLASH_2_LONGRED;

    {LED_WHITE,WAIT_MS(200),LED_OFF,WAIT_MS(200),LED_REPEAT_LASTX,CMDS_RPT(2,2),LED_GREEN,WAIT_MS(750),LED_OFF,WAIT_MS(0),
                                                                                                    LED_END_SEQUENCE,LED_END_SEQUENCE},   //LED_SEQUENCE_WHITE_SLOWFLASH_2_LONGGREEN;

    {LED_WHITE,WAIT_MS(200),LED_OFF,WAIT_MS(200),LED_REPEAT_LASTX,CMDS_RPT(2,2),LED_RED,WAIT_MS(750),LED_OFF,WAIT_MS(0),
                                                                                                    LED_END_SEQUENCE,LED_END_SEQUENCE},   //LED_SEQUENCE_WHITE_SLOWFLASH_2_LONGRED;

    {LED_GREEN,WAIT_MS(50),LED_OFF,WAIT_MS(0),LED_END_SEQUENCE,LED_END_SEQUENCE},                                                         //LED_SEQUENCE_GREENFLASH_1

    {LED_RED,WAIT_MS(200),LED_OFF,WAIT_MS(200),LED_YELLOW,WAIT_MS(200),LED_OFF,WAIT_MS(200),LED_GREEN,WAIT_MS(750),LED_OFF,WAIT_MS(0),
                                                                                                    LED_END_SEQUENCE,LED_END_SEQUENCE},    //LED_SEQUENCE_RED_YELLOW_LONGGREEN

    {LED_BLUE,WAIT_MS(200),LED_OFF,WAIT_MS(200),LED_REPEAT_LASTX,CMDS_RPT(2,2),LED_GREEN,WAIT_MS(750),LED_OFF,WAIT_MS(0),
                                                                                                    LED_END_SEQUENCE,LED_END_SEQUENCE},   //LED_SEQUENCE_BLUE_SLOWFLASH_2_LONGGREEN;

    {LED_BLUE,WAIT_MS(200),LED_OFF,WAIT_MS(200),LED_REPEAT_LASTX,CMDS_RPT(2,2),LED_RED,WAIT_MS(750),LED_OFF,WAIT_MS(0),
                                                                                                    LED_END_SEQUENCE,LED_END_SEQUENCE}   //LED_SEQUENCE_BLUE_SLOWFLASH_2_LONGRED;

};


// ui8_sequence
#define LED_SEQUENCE_SHORT_GREEN                    0
#define LED_SEQUENCE_SHORT_RED                      1
#define LED_SEQUENCE_SHORT_BLUE                     2
#define LED_SEQUENCE_SHORT_YELLOW                   3
#define LED_SEQUENCE_BLUEFLASH_1                    4
#define LED_SEQUENCE_BLUEFLASH_2                    5
#define LED_SEQUENCE_BLUEFLASH_3                    6
#define LED_SEQUENCE_BLUEFLASH_4                    7
#define LED_SEQUENCE_BLUEFLASH_5                    8
#define LED_SEQUENCE_BLUEFLASH_6                    9
#define LED_SEQUENCE_BLUEFLASH_7                    10
#define LED_SEQUENCE_BLUEFLASH_8                    11
#define LED_SEQUENCE_BLUEFLASH_9                    12
#define LED_SEQUENCE_BLUEFLASH_10                   13
#define LED_SEQUENCE_YELLOW_SLOWFLASH_10            14
#define LED_SEQUENCE_GREEN_SLOWFLASH_5              15
#define LED_SEQUENCE_GREEN_SLOWFLASH_2_LONGGREEN    16
#define LED_SEQUENCE_RED_SLOWFLASH_5                17
#define LED_SEQUENCE_RED_SLOWFLASH_2_LONGRED        18
#define LED_SEQUENCE_WHITE_SLOWFLASH_2_LONGGREEN    19
#define LED_SEQUENCE_WHITE_SLOWFLASH_2_LONGRED      20
#define LED_SEQUENCE_GREENFLASH_1                   21
#define LED_SEQUENCE_RED_YELLOW_LONGGREEN           22
#define LED_SEQUENCE_BLUE_SLOWFLASH_2_LONGGREEN     23
#define LED_SEQUENCE_BLUE_SLOWFLASH_2_LONGRED       24

void led_clock(void);                   // Call this every LED_CLOCK_MS mS.
void led_alert(uint8_t ui8_sequence);   // call this to queue and play a sequence - e.g. led_alert(LED_SEQUENCE_SHORT_GREEN);

