#include "acceleration.h"
#include "can_bus.h"
#include "can_mia_configuration.h"
#include "project.h"
#include <stdio.h>

dbc_SENSOR_ACCMTR_s accmtr_recv_values;

void can_handler__manage_mia_10hz(void) {
  // We are in 10hz slot, so increment MIA counter by 100ms
  const uint32_t mia_increment_value = 100;

  if (can_mia__service_mia_SENSOR_ACCMTR(&accmtr_recv_values, mia_increment_value)) {
    // Take action when a message has gone MIA?
    // Maybe light up an LED?
    // fprintf(stderr, "MIA occured\n");
  }
}

void can_handler__handle_all_incoming_messages(void) {
  can__msg_t can_msg = {};
  // dbc_SENSOR_ACCMTR_s decoded_sensor_accmtr = {};

  while (can__rx(can1, &can_msg, 5)) {
    // Construct "message header" that we need for the decode_*() API
    const dbc_message_header_t header = {
        .message_id = can_msg.msg_id,
        .message_dlc = can_msg.frame_fields.data_len,
    };

    if (dbc_decode_SENSOR_ACCMTR(&accmtr_recv_values, header, can_msg.data.bytes)) {
      fprintf(stderr, "Received data success %ld, %ld\n", accmtr_recv_values.SENSOR_ACCMTR_x,
              accmtr_recv_values.SENSOR_ACCMTR_y);
    } else {
      fprintf(stderr, "failed to decode\n");
    }
    // dbc_decode_SENSOR_BUTTON(&decoded_sensor_accmtr, header, can_msg.data.bytes);
  }
}

void can_handler__transmit_message_10hz(void) {
  acceleration__axis_data_s axis_data = acceleration__get_data();
  dbc_SENSOR_ACCMTR_s accmtr_struct = {};

  // Populate accmtr structure
  accmtr_struct.SENSOR_ACCMTR_x = axis_data.x;
  accmtr_struct.SENSOR_ACCMTR_y = axis_data.y;

  // Encode struct to bytes of the CAN message
  can__msg_t can_msg_tx = {};
  const dbc_message_header_t header = dbc_encode_SENSOR_ACCMTR(can_msg_tx.data.bytes, &accmtr_struct);

  can_msg_tx.msg_id = header.message_id;
  can_msg_tx.frame_fields.data_len = header.message_dlc;
  if (!can__tx(can1, &can_msg_tx, 0)) {
    fprintf(stderr, "Not able sent\n");
  } else {
    fprintf(stderr, "failed to send\n");
  }
}