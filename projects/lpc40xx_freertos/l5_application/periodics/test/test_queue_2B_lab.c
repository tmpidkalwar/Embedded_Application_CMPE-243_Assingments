#include "unity.h"

#include "queue_2B_lab.h"
#include <stdbool.h>
#include <stdio.h>

static uint8_t test_array[200];

queue_s q;

void test_queue__init_2(void) {
  queue__init_2(&q, test_array, sizeof(test_array));
  TEST_ASSERT_EQUAL(0, q.next_index);
  TEST_ASSERT_EQUAL(0, q.size);
  TEST_ASSERT_EQUAL(sizeof(test_array), q.static_memory_size_in_bytes);
}

void test_queue__get_item_count_2(void) {
  queue_s queue_item;
  queue__init_2(&queue_item, test_array, sizeof(test_array));

  for (int i = 1; i <= 20; i++)
    queue__push_2(&queue_item, 20);

  TEST_ASSERT_EQUAL(20, queue__get_item_count_2(&queue_item));
}

void test_queue__push_positive_2(void) {
  queue_s queue_push;
  queue_push.size = 40;
  queue_push.static_memory_for_queue = test_array;
  queue_push.static_memory_size_in_bytes = sizeof(test_array);
  queue_push.next_index = 0;
  bool result = queue__push_2(&queue_push, 10);
  TEST_ASSERT_TRUE(queue__push_2(&queue_push, 10));

  TEST_ASSERT_EQUAL(2, queue_push.next_index);
  TEST_ASSERT_EQUAL(42, queue_push.size);
  TEST_ASSERT_EQUAL(10, queue_push.static_memory_for_queue[0]);
  TEST_ASSERT_TRUE(result);
}

void test_queue__pop_positive_2(void) {

  queue_s queue_pop;
  uint8_t value;
  queue_pop.static_memory_for_queue = test_array;

  queue__push_2(&queue_pop, 10);
  queue__push_2(&queue_pop, 10);
  queue__push_2(&queue_pop, 10);

  bool result = queue__pop_2(&queue_pop, &value);
  TEST_ASSERT_EQUAL(10, value);
  TEST_ASSERT_TRUE(result);
}

void test_queue__pop_negative_2(void) {

  queue_s queue_pop;
  uint8_t value;
  queue_pop.size = 0;
  queue_pop.static_memory_for_queue = test_array;
  queue__push_2(&queue_pop, 10);    // push one item
  queue__pop_2(&queue_pop, &value); // pop 2 times. must return false
  bool result = queue__pop_2(&queue_pop, &value);
  TEST_ASSERT_FALSE(result);
}

void test_shift_elements_in_queue_2(void) {
  queue_s queue;
  uint8_t value;
  queue.size = 0;
  queue.next_index = 0;
  queue.static_memory_for_queue = test_array;
  queue.static_memory_size_in_bytes = sizeof(test_array);
  queue__push_2(&queue, 10);
  queue__push_2(&queue, 44);

  shift_elements_in_queue_2(&queue);

  // After shift value at index zero should be 44
  TEST_ASSERT_EQUAL(44, queue.static_memory_for_queue[0]);
}