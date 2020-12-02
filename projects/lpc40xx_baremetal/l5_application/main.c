#include "gpio.h"
#include <stdio.h>

static void delay(void) {
  for (volatile int i = 0; i < 100 * 1000; i++) {
  }
}

int main(void) {
  gpio_s led0 = gpio__construct_as_output(GPIO__PORT_2, 3);
  while (1) {
    gpio__set(led0);
    delay();
    gpio__reset(led0);
    delay();
  }

  return 0;
}

typedef void (*function__void_f)(void);
extern void *_estack;
void halt(void) {
  while (1)
    ;
}
void entry_point(void) { main(); }

__attribute__((section(".interrupt_vector_table"))) const function__void_f interrupt_vector_table[] = {
    /**
     * Core interrupt vectors
     */
    (function__void_f)&_estack, // 0 ARM: Initial stack pointer
    entry_point,                // 1 ARM: Initial program counter; your board will explode if you change this
    halt,                       // 2 ARM: Non-maskable interrupt
    halt,                       // 3 ARM: Hard fault
    halt,                       // 4 ARM: Memory management fault
    halt,                       // 5 ARM: Bus fault
    halt,                       // 6 ARM: Usage fault
    halt,                       // 7 ARM: Reserved
    halt,                       // 8 ARM: Reserved
    halt,                       // 9 ARM: Reserved
    halt,                       // 10 ARM: Reserved
    halt,                       // 11 ARM: Supervisor call (SVCall)
    halt,                       // 12 ARM: Debug monitor
    halt,                       // 13 ARM: Reserved
    halt,                       // 14 ARM: Pendable request for system service (PendableSrvReq)
    halt,                       // 15 ARM: System Tick Timer (SysTick)
};
