#include "common.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_task_wdt.h"
#include "driver/i2c.h"

void Task_Vision(void *pvParameters) {
  SemaphoreHandle_t mtx = (SemaphoreHandle_t)pvParameters;
  esp_task_wdt_add(NULL);
  for (;;) {
    uint8_t dummy[2];
    if (mtx) {
      xSemaphoreTake(mtx, portMAX_DELAY);
      i2c_master_read_from_device(I2C_NUM_0, 0x42, dummy, sizeof(dummy), pdMS_TO_TICKS(100));
      xSemaphoreGive(mtx);
    }
    event_t evt = { .ver = 1, .type = 1 };
    xQueueSend(q_events, &evt, portMAX_DELAY);
    esp_task_wdt_reset();
    vTaskDelay(pdMS_TO_TICKS(5000));
  }
}
