#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#include "message_processor.h"

static bool message__analyze(message_s *message_to_read) {
  if (message_to_read->data[0] == '$') {
    message_to_read->data[1] = '^';
    return true;
  } else if (message_to_read->data[0] == '#') {
    message_to_read->data[1] = '%';
    return true;
  } else {
    return false;
  }
}

bool message_processor(void) {
  bool symbol_found = false;
  message_s message;
  memset(&message, 0, sizeof(message));

  const static size_t max_messages_to_process = 3;
  for (size_t message_count = 0; message_count < max_messages_to_process; message_count++) {
    if (!message__read(&message)) {
      break;
    } else if (message_count < 2) {
      // NOOP("Wait for last message");
    } else {
      message__analyze(&message);
    }
  }

  return symbol_found;
}
