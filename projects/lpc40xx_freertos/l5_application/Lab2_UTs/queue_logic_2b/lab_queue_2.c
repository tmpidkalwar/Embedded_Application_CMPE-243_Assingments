#include "lab_queue_2.h"
#include <stdlib.h>

/* Initialize the queue with user provided static memory
 * @param static_memory_for_queue This memory pointer should not go out of scope
 */
void queue__init_2b(queue_t *queue, uint8_t *static_memory_for_queue, uint8_t num_of_bytes) {
  queue->static_memory_for_queue = static_memory_for_queue;
  queue->static_memory_size_in_bytes = num_of_bytes;
  queue->num_of_elements_in_queue = 0;
}

/// @returns false if the queue is full
bool queue__push_2b(queue_t *queue, uint8_t push_value) {
  if (queue->num_of_elements_in_queue == queue->static_memory_size_in_bytes) { // Overflow
    return 0;
  } else {
    queue->static_memory_for_queue[queue->num_of_elements_in_queue++] = push_value;
    return 1;
  }
}

/// @returns false if the queue was empty
bool queue__pop_2b(queue_t *queue, uint8_t *pop_value) {
  if (0 == queue->num_of_elements_in_queue) { // Underflow
    return 0;
  } else {
    *pop_value = queue->static_memory_for_queue[0];
    helper__shift_one_element_to_left_2b(queue);
    queue->num_of_elements_in_queue--;
    return 1;
  }
}

uint8_t queue__get_item_count_2b(queue_t *queue) { return queue->num_of_elements_in_queue; }

queue_t *helper__shift_one_element_to_left_2b(queue_t *queue) {
  for (int i = 0; i < queue->num_of_elements_in_queue; i++) {
    queue->static_memory_for_queue[i] = queue->static_memory_for_queue[i + 1];
  }
  return queue;
}