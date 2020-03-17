#pragma once

#include <stdint.h>
#include <stdlib.h>

#include "gpio.h"

typedef struct ultrasonic {
  size_t distance;
  gpio_s trigger_out;
  gpio_s echo_in;
} ultrasonic_s;

void ultrasonic__initialize(ultrasonic_s *ultrasonic, gpio__port_e trigger_port, uint8_t trigger_pin_number_0_to_31,
                            gpio__port_e echo_port, uint8_t echo_pin_number_0_to_31);

uint16_t ultrasonic__get_fake_range();

void ultrasonic__get_range(ultrasonic_s *ultrasonic);
