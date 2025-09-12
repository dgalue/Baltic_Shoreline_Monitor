#pragma once

#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

// Pin definitions
#define I2C_SDA   5
#define I2C_SCL   6
#define UART_TX  43
#define UART_RX  44

// Event structure as defined in the guide
typedef struct __attribute__((packed)) {
  uint8_t  ver, type, conf_q7, flags;
  uint32_t ts_unix;
  int32_t  lat_e7, lon_e7;
  uint16_t feat_hash;
  uint32_t sd_offset;
} event_t;

// Audio block container for q_audio
typedef struct {
  int16_t samples[512];
} audio_block_t;

// Log blob container for q_log
typedef struct {
  size_t len;
  uint8_t data[256];
} log_blob_t;

// Shared queues
extern QueueHandle_t q_audio;
extern QueueHandle_t q_events;
extern QueueHandle_t q_log;
