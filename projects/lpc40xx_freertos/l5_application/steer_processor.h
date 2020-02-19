#pragma once

#include <stdint.h>

#include "steering.h"

/**
 * Assume that a threshold value is 50cm
 * Objective is to invoke steer function if a sensor value is less than the threshold
 *
 * Example: If left sensor is 49cm, and right is 70cm, then we should call steer_right()
 */
void steer_processor(uint32_t left_sensor_cm, uint32_t right_sensor_cm);
