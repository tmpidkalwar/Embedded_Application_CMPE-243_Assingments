#include "switch_led_logic.h"

gpio_s my_led;
gpio_s my_switch;

void switch_led_logic__initialize() {
  my_led = gpio__construct_as_output(GPIO__PORT_1, 18);
  my_switch = gpio__construct_as_input(GPIO__PORT_0, 29);
}

void switch_led_logic__run_once() {
  if (gpio__get(my_switch)) {
    gpio__set(my_led);
  } else {
    gpio__reset(my_led);
  }
}