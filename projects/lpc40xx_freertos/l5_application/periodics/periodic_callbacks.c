#include "periodic_callbacks.h"

#include "board_io.h"
#include "gpio.h"
#include "gps.h"
#include <stdio.h>

/******************************************************************************
 * Your board will reset if the periodic function does not return within its deadline
 * For 1Hz, the function must return within 1000ms
 * For 1000Hz, the function must return within 1ms
 */
void periodic_callbacks__initialize(void) {
  // This method is invoked once when the periodic tasks are created
  gpio__construct_with_function(GPIO__PORT_4, 28, GPIO__FUNCTION_2);
  gpio__construct_with_function(GPIO__PORT_4, 29, GPIO__FUNCTION_2);
  gps__init();
}

void periodic_callbacks__1Hz(uint32_t callback_count) {

  // gpio__toggle(board_io__get_led0());

  // Add your code here
}

void periodic_callbacks__10Hz(uint32_t callback_count) {
  // gpio__toggle(board_io__get_led1());
  // Add your code here
}
void periodic_callbacks__100Hz(uint32_t callback_count) { gps__run_once(); }

void periodic_callbacks__1000Hz(uint32_t callback_count) {
  // gpio__toggle(board_io__get_led3());
  // Add your code here
}