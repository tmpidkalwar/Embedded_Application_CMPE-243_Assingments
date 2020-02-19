#include "queue1.h"
void queue__init(queue_s *queue) {
  queue->front = queue->size = 0;
  queue->rear = 99;
  queue->count = 0;
}
bool queue__push(queue_s *queue, uint8_t push_value) {

  if (queue->size >= 100) {
    return false;
  } else {
    queue->rear = (queue->rear + 1) % 100;
    queue->queue_memory[queue->rear] = push_value;
    queue->size = queue->size + 1;
    queue->count++;
    return true;
  }
}
bool queue__pop(queue_s *queue, uint8_t *pop_value) {
  if (queue->size == 0) {
    return false;
  } else {
    *pop_value = queue->queue_memory[queue->front];
    queue->front = (queue->front + 1) % 100;
    queue->size = queue->size - 1;
    queue->count--;
    return true;
  }
}
size_t queue__get_item_count(const queue_s *queue) { return queue->count; }
