#include "unity.h"

#include "set_bit_counter.h"

void setup(void) {}

void teardown(void) {}

void test_set_bit_counter_for_zero(void) {
  uint32_t value = 0;

  TEST_ASSERT_EQUAL(0, count_bits(value));
}

void test_set_bit_counter_for_valid_number(void) {
  uint32_t value = 0b11100111001101;

  const uint32_t exp_ans = 9;

  TEST_ASSERT_EQUAL(exp_ans, count_bits(value));
}