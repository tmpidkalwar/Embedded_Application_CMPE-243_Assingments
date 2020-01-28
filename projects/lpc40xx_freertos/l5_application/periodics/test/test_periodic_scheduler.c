#include <stdio.h>
#include <string.h>

#include "unity.h"

// Include the source we wish to test
// We #include the *.c source file in this case if we wish to access the "internal" as in "static" members
#include "periodic_scheduler.c"

void setUp(void) {

}

void tearDown(void) {

}

void test_periodic_scheduler__initialize(void) {
    periodic_scheduler__initialize(1024);
}