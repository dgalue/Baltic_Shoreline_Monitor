#include "common.h"
#include "tasks/tasks.h"
#include "freertos/task.h"

extern "C" void meshtastic_init(); // Provided by Meshtastic firmware

QueueHandle_t q_audio;
QueueHandle_t q_events;
QueueHandle_t q_log;

extern "C" void app_main(void) {
  meshtastic_init();

  if (!hardware_init()) {
    return; // fail safely
  }

  q_audio = xQueueCreate(8, sizeof(audio_block_t));
  q_events = xQueueCreate(64, sizeof(event_t));
  q_log   = xQueueCreate(32, sizeof(log_blob_t));
  if (!q_audio || !q_events || !q_log) {
    return; // fail if queues cannot be created
  }

  xTaskCreatePinnedToCore(Task_SenseAudio, "Task_SenseAudio", 4096, NULL, 4, NULL, 1);
  xTaskCreatePinnedToCore(Task_GPS,        "Task_GPS",        4096, NULL, 3, NULL, 1);
  xTaskCreatePinnedToCore(Task_Vision,     "Task_Vision",     4096, mtx_i2c, 4, NULL, 1);
  xTaskCreatePinnedToCore(Task_Logger,     "Task_Logger",     4096, NULL, 5, NULL, 1);
  xTaskCreatePinnedToCore(Task_Uplink,     "Task_Uplink",     4096, NULL, 5, NULL, 1);
}
