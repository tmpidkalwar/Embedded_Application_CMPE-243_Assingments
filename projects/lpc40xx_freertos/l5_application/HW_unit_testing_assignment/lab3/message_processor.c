#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#include "message_processor.h"

/**
 * This processes messages by calling message__read() until:
 *   - There are no messages to process -- which happens when message__read() returns false
 *   - At most 3 messages have been read
 */

const static size_t max_messages_to_process = 3;
bool message_processor(void) {
  message_s message;
  memset(&message, 0, sizeof(message));
  for (size_t message_count = 0; message_count < max_messages_to_process; message_count++) {
    if (!message__read(&message)) {
      return false;
      break;
    }
  }
}
bool process_message_with_dollar_sign(void) {
  message_s message;
  for (size_t message_count = 0; message_count < max_messages_to_process; message_count++) {
    if (message.data[0] == '$') {
      return true;
      break;
    } else {
      return false;
    }
  }
}
bool process_messages_without_any_dollar_sign(void) {
  message_s message;
  for (size_t message_count = 0; message_count < max_messages_to_process; message_count++) {
    if (message.data[0] != '$') {
      return true;
      break;
    } else {
      return false;
    }
  }
}