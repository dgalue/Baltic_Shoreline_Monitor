#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void Task_SenseAudio(void *pvParameters);
void Task_GPS(void *pvParameters);
void Task_Vision(void *pvParameters);
void Task_Logger(void *pvParameters);
void Task_Uplink(void *pvParameters);

#ifdef __cplusplus
}
#endif
