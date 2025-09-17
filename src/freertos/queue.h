#pragma once
#include "FreeRTOS.h"
#include <queue>
#include <vector>
#include <string.h>

typedef struct {
    size_t item_size;
    size_t length;
    std::queue<std::vector<uint8_t>> data;
} Queue;

typedef Queue* QueueHandle_t;

static inline QueueHandle_t xQueueCreate(UBaseType_t length, UBaseType_t item_size) {
    Queue* q = new Queue{(size_t)item_size, (size_t)length};
    return q;
}

static inline BaseType_t xQueueSend(QueueHandle_t q, const void* item, TickType_t wait) {
    (void)wait;
    if (q->data.size() >= q->length) return pdFAIL;
    std::vector<uint8_t> bytes(q->item_size);
    memcpy(bytes.data(), item, q->item_size);
    q->data.push(std::move(bytes));
    return pdPASS;
}

static inline BaseType_t xQueueReceive(QueueHandle_t q, void* item, TickType_t wait) {
    (void)wait;
    if (q->data.empty()) return pdFAIL;
    auto bytes = q->data.front();
    q->data.pop();
    memcpy(item, bytes.data(), q->item_size);
    return pdPASS;
}

static inline UBaseType_t uxQueueMessagesWaiting(QueueHandle_t q) {
    return (UBaseType_t)q->data.size();
}
