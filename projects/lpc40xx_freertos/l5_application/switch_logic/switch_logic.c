#include "switch_logic.h"

void switch_led_logic__initialize(void) {
  led = board_io__get_led1();
  switch_button = gpio__construct_as_input(GPIO__PORT_0, 30);
}

void switch_led_logic__run_once(void) {
  if (gpio__get(switch_button)) {
    gpio__set(led);
  } else {
    gpio__reset(led);
  }
}