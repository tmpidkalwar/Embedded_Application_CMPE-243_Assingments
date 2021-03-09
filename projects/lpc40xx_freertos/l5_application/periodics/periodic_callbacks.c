#include "periodic_callbacks.h"

#include <stdio.h>

#include "board_io.h"
#include "gpio.h"

#include "can_bus.h"
#include "can_bus_initializer.h"
//#include "portmacro.h"

static uint16_t can_msg_rx_count;
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
  // can__msg_t msg = {};
  // msg.msg_id = 0x12;
  // msg.frame_fields.is_29bit = 0;
  // msg.frame_fields.data_len = 8;
  // msg.data.qword = 0x1122334455667788;
  // if (can__tx(can1, &msg, 5)) {
  //   gpio__toggle(board_io__get_led2());
  // }

  can__msg_t received_msg = {};
  while (can__rx(can1, &received_msg, 5)) {
    // if (received_msg.data.bytes[7] == 0x11) {
    // can_msg_rx_count++;
    fprintf(stderr, "rx complete : %d", received_msg.data.bytes[0]);
    //}
    gpio__toggle(board_io__get_led3());
  }
  // bool can1_status = can__is_bus_off(can1);
  // fprintf(stderr, "CAN1 is %d", can1_status);
  // Add your code here
}

/**
 * @warning
 * This is a very fast 1ms task and care must be taken to use this
 * This may be disabled based on intialization of periodic_scheduler__initialize()
 */
void periodic_callbacks__1000Hz(uint32_t callback_count) {
  gpio__toggle(board_io__get_led3());
  // Add your code here
}