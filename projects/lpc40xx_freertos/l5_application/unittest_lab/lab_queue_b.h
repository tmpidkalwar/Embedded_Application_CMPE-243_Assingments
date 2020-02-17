#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* In this code lab, the queue memory is statically defined
 * by the user and provided to you upon queue__init()
 */
typedef struct {
  void *static_memory_for_queue;
  size_t static_memory_size_in_bytes;

  // TODO: Add more members as needed
  int count;
} queue_s;

/* Initialize the queue with user provided static memory
 * @param static_memory_for_queue This memory pointer should not go out of scope
 */
void queue__init(queue_s *queue, void *static_memory_for_queue, size_t static_memory_size_in_bytes);

/// @returns false if the queue is full
bool queue__push(queue_s *queue, uint8_t push_value);

/// @returns false if the queue was empty
bool queue__pop(queue_s *queue, uint8_t *pop_value);

size_t queue__get_item_count(const queue_s *queue);
