#include <stdbool.h>

#include "FreeRTOS.h"
#include "task.h"

#include "periodic_callbacks.h"

/// Task data structure of each periodic task
typedef struct {
  const TickType_t task_delay_in_ticks;
  periodic_callbacks_f callback; ///< This callback is invoked from the task

  /// After the callback() is invoked, this flag is set which is later checked by periodic_scheduler__task_monitor()
  bool task_finished_flag;
} periodic_scheduler_s;

/// Instances of the 4 periodic tasks and their callback function pointer
static periodic_scheduler_s periodic_scheduler__1Hz = {1, periodic_callbacks__1Hz};
static periodic_scheduler_s periodic_scheduler__10Hz = {10, periodic_callbacks__10Hz};
static periodic_scheduler_s periodic_scheduler__100Hz = {100, periodic_callbacks__100Hz};
static periodic_scheduler_s periodic_scheduler__1000Hz = {1000, periodic_callbacks__1000Hz};

/// Common task runner for each periodic task
static void periodic_scheduler__run(periodic_scheduler_s *periodic) {
  TickType_t previous_tick = 0;
  uint32_t callback_counter = 0;

  // Note: the order of the while loop's logic is critical; change with care
  while (true) {
    periodic->callback(callback_counter);
    ++callback_counter;
    periodic->task_finished_flag = true;

    vTaskDelayUntil(&previous_tick, periodic->task_delay_in_ticks);
  }
}

static void periodic_scheduler__1Hz_task(void *param) { periodic_scheduler__run(&periodic_scheduler__1Hz); }
static void periodic_scheduler__10Hz_task(void *param) { periodic_scheduler__run(&periodic_scheduler__10Hz); }
static void periodic_scheduler__100Hz_task(void *param) { periodic_scheduler__run(&periodic_scheduler__100Hz); }
static void periodic_scheduler__1000Hz_task(void *param) { periodic_scheduler__run(&periodic_scheduler__1000Hz); }

static void periodic_scheduler__check_flag(periodic_scheduler_s *periodic_task) {
  if (periodic_task->task_finished_flag) {
    periodic_task->task_finished_flag = false;
  } else {
    // Reboot
  }
}
static void periodic_scheduler__task_monitor(void *param) {
  // We let all the other tasks run first, and then check their flags
  while (1) {
    vTaskDelay(1);
    periodic_scheduler__check_flag(&periodic_scheduler__1000Hz);

    const unsigned milliseconds = xTaskGetTickCount();
    if (0 == (milliseconds % 10)) {
      periodic_scheduler__check_flag(&periodic_scheduler__100Hz);
      if (0 == (milliseconds % 100)) {
        periodic_scheduler__check_flag(&periodic_scheduler__10Hz);
        if (0 == (milliseconds % 1000)) {
          periodic_scheduler__check_flag(&periodic_scheduler__1Hz);
        }
      }
    }
  }
}

void periodic_scheduler__initialize(uint32_t task_stack_size) {
  xTaskCreate(periodic_scheduler__1Hz_task, "1Hz", task_stack_size, NULL, PRIORITY_PERIODIC_1HZ, NULL);
  xTaskCreate(periodic_scheduler__10Hz_task, "10Hz", task_stack_size, NULL, PRIORITY_PERIODIC_10HZ, NULL);
  xTaskCreate(periodic_scheduler__100Hz_task, "100Hz", task_stack_size, NULL, PRIORITY_PERIODIC_100HZ, NULL);
  xTaskCreate(periodic_scheduler__1000Hz_task, "1000Hz", task_stack_size, NULL, PRIORITY_PERIODIC_1000HZ, NULL);

  xTaskCreate(periodic_scheduler__task_monitor, "xHz", task_stack_size, NULL, PRIORITY_PERIODIC_MONITOR, NULL);

  periodic_callbacks__initialize();
}