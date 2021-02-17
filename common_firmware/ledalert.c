/*
 * LED Sequence player
 *
 * Copyright (C) 4var1/beemac, 2020.
 *
 * Released under the GPL License, Version 3
 */

#include "boards.h"
#include "ledalert.h"
#include "app_timer.h"
#include "stdint.h"
#include "nrf_delay.h"

APP_TIMER_DEF(led_timer);
#define LED_PWM_INTERVAL APP_TIMER_TICKS(1) // 16Khz - means the lowest intensity (green) is pulsing at about 1Khz.. each pulse is c.61 microseconds in length (1/16384s)
APP_TIMER_DEF(led_clock_timer);

#define LED_PWM_TABLE_LEN 8
#define LED_NOCOMMAND 255
#define LED_SEQUENCE_BUFFER_SIZE 16

uint16_t ui16_pwm_table_red[LED_PWM_TABLE_LEN] =
    {
        0b0000000000000000, // 0 pulses/mS
        0b0000000100000001, // 2 pulses/mS
        0b0100010000100010, // 4 pulses/mS
        0b1001001001010100, // 6 pulses/mS
        0b1010101010101010, // 8 pulses/mS
        0b1110101011101010, // 10 pulses/mS
        0b1011101011101111, // 12 pulses/mS
        0b1111101111101111, // 14 pulses/mS
};

uint16_t ui16_pwm_table_green[LED_PWM_TABLE_LEN] = //Make green less bright
    {
        0b0000000000000000, // 0 pulses/mS
        0b0001000000000000, // 1 pulses/mS
        0b1000000100000000, // 2 pulses/mS
        0b1000010000010000, // 3 pulses/mS
        0b1000100010001000, // 4 pulses/mS
        0b0100100100100100, // 5 pulses/mS
        0b0010100101001010, // 6 pulses/mS
        0b1010101010101010, // 7 pulses/mS
};

uint16_t ui16_pwm_table_blue[LED_PWM_TABLE_LEN] =
    {
        0b0000000000000000, // 0 pulses/mS
        0b0000010000000100, // 2 pulses/mS
        0b0100100010000100, // 4 pulses/mS
        0b0100100101010010, // 6 pulses/mS
        0b1010101010101010, // 8 pulses/mS
        0b1010101110101011, // 10 pulses/mS
        0b1110101110111110, // 12 pulses/mS
        0b1110111110111111, // 14 pulses/mS
};

uint8_t ui8_led_red_intensity;
uint8_t ui8_led_green_intensity;
uint8_t ui8_led_blue_intensity;
uint16_t ui16_pwm_mask = 32768;
uint8_t ui8_led_sequence_queue[LED_SEQUENCE_BUFFER_SIZE];
uint8_t ui8_led_sequence_queue_read_position = 0;
uint8_t ui8_led_sequence_queue_write_position = 0;
uint8_t ui8_led_sequence_current_command = 0;
uint8_t ui8_led_sequence_current_parameter = 0;
uint8_t ui8_led_sequence_current_sequence_command_index = 0;
uint8_t ui8_led_sequence_current_sequence = 0;
uint8_t ui8_led_sequence_isplaying_now = 0;
uint8_t ui8_led_sequence_repeat_counter = 0;
uint8_t ui8_led_sequence_repeat_goto_index = 0;
uint8_t ui8_led_queue_held = 0;
uint8_t ui8_led_global_brightness = 0;


#include "pins.h"


void set_led(uint8_t rgb)
{
    ui8_led_red_intensity = ((rgb & 1) * ui8_led_global_brightness);
    ui8_led_green_intensity = (((rgb & 2) >> 1) * ui8_led_global_brightness);
    ui8_led_blue_intensity = (((rgb & 4) >> 2) * ui8_led_global_brightness);

    if (rgb != 0) 
        app_timer_start(led_timer, LED_PWM_INTERVAL, NULL); 
    else 
    {
        app_timer_stop(led_timer);
        bsp_board_led_off(LED_R__PIN);
        bsp_board_led_off(LED_G__PIN);
        bsp_board_led_off(LED_B__PIN);
    }
}


void led_set_global_brightness(uint8_t ui8_global_brightness)
{
    ui8_led_global_brightness = (ui8_global_brightness & 7);
}

