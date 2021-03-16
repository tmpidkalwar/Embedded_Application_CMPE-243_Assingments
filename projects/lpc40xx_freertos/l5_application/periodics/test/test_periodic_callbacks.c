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

void test__periodic_callbacks__1Hz(void) { periodic_callbacks__1Hz(0); }

void test__periodic_callbacks__10Hz_can_receive_mode(void) {
#ifdef CAN_RX
  // can_handler__manage_mia_10hz_Expect();
  can_handler__handle_all_incoming_messages_Expect();
  can_handler__manage_mia_10hz_Expect();
  periodic_callbacks__10Hz(0);
#endif
}

void test__periodic_callbacks__10Hz_can_transmit_mode(void) {
#ifdef CAN_TX
  can_handler__transmit_message_10hz_Expect();
  periodic_callbacks__10Hz(0);
#endif
}