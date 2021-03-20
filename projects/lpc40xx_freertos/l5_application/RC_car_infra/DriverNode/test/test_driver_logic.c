#include "unity.h"

#include "project.h"

// Include source file of driver logic as this test file will need static variable to acccess
#include "driver_logic.c"

void setup(void) {
  sensor_right = 0;
  sensor_left = 0;
}

void teardown(void) {}

// Note: left steering angle values are in postive range
void test_steer_left_with_left_sens_val_less_than_max(void) {

  dbc_DRIVER_TO_MOTOR_s motor_signal; // = {.MOTOR_direction = 0, .MOTOR_speed = 0};
  // Assign some value to right sensor so that it will be always greater than left sensor value, to cause left steering
  sensor_right = max_sensor_value_to_start_steering;

  // case 1: left sensor value very near to value start cauring left steer
  sensor_left = max_sensor_value_to_start_steering - 1;

  motor_signal = driver_logic__get_motor_command();
  TEST_ASSERT_GREATER_OR_EQUAL(5, motor_signal.MOTOR_direction);
}

// Note: left steering angle values are in postive range
void test_steer_left_with_left_sens_val_greater_than_min(void) {

  dbc_DRIVER_TO_MOTOR_s motor_signal; // = {.MOTOR_direction = 0, .MOTOR_speed = 0};
  // Assign some value to right sensor so that it will be always greater than left sensor value, to cause left
  // steering
  sensor_right = max_sensor_value_to_start_steering;

  // case 2: left sensor value in min and max range of values mapped to angle value
  sensor_left = min_sensor_value_to_map_max_steering_angle + 1;

  motor_signal = driver_logic__get_motor_command();
  TEST_ASSERT_GREATER_OR_EQUAL(min_angle, motor_signal.MOTOR_direction);
}

// Note: left steering angle values are in postive range
void test_steer_left_with_left_sens_val_less_than_min(void) {

  dbc_DRIVER_TO_MOTOR_s motor_signal; // = {.MOTOR_direction = 0, .MOTOR_speed = 0};
  // Assign some value to right sensor so that it will be always greater than left sensor value, to cause left
  // steering
  sensor_right = max_sensor_value_to_start_steering;

  // case 3: left sensor value less than min sensor value mapped to steering angle
  sensor_left = min_sensor_value_to_map_max_steering_angle - 1;

  motor_signal = driver_logic__get_motor_command();
  TEST_ASSERT_GREATER_OR_EQUAL(min_angle, motor_signal.MOTOR_direction);
}

void test_not_to_steer_with_left_right_sensor_val_great_than_max(void) {

  dbc_DRIVER_TO_MOTOR_s motor_signal = {.MOTOR_direction = 0, .MOTOR_speed = 0};
  // Assign some value to right and left sensor greater than max mapped value responsible to start steering
  sensor_right = max_sensor_value_to_start_steering + 1;
  sensor_left = max_sensor_value_to_start_steering + 1;

  motor_signal = driver_logic__get_motor_command();
  TEST_ASSERT_LESS_THAN(min_angle, motor_signal.MOTOR_direction);
}

// Note: right steering angle values are in negative range
void test_steer_right_with_right_sens_val_less_than_max(void) {

  dbc_DRIVER_TO_MOTOR_s motor_signal; // = {.MOTOR_direction = 0, .MOTOR_speed = 0};
  // Assign some value to right sensor so that it will be always greater than left sensor value, to cause left
  // steering
  sensor_left = max_sensor_value_to_start_steering;

  // case 1: left sensor value very near to value start cauring left steer
  sensor_right = max_sensor_value_to_start_steering - 1;

  motor_signal = driver_logic__get_motor_command();
  TEST_ASSERT_LESS_OR_EQUAL((min_angle * -1), motor_signal.MOTOR_direction);
}

// Note: left steering angle values are in negative range
void test_steer_right_with_right_sens_val_greater_than_min(void) {

  dbc_DRIVER_TO_MOTOR_s motor_signal = {.MOTOR_direction = 0, .MOTOR_speed = 0};
  // Assign some value to right sensor so that it will be always greater than left sensor value, to cause left
  // steering
  sensor_left = max_sensor_value_to_start_steering;

  // case 2: left sensor value in min and max range of values mapped to angle value
  sensor_right = min_sensor_value_to_map_max_steering_angle + 1;

  motor_signal = driver_logic__get_motor_command();
  TEST_ASSERT_LESS_OR_EQUAL((min_angle * -1), motor_signal.MOTOR_direction);
}

// Note: left steering angle values are in negative range
void test_steer_right_with_right_sens_val_less_than_min(void) {

  dbc_DRIVER_TO_MOTOR_s motor_signal = {.MOTOR_direction = 0, .MOTOR_speed = 0};
  // Assign some value to right sensor so that it will be always greater than left sensor value, to cause left
  // steering
  sensor_left = max_sensor_value_to_start_steering;

  // case 3: left sensor value less than min sensor value mapped to steering angle
  sensor_right = min_sensor_value_to_map_max_steering_angle - 1;

  motor_signal = driver_logic__get_motor_command();
  TEST_ASSERT_LESS_OR_EQUAL((min_angle * -1), motor_signal.MOTOR_direction);
}
