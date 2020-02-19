#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* In this code lab, the queue memory is statically defined
 * by the user and provided to you upon queue__init()
 */
typedef struct {
  uint8_t *static_memory_for_queue;
  size_t static_memory_size_in_bytes;
  uint8_t size;
  uint8_t next_index;
} queue_s;

/* Initialize the queue with user provided static memory
 * @param static_memory_for_queue This memory pointer should not go out of scope
 */
void queue__init_2(queue_s *queue, void *static_memory_for_queue, size_t static_memory_size_in_bytes);

/// @returns false if the queue is full
bool queue__push_2(queue_s *queue, uint8_t push_value);

/// @returns false if the queue was empty
bool queue__pop_2(queue_s *queue, uint8_t *pop_value);

size_t queue__get_item_count_2(const queue_s *queue);

void shift_elements_in_queue_2(queue_s *queue);
