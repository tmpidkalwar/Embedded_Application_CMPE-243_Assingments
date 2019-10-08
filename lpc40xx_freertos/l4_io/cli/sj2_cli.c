#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"

#include "app_cli.h"
#include "sj2_cli_handlers.h"

/// Output all CLI to the standard output
static void sj2_cli__output_function(app_cli__argument_t argument, const char *string) { printf("%s", string); }

static void sj2_cli__task(void *p);
static void sj2_cli__get_line(sl_string_t user_input);

void sj2_cli__init(void) {
  // Note: Some terminals do not support color output so you may see wierd chars like '[34m'
  const bool color_output = true;
  const char *separator = "--------------------------------------------------------------------------------\r\n";

  static app_cli_s sj2_cli_struct;
  sj2_cli_struct = app_cli__initialize(4, sj2_cli__output_function, !color_output, separator);

  // Need static struct that does not go out of scope
  static app_cli__command_s hello_command = {.command_name = "hello",
                                             .help_message_for_command = "responds back with 'hello world'",
                                             .app_cli_handler = cli__hello};
  static app_cli__command_s task_list = {.command_name = "tasklist",
                                         .help_message_for_command = "Outputs list of RTOS tasks, CPU and stack usage",
                                         .app_cli_handler = cli__task_list};

  // Add your CLI commands in sorted order
  app_cli__add_command_handler(&sj2_cli_struct, &hello_command);
  app_cli__add_command_handler(&sj2_cli_struct, &task_list);

  // In case other tasks are hogging the CPU, it would be useful to run the CLI
  // at high priority to at least be able to see what is going on
  xTaskCreate(sj2_cli__task, "cli", 2048 / sizeof(void *), &sj2_cli_struct, PRIORITY_HIGH, NULL);
}

static void sj2_cli__task(void *task_parameter) {
  app_cli_s *sj2_cli_struct = (app_cli_s *)task_parameter;
  void *unused_cli_param = NULL;
  char string_memory[256];

  // Start by greeting the use with the 'help' command
  sl_string_t user_input = sl_string__initialize_from(string_memory, sizeof(string_memory), "help");

  while (true) {
    app_cli__process_input(sj2_cli_struct, unused_cli_param, user_input);
    sj2_cli__get_line(user_input);
  }
}

static void sj2_cli__get_line(sl_string_t user_input) {
  sl_string__clear(user_input);

  // As long as user does not enter a full line, continue to get input
  while (!sl_string__ends_with_newline(user_input)) {
    const char byte = getchar();
    sl_string__append_char(user_input, byte);

    if (sl_string__full(user_input)) {
      break;
    }
  }
}
