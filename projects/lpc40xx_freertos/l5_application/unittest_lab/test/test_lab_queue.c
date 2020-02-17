#include "unity.h"

#include "lab_queue.h"

void test_queue__init() {
  queue_s q;
  queue__init(&q);

  TEST_ASSERT_EQUAL_INT(0, q.count);
  TEST_ASSERT_EACH_EQUAL_UINT8(0, q.queue_memory, 100);
}

void test_queue__push() {
  queue_s q;
  bool result = queue__push(&q, 1);

  TEST_ASSERT_EQUAL_INT(1, q.count);
  TEST_ASSERT_EQUAL_INT(1, q.queue_memory[0]);
  TEST_ASSERT_TRUE(result);
}

void test_queue__push_on_full_queue() {
  queue_s q;
  bool result;
  for (int i = 0; i < 100; i++) {
    queue__push(&q, i);
  }
  result = queue__push(&q, 100);
  TEST_ASSERT_FALSE(result)
  TEST_ASSERT_EQUAL_INT(99, q.queue_memory[99]);
  TEST_ASSERT_EQUAL_INT(100, q.count);
}

void test_queue__pop() {
  queue_s q;
  bool result;
  uint8_t pop_value;
  for (int i = 0; i < 100; i++) {
    queue__push(&q, i);
  }
  result = queue__pop(&q, &pop_value);
  TEST_ASSERT_TRUE(result);
  TEST_ASSERT_EQUAL_INT(99, q.count);
}

void test_queue__pop_empty_queue() {
  queue_s q;
  bool result;
  uint8_t pop_value;
  result = queue__pop(&q, &pop_value);
  TEST_ASSERT_FALSE(result);
  TEST_ASSERT_EQUAL_INT(0, q.count);
}

void test_queue__get_item_count() {
  queue_s q;

  for (int i = 0; i < 50; i++) {
    queue__push(&q, i);
  }
  int queue_size = queue__get_item_count(&q);
  TEST_ASSERT_EQUAL_size_t(50, queue_size);
}

void test_queue__push_then_pop() {
  queue_s q;
  for (int i = 0; i < 20; i++) {
    queue__push(&q, i);
  }
  for (int j = 0; j < 5; j++) {
    queue__pop(&q, (uint8_t)j);
  }
  TEST_ASSERT_EQUAL_INT(15, q.count);
}