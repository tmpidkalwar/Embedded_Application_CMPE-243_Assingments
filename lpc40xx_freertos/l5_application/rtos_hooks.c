#include "FreeRTOS.h"
#include "task.h"

/**
 * Invoked by FreeRTOS when it detects stack overflow
 */
void vApplicationStackOverflowHook(TaskHandle_t task_handle, signed char *task_name) {}
