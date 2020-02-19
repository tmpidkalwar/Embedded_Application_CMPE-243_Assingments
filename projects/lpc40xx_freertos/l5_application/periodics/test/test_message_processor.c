#include "unity.h"

#include "Mockmessage.h"

#include "message_processor.c"

// This only tests if we process at most 3 messages
void test_process_3_messages(void) {
  message__read_ExpectAndReturn(NULL, true);
  message__read_IgnoreArg_message_to_read();

  message__read_ExpectAndReturn(NULL, true);
  message__read_IgnoreArg_message_to_read();

  message__read_ExpectAndReturn(NULL, true);
  message__read_IgnoreArg_message_to_read();

  // Since we did not return a message that starts with '$' this should return false
  TEST_ASSERT_FALSE(message_processor());
}

void test_third_msg(void) {
  message_s m;
  m.data[0] = '$';
  m.data[1] = '%';
  TEST_ASSERT_TRUE(third_message(&m));
  // m.data[0] = '#';
  // TEST_ASSERT_TRUE(third_message());
}

void test_process_message_with_dollar_sign(void) {
  message_s m;
  m.data[0] = '$';

  message__read_ExpectAndReturn(NULL, true);
  message__read_IgnoreArg_message_to_read();
  message__read_ReturnThruPtr_message_to_read(&m);

  message__read_ExpectAndReturn(NULL, true);
  message__read_IgnoreArg_message_to_read();
  message__read_ReturnThruPtr_message_to_read(&m);

  message__read_ExpectAndReturn(NULL, true);
  message__read_IgnoreArg_message_to_read();
  message__read_ReturnThruPtr_message_to_read(&m);

  TEST_ASSERT_TRUE(message_processor());
}

// Since we did not return a message that starts with '$' this should return false

void test_process_messages_without_any_dollar_sign(void) {
  message_s m;
  m.data[0] = 'c';

  message__read_ExpectAndReturn(NULL, true);
  message__read_IgnoreArg_message_to_read();
  message__read_ReturnThruPtr_message_to_read(&m);

  message__read_ExpectAndReturn(NULL, true);
  message__read_IgnoreArg_message_to_read();
  message__read_ReturnThruPtr_message_to_read(&m);

  message__read_ExpectAndReturn(NULL, true);
  message__read_IgnoreArg_message_to_read();
  message__read_ReturnThruPtr_message_to_read(&m);

  TEST_ASSERT_FALSE(message_processor());
}