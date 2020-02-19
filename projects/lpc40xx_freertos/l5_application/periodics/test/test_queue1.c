#include "queue1.h"
#include "unity.h"
void test_queue__init(void) {
  queue_s qu;
  queue__init(&qu);
}
void test_queue__push(void) {
  queue_s que;
  que.size = 101;
  TEST_ASSERT_FALSE(queue__push(&que, 'a'));
}

void test_queue__pop(void) {
  queue_s queu;
  uint8_t *p_val;
  queu.size = 0;
  TEST_ASSERT_FALSE(queue__pop(&queu, p_val));
}

void test_queue__get_item_count(void) {
  queue_s qu1;
  qu1.count = 25;
  TEST_ASSERT_EQUAL(25, queue__get_item_count(&qu1));
}
