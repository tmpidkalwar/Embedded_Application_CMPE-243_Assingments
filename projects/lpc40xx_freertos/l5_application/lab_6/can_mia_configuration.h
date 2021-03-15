#pragma once

#include "project.h"
#include <stdbool.h>
#include <stdint.h>

bool can_mia__service_mia_SENSOR_ACCMTR(dbc_SENSOR_ACCMTR_s *can_msg_sensor_accmtr, const uint32_t mia_increment_vaue);