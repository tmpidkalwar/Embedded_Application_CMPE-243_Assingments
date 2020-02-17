#include "steer_processor.h"

void steer_processor(uint32_t left_sensor_cm, uint32_t right_sensor_cm) {
  if (left_sensor_cm || right_sensor_cm < 50) {
    if (left_sensor_cm < right_sensor_cm) {
      steer_right();
    } else if (right_sensor_cm < left_sensor_cm) {
      steer_left();
    }
  }
}
