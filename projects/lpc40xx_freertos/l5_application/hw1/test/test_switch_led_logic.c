
#include "unity.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
// Include the Mocks
// - This will not pull the REAL source code of these modules (such as board_io.c)
// - This will auto-generate "Mock" versions based on the header file
#include "Mockboard_io.h"
#include "Mockgpio.h"

// Include the source we wish to test
#include "switch_led_logic.h"

void setUp(void) {}

void tearDown(void) {}

void test__switch_led_logic__initialize(void) {
  gpio_s gpio = {};
  gpio__construct_as_output_ExpectAndReturn(GPIO__PORT_2, 0, gpio);
  gpio__construct_as_input_ExpectAndReturn(GPIO__PORT_2, 1, gpio);
  switch_led_logic__initialize();
}

void test__switch_led_logic__run_once(void) {
  gpio_s gpio = {};
  bool val = 0;
  gpio__get_ExpectAndReturn(gpio, val);
  gpio__reset_Expect(gpio);
  switch_led_logic__run_once();

  val = 1;
  gpio__get_ExpectAndReturn(gpio, val);
  gpio__set_Expect(gpio);
  switch_led_logic__run_once();
}