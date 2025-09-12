#include "common.h"
#include "freertos/task.h"
#include "esp_task_wdt.h"

void Task_GPS(void *pvParameters) {
  esp_task_wdt_add(NULL);
  for (;;) {
    // TODO: read GPS data and update event
    event_t evt = { .ver = 1, .type = 2 };
    xQueueSend(q_events, &evt, portMAX_DELAY);
    esp_task_wdt_reset();
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}
