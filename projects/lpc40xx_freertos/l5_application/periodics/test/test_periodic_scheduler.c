#include <stdio.h>
#include <string.h>

#include "unity.h"

void NVIC_SystemReset(void) {}

// Include the source we wish to test
// We #include the *.c source file in this case if we wish to access the "internal" as in "static" members
#include "periodic_scheduler.c"

#include "Mockperiodic_callbacks.h"
#include "Mocktask.h"

void setUp(void) {}

void tearDown(void) {}

void test_periodic_scheduler__initialize(void) {
  const size_t stack_size = 1024;

  xTaskCreate_ExpectAndReturn(periodic_scheduler__1Hz_task, "1Hz", stack_size, NULL, PRIORITY_PERIODIC_1HZ, NULL, 0);
  xTaskCreate_ExpectAndReturn(periodic_scheduler__10Hz_task, "10Hz", stack_size, NULL, PRIORITY_PERIODIC_10HZ, NULL, 0);
  xTaskCreate_ExpectAndReturn(periodic_scheduler__100Hz_task, "100Hz", stack_size, NULL, PRIORITY_PERIODIC_100HZ, NULL,
                              0);
  xTaskCreate_ExpectAndReturn(periodic_scheduler__1000Hz_task, "1000Hz", stack_size, NULL, PRIORITY_PERIODIC_1000HZ,
                              NULL, 0);

  xTaskCreate_ExpectAndReturn(periodic_scheduler__task_monitor, "xHz", stack_size, NULL, PRIORITY_PERIODIC_MONITOR,
                              NULL, 0);

  periodic_callbacks__initialize_Expect();
  periodic_scheduler__initialize(stack_size);
}