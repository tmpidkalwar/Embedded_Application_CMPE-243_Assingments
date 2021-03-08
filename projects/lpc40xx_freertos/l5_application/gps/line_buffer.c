/**
 * @file line_buffer.c
 *
 * @brief This file maintains the APIs for managing line buffer to handle the received
 *      string from GPS line by line.
 */

#include "line_buffer.h"

void line_buffer__init(line_buffer_s *buffer, void *memory, size_t size) {
  buffer->memory = memory;
  buffer->max_size = size;
  buffer->write_index = 0;
  buffer->element_count = 0;
  buffer->read_index = 0;
}

bool line_buffer__add_byte(line_buffer_s *buffer, char byte) {
  if (buffer->element_count < buffer->max_size) {
    //(buffer->memory[buffer->write_index]) = byte;
    *((char *)(buffer->memory) + buffer->write_index) = byte;
    buffer->write_index = (buffer->write_index + 1) % buffer->max_size;
    buffer->element_count++;
    return true;
  }
  return false;
}

bool line_buffer__remove_line(line_buffer_s *buffer, char *line, size_t line_max_size) {

  size_t line_element_index = 0;
  bool retVal = false;
  while ((*((char *)(buffer->memory) + (buffer->read_index + line_element_index)) != '\n') &&
         (buffer->element_count > 0) && (line_element_index < line_max_size)) {

    line[line_element_index] = *((char *)(buffer->memory) + buffer->read_index + line_element_index);
    buffer->element_count--;
    line_element_index++;
  }

  // If we found end of line, udpate read index value to start fetching next line,
  // as we successfully read one line
  if (*((char *)(buffer->memory) + (buffer->read_index + line_element_index)) == '\n') {
    line[line_element_index] = '\0';
    // Read index updated with number of characters read and one additional character for '\n'
    buffer->read_index = ((buffer->read_index + line_element_index + 1) % buffer->max_size);
    retVal = true;
  } else if ((buffer->element_count + line_element_index) == buffer->max_size) {
    // There is no element left in the buffer to read and we still didn't find '\n',
    // read the line out to make space in the buffer.
    // Note: Keep one character space vacant for '\0' in the line buffer
    if (line_element_index == (line_max_size)) {
      // line buffer is full, over write last character with '\0'
      // and update buffer's read index to read this last characer next time
      buffer->read_index = ((buffer->read_index + line_element_index) % buffer->max_size);
      line[line_element_index - 1] = '\0';
    } else {
      buffer->read_index = ((buffer->read_index + line_element_index + 1) % buffer->max_size);
      line[line_element_index] = '\0';
    }
    retVal = true;
  } else {
    // We couldn't find end of line, thus we should revert buffers element count to previous one,
    // as this was not successful reading of a line
    buffer->element_count += line_element_index;
    retVal = false;
  }
  return retVal;
}
