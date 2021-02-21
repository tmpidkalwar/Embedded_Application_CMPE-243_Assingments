#include "unity.h"

#include "queue_handler.h"

void test_comprehensive(void) {
  const size_t max_queue_size = 100; // Change if needed
  queue_s queue;
  queue__init(&queue);

  for (size_t item = 0; item < max_queue_size; item++) {
    TEST_ASSERT_TRUE(queue__push(&queue, item));
    TEST_ASSERT_EQUAL(item + 1, queue__get_item_count(&queue));
  }

  // Should not be able to push anymore
  TEST_ASSERT_FALSE(queue__push(&queue, 123));
  TEST_ASSERT_EQUAL(max_queue_size, queue__get_item_count(&queue));

  uint8_t popped_value = 0;
  // Pull and verify the FIFO order
  for (size_t item = 0; item < max_queue_size; item++) {
    TEST_ASSERT_TRUE(queue__pop(&queue, &popped_value));
    TEST_ASSERT_EQUAL(item, popped_value);
  }

  TEST_ASSERT_EQUAL(0, queue__get_item_count(&queue));
  TEST_ASSERT_FALSE(queue__pop(&queue, &popped_value));
}