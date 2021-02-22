#include "staticQueue_handler.h"

void staticQueue__init(queue_s *queue, void *static_memory_for_queue, size_t static_memory_size_in_bytes) {
  uint8_t *temporary = (uint8_t *)&static_memory_for_queue;
  for (int i = 0; i < static_memory_size_in_bytes; i++) {
    queue->static_memory_for_queue = temporary;
    temporary++;
  }
  queue->queue_element_count = 0;
  queue->static_memory_size_in_bytes = static_memory_size_in_bytes;
  queue->read_index = 0;
  queue->write_index = 0;
}

bool staticQueue__push(queue_s *queue, uint8_t push_value) {
  if (queue->queue_element_count < queue->static_memory_size_in_bytes) {
    queue->static_memory_for_queue[queue->write_index] = push_value;
    queue->queue_element_count++;
    queue->write_index = (queue->write_index + 1) % queue->static_memory_size_in_bytes;
    return true;
  }
  return false;
}

bool staticQueue__pop(queue_s *queue, uint8_t *pop_value_ptr) {
  if (queue->queue_element_count > 0) {
    *pop_value_ptr = queue->static_memory_for_queue[queue->read_index];
    queue->read_index = (queue->read_index + 1) % queue->static_memory_size_in_bytes;
    queue->queue_element_count--;
    return true;
  }
  return false;
}

size_t staticQueue__get_item_count(const queue_s *queue) { return queue->queue_element_count; }