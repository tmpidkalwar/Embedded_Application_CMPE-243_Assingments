#include <stdio.h>
#include <string.h>

#include "unity.h"

// Include the Mocks
// - This will not pull the REAL source code of these modules (such as board_io.c)
// - This will auto-generate "Mock" versions based on the header file
#include "Mockboard_io.h"
#include "Mockcan_bus.h"
#include "Mockcan_bus_initializer.h"
#include "Mockgpio.h"

// Include the source we wish to test
#include "periodic_callbacks.h"

void setUp(void) {}

void tearDown(void) {}

void test__periodic_callbacks__initialize(void) {
  can_bus_initializer__can1_init_ExpectAndReturn(true);
  periodic_callbacks__initialize();
}

void test__periodic_callbacks__1Hz(void) {
  gpio_s gpio = {};
  board_io__get_led0_ExpectAndReturn(gpio);
  gpio__toggle_Expect(gpio);
  periodic_callbacks__1Hz(0);
}

void test__periodic_callbacks__100Hz_success_can_tx_with_switch_press(void) {
#ifdef CAN_AS_TX
  can__msg_t msg = {};
  can__tx_ExpectAndReturn(can1, &msg, 1, true);
  can__tx_IgnoreArg_can_message_ptr();
  get_switch2_status_ExpectAndReturn(true);
  msg.data.qword = 0x1122334455667788;
  can__tx_ReturnThruPtr_can_message_ptr(&msg);
  clear_switch2_status_Expect();
  gpio_s gpio = {};
  board_io__get_led2_ExpectAndReturn(gpio);
  gpio__toggle_Expect(gpio);

  periodic_callbacks__100Hz(0);
#endif
}

void test__periodic_callbacks__100Hz_success_can_rx_with_switch_press(void) {
#ifdef CAN_AS_RX
  can__msg_t msg = {};

  msg.data.qword = 0x1122334455667788;
  can__rx_ExpectAndReturn(can1, &msg, 1, true);
  can__rx_IgnoreArg_can_message_ptr();
  can__rx_ReturnThruPtr_can_message_ptr(&msg);
  gpio_s gpio = {};
  board_io__get_led3_ExpectAndReturn(gpio);
  gpio__toggle_Expect(gpio);
  can__rx_ExpectAndReturn(can1, &msg, 1, false);
  can__rx_IgnoreArg_can_message_ptr();

  periodic_callbacks__100Hz(0);
#endif
}

void test__periodic_callbacks__100Hz_success_can_tx_without_switch_press(void) {
#ifdef CAN_AS_TX
  can__msg_t msg = {};
  can__tx_ExpectAndReturn(can1, &msg, 1, true);
  can__tx_IgnoreArg_can_message_ptr();
  get_switch2_status_ExpectAndReturn(true);
  msg.data.qword = 0x00;
  can__tx_ReturnThruPtr_can_message_ptr(&msg);
  clear_switch2_status_Expect();
  gpio_s gpio = {};
  board_io__get_led2_ExpectAndReturn(gpio);
  gpio__toggle_Expect(gpio);

  periodic_callbacks__100Hz(0);
#endif
}

void test__periodic_callbacks__100Hz_success_can_rx_without_switch_press(void) {
#ifdef CAN_AS_RX
  can__msg_t msg = {};

  msg.data.qword = 0x00;
  can__rx_ExpectAndReturn(can1, &msg, 1, true);
  can__rx_IgnoreArg_can_message_ptr();
  can__rx_ReturnThruPtr_can_message_ptr(&msg);

  can__rx_ExpectAndReturn(can1, &msg, 1, false);
  can__rx_IgnoreArg_can_message_ptr();

  periodic_callbacks__100Hz(0);
#endif
}

void test__periodic_callbacks__100Hz_fail_can_tx(void) {
#ifdef CAN_AS_TX
  can__msg_t msg = {};
  can__tx_ExpectAndReturn(can1, &msg, 1, false);
  can__tx_IgnoreArg_can_message_ptr();
  get_switch2_status_ExpectAndReturn(false);
  periodic_callbacks__100Hz(0);
#endif
}
