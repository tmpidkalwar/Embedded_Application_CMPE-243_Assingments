#include "unity.h"

#include "Mockboard_io.h"
#include "Mockcan_bus.h"
#include "Mockgpio.h"

#include "can_bus_initializer.h"

void setup(void) {}

void tearDown(void) {}

void test_init(void) {}

void test_can_bus_initializer__can1_init(void) {
  can_void_func_t dummy_func;
  can__init_ExpectAndReturn(can1, 100, 1000, 100, dummy_func, dummy_func, true);
  can__init_IgnoreArg_bus_off_cb();
  can__init_IgnoreArg_data_ovr_cb();
  can__bypass_filter_accept_all_msgs_Expect();
  can__reset_bus_Expect(can1);
  can_bus_initializer__can1_init();
}