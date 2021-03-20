/**
 * @file driver_logic.c
 *
 *
 */

#include "driver_logic.h"

static dbc_SENSOR_SONARS_s sensor_recvd_values;

/**
 * Sensor value ranges from 0 to 1024. Lesser the value, nearer the object.
 * By considering some buffer values, let's consider values from 300 to 800 to
 * act upon to start steering away from object.
 * 300 => 45 degree
 * 800 => 5 degree
 * So constant is inverse ration of 40/500
 */
const static long max_sensor_value_to_start_steering = 800;
const static long min_sensor_value_to_map_max_steering_angle = 300;
const static long min_angle = 5;
const static long max_angle = 45;

const static float sensor_to_steer_angle_map_const =
    (max_angle - min_angle) / (max_sensor_value_to_start_steering - min_sensor_value_to_map_max_steering_angle);

static int8_t map(long input, long in_min, long in_max, long out_min, long out_max) {
  return (input - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

static void steer_signal_to_avoid_obstacle(dbc_DRIVER_TO_MOTOR_s *motor_signals) {
  if (sensor_recvd_values.SENSOR_SONARS_left < sensor_recvd_values.SENSOR_SONARS_right) {

    if (sensor_recvd_values.SENSOR_SONARS_left < max_sensor_value_to_start_steering) {

      if()
      motor_signals->MOTOR_direction =
          (int8_t)(sensor_to_steer_angle_map_const * sensor_recvd_values.SENSOR_SONARS_left);
    }
    fprintf(stderr, "i am here  %d\n", motor_signals->MOTOR_direction);
  } else {

    if (sensor_recvd_values.SENSOR_SONARS_right < max_sensor_value_to_start_steering) {
      motor_signals->MOTOR_direction =
          (int8_t)((-1) * sensor_to_steer_angle_map_const * sensor_recvd_values.SENSOR_SONARS_right);
    }
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

const static float sens_val_to_fwd_speed_map_const =
    (fwd_sens_val_to_map_to_max_fwd_speed - fwd_sens_val_to_start_fwd_moving_car) / (max_fwd_speed - min_fwd_speed);

/****************** Reverse Speed Mapping *********************/
const static float rear_sense_val_to_stop_reversing_car = 300;
const static float rear_sense_val_to_map_to_max_reverse_speed = 600;
const static float max_reverse_speed = 12.5;
const static float min_reverse_speed = 0;

const static float rear_sens_val_to_reverse_speed_map_const =
    (rear_sense_val_to_map_to_max_reverse_speed - rear_sense_val_to_stop_reversing_car) /
    (max_reverse_speed - min_reverse_speed);

static void speed_control_to_avoid_obstacle(dbc_DRIVER_TO_MOTOR_s *motor_signals) {
  const uint16_t fwd_sens_val = sensor_recvd_values.SENSOR_SONARS_middle;
  bool start_reversing_car = false;

  if (fwd_sens_val >= fwd_sens_val_to_map_to_max_fwd_speed) {
    motor_signals->MOTOR_speed = max_fwd_speed;
  } else if (fwd_sens_val < fwd_sens_val_to_map_to_max_fwd_speed) {

    if (fwd_sens_val >= fwd_sens_val_to_start_fwd_moving_car) {
      motor_signals->MOTOR_speed = sens_val_to_fwd_speed_map_const * fwd_sens_val;
    } else if (fwd_sens_val > fwd_sens_val_to_start_reversing_car) {
      motor_signals->MOTOR_speed = min_fwd_speed;
    } else {
      start_reversing_car = true;
    }
  }

  const uint16_t rear_sens_val = sensor_recvd_values.SENSOR_SONARS_rear;

  if (start_reversing_car) {
    if (rear_sens_val >= rear_sense_val_to_map_to_max_reverse_speed) {
      motor_signals->MOTOR_speed = (float)(-12.5);
    } else if (rear_sens_val > rear_sense_val_to_stop_reversing_car) {
      motor_signals->MOTOR_speed = (float)(rear_sens_val_to_reverse_speed_map_const * rear_sens_val);
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