#include "sj2_cli_handlers.h"

#include "FreeRTOS.h"
#include "task.h"

app_cli_status_e cli__hello(app_cli__argument_t argument, sl_string_t user_input_minus_command_name,
                            app_cli__print_string_function cli_output) {
  sl_string_t output_string = user_input_minus_command_name; // re-use the string

  sl_string__printf(output_string, "hello world!\n");
  cli_output(argument, output_string);

  return APP_CLI_STATUS__SUCCESS;
}

app_cli_status_e cli__task_list(app_cli__argument_t argument, sl_string_t user_input_minus_command_name,
                                app_cli__print_string_function cli_output) {
  sl_string_t output_string = user_input_minus_command_name; // re-use the string

  // Enum to char : eRunning, eReady, eBlocked, eSuspended, eDeleted
  const char *const task_status_table[] = {"running", " ready ", "blocked", "suspend", "deleted"};

  // Limit the tasks to avoid heap allocation.
  const unsigned portBASE_TYPE max_tasks = 10;
  TaskStatus_t status[max_tasks];
  uint32_t total_cpu_runtime = 0;
  uint32_t total_tasks_runtime = 0;
  const unsigned portBASE_TYPE uxArraySize = uxTaskGetSystemState(&status[0], max_tasks, &total_cpu_runtime);

  sl_string__printf(output_string, "%10s  Status Pr Stack CPU%%          Time\n", "Name");
  cli_output(argument, output_string);

  for (unsigned priority_number = 0; priority_number < configMAX_PRIORITIES; priority_number++) {
    /* Print in sorted priority order */
    for (unsigned i = 0; i < uxArraySize; i++) {
      const TaskStatus_t *e = &status[i];
      if (e->uxBasePriority == priority_number) {
        total_tasks_runtime += e->ulRunTimeCounter;

        const unsigned cpu_percent = (0 == total_cpu_runtime) ? 0 : e->ulRunTimeCounter / (total_cpu_runtime / 100);
        const unsigned time_us = e->ulRunTimeCounter;
        const unsigned stack_in_bytes = (sizeof(void *) * e->usStackHighWaterMark);

        sl_string__printf(output_string, "%10s %s %2u %5u %4u %10u us\n", e->pcTaskName,
                          task_status_table[e->eCurrentState], (unsigned)e->uxBasePriority, stack_in_bytes, cpu_percent,
                          time_us);
        cli_output(argument, output_string);
      }
    }
  }

  return APP_CLI_STATUS__SUCCESS;
}