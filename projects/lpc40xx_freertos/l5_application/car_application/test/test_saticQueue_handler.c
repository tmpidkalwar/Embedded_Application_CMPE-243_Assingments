#include "unity.h"

#include "staticQueue_handler.h"

void test_staticQueue(void) {
  uint32_t memory_chunk_for_queue[120];
  queue_s queue;
  const size_t static_queue_size_in_bytes = 120;
  staticQueue__init(&queue, (void *)&memory_chunk_for_queue, static_queue_size_in_bytes);

  // Fill the queue to maximum alloted size
  for (size_t item = 0; item < static_queue_size_in_bytes; item++) {
    TEST_ASSERT_TRUE(staticQueue__push(&queue, item));
    TEST_ASSERT_EQUAL(item + 1, staticQueue__get_item_count(&queue));
  }

  // Try filling one more element beyond the alloted size
  //  TEST_ASSERT_TRUE(staticQueue__push(&queue, 189));
  TEST_ASSERT_FALSE(staticQueue__push(&queue, 189));
  TEST_ASSERT_EQUAL(static_queue_size_in_bytes, staticQueue__get_item_count(&queue));

  uint8_t popped_element = 0;
  // Pull and verify the elements from Queue in FIFO order
  for (size_t item = 0; item < static_queue_size_in_bytes; item++) {
    TEST_ASSERT_TRUE(staticQueue__pop(&queue, &popped_element));
    TEST_ASSERT_EQUAL(item, popped_element);
  }

  TEST_ASSERT_EQUAL(0, staticQueue__get_item_count(&queue));
  TEST_ASSERT_FALSE(staticQueue__pop(&queue, &popped_element));
}