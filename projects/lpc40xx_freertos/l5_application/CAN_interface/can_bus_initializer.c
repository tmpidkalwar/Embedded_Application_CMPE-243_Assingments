#include "can_bus_initializer.h"
#include "board_io.h"
#include "gpio.h"
#include "lpc40xx.h"
#include "stdio.h"

static bool is_switch2_pressed = false;

bool can_bus_initializer__can1_init(void) {
  can__init(can1, 100, 1000, 100, NULL, NULL);
  can__bypass_filter_accept_all_msgs();
  can__reset_bus(can1);
  return true;
}

void switch2_press_handler(void) {
  is_switch2_pressed = true;
  LPC_GPIOINT->IO0IntClr |= (1 << 30);
}

bool get_switch2_status(void) { return is_switch2_pressed; }
void clear_switch2_status(void) { is_switch2_pressed = false; }