// Called every LED_CLOCK_MS mS (50ms)
void led_sequence_clock(void *p_context)
{
    UNUSED_PARAMETER(p_context);

    if ((!ui8_led_sequence_isplaying_now) && (ui8_led_sequence_queue_read_position != ui8_led_sequence_queue_write_position))
    {
        // If something is in the queue and we're not currently playing something - let's play a sequence
        ui8_led_sequence_isplaying_now = 1;
        ui8_led_sequence_current_sequence = ui8_led_sequence_queue[ui8_led_sequence_queue_read_position];

        //Move the read pointer forward - unless the queue is held...
        if (!ui8_led_queue_held)
            ui8_led_sequence_queue_read_position = ((ui8_led_sequence_queue_read_position+1) % LED_SEQUENCE_BUFFER_SIZE);

        ui8_led_sequence_current_sequence_command_index = 0;
        ui8_led_sequence_current_command = LED_NOCOMMAND;
    }

    if (ui8_led_sequence_isplaying_now)
    {
        // Process sequence commands
        if (ui8_led_sequence_current_command == LED_NOCOMMAND)
        {
            ui8_led_sequence_current_command = ui8_led_sequences[ui8_led_sequence_current_sequence][ui8_led_sequence_current_sequence_command_index];
            ui8_led_sequence_current_parameter = ui8_led_sequences[ui8_led_sequence_current_sequence][ui8_led_sequence_current_sequence_command_index + 1];
            ui8_led_sequence_current_sequence_command_index += 2;
        }

        if (ui8_led_sequence_current_command < 8)
        {
            // Normal set LED colour command
            set_led(ui8_led_sequence_current_command);
            if (ui8_led_sequence_current_parameter != 0)
                ui8_led_sequence_current_parameter--;
            else
            {
                ui8_led_sequence_current_command = LED_NOCOMMAND;
            }
        }
        else
        {
            // Special commands
            switch (ui8_led_sequence_current_command)
            {
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
                    if (ui8_led_sequence_current_sequence_command_index <= (2 * ((ui8_led_sequence_current_parameter & 0xf) + 1)))
                        ui8_led_sequence_repeat_goto_index = 0;
                    else
                        ui8_led_sequence_repeat_goto_index = ui8_led_sequence_current_sequence_command_index - (2 * ((ui8_led_sequence_current_parameter & 0xf) + 1));
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

static void led_pwm_timer_timeout(void *p_context)
{
    UNUSED_PARAMETER(p_context);

    // Do LED PWM - this code needs to be fast and compact - if I could read thumb I might be able to tell, but it should be :)

    ui16_pwm_mask = ui16_pwm_mask >> 1;
    if (ui16_pwm_mask == 0) ui16_pwm_mask = 32768;

    if (ui16_pwm_table_red[ui8_led_red_intensity] & ui16_pwm_mask) bsp_board_led_on(LED_R__PIN); else bsp_board_led_off(LED_R__PIN);
    if (ui16_pwm_table_green[ui8_led_green_intensity] & ui16_pwm_mask) bsp_board_led_on(LED_G__PIN); else bsp_board_led_off(LED_G__PIN);
    if (ui16_pwm_table_blue[ui8_led_blue_intensity] & ui16_pwm_mask) bsp_board_led_on(LED_B__PIN); else bsp_board_led_off(LED_B__PIN);

}

void led_init(void)
{
    ui8_led_red_intensity = 0;
    ui8_led_green_intensity = 0;
    ui8_led_blue_intensity = 0;
    ui8_led_sequence_queue_read_position = 0;
    ui8_led_sequence_queue_write_position = 0;
    led_set_global_brightness(1); // Default to lowest 'on' brightness

    ret_code_t err_code;
    err_code = app_timer_create(&led_timer, APP_TIMER_MODE_REPEATED, led_pwm_timer_timeout);
    APP_ERROR_CHECK(err_code);

    err_code = app_timer_create(&led_clock_timer, APP_TIMER_MODE_REPEATED, led_sequence_clock);
    APP_ERROR_CHECK(err_code);

    err_code = app_timer_start(led_clock_timer,APP_TIMER_TICKS(LED_CLOCK_MS), NULL); 
    APP_ERROR_CHECK(err_code);
}

void led_clear_queue(void) //used if you want to play a sequence right now.
{
    ui8_led_sequence_queue_write_position = ui8_led_sequence_queue_read_position;
}

void led_hold_queue(void) // Used to keep the current sequence playing
{
    ui8_led_queue_held = 1;
}

void led_release_queue(void) // Go back to normal - play the queue as it happens
{
    ui8_led_queue_held = 0;
}

void led_alert(uint8_t ui8_sequence)
{
    // Write the sequence to the buffer
    ui8_led_sequence_queue[ui8_led_sequence_queue_write_position] = ui8_sequence;
    // Move write pointer forward (wrapping)
    ui8_led_sequence_queue_write_position = ((ui8_led_sequence_queue_write_position+1) % LED_SEQUENCE_BUFFER_SIZE);
    // If write pointer hits read pointer - move read pointer forward (lose oldest queued sequence) - unless queue is held - as you'll probably overwrite the held sequence that's playing
    // so if the queue is held - queue overflows and new sequences are lost.
    if ((ui8_led_sequence_queue_write_position == ui8_led_sequence_queue_read_position) & !ui8_led_queue_held)
        ui8_led_sequence_queue_read_position = ((ui8_led_sequence_queue_read_position+1) % LED_SEQUENCE_BUFFER_SIZE);
}