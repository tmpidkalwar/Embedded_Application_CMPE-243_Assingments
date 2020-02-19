#include "queue_2A_lab.h"
#include <stdio.h>
#include <string.h>

void queue__init(queue_s *queue) {

  memset(queue->queue_memory, 0, (sizeof(int) * 100)); // Zero initialize array
  queue->next_index = 0;
  queue->size = 0;
}

bool queue__push(queue_s *queue, uint8_t push_value) {

  if (queue->size <= 100) {
    queue->queue_memory[queue->next_index] = push_value;
    queue->next_index = queue->next_index + 1;
    queue->size = queue->size + 1;
    return true;
  } else
    return false;
}

void shift_elements_in_queue(queue_s *queue) {
  if (queue->size < 100) {
    for (int i = 0; i < queue->size; i++)
      queue->queue_memory[i] = queue->queue_memory[i + 1];
  } else {
    for (int i = 0; i <= 98; i++)
      queue->queue_memory[i] = queue->queue_memory[i + 1];

    queue->queue_memory[99] = 0;
  }
}

bool queue__pop(queue_s *queue, uint8_t *pop_value) {

  if (queue->size > 0) {
    // As queue is FIFO we will always pop the value at index 0
    *pop_value = queue->queue_memory[0];
    shift_elements_in_queue(queue);
    queue->size = queue->size - 1;

    if (queue->next_index > 0)
      queue->next_index = queue->next_index + 1;
    else
      queue->next_index = 0;
    return true;

  } else
    return false;
}

size_t queue__get_item_count(const queue_s *queue) { return queue->size; }