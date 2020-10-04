#include <stdio.h>

#include "clock.h"
#include "peripherals_init.h"
#include "startup.h"
#include "sys_time.h"

extern void main(void);
static void entry_point__halt(void);

void entry_point(void) {
  startup__initialize_ram();
  startup__initialize_fpu();
  startup__initialize_interrupts();

  clock__initialize_system_clock_96mhz();
  sys_time__init(clock__get_peripheral_clock_hz());

  // Peripherals init initializes UART and then we can print the crash report if applicable
  peripherals_init();

  printf("\n%s(): Entering main()\n", __FUNCTION__);
  main();

  entry_point__halt();
}

static void entry_point__halt(void) {
  // main() should never return: CPU will now halt forever at this point.
  fprintf(stderr, "ERROR: main() should never return, program has been halted");
  while (1) {
  }
}
