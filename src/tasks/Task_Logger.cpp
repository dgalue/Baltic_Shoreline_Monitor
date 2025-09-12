#include "common.h"
#include "freertos/task.h"

void Task_Logger(void *pvParameters) {
  log_blob_t blob;
  for (;;) {
    if (xQueueReceive(q_log, &blob, portMAX_DELAY)) {
      // TODO: write blob to SD card
    }
  }
}
