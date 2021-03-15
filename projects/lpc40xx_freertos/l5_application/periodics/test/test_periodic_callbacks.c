#include <stdio.h>
#include <string.h>

#include "unity.h"

// Include the Mocks
// - This will not pull the REAL source code of these modules (such as board_io.c)
// - This will auto-generate "Mock" versions based on the header file
#include "Mockacceleration.h"
#include "Mockboard_io.h"
#include "Mockcan_bus.h"
#include "Mockcan_handler.h"
#include "Mockgpio.h"

// Include the source we wish to test
#include "periodic_callbacks.h"

void setUp(void) {}

void tearDown(void) {}

void test__periodic_callbacks__initialize(void) {
  can__init_ExpectAndReturn(can1, 100, 100, 100, NULL, NULL, true);
  can__init_IgnoreArg_bus_off_cb();
  can__init_IgnoreArg_data_ovr_cb();
  acceleration__init_ExpectAndReturn(true);
  can__bypass_filter_accept_all_msgs_Expect();
  can__reset_bus_Expect(can1);
  periodic_callbacks__initialize();
}

void test__periodic_callbacks__1Hz(void) {
  gpio_s gpio = {};
  board_io__get_led0_ExpectAndReturn(gpio);
  gpio__toggle_Expect(gpio);
  periodic_callbacks__1Hz(0);
}

// void test__periodic_callbacks__10Hz(void) {
//   gpio_s gpio = {};
//   can_handler__manage_mia_10hz_Expect();
//   board_io__get_led1_ExpectAndReturn(gpio);
//   gpio__toggle_Expect(gpio);
//   periodic_callbacks__10Hz(0);
// }