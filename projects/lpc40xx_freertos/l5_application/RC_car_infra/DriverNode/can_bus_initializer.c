#include "can_bus_initializer.h"
#include <stddef.h>

void can_bus_initialize(can__num_e can_num) {
  can__init(can_num, CAN_BAUD_RATE, CAN_RX_QUEUE_SIZE, CAN_TX_QUEUE_SIZE, NULL, NULL);
  can__bypass_filter_accept_all_msgs();
  can__reset_bus(can_num);
}
