#pragma once

#include "gpio.h"
#include <stdbool.h>
#include <stdint.h>
void switch_led_logic__initialize();
void switch_led_logic__run_once();
gpio_s my_led;
gpio_s my_switch;