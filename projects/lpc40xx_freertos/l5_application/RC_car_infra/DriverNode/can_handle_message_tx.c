#include "board_io.h"
#include "can_bus.h"
#include "can_handle_message_rx.h"
#include "driver_logic.h"
#include "gpio.h"
#include "project.h"

#include <stdio.h>

// define this at maybe dbc_to_can_driver_glue.c
bool dbc_send_can_message(void *argument, uint32_t message_id, const uint8_t bytes[8], uint8_t dlc) {
  can__msg_t msg = {};

  msg.frame_fields.data_len = dlc;
  msg.msg_id = message_id;
  memcpy(msg.data.bytes, bytes, 8);

  return can__tx(can1, &msg, 0);
}

void handle_all_can_messages_tx(can__num_e can_num) {
  dbc_DRIVER_TO_MOTOR_s motor_data = driver_logic__get_motor_command();
  if (dbc_encode_and_send_DRIVER_TO_MOTOR(NULL, &motor_data)) {
    gpio__toggle(board_io__get_led2());
  } else {
    gpio__toggle(board_io__get_led1());
  }
}
