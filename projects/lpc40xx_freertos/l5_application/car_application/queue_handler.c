#include "queue_handler.h"

#include "string.h"

void queue__init(queue_s *queue) {
  memset(queue->queue_memory, 0, 100);
  queue->queue_elements_count = 0;
}

bool queue__push(queue_s *queue, uint8_t push_value) {
  if (queue->queue_elements_count < 100) {
    queue->queue_memory[queue->queue_elements_count] = push_value;
    queue->queue_elements_count++;
    return true;
  }
  return false;
}

bool queue__pop(queue_s *queue, uint8_t *pop_value) {
  if (queue->queue_elements_count > 0) {
    *pop_value = queue->queue_memory[0];
    for (int i = 0; i < queue->queue_elements_count - 1; i++) {
      queue->queue_memory[i] = queue->queue_memory[i + 1];
    }
    queue->queue_elements_count--;

    return true;
  }
  return false;
}

size_t queue__get_item_count(const queue_s *queue) { return queue->queue_elements_count; }