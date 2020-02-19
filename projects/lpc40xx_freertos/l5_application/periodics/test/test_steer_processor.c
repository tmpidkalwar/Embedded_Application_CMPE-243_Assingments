#include "unity.h"

#include "steer_processor.h"

#include "Mocksteering.h"

void test_steer_processor__move_right(void) {
  // Then
  steer_right_Expect();

  // When
  steer_processor(49, 51);
}

void test_steer_processor__move_left(void) {
  // Then
  steer_left_Expect();

  // When
  steer_processor(51, 49);
}

void test_steer_processor__both_sensors_less_than_threshold(void) {
  // Then1
  steer_right_Expect();
  // When1
  steer_processor(10, 20);
  // Then2
  steer_left_Expect();
  // When2
  steer_processor(20, 10);
}

// Hint: If you do not setup an Expect()
// then this test will only pass none of the steer functions is called
void test_steer_processor__both_sensors_more_than_threshold(void) { steer_processor(51, 51); }
