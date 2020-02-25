#include "line_buffer.h"
void line_buffer__init(line_buffer_s *buffer, void *memory, size_t size) {
  buffer->memory = (uint8_t *)memory;
  buffer->max_size = size - 1;
  buffer->write_index = 0;
}

bool line_buffer__add_byte(line_buffer_s *buffer, char byte) {
  if (buffer->write_index >= (buffer->max_size)) {
    return false;
  } else {
    ((char *)buffer->memory)[buffer->write_index] = byte;
    buffer->write_index++;
    return true;
  }
}
bool line_buffer__remove_line(line_buffer_s *buffer, char *line, size_t line_max_size) {
  int i = 0, j = 0;
  if (buffer->write_index < 0)
    return false;
  else {
    for (i = 0; i < line_max_size; i++) {
      if (((char *)buffer->memory)[i] == '\n') {
        line[i] = '\0';
        left_shift((char *)buffer->memory, line_max_size, i);
        j = 0;
      } else {
        line[i] = ((char *)buffer->memory)[j];
        j++;
      }
    }
    return true;
  }
}

void left_shift(char *arr, size_t max_size, int shift) {
  int k;
  for (k = 0; k < max_size; k++) {
    arr[k] = arr[k + shift + 1];
  }
}