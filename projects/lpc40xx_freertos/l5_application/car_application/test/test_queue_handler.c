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

void test_queue_filling_fully_vacating_partially_again_filling_to_fullest(void) {
  const size_t max_queue_size = 100; // Change if needed
  queue_s queue;
  queue__init(&queue);
  size_t write_item = 0;

  for (write_item = 0; write_item < max_queue_size; write_item++) {
    TEST_ASSERT_TRUE(queue__push(&queue, write_item));
    TEST_ASSERT_EQUAL(write_item + 1, queue__get_item_count(&queue));
  }

  // Partially vacate the queue by popping elements
  uint8_t popped_value = 0;
  size_t read_item = 0;
  // Pull and verify the FIFO order
  for (read_item = 0; read_item < 50; read_item++) {
    TEST_ASSERT_TRUE(queue__pop(&queue, &popped_value));
    TEST_ASSERT_EQUAL(read_item, popped_value);
  }

  // Now again start pushing elements to max queue limit
  for (; write_item < max_queue_size + 50; write_item++) {
    TEST_ASSERT_TRUE(queue__push(&queue, write_item));
    TEST_ASSERT_EQUAL(write_item + 1 - 50, queue__get_item_count(&queue));
  }

  // Pull and verify the FIFO order
  for (; read_item < max_queue_size + 50; read_item++) {
    TEST_ASSERT_TRUE(queue__pop(&queue, &popped_value));
    TEST_ASSERT_EQUAL(read_item, popped_value);
  }
}