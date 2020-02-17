#include "lab_queue.h"

void queue__init(queue_s *queue) {
  for (int i = 0; i < 100; i++) {
    queue->queue_memory[i] = 0;
  }
  queue->count = 0;
}

/// @returns false if the queue is full
bool queue__push(queue_s *queue, uint8_t push_value) {
  if (queue->count > 99) {
    return false;
  } else {
    queue->queue_memory[queue->count] = push_value;
    queue->count++;
    return true;
  }
}

/// @returns false if the queue was empty
bool queue__pop(queue_s *queue, uint8_t *pop_value) {
  if (queue->count < 1) {
    return false;
  } else {
    // pop_value = queue->queue_memory[0];
    for (int i = 0; i < queue->count - 1; i++) {
      queue->queue_memory[i] = queue->queue_memory[i + 1];
    }
    queue->queue_memory[queue->count - 1] = 0;
    queue->count--;
    return true;
  }
}

size_t queue__get_item_count(const queue_s *queue) {
  int count = queue->count;
  return queue->count;
}