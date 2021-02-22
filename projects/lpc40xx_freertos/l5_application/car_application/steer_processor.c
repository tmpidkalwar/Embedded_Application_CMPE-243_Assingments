#include "steer_processor.h"

void steer_processor(uint32_t left_sensor_cm, uint32_t right_sensor_cm) {
  // Make sure this funciton processes when one of the sensor value is below threshold
  if ((left_sensor_cm < 50) || (right_sensor_cm < 50)) {
    if (left_sensor_cm > right_sensor_cm) {
      steer_left();
    } else {
      steer_right();
    }
  }
}