#include "common.h"
#include "freertos/task.h"
codex/enable-task-watchdog-and-add-handlers
#include "esp_task_wdt.h"

void Task_Vision(void *pvParameters) {
  esp_task_wdt_add(NULL);

#include "freertos/semphr.h"

void Task_Vision(void *pvParameters) {
  SemaphoreHandle_t mtx = (SemaphoreHandle_t)pvParameters;
main
  for (;;) {
    if (mtx) {
      xSemaphoreTake(mtx, portMAX_DELAY);
      // TODO: run vision inference with I2C sensors
      xSemaphoreGive(mtx);
    }
    event_t evt = { .ver = 1, .type = 1 };
    xQueueSend(q_events, &evt, portMAX_DELAY);
    esp_task_wdt_reset();
    vTaskDelay(pdMS_TO_TICKS(5000));
  }
}
