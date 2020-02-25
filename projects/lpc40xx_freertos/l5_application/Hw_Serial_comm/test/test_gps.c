#include "unity.h"

// Mocks
#include "Mockclock.h"
#include "Mockuart.h"

#include "Mockqueue.h"

// Use the real implementation (not mocks) for:
#include "line_buffer.h"

// Include the source we wish to test
#include "gps.c"
#include <stdio.h>

void test_gps_init(void) {

  clock__get_peripheral_clock_hz_ExpectAndReturn(100);
  uart__init_ExpectAnyArgs();

  QueueHandle_t rxq_handle, txq_handle;
  xQueueCreate_ExpectAndReturn(50, sizeof(char), rxq_handle);
  xQueueCreate_ExpectAndReturn(8, sizeof(char), txq_handle);

  uart__enable_queues_ExpectAndReturn(gps_uart, rxq_handle, txq_handle, true);
  gps__init();
}
void test_gps__absorb_data(void) {
  uart__get_ExpectAndReturn(gps_uart, NULL, 0, true);
  uart__get_IgnoreArg_input_byte();
  uart__get_ExpectAndReturn(gps_uart, NULL, 0, false);
  uart__get_IgnoreArg_input_byte();
  gps__absorb_data();
}
void test_gps_handle_line(void) {

  line_buffer__init(&line, line_buffer, sizeof(line_buffer));

  char gps_string[sizeof(line_buffer)] = {
      "$GPGGA,181908.00,3404.7041778,N,07044.3966270,W,4,13,1.00,495.144,M,29.200,M,0.10,0000*40"};
  for (int i = 0; i < 80; i++)
    line_buffer__add_byte(&line, gps_string[i]);
  line_buffer__add_byte(&line, '\n');

  gps__handle_line();

  TEST_ASSERT_EQUAL_STRING(latitude, "3404.7041778");
  TEST_ASSERT_EQUAL_STRING(longitude, "07044.3966270");
  TEST_ASSERT_EQUAL_FLOAT(parsed_coordinates.latitude, 3404.7041778);
  TEST_ASSERT_EQUAL_FLOAT(parsed_coordinates.longitude, 07044.3966270);
}
void test_gps__get_coordinates(void) {
  gps_coordinates_t get_coordinates;
  get_coordinates = gps__get_coordinates();
  TEST_ASSERT_EQUAL_FLOAT(get_coordinates.latitude, 3404.7041778);
  TEST_ASSERT_EQUAL_FLOAT(get_coordinates.longitude, 07044.3966270);
}
