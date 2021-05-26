#include "unity.h"

#include "solution.h"

void setup(void) {}

void test_get_matching_index_array_input_NULL(void) {
  gps_t **gps_array = NULL;
  gps_t *compare = {0};
  TEST_ASSERT_EQUAL(-2, get_matching_index(gps_array, 2, compare));
}

void test_get_matching_index_caomparing_input_NULL(void) {
  gps_t **gps_array = {0};
  gps_t *compare = NULL;
  TEST_ASSERT_EQUAL(-2, get_matching_index(gps_array, 2, compare));
}

void test_get_matching_index_one_of_array_input_NULL(void) {
  gps_t *gps_array[2];
  gps_t gps_array_elem1 = {.latitude = 0.443, .longitude = 0.54};
  gps_array[0] = &gps_array_elem1;
  gps_array[1] = NULL;
  gps_t *compare = {0};
  TEST_ASSERT_EQUAL(-2, get_matching_index(gps_array, 2, compare));
}

void test_get_matching_index_not_found_in_array(void) {
  static const size_t elem_count = 3;
  gps_t *gps_array[elem_count];
  gps_t gps_array_elem1 = {.latitude = 0.443, .longitude = 0.54};
  gps_t gps_array_elem2 = {.latitude = 123.7, .longitude = 12.08};
  gps_t gps_array_elem3 = {.latitude = 1.2, .longitude = 3.5};
  gps_array[0] = &gps_array_elem1;
  gps_array[1] = &gps_array_elem2;
  gps_array[2] = &gps_array_elem3;
  gps_t compare = {.latitude = 0, .longitude = 0};
  TEST_ASSERT_EQUAL(-1, get_matching_index(gps_array, elem_count, &compare));
}

void test_get_matching_index_found_in_array(void) {
  static const size_t elem_count = 3;
  gps_t *gps_array[elem_count];
  gps_t gps_array_elem1 = {.latitude = 0.443, .longitude = 0.54};
  gps_t gps_array_elem2 = {.latitude = 123.7, .longitude = 12.08};
  gps_t gps_array_elem3 = {.latitude = 1.2, .longitude = 3.5};
  gps_array[0] = &gps_array_elem1;
  gps_array[1] = &gps_array_elem2;
  gps_array[2] = &gps_array_elem3;
  gps_t compare = {.latitude = 123.7, .longitude = 12.08};
  TEST_ASSERT_EQUAL(1, get_matching_index(gps_array, elem_count, &compare));
}