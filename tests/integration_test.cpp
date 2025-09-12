#include "common.h"
#include "tasks/tasks.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_task_wdt.h"
#include <assert.h>
#include <stdio.h>

extern "C" void meshtastic_init();

static void monitor_task(void *pvParameters) {
    const TickType_t delay = pdMS_TO_TICKS(1000);
    for (int i = 0; i < 10; ++i) {
        assert(esp_task_wdt_reset() == ESP_OK);
        printf("[TST] tick %d q_audio=%u q_events=%u q_log=%u\n", i,
               (unsigned)uxQueueMessagesWaiting(q_audio),
               (unsigned)uxQueueMessagesWaiting(q_events),
               (unsigned)uxQueueMessagesWaiting(q_log));
        vTaskDelay(delay);
    }
    printf("[TST] integration test complete\n");
    vTaskDelete(NULL);
}

extern "C" void app_main(void) {
    meshtastic_init();

    q_audio = xQueueCreate(8, sizeof(audio_block_t));
    q_events = xQueueCreate(64, sizeof(event_t));
    q_log   = xQueueCreate(32, sizeof(log_blob_t));

    xTaskCreatePinnedToCore(Task_SenseAudio, "Task_SenseAudio", 4096, NULL, 4, NULL, 1);
    xTaskCreatePinnedToCore(Task_GPS,        "Task_GPS",        4096, NULL, 3, NULL, 1);
    xTaskCreatePinnedToCore(Task_Vision,     "Task_Vision",     4096, NULL, 4, NULL, 1);
    xTaskCreatePinnedToCore(Task_Logger,     "Task_Logger",     4096, NULL, 5, NULL, 1);
    xTaskCreatePinnedToCore(Task_Uplink,     "Task_Uplink",     4096, NULL, 5, NULL, 1);

    xTaskCreate(monitor_task, "monitor_task", 4096, NULL, 5, NULL);
}

