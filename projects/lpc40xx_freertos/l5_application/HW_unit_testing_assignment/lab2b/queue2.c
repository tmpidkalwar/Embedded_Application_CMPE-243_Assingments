#include "queue2.h"
void queue__init1(queue_s *queue, uint8_t *static_memory_for_queue, size_t static_memory_size_in_bytes) {
  static_memory_for_queue = (uint8_t *)malloc(static_memory_size_in_bytes);
  queue->capacity = static_memory_size_in_bytes;
  queue->front = queue->size = 0;
  queue->rear = queue->capacity - 1;
  queue->count1 = 0;
}
bool queue__push1(queue_s *queue, uint8_t push_value) {
  if (queue->size >= queue->capacity) {
    return false;
  } else {
    queue->rear = (queue->rear + 1) % queue->capacity;
    queue->static_memory_for_queue[queue->rear] = push_value;
    queue->size = queue->size + 1;
    queue->count1++;
    return true;
  }
}
bool queue__pop1(queue_s *queue, uint8_t *pop_value) {
  if (queue->size == 0) {
    return false;
  } else {
    *pop_value = queue->static_memory_for_queue[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size = queue->size - 1;
    queue->count1--;
    return true;
  }
}
size_t queue__get_item_count1(const queue_s *queue) { return queue->count1; }