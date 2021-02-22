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

void test_process_message_with_dollar_sign(void) {
  message_s dummy_message_1;
  memset(dummy_message_1.data, 'N', sizeof(dummy_message_1));
  message_s dummy_message_2;
  memset(dummy_message_2.data, 'M', sizeof(dummy_message_2));
  message_s message_3;
  memset(&message_3, 'O', sizeof(message_3));
  message_3.data[0] = '$';

  message__read_ExpectAndReturn(NULL, true);
  message__read_IgnoreArg_message_to_read();
  message__read_ReturnThruPtr_message_to_read(&dummy_message_1);

  message__read_ExpectAndReturn(NULL, true);
  message__read_IgnoreArg_message_to_read();
  message__read_ReturnThruPtr_message_to_read(&dummy_message_2);

  message__read_ExpectAndReturn(NULL, true);
  message__read_IgnoreArg_message_to_read();
  message__read_ReturnThruPtr_message_to_read(&message_3);

  TEST_ASSERT_TRUE(message_first_byte_checker(message_3));

  TEST_ASSERT_TRUE(message_processor());
}

void test_process_messages_without_any_dollar_or_pound_sign(void) {
  message_s dummy_message_1;
  memset(dummy_message_1.data, 'N', sizeof(dummy_message_1));
  message_s dummy_message_2;
  memset(dummy_message_2.data, 'M', sizeof(dummy_message_2));
  message_s message_3;
  memset(&message_3, 'O', sizeof(message_3));

  message__read_ExpectAndReturn(NULL, true);
  message__read_IgnoreArg_message_to_read();
  message__read_ReturnThruPtr_message_to_read(&dummy_message_1);

  message__read_ExpectAndReturn(NULL, true);
  message__read_IgnoreArg_message_to_read();
  message__read_ReturnThruPtr_message_to_read(&dummy_message_2);

  message__read_ExpectAndReturn(NULL, true);
  message__read_IgnoreArg_message_to_read();
  message__read_ReturnThruPtr_message_to_read(&message_3);

  TEST_ASSERT_FALSE(message_first_byte_checker(message_3));

  TEST_ASSERT_FALSE(message_processor());
}

void test_process_message_with_pound_sign(void) {
  message_s dummy_message_1;
  memset(dummy_message_1.data, 'N', sizeof(dummy_message_1));
  message_s dummy_message_2;
  memset(dummy_message_2.data, 'M', sizeof(dummy_message_2));
  message_s message_3;
  memset(&message_3, 'O', sizeof(message_3));
  message_3.data[0] = '#';

  message__read_ExpectAndReturn(NULL, true);
  message__read_IgnoreArg_message_to_read();
  message__read_ReturnThruPtr_message_to_read(&dummy_message_1);

  message__read_ExpectAndReturn(NULL, true);
  message__read_IgnoreArg_message_to_read();
  message__read_ReturnThruPtr_message_to_read(&dummy_message_2);

  message__read_ExpectAndReturn(NULL, true);
  message__read_IgnoreArg_message_to_read();
  message__read_ReturnThruPtr_message_to_read(&message_3);

  TEST_ASSERT_TRUE(message_first_byte_checker(message_3));

  TEST_ASSERT_TRUE(message_processor());
}

void test_process_message_with_pound_or_dollar_sign_in_non_third_message(void) {
  message_s dummy_message_1;
  memset(dummy_message_1.data, 'N', sizeof(dummy_message_1));
  dummy_message_1.data[0] = '$';
  message_s dummy_message_2;
  memset(dummy_message_2.data, 'M', sizeof(dummy_message_2));
  dummy_message_2.data[0] = '#';
  message_s message_3;
  memset(&message_3, 'O', sizeof(message_3));

  message__read_ExpectAndReturn(NULL, true);
  message__read_IgnoreArg_message_to_read();
  message__read_ReturnThruPtr_message_to_read(&dummy_message_1);

  message__read_ExpectAndReturn(NULL, true);
  message__read_IgnoreArg_message_to_read();
  message__read_ReturnThruPtr_message_to_read(&dummy_message_2);

  message__read_ExpectAndReturn(NULL, true);
  message__read_IgnoreArg_message_to_read();
  message__read_ReturnThruPtr_message_to_read(&message_3);

  TEST_ASSERT_FALSE(message_first_byte_checker(message_3));
  // Since we didn't return message with '$' or '#' sign as first byte
  // of third message, function should return FALSE
  TEST_ASSERT_FALSE(message_processor());
}