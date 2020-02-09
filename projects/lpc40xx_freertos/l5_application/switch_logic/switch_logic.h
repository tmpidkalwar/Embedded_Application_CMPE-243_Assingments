#ifndef SWITCH_LOGIC_H
#define SWITCH_LOGIC_H
#include "board_io.h"
#include "gpio.h"
#include "stdbool.h"
gpio_s led, switch_button;
void switch_led_logic__initialize(void);
void switch_led_logic__run_once(void);
#endif