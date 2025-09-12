#include "common.h"
#include "freertos/task.h"
#include "esp_task_wdt.h"

void Task_Uplink(void *pvParameters) {
  event_t evt;
  esp_task_wdt_add(NULL);
  for (;;) {
    esp_task_wdt_reset();
    if (xQueueReceive(q_events, &evt, pdMS_TO_TICKS(1000))) {
      // TODO: send event via Meshtastic
    }
  }
}
