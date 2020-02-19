#include "unity.h"

#include "Mockmessage.h"

#include "message_processor.c"
#include <string.h>

extern bool check_dollar(message_s *message);

// This only tests if we process at most 3 messages
void test_process_3_messages_positive(void) {

  message_s message1;
  message_s message2;
  message_s message3;

  strcpy(message1.data, "Test 1");
  strcpy(message2.data, "Test 2");
  strcpy(message3.data, "Test 3");

  message__read_ExpectAndReturn(NULL, true);
  message__read_IgnoreArg_message_to_read();
  message__read_ReturnThruPtr_message_to_read(&message1);

  message__read_ExpectAndReturn(NULL, true);
  message__read_IgnoreArg_message_to_read();
  message__read_ReturnThruPtr_message_to_read(&message2);

  message__read_ExpectAndReturn(NULL, true);
  message__read_IgnoreArg_message_to_read();
  message__read_ReturnThruPtr_message_to_read(&message3);

  // Since we did not return a message that starts with '$' this should return false
  TEST_ASSERT_FALSE(message_processor());
}

void test_process_3_messages_negative(void) {

  message_s message1;
  message_s message2;
  message_s message3;

  strcpy(message1.data, "Test 1");
  strcpy(message2.data, "Test 2");
  strcpy(message3.data, "$ Test");

  message__read_ExpectAndReturn(NULL, true);
  message__read_IgnoreArg_message_to_read();
  message__read_ReturnThruPtr_message_to_read(&message1);

  message__read_ExpectAndReturn(NULL, true);
  message__read_IgnoreArg_message_to_read();
  message__read_ReturnThruPtr_message_to_read(&message2);

  message__read_ExpectAndReturn(NULL, true);
  message__read_IgnoreArg_message_to_read();
  message__read_ReturnThruPtr_message_to_read(&message3);

  // Since we did not return a message that starts with '$' this should return false
  TEST_ASSERT_TRUE(message_processor());
}

bool stub_check_dollar(message_s *message_to_read) {
  char test_data[] = "$ Test";
  memcpy(message_to_read->data, test_data, sizeof(message_to_read->data));
  return true;
}

bool stub_check_no_dollar(message_s *message_to_read) {
  char test_data[] = "Test";
  memcpy(message_to_read->data, test_data, sizeof(message_to_read->data));
  return false;
}

void test_process_message_with_dollar_sign(void) {
  message__read_StubWithCallback(stub_check_dollar);
  TEST_ASSERT_TRUE(message_processor());
}

void test_process_messages_without_any_dollar_sign(void) {
  message__read_StubWithCallback(stub_check_no_dollar);
  TEST_ASSERT_FALSE(message_processor());
}

void test_static_function_to_check_dollar(void) {
  message_s test;
  strcpy(test.data, "$ Unit");

  TEST_ASSERT_TRUE(check_dollar(&test));
}

void test_static_function_to_check_dollar_negative(void) {
  message_s test;
  strcpy(test.data, "Unit");

  TEST_ASSERT_FALSE(check_dollar(&test));
}
