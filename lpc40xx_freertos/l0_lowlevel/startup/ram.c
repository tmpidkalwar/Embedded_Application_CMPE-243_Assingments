#include <stdint.h>

#include "ram.h"

static void ram__init_data_sram(void);
static void ram__init_bss_sram(void);

void ram__init_data(void) { ram__init_data_sram(); }

void ram__init_bss(void) { ram__init_bss_sram(); }

static void ram__init_data_sram(void) {
  extern void *_bdata_lma;
  extern void *_bdata_vma;
  extern void *_data_end;

  uint8_t *src = (uint8_t *)&_bdata_lma;
  uint8_t *dest = (uint8_t *)&_bdata_vma;
  while (dest < (uint8_t *)&_data_end) {
    *dest = *src;
    dest++;
    src++;
  }
}

static void ram__init_bss_sram(void) {
  extern void *_bss_start;
  extern void *_bss_end;

  uint8_t *ptr = (uint8_t *)&_bss_start;
  while (ptr < (uint8_t *)&_bss_end) {
    *ptr = 0U;
    ptr++;
  }
}
