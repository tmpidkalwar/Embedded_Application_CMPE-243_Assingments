#include "unity.h"

#include "queue_2A_lab.h"

queue_s queue_test;
void test_queue__init(void) {
  queue__init(&queue_test);
  TEST_ASSERT_EQUAL(0, queue_test.next_index);
  TEST_ASSERT_EQUAL(0, queue_test.size);
  TEST_ASSERT_EQUAL(0, queue_test.queue_memory[0]);
}

void test_queue__push_positive(void) {
  queue_s queue_push;
  queue_push.size = 40;
  queue_push.next_index = 0;
  bool result = queue__push(&queue_push, 10);
  TEST_ASSERT_EQUAL(1, queue_push.next_index);
  TEST_ASSERT_EQUAL(41, queue_push.size);
  TEST_ASSERT_EQUAL(10, queue_push.queue_memory[0]);
  TEST_ASSERT_TRUE(result);
}

void test_queue__push_negative(void) {
  queue_s queue_push;
  queue_push.size = 101;
  bool result = queue__push(&queue_push, 10);
  TEST_ASSERT_FALSE(result);
}

void test_shift_elements_in_queue(void) {
  queue_s queue;
  uint8_t value;
  queue.size = 2;
  queue__push(&queue, 10);
  queue__push(&queue, 44);

  shift_elements_in_queue(&queue);

  TEST_ASSERT_EQUAL(44, queue.queue_memory[0]);
}

void test_queue__pop_positive(void) {

  queue_s queue_pop;
  uint8_t value;
  queue_pop.size = 1;

  queue__push(&queue_pop, 10);

  bool result = queue__pop(&queue_pop, &value);
  TEST_ASSERT_EQUAL(10, value);
  TEST_ASSERT_TRUE(result);
}

void test_queue__pop_negative(void) {

  queue_s queue_pop;
  uint8_t value;
  queue_pop.size = 0;

  bool result = queue__pop(&queue_pop, &value);
  TEST_ASSERT_FALSE(result);
}

void test_queue__get_item_count(void) {
  queue_s queue_item;
  queue_item.size = 40;
  TEST_ASSERT_EQUAL(40, queue__get_item_count(&queue_item));

  for (int i = 1; i <= 20; i++)
    queue__push(&queue_item, 20);

  TEST_ASSERT_EQUAL(60, queue__get_item_count(&queue_item));
}
