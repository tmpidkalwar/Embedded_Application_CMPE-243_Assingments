#include <stdio.h>
#include <string.h>

#include "unity.h"

// Include the Mocks
#include "Mockboard_io.h"
#include "Mockgpio.h"

// Include the source we wish to test
#include "switch_led_logic.h"

void setUp(void) {}

void tearDown(void) {}

gpio_s led, sw, extra_arg;

void test__switch_led_logic__initialize() {
  gpio__construct_as_output_ExpectAndReturn(GPIO__PORT_1, 18, extra_arg);
  gpio__construct_as_input_ExpectAndReturn(GPIO__PORT_0, 29, extra_arg);
  switch_led_logic__initialize();
}

void test__switch_led_logic__run_once() {
  bool extra;
  gpio__get_ExpectAndReturn(sw, extra);

  bool sw_val = false;
  if (sw_val) {
    gpio__set_Expect(led);
  } else {
    gpio__reset_Expect(led);
  }

  switch_led_logic__run_once();
}