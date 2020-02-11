#include <stdio.h>
#include <string.h>

#include "unity.h"

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
  gpio_s led_ = {};
  gpio_s switch_ = {};
  gpio__construct_as_output_IgnoreAndReturn(led_);
  gpio__construct_as_input_IgnoreAndReturn(switch_);
  switch_led_logic__initialize();
}

void test__switch_led_logic__run_once_switch_pressed(void) {
  gpio_s led_gpio = {};
  gpio_s switch_gpio = {};
  bool switch_state = false;

  gpio__get_ExpectAndReturn(switch_gpio, switch_state);
  gpio__set_Expect(led_gpio);
  switch_led_logic__run_once();
}

void test__switch_led_logic__run_once_switch_not_pressed(void) {
  gpio_s led_gpio = {};
  gpio_s switch_gpio = {};
  bool switch_state = true;

  gpio__get_ExpectAndReturn(switch_gpio, switch_state);
  gpio__reset_Expect(led_gpio);
  switch_led_logic__run_once();
}