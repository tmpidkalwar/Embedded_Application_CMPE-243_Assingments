#pragma once
#include "can_bus.h"
#include "project.h"

#define SENSOR_SONARS_MIA_THRESHOLD 5

void handle_all_can_messages_rx(can__num_e can_num);
