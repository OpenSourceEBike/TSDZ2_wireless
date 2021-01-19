/*
 * TSDZ2 EBike wireless bootloader
 *
 * Copyright (C) rananna, 2020
 *
 * Released under the GPL License, Version 3
 */

#include <stdint.h>
#include "boards.h"
#include "bsp.h"
#include "nrf_mbr.h"
#include "nrf_bootloader.h"
#include "nrf_bootloader_app_start.h"
#include "nrf_bootloader_dfu_timers.h"
#include "nrf_dfu.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "app_error.h"
#include "app_error_weak.h"
#include "nrf_bootloader_info.h"
#include "nrfx.h"
#include "app_timer.h"
#include "nrf_drv_clock.h"
#include "nrf_delay.h"
#include "nrf_power.h"
//#include "custom_board.h"
#include "pins.h"
#include "nrf_gpio.h"
#include "nrf_drv_gpiote.h"
#include "nrf_sdh.h"
#include "boards.h"
#include "app_button.h"
#include "app_scheduler.h"
#include "nrf_drv_rtc.h"

const nrf_drv_rtc_t rtc = NRF_DRV_RTC_INSTANCE(0);

bool g_start_bootloader = false;

static void lfclk_start(void)
{
  NRF_CLOCK->LFCLKSRC = CLOCK_LFCLKSRC_SRC_Synth;
  NRF_CLOCK->EVENTS_LFCLKSTARTED = 0;
  NRF_CLOCK->TASKS_LFCLKSTART = 1;

  while (NRF_CLOCK->EVENTS_LFCLKSTARTED == 0)
  {
  }
}
static void do_reset(void)
{
  NRF_LOG_FINAL_FLUSH();

#if NRF_MODULE_ENABLED(NRF_LOG_BACKEND_RTT)
  // To allow the buffer to be flushed by the host.
  nrf_delay_ms(500);
#endif

  nrf_delay_ms(NRF_BL_RESET_DELAY_MS);

  NVIC_SystemReset();
}

static void leds_init(void)
{
  ret_code_t ret_val;
  if (LEDS_NUMBER > 0)
  {
    bsp_board_init(BSP_INIT_LEDS);
    ret_val = bsp_init(BSP_INIT_LEDS, NULL);
    APP_ERROR_CHECK(ret_val);
  }
  // turn on the led to indicate we are in the bootloader
 // bsp_board_led_on(BSP_BOARD_LED_1); //indicate that the bootloader is active
}

static void on_error(void)
{
  do_reset();
}

void app_error_handler(uint32_t error_code, uint32_t line_num, const uint8_t *p_file_name)
{
  on_error();
}

void app_error_fault_handler(uint32_t id, uint32_t pc, uint32_t info)
{
  on_error();
}

void app_error_handler_bare(uint32_t error_code)
{
  on_error();
}

/**
 * @brief Function notifies certain events in DFU process.
 */
static void dfu_observer(nrf_dfu_evt_type_t evt_type)
{
    switch (evt_type)
    {
    case NRF_DFU_EVT_DFU_FAILED:
    case NRF_DFU_EVT_DFU_ABORTED:
    case NRF_DFU_EVT_DFU_INITIALIZED:
        /*if (LEDS_NUMBER > 0)
        {
            bsp_board_led_on(BSP_BOARD_LED_0);
            if (LEDS_NUMBER <= 2)
            {
                bsp_indication_set(BSP_INDICATE_ADVERTISING_DIRECTED);
            }
            else
            {
                bsp_board_led_on(BSP_BOARD_LED_1);
                bsp_board_led_off(BSP_BOARD_LED_2);
            }
        }
        */
        break;
    case NRF_DFU_EVT_TRANSPORT_ACTIVATED:
       /* if (LEDS_NUMBER > 2)
        {
            bsp_board_led_off(BSP_BOARD_LED_0);
            bsp_board_led_off(BSP_BOARD_LED_1);
            bsp_board_led_on(BSP_BOARD_LED_2);
            bsp_indication_set(BSP_INDICATE_ADVERTISING_DIRECTED);
        }
        */
        break;
    case NRF_DFU_EVT_DFU_STARTED:
        break;
    default:
        break;
    }
}

void button_released(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
  switch (action)
  {
  case NRF_GPIOTE_POLARITY_LOTOHI:
  {
  //  bsp_board_led_off(BSP_BOARD_LED_1);
   // bsp_board_led_off(BSP_BOARD_LED_0);
  }
  break;
  case NRF_GPIOTE_POLARITY_HITOLO:
      break;
  case NRF_GPIOTE_POLARITY_TOGGLE:
      break;
  }
}

/* This function is specifically to avoid the follwing issue:
[173] GPIO: Writes to LATCH register take several CPU cycles to take effect */
static __attribute__((optimize("O0"))) bool read_pin(uint32_t pin)
{
  nrf_gpio_cfg_input(pin, GPIO_PIN_CNF_PULL_Pullup);

  static volatile uint32_t temp;
  temp = NRF_P0->IN;
  temp = NRF_P0->IN;
  temp = NRF_P0->IN;

  if (nrf_gpio_pin_read(pin))
    return true;
  else
    return false;
}

