#include "unity.h"

#include "Mockboard_io.h"
#include "Mockcan_bus.h"
#include "Mockdriver_logic.h"
#include "Mockgpio.h"

#include "can_handle_message_tx.h"
#include "project.h"

void setup(void) {}

void teardown(void) {}

void test_success_can_msg_tx(void) {
  dbc_DRIVER_TO_MOTOR_s motor_data;
  driver_logic__get_motor_command_ExpectAndReturn(motor_data);
  gpio_s gpio;
  board_io__get_led0_ExpectAndReturn(gpio);
  gpio__toggle_Expect(gpio);
  can__msg_t msg = {};
  can__tx_ExpectAndReturn(can1, &msg, 0, true);
  can__tx_IgnoreArg_can_message_ptr();
  handle_all_can_messages_tx(can1);
}

void test_failed_can_msg_tx(void) {
  dbc_DRIVER_TO_MOTOR_s motor_data;
  driver_logic__get_motor_command_ExpectAndReturn(motor_data);
  gpio_s gpio;
  board_io__get_led1_ExpectAndReturn(gpio);
  gpio__toggle_Expect(gpio);
  can__msg_t msg = {};
  can__tx_ExpectAndReturn(can1, &msg, 0, false);
  can__tx_IgnoreArg_can_message_ptr();
  handle_all_can_messages_tx(can1);
}