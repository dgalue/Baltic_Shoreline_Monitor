#include "common.h"
#include "freertos/task.h"

void Task_GPS(void *pvParameters) {
  for (;;) {
    // TODO: read GPS data and update event
    event_t evt = { .ver = 1, .type = 2 };
    xQueueSend(q_events, &evt, portMAX_DELAY);
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}
