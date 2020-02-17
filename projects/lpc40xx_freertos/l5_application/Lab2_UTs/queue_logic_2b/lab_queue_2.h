#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* In this code lab, the queue memory is statically defined
 * by the user and provided to you upon queue__init()
 */
typedef struct {
  uint8_t *static_memory_for_queue;
  uint8_t static_memory_size_in_bytes;
  uint8_t num_of_elements_in_queue;
} queue_t;

/* Initialize the queue with user provided static memory
 * @param static_memory_for_queue This memory pointer should not go out of scope
 */
void queue__init_2b(queue_t *queue, uint8_t *static_memory_for_queue, uint8_t static_memory_size_in_bytes);

/// @returns false if the queue is full
bool queue__push_2b(queue_t *queue, uint8_t push_value);

/// @returns false if the queue was empty
bool queue__pop_2b(queue_t *queue, uint8_t *pop_value);

uint8_t queue__get_item_count_2b(queue_t *queue);

queue_t *helper__shift_one_element_to_left_2b(queue_t *queue);
