#pragma once

#include "can_bus.h"
#define CAN_BAUD_RATE 100 // Kbps
#define CAN_RX_QUEUE_SIZE 50
#define CAN_TX_QUEUE_SIZE 50

void can_bus_initialize(can__num_e can_num);
