#include "board_io.h"
#include "gpio.h"

static gpio_s my_led;
static gpio_s my_switch;

void switch_led_logic__initialize();

void switch_led_logic__run_once();