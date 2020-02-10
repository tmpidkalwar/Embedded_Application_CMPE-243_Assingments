#include "switch_led_logic.h"
#include "board_io.h"
#include "gpio.h"

static gpio_s led0;
static gpio_s switch0;

void switch_led_logic__initialize() {
  led0 = gpio__construct_as_output(GPIO__PORT_1, 18);
  switch0 = gpio__construct_as_input(GPIO__PORT_0, 29);
  gpio__reset(led0);
  gpio__reset(switch0);
}

void switch_led_logic__run_once() {
  if (gpio__get(switch0)) {
    gpio__set(led0);
  } else {
    gpio__reset(led0);
  }
}