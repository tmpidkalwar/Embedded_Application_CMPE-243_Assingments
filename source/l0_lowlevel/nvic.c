#include "nvic.h"

void NVIC_enable_interrupt_request(IRQn_Type request_number)
{
    NVIC_EnableIRQ(request_number);
}

void NVIC_disable_interrupt_request(IRQn_Type request_number)
{
    NVIC_DisableIRQ(request_number);
}

void NVIC_clear_interrupt_request(IRQn_Type request_number)
{
    NVIC_ClearPendingIRQ(request_number);
}

void NVIC_config_interrupt_request_priority(IRQn_Type request_number, uint32_t priority_level)
{
    NVIC_SetPriority(request_number, priority_level);
}

void NVIC_reset(void)
{
    NVIC_SystemReset();
}
