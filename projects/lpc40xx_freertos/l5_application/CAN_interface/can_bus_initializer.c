#include "can_bus_initializer.h"
#include "stdio.h"

bool can_bus_initializer__can1_init(void) {
  can__init(can1, 100, 1000, 100, NULL, NULL);
  can__bypass_filter_accept_all_msgs();
  can__reset_bus(can1);
  return true;
}