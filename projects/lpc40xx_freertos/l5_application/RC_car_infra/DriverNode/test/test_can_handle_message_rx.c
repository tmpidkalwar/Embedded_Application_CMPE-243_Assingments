#include "Mockboard_io.h"
#include "Mockcan_bus.h"
#include "Mockdriver_logic.h"
#include "Mockgpio.h"
#include "can_handle_message_rx.c"
#include "project.h"
#include "unity.h"

void setUp() { memset((void *)&sensor_data, 0, sizeof(sensor_data)); }

void test_handle_all_can_messages_rx() {
  can__msg_t rx_msg_can = {0};
  dbc_SENSOR_SONARS_s sensor_data = {};
  dbc_message_header_t header;
  gpio_s gpio = {};

  // Valid Case
  rx_msg_can.msg_id = 200;
  rx_msg_can.data.bytes[0] = 0xAA;
  rx_msg_can.data.bytes[1] = 0xAA;
  rx_msg_can.data.bytes[2] = 0xAA;
  rx_msg_can.data.bytes[3] = 0xAA;
  rx_msg_can.data.bytes[4] = 0xAA;
  rx_msg_can.frame_fields.data_len = 5;
  header.message_id = rx_msg_can.msg_id;
  header.message_dlc = rx_msg_can.frame_fields.data_len;

  dbc_decode_SENSOR_SONARS(&sensor_data, header, rx_msg_can.data.bytes);

  board_io__get_led0_ExpectAndReturn(gpio);
  gpio__reset_Expect(gpio);
  can__rx_ExpectAndReturn(can1, &rx_msg_can, 0, 1);
  can__rx_IgnoreArg_can_message_ptr();
  can__rx_ReturnThruPtr_can_message_ptr(&rx_msg_can);
  can__rx_ExpectAndReturn(can1, &rx_msg_can, 0, 0);
  can__rx_IgnoreArg_can_message_ptr();
  driver_process_input_Expect(&sensor_data);
  handle_all_can_messages_rx(can1);

  // Invalid msg id, should not call driver_process_input
  rx_msg_can.msg_id = 201;
  rx_msg_can.data.bytes[0] = 0xAA;
  rx_msg_can.data.bytes[1] = 0xAA;
  rx_msg_can.data.bytes[2] = 0xAA;
  rx_msg_can.data.bytes[3] = 0xAA;
  rx_msg_can.data.bytes[4] = 0xAA;
  rx_msg_can.frame_fields.data_len = 5;
  header.message_id = rx_msg_can.msg_id;
  header.message_dlc = rx_msg_can.frame_fields.data_len;

  dbc_decode_SENSOR_SONARS(&sensor_data, header, rx_msg_can.data.bytes);

  board_io__get_led0_ExpectAndReturn(gpio);
  gpio__reset_Expect(gpio);
  can__rx_ExpectAndReturn(can1, &rx_msg_can, 0, 1);
  can__rx_IgnoreArg_can_message_ptr();
  can__rx_ReturnThruPtr_can_message_ptr(&rx_msg_can);
  can__rx_ExpectAndReturn(can1, &rx_msg_can, 0, 0);
  can__rx_IgnoreArg_can_message_ptr();
  handle_all_can_messages_rx(can1);

  // Valid message id, wrong data len, should not call driver_process_input
  rx_msg_can.msg_id = 200;
  rx_msg_can.data.bytes[0] = 0xAA;
  rx_msg_can.data.bytes[1] = 0xAA;
  rx_msg_can.data.bytes[2] = 0xAA;
  rx_msg_can.data.bytes[3] = 0xAA;
  rx_msg_can.data.bytes[4] = 0xAA;
  rx_msg_can.frame_fields.data_len = 4;
  header.message_id = rx_msg_can.msg_id;
  header.message_dlc = rx_msg_can.frame_fields.data_len;

  dbc_decode_SENSOR_SONARS(&sensor_data, header, rx_msg_can.data.bytes);

  board_io__get_led0_ExpectAndReturn(gpio);
  gpio__reset_Expect(gpio);
  can__rx_ExpectAndReturn(can1, &rx_msg_can, 0, 1);
  can__rx_IgnoreArg_can_message_ptr();
  can__rx_ReturnThruPtr_can_message_ptr(&rx_msg_can);
  can__rx_ExpectAndReturn(can1, &rx_msg_can, 0, 0);
  can__rx_IgnoreArg_can_message_ptr();
  handle_all_can_messages_rx(can1);
}

void test_mia_can_bus_message_handler_rx() {
  can__msg_t rx_msg_can = {};
  gpio_s gpio = {};
  int i;
  // dbc_SENSOR_SONARS_s sensor_data = {};

  rx_msg_can.msg_id = 201; // Dummy message ID
  // rx_msg_can.data.bytes[0] = 0xAA;
  rx_msg_can.frame_fields.data_len = 5;
  for (i = 0; i < SENSOR_SONARS_MIA_THRESHOLD - 1; i++) {
    can__rx_ExpectAndReturn(can1, &rx_msg_can, 0, 1);
    can__rx_IgnoreArg_can_message_ptr();
    can__rx_ReturnThruPtr_can_message_ptr(&rx_msg_can);
    can__rx_ExpectAndReturn(can1, &rx_msg_can, 0, 0);
    can__rx_IgnoreArg_can_message_ptr();
    board_io__get_led0_ExpectAndReturn(gpio);
    // driver_process_input_Expect(&sensor_data);
    gpio__reset_Expect(gpio);
    handle_all_can_messages_rx(can1);
  }
  board_io__get_led0_ExpectAndReturn(gpio);
  // driver_process_input_Expect(&sensor_data);
  gpio__set_Expect(gpio);
  can__rx_ExpectAndReturn(can1, &rx_msg_can, 0, 1);
  can__rx_IgnoreArg_can_message_ptr();
  can__rx_ReturnThruPtr_can_message_ptr(&rx_msg_can);
  can__rx_ExpectAndReturn(can1, &rx_msg_can, 0, 0);
  can__rx_IgnoreArg_can_message_ptr();
  // MIA should be triggered, led should be set
  handle_all_can_messages_rx(can1);
}
