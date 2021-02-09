/*
 * LED Sequence player
 *
 * Copyright (C) 4var1/beemac, 2020.
 *
 * Released under the GPL License, Version 3
 */

#include "boards.h"
#include "ledalert.h"

#define LED_NOCOMMAND                           255

#if defined(BOARD_PCA10059)
#include "pins.h"
void set_led(uint8_t rgb)
{
    if (rgb & 1) bsp_board_led_on(LED_R__PIN); else bsp_board_led_off(LED_R__PIN);
    if (rgb & 2) bsp_board_led_on(LED_G__PIN); else bsp_board_led_off(LED_G__PIN);
    if (rgb & 4) bsp_board_led_on(LED_B__PIN); else bsp_board_led_off(LED_B__PIN);
}
#endif

#define LED_SEQUENCE_BUFFER_SIZE                16

volatile uint8_t ui8_led_sequence_queue[LED_SEQUENCE_BUFFER_SIZE];
volatile uint8_t ui8_led_sequence_queue_read_position = 0;
volatile uint8_t ui8_led_sequence_queue_write_position = 0;
volatile uint8_t ui8_led_sequence_current_command = 0;
volatile uint8_t ui8_led_sequence_current_parameter = 0;
volatile uint8_t ui8_led_sequence_current_sequence_command_index = 0;
volatile uint8_t ui8_led_sequence_current_sequence = 0;
volatile uint8_t ui8_led_sequence_isplaying_now = 0;
volatile uint8_t ui8_led_sequence_repeat_counter = 0;
volatile uint8_t ui8_led_sequence_repeat_goto_index = 0;

void led_alert(uint8_t ui8_sequence)
{
    // Write the sequence to the buffer
    ui8_led_sequence_queue[ui8_led_sequence_queue_write_position] = ui8_sequence;
    // Move write pointer forward (wrapping)
    ui8_led_sequence_queue_write_position=((++ui8_led_sequence_queue_write_position)% LED_SEQUENCE_BUFFER_SIZE);
    // If write pointer hits read pointer - move read pointer forward (lose oldest queued sequence)
    if (ui8_led_sequence_queue_write_position==ui8_led_sequence_queue_read_position) ui8_led_sequence_queue_read_position=((++ui8_led_sequence_queue_read_position) % LED_SEQUENCE_BUFFER_SIZE);
}

// Called every LED_CLOCK_MS mS (50ms)
void led_clock(void)
{
    if ((!ui8_led_sequence_isplaying_now) && (ui8_led_sequence_queue_read_position != ui8_led_sequence_queue_write_position))
    {
        // If something is in the queue and we're not currently playing something - let's play a sequence
        ui8_led_sequence_isplaying_now = 1;
        ui8_led_sequence_current_sequence = ui8_led_sequence_queue[ui8_led_sequence_queue_read_position];
        ui8_led_sequence_queue_read_position=((++ui8_led_sequence_queue_read_position)% LED_SEQUENCE_BUFFER_SIZE);
        ui8_led_sequence_current_sequence_command_index = 0;
        ui8_led_sequence_current_command = LED_NOCOMMAND;
    }

    if (ui8_led_sequence_isplaying_now)
    {
        // Process sequence commands
        if (ui8_led_sequence_current_command == LED_NOCOMMAND)
        {
            ui8_led_sequence_current_command = ui8_led_sequences[ui8_led_sequence_current_sequence][ui8_led_sequence_current_sequence_command_index];
            ui8_led_sequence_current_parameter = ui8_led_sequences[ui8_led_sequence_current_sequence][ui8_led_sequence_current_sequence_command_index+1];
            ui8_led_sequence_current_sequence_command_index+=2;
        }
        
        if (ui8_led_sequence_current_command<8)
        {
            // Normal set LED colour command
            set_led(ui8_led_sequence_current_command);
            if (ui8_led_sequence_current_parameter != 0) ui8_led_sequence_current_parameter--;
            else
            {
                ui8_led_sequence_current_command = LED_NOCOMMAND;
            }
        }
        else
        {
            // Special commands
            switch (ui8_led_sequence_current_command) {
                case LED_END_SEQUENCE:
                // Last command in sequence - reset
                ui8_led_sequence_isplaying_now = 0;
                ui8_led_sequence_current_sequence_command_index = 0;
                ui8_led_sequence_current_command = LED_NOCOMMAND;
                break;

                case LED_REPEAT_LASTX:
                // Repeat last x commands
                if (ui8_led_sequence_repeat_counter == 0)
                {
                    ui8_led_sequence_repeat_counter = (ui8_led_sequence_current_parameter >> 4);
                    if (ui8_led_sequence_current_sequence_command_index <= (2 *((ui8_led_sequence_current_parameter & 0xf)+1)))
                        ui8_led_sequence_repeat_goto_index = 0;
                    else
                        ui8_led_sequence_repeat_goto_index = ui8_led_sequence_current_sequence_command_index - (2 *((ui8_led_sequence_current_parameter & 0xf)+1));
                }

                ui8_led_sequence_repeat_counter--;
                
                if (ui8_led_sequence_repeat_counter == 0) 
                {
                    ui8_led_sequence_repeat_counter = 0;
                    ui8_led_sequence_repeat_goto_index = 0;
                    ui8_led_sequence_current_command = LED_NOCOMMAND;
                }
                else
                {
                    ui8_led_sequence_current_sequence_command_index = ui8_led_sequence_repeat_goto_index;
                    ui8_led_sequence_current_command = LED_NOCOMMAND;
                }
                break;

            }
        } 
    }
}

