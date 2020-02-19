#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct {
  uint8_t *static_memory_for_queue;
  size_t static_memory_size_in_bytes;
  int front, rear, size, count1, capacity;
  // TODO: Add more members as needed
} queue_s;

/* Initialize the queue with user provided static memory
 * @param static_memory_for_queue This memory pointer should not go out of scope
 */
void queue__init1(queue_s *queue, uint8_t *static_memory_for_queue, size_t static_memory_size_in_bytes);

/// @returns false if the queue is full
bool queue__push1(queue_s *queue, uint8_t push_value);

/// @returns false if the queue was empty
bool queue__pop1(queue_s *queue, uint8_t *pop_value);

size_t queue__get_item_count1(const queue_s *queue);
