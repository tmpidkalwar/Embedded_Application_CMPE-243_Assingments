#include "unity.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "lab_queue_2.h"

#define NO_OF_ELEMENTS 4
static uint8_t queue_buffer[NO_OF_ELEMENTS];
queue_t *queue_ptr = NULL;

void setUp(void) { queue_ptr = (queue_t *)malloc(sizeof(queue_t)); }

void tearDown(void) {
  free(queue_ptr->static_memory_for_queue);
  free(queue_ptr);
}

void test_queue__init_2b(void) {
  queue__init_2b(queue_ptr, queue_buffer, NO_OF_ELEMENTS);
  TEST_ASSERT_TRUE(queue_ptr);
  TEST_ASSERT_TRUE(queue_ptr->static_memory_for_queue);
  TEST_ASSERT_EQUAL_UINT8(NO_OF_ELEMENTS, queue_ptr->static_memory_size_in_bytes);
  TEST_ASSERT_EQUAL_UINT8(0, queue_ptr->num_of_elements_in_queue);
}

void test_queue__push_2b(void) {
  for (uint8_t i = 0; i < NO_OF_ELEMENTS; i++) {
    TEST_ASSERT_TRUE(queue__push_2b(queue_ptr, i));
  }
  TEST_ASSERT_EQUAL_UINT8(NO_OF_ELEMENTS, queue_ptr->num_of_elements_in_queue);
}

void test_queue__pop_2b(void) {
  test_queue__push_2b();
  uint8_t pop_value = 0;
  for (uint8_t i = NO_OF_ELEMENTS; i >= 1; i--) {
    TEST_ASSERT_TRUE(queue__pop_2b(queue_ptr, &pop_value));
    TEST_ASSERT_EQUAL_UINT8((NO_OF_ELEMENTS - i), pop_value);
  }
  TEST_ASSERT_EQUAL_UINT8(0, queue_ptr->num_of_elements_in_queue);
}

void test_queue__get_item_count_2b(void) {
  for (uint8_t i = 0; i < NO_OF_ELEMENTS; i++) {
    TEST_ASSERT_TRUE(queue__push_2b(queue_ptr, i));
  }
  TEST_ASSERT_EQUAL_UINT8(NO_OF_ELEMENTS, queue__get_item_count_2b(queue_ptr));
}
