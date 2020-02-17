#include "lab_queue.h"

void queue__init(queue_s *queue) {
  queue->head = -1;
  queue->tail = -1;
}

/// @returns false if the queue is full
bool queue__push(queue_s *queue, uint8_t push_value) {
  if (100 == queue->tail) { // Overflow
    return 0;
  } else if (-1 == queue->tail) { // First Element
    queue->head++;
  }
  queue->tail++;
  queue->queue_memory[queue->tail] = push_value;
  return 1;
}

/// @returns false if the queue was empty
bool queue__pop(queue_s *queue, uint8_t *pop_value) {
  if ((-1 == queue->head) && (-1 == queue->tail)) { // Underflow
    return 0;
  } else if (!(queue->head || queue->tail)) { // Last Element
    *pop_value = queue->queue_memory[queue->head];
    queue->tail = -1;
    queue->head = -1;
    return 1;
  } else {
    *pop_value = queue->queue_memory[queue->head];
    queue = (queue_s *)helper__shift_one_element_to_left(queue);
    queue->tail--;
    return 1;
  }
}

int8_t queue__get_item_count(queue_s *queue) { return queue->tail + 1; }

queue_s *helper__shift_one_element_to_left(queue_s *queue) {
  for (int i = 0; i < queue->tail; i++) {
    queue->queue_memory[i] = queue->queue_memory[i + 1];
  }
  return queue;
}
