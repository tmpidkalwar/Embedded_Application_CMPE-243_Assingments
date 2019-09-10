#include "lpc40xx.h"
#include "ram.h"
#include "startup.h"

void startup__initialize_ram(void)
{
    ram__init_data();
    ram__init_bss();
}


/**
 * References:
 * - http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.ddi0439b/BEHBJHIG.html
 */
void startup__initialize_fpu(void)
{
    SCB->CPACR |= (0xF << 20);  // Set [23:20]
}

void startup__initialize_system_clock(void)
{
    // TODO: Change default core clock
}
