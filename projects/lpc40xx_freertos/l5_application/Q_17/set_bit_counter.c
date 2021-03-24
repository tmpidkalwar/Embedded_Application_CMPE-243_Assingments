#include "set_bit_counter.h"

uint32_t count_bits(uint32_t value) {
  uint32_t count = 0;
  while (value) {
    count += value & 1;
    value >>= 1;
  }
  return count;
}