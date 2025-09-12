#include "common.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

void Task_Vision(void *pvParameters) {
  SemaphoreHandle_t mtx = (SemaphoreHandle_t)pvParameters;
  for (;;) {
    if (mtx) {
      xSemaphoreTake(mtx, portMAX_DELAY);
      // TODO: run vision inference with I2C sensors
      xSemaphoreGive(mtx);
    }
    event_t evt = { .ver = 1, .type = 1 };
    xQueueSend(q_events, &evt, portMAX_DELAY);
    vTaskDelay(pdMS_TO_TICKS(5000));
  }
}
