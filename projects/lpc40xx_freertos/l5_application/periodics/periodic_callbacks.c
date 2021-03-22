#include "periodic_callbacks.h"

#include "can_bus.h"
#include "can_bus_initializer.h"
#include "can_handle_message_rx.h"
#include "can_handle_message_tx.h"
/******************************************************************************
 * Your board will reset if the periodic function does not return within its deadline
 * For 1Hz, the function must return within 1000ms
 * For 1000Hz, the function must return within 1ms
 */
void periodic_callbacks__initialize(void) {
  // This method is invoked once when the periodic tasks are created
  can_bus_initialize(can1);
}

void periodic_callbacks__1Hz(uint32_t callback_count) {
  //  gpio__toggle(board_io__get_led0());
  // Add your code here
  handle_all_can_messages_rx(can1);
  handle_all_can_messages_tx(can1);
}

void periodic_callbacks__10Hz(uint32_t callback_count) {
  // Add your code here
  // handle_all_can_messages_rx(can1);
  // handle_all_can_messages_tx(can1);
}
void periodic_callbacks__100Hz(uint32_t callback_count) {
  //  gpio__toggle(board_io__get_led2());
  // Add your code here
}

/**
 * @warning
 * This is a very fast 1ms task and care must be taken to use this
 * This may be disabled based on intialization of periodic_scheduler__initialize()
 */
void periodic_callbacks__1000Hz(uint32_t callback_count) {
  // Add your code here
}
