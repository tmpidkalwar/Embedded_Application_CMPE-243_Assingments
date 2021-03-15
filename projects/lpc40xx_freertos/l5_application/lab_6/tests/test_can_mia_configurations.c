#include "unity.h"

#include "can_mia_configuration.h"
#include "project.h"

void setup(void) {}

void tearDown(void) {}

void test_can_mia_service_SENSOR_ACCTMTR_above_mia_threshold(void) {
  dbc_SENSOR_ACCMTR_s can_msg_accmtr = {0};
  can_msg_accmtr.mia_info.mia_counter = dbc_mia_threshold_SENSOR_ACCMTR - 100 + 1;
  TEST_ASSERT_EQUAL(true, can_mia__service_mia_SENSOR_ACCMTR(&can_msg_accmtr, 100));
}

void test_can_mia_service_SENSOR_ACCTMTR_below_mia_threshold(void) {
  dbc_SENSOR_ACCMTR_s can_msg_accmtr = {0};
  can_msg_accmtr.mia_info.mia_counter = dbc_mia_threshold_SENSOR_ACCMTR - 100 - 1;
  TEST_ASSERT_EQUAL(false, can_mia__service_mia_SENSOR_ACCMTR(&can_msg_accmtr, 100));
}

void test_can_mia_service_SENSOR_ACCTMTR_equal_mia_threshold(void) {
  dbc_SENSOR_ACCMTR_s can_msg_accmtr = {0};
  can_msg_accmtr.mia_info.mia_counter = dbc_mia_threshold_SENSOR_ACCMTR - 100; // Pre-increment by 100 value
  TEST_ASSERT_EQUAL(true, can_mia__service_mia_SENSOR_ACCMTR(&can_msg_accmtr, 100));
}