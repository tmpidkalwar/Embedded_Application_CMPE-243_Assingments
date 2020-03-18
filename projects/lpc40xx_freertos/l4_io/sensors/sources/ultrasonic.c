#include "ultrasonic.h"

#include <math.h>

#include "sys_time.h"

/**
  Reference:
  https://www.mpja.com/download/hc-sr04_ultrasonic_module_user_guidejohn.pdf

  Ultrasonic ranging module HC - SR04 provides 2cm - 400cm non-contact
  measurement function, the ranging accuracy can reach to 3mm. The modules
  includes ultrasonic transmitters, receiver and control circuit. The basic principle
  of work:
  (1) Using IO trigger for at least 10us high level signal,
  (2) The Module automatically sends eight 40 kHz and detect whether there is a
  pulse signal back.
  (3) IF the signal back, through high level , time of high output IO duration is
  the time from sending ultrasonic to returning.
  Test distance = (high level time × velocity of sound (340M/S) / 2,

  Working Voltage DC 5 V
  Working Current 15mA
  Working Frequency 40Hz
  Max Range 4m
  Min Range 2cm
  MeasuringAngle 15 degree
  Trigger Input Signal 10uS TTL pulse
  Echo Output Signal Input TTL lever signal and the range in proportion
 */

void ultrasonic__initialize(ultrasonic_s *ultrasonic, gpio__port_e trigger_port, uint8_t trigger_pin_number_0_to_31,
                            gpio__port_e echo_port, uint8_t echo_pin_number_0_to_31) {
  ultrasonic->trigger_out = gpio__construct_as_output(trigger_port, trigger_pin_number_0_to_31);
  ultrasonic->echo_in = gpio__construct_as_input(echo_port, echo_pin_number_0_to_31);
  ultrasonic->distance = 0U;

  gpio__reset(ultrasonic->trigger_out);
}

uint16_t ultrasonic__get_fake_range() { return rand() % 12; }

// Blocking get range function which can stall at maximum
// 60 ms + 10 us
void ultrasonic__get_range(ultrasonic_s *ultrasonic) {
  // ultrasonic->distance = 0U;
  uint64_t timer = sys_time__get_uptime_us();
  gpio__set(ultrasonic->trigger_out);
  while ((sys_time__get_uptime_us() - timer) < 10U) {
    ; // Hold trigger line high for 10 microseconds
  }
  gpio__reset(ultrasonic->trigger_out);
  timer = sys_time__get_uptime_us();

  bool echo_found = false;
  do {
    if (gpio__get(ultrasonic->echo_in)) { // sample echo until high
      echo_found = true;
    } else {
      if (echo_found) { // echo was sampled high and deasserted
        const size_t echo_high_level_time_us = (sys_time__get_uptime_us() - timer); // / 1000;
        ultrasonic->distance = echo_high_level_time_us / 148 / 2;
        break; // Exit when echo falling edge detected
      }
    }
  } while ((sys_time__get_uptime_us() - timer) < 60000U); // timeout on 60 milliseconds
}
