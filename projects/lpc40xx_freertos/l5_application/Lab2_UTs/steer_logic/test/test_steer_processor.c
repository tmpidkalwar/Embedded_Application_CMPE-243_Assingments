#include "unity.h"

#include "steer_processor.h"

#include "Mocksteering.h"

void test_steer_processor__move_left(void) {
  steer_left_Expect();
  steer_processor(49, 51);
}

void test_steer_processor__move_right(void) {
  steer_right_Expect();
  steer_processor(51, 49);
}

void test_steer_processor__both_sensors_less_than_threshold(void) {
  uint32_t left_sensor = 40;
  uint32_t right_sensor = 49;
  if (left_sensor < right_sensor) {
    steer_right_Expect();
  } else if (left_sensor > right_sensor) {
    steer_left_Expect();
  } else {
  }
  steer_processor(left_sensor, right_sensor);
}

// Hint: If you do not setup an Expect()
// then this test will only pass none of the steer functions is called
void test_steer_processor__both_sensors_more_than_threshold(void) { steer_processor(60, 70); }

// Do not modify this test case
// Modify your implementation of steer_processor() to make it pass
// This tests corner case of both sensors below the threshold
void test_steer_processor(void) {
  steer_right_Expect();
  steer_processor(10, 20);

  steer_left_Expect();
  steer_processor(20, 10);
}
