/**
 * @file test_gps.c
 * @brief This file includes all unit test case implementation testing all APIs of 
 *      gps.c file. 
 * 
 */
#include "unity.h"

// Mocks
#include "Mockclock.h"
#include "Mockgpio.h"
#include "Mockuart.h"
#include "Mockqueue.h"

// Use the real implementation (not mocks) for:
#include "line_buffer.h"

// Include the source we wish to test
#include "gps.c"

const static uint8_t GPGGA_latitude_index = 3;
const static uint8_t GPGGA_longitutde_index = 5;

void setUp(void) {}
void tearDown(void) {}

void test_init(void) {
  uint32_t clock_value;
  clock__get_peripheral_clock_hz_ExpectAndReturn(clock_value);
  uart__init_Expect(UART__2, clock_value, 38400);
  gpio_s uart_tx_gpio = {GPIO__PORT_0, 10};
  gpio_s uart_rx_gpio = {GPIO__PORT_0, 11};
  gpio__construct_with_function_ExpectAndReturn(GPIO__PORT_0, 10, GPIO__FUNCTION_1, uart_tx_gpio);
  gpio__construct_with_function_ExpectAndReturn(GPIO__PORT_0, 11, GPIO__FUNCTION_1, uart_rx_gpio);
  QueueHandle_t rx_queue_handle;
  xQueueCreate_ExpectAndReturn(100, sizeof(char), rx_queue_handle);
  QueueHandle_t tx_queue_handle;
  xQueueCreate_ExpectAndReturn(8, sizeof(char), tx_queue_handle);
  uint32_t retVal = true;
  uart__enable_queues_ExpectAndReturn(UART__2, rx_queue_handle, tx_queue_handle, retVal);
  gps__init();
}

void test_GPGLL_line_is_ignored(void) {
  char gps_line[] = "$GPGLL,1111.11,a,2222.22,a,hhmmss.ss,A\r\n";
  char *ptr;
  for (size_t index = 0; index < strlen(gps_line) - 1; index++) {
    char the_char_to_return = gps_line[index];
    bool last_char = true;
    uart__get_ExpectAndReturn(UART__2, ptr, 0, last_char);
    uart__get_IgnoreArg_input_byte();
    uart__get_ReturnThruPtr_input_byte(&the_char_to_return);

    last_char = false;
    uart__get_ExpectAndReturn(UART__2, ptr, 0, last_char);
    uart__get_IgnoreArg_input_byte();

    gps__run_once();
  }
  char the_char_to_return = '\n';
  bool last_char = true;
  uart__get_ExpectAndReturn(UART__2, ptr, 0, last_char);
  uart__get_IgnoreArg_input_byte();
  uart__get_ReturnThruPtr_input_byte(&the_char_to_return);

  last_char = false;
  uart__get_ExpectAndReturn(UART__2, ptr, 0, last_char);
  uart__get_IgnoreArg_input_byte();

  gps__run_once();

  gps_coordinates_t got_gps_coordinate = gps__get_coordinates();

  // printf("coordinates got %f %f", got_gps_coordinate.latitude, got_gps_coordinate.longitude);
  TEST_ASSERT_EQUAL_FLOAT((float)0.0, got_gps_coordinate.latitude);
  TEST_ASSERT_EQUAL_FLOAT((float)0.0, got_gps_coordinate.longitude);
}