static bool check_pin_and_enable_interrupt(uint32_t pin)
{
  // enable button release interrupt only if the pin is pressed
  if (read_pin(pin) == 0)
  {
    ret_code_t err_code;
    nrf_drv_gpiote_in_config_t in_config = GPIOTE_CONFIG_IN_SENSE_LOTOHI(true);
    in_config.pull = NRF_GPIO_PIN_PULLUP;

    err_code = nrf_drv_gpiote_in_init(pin, &in_config, button_released);
    nrf_drv_gpiote_in_event_enable(pin, true);
    APP_ERROR_CHECK(err_code);

    return true;
  }

  return false;
}

static bool gpio_init(void)
{
  ret_code_t err_code;
  bool bootloader_pin_pressed = false;

  err_code = nrf_drv_gpiote_init();
  APP_ERROR_CHECK(err_code);
    
 // bootloader_pin_pressed = check_pin_and_enable_interrupt(PLUS__PIN);
 // bootloader_pin_pressed |= check_pin_and_enable_interrupt(MINUS__PIN);
  bootloader_pin_pressed = check_pin_and_enable_interrupt(ENTER__PIN);
  bootloader_pin_pressed |= check_pin_and_enable_interrupt(STANDBY__PIN);
  bootloader_pin_pressed |= check_pin_and_enable_interrupt(BUTTON_1);

  return bootloader_pin_pressed;
}

static void rtc_handler(nrf_drv_rtc_int_type_t int_type)
{
  if (int_type == NRF_DRV_RTC_INT_COMPARE0)
  {
    if (((read_pin(PLUS__PIN) == 0) &&
        (read_pin(MINUS__PIN) == 0) &&
        (read_pin(ENTER__PIN) == 0) &&
        (read_pin(STANDBY__PIN) == 0)) ||
        (read_pin(BUTTON_1) == 0))
    {
      g_start_bootloader = true;
    }
  }
}

static void lfclk_config(void)
{
  ret_code_t err_code = nrf_drv_clock_init();
  APP_ERROR_CHECK(err_code);

  nrf_drv_clock_lfclk_request(NULL);
}

static void rtc_config(void)
{
  uint32_t err_code;

  //Initialize RTC instance
  nrf_drv_rtc_config_t config = NRF_DRV_RTC_DEFAULT_CONFIG;
  config.prescaler = 4095;
  config.interrupt_priority = 7;
  err_code = nrf_drv_rtc_init(&rtc, &config, rtc_handler);
  APP_ERROR_CHECK(err_code);

  //Set compare channel to trigger interrupt after COMPARE_COUNTERTIME seconds
  err_code = nrf_drv_rtc_cc_set(&rtc, 0, 10 * 8,true);
  APP_ERROR_CHECK(err_code);

  //Power on RTC instance
  nrf_drv_rtc_enable(&rtc);
}

static void rtc_uninit(void)
{
  nrf_drv_rtc_uninit(&rtc);
}

/**@brief Function for application main entry. */
int main(void)
{
  ret_code_t ret_val;
  bool bootloader_pin_pressed = false;

  leds_init(); //turn on the red led

  // check if bootloader start flag was set before previous reset
  if (nrf_power_gpregret_get() == BOOTLOADER_DFU_START)
  {
    g_start_bootloader = true;
  }
  else
  // check for timeout or bootloader pins
  {
    // init GPIOS and init the release interrupts if applicable
    bootloader_pin_pressed = gpio_init();

    // if at least one bootloader button is pressed 
    if (bootloader_pin_pressed)
    {
      lfclk_start(); // start the low freq clock (for enter low power next)

      // start RTC timer timeout
      rtc_config();

      // will enter in low power mode and block
      // Will unblock and move forward only when timeout happens or one of the bootloader buttons are released
      __SEV();
      __WFE();
      __WFE();

      // let's increase the clock frequency for regular values to save power
      lfclk_config();

      // disable buttons
      nrf_drv_gpiote_in_uninit(PLUS__PIN);
      nrf_drv_gpiote_in_event_disable(PLUS__PIN);
      nrf_drv_gpiote_in_uninit(MINUS__PIN);
      nrf_drv_gpiote_in_event_disable(MINUS__PIN);
      nrf_drv_gpiote_in_uninit(ENTER__PIN);
      nrf_drv_gpiote_in_event_disable(ENTER__PIN);
      nrf_drv_gpiote_in_uninit(STANDBY__PIN);
      nrf_drv_gpiote_in_event_disable(STANDBY__PIN);
      nrf_drv_gpiote_in_uninit(BUTTON_1);
      nrf_drv_gpiote_in_event_disable(BUTTON_1);

      // RTC uninit because it will be used by the softdevice
      rtc_uninit();
    }
  }

  // Protect MBR and bootloader code from being overwritten.
  ret_val = nrf_bootloader_flash_protect(0, MBR_SIZE, false);
  APP_ERROR_CHECK(ret_val);
  ret_val = nrf_bootloader_flash_protect(BOOTLOADER_START_ADDR, BOOTLOADER_SIZE, false);
  APP_ERROR_CHECK(ret_val);

  ret_val = app_timer_init();
  APP_ERROR_CHECK(ret_val);

  ret_val = nrf_bootloader_init(dfu_observer);
  APP_ERROR_CHECK(ret_val);
  //if the program is here there was either
  //-no DFU requested in the bootloader
  // or the DFU module detected no ongoing DFU operation and found a valid main application.
  //so, load the installed application
  //bsp_board_led_off(BSP_BOARD_LED_1);
 // bsp_board_led_off(BSP_BOARD_LED_0);
  nrf_bootloader_app_start();
}
