#include <stdio.h>
#include <string.h>

#include "unity.h"

// Include the Mocks
// - This will not pull the REAL source code of these modules (such as board_io.c)
// - This will auto-generate "Mock" versions based on the header file
#include "Mockboard_io.h"
#include "Mockgpio.h"
#include "Mockswitch_led_logic.h"

// Include the source we wish to test
#include "periodic_callbacks.h"

void setUp(void) {}

void tearDown(void) {}

void test__periodic_callbacks__initialize(void) {
  switch_led_logic__initialize_Expect();
  periodic_callbacks__initialize();
}

gpio_s gpio = {};

void test__periodic_callbacks__1Hz(void) {
  // board_io__get_led0_ExpectAndReturn(gpio);
  // gpio__toggle_Expect(gpio);

  switch_led_logic__run_once_Expect();
  periodic_callbacks__1Hz(0);
}

void test__periodic_callbacks__10Hz(void) {
  board_io__get_led1_ExpectAndReturn(gpio);
  gpio__toggle_Expect(gpio);

  periodic_callbacks__10Hz(0);
}

void test__periodic_callbacks__100Hz(void) {
  board_io__get_led2_ExpectAndReturn(gpio);
  gpio__toggle_Expect(gpio);

  periodic_callbacks__100Hz(0);
}

void test__periodic_callbacks__1000Hz(void) {
  board_io__get_led3_ExpectAndReturn(gpio);
  gpio__toggle_Expect(gpio);

  periodic_callbacks__1000Hz(0);
}