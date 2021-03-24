#include "unity.h"

#include "highest.h"

void setup(void) {}

void teardown(void) {}

void test_highest_func_with_array_pointing_to_null(void) {
  const uint32_t *array = NULL;

  TEST_ASSERT_EQUAL(0, get_highest(array, 1));
}

void test_highest_func_with_array_size_zero(void) {
  const uint32_t *array;
  size_t array_size = 0;

  TEST_ASSERT_EQUAL(0, get_highest(array, array_size));
}

void test_highest_func_with_valid_array(void) {
  const uint32_t array[5] = {7, 9, 15, 1, 4};
  size_t array_size = 5;

  TEST_ASSERT_EQUAL(15, get_highest(array, array_size));
}

void test_highest_func_with_array_with_all_same_values(void) {
  const uint32_t array[4] = {0xFF, 0xFF, 0xFF, 0xFF};
  size_t array_size = 4;

  TEST_ASSERT_EQUAL(0xFF, get_highest(array, array_size));
}

void test_highest_func_with_uinitialized_array(void) {
  const uint32_t array[] = {};
  size_t array_size = sizeof(array);

  TEST_ASSERT_EQUAL(0, get_highest(array, array_size));
}