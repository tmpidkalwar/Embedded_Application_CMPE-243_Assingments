#include "can_handle_message_rx.h"
#include "can_bus.h"
#include "driver_logic.h"
#include "project.h"

#include <stdio.h>

void handle_all_can_messages_rx(can__num_e can_num) {
  dbc_SENSOR_SONARS_s sensor_data = {};
  can__msg_t can_msg = {};
  int timeout = 0;

  while (can__rx(can_num, &can_msg, timeout)) {
    const dbc_message_header_t header = {.message_id = can_msg.msg_id, .message_dlc = can_msg.frame_fields.data_len};

    if (dbc_decode_SENSOR_SONARS(&sensor_data, header, can_msg.data.bytes)) {
      driver_process_input(&sensor_data);
    }
  }
}
