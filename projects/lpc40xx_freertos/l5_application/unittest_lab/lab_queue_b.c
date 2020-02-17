#include "unity.h"

#include "lab_queue_b.h"

/* Initialize the queue with user provided static memory
 * @param static_memory_for_queue This memory pointer should not go out of scope
 */
void queue__init(queue_s *queue, void *static_memory_for_queue, size_t static_memory_size_in_bytes) {
  queue->static_memory_for_queue = &static_memory_size_in_bytes;
  queue->static_memory_size_in_bytes = static_memory_size_in_bytes;
  queue->count = 0;
}

/// @returns false if the queue is full
bool queue__push(queue_s *queue, uint8_t push_value) {
  if (queue->count > queue->static_memory_size_in_bytes / sizeof(uint8_t) - 1) {
    return false;
  } else {
    queue->count++;
    uint8_t *memptr = queue->static_memory_for_queue;
    memptr[queue->count] = push_value;

    return true;
  }
}

/// @returns false if the queue was empty
bool queue__pop(queue_s *queue, uint8_t *pop_value) {
  if (queue->count < 1) {
    return false;
  } else {
    queue->count--;
    uint8_t *memptr = queue->static_memory_for_queue;
    for (int i = 0; i < queue->count - 1; i++) {
      memptr[i] = memptr[i + 1];
    }
    memptr[queue->count - 1] = 0;

    return true;
  }
}

size_t queue__get_item_count(const queue_s *queue) { return queue->count; }