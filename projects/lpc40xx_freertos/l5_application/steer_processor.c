#include "steer_processor.h"

void steer_processor(uint32_t left_sensor_cm, uint32_t right_sensor_cm) {
  int threshold = 50;
  if (left_sensor_cm < threshold && right_sensor_cm > threshold)
    steer_right();
  else if (right_sensor_cm < threshold && left_sensor_cm > threshold)
    steer_left();
  else if (left_sensor_cm < threshold && right_sensor_cm < threshold)
    left_sensor_cm < right_sensor_cm ? steer_right() : steer_left();
}