void test_GPGGA_coordinates_are_parsed(void) {
  const char *uart_driver_returned_data = "$GPGGA,hhmmss.ss,llll.ll,a,yyyyy.yy,a,x,xx,x.x,x.x,M,x.x,M,x.x,xxxx*hh\r\n";

  char gps_line[] = "$GPGGA,hhmmss.ss,1111.11,a,2222.22,a,x,xx,x.x,x.x,M,x.x,M,x.x,xxxx*hh\r\n";
  char *ptr;
  for (size_t index = 0; index < strlen(gps_line) - 1; index++) {

    char the_char_to_return = gps_line[index];
    bool last_char = true;
    uart__get_ExpectAndReturn(UART__2, ptr, 0, last_char);
    uart__get_IgnoreArg_input_byte();
    uart__get_ReturnThruPtr_input_byte(&the_char_to_return);

    last_char = false;
    uart__get_ExpectAndReturn(UART__2, ptr, 0, last_char);
    uart__get_IgnoreArg_input_byte();

    gps__run_once();
  }
  char the_char_to_return = '\n';
  bool last_char = true;
  uart__get_ExpectAndReturn(UART__2, ptr, 0, last_char);
  uart__get_IgnoreArg_input_byte();
  uart__get_ReturnThruPtr_input_byte(&the_char_to_return);

  last_char = false;
  uart__get_ExpectAndReturn(UART__2, ptr, 0, last_char);
  uart__get_IgnoreArg_input_byte();

  gps__run_once();

  gps_coordinates_t got_gps_coordinate = gps__get_coordinates();

  printf("coordinates got %f %f", got_gps_coordinate.latitude, got_gps_coordinate.longitude);
  TEST_ASSERT_EQUAL_FLOAT((float)1111.11, got_gps_coordinate.latitude);
  TEST_ASSERT_EQUAL_FLOAT((float)2222.22, got_gps_coordinate.longitude);
}

void test_GPGGA_incomplete_line(void) {
  char gps_line[] = "$GPGGA,hhmmss.ss,1111.22,a,2222.11,a,x,xx,x.x,x.x,M,x.x,M,x.x";
  char *ptr;
  size_t index;
  for (index = 0; index < strlen(gps_line); index++) {
    char the_char_to_return = gps_line[index];
    bool last_char = true;
    uart__get_ExpectAndReturn(UART__2, ptr, 0, last_char);
    uart__get_IgnoreArg_input_byte();
    uart__get_ReturnThruPtr_input_byte(&the_char_to_return);

    last_char = false;
    uart__get_ExpectAndReturn(UART__2, ptr, 0, last_char);
    uart__get_IgnoreArg_input_byte();

    gps__run_once();
  }

  gps_coordinates_t got_gps_coordinate = gps__get_coordinates();

  TEST_ASSERT_NOT_EQUAL((float)1111.22, got_gps_coordinate.latitude);
  TEST_ASSERT_NOT_EQUAL((float)2222.11, got_gps_coordinate.longitude);
}

void test_gps_line_parsing_while_buffer_overflow(void) {
  test_init();
  // Fill buffer to 180 element without '\n' character to simulate buffer overflow
  char gps_line[] = "$GPGGA,hhmmss.ss,1111.11,a,2222.22,a,x,xx,x.x,x.x,M,x.x,M,xx$GPGGA,hhmmss.ss,1111.11,a,2222.22,a,"
                    "x,xx,x.x,x.x,M,x.x,M,xx$GPGGA,hhmmss.ss,1111.11,a,2222.22,a,x,xx,x.x,x.x,M,x.x,M,xx";
  int index = 0;
  while (index < strlen(gps_line)) {
    line_buffer__add_byte(&line, gps_line[index]);
    index++;
  }

  printf("write_index is %ld\n", line.element_count);
  // Now when we start filling buffer again, it will reach max limit and vacate the data to line buffer
  // That line bufffer should get parsed to have updated coordinates
  char gps_line1[] = "$GPGGA,hhmmss.ss,1111.11,a,2222.22,a,x,xx,x.x,x.x,M";
  char *ptr;
  for (index = 0; index < strlen(gps_line1); index++) {
    char the_char_to_return = gps_line1[index];
    bool last_char = true;
    uart__get_ExpectAndReturn(UART__2, ptr, 0, last_char);
    uart__get_IgnoreArg_input_byte();
    uart__get_ReturnThruPtr_input_byte(&the_char_to_return);

    last_char = false;
    uart__get_ExpectAndReturn(UART__2, ptr, 0, last_char);
    uart__get_IgnoreArg_input_byte();

    gps__run_once();
    printf("write_index is %ld\n", line.element_count);
  }
  gps_coordinates_t got_gps_coordinate = gps__get_coordinates();

  // printf("coordinates got %f %f", got_gps_coordinate.latitude, got_gps_coordinate.longitude);
  TEST_ASSERT_EQUAL_FLOAT((float)1111.11, got_gps_coordinate.latitude);
}