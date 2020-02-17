#include "unity.h"

#include "lab_queue_b.h"

void test_queue__init() {
  queue_s q;
  uint8_t array[8];
  size_t size = sizeof(uint8_t) * 8;
  queue__init(&q, &array, size);

  TEST_ASSERT_EQUAL_INT(0, q.count);
}

void test_queue__push() {
  queue_s q;
  uint8_t array[8];
  size_t size = sizeof(uint8_t) * 8;
  queue__init(&q, &array, size);

  bool result = queue__push(&q, 1);
  TEST_ASSERT_TRUE(result);
  TEST_ASSERT_EQUAL_INT(1, q.count);
}

void test_queue__push_on_full_queue() {
  queue_s q;
  uint8_t array[8];
  size_t size = sizeof(uint8_t) * 8;
  queue__init(&q, &array, size);
  bool result;
  for (int i = 0; i < 8; i++) {
    queue__push(&q, i);
  }
  result = queue__push(&q, 1);
  TEST_ASSERT_FALSE(result)
  TEST_ASSERT_EQUAL_INT(8, q.count);
}

void test_queue__pop() {
  queue_s q;
  uint8_t array[8];
  size_t size = sizeof(uint8_t) * 8;
  queue__init(&q, &array, size);
  bool result;
  uint8_t pop_value;
  for (int i = 0; i < 4; i++) {
    queue__push(&q, i);
  }
  result = queue__pop(&q, &pop_value);
  TEST_ASSERT_TRUE(result);
  TEST_ASSERT_EQUAL_INT(3, q.count);
}

void test_queue__pop_empty_queue() {
  queue_s q;
  uint8_t array[8];
  size_t size = sizeof(uint8_t) * 8;
  queue__init(&q, &array, size);
  bool result;
  uint8_t pop_value;
  result = queue__pop(&q, &pop_value);
  TEST_ASSERT_FALSE(result);
  TEST_ASSERT_EQUAL_INT(0, q.count);
}

void test_queue__get_item_count() {
  queue_s q;
  uint8_t array[8];
  size_t size = sizeof(uint8_t) * 8;
  queue__init(&q, &array, size);

  for (int i = 0; i < 2; i++) {
    queue__push(&q, i);
  }
  int queue_size = queue__get_item_count(&q);
  TEST_ASSERT_EQUAL_INT(2, queue_size);
}

void test_queue__push_then_pop() {
  queue_s q;
  uint8_t array[8];
  size_t size = sizeof(uint8_t) * 8;
  queue__init(&q, &array, size);
  for (int i = 0; i < 5; i++) {
    queue__push(&q, i);
  }
  for (int j = 0; j < 2; j++) {
    queue__pop(&q, j);
  }
  TEST_ASSERT_EQUAL_INT(3, q.count);
}