#include "common.h"
#include "freertos/task.h"
#include "esp_task_wdt.h"

void Task_Logger(void *pvParameters) {
  log_blob_t blob;
  esp_task_wdt_add(NULL);
  for (;;) {
    esp_task_wdt_reset();
    if (xQueueReceive(q_log, &blob, pdMS_TO_TICKS(1000))) {
      // TODO: write blob to SD card
    }
  }
}
