#include "common.h"
#include "freertos/task.h"
#include "esp_task_wdt.h"

void Task_Vision(void *pvParameters) {
  esp_task_wdt_add(NULL);
  for (;;) {
    // TODO: run vision inference
    event_t evt = { .ver = 1, .type = 1 };
    xQueueSend(q_events, &evt, portMAX_DELAY);
    esp_task_wdt_reset();
    vTaskDelay(pdMS_TO_TICKS(5000));
  }
}
