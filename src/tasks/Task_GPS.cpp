#include "common.h"
#include "freertos/task.h"
#include "esp_task_wdt.h"
#include "driver/uart.h"
#include <string.h>
#include <math.h>
#include <time.h>

static bool parse_gpgga(const char *sentence, event_t &evt) {
  double rawLat, rawLon;
  char ns, ew;
  if (sscanf(sentence, "$GPGGA,%*[^,],%lf,%c,%lf,%c", &rawLat, &ns, &rawLon, &ew) < 5) {
    return false;
  }

  double lat_deg = floor(rawLat / 100.0);
  double lat_min = rawLat - lat_deg * 100.0;
  double lat = lat_deg + lat_min / 60.0;
  if (ns == 'S') lat = -lat;

  double lon_deg = floor(rawLon / 100.0);
  double lon_min = rawLon - lon_deg * 100.0;
  double lon = lon_deg + lon_min / 60.0;
  if (ew == 'W') lon = -lon;

  evt.lat_e7 = (int32_t)(lat * 1e7);
  evt.lon_e7 = (int32_t)(lon * 1e7);
  evt.ts_unix = time(NULL);
  return true;
}

void Task_GPS(void *pvParameters) {
  esp_task_wdt_add(NULL);
  char buf[128];
  event_t evt = { .ver = 1, .type = 2 };
  for (;;) {
    int len = uart_read_bytes(UART_NUM_1, (uint8_t *)buf, sizeof(buf) - 1, pdMS_TO_TICKS(1000));
    if (len > 0) {
      buf[len] = '\0';
      char *gpgga = strstr(buf, "$GPGGA");
      if (gpgga && parse_gpgga(gpgga, evt)) {
        xQueueSend(q_events, &evt, 0);
      }
    }
    esp_task_wdt_reset();
  }
}
