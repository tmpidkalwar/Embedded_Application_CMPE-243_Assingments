
#include "switch_led_logic.h"

void switch_led_logic__initialize() {
  led1 = gpio__construct_as_output(GPIO__PORT_1, 26);

  sw3 = gpio__construct_as_input(GPIO__PORT_0, 29);
}
void switch_led_logic__simple_switch() {
  if (gpio__get(sw3)) {
    gpio__set(led1);
  } else {
    gpio__reset(led1);
  }
}