#pragma once

#include <stdint.h>

void uart_init(void);
const uint8_t* uart_get_rx_buffer_rdy(void);
uint8_t* uart_get_tx_buffer(void);
void uart_send_tx_buffer(uint8_t *tx_buffer, uint8_t ui8_len);
void uart_get_rx_buffer_rdy_clear(void);
void uart_reset_rx_buffer(void);

#define UART_NUMBER_DATA_BYTES_TO_RECEIVE       256 //29 Up buffer to 256 bytes to prevent overflow/potential memory corruption if recieved packet length byte too big
#define UART_NUMBER_DATA_BYTES_TO_SEND          256 //88 

