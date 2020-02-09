#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "Mockboard_io.h"
#include "Mockgpio.h"
#include "unity.h"

// Include the source we wish to test
#include "switch_logic.h"

void setUp(void) { puts("Before Test"); }

void tearDown(void) { puts("After Test"); }

static gpio_s test_led, test_switch_button;

void test__switch_led_logic__initialize(void) {
  board_io__get_led1_ExpectAndReturn(test_led);
  gpio__construct_as_input_ExpectAndReturn(GPIO__PORT_0, 30, test_switch_button);
  switch_led_logic__initialize();
}

void test__switch_led_logic__run_once(void) {
  bool switch_button_status;
  gpio__get_ExpectAndReturn(test_switch_button, switch_button_status);

  if (switch_button_status) {
    gpio__set_Expect(test_led);
  } else {
    gpio__reset_Expect(test_led);
  }
  switch_led_logic__run_once();
}