#pragma once

#include "stdbool.h"

#include "can_bus.h"

#define can_rx_queue_size 1000

bool can_bus_initializer__can1_init(void);
void switch2_press_handler(void);
bool get_switch2_status(void);
void clear_switch2_status(void);