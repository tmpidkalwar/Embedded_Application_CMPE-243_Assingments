#include "queue_2B_lab.h"
#include <stdio.h>
#include <string.h>

void queue__init_2(queue_s *queue, void *static_memory_for_queue, size_t static_memory_size_in_bytes) {

  queue->static_memory_for_queue = static_memory_for_queue;
  memset(queue->static_memory_for_queue, 0, (sizeof(uint8_t) * static_memory_size_in_bytes)); // Zero initialize
  queue->next_index = 0;
  queue->size = 0;
  queue->static_memory_size_in_bytes = static_memory_size_in_bytes;
}

bool queue__push_2(queue_s *queue, uint8_t push_value) {

  // Check to avoid overflow
  if (queue->size <= queue->static_memory_size_in_bytes) {
    queue->static_memory_for_queue[queue->next_index] = push_value;
    queue->next_index = queue->next_index + 1;
    queue->size = queue->size + 1;
    return true;
  } else
    return false;
}

void shift_elements_in_queue_2(queue_s *queue) {
  if (queue->size < queue->static_memory_size_in_bytes) {
    for (int i = 0; i < queue->size; i++)
      queue->static_memory_for_queue[i] = queue->static_memory_for_queue[i + 1];
  } else {
    for (int i = 0; i <= queue->static_memory_size_in_bytes - 2; i++)
      queue->static_memory_for_queue[i] = queue->static_memory_for_queue[i + 1];

    queue->static_memory_for_queue[queue->static_memory_size_in_bytes - 1] = 0;
  }
}

bool queue__pop_2(queue_s *queue, uint8_t *pop_value) {

  if (queue->size > 0) {
    // As queue is FIFO we will always pop the value at index 0
    *pop_value = queue->static_memory_for_queue[0];
    shift_elements_in_queue_2(queue);
    queue->size = queue->size - 1;

    if (queue->next_index > 0)
      queue->next_index = queue->next_index + 1;
    else
      queue->next_index = 0;
    return true;

  } else
    return false;
}

size_t queue__get_item_count_2(const queue_s *queue) { return queue->size; }