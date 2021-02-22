#include "queue_handler.h"

#include "string.h"

void queue__init(queue_s *queue) {
  memset(queue->queue_memory, 0, 100);
  queue->queue_elements_count = 0;
  queue->read_index = 0;
  queue->write_index = 0;
}

bool queue__push(queue_s *queue, uint8_t push_value) {
  if (queue->queue_elements_count < 100) {
    queue->queue_memory[queue->write_index] = push_value;
    queue->write_index = (queue->write_index + 1) % 100;
    queue->queue_elements_count++;
    return true;
  }
  return false;
}

bool queue__pop(queue_s *queue, uint8_t *pop_value) {
  if (queue->queue_elements_count > 0) {
    *pop_value = queue->queue_memory[queue->read_index];
    queue->queue_elements_count--;
    queue->read_index = (queue->read_index + 1) % 100;

    return true;
  }
  return false;
}

size_t queue__get_item_count(const queue_s *queue) { return queue->queue_elements_count; }