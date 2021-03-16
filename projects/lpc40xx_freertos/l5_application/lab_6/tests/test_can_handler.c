#include "unity.h"

#include "Mockacceleration.h"
#include "Mockboard_io.h"
#include "Mockcan_bus.h"
#include "Mockcan_mia_configuration.h"
#include "Mockgpio.h"

#include "can_handler.h"
#include "project.h"

void setup(void){};
void tearDown(void){};

void test_can_msg_sensor_accmtr_mia_occured(void) {
  dbc_SENSOR_ACCMTR_s accmtr_recv_values;
  gpio_s gpio = {GPIO__PORT_0, 17};
  gpio__construct_as_output_ExpectAndReturn(GPIO__PORT_0, 17, gpio);
  can_mia__service_mia_SENSOR_ACCMTR_ExpectAndReturn(&accmtr_recv_values, 100, true);
  can_mia__service_mia_SENSOR_ACCMTR_IgnoreArg_can_msg_sensor_accmtr();
  gpio__set_Expect(gpio);
  can_handler__manage_mia_10hz();
}

void test_can_msg_sensor_accmtr_mia_not_occured(void) {
  dbc_SENSOR_ACCMTR_s accmtr_recv_values;
  gpio_s gpio = {GPIO__PORT_0, 17};
  gpio__construct_as_output_ExpectAndReturn(GPIO__PORT_0, 17, gpio);
  can_mia__service_mia_SENSOR_ACCMTR_ExpectAndReturn(&accmtr_recv_values, 100, false);
  can_mia__service_mia_SENSOR_ACCMTR_IgnoreArg_can_msg_sensor_accmtr();
  gpio__reset_Expect(gpio);
  can_handler__manage_mia_10hz();
}

void test_handle_sensor_accmtr_incoming_messages_decode_success_led0_on(void) {
  can__msg_t can_msg = {};
  can_msg.msg_id = dbc_header_SENSOR_ACCMTR.message_id;
  can_msg.frame_fields.data_len = dbc_header_SENSOR_ACCMTR.message_dlc;
  can__rx_ExpectAndReturn(can1, &can_msg, 0, true);
  can__rx_IgnoreArg_can_message_ptr();
  can__rx_ReturnThruPtr_can_message_ptr(&can_msg);
  dbc_SENSOR_ACCMTR_s accmtr_recv_values;
  accmtr_recv_values.SENSOR_ACCMTR_x = (int32_t)(-700);

  const dbc_message_header_t header = {
      .message_id = can_msg.msg_id,
      .message_dlc = can_msg.frame_fields.data_len,
  };
  gpio_s gpio = {};
  TEST_ASSERT_EQUAL(true, dbc_decode_SENSOR_ACCMTR(&accmtr_recv_values, header, can_msg.data.bytes));
  board_io__get_led0_ExpectAndReturn(gpio);
  gpio__set_Expect(gpio);
  board_io__get_led1_ExpectAndReturn(gpio);
  gpio__set_Expect(gpio);
  board_io__get_led2_ExpectAndReturn(gpio);
  gpio__set_Expect(gpio);
  board_io__get_led3_ExpectAndReturn(gpio);
  gpio__set_Expect(gpio);
  board_io__get_led0_ExpectAndReturn(gpio);
  gpio__reset_Expect(gpio);
  can__rx_ExpectAndReturn(can1, &can_msg, 0, false);
  can__rx_IgnoreArg_can_message_ptr();
  can_handler__handle_all_incoming_messages();
}

void test_handle_sensor_accmtr_incoming_messages_decode_fail_wrong_msg_id(void) {
  can__msg_t can_msg = {};
  // To simulate decode fail, putting wrong message id to fail header check while decoding
  can_msg.msg_id = dbc_header_SENSOR_BUTTON.message_id;
  can_msg.frame_fields.data_len = dbc_header_SENSOR_ACCMTR.message_dlc;
  can__rx_ExpectAndReturn(can1, &can_msg, 0, true);
  can__rx_IgnoreArg_can_message_ptr();
  can__rx_ReturnThruPtr_can_message_ptr(&can_msg);
  dbc_SENSOR_ACCMTR_s accmtr_recv_values;

  const dbc_message_header_t header = {
      .message_id = can_msg.msg_id,
      .message_dlc = can_msg.frame_fields.data_len,
  };

  TEST_ASSERT_EQUAL(false, dbc_decode_SENSOR_ACCMTR(&accmtr_recv_values, header, can_msg.data.bytes));
  can__rx_ExpectAndReturn(can1, &can_msg, 0, false);
  can__rx_IgnoreArg_can_message_ptr();
  can_handler__handle_all_incoming_messages();
}

