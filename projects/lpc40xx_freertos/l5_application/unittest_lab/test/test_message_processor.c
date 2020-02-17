#include "unity.h"

#include "Mockmessage.h"

#include "message_processor.c"

void test_process_3_messages(void) {
  message__read_ExpectAndReturn(NULL, true);
  message__read_IgnoreArg_message_to_read();

  message__read_ExpectAndReturn(NULL, true);
  message__read_IgnoreArg_message_to_read();

  message__read_ExpectAndReturn(NULL, true);
  message__read_IgnoreArg_message_to_read();

  TEST_ASSERT_EQUAL(0, message_processor());
}

void test_process_messages__message__read_false() {
  message__read_IgnoreAndReturn(false);
  TEST_ASSERT_EQUAL(0, message_processor());
}

void test_message__analyze_valid_message() {
  message_s message;
  message.data[0] = '$';
  message.data[1] = 'a';
  bool result = message__analyze(&message);
  TEST_ASSERT_TRUE(result);
}