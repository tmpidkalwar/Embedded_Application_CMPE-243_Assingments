// @file gps.c
#include "gps.h"

// Our 'private' modules: We hide and abstract away these details from the user
// Whoever #includes "Mockgps.h" will not need to deal with these because
// these are included in this source file rather than the header file
#include "line_buffer.h"
#include "uart.h"

void gps__run_once(void) {
  // ...
}