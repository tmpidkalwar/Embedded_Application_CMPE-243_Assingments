// @file: fake_gps.c
#include "fake_gps.h" // TODO: You need to create this module, unit-tests for this are optional

#include "uart.h"
#include "uart_printf.h"

#include "clock.h" // needed for UART initialization

// Change this according to which UART you plan to use
static uart_e gps_uart = UART__1;

void fake_gps__init(void) {
  uart__init(gps_uart, clock__get_peripheral_clock_hz(), 38400);

  QueueHandle_t rxq_handle = xQueueCreate(4, sizeof(char));   // Nothing to receive
  QueueHandle_t txq_handle = xQueueCreate(100, sizeof(char)); // We send a lot of data
  uart__enable_queues(gps_uart, rxq_handle, txq_handle);
}

/// Send a fake GPS string
/// TODO: You may want to be somewhat random about the coordinates that you send here
void fake_gps__run_once(void) {
  static float longitude = 0;
  uart_printf(gps_uart, "$GPGGA,230612.015,%4.4f,N,12102.4634,W,0,04,5.7,508.3,M,,,,0000*13\r\n", longitude);
  longitude += 1.15; // random incrementing value
}