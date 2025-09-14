#pragma once

#define ESP_OK 0

static inline int esp_task_wdt_reset(void) { return ESP_OK; }
static inline int esp_task_wdt_init(int, bool) { return ESP_OK; }
static inline int esp_task_wdt_set_user_handler(void (*handler)(void)) { (void)handler; return ESP_OK; }
