#include "common.h"
#include "freertos/task.h"

void Task_Vision(void *pvParameters) {
  for (;;) {
    // TODO: run vision inference
    event_t evt = { .ver = 1, .type = 1 };
    xQueueSend(q_events, &evt, portMAX_DELAY);
    vTaskDelay(pdMS_TO_TICKS(5000));
  }
}
