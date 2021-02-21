#include "staticQueue_handler.h"

void staticQueue__init(queue_s *queue, void *static_memory_for_queue, size_t static_memory_size_in_bytes) {
  uint8_t *temporary = (uint8_t *)&static_memory_for_queue;
  for (int i = 0; i < static_memory_size_in_bytes; i++) {
    queue->static_memory_for_queue = temporary;
    temporary++;
  }
  queue->queue_element_count = 0;
  queue->static_memory_size_in_bytes = static_memory_size_in_bytes;
}

bool staticQueue__push(queue_s *queue, uint8_t push_value) {
  if (queue->queue_element_count < queue->static_memory_size_in_bytes) {
    queue->static_memory_for_queue[queue->queue_element_count] = push_value;
    queue->queue_element_count++;
    return true;
  }
  return false;
}

bool staticQueue__pop(queue_s *queue, uint8_t *pop_value_ptr) {
  if (queue->queue_element_count > 0) {
    *pop_value_ptr = queue->static_memory_for_queue[0];
    for (int i = 0; i < queue->queue_element_count - 1; i++) {
      queue->static_memory_for_queue[i] = queue->static_memory_for_queue[i + 1];
    }
    queue->queue_element_count--;
    return true;
  }
  return false;
}

size_t staticQueue__get_item_count(const queue_s *queue) { return queue->queue_element_count; }