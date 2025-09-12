#include "common.h"
#include "driver/i2c.h"
#include "driver/spi_master.h"
#include "driver/uart.h"
#include "driver/adc.h"
#include "esp_err.h"

SemaphoreHandle_t mtx_i2c = nullptr;

bool hardware_init() {
  esp_err_t err;

  // Create I2C mutex
  mtx_i2c = xSemaphoreCreateMutex();
  if (!mtx_i2c) {
    return false;
  }

  // ---- I2C initialisation ----
  i2c_config_t i2c_conf = {};
  i2c_conf.mode = I2C_MODE_MASTER;
  i2c_conf.sda_io_num = I2C_SDA;
  i2c_conf.scl_io_num = I2C_SCL;
  i2c_conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
  i2c_conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
  i2c_conf.master.clk_speed = 400000;
  err = i2c_param_config(I2C_NUM_0, &i2c_conf);
  if (err != ESP_OK) return false;
  err = i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER, 0, 0, 0);
  if (err != ESP_OK) return false;

  // ---- SPI / SDMMC initialisation ----
  spi_bus_config_t buscfg = {};
  buscfg.mosi_io_num = 11; // placeholder pin numbers
  buscfg.miso_io_num = 13;
  buscfg.sclk_io_num = 12;
  buscfg.quadwp_io_num = -1;
  buscfg.quadhd_io_num = -1;
  err = spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_CH_AUTO);
  if (err != ESP_OK) return false;

  // ---- UART for GPS ----
  uart_config_t uart_cfg = {};
  uart_cfg.baud_rate = 9600;
  uart_cfg.data_bits = UART_DATA_8_BITS;
  uart_cfg.parity    = UART_PARITY_DISABLE;
  uart_cfg.stop_bits = UART_STOP_BITS_1;
  uart_cfg.flow_ctrl = UART_HW_FLOWCTRL_DISABLE;
  err = uart_param_config(UART_NUM_1, &uart_cfg);
  if (err != ESP_OK) return false;
  err = uart_set_pin(UART_NUM_1, UART_TX, UART_RX, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
  if (err != ESP_OK) return false;
  err = uart_driver_install(UART_NUM_1, 1024, 0, 0, nullptr, 0);
  if (err != ESP_OK) return false;

  // ---- ADC DMA for audio ----
  adc_digi_init_config_t adc_conf = {};
  adc_conf.max_store_buf_size = 1024;
  adc_conf.conv_num_each_intr = 1024;
  adc_conf.sample_freq_hz = 16000;
  err = adc_digi_initialize(&adc_conf);
  if (err != ESP_OK) return false;

  return true;
}

