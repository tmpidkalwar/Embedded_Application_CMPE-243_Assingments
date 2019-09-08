#pragma once

/**
 * @file
 * This file provides API to turn on peripheral power, and their interrupts
 * Turning off power and disabling peripherals is a YAGNI so do not be suprised of the omission
 */

/**
 * These enum values map to the interrupt vector table
 * Check UM10562.pdf, Chapter 5: NVIC
 *
 * @note If you add more peripherals here, you will have to modify lpc_peripherals.c too
 /
typedef enum {
  LPC_PERIPHERAL__UART0 = 5,
  LPC_PERIPHERAL__UART1 = 6,
  LPC_PERIPHERAL__UART2 = 7,
  LPC_PERIPHERAL__UART3 = 8,
} lpc_peripheral_e;

/// Function pointer type for an interrupt
typedef void (*lpc_peripheral__isr_callback_f)(void);

void lpc_peripheral__turn_on_power_to(lpc_peripherals_e peripheral);

void lpc_peripheral__enable_interrupt(lpc_peripherals_e peripheral, lpc_peripheral__isr_callback_f isr_callback);
