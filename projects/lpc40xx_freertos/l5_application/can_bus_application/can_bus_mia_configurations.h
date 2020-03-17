#pragma once

#include <stdbool.h>
#include <stdint.h>

// Implement DBC encode and send abstract function
extern bool dbc_send_can_message(void *argument_from_dbc_encode_and_send, uint32_t message_id, const uint8_t bytes[8],
                                 uint8_t dlc);
