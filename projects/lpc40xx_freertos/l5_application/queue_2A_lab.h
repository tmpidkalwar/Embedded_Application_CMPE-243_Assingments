#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* In this code lab, the queue memory is statically defined
 * and fixed at compile time for 100 uint8s
 */
typedef struct {
  uint8_t queue_memory[100];
  uint8_t size;
  uint8_t next_index;
} queue_s;

void queue__init(queue_s *queue);

/// @returns false if the queue is full
bool queue__push(queue_s *queue, uint8_t push_value);

/// @returns false if the queue was empty
bool queue__pop(queue_s *queue, uint8_t *pop_value);

size_t queue__get_item_count(const queue_s *queue);

void shift_elements_in_queue(queue_s *queue);
