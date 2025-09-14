#pragma once
#include "FreeRTOS.h"
#include <mutex>

typedef std::mutex* SemaphoreHandle_t;

static inline SemaphoreHandle_t xSemaphoreCreateMutex(void) {
    return new std::mutex();
}

static inline BaseType_t xSemaphoreTake(SemaphoreHandle_t m, TickType_t wait) {
    (void)wait;
    m->lock();
    return pdPASS;
}

static inline BaseType_t xSemaphoreGive(SemaphoreHandle_t m) {
    m->unlock();
    return pdPASS;
}
