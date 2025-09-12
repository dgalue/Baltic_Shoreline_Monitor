#include "mesh/MeshService.h"
#include "concurrency/OSThread.h"
#include "TestUtil.h"
#include "freertosinc.h"
#if defined(ARCH_ESP32)
#include "esp_task_wdt.h"
#endif
#include <unity.h>

extern MeshService *service;

static void stub_task(void *param)
{
#if defined(ARCH_ESP32)
    esp_task_wdt_add(NULL);
#endif
    while (true) {
#if defined(ARCH_ESP32)
        esp_task_wdt_reset();
#endif
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void test_task_coexistence()
{
#if defined(ARCH_ESP32)
    esp_task_wdt_init(2, false);
    esp_task_wdt_add(NULL);
#endif

    service = new MeshService();
    service->init();

    xTaskCreate(stub_task, "stub1", 2048, NULL, tskIDLE_PRIORITY + 1, NULL);
    xTaskCreate(stub_task, "stub2", 2048, NULL, tskIDLE_PRIORITY + 1, NULL);

    for (int i = 0; i < 20; ++i) {
#if defined(ARCH_ESP32)
        esp_task_wdt_reset();
#endif
        vTaskDelay(pdMS_TO_TICKS(100));
    }

    TEST_ASSERT_TRUE(true);
}

void setup()
{
    delay(10);
    initializeTestEnvironment();
    UNITY_BEGIN();
    RUN_TEST(test_task_coexistence);
    exit(UNITY_END());
}

void loop() {}

