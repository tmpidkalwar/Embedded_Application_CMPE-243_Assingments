#pragma once

#include "stdbool.h"

#include "can_bus.h"

#define can_rx_queue_size 1000

bool can_bus_initializer__can1_init(void);