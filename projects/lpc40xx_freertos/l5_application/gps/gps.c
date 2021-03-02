// @file gps.c
#include "gps.h"

// Our 'private' modules: We hide and abstract away these details from the user
// Whoever #includes "Mockgps.h" will not need to deal with these because
// these are included in this source file rather than the header file
#include "line_buffer.h"
#include "uart.h"

// Rtos dependency for the UART driver
#include "FreeRTOS.h"
#include "queue.h"

#include "clock.h" // needed for UART initialization

// Change this according to which UART you plan to use
static const uart_e gps_uart = UART__2;

// Space for the line buffer, and the line buffer data structure instance
static char line_buffer[200];
static line_buffer_s line;

static gps_coordinates_t parsed_coordinates;

static int isDigit(char c) {
  if ((c >= '0') && (c <= '9'))
    return 1;
  return 0;
}

static float strtof(char *str) {
  float res = 0.0F;
  bool afterDecimalPoint = false;
  float div = 1; // Divider to place digits after the deciaml point

  while (*str != '\0') {
    if (isDigit(*str)) {
      // Integer part
      if (!afterDecimalPoint) {
        res *= 10;         // Shift the previous digits to the left
        res += *str - '0'; // Add the new one
      }
      // Decimal part
      else {
        div *= 10;
        res += (float)(*str - '0') / div;
      }
    } else if (*str == '.') {
      afterDecimalPoint = true;
    } else {
      break;
    }

    str++;
  }

  return res;
}

static void gps__transfer_data_from_uart_driver_to_line_buffer(void) {
  char byte;
  const uint32_t zero_timeout = 0;

  while (uart__get(gps_uart, &byte, zero_timeout)) {
    line_buffer__add_byte(&line, byte);
  }
}

static void gps__parse_GPGGA_line(char *gps_line) {
  char *parsed_param = strtok(gps_line, ",");
  char *gps_str_identifier = "$GPGGA";
  if (*parsed_param == *gps_str_identifier) {
    parsed_param = strtok(NULL, ",");
    parsed_param = strtok(NULL, ",");
    parsed_coordinates.latitude = strtof(parsed_param);
    parsed_param = strtok(NULL, ",");
    parsed_param = strtok(NULL, ",");
    parsed_coordinates.longitude = strtof(parsed_param);
  }
}

static void gps__parse_coordinates_from_line(void) {
  char gps_line[120];
  if (line_buffer__remove_line(&line, gps_line, sizeof(gps_line))) {

    gps__parse_GPGGA_line(gps_line);
  } else {
    parsed_coordinates.latitude = 0.0;
    parsed_coordinates.longitude = 0.0;
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
  gps__transfer_data_from_uart_driver_to_line_buffer();
  gps__parse_coordinates_from_line();
}

gps_coordinates_t gps__get_coordinates(void) { return parsed_coordinates; }