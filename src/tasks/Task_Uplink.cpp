#include "common.h"
#include "freertos/task.h"

void Task_Uplink(void *pvParameters) {
  event_t evt;
  for (;;) {
    if (xQueueReceive(q_events, &evt, portMAX_DELAY)) {
      // TODO: send event via Meshtastic
    }
  }
}
