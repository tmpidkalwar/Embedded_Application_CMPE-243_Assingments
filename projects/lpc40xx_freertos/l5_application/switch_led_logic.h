#pragma once

#include "gpio.h"
#include "lpc40xx.h"

static gpio_s led1;
static gpio_s sw3;

void switch_led_logic__initialize();
void switch_led_logic__simple_switch();
