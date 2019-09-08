#include "lpc_peripherals.h"

#include "lpc40xx.h"

/**
 * The enumeration values correspond to the peripherals' power-on bit
 * Check UM10562.pdf, Chapter 3: System and clock control
 */
static const uint8_t lpc_peripheral_pconp_bit_map[] = {
  [LPC_PERIPHERAL__UART0] = 3,
  [LPC_PERIPHERAL__UART1] = 4,
  [LPC_PERIPHERAL__UART2] = 24,
  [LPC_PERIPHERAL__UART3] = 25,
}

/// An unregistered interrupt handler is a forever loop
static void lpc_peripheral__unregistered_dispatcher(void) {
  while(1) {}
}

/**
 * Map of the user ISR registrations
 */
static void lpc_peripheral__isr_callback_f lpc_peripheral__isr_registrations[32 + 9] = {
  lpc_peripheral__unregistered_dispatcher, // 16 WDT
  lpc_peripheral__unregistered_dispatcher, // 17 Timer 0
  lpc_peripheral__unregistered_dispatcher, // 18 Timer 1
  lpc_peripheral__unregistered_dispatcher, // 19 Timer 2
  lpc_peripheral__unregistered_dispatcher, // 20 Timer 3
  lpc_peripheral__unregistered_dispatcher, // 21 UART 0
  lpc_peripheral__unregistered_dispatcher, // 22 UART 1
  lpc_peripheral__unregistered_dispatcher, // 23 UART 2

  lpc_peripheral__unregistered_dispatcher, // 24 UART 3
  lpc_peripheral__unregistered_dispatcher, // 25 PWM 1
  lpc_peripheral__unregistered_dispatcher, // 26 I2C 0
  lpc_peripheral__unregistered_dispatcher, // 27 I2C 1
  lpc_peripheral__unregistered_dispatcher, // 28 I2C 2
  lpc_peripheral__unregistered_dispatcher, // 29 UNUSED
  lpc_peripheral__unregistered_dispatcher, // 30 SSP 0
  lpc_peripheral__unregistered_dispatcher, // 31 SSP 1

  lpc_peripheral__unregistered_dispatcher, // 32 PLL 0
  lpc_peripheral__unregistered_dispatcher, // 33 RTC and Event Monitor/Recorder
  lpc_peripheral__unregistered_dispatcher, // 34 External Interrupt 0 (EINT 0)
  lpc_peripheral__unregistered_dispatcher, // 35 External Interrupt 1 (EINT 1)
  lpc_peripheral__unregistered_dispatcher, // 36 External Interrupt 2 (EINT 2)
  lpc_peripheral__unregistered_dispatcher, // 37 External Interrupt 3 (EINT 3)
  lpc_peripheral__unregistered_dispatcher, // 38 ADC
  lpc_peripheral__unregistered_dispatcher, // 39 BOD

  lpc_peripheral__unregistered_dispatcher, // 40 USB
  lpc_peripheral__unregistered_dispatcher, // 41 CAN
  lpc_peripheral__unregistered_dispatcher, // 42 DMA Controller
  lpc_peripheral__unregistered_dispatcher, // 43 I2S
  lpc_peripheral__unregistered_dispatcher, // 44 Ethernet
  lpc_peripheral__unregistered_dispatcher, // 45 SD Card Interface
  lpc_peripheral__unregistered_dispatcher, // 46 Motor Control PWM
  lpc_peripheral__unregistered_dispatcher, // 47 PLL 1

  lpc_peripheral__unregistered_dispatcher, // 48 Quadrature Encoder
  lpc_peripheral__unregistered_dispatcher, // 49 USB Activity
  lpc_peripheral__unregistered_dispatcher, // 50 CAN Activity
  lpc_peripheral__unregistered_dispatcher, // 51 UART 4
  lpc_peripheral__unregistered_dispatcher, // 52 SSP 2
  lpc_peripheral__unregistered_dispatcher, // 53 LCD
  lpc_peripheral__unregistered_dispatcher, // 54 GPIO Interrupt
  lpc_peripheral__unregistered_dispatcher, // 55 PWM 0

  lpc_peripheral__unregistered_dispatcher, // 56 EEPROM
}

/**
 * This function is intentionally not declared at the header file
 * This is registered by the startup code and registered as the interrupt callback for each peripheral
 */
void lpc_peripheral__interrupt_dispatcher(void) {
  // TODO: Check interrupt number and invoke the handler
}

void lpc_peripheral__turn_on_power_to(lpc_peripherals_e peripheral) {
  if (peripheral < sizeof(lpc_peripheral_pconp_bit_map)) {
    const uint32_t power_on_bit = (uint32_t) lpc_peripheral_pconp_bit_map[peripheral];
    LPC_SC->PCONP |= (1 << power_on_bit);
  }
}

void lpc_peripheral__enable_interrupt(lpc_peripherals_e peripheral, lpc_peripheral__isr_callback_f isr_callback) {
  lpc_peripheral__isr_registrations[peripheral] = isr_callback;
  NVIC_EnableIRQ(peripheral);
}
