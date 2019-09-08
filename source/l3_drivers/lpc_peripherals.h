#pragma once

typedef enum {
  LPC_PERIPHERAL__UART0,
  LPC_PERIPHERAL__UART1,
  LPC_PERIPHERAL__UART2,
  LPC_PERIPHERAL__UART3,
} lpc_peripheral_e;

/// Function pointer type for an interrupt
typedef void (*lpc_peripheral__isr_callback_f)(void);

void lpc_peripheral__turn_on_power_to(lpc_peripherals_e peripheral);

void lpc_peripheral__enable_interrupt(lpc_peripherals_e peripheral, lpc_peripheral__isr_callback_f isr_callback);
