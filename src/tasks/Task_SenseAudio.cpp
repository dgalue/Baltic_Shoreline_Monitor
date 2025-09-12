#include "common.h"
#include "freertos/task.h"
#include "esp_task_wdt.h"

void Task_SenseAudio(void *pvParameters) {
  audio_block_t block = {0};
  esp_task_wdt_add(NULL);
  for (;;) {
    // TODO: fill block with ADC samples
    xQueueSend(q_audio, &block, portMAX_DELAY);
    esp_task_wdt_reset();
    vTaskDelay(pdMS_TO_TICKS(32));
  }
}
