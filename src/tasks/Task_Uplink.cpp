#include "common.h"
#include "freertos/task.h"
#include "esp_task_wdt.h"

// Meshtastic application API
#include <meshtastic/application.h>
#include <string.h>

void Task_Uplink(void *pvParameters) {
  event_t evt;
  esp_task_wdt_add(NULL);
  for (;;) {
codex/enable-task-watchdog-and-add-handlers
    esp_task_wdt_reset();
    if (xQueueReceive(q_events, &evt, pdMS_TO_TICKS(1000))) {
      // TODO: send event via Meshtastic

    if (xQueueReceive(q_events, &evt, portMAX_DELAY)) {
      const uint8_t *payload = reinterpret_cast<const uint8_t *>(&evt);
      const size_t len = sizeof(evt);

      bool sent = false;
      TickType_t backoff = pdMS_TO_TICKS(50);
      for (int attempts = 0; attempts < 3 && !sent; ++attempts) {
        auto err = meshtastic::app::sendAppData(payload, len);
        if (err == meshtastic::app::Error::None) {
          sent = true;
        } else if (err == meshtastic::app::Error::Busy) {
          vTaskDelay(backoff);
          backoff *= 2;  // simple exponential backoff
        } else {
          break;
        }
      }

      if (!sent && q_log) {
        log_blob_t blob;
        blob.len = len > sizeof(blob.data) ? sizeof(blob.data) : len;
        memcpy(blob.data, payload, blob.len);
        xQueueSend(q_log, &blob, 0);
      }
main
    }
  }
}
