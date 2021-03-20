/**
 * @file driver_logic.c
 *
 *
 */
#include <stdio.h>

#include "driver_logic.h"

static uint16_t sensor_left;
static uint16_t sensor_right;
static uint16_t sensor_middle;
static uint16_t sensor_rear;

void driver_process_input(dbc_SENSOR_SONARS_s *sensor_data) {
  sensor_left = sensor_data->SENSOR_SONARS_left;
  sensor_right = sensor_data->SENSOR_SONARS_right;
  sensor_middle = sensor_data->SENSOR_SONARS_middle;
  sensor_rear = sensor_data->SENSOR_SONARS_rear;
}

/**
 * Sensor value ranges from 0 to 1024. Lesser the value, nearer the object.
 * By considering some buffer values, let's consider values from 300 to 800 to
 * act upon to start steering away from object.
 * 300 => 45 degree
 * 800 => 5 degree
 * So constant is inverse ration of 40/500
 */
const static float max_sensor_value_to_start_steering = 800;
const static float min_sensor_value_to_map_max_steering_angle = 300;
const static float min_angle = 5;
const static float max_angle = 45;

static int8_t map(long input, float in_min, float in_max, float out_min, float out_max) {
  return (input - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

static void steer_signal_to_avoid_obstacle(dbc_DRIVER_TO_MOTOR_s *motor_signals) {
  motor_signals->MOTOR_direction = 0;
  if (sensor_left < sensor_right) {
    uint16_t left_sensor = sensor_left;
    if (sensor_left < max_sensor_value_to_start_steering) {

      if (sensor_left < min_sensor_value_to_map_max_steering_angle) {
        motor_signals->MOTOR_direction = 45;
      } else {
        motor_signals->MOTOR_direction = map(left_sensor, max_sensor_value_to_start_steering,
                                             min_sensor_value_to_map_max_steering_angle, min_angle, max_angle);
      }
    }
    fprintf(stderr, "i am here  %d\n", motor_signals->MOTOR_direction);
  } else {

    uint16_t right_sensor = sensor_left;

    if (sensor_right < min_sensor_value_to_map_max_steering_angle) {
      motor_signals->MOTOR_direction = (int8_t)((-1) * 45);
    } else {
      if (sensor_right < max_sensor_value_to_start_steering) {
        motor_signals->MOTOR_direction =
            (int8_t)((-1) * map(right_sensor, max_sensor_value_to_start_steering,
                                min_sensor_value_to_map_max_steering_angle, min_angle, max_angle));
      }
    }
    fprintf(stderr, "i am here in 2  %d\n", motor_signals->MOTOR_direction);
  }
}

/**
 * Sensor value ranges from 0 to 1024. Lesser the value, nearer the object.
 * To map forward speed, let's use forward sensor value from 400 to 900.
 * >=900 => 12.5 kph
 * 400 => 0 kph
 * 400 to 300 : neutral and do not move
 * < 300 => start acting upon at rear sensor value
 * rear sensor value map:
 * >= 600 => 12.5 kph
 * <= 300 => 0 kph
 */

/***************** Forward Speed Mapping ****************/
const static float fwd_sens_val_to_map_to_max_fwd_speed = 900;
const static float fwd_sens_val_to_start_fwd_moving_car = 400;
const static float fwd_sens_val_to_start_reversing_car = 300;
const static float max_fwd_speed = 12.5;
const static float min_fwd_speed = 0;

/****************** Reverse Speed Mapping *********************/
const static float rear_sense_val_to_stop_reversing_car = 300;
const static float rear_sense_val_to_map_to_max_reverse_speed = 600;
const static float max_reverse_speed = 12.5;
const static float min_reverse_speed = 0;

static void speed_control_to_avoid_obstacle(dbc_DRIVER_TO_MOTOR_s *motor_signals) {
  bool start_reversing_car = false;

  if (sensor_middle >= fwd_sens_val_to_map_to_max_fwd_speed) {
    motor_signals->MOTOR_speed = max_fwd_speed;
  } else if (sensor_middle < fwd_sens_val_to_map_to_max_fwd_speed) {

    if (sensor_middle >= fwd_sens_val_to_start_fwd_moving_car) {

      motor_signals->MOTOR_speed =
          map(sensor_middle, rear_sense_val_to_stop_reversing_car, rear_sense_val_to_map_to_max_reverse_speed,
              min_reverse_speed, max_reverse_speed);
    } else if (sensor_middle > fwd_sens_val_to_start_reversing_car) {
      motor_signals->MOTOR_speed = min_fwd_speed;
    } else {
      start_reversing_car = true;
    }
  }

  if (start_reversing_car) {
    if (sensor_rear >= rear_sense_val_to_map_to_max_reverse_speed) {
      motor_signals->MOTOR_speed = (float)(-12.5);
    } else if (sensor_rear > rear_sense_val_to_stop_reversing_car) {

      motor_signals->MOTOR_speed =
          map(sensor_rear, rear_sense_val_to_stop_reversing_car, rear_sense_val_to_map_to_max_reverse_speed,
              min_reverse_speed, max_reverse_speed);
    } else {
      motor_signals->MOTOR_speed = 0;
    }
  }
}

dbc_DRIVER_TO_MOTOR_s driver_logic__get_motor_command(void) {
  dbc_DRIVER_TO_MOTOR_s motor_signals;
  steer_signal_to_avoid_obstacle(&motor_signals);
  speed_control_to_avoid_obstacle(&motor_signals);
  return motor_signals;
}
