#pragma once

#include <stdbool.h>

void sensor_node__init(void);

bool sensor_node__send_sonars_over_can(void);

void sensor_node__handle_mia(void);

void sensor_node__handle_messages_over_can(void);

// remove these two when done testing
void sensor_node__print_ultrasonic_data(void);
void sensor_node__collect_ultrasonic_data(void);
