#include "queue2.h"
#include "unity.h"
void test_queue__init1(void) {
  static uint8_t memory[128];
  queue_s qu;
  queue__init1(&qu, memory, sizeof(memory));
}
void test_queue__push1(void) {
  queue_s que;
  que.capacity = 100;
  que.size = 101;
  TEST_ASSERT_FALSE(queue__push1(&que, 'a'));
}

void test_queue__pop1(void) {
  queue_s queu;
  uint8_t *p_val;
  queu.size = 0;
  TEST_ASSERT_FALSE(queue__pop1(&queu, p_val));
}

void test_queue__get_item_count1(void) {
  queue_s qu1;
  qu1.count1 = 25;
  TEST_ASSERT_EQUAL(25, queue__get_item_count1(&qu1));
}
