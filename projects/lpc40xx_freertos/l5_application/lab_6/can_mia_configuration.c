#include "can_mia_configuration.h"
#include "stdio.h"

const uint32_t dbc_mia_threshold_SENSOR_ACCMTR = 3000; // 3000ms
const uint32_t dbc_mia_threshold_SENSOR_BUTTON = 300;  // 300ms

const dbc_SENSOR_ACCMTR_s dbc_mia_replacement_ACCMTR_values;
const dbc_SENSOR_BUTTON_s dbc_mia_replacement_button;

bool can_mia__service_mia_SENSOR_ACCMTR(dbc_SENSOR_ACCMTR_s *can_msg_sensor_accmtr, const uint32_t mia_increment_vaue) {
  bool retValue = false;
  can_msg_sensor_accmtr->mia_info.mia_counter += mia_increment_vaue;
  if (can_msg_sensor_accmtr->mia_info.mia_counter >= dbc_mia_threshold_SENSOR_ACCMTR) {
    can_msg_sensor_accmtr->SENSOR_ACCMTR_x = dbc_mia_replacement_ACCMTR_values.SENSOR_ACCMTR_x;
    can_msg_sensor_accmtr->SENSOR_ACCMTR_y = dbc_mia_replacement_ACCMTR_values.SENSOR_ACCMTR_y;
    retValue = true;
    // printf("hi\n");
  }
  // printf("hello\n");
  return retValue;
}