#include "common.h"
#include "freertos/task.h"

void Task_SenseAudio(void *pvParameters) {
  audio_block_t block = {0};
  for (;;) {
    // TODO: fill block with ADC samples
    xQueueSend(q_audio, &block, portMAX_DELAY);
    vTaskDelay(pdMS_TO_TICKS(32));
  }
}