void test_handle_sensor_accmtr_incoming_messages_decode_fail_wrong_msg_dlc(void) {
  can__msg_t can_msg = {};

  can_msg.msg_id = dbc_header_SENSOR_ACCMTR.message_id;
  // To simulate decode fail, putting wrong message dlc to fail header check while decoding
  can_msg.frame_fields.data_len = dbc_header_SENSOR_BUTTON.message_dlc;
  can__rx_ExpectAndReturn(can1, &can_msg, 0, true);
  can__rx_IgnoreArg_can_message_ptr();
  can__rx_ReturnThruPtr_can_message_ptr(&can_msg);
  dbc_SENSOR_ACCMTR_s accmtr_recv_values;

  const dbc_message_header_t header = {
      .message_id = can_msg.msg_id,
      .message_dlc = can_msg.frame_fields.data_len,
  };

  TEST_ASSERT_EQUAL(false, dbc_decode_SENSOR_ACCMTR(&accmtr_recv_values, header, can_msg.data.bytes));
  can__rx_ExpectAndReturn(can1, &can_msg, 0, false);
  can__rx_IgnoreArg_can_message_ptr();
  can_handler__handle_all_incoming_messages();
}

void test_handle_all_incoming_messages_receive_fail(void) {
  can__msg_t can_msg = {};
  // To simulate decode fail, putting wrong message id to fail header check while decoding
  can_msg.msg_id = dbc_header_SENSOR_BUTTON.message_id;
  can_msg.frame_fields.data_len = dbc_header_SENSOR_ACCMTR.message_dlc;
  can__rx_ExpectAndReturn(can1, &can_msg, 0, false);
  can__rx_IgnoreArg_can_message_ptr();
  can__rx_ReturnThruPtr_can_message_ptr(&can_msg);

  can_handler__handle_all_incoming_messages();
}

void test_handle_transmit_sensor_accmtr_message_success_can_tx(void) {
  can__msg_t can_msg_tx = {};
  can__tx_ExpectAndReturn(can1, &can_msg_tx, 0, true);
  can__tx_IgnoreArg_can_message_ptr();
  acceleration__axis_data_s axis_data;
  acceleration__get_data_ExpectAndReturn(axis_data);
  gpio_s gpio = {};
  board_io__get_led0_ExpectAndReturn(gpio);
  gpio__reset_Expect(gpio);
  can_handler__transmit_sensor_accmtr_message_10hz();
}

void test_handle_transmit_sensor_accmtr_message_failed_can_tx(void) {
  acceleration__axis_data_s axis_data;
  acceleration__get_data_ExpectAndReturn(axis_data);
  can__msg_t can_msg_tx = {};
  can__tx_ExpectAndReturn(can1, &can_msg_tx, 0, false);
  can__tx_IgnoreArg_can_message_ptr();
  gpio_s gpio = {};
  board_io__get_led0_ExpectAndReturn(gpio);
  gpio__set_Expect(gpio);
  can_handler__transmit_sensor_accmtr_message_10hz();
}

void test_handle_transmit_sensor_button_message_success_can_tx(void) {
  gpio_s gpio = {};
  board_io__get_sw0_ExpectAndReturn(gpio);
  gpio__get_ExpectAndReturn(gpio, true);
  can__msg_t can_msg_tx = {};
  can__tx_ExpectAndReturn(can1, &can_msg_tx, 0, true);
  can__tx_IgnoreArg_can_message_ptr();
  board_io__get_led1_ExpectAndReturn(gpio);
  gpio__reset_Expect(gpio);
  can_handler__transmit_sensor_button_message_10hz();
}

void test_handle_transmit_sensor_button_message_failed_can_tx(void) {
  gpio_s gpio = {};
  board_io__get_sw0_ExpectAndReturn(gpio);
  gpio__get_ExpectAndReturn(gpio, true);
  can__msg_t can_msg_tx = {};
  can__tx_ExpectAndReturn(can1, &can_msg_tx, 0, false);
  can__tx_IgnoreArg_can_message_ptr();
  board_io__get_led1_ExpectAndReturn(gpio);
  gpio__set_Expect(gpio);
  can_handler__transmit_sensor_button_message_10hz();
}