/**
 * @file fake_gps.c
 * @brief This file is responsible for simulating GPS module, which send GPS string over UART.
 */

#include "fake_gps.h"

#include "stdio.h"

#include "uart.h"
#include "uart_printf.h"

// Rtos dependency for the UART driver
#include "FreeRTOS.h"
#include "queue.h"

#include "clock.h" // needed for UART initialization

#include "gpio.h"

static uart_e fake_gps_uart = UART__1;

void fake_gps__init(void) {
  uart__init(fake_gps_uart, clock__get_peripheral_clock_hz(), 38400);

  const uint8_t gps_tx_pin = 15;
  const uint8_t gps_rx_pin = 16;

  gpio__construct_with_function(GPIO__PORT_0, gps_tx_pin, GPIO__FUNCTION_1);
  gpio__construct_with_function(GPIO__PORT_0, gps_rx_pin, GPIO__FUNCTION_1);

  QueueHandle_t rxq_handle = xQueueCreate(4, sizeof(char));   // Nothing to receive
  QueueHandle_t txq_handle = xQueueCreate(100, sizeof(char)); // We send a lot of data
  uart__enable_queues(fake_gps_uart, rxq_handle, txq_handle);
}

void fake_gps__run_once(void) {
  static float longitude = 0;
  uart_printf(fake_gps_uart, "$GPGGA,230612.015,%4.4f,N,12102.4634,W,0,04,5.7,508.3,M,x.x,M,x.x,0000*13\r\n",
              longitude);

  // printf("sending GPS coordinates \n");

  longitude += 1.15; // random incrementing value
}