#pragma once

#include <stdint.h>

#include "lpc40xx.h"

void NVIC_enable_interrupt_request(IRQn_Type request_number);
void NVIC_disable_interrupt_request(IRQn_Type request_number);
void NVIC_clear_interrupt_request(IRQn_Type request_number);
void NVIC_config_interrupt_request_priority(IRQn_Type request_number, uint32_t priority_level);
void NVIC_reset(void);


