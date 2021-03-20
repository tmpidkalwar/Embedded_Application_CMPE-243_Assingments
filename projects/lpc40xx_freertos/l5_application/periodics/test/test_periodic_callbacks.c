#include <stdio.h>
#include <string.h>

#include "unity.h"

// Include the Mocks
// - This will not pull the REAL source code of these modules (such as board_io.c)
// - This will auto-generate "Mock" versions based on the header file
#include "Mockboard_io.h"
#include "Mockgpio.h"

// Include the source we wish to test
#include "Mockcan_bus.h"
#include "Mockcan_bus_initializer.h"
#include "Mockcan_handle_message_rx.h"
#include "periodic_callbacks.h"
void setUp(void) {}

void tearDown(void) {}

void test__periodic_callbacks__initialize(void) {
  can__num_e can_num;
  can_num = can1;
  can_bus_initialize_Expect(can_num);
  periodic_callbacks__initialize();
}

void test__periodic_callbacks__10Hz(void) {
  gpio_s gpio = {};
  handle_all_can_messages_rx_Expect(can1);
  periodic_callbacks__10Hz(0);
}
