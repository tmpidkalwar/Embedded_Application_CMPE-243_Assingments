// gps.c
#include "gps.h"
#include <stdlib.h>

// GPS module dependency
#include "line_buffer.h"
#include "uart.h"

// Rtos dependency for the UART driver
#include "FreeRTOS.h"
#include "queue.h"

#include "clock.h" // needed for UART initialization

// Change this according to which UART you plan to use
static uart_e gps_uart = UART__3;

// Space for the line buffer, and the line buffer data structure instance
static char line_buffer[256];
static line_buffer_s line;

static gps_coordinates_t parsed_coordinates;
static char latitude[15];
static char longitude[15];
static float latitude_float;
static float longitude_float;
static void gps__absorb_data(void) {
  char byte;
  while (uart__get(gps_uart, &byte, 0)) {
    line_buffer__add_byte(&line, byte);
  }
}

static void gps__handle_line(void) {
  char gps_line[100];
  int i;

  if (line_buffer__remove_line(&line, gps_line, sizeof(gps_line))) {
    for (i = 0; i < 12; i++) {
      latitude[i] = gps_line[i + 17];
    }
    latitude[12] = '\0';

    latitude_float = atof(latitude);
    parsed_coordinates.latitude = latitude_float;

    // TODO: Parse the line to store GPS coordinates etc.
    // TODO: parse and store to parsed_coordinates

    for (i = 0; i < 13; i++) {
      longitude[i] = gps_line[i + 32];
    }
    longitude[13] = '\0';

    longitude_float = atof(longitude);
    parsed_coordinates.longitude = longitude_float;
  }
}

void gps__init(void) {
  line_buffer__init(&line, line_buffer, sizeof(line_buffer));
  uart__init(gps_uart, clock__get_peripheral_clock_hz(), 38400);

  // RX queue should be sized such that can buffer data in UART driver until gps__run_once() is called
  // Note: Assuming 38400bps, we can get 4 chars per ms, and 40 chars per 10ms (100Hz)
  QueueHandle_t rxq_handle = xQueueCreate(50, sizeof(char));
  QueueHandle_t txq_handle = xQueueCreate(8, sizeof(char)); // We don't send anything to the GPS
  uart__enable_queues(gps_uart, rxq_handle, txq_handle);
}

/// Public functions:
///
void gps__run_once(void) {
  gps__absorb_data();
  gps__handle_line();
}

gps_coordinates_t gps__get_coordinates(void) { return parsed_coordinates; }