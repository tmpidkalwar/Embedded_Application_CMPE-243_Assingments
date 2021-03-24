#include "highest.h"

static inline uint32_t max(uint32_t A, uint32_t B) { return ((A >= B) ? A : B); }

uint32_t get_highest(const uint32_t *array, size_t array_size) {
  uint32_t highest = 0;

  // As size_t expands to unsigned long, array size will never be negative
  if ((array == NULL) || (array_size == 0)) {
    return highest;
  }

  for (int i = 0; i < array_size; i++) {
    uint32_t curr_val = *array;
    highest = max(curr_val, highest);
    array++;
  }
  return highest;
}