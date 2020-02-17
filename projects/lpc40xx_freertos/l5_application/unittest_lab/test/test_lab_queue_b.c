#include "unity.h"

#include "lab_queue_b.h"

void test_queue__init_b() {
  queue_s q;
  uint8_t array[8];
  size_t size = sizeof(uint8_t) * 8;
  queue__init_b(&q, &array, size);

  TEST_ASSERT_EQUAL_INT(0, q.count);
}

void test_queue__push_b() {
  queue_s q;
  uint8_t array[8];
  size_t size = sizeof(uint8_t) * 8;
  queue__init_b(&q, &array, size);

  bool result = queue__push_b(&q, 1);
  TEST_ASSERT_TRUE(result);
  TEST_ASSERT_EQUAL_INT(1, q.count);
}

void test_queue__push_on_full_queue_b() {
  queue_s q;
  uint8_t array[8];
  size_t size = sizeof(uint8_t) * 8;
  queue__init_b(&q, &array, size);
  bool result;
  for (int i = 0; i < 8; i++) {
    queue__push_b(&q, i);
  }
  result = queue__push_b(&q, 1);
  TEST_ASSERT_FALSE(result)
  TEST_ASSERT_EQUAL_INT(8, q.count);
}

void test_queue__pop_b() {
  queue_s q;
  uint8_t array2[8];
  size_t size = sizeof(uint8_t) * 8;
  queue__init_b(&q, &array2, size);
  bool result;
  uint8_t pop_value;
  for (int i = 0; i < 4; i++) {
    queue__push_b(&q, i);
  }
  result = queue__pop_b(&q, &pop_value);
  TEST_ASSERT_TRUE(result);
  TEST_ASSERT_EQUAL_INT(3, q.count);
}

void test_queue__pop_empty_queue_b() {
  queue_s q;
  uint8_t array[8];
  size_t size = sizeof(uint8_t) * 8;
  queue__init_b(&q, &array, size);
  bool result;
  uint8_t pop_value;
  result = queue__pop_b(&q, &pop_value);
  TEST_ASSERT_FALSE(result);
  TEST_ASSERT_EQUAL_INT(0, q.count);
}

void test_queue__get_item_count_b() {
  queue_s q;
  uint8_t array[8];
  size_t size = sizeof(uint8_t) * 8;
  queue__init_b(&q, &array, size);

  for (int i = 0; i < 2; i++) {
    queue__push_b(&q, i);
  }
  int queue_size = queue__get_item_count_b(&q);
  TEST_ASSERT_EQUAL_INT(2, queue_size);
}

void test_queue__push_then_pop_b() {
  queue_s q;
  uint8_t array[8];
  size_t size = sizeof(uint8_t) * 8;
  queue__init_b(&q, &array, size);
  for (int i = 0; i < 5; i++) {
    queue__push_b(&q, i);
  }
  for (int j = 0; j < 2; j++) {
    queue__pop_b(&q, j);
  }
  TEST_ASSERT_EQUAL_INT(3, q.count);
}