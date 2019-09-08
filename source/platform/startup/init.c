#include <stdint.h>

#include "ram.h"

extern void main(void);

static void init_ram(void);
static void init_system_clock(void);

/**
 * Entry point
 */
void start(void)
{
    init_ram();
    init_system_clock();
    main();
}

static void init_ram(void)
{
    ram__init_data();
    ram__init_bss();
}

static void init_system_clock(void)
{
    // TODO: Change default core clock
}
