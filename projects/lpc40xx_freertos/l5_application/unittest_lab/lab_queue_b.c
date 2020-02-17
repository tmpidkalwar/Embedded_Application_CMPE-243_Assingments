#include "unity.h"

#include "lab_queue_b.h"

/* Initialize the queue with user provided static memory
 * @param static_memory_for_queue This memory pointer should not go out of scope
 */
void queue__init_b(queue_s *queue, void *static_memory_for_queue, size_t static_memory_size_in_bytes) {
  queue->static_memory_for_queue = &static_memory_size_in_bytes;
  queue->static_memory_size_in_bytes = static_memory_size_in_bytes;
  queue->count = 0;
}

/// @returns false if the queue is full
bool queue__push_b(queue_s *queue, uint8_t push_value) {
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
bool queue__pop_b(queue_s *queue, uint8_t *pop_value) {
  int count = queue->count;
  if (queue->count < 1) {
    return false;
  } else {
    queue->count--;
    uint8_t *memptr = queue->static_memory_for_queue;
    for (int i = 0; i < count - 1; i++) {
      memptr[i] = memptr[i + 1];
    }
    memptr[count - 1] = 0;

    return true;
  }
}

size_t queue__get_item_count_b(const queue_s *queue) { return queue->count; }