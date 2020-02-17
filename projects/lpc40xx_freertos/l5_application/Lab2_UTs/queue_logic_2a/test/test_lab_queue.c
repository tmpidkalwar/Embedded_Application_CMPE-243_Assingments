#include "unity.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "lab_queue.h"

queue_s *queue_ptr = NULL;

void setUp(void) { queue_ptr = (queue_s *)malloc(sizeof(queue_s)); }

void tearDown(void) {
  queue__init(queue_ptr);
  free(queue_ptr);
}

void test_queue__init(void) {
  queue__init(queue_ptr);
  TEST_ASSERT_TRUE(queue_ptr);
  TEST_ASSERT_EQUAL_UINT8(100, (sizeof(queue_ptr->queue_memory) / sizeof(queue_ptr->queue_memory[0])));
  TEST_ASSERT_EQUAL_INT8(-1, queue_ptr->head);
  TEST_ASSERT_EQUAL_INT8(-1, queue_ptr->tail);
}

void test_queue__push(void) {
  for (uint8_t i = 0; i < 100; i++) {
    TEST_ASSERT_TRUE(queue__push(queue_ptr, i));
  }
  TEST_ASSERT_EQUAL_INT8(0, queue_ptr->head);
  TEST_ASSERT_EQUAL_INT8(99, queue_ptr->tail);
}

void test_queue__pop(void) {
  // repopulate the queue
  test_queue__push();

  uint8_t pop_value = 0;
  for (uint8_t i = 100; i >= 1; i--) {
    TEST_ASSERT_TRUE(queue__pop(queue_ptr, &pop_value));
    TEST_ASSERT_EQUAL_UINT8((100 - i), pop_value);
  }
  TEST_ASSERT_EQUAL_INT8(-1, queue_ptr->head);
  TEST_ASSERT_EQUAL_INT8(-1, queue_ptr->tail);
}

void test_queue__get_item_count(void) {
  TEST_ASSERT_EQUAL_INT8(-1, queue_ptr->head);
  TEST_ASSERT_EQUAL_INT8(-1, queue_ptr->tail);
  for (int8_t i = 0; i < 50; i++) {
    TEST_ASSERT_TRUE(queue__push(queue_ptr, i));
  }
  TEST_ASSERT_EQUAL_UINT8(50, queue__get_item_count(queue_ptr));
}
