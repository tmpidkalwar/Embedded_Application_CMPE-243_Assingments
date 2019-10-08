#include "app_cli.h"

app_cli_status_e cli__hello(app_cli__argument_t argument, sl_string_t user_input_minus_command_name,
                            app_cli__print_string_function output_print_string);

app_cli_status_e cli__task_list(app_cli__argument_t argument, sl_string_t user_input_minus_command_name,
                                app_cli__print_string_function output_print_string);