#include "common.h"
#include "freertos/task.h"
#include "esp_task_wdt.h"
#include "driver/adc.h"

void Task_SenseAudio(void *pvParameters) {
  audio_block_t block;
  esp_task_wdt_add(NULL);
  for (;;) {
    size_t bytes_read = 0;
    adc_digi_read_bytes((uint8_t *)block.samples, sizeof(block.samples), &bytes_read, ADC_MAX_DELAY);
    if (bytes_read == sizeof(block.samples)) {
      xQueueSend(q_audio, &block, portMAX_DELAY);
    }
    esp_task_wdt_reset();
  }
}
