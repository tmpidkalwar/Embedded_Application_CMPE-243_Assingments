#include "periodic_callbacks.h"

#include <stdio.h>

#include "board_io.h"
#include "gpio.h"

#include "can_bus.h"
#include "can_bus_initializer.h"

/******************************************************************************
 * Your board will reset if the periodic function does not return within its deadline
 * For 1Hz, the function must return within 1000ms
 * For 1000Hz, the function must return within 1ms
 */
void periodic_callbacks__initialize(void) {
  // This method is invoked once when the periodic tasks are created
  can_bus_initializer__can1_init();
}

void periodic_callbacks__1Hz(uint32_t callback_count) {
  gpio__toggle(board_io__get_led0());
  // Add your code here
}

void periodic_callbacks__10Hz(uint32_t callback_count) {
  gpio__toggle(board_io__get_led1());
  // Add your code here
}
void periodic_callbacks__100Hz(uint32_t callback_count) {
#ifdef CAN_AS_TX
  can__msg_t msg = {};
  msg.msg_id = 0x11;
  msg.frame_fields.is_29bit = 0;
  msg.frame_fields.data_len = 8;
  if (get_switch2_status()) {
    msg.data.qword = 0x1122334455667788;
    clear_switch2_status();
  } else {
    msg.data.qword = 0x00;
  }
  if (can__tx(can1, &msg, 1)) {
    gpio__toggle(board_io__get_led2());
  }
#endif
#ifdef CAN_AS_RX
  can__msg_t received_msg = {};
  while (can__rx(can1, &received_msg, 1)) {
    fprintf(stderr, "received\n");
    if (received_msg.data.bytes[7] == 0x11) {
      fprintf(stderr, "buton pressed");
      gpio__toggle(board_io__get_led3());
    }
  }
#endif
}

/**
 * @warning
 * This is a very fast 1ms task and care must be taken to use this
 * This may be disabled based on intialization of periodic_scheduler__initialize()
 */
void periodic_callbacks__1000Hz(uint32_t callback_count) { gpio__toggle(board_io__get_led3()); }