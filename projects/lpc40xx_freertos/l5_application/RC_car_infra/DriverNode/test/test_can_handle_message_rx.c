#include "Mockcan_bus.h"
#include "Mockdriver_logic.h"
#include "can_handle_message_rx.h"
#include "project.h"
#include "unity.h"

void test_handle_all_can_messages_rx() {
  can__msg_t rx_msg_can = {0};
  dbc_SENSOR_SONARS_s sensor_data = {};
  dbc_message_header_t header;
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

  can__rx_ExpectAndReturn(can1, &rx_msg_can, 0, 1);
  can__rx_IgnoreArg_can_message_ptr();
  can__rx_ReturnThruPtr_can_message_ptr(&rx_msg_can);
  can__rx_ExpectAndReturn(can1, &rx_msg_can, 0, 0);
  can__rx_IgnoreArg_can_message_ptr();
  handle_all_can_messages_rx(can1);
}
