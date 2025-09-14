#pragma once
#include "FreeRTOS.h"

typedef void (*TaskFunction_t)(void*);

static inline BaseType_t xTaskCreate(TaskFunction_t task, const char* name, uint32_t stack, void* params, UBaseType_t priority, void* handle) {
    (void)task; (void)name; (void)stack; (void)params; (void)priority; (void)handle; return pdPASS;
}

static inline BaseType_t xTaskCreatePinnedToCore(TaskFunction_t task, const char* name, uint32_t stack, void* params, UBaseType_t priority, void* handle, int core) {
    (void)task; (void)name; (void)stack; (void)params; (void)priority; (void)handle; (void)core; return pdPASS;
}

static inline void vTaskDelay(TickType_t ticks) { (void)ticks; }
static inline void vTaskDelete(void* task) { (void)task; }
