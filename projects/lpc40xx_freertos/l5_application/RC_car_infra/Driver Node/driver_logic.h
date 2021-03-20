/**
 * @file driver_logic.h
 * @brief This file includes the declaration for global parameters and functions
 *      involved in building driver node logic. 
 */

#pragma once

#include "project.h"


/**
 * This API will process the received sensor data to convert it to
 * motor understandable signals. It will run the obstacle avoidance logic.
 */
dbc_DRIVER_TO_MOTOR_s driver_logic__get_motor_command(void);